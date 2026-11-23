#include "AcpSymbolApi/AcpSymbolDef.h"
#include "acp_access.h"
#include "struct_io.h"
#include "softmotion.h"

#include <Windows.h>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

extern IO_CONTROL_INTERFACE m_io;
extern SOFTMOTION_API bool smc_init();
extern SOFTMOTION_API ET_RESULT smc_sync_ioctrl_inout();
extern SOFTMOTION_API ET_RESULT smc_sync_axisctrl_inout(int axis_no = -1);
extern SOFTMOTION_API ET_RESULT smc_sync_robotctrl_inout(int robot_no = -1);

static std::mutex m_mutex;
static bool init_done = false;
std::atomic<bool> stop_flag(false);

/**
 * 打开连接.
 *
 * \param ip_addr 控制器ip地址
 * \param mode 通讯模式: 0-socket通讯 1-共享内存
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API bool smc_open(const char* ip_addr, int mode) {
	bool result = acp_open(ip_addr, mode);
	if (result) {
		result = smc_init();		
		/*strncpy_s(m_ip, sizeof(m_ip), ip_addr, strnlen_s(m_ip, 255));
		stop_flag.store(false);
		static std::thread input_sync([] {
			while (!stop_flag) {
				ET_RESULT result = smc_sync_ioctrl_inout();
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
			}
		});	*/	
	}
	init_done = result;
	return result;
}

/**
 * 初始化变量.
 *
 * \return 成功返回true, 失败返回false
 */
SOFTMOTION_API bool smc_init() {	
	bool connected = false;
	int cnt = 0;
	do {
		++cnt;
		connected = acp_conn_status() == ACP_RW_ERR_CONN_SUCCESS;
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (!connected && cnt < 10);

	if (!connected) return false;
	if (smc_sync_ioctrl_inout() != 0) return false;
	if (smc_sync_axisctrl_inout() != 0) return false;
	if (smc_sync_robotctrl_inout() != 0) return false;

	return true;
}

/**
 * 关闭连接.
 *
 * \return
 */
SOFTMOTION_API void smc_close() {
	//stop_flag.store(true);
	acp_close();
}

/**
 * 获取连接状态.
 *
 * \return 连接状态: true-连接成功 false-连接失败
 */
SOFTMOTION_API bool smc_connected_status() {
	return init_done && acp_conn_status() == ACP_RW_ERR_CONN_SUCCESS;
}

/**
 * 读PLC变量.
 * 
 * \param name 变量名
 * \param size 变量大小(字节)
 * \param content 变量内容指针
 * \param timeout 超时时间
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_var_get_value(const char* name, int size, void* content, int timeout) {
	const std::lock_guard<std::mutex> locker(m_mutex);

	PLC_TAG tag;
	tag.name = name;
	tag.size = size;
	tag.content = content;
	return acp_read_tag(tag, timeout);
}


/**
 * 写PLC变量.
 *
 * \param name 变量名
 * \param size 变量大小(字节)
 * \param content 变量内容指针
 * \param timeout 超时时间
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_var_set_value(const char* name, int size, void* content, int timeout) {
	const std::lock_guard<std::mutex> locker(m_mutex);

	PLC_TAG tag;
	tag.name = name;
	tag.size = size;
	tag.content = content;
	return acp_write_tag(tag, timeout);
}