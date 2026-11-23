#include "acp_access.h"
#include "struct_robot.h"
#include "softmotion.h"

#include <Windows.h>
#include <thread>
#include <chrono>
#include <mutex>

static const int c_module_num_robot = 16;
static ROBOT_MODULE_INTERFACE m_robot[c_module_num_robot];
static ROBOT_MODULE_PARAM m_robot_param[c_module_num_robot];
static int16_t m_robot_axis_no[c_module_num_robot][10];
static int16_t m_robot_axis_num[c_module_num_robot];
static std::mutex m_mutex;

static const char* m_prefix = "Application.SmcExt.G_PCMotion";
extern SOFTMOTION_API ET_RESULT smc_sync_robotctrl_inout(int robot_no = -1);
extern SOFTMOTION_API ET_RESULT smc_read_robotctrl_input(int robot_no);
extern SOFTMOTION_API ET_RESULT smc_read_robotctrl_output(int robot_no);
extern SOFTMOTION_API ET_RESULT smc_write_robotctrl_input(int robot_no);

/**
 * 检查机器人编号是否在范围内.
 *
 * \param robot_no 机器人编号
 * \param min 最小值
 * \param max 最大值
 * \return 0-范围外 1-范围内
 */
ET_RESULT check_robot_no(int robot_no, int min = 0, int max = c_module_num_robot) {
	return robot_no >= min && robot_no < max ? ET_RESULT_NO_ERROR : ET_RESULT_CMD_ERR_AXIS_OUT_OF_RANGE;
}

/**
 * 机器人命令预处理.
 *
 * \param robot_no 机器人编号
 * \return 成功返回0, 失败返回-1或者错误码
 */
ET_RESULT preprocess_robot_command(int robot_no, int timeout = 5) {
	ET_RESULT result = smc_read_robotctrl_input(robot_no);
	if (result != ET_RESULT_NO_ERROR)
		return result;

	bool write_input = false;
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		bool start = m_robot[robot_no].stI.xStart;
		bool estop = m_robot[robot_no].stI.xEStop;
		bool reset = m_robot[robot_no].stI.xReset;
		bool jog_f = m_robot[robot_no].stI.xJogForward;
		bool jog_b = m_robot[robot_no].stI.xJogBackward;

		if (start || estop || reset || jog_f || jog_b) {
			m_robot[robot_no].stI.xStart = false;
			m_robot[robot_no].stI.xEStop = false;
			m_robot[robot_no].stI.xReset = false;
			m_robot[robot_no].stI.xJogForward = false;
			m_robot[robot_no].stI.xJogBackward = false;
			write_input = true;			
		}
	}
	
	if (write_input) {
		result = smc_write_robotctrl_input(robot_no);
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	}

	return result;
}

/**
 * 机器人命令后处理.
 *
 * \param robot_no 机器人编号
 * \return 成功返回0, 失败返回-1或者错误码
 */
ET_RESULT postprocess_robot_command(int robot_no, int timeout = 10, int retry = 3) {
	ET_RESULT result = ET_RESULT_NO_ERROR;
	int cnt = 0;
	bool done = false, busy = false, error = false;
	uint16_t error_id;
	do {
		result = smc_robot_command_result(robot_no, &done, &busy, &error, &error_id);
		++cnt;
		std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
	} while (result == ET_RESULT_NO_ERROR && !done && !error && cnt < retry);

	if (result != ET_RESULT_NO_ERROR)
		return result;

	if (error)
		return make_result(ET_RESULT_OFFSET_SMC_ERR, error_id);

	if (cnt >= retry)
		return ET_RESULT_CMD_ERR_TIMEOUT;

	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.xStart = false;
	}

	result = smc_write_robotctrl_input(robot_no);	
	return done ? result : ET_RESULT_CMD_ERR_NO_EXECUTE;
}

