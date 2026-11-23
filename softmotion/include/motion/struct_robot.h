#pragma once
#include "struct_axis.h"
#include <stdint.h>

// 控制模式
enum ET_ROBOTMODULE_CMD : int16_t {
	ET_ROBOTMODULE_CMD_INVALID = 0,						// 无效
	ET_ROBOTMODULE_CMD_MOVEABS = 1,						// 绝对定位(单轴/轴组): 单轴支持中途变速
	ET_ROBOTMODULE_CMD_MOVEABS_LINEAR = 2,				// 绝对定位(轴组) - 直线插补
	ET_ROBOTMODULE_CMD_MOVEABS_CIRCULAR = 3,			// 绝对定位(轴组) - 圆弧插补	
	ET_ROBOTMODULE_CMD_MOVEREL = 4,						// 相对定位(单轴/轴组)
	ET_ROBOTMODULE_CMD_MOVEREL_LINEAR = 5,				// 相对定位(轴组) - 直线插补
	ET_ROBOTMODULE_CMD_MOVEREL_CIRCULAR = 6,			// 相对定位(轴组) - 圆弧插补
	ET_ROBOTMODULE_CMD_HALT = 8,						// 停止(单轴/轴组) 	

	EM_ROBOTMODULE_CMD_DRV_HOME = 10,					// 驱动器回零(单轴/轴组)
	EM_ROBOTMODULE_CMD_SMC_HOME = 11,					// 驱动器回零(单轴/轴组)
	ET_ROBOTMODULE_CMD_SETPOS_ABS = 12,					// 位置设定(单轴/轴组) - 绝对位置
	ET_ROBOTMODULE_CMD_SETPOS_REL = 13,					// 位置设定(单轴/轴组) - 相对位置
	ET_ROBOTMODULE_CMD_JOG = 14,						// 点动(单轴/轴组)

	ET_ROBOTMODULE_CMD_JUMPABS = 20,					// PTP跳跃(轴组)
	ET_ROBOTMODULE_CMD_REPEATABS = 21,					// 绝对往复定位(单轴/轴组)
	ET_ROBOTMODULE_CMD_REPEATREL = 24,					// 相对往复定位(单轴/轴组)

	ET_ROBOTMODULE_CMD_SET_CAXIS_PERIOD = 50,			// 设定CAXIS A4旋转轴周期
	ET_ROBOTMODULE_CMD_SET_ZCSCARA_PERIOD = 51,			// 设定SCARA A4旋转轴周期
	ET_ROBOTMODULE_CMD_SET_ZCSCARA_ELBOW_LEFT = 52,		// 设定SCARA 左手系
	ET_ROBOTMODULE_CMD_SET_ZCSCARA_ELBOW_RIGHT = 53,	// 设定SCARA 右手系

	ET_ROBOTMODULE_CMD_SET_OVVERIDE = 60,				// 设定倍率(轴组)
	ET_ROBOTMODULE_CMD_SET_ANCILLARY_AXIS_LIMITS = 61,	// 设定轴动态限制(轴组)
	ET_ROBOTMODULE_CMD_SET_ANCILLARY_PATH_LIMITS = 62,	// 设定路径动态限制(轴组)
	ET_ROBOTMODULE_CMD_SET_AXIS_LIMIT_FACTORS = 63,		// 设定动态限制倍率(轴组)
	ET_ROBOTMODULE_CMD_GET_TOOL_OFFSET = 64,			// 读取工具偏移量
	ET_ROBOTMODULE_CMD_SET_TOOL_OFFSET = 65				// 设置工具偏移量	
};

// 机器人类型
enum ET_RobotType : int16_t {
	ET_ROBOTTYPE_INVALID = 0x00,

	ET_ROBOTTYPE_GANTRY_2 = 0x01,
	ET_ROBOTTYPE_GANTRY_3 = 0x02,
	ET_ROBOTTYPE_HGANTRY_2 = 0x03,
	ET_ROBOTTYPE_HGANTRY_3 = 0x04,
	ET_ROBOTTYPE_TGANTRY_2 = 0x05,
	ET_ROBOTTYPE_AXIS_5 = 0x06,

//	ET_ROBOTTYPE_SCARA_2 = 0x05,
//	ET_ROBOTTYPE_SCARA_3 = 0x06,
	ET_ROBOTTYPE_SCARA_4ZC = 0x07,
	ET_ROBOTTYPE_SCARA_4ZC_PLS = 0x08,
	
	EM_ROBOT_XXY = 0x10
};

// 坐标位置 - 笛卡尔坐标系
typedef struct _mc_coord_ref {
	double X;
	double Y;
	double Z;
	double A;
	double B;
	double C;
} MC_COORD_REF;

// 坐标位置 - 轴坐标系
typedef struct _axis_pos_ref {
	double a0;
	double a1;
	double a2;
	double a3;
	double a4;
	double a5;
} TRAFO_AXISPOS_REF;

// 轴组位置联合体
typedef union _smc_pos_ref {
	TRAFO_AXISPOS_REF a;
	MC_COORD_REF c;
	double v[6];
} SMC_POS_REF;

