#pragma once

#ifndef SOFTMOTION_API
#define SOFTMOTION_API __declspec(dllexport)
#else
#define SOFTMOTION_API __declspec(dllimport)
#endif

#include "acp_result.h"
#include "struct_io.h"
#include "struct_axis.h"
#include "struct_robot.h"
#include "struct_intp.h"

#ifdef __cplusplus
extern "C" {
#endif
	/* 连接及初始化 */
	SOFTMOTION_API bool smc_open(const char* ip_addr, int mode = 0);
	SOFTMOTION_API void smc_close();
	SOFTMOTION_API bool smc_connected_status();

	/* 变量读写 */
	SOFTMOTION_API ET_RESULT smc_var_get_value(const char* name, int size, void* content, int timeout = 10);
	SOFTMOTION_API ET_RESULT smc_var_set_value(const char* name, int size, void* content, int timeout = 10);

	/* IO控制 */	
	SOFTMOTION_API ET_RESULT smc_io_get_di(uint16_t module_no, uint16_t io_no, bool &value);
	SOFTMOTION_API ET_RESULT smc_io_get_di_32bit(uint16_t module_no, uint32_t &value);
	SOFTMOTION_API ET_RESULT smc_io_get_ai_16bit(uint16_t module_no, uint16_t io_no, int16_t *value);
	SOFTMOTION_API ET_RESULT smc_io_get_ai_32bit(uint16_t module_no, uint16_t io_no, int32_t *value);
	
	SOFTMOTION_API ET_RESULT smc_io_set_dq(uint16_t module_no, uint16_t io_no, bool value);
	SOFTMOTION_API ET_RESULT smc_io_set_dq_reverse(uint16_t module_no, uint16_t io_no, uint32_t time_ms, int16_t num = 1);
	SOFTMOTION_API ET_RESULT smc_io_set_dq_32bit(uint16_t module_no, uint32_t value);
	SOFTMOTION_API ET_RESULT smc_io_set_aq_16bit(uint16_t module_no, uint16_t io_no, int16_t value);
	SOFTMOTION_API ET_RESULT smc_io_set_aq_32bit(uint16_t module_no, uint16_t io_no, int32_t value);
		
	SOFTMOTION_API ET_RESULT smc_io_get_dq(uint16_t module_no, uint16_t io_no, bool &value);
	SOFTMOTION_API ET_RESULT smc_io_get_dq_32bit(int module_no, uint32_t &value);
	SOFTMOTION_API ET_RESULT smc_io_get_aq_16bit(uint16_t module_no, uint16_t io_no, int16_t *value);
	SOFTMOTION_API ET_RESULT smc_io_get_aq_32bit(uint16_t module_no, uint16_t io_no, int32_t *value);
	SOFTMOTION_API ET_RESULT smc_io_get_total_inout(IO_CONTROL_INTERFACE &value);

	/* 单轴状态反馈 */
	SOFTMOTION_API ET_RESULT smc_axis_common_status(int axis_no, int16_t &axis_id, bool &virt, bool &comm_status, bool &power_status, bool &ref_status);
	SOFTMOTION_API ET_RESULT smc_axis_motion_status(int axis_no, SMC_AXIS_STATE &state, bool &stopped, bool &homing, bool &sync);
	SOFTMOTION_API ET_RESULT smc_axis_alarm_status(int axis_no, bool &alarm, uint32_t &alarm_id);
	SOFTMOTION_API ET_RESULT smc_axis_control_status(int axis_no, ET_AXISMODULE_CMD &status);
	SOFTMOTION_API ET_RESULT smc_axis_command_result(int axis_no, bool *done = nullptr, bool *busy = nullptr, bool *error = nullptr, uint16_t *error_id = nullptr);
	SOFTMOTION_API ET_RESULT smc_axis_actual_position(int axis_no, double &value);
	SOFTMOTION_API ET_RESULT smc_axis_actual_velocity(int axis_no, double &value);
	SOFTMOTION_API ET_RESULT smc_axis_actual_torque(int axis_no, double &value);
	SOFTMOTION_API ET_RESULT smc_axis_total_output(int axis_no, AXIS_MODULE_OUTPUTS &value);
	
	/* 单轴参数设置 */
	SOFTMOTION_API ET_RESULT smc_axis_set_profile_position(int axis_no, double value);
	SOFTMOTION_API ET_RESULT smc_axis_set_profile_velocity(int axis_no, double value);
	SOFTMOTION_API ET_RESULT smc_axis_set_profile_ramp(int axis_no, double acc = 0, double dec = 0, double jerk = 0);
	SOFTMOTION_API ET_RESULT smc_axis_set_profile_buffermode(int axis_no, MC_BUFFER_MODE value);
	SOFTMOTION_API ET_RESULT smc_axis_get_profile_setting(int axis_no, double &pos, double &vel, double &acc, double &dec, double &jerk, MC_BUFFER_MODE &buffermode);

	SOFTMOTION_API ET_RESULT smc_axis_set_estop_ramp(int axis_no, double dec = 0, double jerk = 0);
	SOFTMOTION_API ET_RESULT smc_axis_set_estop_io(int axis_no, int16_t module_no = -1, int16_t io_no = -1);
	SOFTMOTION_API ET_RESULT smc_axis_get_estop_ramp(int axis_no, double &dec, double &jerk);
	SOFTMOTION_API ET_RESULT smc_axis_get_estop_io(int axis_no, int16_t &module_no, int16_t &io_no);

	SOFTMOTION_API ET_RESULT smc_axis_set_jog_velocity(int axis_no, double value);
	SOFTMOTION_API ET_RESULT smc_axis_set_jog_ramp(int axis_no, double acc = 0, double dec = 0, double jerk = 0);
	SOFTMOTION_API ET_RESULT smc_axis_get_jog_param(int axis_no, double &inch_dist, double &vel, double &acc, double &dec, double &jerk);
	
	SOFTMOTION_API ET_RESULT smc_axis_set_home_position(int axis_no, double value);
	SOFTMOTION_API ET_RESULT smc_axis_set_home_velocity(int axis_no, double low = 0, double high = 0);
	SOFTMOTION_API ET_RESULT smc_axis_set_home_ramp(int axis_no, double acc = 0, double dec = 0, double jerk = 0);
	SOFTMOTION_API ET_RESULT smc_axis_get_home_param(int axis_no, double &pos, double &low_vel, double &high_vel, double &acc, double &dec, double &jerk);

	SOFTMOTION_API ET_RESULT smc_axis_set_drv_home_mode(int axis_no, uint16_t value);
	SOFTMOTION_API ET_RESULT smc_axis_set_drv_home_param(int axis_no, bool en_sdo = true, bool en_conv = false);
	SOFTMOTION_API ET_RESULT smc_axis_get_drv_home_param(int axis_no, uint16_t &mode, bool &en_sdo, bool &en_conv);
	SOFTMOTION_API ET_RESULT smc_axis_get_drv_inputs(int axis_no, AXIS_DIGITAL_INPUTS &value);

	SOFTMOTION_API ET_RESULT smc_axis_set_plc_home_mode(int axis_no, SMC_HOME_MODE value);
	SOFTMOTION_API ET_RESULT smc_axis_set_plc_home_param(int axis_no, double signal_delay, double index_position, bool return_to_zero, bool ignore_hw_limits);
	SOFTMOTION_API ET_RESULT smc_axis_set_plc_home_io(int axis_no, int16_t switch_module, int16_t switch_io, int16_t index_module, int16_t index_no);
	SOFTMOTION_API ET_RESULT smc_axis_get_plc_home_param(int axis_no, SMC_HOME_MODE &mode, double &signal_delay, double &index_position, bool &return_to_zero, bool &ignore_hw_limits);
	SOFTMOTION_API ET_RESULT smc_axis_get_plc_home_io(int axis_no, int16_t &switch_module, int16_t &switch_io, int16_t &index_module, int16_t &index_no);

	/* 单轴控制 */
	SOFTMOTION_API ET_RESULT smc_axis_power_on(int axis_no);
	SOFTMOTION_API ET_RESULT smc_axis_power_off(int axis_no);
	SOFTMOTION_API ET_RESULT smc_axis_move_absolute(int axis_no, double position, MC_DIRECTION module_direction = MC_DIRECTION_SHORTEST);
	SOFTMOTION_API ET_RESULT smc_axis_move_relative(int axis_no, double distance);
	SOFTMOTION_API ET_RESULT smc_axis_move_velocity(int axis_no, double velocity);
	SOFTMOTION_API ET_RESULT smc_axis_halt(int axis_no);
	SOFTMOTION_API ET_RESULT smc_axis_reset(int axis_no);
	SOFTMOTION_API ET_RESULT smc_axis_estop(int axis_no, bool enable);

	SOFTMOTION_API ET_RESULT smc_axis_jog_forward(int axis_no, bool enable);
	SOFTMOTION_API ET_RESULT smc_axis_jog_backward(int axis_no, bool enable);
	SOFTMOTION_API ET_RESULT smc_axis_inch_forward(int axis_no, bool enable, double distance);
	SOFTMOTION_API ET_RESULT smc_axis_inch_backward(int axis_no, bool enable, double distance);
	SOFTMOTION_API ET_RESULT smc_axis_drv_home(int axis_no, uint16_t mode);
	SOFTMOTION_API ET_RESULT smc_axis_plc_home(int axis_no, SMC_HOME_MODE mode);
	SOFTMOTION_API ET_RESULT smc_axis_setposition(int axis_no, double position = 0, bool relative = false);	

	/* 单轴控制 - 扩展功能 */
	SOFTMOTION_API ET_RESULT smc_axis_set_sdo_param(int axis_no, uint16_t index, uint8_t subindex, uint8_t datalen, double value);
	SOFTMOTION_API ET_RESULT smc_axis_get_sdo_param(int axis_no, uint16_t index, uint8_t subindex, uint8_t datalen, double &value);
	SOFTMOTION_API ET_RESULT smc_axis_get_information(int axis_no, AXIS_INFORMATION &value);
	SOFTMOTION_API ET_RESULT smc_axis_set_dynamic_limits(int axis_no, double max_vel = 0, double max_acc = 0, double max_dec = 0, double max_jerk = 0, double max_torque = 0);
	SOFTMOTION_API ET_RESULT smc_axis_set_software_limits(int axis_no, double pos_limit, double neg_limit, double err_dec, double err_max_distance, bool active = false);
	
	/* 机器人控制 */
	SOFTMOTION_API ET_RESULT smc_robot_common_status(int robot_no, bool &enabled, bool &comm_status, bool &power_status, bool &ref_status);
	SOFTMOTION_API ET_RESULT smc_robot_motion_status(int robot_no, SMC_ROBOT_STATE &state, bool &stopped, bool &homing, bool &sync, bool &tracking);
	SOFTMOTION_API ET_RESULT smc_robot_alarm_status(int robot_no, bool &alarm, uint32_t &alarm_id);	
	SOFTMOTION_API ET_RESULT smc_robot_control_status(int robot_no, ET_ROBOTMODULE_CMD &status);
	SOFTMOTION_API ET_RESULT smc_robot_command_result(int robot_no, bool *done = nullptr, bool *busy = nullptr, bool *error = nullptr, uint16_t *error_id = nullptr, bool *active = nullptr, bool *cmd_aborted = nullptr, bool *cmd_accepted = nullptr);
	SOFTMOTION_API ET_RESULT smc_robot_actual_position(int robot_no, ROBOT_MODULE_COORD &value);
	SOFTMOTION_API ET_RESULT smc_robot_actual_velocity(int robot_no, ROBOT_MODULE_COORD &value);
	SOFTMOTION_API ET_RESULT smc_robot_total_output(int robot_no, ROBOT_MODULE_OUTPUTS &value);

	SOFTMOTION_API ET_RESULT smc_robot_set_profile_circular(int robot_no, SMC_CIRC_MODE circ_mode, MC_CIRC_PATHCHOICE circ_path);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_coordinate(int robot_no, SMC_COORD_SYSTEM &value);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_position(int robot_no, SMC_POS_REF value);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_velocity(int robot_no, double value);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_ramp(int robot_no, double acc = 0, double dec = 0, double jerk = 0);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_factor(int robot_no, double vel_factor = 1.0, double acc_factor = 1.0, double jerk_factor = 1.0, double torque_factor = 1.0);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_buffermode(int robot_no, MC_BUFFER_MODE value);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_transmode_none(int robot_no);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_transmode_start_velocity(int robot_no, double vel_factor, bool blend_with_aux = false);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_transmode_corner_distance(int robot_no, double distance);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_transmode_axis_distance(int robot_no, double line_dist, double ang_dist, bool euc_done, bool blend_with_aux = false);
	SOFTMOTION_API ET_RESULT smc_robot_set_profile_orienmode(int robot_no, SMC_ORIENTATION_MODE mode);

	SOFTMOTION_API ET_RESULT smc_robot_get_profile_circular(int robot_no, SMC_CIRC_MODE &circ_mode, MC_CIRC_PATHCHOICE &circ_path);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_coordinate(int robot_no, SMC_COORD_SYSTEM &value);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_position(int robot_no, SMC_POS_REF &value);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_aux_position(int robot_no, SMC_POS_REF &value);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_velocity(int robot_no, double &value);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_ramp(int robot_no, double &acc, double &dec, double &jerk);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_factor(int robot_no, double &vel_factor, double &acc_factor, double &jerk_factor, double &torque_factor);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_buffermode(int robot_no, MC_BUFFER_MODE &value);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_transmode(int robot_no, MC_TRANSITION_MODE &value);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_transmode_param(int robot_no, double &vel_factor, double &corner_dist, double &line_dist, double &ang_dist, bool &euc_done, bool &blend_with_aux);
	SOFTMOTION_API ET_RESULT smc_robot_get_profile_orienmode(int robot_no, SMC_ORIENTATION_MODE &value);

	SOFTMOTION_API ET_RESULT smc_robot_set_estop_ramp(int robot_no, double dec, double jerk);
	SOFTMOTION_API ET_RESULT smc_robot_set_estop_factor(int robot_no, double dec_factor = 1.0, double jerk_factor = 1.0, double torque_factor = 1.0);
	SOFTMOTION_API ET_RESULT smc_robot_set_estop_clear_movement(int robot_no, bool value);
	SOFTMOTION_API ET_RESULT smc_robot_set_estop_io(int robot_no, int16_t module_no, int16_t io_no);
	SOFTMOTION_API ET_RESULT smc_robot_get_estop_ramp(int robot_no, double &dec, double &jerk);
	SOFTMOTION_API ET_RESULT smc_robot_get_estop_factor(int robot_no, double &dec_factor, double &jerk_factor, double &torque_factor);
	SOFTMOTION_API ET_RESULT smc_robot_get_estop_clear_movement(int robot_no, bool &value);
	SOFTMOTION_API ET_RESULT smc_robot_get_estop_io(int robot_no, int16_t &module_no, int16_t &io_no);

	SOFTMOTION_API ET_RESULT smc_robot_set_jog_limit(int robot_no, double line_dist, double ang_dist);
	SOFTMOTION_API ET_RESULT smc_robot_set_jog_velocity(int robot_no, double value);
	SOFTMOTION_API ET_RESULT smc_robot_set_jog_ramp(int robot_no, double acc = 0, double dec = 0, double jerk = 0);
	SOFTMOTION_API ET_RESULT smc_robot_set_jog_factor(int robot_no, double vel_factor = 1.0, double acc_factor = 1.0, double jerk_factor = 1.0, double torque_factor = 1.0);
	SOFTMOTION_API ET_RESULT smc_robot_get_jog_param(int robot_no, double &line_dist, double &ang_dist, double &vel, double &acc, double &dec, double &jerk);
	SOFTMOTION_API ET_RESULT smc_robot_get_jog_factor(int robot_no, double &vel_factor, double &acc_factor, double &jerk_factor, double &torque_factor);

	SOFTMOTION_API ET_RESULT smc_robot_enable(int robot_no);
	SOFTMOTION_API ET_RESULT smc_robot_disable(int robot_no);
	SOFTMOTION_API ET_RESULT smc_robot_power_on(int robot_no);
	SOFTMOTION_API ET_RESULT smc_robot_power_off(int robot_no);
	SOFTMOTION_API ET_RESULT smc_robot_ptp_absolute(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF position);
	SOFTMOTION_API ET_RESULT smc_robot_ptp_relative(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF distance);
	SOFTMOTION_API ET_RESULT smc_robot_linear_absolute(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF position);
	SOFTMOTION_API ET_RESULT smc_robot_linear_relative(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF distance);
	SOFTMOTION_API ET_RESULT smc_robot_circ_absolute(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF pos, SMC_POS_REF aux_pos);
	SOFTMOTION_API ET_RESULT smc_robot_circ_relative(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF pos, SMC_POS_REF aux_pos);
	SOFTMOTION_API ET_RESULT smc_robot_halt(int robot_no, bool clear_movements = false);
	SOFTMOTION_API ET_RESULT smc_robot_reset(int robot_no);
	SOFTMOTION_API ET_RESULT smc_robot_estop(int robot_no, bool enable, bool clear_movements = false);

	SOFTMOTION_API ET_RESULT smc_robot_set_safe_height(int robot_no, double position);
	SOFTMOTION_API ET_RESULT smc_robot_get_safe_height(int robot_no, double &position);
	SOFTMOTION_API ET_RESULT smc_robot_ptp_jump(int robot_no, SMC_COORD_SYSTEM coord, MC_COORD_REF position);

	SOFTMOTION_API ET_RESULT smc_robot_jog_forward(int robot_no, SMC_COORD_SYSTEM coord, int axis_no, bool enable, bool abc_as_acs = false);
	SOFTMOTION_API ET_RESULT smc_robot_jog_backward(int robot_no, SMC_COORD_SYSTEM coord, int axis_no, bool enable, bool abc_as_acs = false);
	SOFTMOTION_API ET_RESULT smc_robot_setposition(int robot_no, SMC_COORD_SYSTEM coord, SMC_POS_REF position, bool relative = false);

	SOFTMOTION_API ET_RESULT smc_robot_get_axis_list(int robot_no, int16_t *axis_no, int16_t &num);
	SOFTMOTION_API ET_RESULT smc_robot_set_home_sequence(int robot_no, int16_t seq_index, int16_t seq_dat);
	SOFTMOTION_API ET_RESULT smc_robot_drv_home(int robot_no);
	SOFTMOTION_API ET_RESULT smc_robot_plc_home(int robot_no);

	SOFTMOTION_API ET_RESULT smc_robot_get_tool_offset(int robot_no, MC_COORD_REF &offset, SMC_READ_TOOL_MODE mode = SMC_READ_TOOL_LAST_SET);
	SOFTMOTION_API ET_RESULT smc_robot_set_tool_offset(int robot_no, MC_COORD_REF offset);

	SOFTMOTION_API ET_RESULT smc_robot_set_caxis_period(int robot_no, int period);
	SOFTMOTION_API ET_RESULT smc_robot_set_zcscara_period(int robot_no, int period);
	SOFTMOTION_API ET_RESULT smc_robot_set_zcscara_elbow_left(int robot_no);
	SOFTMOTION_API ET_RESULT smc_robot_set_zcscara_elbow_right(int robot_no);

#ifdef __cplusplus
}
#endif