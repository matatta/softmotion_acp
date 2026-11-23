#include "acp_access.h"
#include "softmotion.h"

#include <Windows.h>
#include <thread>
#include <chrono>
#include <mutex>

/* XYZ插补 - 未使用
	SOFTMOTION_API ET_RESULT smc_intp_xyz_command_result(int intp_no, bool *power = nullptr, bool *ready = nullptr, bool *done = nullptr, bool *busy = nullptr, bool *error = nullptr);
	SOFTMOTION_API ET_RESULT smc_intp_xyz_set_profile_param(int intp_no, double vel, double acc, double dec, double jerk);
	SOFTMOTION_API ET_RESULT smc_intp_xyz_get_profile_param(int intp_no, MC_COORD_REF &pos, SMC_VECTOR3D &offset, double &vel, double &acc, double &dec, double &jerk);

	SOFTMOTION_API ET_RESULT smc_intp_xyz_power_on(int intp_no);
	SOFTMOTION_API ET_RESULT smc_intp_xyz_power_off(int intp_no);
	SOFTMOTION_API ET_RESULT smc_intp_xyz_move_absolute(int intp_no, double pos_x, double pos_y, double pos_z, double pos_c, double tool_x, double tool_y);
	SOFTMOTION_API ET_RESULT smc_intp_xyz_move_relative(int intp_no, double pos_x, double pos_y, double pos_z, double pos_c, double tool_x, double tool_y);
	SOFTMOTION_API ET_RESULT smc_intp_xyz_halt(int intp_no);
	SOFTMOTION_API ET_RESULT smc_intp_xyz_reset(int intp_no);
*/

static const int c_intp_num = 1;
static INTP_XYZ_INTERFACE m_intp[c_intp_num];
static INTP_XYZ_PARAM m_intp_param[c_intp_num];
static std::mutex m_mutex;

static const char* m_prefix = "Application.SmcExt.G_PCMotion";
extern SOFTMOTION_API ET_RESULT smc_intp_xyz_command_result(int intp_no, bool *power = nullptr, bool *ready = nullptr, bool *done = nullptr, bool *busy = nullptr, bool *error = nullptr);
extern SOFTMOTION_API ET_RESULT smc_sync_intp_xyz_inout(int intp_no = -1);
extern SOFTMOTION_API ET_RESULT smc_read_intp_xyz_input(int intp_no);
extern SOFTMOTION_API ET_RESULT smc_read_intp_xyz_output(int intp_no);
extern SOFTMOTION_API ET_RESULT smc_write_intp_xyz_input(int intp_no);

/**
 * 检查XYZ通道号是否在范围内.
 *
 * \param intp_no 通道号
 * \param min 最小值
 * \param max 最大值
 * \return 成功返回0, 失败返回-1或者错误码
 */
ET_RESULT check_intp_xyz_no(int intp_no, int min = 0, int max = c_intp_num) {
	return intp_no >= min && intp_no < max ? ET_RESULT_NO_ERROR : ET_RESULT_CMD_ERR_AXIS_OUT_OF_RANGE;
}

/**
 * XYZ命令预处理.
 *
 * \param intp_no 通道号
 * \param timeout 超时(ms)
 * \return 成功返回0, 失败返回-1或者错误码
 */
ET_RESULT preprocess_intp_xyz_command(int intp_no, int timeout = 5) {
	ET_RESULT result = smc_read_intp_xyz_input(intp_no);
	bool write_input = false;
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		bool start = m_intp[intp_no].stI.xStart;
		bool reset = m_intp[intp_no].stI.xReset;
		if (start || reset) {
			m_intp[intp_no].stI.xStart = false;
			m_intp[intp_no].stI.xReset = false;
			write_input = true;
		}
	}

	if (write_input) {
		result = smc_write_intp_xyz_input(intp_no);
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	}

	return result;
}

/**
 * XYZ命令后处理.
 *
 * \param intp_no 通道号
 * \param timeout 超时(ms)
 * \param retry 重试次数
 * \return 成功返回0, 失败返回-1或者错误码
 */
ET_RESULT postprocess_intp_xyz_command(int intp_no, int timeout = 10, int retry = 3) {
	ET_RESULT result = ET_RESULT_NO_ERROR;
	int cnt = 0;
	bool ready = false, done = false, busy = false, error = false;
	do {
		result = smc_intp_xyz_command_result(intp_no, &ready, &done, &busy, &error);
		++cnt;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	} while (result == ET_RESULT_NO_ERROR && !done && !error && cnt < retry);

	if (result != ET_RESULT_NO_ERROR)
		return result;

	if (error)
		return ET_RESULT_CMD_ERR_INTP_XYZ_EXECUTE;

	if (cnt >= retry)
		return ET_RESULT_CMD_ERR_TIMEOUT;

	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_intp[intp_no].stI.xStart = false;
	}

	result = smc_write_intp_xyz_input(intp_no);
	return done ? result : ET_RESULT_CMD_ERR_NO_EXECUTE;
}

