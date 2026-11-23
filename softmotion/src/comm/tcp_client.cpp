#include "tcp_client.h"
#include "crc.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

#define	_DEBUG_ON	0

TcpClient::TcpClient() {
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
	m_socket = INVALID_SOCKET_FD;
	m_server_port = 0;
	m_running = false;
	m_recv_callback = nullptr;	
}

TcpClient::~TcpClient() {
	disconnect();
}

/**
 * 建立TCP连接.
 * 
 * \param ip 服务器IP
 * \param port 服务器端口号
 * \return 0-连接失败 1-连接成功
 */
bool TcpClient::connect(const std::string& ip, uint16_t port) {
	disconnect();
	
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	if (m_socket == INVALID_SOCKET_FD) {
		std::cerr << "socket 创建失败" << std::endl;
		return false;
	}

	// 禁用Nagle算法
	int nagle_off = 1;
	setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&nagle_off, sizeof(nagle_off));

	// 设置端口重用
	int reuse = 1;
	setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));	

	// 设置发送缓冲区（256KB）
	int sndbuf = 256 * 1024; 
	setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&sndbuf, sizeof(sndbuf));
	
	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

	if (::connect(m_socket, (sockaddr*)&addr, sizeof(addr)) < 0) {		
		closesocket(m_socket);
		m_socket = INVALID_SOCKET_FD;
		std::cerr << "连接服务器失败: " << ip << ":" << port << std::endl;
		return false;
	}

	m_running = true;
	m_server_ip = ip;
	m_server_port = port;
	
	m_heart_thread = std::thread(&TcpClient::heartThreadFunc, this);
	m_recv_thread = std::thread(&TcpClient::recvThreadFunc, this);	

	std::cout << "连接服务器成功: " << ip << ":" << port << std::endl;
	return true;
}

/**
 * 断开TCP连接.
 * 
 */
void TcpClient::disconnect() {
	m_running = false;
	if (m_socket != INVALID_SOCKET_FD) {
		closesocket(m_socket);
		m_socket = INVALID_SOCKET_FD;
	}
	if (m_recv_thread.joinable()) {
		m_recv_thread.join();
	}
	if (m_heart_thread.joinable()) { 
		m_heart_thread.join(); 
	}

	std::cout << "连接已断开" << std::endl;
}

/**
 * 发送数据（自动分包）.
 * 
 * \param data 数据内容
 * \param len 数据长度
 * \return 0-发送失败 1-发送成功
 */
bool TcpClient::sendData(const uint8_t* data, uint32_t len) {
	if (!m_running || m_socket == INVALID_SOCKET_FD) {
		return false;
	}

	std::lock_guard<std::mutex> lock(m_send_mutex);
	uint32_t seq = 0, offset = 0;
	while (offset < len) {
		uint32_t sendLen = min(CHUNK_SIZE, len - offset);
		if (!sendPacket(data + offset, sendLen, seq++, offset == 0)) {
			return false;
		}
		offset += sendLen;
		std::cout << "分包发送成功: " << sendLen << " bytes, 序号: " << seq - 1 << std::endl;
	}
	return true;
}

/**
 * 发送单个数据包.
 * 
 * \param data 数据内容
 * \param len 数据长度
 * \param seq 分包标识
 * \param with_header 发送包头
 * \return 0-发送失败 1-发送成功
 */
bool TcpClient::sendPacket(const uint8_t* data, uint32_t len, uint32_t seq, bool with_header) {
	PacketHeader header;
	header.magic = PROTOCOL_MAGIC;
	header.seq = seq;
	header.dataLen = len;
	header.crc32 = CRC::Calculate(data, len, CRC::CRC_32());

	std::vector<uint8_t> sendBuffer(sizeof(PacketHeader) + len);
	if (with_header) {
		memcpy(sendBuffer.data(), &header, sizeof(PacketHeader));		
	}
	if (len > 0) {
		memcpy(with_header ? sendBuffer.data() + sizeof(PacketHeader) : sendBuffer.data(), data, len);
	}

	int send_size = with_header ? sizeof(PacketHeader) + len : len;
	return send_size == send(m_socket, (const char*)sendBuffer.data(), send_size, 0);
}

/**
 * 数据接收线程.
 * 
 */
void TcpClient::recvThreadFunc() {
	uint8_t tmpBuf[4096];
	while (m_running) {
		size_t recv_len = recv(m_socket, (char*)tmpBuf, sizeof(tmpBuf), 0);
		if (recv_len <= 0) {
			disconnect();
			std::cerr << "连接断开，尝试重连..." << std::endl;

			std::this_thread::sleep_for(std::chrono::seconds(RECONNECT_INTERVAL));
			connect(m_server_ip, m_server_port);
			return;
		}

		// 追加到接收缓存
		m_recv_buffer.insert(m_recv_buffer.end(), tmpBuf, tmpBuf + recv_len);

		// 解析数据包
		while (m_recv_buffer.size() >= sizeof(PacketHeader)) {
			PacketHeader header;
			memcpy(&header, m_recv_buffer.data(), sizeof(PacketHeader));

			// 校验包头 - 魔数
			if (header.magic != PROTOCOL_MAGIC) {
				m_recv_buffer.clear();
				std::cerr << "协议魔数错误" << std::endl;
				return;
			}

			// 校验包头 - 数据包长度
			if (header.dataLen > MAX_PACKET_SIZE) {				
				m_recv_buffer.clear();
				std::cerr << "数据包长度超限" << std::endl;
				return;
			}

			// 拼接数据
			uint32_t total_packet_len = sizeof(PacketHeader) + header.dataLen;
			if (m_recv_buffer.size() < total_packet_len) {
				break;
			}

			// 提取数据
			std::vector<uint8_t> packetData(m_recv_buffer.data() + sizeof(PacketHeader), m_recv_buffer.data() + total_packet_len);

			// 校验CRC
			uint32_t crc = CRC::Calculate(packetData.data(), packetData.size(), CRC::CRC_32());				
			if (crc != header.crc32) {				
				m_recv_buffer.erase(m_recv_buffer.begin(), m_recv_buffer.begin() + total_packet_len);
				std::cerr << "CRC校验失败，丢弃数据包" << std::endl;
				continue;
			}

			// 处理有效数据
			procPacket(packetData.data(), packetData.size());

			// 移除已处理数据
			m_recv_buffer.erase(m_recv_buffer.begin(), m_recv_buffer.begin() + total_packet_len);
		}
	}
}

/**
 * 发送心跳线程.
 * 
 */
void TcpClient::heartThreadFunc() {
	while (m_running) {
		std::this_thread::sleep_for(std::chrono::seconds(HEARTBEAT_INTERVAL));
		sendData(nullptr, 0);
	}
}

// 处理接收的数据
void TcpClient::procPacket(const uint8_t* data, uint32_t len) {
	if (len == 0) {
		return;
	}

	if (m_recv_callback) {
		m_recv_callback(data, len);
	}
}

// 设置接收回调
void TcpClient::setReceiveCallback(void(*cb)(const uint8_t*, uint32_t)) {
	m_recv_callback = cb;
}