/**
 * 同步机器人结构体输入输出.
 *
 * \param robot_no 机器人编号: 从0开始, -1表示读取所有机器人
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_sync_robotctrl_inout(int robot_no) {
	ET_RESULT result = ET_RESULT_NO_ERROR;
	PLC_TAG robot_tag, param_tag;
	char robot_name[255], param_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	if (robot_no == -1) {
		sprintf_s(param_name, "%s.astRobotParam[0]", m_prefix);
		param_tag.name = param_name;
		param_tag.size = sizeof(m_robot_param);
		param_tag.content = m_robot_param;
		result = acp_read_tag(param_tag);

		if (result == ET_RESULT_NO_ERROR) {
			sprintf_s(robot_name, "%s.astRobotCtrl[0]", m_prefix);
			robot_tag.name = robot_name;
			robot_tag.size = sizeof(m_robot);
			robot_tag.content = m_robot;
			result = acp_read_tag(robot_tag);
		}
	} else {
		result = check_robot_no(robot_no);
		if (result == ET_RESULT_NO_ERROR) {
			sprintf_s(param_name, "%s.astRobotParam[%d]", m_prefix, robot_no);
			param_tag.name = param_name;
			param_tag.size = sizeof(m_robot_param[robot_no]);
			param_tag.content = &m_robot_param[robot_no];
			result = acp_read_tag(param_tag);	

			if (result == ET_RESULT_NO_ERROR) {
				sprintf_s(robot_name, "%s.astRobotCtrl[%d]", m_prefix, robot_no);
				robot_tag.name = robot_name;
				robot_tag.size = sizeof(m_robot[robot_no]);
				robot_tag.content = &m_robot[robot_no];
				result = acp_read_tag(robot_tag);
			}
		}
	}

	return result;
}

/**
 * 读机器人结构体输入.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_read_robotctrl_input(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG robot_tag, param_tag;
	char robot_name[255], param_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(param_name, "%s.astRobotParam[%d]", m_prefix, robot_no);
	param_tag.name = param_name;
	param_tag.size = sizeof(m_robot_param[robot_no]);
	param_tag.content = &m_robot_param[robot_no];
	result = acp_read_tag(param_tag);

	if (result == ET_RESULT_NO_ERROR) {
		sprintf_s(robot_name, "%s.astRobotCtrl[%d].stI", m_prefix, robot_no);
		robot_tag.name = robot_name;
		robot_tag.size = sizeof(m_robot[robot_no].stI);
		robot_tag.content = &m_robot[robot_no].stI;
		result = acp_read_tag(robot_tag);
	}

	return result;
}

/**
 * 读机器人结构体输出.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_read_robotctrl_output(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG robot_tag;
	char robot_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(robot_name, "%s.astRobotCtrl[%d].stQ", m_prefix, robot_no);
	robot_tag.name = robot_name;
	robot_tag.size = sizeof(m_robot[robot_no].stQ);
	robot_tag.content = &m_robot[robot_no].stQ;

	return acp_read_tag(robot_tag);
}

/**
 * 写机器人结构体输入.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_write_robotctrl_input(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG robot_tag, param_tag;
	char robot_name[255], param_name[255];
	std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(param_name, "%s.astRobotParam[%d]", m_prefix, robot_no);
	param_tag.name = param_name;
	param_tag.size = sizeof(m_robot_param[robot_no]);
	param_tag.content = &m_robot_param[robot_no];
	result = acp_write_tag(param_tag);

	if (result == ET_RESULT_NO_ERROR) {
		sprintf_s(robot_name, "%s.astRobotCtrl[%d].stI", m_prefix, robot_no);
		robot_tag.name = robot_name;
		robot_tag.size = sizeof(m_robot[robot_no].stI);
		robot_tag.content = &m_robot[robot_no].stI;
		result = acp_write_tag(robot_tag);
	}

	return result;
}

/**
 * 读机器人通用状态.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param enabled 0-未使能 1-已使能
 * \param comm_status 0-未通讯上 1-通讯正常
 * \param power_status 0-未使能 1-已使能
 * \param ref_status 0-未回原点 1-已回原点
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_common_status(int robot_no, bool &enabled, bool &comm_status, bool &power_status, bool &ref_status) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_output(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		enabled = m_robot[robot_no].stQ.xEnabled;
		comm_status = m_robot[robot_no].stQ.xCommOK;
		power_status = m_robot[robot_no].stQ.xInRun;
		ref_status = m_robot[robot_no].stQ.xRefOK;
	}	

	return result;
}

/**
 * .
 *
 * \param robot_no 
 * \param status 
 * \return 
 */