/**
 * XYZ结构体输入输出.
 *
 * \param intp_no 通道号: 从0开始, -1表示读取所有轴
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_sync_intp_xyz_inout(int intp_no) {
	ET_RESULT result = ET_RESULT_NO_ERROR;
	PLC_TAG intp_tag, param_tag;
	char intp_name[255], param_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	if (intp_no == -1) {
		sprintf_s(param_name, "%s.astIntpXYZParam[0]", m_prefix);
		param_tag.name = param_name;
		param_tag.size = sizeof(m_intp_param);
		param_tag.content = m_intp_param;
		result = acp_read_tag(param_tag);

		if (result == ET_RESULT_NO_ERROR) {
			sprintf_s(intp_name, "%s.astIntpXYZCtrl[0]", m_prefix);
			intp_tag.name = intp_name;
			intp_tag.size = sizeof(m_intp);
			intp_tag.content = m_intp;
			result = acp_read_tag(intp_tag);
		}
	} else {
		result = check_intp_xyz_no(intp_no);
		if (result == ET_RESULT_NO_ERROR) {
			sprintf_s(param_name, "%s.astIntpXYZParam[%d]", m_prefix, intp_no);
			param_tag.name = param_name;
			param_tag.size = sizeof(m_intp_param[intp_no]);
			param_tag.content = &m_intp_param[intp_no];
			result = acp_read_tag(param_tag);			

			if (result == ET_RESULT_NO_ERROR) {
				sprintf_s(intp_name, "%s.astIntpXYZCtrl[%d]", m_prefix, intp_no);
				intp_tag.name = intp_name;
				intp_tag.size = sizeof(m_intp[intp_no]);
				intp_tag.content = &m_intp[intp_no];
				result = acp_read_tag(intp_tag);
			}
		}
	}

	return result;
}

/**
 * 读XYZ结构体输入.
 *
 * \param intp_no 通道号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_read_intp_xyz_input(int intp_no) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG intp_tag, param_tag;
	char intp_name[255], param_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(param_name, "%s.astIntpXYZParam[%d]", m_prefix, intp_no);
	param_tag.name = param_name;
	param_tag.size = sizeof(m_intp_param[intp_no]);
	param_tag.content = &m_intp_param[intp_no];
	result = acp_read_tag(param_tag);
	
	if (result == ET_RESULT_NO_ERROR) {
		sprintf_s(intp_name, "%s.astIntpXYZCtrl[%d].stI", m_prefix, intp_no);
		intp_tag.name = intp_name;
		intp_tag.size = sizeof(m_intp[intp_no].stI);
		intp_tag.content = &m_intp[intp_no].stI;
		result = acp_read_tag(intp_tag);
	}

	return result;
}

/**
 * 读XYZ结构体输出.
 *
 * \param intp_no 通道号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_read_intp_xyz_output(int intp_no) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG intp_tag;
	char intp_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(intp_name, "%s.astIntpXYZCtrl[%d].stQ", m_prefix, intp_no);
	intp_tag.name = intp_name;
	intp_tag.size = sizeof(m_intp[intp_no].stQ);
	intp_tag.content = &m_intp[intp_no].stQ;

	return acp_read_tag(intp_tag);
}

/**
 * 写XYZ结构体输入.
 *
 * \param intp_no 通道号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_write_intp_xyz_input(int intp_no) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG intp_tag, param_tag;
	char intp_name[255], param_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(param_name, "%s.astIntpXYZParam[%d]", m_prefix, intp_no);
	param_tag.name = param_name;
	param_tag.size = sizeof(m_intp_param[intp_no]);
	param_tag.content = &m_intp_param[intp_no];
	result = acp_write_tag(param_tag);
		
	if (result == ET_RESULT_NO_ERROR) {
		sprintf_s(intp_name, "%s.astIntpXYZCtrl[%d].stI", m_prefix, intp_no);
		intp_tag.name = intp_name;
		intp_tag.size = sizeof(m_intp[intp_no].stI);
		intp_tag.content = &m_intp[intp_no].stI;
		result = acp_write_tag(intp_tag);
	}

	return result;
}

/**
 * 读XYZ指令执行结果.
 *
 * \param intp_no 通道号: 从0开始
 * \param ready 准备好
 * \param done 执行完成
 * \param busy 执行中
 * \param error 执行失败
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_intp_xyz_command_result(int intp_no, bool *power, bool *ready, bool *done, bool *busy, bool *error) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_intp_xyz_output(intp_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if(power != nullptr)
			*power = m_intp[intp_no].stQ.xInRun;
			
		if (ready != nullptr)
			*ready = m_intp[intp_no].stQ.xReady;

		if (done != nullptr)
			*done = m_intp[intp_no].stQ.xDone;

		if (busy != nullptr)
			*busy = m_intp[intp_no].stQ.xBusy;

		if (error != nullptr)
			*error = m_intp[intp_no].stQ.xError;
	}

	return result;
}

/**
 * 设置XYZ运动参数.
 *
 * \param intp_no 通道号: 从0开始
 * \param vel 目标速度(用户单位)
 * \param acc 加速度(用户单位)
 * \param dec 减速度(用户单位)
 * \param jerk 跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_intp_xyz_set_profile_param(int intp_no, double vel, double acc, double dec, double jerk) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_intp_xyz_input(intp_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_intp[intp_no].stI.rVelocity = vel;
		m_intp[intp_no].stI.rAcceleration = acc;
		m_intp[intp_no].stI.rDeceleration = dec;
		m_intp[intp_no].stI.rJerk = jerk;			
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_intp_xyz_input(intp_no);
	}

	return result;
}

/**
 * 读取XYZ运动参数设定.
 *
 * \param intp_no 通道号: 从0开始
 * \param pos 目标位置
 * \param offset 工具偏移
 * \param vel 目标速度(用户单位)
 * \param acc 加速度(用户单位)
 * \param dec 减速度(用户单位)
 * \param jerk 跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_intp_xyz_get_profile_param(int intp_no, MC_COORD_REF &pos, SMC_VECTOR3D &offset, double &vel, double &acc, double &dec, double &jerk) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_intp_xyz_input(intp_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		pos = m_intp[intp_no].stI.stPosition;
		offset = m_intp[intp_no].stI.stToolOffset;
		vel = m_intp[intp_no].stI.rVelocity;
		acc = m_intp[intp_no].stI.rAcceleration;
		dec = m_intp[intp_no].stI.rDeceleration;
		jerk = m_intp[intp_no].stI.rJerk;
	}

	return result;
}

/**
 * XYZ开使能.
 * 
 * \param intp_no 通道号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_intp_xyz_power_on(int intp_no) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_intp_xyz_input(intp_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_intp[intp_no].stI.xPowerOn = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_intp_xyz_input(intp_no);
	}

	return result;
}

/**
 * XYZ关使能.
 *
 * \param intp_no 通道号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_intp_xyz_power_off(int intp_no) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_intp_xyz_input(intp_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_intp[intp_no].stI.xPowerOn = false;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_intp_xyz_input(intp_no);
	}

	return result;
}

/**
 * XYZ绝对定位 - 启动.
 * 
 * \param intp_no 通道号: 从0开始
 * \param pos_x 目标位置 - X轴
 * \param pos_y 目标位置 - Y轴
 * \param pos_z 目标位置 - Z轴
 * \param pos_c 目标位置 - 旋转轴
 * \param tool_x 工具偏移 - X方向
 * \param tool_y 工具偏移 - Y方向
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_intp_xyz_move_absolute(int intp_no, double pos_x, double pos_y, double pos_z, double pos_c, double tool_x, double tool_y) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_intp_xyz_command(intp_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_intp[intp_no].stI.etRunMode = ET_INTPMODULE_CMD_MOVEABS;
		m_intp[intp_no].stI.stPosition.X = pos_x;
		m_intp[intp_no].stI.stPosition.Y = pos_y;
		m_intp[intp_no].stI.stPosition.Z = pos_z;
		m_intp[intp_no].stI.stPosition.C = pos_c;
		m_intp[intp_no].stI.stToolOffset.dX = tool_x;
		m_intp[intp_no].stI.stToolOffset.dY = tool_y;		
		m_intp[intp_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_intp_xyz_input(intp_no);
	}

	return result;
}

 /**
  * XYZ相对定位 - 启动.
  *
  * \param intp_no 通道号: 从0开始
  * \param pos_x 目标位置 - X轴
  * \param pos_y 目标位置 - Y轴
  * \param pos_z 目标位置 - Z轴
  * \param pos_c 目标位置 - 旋转轴
  * \param tool_x 工具偏移 - X方向
  * \param tool_y 工具偏移 - Y方向
  * \return 成功返回0, 失败返回-1或者错误码
  */
