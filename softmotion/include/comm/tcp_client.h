#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdint>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

typedef SOCKET SocketFd;
#define INVALID_SOCKET_FD INVALID_SOCKET
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
typedef int SocketFd;
#define INVALID_SOCKET_FD -1
#define closesocket close
#endif

// 协议配置
constexpr uint16_t PROTOCOL_MAGIC = 0x5AA5;	// 协议魔数
constexpr uint32_t MAX_PACKET_SIZE = 128 * 1024;// 最大单包（128KB）
constexpr uint32_t CHUNK_SIZE = 1460 * 10;		// 分包大小（14.26KB）
constexpr int HEARTBEAT_INTERVAL = 5;			// 心跳间隔5秒
constexpr int RECONNECT_INTERVAL = 3;			// 重连间隔3秒

// 协议包头（固定16字节）
struct PacketHeader {
	uint16_t magic;        // 魔数：校验协议
	uint16_t seq;          // 序列号：分包标识
	uint32_t dataLen;      // 数据长度
	uint32_t crc32;        // 数据CRC32校验值
};

// TCP客户端（收发+心跳+分包组包）
class TcpClient {
public:
	TcpClient();
	~TcpClient();

	bool connect(const std::string& ip, uint16_t port);
	void disconnect();
	bool sendData(const uint8_t* data, uint32_t len);	
	void setReceiveCallback(void(*cb)(const uint8_t*, uint32_t));

private:
	void recvThreadFunc();
	void heartThreadFunc();
	bool sendPacket(const uint8_t* data, uint32_t len, uint32_t seq, bool with_header);
	void procPacket(const uint8_t* data, uint32_t len);

private:
	SocketFd m_socket;
	std::string m_server_ip;
	uint16_t m_server_port;

	std::atomic<bool> m_running;
	std::mutex m_send_mutex;
	std::thread m_recv_thread;
	std::thread m_heart_thread;
	std::vector<uint8_t> m_recv_buffer;
	void(*m_recv_callback)(const uint8_t*, uint32_t);
};