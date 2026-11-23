#ifndef OPCUA_CLIENT_H
#define OPCUA_CLIENT_H

#include "open62541.h"
#include <string>
#include <vector>
#include <map>
#include <atomic>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <functional>
#include <memory>
#include <chrono>
#include <cstring>
#include <future>
#include <condition_variable>

// 连接状态枚举
enum class ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    RECONNECTING,
    CONNECTION_ERROR
};

// 数组类型枚举
enum class ArrayType {
    BOOLEAN,
    INT8, UINT8,
    INT16, UINT16,
    INT32, UINT32,
    INT64, UINT64,
    FLOAT,
    DOUBLE,
    STRING,
    BYTE,
    DATETIME,
    GUID,
    UNKNOWN
};

// 标量读写结果
template<typename T>
struct ReadResult {
    bool success;
    T value;
    std::string errorMsg;
    int errorCode;
    long long latencyUs;

    ReadResult() : success(false), errorCode(0), latencyUs(0) {}
    ReadResult(bool s, const T& v) : success(s), value(v), errorCode(0), latencyUs(0) {}
};

// 数组读取结果
template<typename T>
struct ArrayReadResult {
    bool success;
    std::vector<T> values;
    std::vector<size_t> dimensions;
    std::string errorMsg;
    int errorCode;
    long long latencyUs;
    bool isMultiDimensional;

    ArrayReadResult() : success(false), errorCode(0), latencyUs(0), isMultiDimensional(false) {}
};

// 数组写入数据
template<typename T>
struct ArrayWriteData {
    std::vector<T> values;
    std::vector<size_t> dimensions;
    bool isMultiDimensional;

    ArrayWriteData() : isMultiDimensional(false) {}
    explicit ArrayWriteData(const std::vector<T>& vals) : values(vals), isMultiDimensional(false) {}
    ArrayWriteData(const std::vector<T>& vals, const std::vector<size_t>& dims)
        : values(vals), dimensions(dims), isMultiDimensional(true) {
    }
};

// 回调函数类型
using ReadCallback = std::function<void(bool success, void* data, size_t size, size_t arraySize)>;
using WriteCallback = std::function<void(bool success)>;
using StateChangeCallback = std::function<void(ConnectionState state, const std::string& msg)>;
using ArrayReadCallback = std::function<void(bool success, void* data, size_t elementSize,
    size_t elementCount, const std::vector<size_t>& dimensions)>;

class OPCUAClient {
public:
    struct Config {
        std::string ip = "127.0.0.1";
        int port = 4840;
        int connectionPoolSize = 5;
        int sessionsPerConnection = 10;
        int timeoutMs = 5000;
        int reconnectIntervalMs = 3000;
        int maxReconnectAttempts = -1;
        bool autoReconnect = true;
        bool enableHeartbeat = true;
        std::string heartbeatNode = "ns=0;i=2253";
        int heartbeatIntervalMs = 3000;
        int heartbeatTimeoutMs = 2000;
        int heartbeatRetryCount = 3;
        int workThreadCount = 0;
        bool enableBatchOperation = true;
        bool enableCache = true;
        size_t maxArraySize = 1024 * 1024;
        bool enableArrayCompression = false;
    };

    explicit OPCUAClient(const Config& config = Config());
    ~OPCUAClient();

    // 初始化和关闭
    bool initialize();
    void shutdown();
    bool isRunning() const { return running_.load(); }

    // 标量读写
    template<typename T>
    ReadResult<T> readSync(const std::string& nodeName, int timeoutMs = 0);

    template<typename T>
    bool writeSync(const std::string& nodeName, const T& value, int timeoutMs = 0);

    template<typename T>
    void readAsync(const std::string& nodeName,
        std::function<void(const ReadResult<T>&)> callback,
        int timeoutMs = 0);

    template<typename T>
    void writeAsync(const std::string& nodeName, const T& value,
        std::function<void(bool)> callback,
        int timeoutMs = 0);

    // 数组读写
    template<typename T>
    ArrayReadResult<T> readArray(const std::string& nodeName, int timeoutMs = 0);

    template<typename T>
    void readArrayAsync(const std::string& nodeName,
        std::function<void(const ArrayReadResult<T>&)> callback,
        int timeoutMs = 0);

    template<typename T>
    bool writeArray(const std::string& nodeName, const ArrayWriteData<T>& data, int timeoutMs = 0);

    template<typename T>
    bool writeArray(const std::string& nodeName, const std::vector<T>& values, int timeoutMs = 0);

    template<typename T>
    void writeArrayAsync(const std::string& nodeName, const ArrayWriteData<T>& data,
        std::function<void(bool)> callback,
        int timeoutMs = 0);

