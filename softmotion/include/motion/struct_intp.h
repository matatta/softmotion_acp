#pragma once
#include <stdint.h>
#include "struct_robot.h"

// 控制模式
enum ET_INTPMODULE_CMD : int16_t {
	ET_INTPMODULE_CMD_INVALID = 0,	// 无效
	ET_INTPMODULE_CMD_MOVEABS = 1,	// 绝对定位
	ET_INTPMODULE_CMD_MOVEREL = 4,	// 相对定位
	ET_INTPMODULE_CMD_HALT = 8		// 停止
};

// 3D向量
typedef struct _smc_vector3d {
	double dX;
	double dY;
	double dZ;
} SMC_VECTOR3D;

// XYZ模块参数
typedef struct _intp_xyz_param {
	int16_t iIntpAxisNo_X;			// X轴编号
	int16_t iIntpAxisNo_Y;			// Y轴编号
	int16_t iIntpAxisNo_Z;			// Z轴编号
	int16_t iIntpAxisNo_C;			// C轴编号
	int16_t aiHomeSequence[4];		// 轴回零顺序
} INTP_XYZ_PARAM;

// XYZ模块输入结构体
typedef struct _intp_xyz_inputs {
	bool xPowerOn;					// 使能
	bool xStart;					// 启动
	bool xReset;					// 复位
	ET_INTPMODULE_CMD etRunMode;	// 控制模式
	MC_COORD_REF stPosition;		// 目标位置	
	SMC_VECTOR3D stToolOffset;		// 工具偏移量	
	double rVelocity;				// 目标速度
	double rAcceleration;			// 运行加速度
	double rDeceleration;			// 运行减速度
	double rJerk;					// 运行跃度
} INTP_XYZ_INPUTS;

// XYZ模块输出结构体
typedef struct _intp_xyz_outputs {
	bool xInRun;					// 使能
	bool xReady;					// 准备好
	ET_INTPMODULE_CMD etActiveMode;	// 当前控制模式
	bool xDone;						// 命令执行完成
	bool xBusy;						// 命令执行中
	bool xCommandAborted;			// 命令取消
	bool xError;					// 命令执行失败
//	uint16_t etErrorId;             // 命令执行错误代码
} INTP_XYZ_OUTPUTS;

// 轴控接口
typedef struct _intp_xyz_interface {
	INTP_XYZ_INPUTS stI;			// 轴输入结构体
	INTP_XYZ_OUTPUTS stQ;			// 轴输出结构体
} INTP_XYZ_INTERFACE;