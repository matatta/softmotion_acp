#include "acp_access.h"
#include "struct_io.h"
#include "softmotion.h"

#include <Windows.h>
#include <mutex>

#define	_DEBUG_ON	0

static IO_CONTROL_INTERFACE m_io;
static IO_MODULE_PARAM m_io_param;
static std::mutex m_mutex;

static const char* m_prefix = "Application.SmcExt.G_PCMotion";
extern SOFTMOTION_API ET_RESULT smc_sync_ioctrl_inout();

enum IO_MODULE_TYPE {
	DI_1BIT = 1,
	AI_16BIT = 2,
	AI_32BIT = 3,
	DQ_1BIT = 4,
	AQ_16BIT = 5,
	AQ_32BIT = 6,
};

/**
 * 检查IO编号是否在范围内.
 *
 * \param module_type 模块类型
 * \param module_no 模块编号
 * \param io_no IO编号
 * \return 成功返回0, 失败返回-1或者错误码
 */
ET_RESULT check_io_no(IO_MODULE_TYPE module_type, uint16_t module_no, uint16_t io_no) {
	bool in_range = false;
	switch (module_type) {
	case DI_1BIT:
		in_range = module_no >= 0 && module_no < c_module_num_di && io_no >= 0 && io_no < 32;
		break;
	case AI_16BIT:
		in_range = module_no >= 0 && module_no < c_module_num_ai_16bit && io_no >= 0 && io_no < 8;
		break;
	case AI_32BIT:
		in_range = module_no >= 0 && module_no < c_module_num_ai_32bit && io_no >= 0 && io_no < 8;
		break;
	case DQ_1BIT:
		in_range = module_no >= 0 && module_no < c_module_num_dq && io_no >= 0 && io_no < 32;
		break;
	case AQ_16BIT:
		in_range = module_no >= 0 && module_no < c_module_num_aq_16bit && io_no >= 0 && io_no < 8;
		break;
	case AQ_32BIT:
		in_range = module_no >= 0 && module_no < c_module_num_aq_32bit && io_no >= 0 && io_no < 8;
		break;
	default:
		break;
	}
	return in_range ? ET_RESULT_NO_ERROR : ET_RESULT_CMD_ERR_IO_OUT_OF_RANGE;
}