    template<typename T>
    ArrayReadResult<T> readArrayRange(const std::string& nodeName,
        size_t startIndex,
        size_t count,
        int timeoutMs = 0);

    bool getArrayInfo(const std::string& nodeName,
        size_t& totalSize,
        std::vector<size_t>& dimensions,
        ArrayType& dataType,
        int timeoutMs = 0);

    ArrayType detectArrayType(const UA_Variant* variant);

    // 批量操作
    template<typename T>
    std::vector<ReadResult<T>> readBatch(const std::vector<std::string>& nodeNames,
        int timeoutMs = 0);

    template<typename T>
    std::vector<bool> writeBatch(const std::vector<std::string>& nodeNames,
        const std::vector<T>& values,
        int timeoutMs = 0);

    template<typename T>
    std::vector<ArrayReadResult<T>> readArrayBatch(const std::vector<std::string>& nodeNames,
        int timeoutMs = 0);

    template<typename T>
    std::vector<bool> writeArrayBatch(const std::vector<std::string>& nodeNames,
        const std::vector<ArrayWriteData<T>>& datas,
        int timeoutMs = 0);

    // 订阅
    bool subscribe(const std::string& nodeName,
        std::function<void(UA_Variant*)> callback,
        int intervalMs = 100);
    bool subscribeArray(const std::string& nodeName,
        std::function<void(UA_Variant*)> callback,
        int intervalMs = 100);
    bool unsubscribe(const std::string& nodeName);

    // 状态和统计
    ConnectionState getState() const { return state_.load(); }
    std::string getStateString() const;
    void setStateCallback(StateChangeCallback callback);
    void getStatistics(std::map<std::string, long long>& stats);
    std::string getPoolStatus() const;
    void resetStatistics();

    // 配置
    void setConfig(const Config& config);
    void setTimeout(int timeoutMs);
    void setReconnectInterval(int intervalMs);
    void setMaxArraySize(size_t maxSize);

private:
    // 内部数据结构 - 使用指针避免复制问题
    struct Session {
        UA_Client* client;
        std::atomic<bool> inUse;
        std::chrono::steady_clock::time_point lastUsed;
        std::string sessionId;
        int connectionIndex;

        Session() : client(nullptr), inUse(false), connectionIndex(-1) {}
        ~Session() {
            if (client) {
                UA_Client_disconnect(client);
                UA_Client_delete(client);
                client = nullptr;
            }
        }
        // 禁止复制
        Session(const Session&) = delete;
        Session& operator=(const Session&) = delete;
        // 允许移动
        Session(Session&&) = default;
        Session& operator=(Session&&) = default;
    };

    struct Connection {
        UA_Client* client;
        std::vector<Session*> sessions;
        std::atomic<bool> connected;
        std::atomic<bool> reconnecting;
        std::string url;
        std::mutex mutex;

        Connection() : client(nullptr), connected(false), reconnecting(false) {}
        ~Connection() {
            if (client) {
                UA_Client_disconnect(client);
                UA_Client_delete(client);
                client = nullptr;
            }
            for (auto session : sessions) {
                delete session;
            }
            sessions.clear();
        }
        // 禁止复制
        Connection(const Connection&) = delete;
        Connection& operator=(const Connection&) = delete;
    };

    // AsyncTask - 使用 shared_ptr 管理
    struct AsyncTask {
        enum Type {
            READ, WRITE,
            READ_ARRAY, WRITE_ARRAY,
            READ_ARRAY_RANGE,
            BATCH_READ, BATCH_WRITE,
            BATCH_READ_ARRAY, BATCH_WRITE_ARRAY,
            SUBSCRIBE
        };
        Type type;
        std::string nodeName;
        void* data;
        size_t dataSize;
        size_t arraySize;
        size_t startIndex;
        size_t count;
        int timeoutMs;
        UA_DataType* dataType;
        ReadCallback readCallback;
        WriteCallback writeCallback;
        ArrayReadCallback arrayReadCallback;
        std::chrono::steady_clock::time_point enqueueTime;
        std::shared_ptr<std::promise<void>> promise;
        std::vector<size_t> dimensions;

        AsyncTask() : data(nullptr), dataSize(0), arraySize(0), startIndex(0), count(0),
            timeoutMs(0), dataType(nullptr) {
        }
        ~AsyncTask() {
            if (data) {
                free(data);
                data = nullptr;
            }
        }
        // 禁止复制
        AsyncTask(const AsyncTask&) = delete;
        AsyncTask& operator=(const AsyncTask&) = delete;
        // 允许移动
        AsyncTask(AsyncTask&&) = default;
        AsyncTask& operator=(AsyncTask&&) = default;
    };

