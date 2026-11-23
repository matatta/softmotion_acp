#pragma once
#include <stdint.h>

// 控制模式
enum ET_AXISMODULE_CMD : int16_t {
	ET_AXISMODULE_CMD_INVALID = 0,				// 无效
	ET_AXISMODULE_CMD_MOVEABS = 1,				// 绝对定位(单轴/轴组): 单轴支持中途变速
	ET_AXISMODULE_CMD_MOVEREL = 4,				// 相对定位(单轴/轴组)
	ET_AXISMODULE_CMD_MOVEVEL = 7,				// 速度模式(单轴): csp模式,支持连续变速
	ET_AXISMODULE_CMD_HALT = 8,					// 停止(单轴/轴组) 

	ET_AXISMODULE_CMD_DRV_HOME = 10,			// 驱动器回零(单轴/轴组)
	ET_AXISMODULE_CMD_SMC_HOME = 11,			// 控制器回零(单轴) - 原点接IO
	ET_AXISMODULE_CMD_SETPOS_ABS = 12,			// 位置设定(单轴/轴组) - 绝对位置
	ET_AXISMODULE_CMD_SETPOS_REL = 13,			// 位置设定(单轴/轴组) - 相对位置	
	ET_AXISMODULE_CMD_JOG = 14,					// 点动(单轴/轴组)

	ET_AXISMODULE_CMD_VELOCITY = 15,			// CSV模式 - 扭矩限制 - 待实现
	ET_AXISMODULE_CMD_TORQUE = 16,				// CST模式 - 速度限制 - 待实现
//	ET_AXISMODULE_CMD_GEAR	= 17,				// 电子齿轮 - 待实现
//	ET_AXISMODULE_CMD_GEARINPOS = 18,			// 齿轮同步 - 待实现
//	ET_AXISMODULE_CMD_CAM = 19,					// 电子凸轮 - 待实现	

	ET_AXISMODULE_CMD_REPEATABS = 21,			// 绝对往复定位(单轴/轴组)
	ET_AXISMODULE_CMD_REPEATREL = 24,			// 相对往复定位(单轴/轴组)

	ET_AXISMODULE_CMD_SET_SOFTWARE_LIMITS = 40,	// 软限位设定(单轴)
	ET_AXISMODULE_CMD_SET_DYNAMIC_LIMITS = 41,	// 动态限制设定(单轴)
	ET_AXISMODULE_CMD_SET_SDO_PARAMS = 42,		// SDO参数设定(单轴)	
	ET_AXISMODULE_CMD_GET_SDO_PARAMS = 43		// SDO参数读取(单轴)
};

// 缓冲模式
enum MC_BUFFER_MODE : int16_t {
	MC_BUFFER_MODE_ABORTING = 0,				// Start FB immediately (default mode)
	MC_BUFFER_MODE_BUFFERED = 1,				// Start FB after the currently last motion in queue has finished
	MC_BUFFER_MODE_BLENDING_LOW = 2,			// The path is blended with the minimum of the dynamic limits provided by both FBs
	MC_BUFFER_MODE_BLENDING_PREVIOUS = 3,		// The path is blended with the dynamic limits of the first FB
	MC_BUFFER_MODE_BLENDING_NEXT = 4,			// The path is blended with the dynamic limits of the second FB
	MC_BUFFER_MODE_BLENDING_HIGH = 5			// The path is blended with the maximum of the dynamic limits provided by both FBs
};

// 方向
enum MC_DIRECTION : int16_t {
	MC_DIRECTION_FASTEST = 3,
	MC_DIRECTION_CURRENT = 2,
	MC_DIRECTION_POSITIVE = 1,
	MC_DIRECTION_SHORTEST = 0,
	MC_DIRECTION_NEGATIVE = -1
};

// 回零模式
enum SMC_HOME_MODE : int16_t {
	SMC_HOME_FAST_NEG_BSLOW_S_STOP = 0,
	SMC_HOME_FAST_NEG_BSLOW_STOP_S = 1,
	SMC_HOME_FAST_NEG_BSLOW_I_S_STOP = 2,
	SMC_HOME_FAST_NEG_SLOW_S_STOP = 4,
	SMC_HOME_FAST_NEG_SLOW_STOP_S = 5,
	SMC_HOME_FAST_NEG_SLOW_I_S_STOP = 6,

	SMC_HOME_FAST_POS_BSLOW_S_STOP = 10,
	SMC_HOME_FAST_POS_BSLOW_STOP_S = 11,
	SMC_HOME_FAST_POS_BSLOW_I_S_STOP = 12,
	SMC_HOME_FAST_POS_SLOW_S_STOP = 14,
	SMC_HOME_FAST_POS_SLOW_STOP_S = 15,
	SMC_HOME_FAST_POS_SLOW_I_S_STOP = 16	
};