/**
 * 同步IO结构体输入输出.
 *
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_sync_ioctrl_inout() {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	const std::lock_guard<std::mutex> locker(m_mutex);
	char name[255];
	sprintf_s(name, "%s.stIoCtrl", m_prefix);	

	PLC_TAG io_tag;
	io_tag.name = name;
	io_tag.size = sizeof(m_io);
	io_tag.content = &m_io;

	ET_RESULT result = ET_RESULT_NO_ERROR;
	result = acp_read_tag(io_tag);
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 读IO模块当前值.
 * 
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_get_total_inout(IO_CONTROL_INTERFACE &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	const std::lock_guard<std::mutex> locker(m_mutex);
	char name[255];
	sprintf_s(name, "%s.stIoCtrl", m_prefix);

	PLC_TAG io_tag;
	io_tag.name = name;
	io_tag.size = sizeof(m_io);
	io_tag.content = &m_io;

	ET_RESULT result = ET_RESULT_NO_ERROR;
	result = acp_read_tag(io_tag);
	if (result == ET_RESULT_NO_ERROR)
		value = m_io;

#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 读数字量输入 - 按位访问.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_get_di(uint16_t module_no, uint16_t io_no, bool &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(DI_1BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.DI_1BIT[%d,%d]", m_prefix, module_no, io_no);
		
		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.DI_1BIT[module_no][io_no]);
		io_tag.content = &m_io.DI_1BIT[module_no][io_no];

		result = acp_read_tag(io_tag);
		if (result == ET_RESULT_NO_ERROR)
			value = m_io.DI_1BIT[module_no][io_no];
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 读数字量输入 - 按模块访问.
 *
 * \param module_no 模块编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_get_di_32bit(uint16_t module_no, uint32_t &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(DI_1BIT, module_no, 0);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.DI_1BIT", m_prefix);
		
		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.DI_1BIT);
		io_tag.content = &m_io.DI_1BIT;

		result = acp_read_tag(io_tag);
		if (result == ET_RESULT_NO_ERROR) {
			value = 0x00;
			for (int i = 31; i >= 0; i--) {
				value = value << 1;
				value = value | (m_io.DI_1BIT[module_no][i] ? 0x01 : 0x00);
			}
		}
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 读模拟量输入 - 16bit.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_get_ai_16bit(uint16_t module_no, uint16_t io_no, int16_t *value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(AI_16BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.AI_16BIT[%d,%d]", m_prefix, module_no, io_no);
		
		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.AI_16BIT[module_no][io_no]);
		io_tag.content = &m_io.AI_16BIT[module_no][io_no];

		result = acp_read_tag(io_tag);
		if (result == ET_RESULT_NO_ERROR && value != nullptr)
			*value = m_io.AI_16BIT[module_no][io_no];
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 读模拟量输入 - 32bit.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_get_ai_32bit(uint16_t module_no, uint16_t io_no, int32_t *value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(AI_32BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.AI_32BIT[%d,%d]", m_prefix, module_no, io_no);
		
		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.AI_32BIT[module_no][io_no]);
		io_tag.content = &m_io.AI_32BIT[module_no][io_no];

		result = acp_read_tag(io_tag);
		if (result == ET_RESULT_NO_ERROR && value != nullptr)
			*value = m_io.AI_32BIT[module_no][io_no];
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 写数字量输出 - 按位访问.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_set_dq(uint16_t module_no, uint16_t io_no, bool value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(DQ_1BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.DQ_1BIT[%d,%d]", m_prefix, module_no, io_no);
		
		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.DQ_1BIT[module_no][io_no]);
		io_tag.content = &m_io.DQ_1BIT[module_no][io_no];

		m_io.DQ_1BIT[module_no][io_no] = value;
		result = acp_write_tag(io_tag);
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 写数字量输出延时反转 - 按位访问.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param time_ms 延时时间(ms)
 * \param num 翻转次数: (-1)-无限翻转 0-关闭
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_set_dq_reverse(uint16_t module_no, uint16_t io_no, uint32_t time_ms, int16_t num) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(DQ_1BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char param_name[255];
		sprintf_s(param_name, "%s.stIoParam.astParam_DQ[%d,%d]", m_prefix, module_no, io_no);

		PLC_TAG param_tag;
		param_tag.name = param_name;
		param_tag.size = sizeof(m_io_param.astParam_DQ[module_no][io_no]);
		param_tag.content = &m_io_param.astParam_DQ[module_no][io_no];

		m_io_param.astParam_DQ[module_no][io_no].xEnDqReverse = true;
		m_io_param.astParam_DQ[module_no][io_no].iDqReverseTime = time_ms;
		m_io_param.astParam_DQ[module_no][io_no].iDqReverseNum = num;
		result = acp_write_tag(param_tag);
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 写数字量输出 - 按模块访问.
 *
 * \param module_no 模块编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_set_dq_32bit(uint16_t module_no, uint32_t value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(DQ_1BIT, module_no, 0);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.DQ_1BIT", m_prefix);		

		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.DQ_1BIT);
		io_tag.content = &m_io.DQ_1BIT;

		result = acp_read_tag(io_tag);
		if (result == ET_RESULT_NO_ERROR) {
			for (int i = 0; i < 32; i++) {
				m_io.DQ_1BIT[module_no][i] = (value >> i) & 0x01;
			}
			result = acp_write_tag(io_tag);
		}
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 写模拟量输出 - 16bit.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_set_aq_16bit(uint16_t module_no, uint16_t io_no, int16_t value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(AQ_16BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.AQ_16BIT[%d,%d]", m_prefix, module_no, io_no);
		
		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.AQ_16BIT[module_no][io_no]);
		io_tag.content = &m_io.AQ_16BIT[module_no][io_no];

		m_io.AQ_16BIT[module_no][io_no] = value;
		result = acp_write_tag(io_tag);
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 写模拟量输出 - 32bit.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_set_aq_32bit(uint16_t module_no, uint16_t io_no, int32_t value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(AQ_32BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.AQ_32BIT[%d,%d]", m_prefix, module_no, io_no);		

		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.AQ_32BIT[module_no][io_no]);
		io_tag.content = &m_io.AQ_32BIT[module_no][io_no];

		m_io.AQ_32BIT[module_no][io_no] = value;
		result = acp_write_tag(io_tag);
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 读数字量输出 - 按位访问.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_get_dq(uint16_t module_no, uint16_t io_no, bool &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(DQ_1BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.DQ_1BIT[%d,%d]", m_prefix, module_no, io_no);		

		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.DQ_1BIT[module_no][io_no]);
		io_tag.content = &m_io.DQ_1BIT[module_no][io_no];

		result = acp_read_tag(io_tag);
		if (result == ET_RESULT_NO_ERROR)
			value = m_io.DQ_1BIT[module_no][io_no];
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 读数字量输出 - 按模块访问.
 *
 * \param module_no 模块编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_get_dq_32bit(int module_no, uint32_t &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(DQ_1BIT, module_no, 0);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.DQ_1BIT", m_prefix);		

		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.DQ_1BIT);
		io_tag.content = &m_io.DQ_1BIT;

		result = acp_read_tag(io_tag);
		if (result == ET_RESULT_NO_ERROR) {
			value = 0x00;
			for (int i = 31; i >= 0; i--) {
				value = value << 1;
				value = value | (m_io.DQ_1BIT[module_no][i] ? 0x01 : 0x00);
			}
		}
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 读模拟量输出 - 16bit.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_get_aq_16bit(uint16_t module_no, uint16_t io_no, int16_t *value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(AQ_16BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.AQ_16BIT[%d,%d]", m_prefix, module_no, io_no);		

		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.AQ_16BIT[module_no][io_no]);
		io_tag.content = &m_io.AQ_16BIT[module_no][io_no];

		result = acp_read_tag(io_tag);
		if (result == ET_RESULT_NO_ERROR && value != nullptr)
			*value = m_io.AQ_16BIT[module_no][io_no];
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}

/**
 * 读模拟量输出 - 32bit.
 *
 * \param module_no 模块编号
 * \param io_no 模块内IO编号
 * \param value 数值
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_io_get_aq_32bit(uint16_t module_no, uint16_t io_no, int32_t *value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif
	ET_RESULT result = check_io_no(AQ_32BIT, module_no, io_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		char name[255];
		sprintf_s(name, "%s.stIoCtrl.AQ_32BIT[%d,%d]", m_prefix, module_no, io_no);		

		PLC_TAG io_tag;
		io_tag.name = name;
		io_tag.size = sizeof(m_io.AQ_32BIT[module_no][io_no]);
		io_tag.content = &m_io.AQ_32BIT[module_no][io_no];

		result = acp_read_tag(io_tag);
		if (result == ET_RESULT_NO_ERROR && value != nullptr)
			*value = m_io.AQ_32BIT[module_no][io_no];
	}
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return result;
}