    struct Subscription {
        UA_NodeId nodeId;
        UA_Client* client;
        std::function<void(UA_Variant*)> callback;
        std::atomic<bool> active;
        bool isArray;

        Subscription() : client(nullptr), active(false), isArray(false) {
            UA_NodeId_init(&nodeId);
        }
        ~Subscription() {
            UA_NodeId_clear(&nodeId);
        }
        // 禁止复制
        Subscription(const Subscription&) = delete;
        Subscription& operator=(const Subscription&) = delete;
        // 允许移动
        Subscription(Subscription&&) = default;
        Subscription& operator=(Subscription&&) = default;
    };

    // 内部方法
    bool createConnectionPool();
    Session* acquireSession();
    void releaseSession(Session* session);
    bool reconnectConnection(Connection* conn);
    bool checkConnection(UA_Client* client);

    template<typename T>
    bool readInternal(UA_Client* client, const std::string& nodeName, T& value, int timeoutMs);

    template<typename T>
    bool writeInternal(UA_Client* client, const std::string& nodeName, const T& value, int timeoutMs);

    template<typename T>
    bool readArrayInternal(UA_Client* client, const std::string& nodeName,
        std::vector<T>& values, std::vector<size_t>& dimensions, int timeoutMs);

    template<typename T>
    bool writeArrayInternal(UA_Client* client, const std::string& nodeName,
        const std::vector<T>& values, const std::vector<size_t>& dimensions,
        bool isMultiDimensional, int timeoutMs);

    template<typename T>
    bool readArrayRangeInternal(UA_Client* client, const std::string& nodeName,
        size_t startIndex, size_t count,
        std::vector<T>& values, int timeoutMs);

    UA_NodeId parseNodeId(const std::string& name);
    UA_DataType* getDataTypeForTypeKind(UA_DataTypeKind kind);
    UA_DataType* getDataTypeForArray(ArrayType type);
    ArrayType getArrayTypeFromDataType(const UA_DataType* dataType);
    bool convertVariantToValue(const UA_Variant* variant, void* value, UA_DataType* targetType);
    bool convertVariantToArray(const UA_Variant* variant, void* values, size_t elementSize,
        size_t& elementCount, std::vector<size_t>& dimensions);
    bool createArrayVariant(const void* values, size_t elementSize, size_t count,
        const std::vector<size_t>& dimensions, UA_DataType* dataType,
        UA_Variant& variant);
    void updateLatencyStats(long long latencyUs, bool isRead);
    size_t calculateTotalElements(const std::vector<size_t>& dimensions);

    void workThreadFunc();
    void heartbeatThreadFunc();
    void monitorThreadFunc();
    void subscriptionThreadFunc();

    // 成员变量
    Config config_;

    std::vector<Connection*> connectionPool_;
    std::atomic<int> sessionCounter_;

    std::atomic<ConnectionState> state_;
    std::atomic<bool> running_;
    std::atomic<bool> heartbeatEnabled_;
    std::atomic<long long> lastHeartbeatTime_;

    std::vector<std::thread> workThreads_;
    std::thread heartbeatThread_;
    std::thread monitorThread_;
    std::thread subscriptionThread_;

    // 使用 shared_ptr 管理 AsyncTask
    std::queue<std::shared_ptr<AsyncTask>> taskQueue_;
    std::mutex queueMutex_;
    std::condition_variable queueCond_;

    std::atomic<long long> totalReads_;
    std::atomic<long long> totalWrites_;
    std::atomic<long long> totalArrayReads_;
    std::atomic<long long> totalArrayWrites_;
    std::atomic<long long> failedReads_;
    std::atomic<long long> failedWrites_;
    std::atomic<long long> failedArrayReads_;
    std::atomic<long long> failedArrayWrites_;
    std::atomic<long long> avgReadLatencyUs_;
    std::atomic<long long> avgWriteLatencyUs_;
    std::atomic<long long> maxReadLatencyUs_;
    std::atomic<long long> maxWriteLatencyUs_;
    std::atomic<long long> totalArrayBytesRead_;
    std::atomic<long long> totalArrayBytesWritten_;

    std::map<std::string, UA_NodeId> nodeIdCache_;
    std::shared_mutex cacheMutex_;

    std::map<std::string, std::shared_ptr<Subscription>> subscriptions_;
    std::shared_mutex subscriptionMutex_;

    StateChangeCallback stateCallback_;
    std::mutex callbackMutex_;

    std::vector<char> memoryPool_;
    std::atomic<size_t> memoryPoolIndex_;
};

// 模板实现
#include "opcua_client_impl.h"

#endif // OPCUA_CLIENT_H