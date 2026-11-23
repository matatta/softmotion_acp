#include "handler.h"

#include <thread>
#include <chrono>
#include <mutex>
#include <stdio.h>
#include "open62541.h"

#ifdef _WIN32
#include <windows.h>
#define UA_sleep_ms(X) Sleep(X)
#define THREAD_RETURN DWORD WINAPI
#else
#include <pthread.h>
#include <unistd.h>
#define UA_sleep_ms(X) usleep(X * 1000)
#define THREAD_RETURN void*
#endif

#ifdef _WIN32
static HANDLE clientThread = NULL;
#else
static pthread_t clientThread;
#endif

#define	_DEBUG_ON	1
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

static std::mutex m_mutex;
static const char* m_endpoint = "opc.tcp://192.168.3.1:4840";
static const char* m_prefix = u8"|var|Sinsegye x86_64-Linux-SM-CNC.Application.GVL";

static UA_Boolean m_running = true;
static UA_Boolean m_connected = false;
static UA_Client *m_client = nullptr;

static const int c_module_num_silo = 12;
static ET_SILO_CMD m_silo_command[c_module_num_silo];

/* 监控线程函数 - 定期检查连接状态并尝试重连 */
#ifdef _WIN32
THREAD_RETURN clientThreadFunc(LPVOID param) {
#else
THREAD_RETURN clientThreadFunc(void *param) {
#endif
	UA_Client *client = (UA_Client*)param;
	while (m_running) {
		UA_StatusCode retval = UA_Client_connect(client, m_endpoint);
		m_connected = retval == UA_STATUSCODE_GOOD;
		if (retval != UA_STATUSCODE_GOOD) {
			UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "连接失败，3秒后重试...");
			UA_sleep_ms(3000);
			continue;
		}

		/* 执行客户端的周期性处理，传入超时时间（毫秒） */
		UA_Client_run_iterate(client, 1000);		
	}

	return 0;
}

/* 启动客户端线程 */
static int startClientThread(UA_Client *client) {
#ifdef _WIN32
	clientThread = CreateThread(NULL, 0, clientThreadFunc, client, 0, NULL);
	return (clientThread != NULL) ? 0 : -1;
#else
	return pthread_create(&clientThread, NULL, clientThreadFunc, client);
#endif
}

/* 停止客户端线程 */
static void stopClientThread(void) {
	m_running = false;
#ifdef _WIN32
	if (clientThread != NULL) {
		WaitForSingleObject(clientThread, 5000);
		CloseHandle(clientThread);
		clientThread = NULL;
	}
#else
	if (clientThread) {
		pthread_join(clientThread, NULL);
	}
#endif
}

/* 无活动/断线时的回调函数原型 */
static void cbOpcUaInactivity(UA_Client *client) {
	UA_SecureChannelState channelState;
	UA_SessionState sessionState;
	UA_StatusCode connectStatus;

	UA_Client_getState(client, &channelState, &sessionState, &connectStatus);
	if (connectStatus != UA_STATUSCODE_GOOD) {
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
			"Attempting to reconnect...");

		/*UA_Client_disconnect(client);
		UA_StatusCode retval = UA_Client_connect(client, m_endpoint);
		if (retval == UA_STATUSCODE_GOOD) {
			m_connected = true;
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"Reconnection successful.");
		} else {
			m_connected = false;
			UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"Reconnection failed (code: %s). Retrying in 5 seconds...",
				UA_StatusCode_name(retval));
		}*/
	}

	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
		"Server inactivity detected, forcing disconnect to trigger reconnect...");
}

/**
 * 建立OPC UA连接.
 *
 * \param endpoint 服务器地址
 * \return 0-失败 1-成功
 */
bool opcua_connect(const char* endpoint) {
	if (m_client == nullptr) {
		m_client = UA_Client_new();
		UA_ClientConfig *config = UA_Client_getConfig(m_client);
		UA_ClientConfig_setDefault(config);

		config->timeout = 5000;
		config->inactivityCallback = cbOpcUaInactivity;
		config->connectivityCheckInterval = 1000;
	}

	/* 建立连接 */
	UA_StatusCode retval = UA_Client_connect(m_client, endpoint);
	if (retval != UA_STATUSCODE_GOOD) {
		m_connected = false;
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
			"Initial connection failed (code: %s). Will retry in background.",
			UA_StatusCode_name(retval));		
	} else {
		m_connected = true;
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
			"Initial connection successful.");
	}

	return retval == UA_STATUSCODE_GOOD;
}

/**
 * 断开OPC UA连接.
 *
 */
void opcua_disconnect() {
	if (m_client != nullptr) {
		UA_Client_delete(m_client);
		m_client = nullptr;
	}
}

/**
 * 检查料仓编号是否在范围内.
 *
 * \param no 料仓编号
 * \param min 最小值
 * \param max 最大值
 * \return 成功返回0, 失败返回-1或者错误码
 */
bool check_silo_no(int no, int min = 1, int max = c_module_num_silo) {
	return no >= min && no <= max;
}

/**
 * 发送料仓命令.
 * 
 * \param no 料仓编号
 * \param cmd 料仓命令
 * \return 成功返回0, 失败返回-1或者错误码
 */
HANDLER_API bool hdl_set_silo_command(int no, ET_SILO_CMD cmd) {
	if (!check_silo_no(no)) {
		return false;
	}

	const std::lock_guard<std::mutex> locker(m_mutex);
	char silo_state[512] = "";
	snprintf(silo_state, sizeof(silo_state), u8"%s.G_料仓命令[%d]", m_prefix, no);

	if (!opcua_connect(m_endpoint)) {
		return false;
	}

	//UA_Int16 command = cmd;
	UA_Variant value;
	UA_Variant_init(&value);
	UA_Variant_setScalarCopy(&value, &cmd, &UA_TYPES[UA_TYPES_INT16]);

	UA_NodeId nodeId = UA_NODEID_STRING(4, silo_state);
	UA_StatusCode retval = UA_Client_writeValueAttribute(m_client, nodeId, &value);
	UA_Variant_clear(&value);

	return retval == UA_STATUSCODE_GOOD;
}

