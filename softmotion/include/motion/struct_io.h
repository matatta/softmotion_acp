#pragma once
#include <stdint.h>

const int c_module_num_di = 32;
const int c_module_num_ai_16bit = 16;
const int c_module_num_ai_32bit = 16;

const int c_module_num_dq = 32;
const int c_module_num_aq_16bit = 16;
const int c_module_num_aq_32bit = 16;

typedef struct _io_control_interface {
	bool DI_1BIT[c_module_num_di][32];			// 数字量输入模块 - 每个模块32点
	int16_t AI_16BIT[c_module_num_ai_16bit][8];	// 模拟量输入模块(16bit) - 每个模块8通道
	int32_t AI_32BIT[c_module_num_ai_32bit][8];	// 模拟量输入模块(32bit) - 每个模块8通道

	bool DQ_1BIT[c_module_num_dq][32];			// 数字量输出模块 - 每个模块32点
	int16_t AQ_16BIT[c_module_num_aq_16bit][8];	// 模拟量输出模块(16bit) - 每个模块8通道
	int32_t AQ_32BIT[c_module_num_aq_32bit][8];	// 模拟量输出模块(32bit) - 每个模块8通道
} IO_CONTROL_INTERFACE;

typedef struct _io_param_dq {
	bool xEnDqReverse;							// 翻转使能
	uint32_t iDqReverseTime;					// 翻转延时(ms)
	int16_t iDqReverseNum;						// 翻转次数
} IO_PARAM_DQ;

typedef struct _io_module_param {
	IO_PARAM_DQ astParam_DQ[c_module_num_dq][32];
} IO_MODULE_PARAM;