// 坐标位置(含附加轴) - 轴坐标系
typedef struct _axis_position_ext {
	double a0;
	double a1;
	double a2;
	double a3;
	double a4;
	double a5;

	double add0;
	double add1;
	double add2;
	double add3;
} AXIS_POSITION_EXT;

// 坐标位置 - 各坐标系下
typedef struct _robot_module_coord {
	AXIS_POSITION_EXT stValInAcs;	// 轴坐标系下
	MC_COORD_REF stValInMcs;		// 机械坐标系下
	MC_COORD_REF stValInWcs;		// 世界坐标系下	
	MC_COORD_REF stValInPcsPick;	// 产品坐标系下(抓取皮带)
	MC_COORD_REF stValInPcsPlace;	// 产品坐标系下(放置皮带)
	MC_COORD_REF stValInTcs;		// 工具坐标系下
} ROBOT_MODULE_COORD;

// 坐标系
enum SMC_COORD_SYSTEM : int16_t {
	SMC_COORD_ACS,			// 轴坐标系
	SMC_COORD_MCS,			// 机械坐标系
	SMC_COORD_WCS,			// 世界坐标系
	SMC_COORD_PCS_1,		// 产品坐标系(抓取皮带)
	SMC_COORD_PCS_2,		// 产品坐标系(放置皮带)
	SMC_COORD_TCS			// 工具坐标系
};

// 混合模式
enum MC_TRANSITION_MODE : int16_t {
	MC_TRANSITION_TMNONE,
	MC_TRANSITION_TMSTART_VELOCITY,
	MC_TRANSITION_TMCORNER_DISTANCE,
	MC_TRANSITION_TMAXIS_DISTANCE
};

// 方向
enum SMC_ORIENTATION_MODE : int16_t {
	SMC_ORIENTATION_GREATCIRCLE,
	SMC_ORIENTATION_AXIS
};

// 圆弧插补模式
enum SMC_CIRC_MODE : int16_t {
	SMC_CIRC_MODE_BORDER,					// 三点圆弧
	SMC_CIRC_MODE_CENTER,					// 两点+圆心圆弧
	SMC_CIRC_MODE_RADIUS					// 两点+矢量方向圆弧
};

// 圆弧插补方向
enum MC_CIRC_PATHCHOICE : int16_t {
	MC_CIRC_PATH_CLOCKWISE,					// 顺时针
	MC_CIRC_PATH_COUNTER_CLOCKWISE			// 逆时针
};

// 轴组运动状态
enum SMC_ROBOT_STATE : int16_t {
	SMC_ROBOT_STATE_DISABLED,
	SMC_ROBOT_STATE_STANDBY,
	SMC_ROBOT_STATE_MOVING,
	SMC_ROBOT_STATE_HOMING,
	SMC_ROBOT_STATE_STOPPING,
	SMC_ROBOT_STATE_ERRORSTOP
};

// 工具坐标系读取模式
enum SMC_READ_TOOL_MODE : int16_t {
	SMC_READ_TOOL_LAST_SET,
	SMC_READ_TOOL_ACTIVE_MOVEMENT,
	SMC_READ_TOOL_LAST_MOVEMENT
};

// 轴组参数
typedef struct _robot_module_params {
	ET_RobotType etRobotType;				// 机器人类型

	// 运动参数
	double rFBVelFactor;					// 比例系数 - 速度
	double rFBAccFactor;					// 比例系数 - 加速度
	double rFBJerkFactor;					// 比例系数 - 跃度
	double rFBTorqueFactor;					// 比例系数 - 力矩

	// 倍率设置 
	bool xEnSetOverride;					// 启用倍率覆盖
	double rAxisVelFactor;					// PTP运动 - 轴比例系数 - 速度
	double rAxisAccFactor;					// PTP运动 - 轴比例系数 - 加速度
	double rAxisJerkFactor;					// PTP运动 - 轴比例系数 - 跃度
	double rAxisTorqueFactor;				// PTP运动 - 轴比例系数 - 力矩
	double rPathVelFactor;					// 插补运动 - 路径比例系数 - 速度
	double rPathAccFactor;					// 插补运动 - 路径比例系数 - 加速度
	double rPathJerkFactor;					// 插补运动 - 路径比例系数 - 跃度

	// 平移混合 - 参数
	bool xTMWithAux;						// 辅助轴是否参与混合,支持的混合模式: TMStartVelocity, TMAxisDistance
	double rTMVelFactor;					// 基于速度混合: 速度系数
	double rTMCornerDistance;				// 基于距离混合: 转角距离

	bool xTMAxisDoneOnEucDistReached;		// 基于轴距离混合: 0.第一轴距离 1.欧氏距离 到达后发送完成信号
	double rTMAxisLine;						// 基于轴距离混合: 直线
	double rTMAxisAngle;					// 基于轴距离混合: 转角

	// 急停相关
	bool xHaltClearMovements;				// 中止 - 清除运动缓存
	bool xEStopClearMovements;				// 急停 - 清除运动缓存
	double rEStopRamp;						// 急停 - 减速度
	double rEStopJerk;						// 急停 - 跃度
	double rEStopRampFactor;				// 急停 - 加速度系数
	double rEStopJerkFactor;				// 急停 - 跃度系数
	double rEStopTorqueFactor;				// 急停 - 扭矩系数
	int16_t iEStopModuleNo;					// 急停开关 - 模块编号
	int16_t iEStopIoNo;						// 急停开关 - 输入点编号

	// 点动参数	
	double rJogMaxDistOfLinear;				// 点动 - 最大直线距离
	double rJogMaxDistOfAngular;			// 点动 - 最大运动角度
	double rJogVelocity;					// 点动 - 速度
	double rJogAcceleration;				// 点动 - 加速度
	double rJogDeceleration;				// 点动 - 减速度
	double rJogJerk;						// 点动 - 跃度
	double rJogVelFactor;					// 点动 - 速度系数
	double rJogAccFactor;					// 点动 - 加速度系数
	double rJogJerkFactor;					// 点动 - 跃度系数
	double rJogTorqueFactor;				// 点动 - 扭矩系数

	// PickPlace
	double rSafeHeight;						// 安全高度

	// 其它参数
	int16_t aiHomeSequence[10];				// 轴回零顺序
	MC_COORD_REF stSetToolOffset;			// 工具坐标系 - 设定的参数
	MC_COORD_REF stGetToolOffset;			// 工具坐标系 - 读取的参数
	SMC_READ_TOOL_MODE stGetToolMode;		// 工具坐标系 - 读取模式
	int32_t iToolPeriod;					// 工具轴A4 - 所在旋转周期
} ROBOT_MODULE_PARAM;