/**
 * 读取料仓命令执行结果.
 *
 * \param no 料仓编号
 * \param result 执行结果
 * \return 成功返回0, 失败返回-1或者错误码
 */
HANDLER_API bool hdl_get_silo_result(int no, ET_SILO_RESULT &result) {
	if (!check_silo_no(no)) {
		return false;
	}

	const std::lock_guard<std::mutex> locker(m_mutex);
	char silo_state[512] = "";
	snprintf(silo_state, sizeof(silo_state), u8"%s.G_料仓结果[%d]", m_prefix, no);

	if (!opcua_connect(m_endpoint)) {
		return false;
	}
	
	UA_Variant value;
	UA_Variant_init(&value);

	UA_NodeId nodeId = UA_NODEID_STRING(4, silo_state);
	UA_StatusCode retval = UA_Client_readValueAttribute(m_client, nodeId, &value);
	if (retval == UA_STATUSCODE_GOOD) {
		UA_Int16 *dat = (UA_Int16 *)value.data;
		result = (ET_SILO_RESULT)*dat;		
	}
	UA_Variant_clear(&value);

	return retval == UA_STATUSCODE_GOOD;
}

/**
 * 读取料仓状态.
 *
 * \param no 料仓编号
 * \param state 料仓状态
 * \return 成功返回0, 失败返回-1或者错误码
 */
HANDLER_API bool hdl_get_silo_state(int no, ET_SILO_STATE &state) {
	if (!check_silo_no(no)) {
		return false;
	}

	const std::lock_guard<std::mutex> locker(m_mutex);
	char silo_state[512] = "";
	snprintf(silo_state, sizeof(silo_state), u8"%s.G_料仓状态[%d]", m_prefix, no);

	if (!opcua_connect(m_endpoint)) {
		return false;
	}

	UA_Variant value;
	UA_Variant_init(&value);

	UA_NodeId nodeId = UA_NODEID_STRING(4, silo_state);
	UA_StatusCode retval = UA_Client_readValueAttribute(m_client, nodeId, &value);
	if (retval == UA_STATUSCODE_GOOD) {
		UA_Int16 *dat = (UA_Int16 *)value.data;
		state = (ET_SILO_STATE)*dat;
	}
	UA_Variant_clear(&value);

	return retval == UA_STATUSCODE_GOOD;
}

/**
 * 设置料仓参数.
 * 
 * \param no 料仓编号
 * \param max_dist 料仓最大行程
 * \param thickness 料仓盘片厚度
 * \param vel_h 料仓高速
 * \param vel_l 料仓低速
 * \return 成功返回0, 失败返回-1或者错误码
 */
HANDLER_API bool hdl_set_silo_param(int no, double max_dist, double thickness, double vel_h, double vel_l) {
	return false;
}

/**
 * 读取料仓参数.
 * 
 * \param no 料仓编号
 * \param max_dist 料仓最大行程
 * \param thickness 料仓盘片厚度
 * \param vel_h 料仓高速
 * \param vel_l 料仓低速
 * \return 成功返回0, 失败返回-1或者错误码
 */
HANDLER_API bool hdl_get_silo_param(int no, double &max_dist, double &thickness, double &vel_h, double &vel_l) {
	if (!check_silo_no(no)) {
		return false;
	}

	const std::lock_guard<std::mutex> locker(m_mutex);
	char silo_max_dist[512] = "";
	char silo_thickness[512] = "";
	char silo_vel_h[512] = "";
	char silo_vel_l[512] = "";
	snprintf(silo_max_dist, sizeof(silo_max_dist), u8"%s.G_料仓最大行程[%d]", m_prefix, no);
	snprintf(silo_thickness, sizeof(silo_thickness), u8"%s.G_料仓盘片厚度[%d]", m_prefix, no);
	snprintf(silo_vel_h, sizeof(silo_vel_h), u8"%s.G_料仓高速[%d]", m_prefix, no);
	snprintf(silo_vel_l, sizeof(silo_vel_l), u8"%s.G_料仓低速[%d]", m_prefix, no);

	if (!opcua_connect(m_endpoint)) {
		return false;
	}

	//UA_Variant ua_max_dist;
	//UA_Variant_init(&ua_max_dist);
	//UA_NodeId nodeId = UA_NODEID_STRING(4, silo_max_dist);
	//UA_StatusCode retval = UA_Client_readValueAttribute(m_client, nodeId, &ua_max_dist);
	//if (retval == UA_STATUSCODE_GOOD) {
	//	UA_Double *dat = (UA_Double *)ua_max_dist.data;
	//	max_dist = *dat;
	//}
	//UA_Variant_clear(&ua_max_dist);

	//UA_Variant ua_thickness;
	//UA_Variant_init(&ua_thickness);
	//UA_NodeId nodeId = UA_NODEID_STRING(4, silo_thickness);
	//UA_StatusCode retval = UA_Client_readValueAttribute(m_client, nodeId, &ua_thickness);
	//if (retval == UA_STATUSCODE_GOOD) {
	//	UA_Double *dat = (UA_Double *)ua_thickness.data;
	//	thickness = *dat;
	//}
	//UA_Variant_clear(&ua_thickness);


	//return retval == UA_STATUSCODE_GOOD;
	return false;
}