SOFTMOTION_API ET_RESULT smc_intp_xyz_move_relative(int intp_no, double pos_x, double pos_y, double pos_z, double pos_c, double tool_x, double tool_y) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_intp_xyz_command(intp_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_intp[intp_no].stI.etRunMode = ET_INTPMODULE_CMD_MOVEREL;
		m_intp[intp_no].stI.stPosition.X = pos_x;
		m_intp[intp_no].stI.stPosition.Y = pos_y;
		m_intp[intp_no].stI.stPosition.Z = pos_z;
		m_intp[intp_no].stI.stPosition.C = pos_c;
		m_intp[intp_no].stI.stToolOffset.dX = tool_x;
		m_intp[intp_no].stI.stToolOffset.dY = tool_y;
		m_intp[intp_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_intp_xyz_input(intp_no);
	}

	return result;
}

/**
 * XYZ停止.
 *
 * \param intp_no 通道号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_intp_xyz_halt(int intp_no) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_intp_xyz_command(intp_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_intp[intp_no].stI.etRunMode = ET_INTPMODULE_CMD_HALT;
		m_intp[intp_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_intp_xyz_input(intp_no);
	}

	return result;
}

/**
 * XYZ故障复位.
 *
 * \param intp_no 通道号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_intp_xyz_reset(int intp_no) {
	ET_RESULT result = check_intp_xyz_no(intp_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_intp_xyz_command(intp_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_intp[intp_no].stI.xReset = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_intp_xyz_input(intp_no);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_intp[intp_no].stI.xReset = false;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_intp_xyz_input(intp_no);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return result;
}