/**
 * 读机器人运动状态.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param state 机器人运动状态
 * \param stopped 0-运动中 1-停止
 * \param homing 1-回原点中
 * \param sync 0-未同步 1-已同步
 * \param tracking 1-跟踪中
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_motion_status(int robot_no, SMC_ROBOT_STATE &state, bool &stopped, bool &homing, bool &sync, bool &tracking) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_output(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		state = m_robot[robot_no].stQ.etGrpState;
		stopped = m_robot[robot_no].stQ.xStopped;
		homing = m_robot[robot_no].stQ.xHoming;
		sync = m_robot[robot_no].stQ.xInSync;
		tracking = m_robot[robot_no].stQ.xTrackingDynamicCS;
	}

	return result;
}

/**
 * 读机器人报警.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param alarm 0-无报警 1-报警
 * \param alarm_id 轴报警代码
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_alarm_status(int robot_no, bool &alarm, uint32_t &alarm_id) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_output(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		alarm = m_robot[robot_no].stQ.xGrpAlarm;
		alarm_id = m_robot[robot_no].stQ.dwGrpAlarmID;
	}

	return result;
}

/**
 * 读机器人当前控制模式.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param status 当前控制模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_control_status(int robot_no, ET_ROBOTMODULE_CMD &status) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}
	
	result = smc_read_robotctrl_output(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		status = m_robot[robot_no].stQ.etActiveMode;
	}

	return result;
}

/**
 * 读机器人指令执行结果.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param done 执行完成
 * \param busy 执行中
 * \param error 执行失败
 * \param error_id 故障代码
 * \param active 命令已激活
 * \param cmd_aborted 命令被打断
 * \param cmd_accepted 命令已接受
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_command_result(int robot_no, bool *done, bool *busy, bool *error, uint16_t *error_id, bool *active, bool *cmd_aborted, bool *cmd_accepted) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}
	
	result = smc_read_robotctrl_output(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (done != nullptr)
			*done = m_robot[robot_no].stQ.xDone;

		if (busy != nullptr)
			*busy = m_robot[robot_no].stQ.xBusy;

		if (error != nullptr)
			*error = m_robot[robot_no].stQ.xError;

		if (error_id != nullptr)
			*error_id = m_robot[robot_no].stQ.etErrorId;

		if (active != nullptr)
			*active = m_robot[robot_no].stQ.xActive;

		if (cmd_aborted != nullptr)
			*cmd_aborted = m_robot[robot_no].stQ.xCommandAborted;

		if (cmd_accepted != nullptr)
			*cmd_accepted = m_robot[robot_no].stQ.xCommandAccepted;
	}

	return result;
}

/**
 * 读机器人当前位置.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 机器人当前位置
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_actual_position(int robot_no, ROBOT_MODULE_COORD &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}
	
	result = smc_read_robotctrl_output(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stQ.stActPosition;
	}

	return result;
}

/**
 * 读机器人当前速度.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 机器人当前速度
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_actual_velocity(int robot_no, ROBOT_MODULE_COORD &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}
	
	result = smc_read_robotctrl_output(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stQ.stActVelocity;
	}

	return result;
}

/**
 * 读机器人输出结构体.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 机器人输出结构体
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_total_output(int robot_no, ROBOT_MODULE_OUTPUTS &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_output(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stQ;
	}

	return result;
}

/**
 * 设置机器人运动参数 - 圆弧插补参数.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param circ_mode 圆弧插补模式
 * \param circ_path 圆弧插补方向
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_circular(int robot_no, SMC_CIRC_MODE circ_mode, MC_CIRC_PATHCHOICE circ_path) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etCircMode = circ_mode;
		m_robot[robot_no].stI.etCircPathChoice = circ_path;		
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 坐标系.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 坐标系
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_coordinate(int robot_no, SMC_COORD_SYSTEM &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etCoordSystem = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 目标位置.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 目标位置(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_position(int robot_no, SMC_POS_REF value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.stPosition = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 路径速度.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 目标速度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_velocity(int robot_no, double value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.rVelocity = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 路径加减速及S曲线斜坡.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param acc 加速度(用户单位)
 * \param dec 减速度(用户单位)
 * \param jerk 跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_ramp(int robot_no, double acc, double dec, double jerk) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (acc > 0.001)
			m_robot[robot_no].stI.rAcceleration = acc;

		if (dec > 0.001)
			m_robot[robot_no].stI.rDeceleration = dec;

		if (jerk > 0.001)
			m_robot[robot_no].stI.rJerk = jerk;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 比例系数.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param acc_factor 加速比例系数(0-1)
 * \param dec_factor 减速比例系数(0-1)
 * \param jerk_factor 跃度比例系数(0-1)
 * \param torque_factor 扭矩比例系数(0-1)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_factor(int robot_no, double vel_factor, double acc_factor, double jerk_factor, double torque_factor) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].rFBVelFactor = vel_factor;
		m_robot_param[robot_no].rFBAccFactor = acc_factor;
		m_robot_param[robot_no].rFBJerkFactor = jerk_factor;
		m_robot_param[robot_no].rFBTorqueFactor = torque_factor;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动缓冲模式.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 缓冲模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_buffermode(int robot_no, MC_BUFFER_MODE value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etBufferMode = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人平移混合模式 - 无混合.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_transmode_none(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etTransitionMode = MC_TRANSITION_TMNONE;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人平移混合模式 - 基于速度混合.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param vel_factor 速度系数
 * \param blend_with_aux 辅助轴是否参与混合
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_transmode_start_velocity(int robot_no, double vel_factor, bool blend_with_aux) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etTransitionMode = MC_TRANSITION_TMSTART_VELOCITY;
		m_robot_param[robot_no].rTMVelFactor = vel_factor;
		m_robot_param[robot_no].xTMWithAux = blend_with_aux;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人平移混合模式 - 基于距离混合.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param distance 转角距离
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_transmode_corner_distance(int robot_no, double distance) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etTransitionMode = MC_TRANSITION_TMCORNER_DISTANCE;
		m_robot_param[robot_no].rTMCornerDistance = distance;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人平移混合模式 - 基于轴距离混合.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param line_dist 直线距离
 * \param ang_dist 转角距离
 * \param blend_with_aux 辅助轴是否参与混合
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_transmode_axis_distance(int robot_no, double line_dist, double ang_dist, bool euc_done, bool blend_with_aux) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etTransitionMode = MC_TRANSITION_TMAXIS_DISTANCE;
		m_robot_param[robot_no].rTMAxisLine = line_dist;
		m_robot_param[robot_no].rTMAxisAngle = ang_dist;
		m_robot_param[robot_no].xTMAxisDoneOnEucDistReached = euc_done;
		m_robot_param[robot_no].xTMWithAux = blend_with_aux;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人旋转混合模式.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param mode 旋转混合模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_profile_orienmode(int robot_no, SMC_ORIENTATION_MODE mode) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etOrientationMode = mode;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 读取机器人运动参数 - 圆弧插补参数.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param circ_mode 圆弧插补模式
 * \param circ_path 圆弧插补方向
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_circular(int robot_no, SMC_CIRC_MODE &circ_mode, MC_CIRC_PATHCHOICE &circ_path) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		circ_mode = m_robot[robot_no].stI.etCircMode;
		circ_path = m_robot[robot_no].stI.etCircPathChoice;
	}

	return result;
}

/**
 * 读取机器人运动参数 - 坐标系.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 坐标系
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_coordinate(int robot_no, SMC_COORD_SYSTEM &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stI.etCoordSystem;
	}

	return result;
}

/**
 * 读取机器人运动参数 - 目标位置.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 目标位置(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_position(int robot_no, SMC_POS_REF &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stI.stPosition;
	}

	return result;
}

/**
 * 读取机器人运动参数 - 辅助点位置.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param value 中间位置(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_aux_position(int robot_no, SMC_POS_REF &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stI.stCircAuxPoint;
	}

	return result;
}

/**
 * 读取机器人运动参数 - 路径速度.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 目标速度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_velocity(int robot_no, double &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stI.rVelocity;
	}

	return result;
}

/**
 * 读取机器人运动参数 - 路径加减速及S曲线斜坡.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param acc 加速度(用户单位)
 * \param dec 减速度(用户单位)
 * \param jerk 跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_ramp(int robot_no, double &acc, double &dec, double &jerk) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		acc = m_robot[robot_no].stI.rAcceleration;
		dec = m_robot[robot_no].stI.rDeceleration;
		jerk = m_robot[robot_no].stI.rJerk;
	}

	return result;
}

/**
 * 读取机器人运动参数 - 比例系数.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param vel_factor 速度比例系数(0-1)
 * \param acc_factor 加速比例系数(0-1)
 * \param jerk_factor 跃度比例系数(0-1)
 * \param torque_factor 扭矩比例系数(0-1)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_factor(int robot_no, double &vel_factor, double &acc_factor, double &jerk_factor, double &torque_factor) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		vel_factor = m_robot_param[robot_no].rFBVelFactor;
		acc_factor = m_robot_param[robot_no].rFBAccFactor;
		jerk_factor = m_robot_param[robot_no].rFBJerkFactor;
		torque_factor = m_robot_param[robot_no].rFBTorqueFactor;
	}

	return result;
}

/**
 * 读取机器人运动缓冲模式.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 缓冲模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_buffermode(int robot_no, MC_BUFFER_MODE &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stI.etBufferMode;
	}

	return result;
}

/**
 * 读取机器人平移混合模式.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param value 平移混合模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_transmode(int robot_no, MC_TRANSITION_MODE &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stI.etTransitionMode;
	}

	return result;
}

/**
 * 读取机器人平移混合参数.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param vel_factor 速度系数(基于速度混合)
 * \param corner_dist 转角距离(基于转角距离混合)
 * \param line_dist 直线距离(基于轴距离混合)
 * \param ang_dist 角度(基于轴距离混合)
 * \param euc_done 位移到达后输出done(基于轴距离混合)
 * \param blend_with_aux 辅助轴是否参与混合(基于速度混合\基于轴距离混合)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_transmode_param(int robot_no, double &vel_factor, double &corner_dist, double &line_dist, double &ang_dist, bool &euc_done, bool &blend_with_aux) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		vel_factor = m_robot_param[robot_no].rTMVelFactor;
		corner_dist = m_robot_param[robot_no].rTMCornerDistance;
		line_dist = m_robot_param[robot_no].rTMAxisLine;
		ang_dist = m_robot_param[robot_no].rTMAxisAngle;
		euc_done = m_robot_param[robot_no].xTMAxisDoneOnEucDistReached;
		blend_with_aux = m_robot_param[robot_no].xTMWithAux;
	}

	return result;
}

/**
 * 读取机器人旋转混合模式.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 旋转混合模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_profile_orienmode(int robot_no, SMC_ORIENTATION_MODE &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot[robot_no].stI.etOrientationMode;
	}

	return result;
}

/**
 * 设置机器人运动参数 - 急停加减速及S曲线斜坡.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param dec 急停减速度(用户单位)
 * \param jerk 急停跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_estop_ramp(int robot_no, double dec, double jerk) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (dec > 0.001)
			m_robot_param[robot_no].rEStopRamp = dec;

		if (jerk > 0.001)
			m_robot_param[robot_no].rEStopJerk = jerk;		
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 急停加减速及S曲线比例系数.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param dec_factor 急停减速度系数(0-1)
 * \param jerk_factor 急停跃度系数(0-1)
 * \param torque_factor 急停扭矩系数(0-1)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_estop_factor(int robot_no, double dec_factor, double jerk_factor, double torque_factor) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].rEStopRampFactor = dec_factor;
		m_robot_param[robot_no].rEStopJerkFactor = jerk_factor;
		m_robot_param[robot_no].rEStopTorqueFactor = torque_factor;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 急停清除运动缓存.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 0-不清除缓存 1-清除缓存
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_estop_clear_movement(int robot_no, bool value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].xEStopClearMovements = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置急停IO.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param module_no 急停IO模块编号
 * \param io_no 急停IO模块输入点编号
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_estop_io(int robot_no, int16_t module_no, int16_t io_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].iEStopModuleNo = module_no;
		m_robot_param[robot_no].iEStopIoNo = module_no;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 读取机器人运动参数 - 急停加减速及S曲线斜坡.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param dec 急停减速度(用户单位)
 * \param jerk 急停跃度(用户单位)
 * \param reload 0-从缓存读取 1-通讯读取
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_estop_ramp(int robot_no, double &dec, double &jerk) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		dec = m_robot_param[robot_no].rEStopRamp;
		jerk = m_robot_param[robot_no].rEStopJerk;
	}

	return result;
}

/**
 * 读取机器人运动参数 - 急停加减速及S曲线比例系数.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param dec_factor 急停减速度系数(0-1)
 * \param jerk_factor 急停跃度系数(0-1)
 * \param torque_factor 急停扭矩系数(0-1)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_estop_factor(int robot_no, double &dec_factor, double &jerk_factor, double &torque_factor) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		dec_factor = m_robot_param[robot_no].rEStopRampFactor;
		jerk_factor = m_robot_param[robot_no].rEStopJerkFactor;
		torque_factor = m_robot_param[robot_no].rEStopTorqueFactor;
	}

	return result;
}

/**
 * 读取机器人运动参数 - 急停清除运动缓存.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 0-不清除缓存 1-清除缓存
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_estop_clear_movement(int robot_no, bool &value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		value = m_robot_param[robot_no].xEStopClearMovements;
	}

	return result;
}

/**
 * 读取急停IO.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param module_no 急停IO模块编号
 * \param io_no 急停IO模块输入点编号
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_estop_io(int robot_no, int16_t &module_no, int16_t &io_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		module_no = m_robot_param[robot_no].iEStopModuleNo;
		io_no = m_robot_param[robot_no].iEStopIoNo;
	}

	return result;
}

/**
 * 设置机器人运动参数 - 点动距离限制.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param line_dist 直线距离限制
 * \param ang_dist 角度限制
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_jog_limit(int robot_no, double line_dist, double ang_dist) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].rJogMaxDistOfLinear = line_dist;
		m_robot_param[robot_no].rJogMaxDistOfAngular = ang_dist;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 点动速度.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param value 点动速度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_jog_velocity(int robot_no, double value) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].rJogVelocity = value;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 点动加减速及S曲线斜坡.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param acc 点动加速度(用户单位)
 * \param dec 点动减速度(用户单位)
 * \param jerk 点动跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_jog_ramp(int robot_no, double acc, double dec, double jerk) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (acc > 0.001)
			m_robot_param[robot_no].rJogAcceleration = acc;

		if (dec > 0.001)
			m_robot_param[robot_no].rJogDeceleration = dec;

		if (jerk > 0.001)
			m_robot_param[robot_no].rJogJerk = jerk;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设置机器人运动参数 - 点动加减速及S曲线比例系数.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param vel_factor 点动速度系数(0-1)
 * \param acc_factor 点动加减速系数(0-1)
 * \param jerk_factor 点动跃度系数(0-1)
 * \param torque_factor 点动扭矩系数(0-1)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_jog_factor(int robot_no, double vel_factor, double acc_factor, double jerk_factor, double torque_factor) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].rJogVelFactor = vel_factor;
		m_robot_param[robot_no].rJogAccFactor = acc_factor;
		m_robot_param[robot_no].rJogJerkFactor = jerk_factor;
		m_robot_param[robot_no].rJogTorqueFactor = torque_factor;			
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 读取机器人运动参数 - 点动.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param line_dist 直线距离限制(用户单位)
 * \param ang_dist 角度限制(用户单位)
 * \param vel 点动速度(用户单位)
 * \param acc 点动加速度(用户单位)
 * \param dec 点动减速度(用户单位)
 * \param jerk 点动跃度(用户单位)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_jog_param(int robot_no, double &line_dist, double &ang_dist, double &vel, double &acc, double &dec, double &jerk) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		line_dist = m_robot_param[robot_no].rJogMaxDistOfLinear;
		ang_dist = m_robot_param[robot_no].rJogMaxDistOfAngular;
		vel = m_robot_param[robot_no].rJogVelocity;
		acc = m_robot_param[robot_no].rJogAcceleration;
		dec = m_robot_param[robot_no].rJogDeceleration;
		jerk = m_robot_param[robot_no].rJogJerk;
	}

	return result;
}

/**
 * 读取机器人运动参数 - 点动加减速及S曲线比例系数.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param vel_factor 点动速度系数(0-1)
 * \param acc_factor 点动加减速系数(0-1)
 * \param jerk_factor 点动跃度系数(0-1)
 * \param torque_factor 点动扭矩系数(0-1)
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_jog_factor(int robot_no, double &vel_factor, double &acc_factor, double &jerk_factor, double &torque_factor) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		vel_factor = m_robot_param[robot_no].rJogVelFactor;
		acc_factor = m_robot_param[robot_no].rJogAccFactor;
		jerk_factor = m_robot_param[robot_no].rJogJerkFactor;
		torque_factor = m_robot_param[robot_no].rJogTorqueFactor;
	}

	return result;
}

/**
 * 机器人启用.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_enable(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.xEnable = true;		
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人禁用.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_disable(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.xEnable = false;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人开使能.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_power_on(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.xPowerOn = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人关使能.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_power_off(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.xPowerOn = false;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人点到点绝对运动 - 启动.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param coord 坐标系
 * \param position 目标位置
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_ptp_absolute(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF position) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_MOVEABS;
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.stPosition = position;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人点到点相对运动 - 启动.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param coord 坐标系
 * \param distance 目标距离
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_ptp_relative(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF distance) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_MOVEREL;
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.stPosition = distance;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人直线插补绝对运动 - 启动.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param coord 坐标系
 * \param position 目标位置
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_linear_absolute(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF position) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_MOVEABS_LINEAR;
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.stPosition = position;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人直线插补相对运动 - 启动.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param coord 坐标系
 * \param position 目标位置
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_linear_relative(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF distance) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_MOVEREL_LINEAR;
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.stPosition = distance;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人圆弧插补绝对运动 - 启动.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param coord 坐标系
 * \param pos 目标位置
 * \param aux_pos 辅助位置
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_circ_absolute(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF pos, SMC_POS_REF aux_pos) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_MOVEABS_CIRCULAR;
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.stPosition = pos;
		m_robot[robot_no].stI.stCircAuxPoint = aux_pos;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人圆弧插补相对运动 - 启动.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param coord 坐标系
 * \param pos 目标位置
 * \param aux_pos 辅助位置
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_circ_relative(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF pos, SMC_POS_REF aux_pos) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_MOVEREL_CIRCULAR;
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.stPosition = pos;
		m_robot[robot_no].stI.stCircAuxPoint = aux_pos;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人暂停.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param clear_movements 清除运动缓存
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_halt(int robot_no, bool clear_movements) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].xHaltClearMovements = clear_movements;
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_HALT;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人复位.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_reset(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.xReset = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);	
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.xReset = false;		
	}
	
	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人急停 - EStop.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param enable 0-释放急停 1-按下急停
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_estop(int robot_no, bool enable, bool clear_movements) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result != ET_RESULT_NO_ERROR)
		return result;

	static int16_t module_no = -1, io_no = -1;
	if (enable) {
		if (result == ET_RESULT_NO_ERROR) {
			result = smc_robot_get_estop_io(robot_no, module_no, io_no);
		}

		if (result == ET_RESULT_NO_ERROR) {
			result = smc_robot_set_estop_io(robot_no, -1, -1);
		}

		if (result == ET_RESULT_NO_ERROR) {
			const std::lock_guard<std::mutex> locker(m_mutex);
			m_robot_param[robot_no].xEStopClearMovements = clear_movements;
			m_robot[robot_no].stI.xEStop = true;
		}

		if (result == ET_RESULT_NO_ERROR) {
			smc_write_robotctrl_input(robot_no);
		}

	} else {
		if (result == ET_RESULT_NO_ERROR) {
			const std::lock_guard<std::mutex> locker(m_mutex);
			m_robot_param[robot_no].xEStopClearMovements = clear_movements;
			m_robot[robot_no].stI.xEStop = false;
		}

		if (result == ET_RESULT_NO_ERROR) {
			result = smc_write_robotctrl_input(robot_no);
		}

		if (result == ET_RESULT_NO_ERROR) {
			result = smc_robot_set_estop_io(robot_no, module_no, io_no);
		}
	}

	return result;
}

/**
 * 机器人安全高度设置.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param position 安全高度
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_safe_height(int robot_no, double position) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].rSafeHeight = position;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人安全高度读取.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param position 安全高度
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_safe_height(int robot_no, double &position) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = smc_read_robotctrl_input(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		position = m_robot_param[robot_no].rSafeHeight;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人跳跃.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param position 目标位置
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_ptp_jump(int robot_no, SMC_COORD_SYSTEM coord, MC_COORD_REF position) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_JUMPABS;
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.stPosition.c = position;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人点动前进.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param axis_no 点动轴
 * \param enable 0-停止点动 1-开始点动
 * \param abc_as_acs 轴坐标系下点动ABC轴
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_jog_forward(int robot_no, SMC_COORD_SYSTEM coord, int axis_no, bool enable, bool abc_as_acs) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.iJogAxisNo = axis_no;
		m_robot[robot_no].stI.xJogAbcAsAcs = abc_as_acs;
		m_robot[robot_no].stI.xJogForward = enable;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人点动后退.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param axis_no 点动轴
 * \param enable 0-停止点动 1-开始点动
 * \param abc_as_acs 轴坐标系下点动ABC轴
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_jog_backward(int robot_no, SMC_COORD_SYSTEM coord, int axis_no, bool enable, bool abc_as_acs) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.iJogAxisNo = axis_no;
		m_robot[robot_no].stI.xJogAbcAsAcs = abc_as_acs;
		m_robot[robot_no].stI.xJogBackward = enable;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人设置坐标值.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param coord 坐标系
 * \param position 设定位置
 * \param relative 0-绝对位置 1-相对位置
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_setposition(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF position, bool relative) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etCoordSystem = coord;
		m_robot[robot_no].stI.stPosition = position;
		m_robot[robot_no].stI.etRunMode = relative ? ET_ROBOTMODULE_CMD_SETPOS_REL : ET_ROBOTMODULE_CMD_SETPOS_ABS;
		m_robot[robot_no].stI.xStart = true;		
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 读取机器人包含的轴数量及编号.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param axis_no 轴编号列表
 * \param num 轴数量
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_axis_list(int robot_no, int16_t *axis_no, int16_t &num) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	PLC_TAG param_tag;
	char param_name[255];
	const std::lock_guard<std::mutex> locker(m_mutex);

	sprintf_s(param_name, "%s.aiRobotAxisNo", m_prefix);
	param_tag.name = param_name;
	param_tag.size = sizeof(m_robot_axis_no);
	param_tag.content = m_robot_axis_no;
	result = acp_read_tag(param_tag);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	sprintf_s(param_name, "%s.aiRobotAxisNum", m_prefix);
	param_tag.name = param_name;
	param_tag.size = sizeof(m_robot_axis_num);
	param_tag.content = m_robot_axis_num;
	result = acp_read_tag(param_tag);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	num = m_robot_axis_num[robot_no];
	for (int i = 0; i < num; i++) {
		axis_no[i] = m_robot_axis_no[robot_no][i];
	}

	return result;
}

/**
 * 机器人回零顺序设置.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param seq_index 回零次序
 * \param seq_dat 该次序回零包含的轴: 每个bit代表一个轴
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_home_sequence(int robot_no, int16_t seq_index, int16_t seq_dat) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		if (seq_index >= 0 && seq_index < 10) {
			m_robot_param[robot_no].aiHomeSequence[seq_index] = seq_dat;
		}
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人回零 - 驱动器回零.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_drv_home(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = EM_ROBOTMODULE_CMD_DRV_HOME;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人回零 - 控制器回零.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_plc_home(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = EM_ROBOTMODULE_CMD_SMC_HOME;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 机器人读取工具偏移量.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param offset 工具偏移量
 * \param mode 读取模式
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_get_tool_offset(int robot_no, MC_COORD_REF &offset, SMC_READ_TOOL_MODE mode) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].stGetToolMode = mode;
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_GET_TOOL_OFFSET;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_read_robotctrl_input(robot_no);
	}

	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		offset = m_robot_param[robot_no].stGetToolOffset;
	}

	return result;
}

/**
 * 机器人设置工具偏移量.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param offset 工具偏移量
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_tool_offset(int robot_no, MC_COORD_REF offset) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].stSetToolOffset = offset;
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_SET_TOOL_OFFSET;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设定CAXIS A4旋转轴周期 XYZ+R机器人可用.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \param period 旋转轴周期: (>0)->[360°*(i-1)-180°,360°*i-180°[; (<0)->[360°*i-180°,360°*(i-1)-180°[
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_caxis_period(int robot_no, int period) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].iToolPeriod = period;
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_SET_CAXIS_PERIOD;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设定ZCSCARA A4旋转轴周期.
 *
 * \param robot_no 机器人编号: 从0开始
 * \param period 旋转轴周期: (>0)->[360°*(i-1)-180°,360°*i-180°[; (<0)->[360°*i-180°,360°*(i-1)-180°[
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_zcscara_period(int robot_no, int period) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot_param[robot_no].iToolPeriod = period;
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_SET_ZCSCARA_PERIOD;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设定ZCSCARA机器人为左手系.
 * 
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_zcscara_elbow_left(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_SET_ZCSCARA_ELBOW_LEFT;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}

/**
 * 设定ZCSCARA机器人为右手系.
 *
 * \param robot_no 机器人编号: 从0开始
 * \return 成功返回0, 失败返回-1或者错误码
 */
SOFTMOTION_API ET_RESULT smc_robot_set_zcscara_elbow_right(int robot_no) {
	ET_RESULT result = check_robot_no(robot_no);
	if (result != ET_RESULT_NO_ERROR) {
		return result;
	}

	result = preprocess_robot_command(robot_no);
	if (result == ET_RESULT_NO_ERROR) {
		const std::lock_guard<std::mutex> locker(m_mutex);
		m_robot[robot_no].stI.etRunMode = ET_ROBOTMODULE_CMD_SET_ZCSCARA_ELBOW_RIGHT;
		m_robot[robot_no].stI.xStart = true;
	}

	if (result == ET_RESULT_NO_ERROR) {
		result = smc_write_robotctrl_input(robot_no);
	}

	return result;
}