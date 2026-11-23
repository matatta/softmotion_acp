#pragma once

#ifndef HANDLER_API
#define HANDLER_API __declspec(dllexport)
#else
#define HANDLER_API __declspec(dllimport)
#endif

#include <stdint.h>

// 控制命令 - 料仓
enum ET_SILO_CMD : int16_t {
	EM_SILO_INVALID = 0x00,				// 无效
	EM_SILO_HOME = 0x01,				// 料仓回零
	EM_SILO_GOTO_STANDBY = 0x02,		// 回等待位
	EM_SILO_REPEAT = 0x03,				// 料仓往返
	EM_SILO_LOAD = 0x04,				// 料仓上料
	EM_SILO_RELEASE = 0x05,				// 释放气缸
	EM_SILO_CLAMP = 0x06,				// 夹盘
	EM_SILO_UNLOAD = 0x07,				// 料仓下料
	EM_SILO_RESET = 0x08				// 故障复位
};

// 故障代码 - 料仓
enum ET_SILO_ERROR : int16_t {
	EM_SILO_NO_ERROR = 0x00,			// 料仓: 无故障
	EM_SILO_DRAWER_OPEND_ERROR = 0x01,	// 料仓: 抽屉未关闭故障
	EM_SILO_LACK_BASKET_ERROR = 0x02,	// 料仓: 未检测到料仓故障
	EM_SILO_CYLINDER_ERROR = 0x03,		// 料仓: 气缸故障
	EM_SILO_BOTTOM_DETECT_ERROR = 0x04,	// 料仓: 走到顶没碰到料仓下对射故障
	EM_SILO_TOP_DETECT_ERROR = 0x05,	// 料仓: 走到顶没碰到料仓上对射故障
	EM_SILO_AXIS_ERROR = 0x06,			// 料仓: 轴故障
	EM_SILO_AXIS_NOT_HOMED = 0x07,		// 料仓: 轴未回零
	EM_SILO_LOAD_FULL_PLATE = 0x08,		// 料仓: 上料时料盘有料
	EM_SILO_LACK_PLATE = 0x10,			// 料仓: 缺料
	EM_SILO_FULL_PLATE = 0x11,			// 料仓: 满料
	EM_SILO_TOP_PLATE_REMOVED = 0x12,	// 料仓: 被拿走了一个盘
	EM_SILO_TOP_PLATE_PLACED = 0x13		// 料仓: 被放置了一个盘
};

// 状态 - 料仓
enum ET_SILO_STATE : int16_t {
	EM_SILO_WAIT_CMD = 0x00,			// 等待命令
	EM_SILO_HOMING = 0x01,				// 回零中
	EM_SILO_TO_STANDBY = 0x02,			// 回等待位中
	EM_SILO_REPEATING = 0x03,			// 料仓往返中
	EM_SILO_LOADING = 0x04,				// 上料
	EM_SILO_RELEASING = 0x05,			// 松开气缸
	EM_SILO_CLAMPING = 0x06,			// 夹盘气缸
	EM_SILO_UNLOADING = 0x07,			// 下料
	EM_SILO_STOPPING = 0x08,			// 停止
	EM_SILO_ERROR = 0x09				// 故障
};

// 结果 - 料仓
enum ET_SILO_RESULT : int16_t {
	EM_SILO_RESULT_INVALID = 0x00,				// 无效
	EM_SILO_RESULT_SUCCEED = 0x01,				// 成功
	EM_SILO_RESULT_FAILED = 0x02				// 失败
};

#ifdef __cplusplus
extern "C" {
#endif
	HANDLER_API bool hdl_set_silo_command(int no, ET_SILO_CMD cmd);
	HANDLER_API bool hdl_get_silo_result(int no, ET_SILO_RESULT &result);
	HANDLER_API bool hdl_get_silo_state(int no, ET_SILO_STATE &state);

	HANDLER_API bool hdl_set_silo_param(int no, double max_dist, double thickness, double vel_h, double vel_l);
	HANDLER_API bool hdl_get_silo_param(int no, double &max_dist, double &thickness, double &vel_h, double &vel_l);	

#ifdef __cplusplus
}
#endif