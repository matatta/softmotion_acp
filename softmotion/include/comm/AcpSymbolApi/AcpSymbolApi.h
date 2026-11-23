#ifndef ACPSYMBOLAPI_ACPSYMBOLAPI_H_
#define ACPSYMBOLAPI_ACPSYMBOLAPI_H_
#include <stdint.h>

#include <cstddef>
#include <cstring>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

typedef bool BOOL;
typedef int8_t BYTE;
typedef int32_t DINT;
typedef uint32_t DWORD;
typedef int16_t INT;
typedef int64_t LINT;
typedef double LREAL;
typedef uint64_t LWORD;
typedef float REAL;
typedef int8_t SINT;
typedef uint32_t UDINT;
typedef uint16_t UINT;
typedef uint64_t ULINT;
typedef uint8_t USINT;
typedef uint16_t WORD;

#ifdef _WIN32
#ifdef AcpSymbolApi_EXPORTS
#define AcpSymbolApi_API __declspec(dllexport)
#else
#define AcpSymbolApi_API __declspec(dllimport)
#endif
#else
#define AcpSymbolApi_API __attribute__((visibility("default")))
#endif  // _WIN32

namespace AcpSymbolApi {
template <size_t N>

/**
 * @brief 字符串class
 */
class STRING_ {
 public:
  /**
   * @brief 构造函数
   */
  STRING_() : value_() {}
  /**
   * @brief 构造函数
   * @param in: 字符串初始值
   */
  STRING_(const char* str) {  // NOLINT
    if (strlen(str) >= N) {
      throw std::out_of_range("String too long for STRING_");
    }
#if __linux__
    strncpy(value_, str, N);
#else
    strncpy_s(value_, str, N);
#endif
    value_[N] = '\0';  // 确保字符串以null结尾
  }

  /**
   * @brief 赋值操作符
   * @param in: 赋值的字符串
   */
  STRING_& operator=(const char* str) {
    if (strlen(str) >= N) {
      throw std::out_of_range("String too long for STRING_");
    }
#if __linux__
    strncpy(value_, str, N);
#else
    strncpy_s(value_, str, N);
#endif
    value_[N] = '\0';
    return *this;
  }

  /**
   * @brief 比较操作符
   * @param in: 比较值
   */
  bool operator==(const STRING_& other) const { return strncmp(value_, other.value_, N) == 0; }

  /**
   * @brief 索引操作符
   * @param in: 索引值
   */
  char operator[](size_t index) const {
    if (index >= N) {
      throw std::out_of_range("Index out of range for STRING_");
    }
    return value_[index];
  }

  /**
   * @brief 获取字符串size
   * return: 返回字符串size
   */
  size_t GetSize() { return sizeof(value_); }

  /**
   * @brief 获取字符串值
   * return: 返回字符串值
   */
  std::string value() {
    std::string str(value_);
    return str;
  }

 private:
  char value_[N + 1];  // 额外的一个字符用于存储字符串结束符'\0'
};

/**
 * @param void*:        symbol address
 * @param uint32_t:     size
 */
using VarPair = std::pair<void*, uint32_t>;

/**
 * @param std::string:  symbol name
 * @param VarPair:     symbol address and size
 */
using VarMap = std::map<std::string, VarPair>;

using VarNames = std::vector<std::string>;
/**
 * @param std::string:  symbol
 * @param uint32_t:     GTypeClass3
 * @param uint32_t:     size
 * @param uint64_t:     timemsec
 * @param std::string:  value
 */
using symbolsTuple = std::tuple<std::string, uint32_t, uint32_t, uint64_t, std::string>;

/**
 * @brief 回调返回类型
 */
enum SubReplyType {
  subscribefailed = 0,  // 订阅失败
  subscribeok = 1,      // 订阅订阅成功
  subscribedelok = 2,   // 删除订阅成功
  subscribedata = 3     // 订阅数据返回
};

/**
 * @brief 传出结构
 */
struct SymbolSubReply {
  int64_t reqid;
  SubReplyType type;
  int64_t subid;
  std::vector<symbolsTuple> symbolsVec;
  std::vector<std::string> faliedsymbolsVec;
};

/**
 * @brief 回调函数
 * @param in: 取出结构体
 * return: 成功返回true,失败返回false
 */
typedef void (*ptSymbolSubReplyCallBack)(SymbolSubReply* symbolSubReply);

/**
 * @brief Acp客户端
 */
class AcpSymbolApi_API AcpClient {
 public:
  /**
   * @brief Acp客户端的构造函数
   */
  AcpClient();
  /**
   * @brief Acp客户端的析构函数
   */
  ~AcpClient();

  /**
   * @brief 初始化Acp客户端，并和服务端建立连接
   * @param in: 服务端的ip地址
   * return: 成功返回true,失败返回false
   */
  bool Init(const std::string& Ip);

  /**
   * @brief 读取变量
   * @param in/out: 变量map
   * @param in: 超时时间
   * return: 成功返回true,失败返回false
   */
  bool ReadSymbols(const VarMap& symbols, const uint32_t& timeout = 1000);

  /**
   * @brief 写入变量
   * @param in: 变量map
   * @param in: 超时时间
   * return: 成功返回true,失败返回false
   */
  bool WriteSymbols(const VarMap& symbols, const uint32_t& timeout = 1000);

  /**
   * @brief 写入变量，无需返回响应消息
   * @param in: 变量map
   * @param in: 超时时间
   * return: 成功返回true,失败返回false
   */
  bool WriteOnlySymbols(const VarMap& symbols, const uint32_t& timeout = 1000);

  /**
   * @brief 注册订阅
   * @param in: 回调函数
   * return: 成功返回true,失败返回false
   */
  bool RegisterSubSymbol(ptSymbolSubReplyCallBack cb);

  /**
   * @brief 反注册订阅
   * return: 成功返回true,失败返回false
   */
  bool UnregisterSubSymbol();

  /**
   * @brief 添加订阅
   * @param in: 订阅节点数组
   * @param in: 请求ID
   * @param in: 超时时间
   * return: 成功返回true,失败返回false
   */
  bool AddBatchSubVar(const VarNames& names, int64_t reqid, uint32_t Intervalms, int timeout = 500);

  /**
   * @brief 取消订阅
   * @param in: 订阅的subid
   * return: 成功返回true,失败返回false
   */
  bool DelBatchSubVar(int64_t subid, int timeout = 500);

 private:
  /**
   *@breif: 注册路由事件回调函数
   *@param: data      数据缓冲区起始地址
   *@param: dataLen   数据缓冲区大小
   *@param: port      路由的port
   *@param: conn      接收到消息的回调的连接句柄
                      客户端注册的回调则是自己的句柄
                      服务端回调则是客户端连接的句柄
  *@param: lpContext 添加路由的时候用户指针
  */
  static void SubscribeMsgRouterCallBack(uint16_t port, const char* data, uint32_t len, void* lpContext);

 private:
  void* handle_;
  ptSymbolSubReplyCallBack symbolSubReplyCallBack_;
};
}  // namespace AcpSymbolApi

#endif  // ACPSYMBOLAPI_ACPSYMBOLAPI_H_
