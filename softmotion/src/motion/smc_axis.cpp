#include "acp_access.h"
#include "struct_axis.h"
#include "softmotion.h"

#include <Windows.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <stdio.h>

#define	_DEBUG_ON	1

static const int c_module_num_axis = 64;
static AXIS_MODULE_INTERFACE m_axis[c_module_num_axis];
static AXIS_MODULE_PARAM m_axis_param[c_module_num_axis];
static std::mutex m_mutex;

static const char* m_prefix = "Application.SmcExt.G_PCMotion";
extern SOFTMOTION_API ET_RESULT smc_sync_axisctrl_inout(int axis_no = -1);
extern SOFTMOTION_API ET_RESULT smc_read_axisctrl_input(int axis_no);
extern SOFTMOTION_API ET_RESULT smc_read_axisctrl_output(int axis_no);
extern SOFTMOTION_API ET_RESULT smc_write_axisctrl_input(int axis_no);

/**
 * 检查轴号是否在范围内.
 * 
 * \param axis_no 轴号
 * \param min 最小值
 * \param max 最大值
 * \return 成功返回0, 失败返回-1或者错误码
 */
ET_RESULT check_axis_no(int axis_no, int min = 0, int max = c_module_num_axis) {
	return axis_no >= min && axis_no < max ? ET_RESULT_NO_ERROR : ET_RESULT_CMD_ERR_AXIS_OUT_OF_RANGE;
}

/**
 * 轴命令预处理.
 * 
 * \param axis_no 轴号
 * \param timeout 超时(ms)
 * \return 成功返回0, 失败返回-1或者错误码
 */
ET_RESULT preprocess_axis_command(int axis_no, int timeout = 5) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = smc_read_axisctrl_input(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	bool write_input = false;
	{
		const std::lock_guard<std::mutex> locker(m_mutex);
		bool start = m_axis[axis_no].stI.xStart;
		bool estop = m_axis[axis_no].stI.xEStop;
		bool reset = m_axis[axis_no].stI.xReset;
		bool jog_f = m_axis[axis_no].stI.xJogForward;
		bool jog_b = m_axis[axis_no].stI.xJogBackward;
		if (start || estop || reset || jog_f || jog_b) {
			m_axis[axis_no].stI.xStart = false;
			m_axis[axis_no].stI.xEStop = false;
			m_axis[axis_no].stI.xReset = false;
			m_axis[axis_no].stI.xJogForward = false;
			m_axis[axis_no].stI.xJogBackward = false;
			write_input = true;
		}
	}

	if (write_input) {
		result = smc_write_axisctrl_input(axis_no);
		if (result != ET_RESULT_NO_ERROR) {
			return result;
		}
	}

	int cnt = 0;
	bool done = false, busy = false, error = false;
	uint16_t error_id;
	do {
		result = smc_axis_command_result(axis_no, &done, &busy, &error, &error_id);
		++cnt;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	} while (result == ET_RESULT_NO_ERROR && (done || busy || error) && cnt < 100);

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
 * 轴命令后处理.
 *
 * \param axis_no 轴号
 * \param timeout 超时(ms)
 * \param retry 重试次数
 * \return 成功返回0, 失败返回-1或者错误码
 */
ET_RESULT postprocess_axis_command(int axis_no, int timeout = 10, int retry = 3) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = ET_RESULT_NO_ERROR;
	int cnt = 0;
	bool done = false, busy = false, error = false;
	uint16_t error_id;
	do {
		result = smc_axis_command_result(axis_no, &done, &busy, &error, &error_id);
		++cnt;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	} while (result == ET_RESULT_NO_ERROR && !done && !error && cnt < retry);

	if (result != ET_RESULT_NO_ERROR)
		return result;
	
	if (error)
		return make_result(ET_RESULT_OFFSET_SMC_ERR, error_id);

	if(cnt >= retry)
		return ET_RESULT_CMD_ERR_TIMEOUT;
		
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.xStart = false;
	}

	result = smc_write_axisctrl_input(axis_no);

#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto end = std::chrono::steady_clock::now();
	double milliseconds = std::chrono::duration<double, std::milli>(end - start).count();

	char log[256] = "";
	snprintf(log, sizeof(log), "line(%d) -> call api(%s): time = %fms, result = %d\n", __LINE__, __func__, milliseconds, result);
	OutputDebugStringA(log);