// 轴状态
enum SMC_AXIS_STATE : int16_t {
	SMC_AXIS_STATE_POWER_OFF = 0,			// 未使能
	SMC_AXIS_STATE_ERRORSTOP = 1,			// 故障停止
	SMC_AXIS_STATE_STOPPING = 2,			// 急停触发
	SMC_AXIS_STATE_STANDSTILL = 3,			// 待机
	SMC_AXIS_STATE_DISCRETE_MOTION = 4,		// PTP运动中
	SMC_AXIS_STATE_CONTINUES_MOTION = 5,	// 连续运动中(电子凸轮/齿轮从轴)
	SMC_AXIS_STATE_SYNCHRONIZED_MOTION = 6,	// 同步运动中
	SMC_AXIS_STATE_HOMING = 7				// 回原点中
};

// 驱动器IO输入
typedef struct _axis_digital_inputs {
	bool xDINegativeLimit;			// 负限位
	bool xDIPositiveLimit;			// 正限位
	bool xDIHomeSwitch;				// 原点开关
	bool xDIQuickStop;				// 急停
	bool xDIVoltageOk;				// 使能电压
	bool xDIBrakeReleased;			// 抱闸反馈
	bool xDIWarning;				// 警告
	bool xDIReady;					// 驱动器就绪
	bool xDIFault;					// 驱动器故障
} AXIS_DIGITAL_INPUTS;

// 轴信息
typedef struct _axis_information {	
	bool xRotary;					// 旋转轴
	bool xModulo;					// 模数轴
	double rPositionPeriod;			// 模数

	double rSWMaxVelocity;			// 动态限制(读) - 最大速度
	double rSWMaxAcceleration;		// 动态限制(读) - 最大加速度
	double rSWMaxDeceleration;		// 动态限制(读) - 最大减速度
	double rSWMaxJerk;				// 动态限制(读) - 最大跃度
	double rSWMaxTorque;			// 动态限制(读) - 最大扭矩

	bool xHWLimitEnable;			// 硬限位开启(读)
	bool xSWLimitEnable;			// 软限位开启(读)
	double rSWLimitPositive;		// 软限位(读) - 正限位
	double rSWLimitNegative;		// 软限位(读) - 负限位
	double rSWLimitDeceleration;	// 软限位(读) - 减速度
	double rSWLErrorMaxDistance;	// 软限位(读) - 故障停机距离
} AXIS_INFORMATION;

// 轴参数
typedef struct _axis_module_param {
	int16_t nAxisId;							// 轴编号 - 只读

	double rInchDistance;						// 寸动距离
	double rJogVelocity;						// 点动速度
	double rJogAcceleration;					// 点动加速度
	double rJogDeceleration;					// 点动减速度
	double rJogJerk;							// 点动跃度	
	
	double rRepeatStartPos;						// 往复运动 起始位置
	double rRepeatEndPos;						// 往复运动 终点位置
	double rRepeatVelocity;						// 往复运动 速度
	double rRepeatAcceleration;					// 往复运动 加速度
	double rRepeatDeceleration;					// 往复运动 减速度
	double rRepeatJerk;							// 往复运动 跃度
	int16_t iRepeatCycle;						// 往复运动 总次数
	int16_t iRepeatCount;						// 往复运动 计数
	int16_t iRepeatIdleTime;					// 往复运动 间隔时间(ms)

	double rEStopRamp;							// 急停减速度
	double rEStopJerk;							// 急停跃度
	int16_t iEStopModuleNo;						// 急停开关 - 模块编号
	int16_t iEStopIoNo;							// 急停开关 - 模块上输入点编号

	double rHomePosition;						// 回零位置
	double rHomeVelocitySlow;					// 回零速度 - 慢速
	double rHomeVelocityFast;					// 回零速度 - 快速
	double rHomeAcceleration;					// 回原点加速度
	double rHomeDeceleration;					// 回原点减速度
	double rHomeJerk;							// 回原点跃度

	bool xEnHomeSDOWrite;						// 驱动器回零：允许SDO写入回零参数
	bool xEnHomeConvUserUnit;					// 驱动器回零：SDO写入启用用户单位转换
	uint16_t iDrvHomeMode;						// 驱动器回零: 回零模式

	SMC_HOME_MODE etSmcHomeMode;				// 控制器回零：回零模式
	double rSmcSignalDelay;						// 控制器回零：原点开关信号延时(s)
	MC_DIRECTION etSmcHomeDirection;			// 控制器回零：方向
	bool xSmcHomeReturnToZero;					// 控制器回零：完成后移动到零位
	bool xSmcHomeIndexOccured;					// 控制器回零：零位脉冲信号
	double rSmcHomeIndexPosition;				// 控制器回零：零位脉冲位置
	bool xSmcHomeIgnoreHWLimit;					// 控制器回零：过程中忽略软限位
	int16_t iSmcHomeSwitchModuleNo;				// 控制器原点开关 - 模块编号
	int16_t iSmcHomeSwitchIoNo;					// 控制器原点开关 - 模块上输入点编号
	int16_t iSmcHomeIndexModuleNo;				// 控制器电机信号索引 - 模块编号
	int16_t iSmcHomeIndexIoNo;					// 控制器电机信号索引 - 模块上输入点编号
	
	double rRefSWMaxVelocity;					// 动态限制(写) - 最大速度
	double rRefSWMaxAcceleration;				// 动态限制(写) - 最大加速度
	double rRefSWMaxDeceleration;				// 动态限制(写) - 最大减速度
	double rRefSWMaxJerk;						// 动态限制(写) - 最大跃度
	double rRefSWMaxTorque;						// 动态限制(写) - 最大扭矩

	bool xRefSWLimitEnable;						// 开启软限位(写)
	double rRefSWLimitPositive;					// 软限位(写) - 正限位
	double rRefSWLimitNegative;					// 软限位(写) - 负限位
	double rRefSWLimitDeceleration;				// 软限位(写) - 减速度
	double rRefSWLErrorMaxDistance;				// 软限位(写) - 故障停机距离: 必须为正
	
	double lrSdoSetData;						// SDO数据内容(写)
	double lrSdoGetData;						// SDO数据内容(读)
} AXIS_MODULE_PARAM;

