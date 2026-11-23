#ifndef ACPSYMBOLAPI_ACPSYMBOLDEF_H_
#define ACPSYMBOLAPI_ACPSYMBOLDEF_H_

#include <stdint.h>
#include <string.h>

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

typedef bool ST_BOOL;
typedef uint8_t ST_BYTE;
typedef int32_t ST_DINT;
typedef uint32_t ST_DWORD;
typedef int16_t ST_INT;
typedef int64_t ST_LINT;
typedef double ST_LREAL;
typedef uint64_t ST_LWORD;
typedef float ST_REAL;
typedef int8_t ST_SINT;
typedef uint32_t ST_UDINT;
typedef uint16_t ST_UINT;
typedef uint64_t ST_ULINT;
typedef uint8_t ST_USINT;
typedef uint16_t ST_WORD;
typedef char ST_STRING;
typedef wchar_t ST_WSTRING;

typedef enum AcpRwErrors {
  ACP_RW_ERR_SUCCESS = 0x0000,              // 成功
  ACP_RW_ERR_INVAILD_HANDLE = 0x0001,       // 无效句柄
  ACP_RW_ERR_SEND_ERROR = 0x0002,           // 发送失败
  ACP_RW_ERR_RECV_ERROR = 0x0003,           // 收取失败
  ACP_RW_ERR_LOOP_FAILED = 0x0004,          // 订阅循环失败
  ACP_RW_ERR_REGISTE_FAILED = 0x0005,       // 注册失败
  ACP_RW_ERR_UNREGISTE_FAILED = 0x0006,     // 取消注册失败
  ACP_RW_ERR_SERIALIZED_FAILED = 0x0007,    // 序列化失败
  ACP_RW_ERR_DESERIALIZED_FAILED = 0x0008,  // 反序列化失败
} ACPRWERRORS;

typedef enum AcpConnectionStatus {
  ACP_RW_ERR_CONN_SUCCESS = 0x0001,       // 连接成功
  ACP_RW_ERR_CONN_PENDING = 0x0002,       // 正在连接
  ACP_RW_ERR_NO_CONN = 0x0003,            // 未连接
  ACP_RW_ERR_CONN_REFUSED = 0x0004,       // 拒绝连接
  ACP_RW_ERR_CONN_LOST = 0x0005,          // 断开连接
  ACP_RW_ERR_CONN_TIMEOUT = 0x0006,       // 连接超时
} ACPCONNECTIONSTATUS;

typedef enum AcpConnType {
  ACP_RW_CONN_TYPE_TCP = 0,         // TCP 标签通讯
  ACP_RW_CONN_TYPE_UNIXDOMAIN = 1,  // unix domain进行通讯
  ACP_RW_CONN_TYPE_LINUXSHM = 2,    // Linux的单域共享内存 未实现
  ACP_RW_CONN_TYPE_DUALSHM = 3,     // 双域共享内存 未实现
} ACPCONNTYPE;

typedef enum AcpTransMode {
  ACPTRANS_SERVERCYCLE = 0,  // 服务端变化推送  默认采用服务端变化进行推送
  ACPTRANS_SERVERONCHA = 1,  //  定周期推送
} ACPTRANSMODE;

typedef enum AcpSubReplyType {
  subscribefailed = 0,  // 订阅失败
  subscribeok = 1,      // 订阅订阅成功
  subscribedelok = 2,   // 删除订阅成功
  subscribeNotify = 3   // 订阅数据通知
} ACPSUBREPLYTYPE;

typedef enum AcpQueryNodeType {
  ROOT = 0,    // 根节点
  BRANCH = 1,  // 分支节点
  LEAF = 2,    // 页节点
} ACPQUERYNODETYPE;

// 暂时只支持TCP连接 方便后期扩展
typedef struct _ConnParam {
  ACPCONNTYPE connType;
  char szConnParam[256];  // 当连接类型为TCP时，传入IP地址,
} ConnParam;

// 结构体描述具体的某一个变量的值
typedef struct _TagValue {
  void* pValueData;  // 存储变量的内存地址
  int nValueSize;    // 变量的内存大小,单位为字节
} TagValue;

// 结构体描述一个变量的一些基础信息，例如变量名称，方便后面扩展
typedef struct _TagDesc {
  const char* szSymbolFullName;  // ANSI编码 使用者开辟内存
} TagDesc;

//  结构体描述一个变量的基础信息
typedef struct _TagInfo {
  TagDesc tagDesc;
  TagValue tagValue;
  uint64_t timeMsec;
} TagInfo, *pTagInfo;

// 订阅的参数说明
typedef struct _TagSubParam {
  int nIntervalMs;  // 订阅变量的变化的监听的时间间隔
  int nRequestId;   // 订阅的ID
  ACPTRANSMODE enTransMode;
} TagSubParam;

// 订阅的信息
typedef struct _TagSubInfo {
  int nRequestId;
  int nReplySubId;
  int subOkSize;
  int subFailSize;
  ACPTRANSMODE enTransMode;
  ACPSUBREPLYTYPE enReplyType;
  TagInfo* subOkTagsInfo;
  TagInfo* subFailTagsInfo;
} TagSubInfo, *pTagSubInfo;

// 查询结果说明
typedef struct _TagQueryInfo {
  AcpQueryNodeType nodeType;  // 节点类型
  char nodeName[256];         // 订阅的ID
  int varType;
  int matchsize;
} TagQueryInfo;

// 连接句柄
typedef void* TAG_HANDER;

/**
 * @brief 回调函数
 * @param in: 取出结构体
 * @param in: 用户指针
 */
typedef void (*pfnNotifyCallback)(TagSubInfo* pTagSubInfos, void* lpContext);

typedef void (*pfnConnectionCallback)(AcpConnectionStatus status, void* lpContext);

#endif  // ACPSYMBOLAPI_ACPSYMBOLDEF_H_