#endif
#endif
	return done ? result : ET_RESULT_CMD_ERR_NO_EXECUTE;
}

/**
 * 同步轴结构体输入输出.
 * 
 * \param axis_no 轴号: 从0开始, -1表示读取所有轴
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_sync_axisctrl_inout(int axis_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = ET_RESULT_NO_ERROR;
	PLC_TAG axis_tag, param_tag;
	char axis_name[255], param_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	if (axis_no == -1) {
		sprintf_s(param_name, "%s.astAxisParam[0]", m_prefix);
		param_tag.name = param_name;
		param_tag.size = sizeof(m_axis_param);
		param_tag.content = m_axis_param;
		result = acp_read_tag(param_tag);

		if (result == ET_RESULT_NO_ERROR) {
			sprintf_s(axis_name, "%s.astAxisCtrl[0]", m_prefix);
			axis_tag.name = axis_name;
			axis_tag.size = sizeof(m_axis);
			axis_tag.content = m_axis;
			result = acp_read_tag(axis_tag);
		}
	} else {
		result = check_axis_no(axis_no);
		if (result == ET_RESULT_NO_ERROR) {
			sprintf_s(param_name, "%s.astAxisParam[%d]", m_prefix, axis_no);
			param_tag.name = param_name;
			param_tag.size = sizeof(m_axis_param[axis_no]);
			param_tag.content = &m_axis_param[axis_no];
			result = acp_read_tag(param_tag);			

			if (result == ET_RESULT_NO_ERROR) {
				sprintf_s(axis_name, "%s.astAxisCtrl[%d]", m_prefix, axis_no);
				axis_tag.name = axis_name;
				axis_tag.size = sizeof(m_axis[axis_no]);
				axis_tag.content = &m_axis[axis_no];
				result = acp_read_tag(axis_tag);
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
 * 读轴结构体输入.
 * 
 * \param axis_no 轴号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_read_axisctrl_input(int axis_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG axis_tag, param_tag;
	char axis_name[255], param_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(param_name, "%s.astAxisParam[%d]", m_prefix, axis_no);
	param_tag.name = param_name;
	param_tag.size = sizeof(m_axis_param[axis_no]);
	param_tag.content = &m_axis_param[axis_no];
	result = acp_read_tag(param_tag);
	
	if (result == ET_RESULT_NO_ERROR) {
		sprintf_s(axis_name, "%s.astAxisCtrl[%d].stI", m_prefix, axis_no);
		axis_tag.name = axis_name;
		axis_tag.size = sizeof(m_axis[axis_no].stI);
		axis_tag.content = &m_axis[axis_no].stI;
		result = acp_read_tag(axis_tag);
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
 * 读轴结构体输出.
 *
 * \param axis_no 轴号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_read_axisctrl_output(int axis_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG axis_tag;
	char axis_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(axis_name, "%s.astAxisCtrl[%d].stQ", m_prefix, axis_no);
	axis_tag.name = axis_name;
	axis_tag.size = sizeof(m_axis[axis_no].stQ);
	axis_tag.content = &m_axis[axis_no].stQ;
	result = acp_read_tag(axis_tag);

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
 * 写轴结构体输入.
 *
 * \param axis_no 轴号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_write_axisctrl_input(int axis_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG axis_tag, param_tag;
	char axis_name[255], param_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(param_name, "%s.astAxisParam[%d]", m_prefix, axis_no);
	param_tag.name = param_name;
	param_tag.size = sizeof(m_axis_param[axis_no]);
	param_tag.content = &m_axis_param[axis_no];
	result = acp_write_tag(param_tag);
		
	if (result == ET_RESULT_NO_ERROR) {
		sprintf_s(axis_name, "%s.astAxisCtrl[%d].stI", m_prefix, axis_no);
		axis_tag.name = axis_name;
		axis_tag.size = sizeof(m_axis[axis_no].stI);
		axis_tag.content = &m_axis[axis_no].stI;
		result = acp_write_tag(axis_tag);
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
 * 读轴通用状态.
 * 
 * \param axis_no 轴号: 从0开始
 * \param axis_id 轴Id
 * \param virt 0-实轴 1-虚轴
 * \param comm_status 0-未通讯上 1-通讯正常
 * \param power_status 0-未使能 1-已使能
 * \param ref_status 0-未回原点 1-已回原点
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_common_status(int axis_no, int16_t &axis_id, bool &virt, bool &comm_status, bool &power_status, bool &ref_status) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		axis_id = m_axis[axis_no].stQ.nAxisId;
		virt = m_axis[axis_no].stQ.xVirtual;
		comm_status = m_axis[axis_no].stQ.xCommOK;
		power_status = m_axis[axis_no].stQ.xInRun;
		ref_status = m_axis[axis_no].stQ.xRefOK;
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
 * 读轴运动状态.
 * 
 * \param axis_no 轴号: 从0开始
 * \param stopped 0-运动中 1-停止
 * \param homing 1-回原点中
 * \param sync 1-同步运动中
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_motion_status(int axis_no, SMC_AXIS_STATE &state, bool &stopped, bool &homing, bool &sync) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		state = m_axis[axis_no].stQ.etAxisState;
		stopped = m_axis[axis_no].stQ.xStopped;
		homing = m_axis[axis_no].stQ.xHoming;
		sync = m_axis[axis_no].stQ.xSynchronized;
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
 * 读轴报警.
 * 
 * \param axis_no 轴号: 从0开始
 * \param alarm 0-无报警 1-报警
 * \param alarm_id 轴报警代码
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_alarm_status(int axis_no, bool &alarm, uint32_t &alarm_id) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		alarm = m_axis[axis_no].stQ.xAxisAlarm;
		alarm_id = m_axis[axis_no].stQ.dwAxisAlarmID;
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
 * 读当前控制模式.
 * 
 * \param axis_no 轴号: 从0开始
 * \param status 当前控制模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_control_status(int axis_no, ET_AXISMODULE_CMD &status) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		status = m_axis[axis_no].stQ.etActiveMode;
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
 * 读指令执行结果.
 * 
 * \param axis_no 轴号: 从0开始
 * \param done 执行完成
 * \param busy 执行中
 * \param error 执行失败
 * \param error_id 故障代码
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_command_result(int axis_no, bool *done, bool *busy, bool *error, uint16_t *error_id) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (done != nullptr)
			*done = m_axis[axis_no].stQ.xDone;

		if (busy != nullptr)
			*busy = m_axis[axis_no].stQ.xBusy;

		if (error != nullptr)
			*error = m_axis[axis_no].stQ.xError;

		if (error_id != nullptr)
			*error_id = m_axis[axis_no].stQ.etErrorId;
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
 * 读轴当前位置.
 * 
 * \param axis_no 轴号: 从0开始
 * \param value 轴当前位置
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_actual_position(int axis_no, double &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_axis[axis_no].stQ.rActPosition;
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
 * 读轴当前速度.
 *
 * \param axis_no 轴号: 从0开始
 * \param value 轴当前速度
 * \param reload 0-从缓存读取 1-通讯读取
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_actual_velocity(int axis_no, double &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_axis[axis_no].stQ.rActVelocity;
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
 * 读轴当前扭矩.
 *
 * \param axis_no 轴号: 从0开始
 * \param value 轴当前扭矩
 * \param reload 0-从缓存读取 1-通讯读取
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_actual_torque(int axis_no, double &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_axis[axis_no].stQ.rActTorque;
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
 * 读轴结构体输出.
 * 
 * \param axis_no 轴号: 从0开始
 * \param value 轴结构体输出
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_total_output(int axis_no, AXIS_MODULE_OUTPUTS &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_axis[axis_no].stQ;
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
 * 设置轴运动参数 - 目标位置.
 * 
 * \param axis_no 轴号: 从0开始
 * \param value 目标位置(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_profile_position(int axis_no, double value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.rPosition = value;		
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置轴运动参数 - 目标速度.
 *
 * \param axis_no 轴号: 从0开始
 * \param value 目标速度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_profile_velocity(int axis_no, double value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.rVelocity = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置轴运动参数 - 加减速及S曲线斜坡.
 * 
 * \param axis_no 轴号: 从0开始
 * \param acc 加速度(用户单位)
 * \param dec 减速度(用户单位)
 * \param jerk 跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_profile_ramp(int axis_no, double acc, double dec, double jerk) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (acc > 0.001)
			m_axis[axis_no].stI.rAcceleration = acc;

		if (dec > 0.001)
			m_axis[axis_no].stI.rDeceleration = dec;

		if (jerk > 0.001)
			m_axis[axis_no].stI.rJerk = jerk;
	}	

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置轴运动缓冲模式.
 *
 * \param axis_no 轴号: 从0开始
 * \param value 缓冲模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_profile_buffermode(int axis_no, MC_BUFFER_MODE value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.etBufferMode = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 读取轴参数设定值.
 *
 * \param axis_no 轴号: 从0开始
 * \param pos 目标位置(用户单位)
 * \param vel 速度(用户单位)
 * \param acc 加速度(用户单位)
 * \param dec 减速度(用户单位)
 * \param jerk 跃度(用户单位)
 * \param buffermode 缓冲模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_profile_setting(int axis_no, double &pos, double &vel, double &acc, double &dec, double &jerk, MC_BUFFER_MODE &buffermode) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}
	
	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
			pos = m_axis[axis_no].stI.rPosition;
			vel = m_axis[axis_no].stI.rVelocity;
			acc = m_axis[axis_no].stI.rAcceleration;
			dec = m_axis[axis_no].stI.rDeceleration;
			jerk = m_axis[axis_no].stI.rJerk;
			buffermode = m_axis[axis_no].stI.etBufferMode;		
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
 * 设置轴运动参数 - 急停加减速及S曲线斜坡.
 *
 * \param axis_no 轴号: 从0开始
 * \param dec 急停减速度(用户单位)
 * \param jerk 急停跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_estop_ramp(int axis_no, double dec, double jerk) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (dec > 0.001)
			m_axis_param[axis_no].rEStopRamp = dec;

		if (jerk > 0.001)
			m_axis_param[axis_no].rEStopJerk = jerk;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置急停IO.
 *
 * \param axis_no 轴号: 从0开始
 * \param module_no 急停输入 - 模块编号
 * \param io_no 急停输入 - IO点编号
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_estop_io(int axis_no, int16_t module_no, int16_t io_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].iEStopModuleNo = module_no;
		m_axis_param[axis_no].iEStopIoNo = io_no;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 读取轴运动参数 - 急停加减速及S曲线斜坡.
 *
 * \param axis_no 轴号: 从0开始
 * \param dec 急停减速度(用户单位)
 * \param jerk 急停跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_estop_ramp(int axis_no, double &dec, double &jerk) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		dec = m_axis_param[axis_no].rEStopRamp;
		jerk = m_axis_param[axis_no].rEStopJerk;			
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
 * 读取急停IO.
 * 
 * \param axis_no 轴号: 从0开始
 * \param module_no 急停输入 - 模块编号
 * \param io_no 急停输入 - IO点编号
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_estop_io(int axis_no, int16_t &module_no, int16_t &io_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		module_no = m_axis_param[axis_no].iEStopModuleNo;
		io_no = m_axis_param[axis_no].iEStopIoNo;
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
 * 设置轴运动参数 - 点动速度.
 * 
 * \param axis_no 轴号: 从0开始
 * \param value 点动速度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_jog_velocity(int axis_no, double value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].rJogVelocity = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置轴运动参数 - 点动加减速及S曲线斜坡.
 *
 * \param axis_no 轴号: 从0开始
 * \param acc 点动加速度(用户单位)
 * \param dec 点动减速度(用户单位)
 * \param jerk 点动跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_jog_ramp(int axis_no, double acc, double dec, double jerk) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (acc > 0.001)
			m_axis_param[axis_no].rJogAcceleration = acc;

		if (dec > 0.001)
			m_axis_param[axis_no].rJogDeceleration = dec;

		if (jerk > 0.001)
			m_axis_param[axis_no].rJogJerk = jerk;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 读取轴运动参数 - 点动/寸动参数.
 * 
 * \param axis_no 轴号: 从0开始
 * \param inch_dist 寸动距离
 * \param vel 点动速度
 * \param acc 点动加速度(用户单位)
 * \param dec 点动减速度(用户单位)
 * \param jerk 点动跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_jog_param(int axis_no, double &inch_dist, double &vel, double &acc, double &dec, double &jerk) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}
	
	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		inch_dist = m_axis_param[axis_no].rInchDistance;
		vel = m_axis_param[axis_no].rJogVelocity;
		acc = m_axis_param[axis_no].rJogAcceleration;
		dec = m_axis_param[axis_no].rJogDeceleration;
		jerk = m_axis_param[axis_no].rJogJerk;
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
 * 设置回原点位置 - 驱动器回零/PLC回零.
 * 
 * \param axis_no 轴号: 从0开始
 * \param value 回原点位置(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_home_position(int axis_no, double value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].rHomePosition = value;		
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置回原点速度 - 驱动器回零/PLC回零.
 * 
 * \param axis_no 轴号: 从0开始
 * \param low 低速(用户单位)
 * \param high 高速(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_home_velocity(int axis_no, double low, double high) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (low > 0.001)
			m_axis_param[axis_no].rHomeVelocitySlow = low;

		if (high > 0.001)
			m_axis_param[axis_no].rHomeVelocityFast = high;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置回原点参数 - 加减速及S曲线斜坡.
 *
 * \param axis_no 轴号: 从0开始
 * \param acc 回原点加速度(用户单位)
 * \param dec 回原点减速度(用户单位)
 * \param jerk 回原点跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_home_ramp(int axis_no, double acc, double dec, double jerk) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (acc > 0.001)
			m_axis_param[axis_no].rHomeAcceleration = acc;

		if (dec > 0.001)
			m_axis_param[axis_no].rHomeDeceleration = dec;

		if (jerk > 0.001)
			m_axis_param[axis_no].rHomeJerk = jerk;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 读取回原点参数.
 *
 * \param axis_no 轴号: 从0开始
 * \param pos 回原点位置(用户单位)
 * \param low_vel 低速(用户单位)
 * \param high_vel 高速(用户单位)
 * \param acc 回原点加速度(用户单位)
 * \param dec 回原点减速度(用户单位)
 * \param jerk 回原点跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_home_param(int axis_no, double &pos, double &low_vel, double &high_vel, double &acc, double &dec, double &jerk) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		pos = m_axis_param[axis_no].rHomePosition;
		low_vel = m_axis_param[axis_no].rHomeVelocitySlow;
		high_vel = m_axis_param[axis_no].rHomeVelocityFast;
		acc = m_axis_param[axis_no].rHomeAcceleration;
		dec = m_axis_param[axis_no].rHomeDeceleration;
		jerk = m_axis_param[axis_no].rHomeJerk;
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
 * 设置回原点模式 - 驱动器回零.
 *
 * \param axis_no 轴号: 从0开始
 * \param value 回原点模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_drv_home_mode(int axis_no, uint16_t value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].iDrvHomeMode = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置回原点参数 - 驱动器回零.
 * 
 * \param axis_no 轴号: 从0开始
 * \param en_sdo 0-回零时不写入SDO 1-回零时通过SDO写入加减速及偏移量
 * \param en_conv 1-启用用户单位转换
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_drv_home_param(int axis_no, bool en_sdo, bool en_conv) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].xEnHomeSDOWrite = en_sdo;
		m_axis_param[axis_no].xEnHomeConvUserUnit = en_conv;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 读取驱动器回零参数.
 *
 * \param axis_no 轴号: 从0开始
 * \param mode 回原点模式
 * \param en_sdo 0-回零时不写入SDO 1-回零时通过SDO写入加减速及偏移量
 * \param en_conv 1-启用用户单位转换
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_drv_home_param(int axis_no, uint16_t &mode, bool &en_sdo, bool &en_conv) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		mode = m_axis_param[axis_no].iDrvHomeMode;
		en_sdo = m_axis_param[axis_no].xEnHomeSDOWrite;
		en_conv = m_axis_param[axis_no].xEnHomeConvUserUnit;
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
 * 读驱动器输入信号.
 *
 * \param axis_no 轴号: 从0开始
 * \param value 驱动器输入
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_drv_inputs(int axis_no, AXIS_DIGITAL_INPUTS &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_axis[axis_no].stQ.stDigitalInputs;
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
 * 设置回原点模式 - PLC回零.
 *
 * \param axis_no 轴号: 从0开始
 * \param value 回原点模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_plc_home_mode(int axis_no, SMC_HOME_MODE value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].etSmcHomeMode = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置回原点参数 - PLC回零.
 * 
 * \param axis_no 轴号: 从0开始
 * \param signal_delay 原点开关信号延时
 * \param index_position 原点索引位置
 * \param return_to_zero 回零完成后返回原点
 * \param ignore_hw_limits 回零过程中忽略限位开关
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_plc_home_param(int axis_no, double signal_delay, double index_position, bool return_to_zero, bool ignore_hw_limits) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].rSmcSignalDelay = signal_delay;
		m_axis_param[axis_no].rSmcHomeIndexPosition = index_position;
		m_axis_param[axis_no].xSmcHomeReturnToZero = return_to_zero;
		m_axis_param[axis_no].xSmcHomeIgnoreHWLimit = ignore_hw_limits;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 设置PLC回零参数 - IO.
 * 
 * \param axis_no 轴号: 从0开始
 * \param switch_module 原点开关 - 模块编号
 * \param switch_io 原点开关 - 输入点编号
 * \param index_module 原点索引 - 模块编号
 * \param index_no 原点索引 - 输入点编号
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_plc_home_io(int axis_no, int16_t switch_module, int16_t switch_io, int16_t index_module, int16_t index_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].iSmcHomeSwitchModuleNo = switch_module;
		m_axis_param[axis_no].iSmcHomeSwitchIoNo = switch_io;
		m_axis_param[axis_no].iSmcHomeIndexModuleNo = index_module;
		m_axis_param[axis_no].iSmcHomeIndexIoNo = index_no;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 读取PLC回零参数 - 其它参数.
 *
 * \param axis_no 轴号: 从0开始
 * \param mode 回原点模式
 * \param signal_delay 原点开关信号延时
 * \param index_position 原点索引位置
 * \param return_to_zero 回零完成后返回原点
 * \param ignore_hw_limits 回零过程中忽略限位开关
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_plc_home_param(int axis_no, SMC_HOME_MODE &mode, double &signal_delay, double &index_position, bool &return_to_zero, bool &ignore_hw_limits) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		mode = m_axis_param[axis_no].etSmcHomeMode;
		signal_delay = m_axis_param[axis_no].rSmcSignalDelay;
		index_position = m_axis_param[axis_no].rSmcHomeIndexPosition;
		return_to_zero = m_axis_param[axis_no].xSmcHomeReturnToZero;
		ignore_hw_limits = m_axis_param[axis_no].xSmcHomeIgnoreHWLimit;
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
 * 读取PLC回零参数 - IO.
 *
 * \param axis_no 轴号: 从0开始
 * \param switch_module 原点开关 - 模块编号
 * \param switch_io 原点开关 - 输入点编号
 * \param index_module 原点索引 - 模块编号
 * \param index_no 原点索引 - 输入点编号
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_plc_home_io(int axis_no, int16_t &switch_module, int16_t &switch_io, int16_t &index_module, int16_t &index_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		switch_module = m_axis_param[axis_no].iSmcHomeSwitchModuleNo;
		switch_io = m_axis_param[axis_no].iSmcHomeSwitchIoNo;
		index_module = m_axis_param[axis_no].iSmcHomeIndexModuleNo;
		index_no = m_axis_param[axis_no].iSmcHomeIndexIoNo;
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
 * 轴开使能.
 *
 * \param axis_no 轴号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_power_on(int axis_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.xEnable = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴关使能.
 *
 * \param axis_no 轴号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_power_off(int axis_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.xEnable = false;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴绝对定位 - 启动.
 * 
 * \param axis_no 轴号: 从0开始
 * \param position 目标位置
 * \param module_direction 运动方向（模数轴有效）
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_move_absolute(int axis_no, double position, MC_DIRECTION module_direction) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.etAbsDirection = module_direction;
		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_MOVEABS;
		m_axis[axis_no].stI.rPosition = position;
		m_axis[axis_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴相对定位 - 启动.
 * 
 * \param axis_no 轴号: 从0开始
 * \param position 目标距离
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_move_relative(int axis_no, double distance) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_MOVEREL;
		m_axis[axis_no].stI.rPosition = distance;
		m_axis[axis_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴按指定速度运行 - 启动.
 * 
 * \param axis_no 轴号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_move_velocity(int axis_no, double velocity) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_MOVEVEL;
		m_axis[axis_no].stI.rVelocity = velocity;
		m_axis[axis_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴暂停 - Halt(未考虑回零无法中止).
 *
 * \param axis_no 轴号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_halt_v0(int axis_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_HALT;
		m_axis[axis_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴暂停 - Halt 轴在回零中则调用EStop.
 * 
 * \param axis_no 轴号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_halt(int axis_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	bool homing = false;
	result = smc_read_axisctrl_output(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		homing = m_axis[axis_no].stQ.xHoming;
	}

	if (!homing) {
		result = smc_axis_halt_v0(axis_no);		
	} else {
		result = smc_axis_estop(axis_no, true);
		if (result == ET_RESULT_NO_ERROR) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			result = smc_axis_estop(axis_no, false);
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
 * 轴故障复位,解除急停后复位.
 *
 * \param axis_no 轴号: 从0开始
 * \param enable 0-释放复位 1-按下复位
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_reset(int axis_no) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.xReset = true;		
	} 

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);		
		m_axis[axis_no].stI.xReset = false;			
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴急停 - Stop.
 *
 * \param axis_no 轴号: 从0开始
 * \param enable 0-释放急停 1-按下急停
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_estop(int axis_no, bool enable) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result != ET_RESULT_NO_ERROR)
		return result;

	static int16_t module_no = -1, io_no = -1;
	if (enable) {
		if (result == ET_RESULT_NO_ERROR) {
			result = smc_axis_get_estop_io(axis_no, module_no, io_no);
		}

		if (result == ET_RESULT_NO_ERROR) {
			result = smc_axis_set_estop_io(axis_no, -1, -1);
		}

		if (result == ET_RESULT_NO_ERROR) {
			const std::lock_guard<std::mutex> locker(m_mutex);
			m_axis[axis_no].stI.xEStop = true;
		}

		if (result == ET_RESULT_NO_ERROR) {
			result = smc_write_axisctrl_input(axis_no);
		}		

	} else {
		if (result == ET_RESULT_NO_ERROR) {
			const std::lock_guard<std::mutex> locker(m_mutex);
			m_axis[axis_no].stI.xEStop = false;
		}
		
		if (result == ET_RESULT_NO_ERROR) {
			result = smc_write_axisctrl_input(axis_no);
		}

		if (result == ET_RESULT_NO_ERROR) {
			result = smc_axis_set_estop_io(axis_no, module_no, io_no);
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
 * 轴点动前进.
 * 
 * \param axis_no 轴号: 从0开始
 * \param enable 0-停止点动 1-开始点动
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_jog_forward(int axis_no, bool enable) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.xInchMode = false;
		m_axis[axis_no].stI.xJogForward = enable;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴点动后退.
 *
 * \param axis_no 轴号: 从0开始
 * \param enable 0-停止点动 1-开始点动
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_jog_backward(int axis_no, bool enable) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.xInchMode = false;
		m_axis[axis_no].stI.xJogBackward = enable;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴寸动前进.
 * 
 * \param axis_no 轴号: 从0开始 
 * \param enable 0-停止寸动 1-开始寸动
 * \param distance 寸动距离
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_inch_forward(int axis_no, bool enable, double distance) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.xInchMode = true;
		m_axis_param[axis_no].rInchDistance = distance;
		m_axis[axis_no].stI.xJogForward = enable;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴寸动后退.
 *
 * \param axis_no 轴号: 从0开始
 * \param enable 0-停止寸动 1-开始寸动
 * \param distance 寸动距离
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_inch_backward(int axis_no, bool enable, double distance) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.xInchMode = true;
		m_axis_param[axis_no].rInchDistance = distance;
		m_axis[axis_no].stI.xJogBackward = enable;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 驱动器回零.
 * 
 * \param axis_no 轴号: 从0开始
 * \param mode 驱动器回零模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_drv_home(int axis_no, uint16_t mode) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].iDrvHomeMode = mode;
		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_DRV_HOME;
		m_axis[axis_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * PLC回零.
 * 
 * \param axis_no 轴号: 从0开始
 * \param mode PLC回零模式
 * \param position 回零位置(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_plc_home(int axis_no, SMC_HOME_MODE mode) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].etSmcHomeMode = mode;
		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_SMC_HOME;
		m_axis[axis_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
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
 * 轴位置设定.
 * 
 * \param axis_no 轴号: 从0开始
 * \param position 设定位置(用户单位)
 * \param relative 0-绝对模式 1-相对模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_setposition(int axis_no, double position, bool relative) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.rPosition = position;
		m_axis[axis_no].stI.etRunMode = relative ? ET_AXISMODULE_CMD_SETPOS_REL : ET_AXISMODULE_CMD_SETPOS_ABS;
		m_axis[axis_no].stI.xStart = true;		
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = postprocess_axis_command(axis_no);
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
 * 设置轴动态限制.
 * 
 * \param axis_no 轴号: 从0开始
 * \param max_vel 最大速度(用户单位)
 * \param max_acc 最大加速度(用户单位)
 * \param max_dec 最大减速度(用户单位)
 * \param max_jerk 最大跃度(用户单位)
 * \param max_torque 最大扭矩(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_dynamic_limits(int axis_no, double max_vel, double max_acc, double max_dec, double max_jerk, double max_torque) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (max_vel > 0.001)
			m_axis_param[axis_no].rRefSWMaxVelocity = max_vel;

		if (max_acc > 0.001)
			m_axis_param[axis_no].rRefSWMaxAcceleration = max_acc;

		if (max_dec > 0.001)
			m_axis_param[axis_no].rRefSWMaxDeceleration = max_dec;

		if (max_jerk > 0.001)
			m_axis_param[axis_no].rRefSWMaxJerk = max_jerk;

		if (max_torque > 0.001)
			m_axis_param[axis_no].rRefSWMaxTorque = max_torque;

		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_SET_DYNAMIC_LIMITS;
		m_axis[axis_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = postprocess_axis_command(axis_no);
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
 * 设置轴软限位.
 * 
 * \param axis_no 轴号: 从0开始
 * \param pos_limit 正限位(用户单位)
 * \param neg_limit 负限位(用户单位)
 * \param err_dec 急停减速度(用户单位)
 * \param err_max_distance 急停减速距离(用户单位)
 * \param active 0-不激活 1-激活
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_software_limits(int axis_no, double pos_limit, double neg_limit, double err_dec, double err_max_distance, bool active) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis_param[axis_no].xRefSWLimitEnable = active;
		m_axis_param[axis_no].rRefSWLimitPositive = pos_limit;
		m_axis_param[axis_no].rRefSWLimitNegative = neg_limit;
		m_axis_param[axis_no].rRefSWLimitDeceleration = err_dec;
		m_axis_param[axis_no].rRefSWLErrorMaxDistance = err_max_distance;
		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_SET_SOFTWARE_LIMITS;
		m_axis[axis_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = postprocess_axis_command(axis_no);
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
 * 写轴参数 - 通过SDO.
 *
 * \param axis_no 轴号: 从0开始
 * \param index 索引
 * \param subindex 子索引
 * \param datalen 数据长度(字节)
 * \param value 数据内容
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_set_sdo_param(int axis_no, uint16_t index, uint8_t subindex, uint8_t datalen, double value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.iSdoRwIndex = index;
		m_axis[axis_no].stI.iSdoRwSubIndex = subindex;
		m_axis[axis_no].stI.iSdoRwDataLen = datalen;
		m_axis_param[axis_no].lrSdoSetData = value;
		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_SET_SDO_PARAMS;
		m_axis[axis_no].stI.xStart = true;		
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = postprocess_axis_command(axis_no);
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
 * 读轴参数 - 通过SDO.
 *
 * \param axis_no 轴号: 从0开始
 * \param index 索引
 * \param subindex 子索引
 * \param datalen 数据长度(字节)
 * \param value 数据内容
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_sdo_param(int axis_no, uint16_t index, uint8_t subindex, uint8_t datalen, double &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_axis_command(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_axis[axis_no].stI.iSdoRwIndex = index;
		m_axis[axis_no].stI.iSdoRwSubIndex = subindex;
		m_axis[axis_no].stI.iSdoRwDataLen = datalen;
		m_axis[axis_no].stI.etRunMode = ET_AXISMODULE_CMD_GET_SDO_PARAMS;
		m_axis[axis_no].stI.xStart = true;
	}
	
	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_axisctrl_input(axis_no);
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = postprocess_axis_command(axis_no);
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_read_axisctrl_input(axis_no);
	}
	
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_axis_param[axis_no].lrSdoGetData;
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
 * 读取轴信息.
 *
 * \param axis_no 轴号: 从0开始
 * \param value 轴信息结构体
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_axis_get_information(int axis_no, AXIS_INFORMATION &value) {
#ifdef _DEBUG
#if _DEBUG_ON == 1
	auto start = std::chrono::steady_clock::now();
#endif
#endif

	ET_RESULT result = check_axis_no(axis_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_axisctrl_input(axis_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_axis[axis_no].stQ.stInformation;
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