typedef struct _robot_control_inputs {
	bool xEnable;							// 轴组启用
	bool xPowerOn;							// 轴组使能
	bool xStart;							// 轴组启动
	ET_ROBOTMODULE_CMD etRunMode;			// 控制模式
	bool xReset;							// 故障复位
	bool xEStop;							// 停止
	SMC_POS_REF stPosition;					// 目标位置
	SMC_POS_REF stCircAuxPoint;				// 圆弧插补 - 辅助位置
	SMC_CIRC_MODE etCircMode;				// 圆弧插补 - 模式
	MC_CIRC_PATHCHOICE etCircPathChoice;	// 圆弧插补 - 方向
	double rVelocity;						// 目标速度
	double rAcceleration;					// 运行加速度
	double rDeceleration;					// 运行减速度
	double rJerk;							// 运行跃度

	SMC_COORD_SYSTEM etCoordSystem;			// 坐标系	
	MC_BUFFER_MODE etBufferMode;			// 缓冲模式	
	MC_TRANSITION_MODE etTransitionMode;	// 混合模式
	SMC_ORIENTATION_MODE etOrientationMode;	// 方向

	bool xJogForward;						// 点动前进
	bool xJogBackward;						// 点动后退
	bool xJogAbcAsAcs;						// 轴坐标系下点动ABC轴
	int16_t iJogAxisNo;						// 点动轴选择
} ROBOT_MODULE_INPUTS;

typedef struct _robot_control_outputs {
	bool xEnabled;							// 已启用
	bool xCommOK;							// 通讯正常指示
	bool xInRun;							// 轴组使能状态
	ET_ROBOTMODULE_CMD etActiveMode;		// 当前控制模式
	bool xRefOK;							// 轴组已回原点
	bool xStopped;							// 轴组停止状态
	bool xHoming;							// 轴组回原点中
	bool xInSync;							// 轴组同步运动中
	bool xTrackingDynamicCS;				// 动态坐标系追踪中
	bool xDone;								// 命令执行完成
	bool xBusy;								// 命令执行中
	bool xActive;							// 命令执行已激活
	bool xCommandAborted;					// 命令被打断
	bool xCommandAccepted;					// 命令已接收
	bool xError;							// 命令执行失败
	uint16_t etErrorId;						// 命令执行错误代码
	bool xGrpAlarm;							// 轴组报警
	uint32_t dwGrpAlarmID;					// 轴组报警ID
	SMC_ROBOT_STATE etGrpState;				// 轴组运动状态

	uint64_t ulMovementId;					// 运动Id - 功能块输出
	uint64_t ulActiveMovementId;			// 运动Id - 当前激活
	uint64_t ulFirstRefMovementId;			// 运动Id - 首次运动
	uint64_t ulLastAcceptedMovementId;		// 运动Id - 上一次接受

	ROBOT_MODULE_COORD stActPosition;		// 轴组当前位置
	ROBOT_MODULE_COORD stActVelocity;		// 轴组当前速度
} ROBOT_MODULE_OUTPUTS;

typedef struct _robot_control_interface {
	ROBOT_MODULE_INPUTS stI;				// 轴组结构体输入
	ROBOT_MODULE_OUTPUTS stQ;				// 轴组结构体输出
} ROBOT_MODULE_INTERFACE;
