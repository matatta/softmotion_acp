#ifndef ACPSYMBOLAPI_ACPSYMBOLAPI_C_H_
#define ACPSYMBOLAPI_ACPSYMBOLAPI_C_H_

#include <stdint.h>
#include <string.h>

#include "AcpSymbolApi/AcpSymbolDef.h"

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifdef _WIN32
#define ACP_SYMBOL_API_C_EXPORTS extern "C" __declspec(dllexport)
#else
#define ACP_SYMBOL_API_C_EXPORTS extern "C" __attribute__((visibility("default")))
#endif

/**
 * @brief  创建一个连接
 * return: 成功返回handler的句柄，失败返回nullptr
 */
ACP_SYMBOL_API_C_EXPORTS TAG_HANDER AcpCreateSocketHandler(ACPCONNTYPE type, const char* szDeviceIp,
                                                           pfnConnectionCallback callback, void* lpContext);

/**
 * @brief 创建一个双域共享内存连接
 *
 * @param nGroupIndex 共享内存组ID，当前只支持0和1
 * @param nMaxPacketSize 允许使用的共享内存大小，单位字节，不能超过32MB
 * @param callback 回调函数
 * @param lpContext 上下文指针
 * @return 成功返回handler的句柄，失败返回nullptr
 */
ACP_SYMBOL_API_C_EXPORTS TAG_HANDER AcpCreateIvshmConnHandler(int nGroupIndex, int nMaxPacketSize,
                                                              pfnConnectionCallback callback, void* lpContext);

/**
 * @brief  销毁一个连接句柄
 */
ACP_SYMBOL_API_C_EXPORTS int AcpDestoryConnHandler(TAG_HANDER handler);

/**
 * @brief  读取单一变量
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpReadTagValue(TAG_HANDER handler, TagInfo* taginfo, int nTimeOutMs);

/**
 * @brief  写单一变量
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpWriteTagValue(TAG_HANDER handler, TagInfo* taginfo, int nTimeOutMs);

/**
 * @brief  写单一变量，无需返回响应消息
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpWriteOnlyTagValue(TAG_HANDER handler, TagInfo* taginfo, int nTimeOutMs);

/**
 * @brief  读取多个变量
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpReadTagList(TAG_HANDER handler, TagInfo* taginfo, int tagNum, int nTimeOutMs);

/**
 * @brief  写多个变量
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpWriteTagList(TAG_HANDER handler, TagInfo* taginfo, int tagNum, int nTimeOutMs);

/**
 * @brief  写多个变量，无需返回响应消息
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpWriteOnlyTagList(TAG_HANDER handler, TagInfo* taginfo, int tagNum, int nTimeOutMs);

/**
 * @brief  注册订阅回调
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpRegisterNotifyCallBack(TAG_HANDER handler, pfnNotifyCallback cb, void* lpContext);

/**
 * @brief  移除订阅回调
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpRemoveNotifyCallBack(TAG_HANDER handler);

/**
 * @brief  添加订阅
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpAddTagsNotification(TAG_HANDER handler, TagInfo* tagInfo, int tagNum,
                                                    TagSubParam* pSubParam, int nTimeOutMs);

/**
 * @brief  删除变量的订阅
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpDelTagsNotification(TAG_HANDER handler, int nSubId, int nTimeOutMs);

/**
 * @brief  展开变量
 * @param in: 句柄
 * @param in: 超时时间
 * @param in/out: 查询结果
 * @param in/out: 查询结果数量
 * @param in: 查询节点名称（查询root节点时传nullptr，其他传变量节点名称即可）
 * return: 成功返回0,失败返回-1或者错误码
 */
ACP_SYMBOL_API_C_EXPORTS int AcpExpandVar(TAG_HANDER handler, int nTimeOutMs, TagQueryInfo* pInfo, int* pTagNum,
                                          TagDesc desc);

#endif  // ACPSYMBOLAPI_ACPSYMBOLAPI_C_H_