// 轴输入结构体
typedef struct _axis_module_inputs {
	bool xEnable;						// 轴使能
	bool xStart;						// 启动
	ET_AXISMODULE_CMD etRunMode;		// 控制模式
	bool xReset;						// 故障复位
	bool xEStop;						// 急停
	double rPosition;					// 目标位置
	double rVelocity;					// 目标速度
	double rAcceleration;				// 运行加速度
	double rDeceleration;				// 运行减速度
	double rJerk;						// 运行跃度
	MC_BUFFER_MODE etBufferMode;		// 缓冲模式	
	MC_DIRECTION etAbsDirection;		// 模数轴运动方向
	bool xInchMode;						// 寸动使能
	bool xJogForward;					// 点动前进
	bool xJogBackward;					// 点动后退
	bool xSmcHomeSwitch;				// 控制器原点开关
	bool xSmcHomeIndex;					// 控制器原点电机信号索引
	uint16_t iAxisSubIndex;				// 驱动器下轴编号
	uint16_t iSdoRwIndex;				// SDO索引
	uint8_t iSdoRwSubIndex;				// SDO子索引
	uint8_t iSdoRwDataLen;				// SDO数据长度 - 字节
} AXIS_MODULE_INPUTS;

// 轴输出结构体
typedef struct _axis_module_outputs {
	int16_t nAxisId;					// 轴编号
	bool xVirtual;						// 虚轴
	bool xCommOK;						// 通讯正常指示
	bool xInRun;						// 使能状态
	ET_AXISMODULE_CMD etActiveMode;		// 当前控制模式
	bool xRefOK;						// 回原点完成
	bool xStopped;						// 轴停止状态
	bool xHoming;						// 回原点中
	bool xSynchronized;					// 轴同步运行状态
	bool xDone;							// 命令执行完成
	bool xBusy;							// 命令执行中
//	bool xActive;						// 命令执行已激活
//	bool xCommandAborted;				// 命令被打断
	bool xError;						// 命令执行失败
	uint16_t etErrorId;					// 命令执行错误代码
	bool xAxisAlarm;					// 轴报警
	uint32_t dwAxisAlarmID;				// 轴报警ID
	SMC_AXIS_STATE etAxisState;			// 轴运动状态
//	ST_STRING sAxisStateExt[81];		// 轴运动状态
	double rActPosition;				// 轴当前位置
	double rActVelocity;				// 轴当前速度
	double rActTorque;					// 轴当前扭矩
	AXIS_INFORMATION stInformation;		// 轴信息
	AXIS_DIGITAL_INPUTS stDigitalInputs;// 驱动器输入	
} AXIS_MODULE_OUTPUTS;

// 轴控接口
typedef struct _axis_control_interface {
	AXIS_MODULE_INPUTS stI;				// 轴输入结构体
	AXIS_MODULE_OUTPUTS stQ;			// 轴输出结构体
} AXIS_MODULE_INTERFACE;