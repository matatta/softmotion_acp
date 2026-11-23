#include "opcua_client.h"
#include <iostream>
#include <chrono>
#include <algorithm>

// ============================================================================
// 构造函数和析构函数
// ============================================================================

OPCUAClient::OPCUAClient(const Config& config)
    : config_(config)
    , sessionCounter_(0)
    , state_(ConnectionState::DISCONNECTED)
    , running_(false)
    , heartbeatEnabled_(false)
    , lastHeartbeatTime_(0)
    , totalReads_(0)
    , totalWrites_(0)
    , totalArrayReads_(0)
    , totalArrayWrites_(0)
    , failedReads_(0)
    , failedWrites_(0)
    , failedArrayReads_(0)
    , failedArrayWrites_(0)
    , avgReadLatencyUs_(0)
    , avgWriteLatencyUs_(0)
    , maxReadLatencyUs_(0)
    , maxWriteLatencyUs_(0)
    , totalArrayBytesRead_(0)
    , totalArrayBytesWritten_(0)
    , memoryPoolIndex_(0) {

    if (config_.workThreadCount <= 0) {
        config_.workThreadCount = std::thread::hardware_concurrency() * 2;
        if (config_.workThreadCount < 2) config_.workThreadCount = 4;
    }

    memoryPool_.resize(1024 * 1024 * 5);

    std::cout << "OPCUAClient created with "
        << config_.workThreadCount << " worker threads" << std::endl;
}

OPCUAClient::~OPCUAClient() {
    shutdown();
}

// ============================================================================
// 初始化和关闭
// ============================================================================

bool OPCUAClient::initialize() {
    if (running_) return true;

    running_ = true;
    heartbeatEnabled_ = config_.enableHeartbeat;
    state_ = ConnectionState::CONNECTING;

    std::cout << "Initializing OPC UA client..." << std::endl;

    if (!createConnectionPool()) {
        running_ = false;
        state_ = ConnectionState::CONNECTION_ERROR;
        std::cerr << "Failed to create connection pool" << std::endl;
        return false;
    }

    for (int i = 0; i < config_.workThreadCount; ++i) {
        workThreads_.emplace_back(&OPCUAClient::workThreadFunc, this);
    }

    if (heartbeatEnabled_) {
        heartbeatThread_ = std::thread(&OPCUAClient::heartbeatThreadFunc, this);
    }

    monitorThread_ = std::thread(&OPCUAClient::monitorThreadFunc, this);
    subscriptionThread_ = std::thread(&OPCUAClient::subscriptionThreadFunc, this);

    state_ = ConnectionState::CONNECTED;
    std::cout << "Client initialized successfully. Connections: "
        << connectionPool_.size() << std::endl;

    return true;
}

bool OPCUAClient::createConnectionPool() {
    for (int i = 0; i < config_.connectionPoolSize; ++i) {
        auto conn = new Connection();
        conn->url = "opc.tcp://" + config_.ip + ":" + std::to_string(config_.port);

        conn->client = UA_Client_new();
        UA_ClientConfig_setDefault(UA_Client_getConfig(conn->client));

        UA_ClientConfig* cfg = UA_Client_getConfig(conn->client);
        cfg->timeout = config_.timeoutMs;
        cfg->secureChannelLifeTime = 60000;
        cfg->requestedSessionTimeout = 60000;

        UA_StatusCode status = UA_Client_connect(conn->client, conn->url.c_str());
        if (status != UA_STATUSCODE_GOOD) {
            std::cerr << "Failed to connect connection " << i << ": "
                << UA_StatusCode_name(status) << std::endl;
            UA_Client_delete(conn->client);
            delete conn;
            continue;
        }

        conn->connected = true;

        for (int j = 0; j < config_.sessionsPerConnection; ++j) {
            auto session = new Session();
            session->client = UA_Client_new();
            UA_ClientConfig_setDefault(UA_Client_getConfig(session->client));

            UA_ClientConfig* sessionCfg = UA_Client_getConfig(session->client);
            sessionCfg->timeout = config_.timeoutMs;
            sessionCfg->secureChannelLifeTime = cfg->secureChannelLifeTime;
            sessionCfg->requestedSessionTimeout = cfg->requestedSessionTimeout;

            status = UA_Client_connect(session->client, conn->url.c_str());
            if (status == UA_STATUSCODE_GOOD) {
                session->sessionId = "session_" + std::to_string(sessionCounter_++);
                session->connectionIndex = i;
                conn->sessions.push_back(session);
            }
            else {
                std::cerr << "Failed to create session " << j << " for connection " << i << std::endl;
                UA_Client_delete(session->client);
                delete session;
            }
        }

        if (!conn->sessions.empty()) {
            connectionPool_.push_back(conn);
            std::cout << "Connection " << i << " created with "
                << conn->sessions.size() << " sessions" << std::endl;
        }
        else {
            UA_Client_disconnect(conn->client);
            UA_Client_delete(conn->client);
            delete conn;
        }
    }

    return !connectionPool_.empty();
}

void OPCUAClient::shutdown() {
    if (!running_) return;

    running_ = false;
    heartbeatEnabled_ = false;
    state_ = ConnectionState::DISCONNECTED;

    std::cout << "Shutting down client..." << std::endl;

    queueCond_.notify_all();

    for (auto& t : workThreads_) {
        if (t.joinable()) t.join();
    }
    workThreads_.clear();

    if (heartbeatThread_.joinable()) heartbeatThread_.join();
    if (monitorThread_.joinable()) monitorThread_.join();
    if (subscriptionThread_.joinable()) subscriptionThread_.join();

    for (auto conn : connectionPool_) {
        delete conn;
    }
    connectionPool_.clear();

    {
        std::unique_lock<std::shared_mutex> lock(cacheMutex_);
        for (auto& pair : nodeIdCache_) {
            UA_NodeId_clear(&pair.second);
        }
        nodeIdCache_.clear();
    }

    std::cout << "Client shutdown complete" << std::endl;
}

// ============================================================================
// 会话管理
// ============================================================================

OPCUAClient::Session* OPCUAClient::acquireSession() {
    if (connectionPool_.empty()) return nullptr;

    static thread_local size_t lastIndex = 0;
    size_t startIndex = lastIndex;
    size_t attempts = 0;
    size_t totalSessions = 0;

    for (auto conn : connectionPool_) {
        totalSessions += conn->sessions.size();
    }

    while (attempts < totalSessions) {
        size_t connIndex = (startIndex + attempts) % connectionPool_.size();
        auto conn = connectionPool_[connIndex];

        if (!conn->connected) {
            attempts++;
            continue;
        }

        for (auto session : conn->sessions) {
            bool expected = false;
            if (session->inUse.compare_exchange_strong(expected, true)) {
                session->lastUsed = std::chrono::steady_clock::now();
                lastIndex = connIndex;
                return session;
            }
        }

        attempts++;
    }

    // 所有会话都在使用，创建临时会话
    for (auto conn : connectionPool_) {
        if (!conn->connected) continue;

        auto session = new Session();
        session->client = UA_Client_new();
        UA_ClientConfig_setDefault(UA_Client_getConfig(session->client));

        UA_ClientConfig* cfg = UA_Client_getConfig(session->client);
        cfg->timeout = config_.timeoutMs;

        UA_StatusCode status = UA_Client_connect(session->client, conn->url.c_str());
        if (status == UA_STATUSCODE_GOOD) {
            session->sessionId = "temp_" + std::to_string(sessionCounter_++);
            session->inUse = true;
            session->connectionIndex = static_cast<int>(connectionPool_.size() - 1);
            return session;
        }

        UA_Client_delete(session->client);
        delete session;
    }

    return nullptr;
}

void OPCUAClient::releaseSession(Session* session) {
    if (!session) return;

    // UA_Client_getState 需要4个参数
    UA_SecureChannelState channelState;
    UA_SessionState sessionState;
    UA_StatusCode connectStatus;

    UA_Client_getState(session->client, &channelState, &sessionState, &connectStatus);

    // 检查连接状态
    if (connectStatus != UA_STATUSCODE_GOOD || sessionState == UA_SESSIONSTATE_CLOSED) {
        UA_Client_disconnect(session->client);
        UA_Client_delete(session->client);
        session->client = nullptr;

        if (session->connectionIndex >= 0 &&
            session->connectionIndex < static_cast<int>(connectionPool_.size())) {
            auto conn = connectionPool_[session->connectionIndex];
            if (conn && conn->connected) {
                UA_Client* newClient = UA_Client_new();
                UA_ClientConfig_setDefault(UA_Client_getConfig(newClient));
                UA_StatusCode connStatus = UA_Client_connect(newClient, conn->url.c_str());
                if (connStatus == UA_STATUSCODE_GOOD) {
                    session->client = newClient;
                }
                else {
                    UA_Client_delete(newClient);
                }
            }
        }
    }

    session->inUse = false;
}

// ============================================================================
// 工作线程（修正 promise 问题）
// ============================================================================

void OPCUAClient::workThreadFunc() {
    while (running_) {
        std::shared_ptr<AsyncTask> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            if (taskQueue_.empty()) {
                queueCond_.wait_for(lock, std::chrono::milliseconds(10));
                continue;
            }
            task = taskQueue_.front();
            taskQueue_.pop();
        }

        if (!task) continue;

        // 处理任务
        bool taskCompleted = false;

        switch (task->type) {
        case AsyncTask::READ: {
            Session* session = acquireSession();
            if (session) {
                void* data = malloc(task->dataSize);
                bool success = false;
                // 实际应该调用 readInternal
                if (task->readCallback) {
                    task->readCallback(success, data, task->dataSize, 0);
                }
                free(data);
                releaseSession(session);
                taskCompleted = true;
            }
            else if (task->readCallback) {
                task->readCallback(false, nullptr, 0, 0);
                taskCompleted = true;
            }
            break;
        }
        case AsyncTask::WRITE: {
            Session* session = acquireSession();
            if (session) {
                bool success = false;
                if (task->writeCallback) {
                    task->writeCallback(success);
                }
                releaseSession(session);
                taskCompleted = true;
            }
            else if (task->writeCallback) {
                task->writeCallback(false);
                taskCompleted = true;
            }
            break;
        }
        case AsyncTask::READ_ARRAY: {
            Session* session = acquireSession();
            if (session) {
                if (task->arrayReadCallback) {
                    std::vector<size_t> dimensions;
                    task->arrayReadCallback(false, nullptr, 0, 0, dimensions);
                }
                releaseSession(session);
                taskCompleted = true;
            }
            else if (task->arrayReadCallback) {
                std::vector<size_t> dimensions;
                task->arrayReadCallback(false, nullptr, 0, 0, dimensions);
                taskCompleted = true;
            }
            break;
        }
        case AsyncTask::WRITE_ARRAY: {
            Session* session = acquireSession();
            if (session) {
                if (task->writeCallback) {
                    task->writeCallback(false);
                }
                releaseSession(session);
                taskCompleted = true;
            }
            else if (task->writeCallback) {
                task->writeCallback(false);
                taskCompleted = true;
            }
            break;
        }
        default:
            taskCompleted = true;
            break;
        }

        // 设置 promise 值
        if (task->promise && taskCompleted) {
            task->promise->set_value();
        }
    }
}

// ============================================================================
// 心跳线程
// ============================================================================

void OPCUAClient::heartbeatThreadFunc() {
    int failCount = 0;

    while (running_ && heartbeatEnabled_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(config_.heartbeatIntervalMs));

        if (!running_) break;

        bool anyConnected = false;

        for (auto conn : connectionPool_) {
            if (!conn->connected) {
                if (config_.autoReconnect) {
                    reconnectConnection(conn);
                }
                continue;
            }

            Session* session = acquireSession();
            if (session) {
                UA_NodeId nodeId = parseNodeId(config_.heartbeatNode);
                if (!UA_NodeId_isNull(&nodeId)) {
                    UA_Variant variant;
                    UA_Variant_init(&variant);

                    UA_ClientConfig* cfg = UA_Client_getConfig(session->client);
                    int originalTimeout = cfg->timeout;
                    cfg->timeout = config_.heartbeatTimeoutMs;

                    UA_StatusCode status = UA_Client_readValueAttribute(
                        session->client, nodeId, &variant);

                    cfg->timeout = originalTimeout;

                    if (status == UA_STATUSCODE_GOOD) {
                        anyConnected = true;
                        lastHeartbeatTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch()).count();
                        failCount = 0;
                    }
                    else {
                        failCount++;
                        if (failCount >= config_.heartbeatRetryCount) {
                            conn->connected = false;
                            std::cerr << "Heartbeat failed for connection" << std::endl;
                            state_ = ConnectionState::DISCONNECTED;

                            if (stateCallback_) {
                                stateCallback_(state_, "Heartbeat failed");
                            }
                        }
                    }

                    UA_Variant_clear(&variant);
                    UA_NodeId_clear(&nodeId);
                }

                releaseSession(session);
            }
        }

        if (anyConnected && state_ != ConnectionState::CONNECTED) {
            state_ = ConnectionState::CONNECTED;
            if (stateCallback_) {
                stateCallback_(state_, "Heartbeat recovered");
            }
        }
    }
}

// ============================================================================
// 监控线程
// ============================================================================

void OPCUAClient::monitorThreadFunc() {
    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(10));

        if (!running_) break;

        int totalSessions = 0;
        int activeSessions = 0;
        int connectedCount = 0;

        for (auto conn : connectionPool_) {
            if (conn->connected) connectedCount++;
            totalSessions += static_cast<int>(conn->sessions.size());
            for (auto session : conn->sessions) {
                if (session->inUse) activeSessions++;
            }
        }
    }
}

// ============================================================================
// 订阅线程
// ============================================================================

void OPCUAClient::subscriptionThreadFunc() {
    while (running_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (!running_) break;

        std::shared_lock<std::shared_mutex> lock(subscriptionMutex_);
        for (auto& pair : subscriptions_) {
            if (!pair.second || !pair.second->active) continue;

            UA_Variant variant;
            UA_Variant_init(&variant);

            UA_ClientConfig* cfg = UA_Client_getConfig(pair.second->client);
            int originalTimeout = cfg->timeout;
            cfg->timeout = 1000;

            UA_StatusCode status = UA_Client_readValueAttribute(
                pair.second->client, pair.second->nodeId, &variant);

            cfg->timeout = originalTimeout;

            if (status == UA_STATUSCODE_GOOD && pair.second->callback) {
                pair.second->callback(&variant);
            }

            UA_Variant_clear(&variant);
        }
    }
}

// ============================================================================
// 辅助函数 - parseNodeId
// ============================================================================

UA_NodeId OPCUAClient::parseNodeId(const std::string& name) {
    UA_NodeId nodeId = UA_NODEID_NULL;

    if (name.find("ns=") != std::string::npos) {
        size_t nsPos = name.find("ns=");
        size_t sPos = name.find(";s=");
        size_t iPos = name.find(";i=");

        if (nsPos != std::string::npos && sPos != std::string::npos) {
            std::string nsStr = name.substr(nsPos + 3, sPos - nsPos - 3);
            std::string nameStr = name.substr(sPos + 3);
            uint16_t ns = static_cast<uint16_t>(std::stoi(nsStr));
            nodeId = UA_NODEID_STRING_ALLOC(ns, nameStr.c_str());
        }
        else if (nsPos != std::string::npos && iPos != std::string::npos) {
            std::string nsStr = name.substr(nsPos + 3, iPos - nsPos - 3);
            std::string idStr = name.substr(iPos + 3);
            uint16_t ns = static_cast<uint16_t>(std::stoi(nsStr));
            uint32_t id = static_cast<uint32_t>(std::stoul(idStr));
            nodeId = UA_NODEID_NUMERIC(ns, id);
        }
    }
    else {
        nodeId = UA_NODEID_STRING_ALLOC(1, name.c_str());
    }

    return nodeId;
}

// ============================================================================
// 辅助函数 - 类型转换
// ============================================================================

bool OPCUAClient::convertVariantToValue(const UA_Variant* variant, void* value,
    UA_DataType* targetType) {
    if (!variant->data || !variant->type) return false;

    // 使用 variant->type->typeKind，它已经是 UA_DataTypeKind 类型
    UA_DataTypeKind kind = static_cast<UA_DataTypeKind>(variant->type->typeKind);
    switch (kind) {
    case UA_DATATYPEKIND_BOOLEAN:
        if (targetType->typeKind == UA_DATATYPEKIND_BOOLEAN) {
            *(bool*)value = *(UA_Boolean*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_SBYTE:
        if (targetType->typeKind == UA_DATATYPEKIND_SBYTE) {
            *(int8_t*)value = *(UA_SByte*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_BYTE:
        if (targetType->typeKind == UA_DATATYPEKIND_BYTE) {
            *(uint8_t*)value = *(UA_Byte*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_INT16:
        if (targetType->typeKind == UA_DATATYPEKIND_INT16) {
            *(int16_t*)value = *(UA_Int16*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_UINT16:
        if (targetType->typeKind == UA_DATATYPEKIND_UINT16) {
            *(uint16_t*)value = *(UA_UInt16*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_INT32:
        if (targetType->typeKind == UA_DATATYPEKIND_INT32) {
            *(int32_t*)value = *(UA_Int32*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_UINT32:
        if (targetType->typeKind == UA_DATATYPEKIND_UINT32) {
            *(uint32_t*)value = *(UA_UInt32*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_INT64:
        if (targetType->typeKind == UA_DATATYPEKIND_INT64) {
            *(int64_t*)value = *(UA_Int64*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_UINT64:
        if (targetType->typeKind == UA_DATATYPEKIND_UINT64) {
            *(uint64_t*)value = *(UA_UInt64*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_FLOAT:
        if (targetType->typeKind == UA_DATATYPEKIND_FLOAT) {
            *(float*)value = *(UA_Float*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_DOUBLE:
        if (targetType->typeKind == UA_DATATYPEKIND_DOUBLE) {
            *(double*)value = *(UA_Double*)variant->data;
            return true;
        }
        break;
    case UA_DATATYPEKIND_STRING:
        if (targetType->typeKind == UA_DATATYPEKIND_STRING) {
            UA_String* str = (UA_String*)variant->data;
            std::string* result = (std::string*)value;
            result->assign((char*)str->data, str->length);
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

UA_DataType* OPCUAClient::getDataTypeForTypeKind(UA_DataTypeKind kind) {
    switch (kind) {
    case UA_DATATYPEKIND_BOOLEAN: return &UA_TYPES[UA_TYPES_BOOLEAN];
    case UA_DATATYPEKIND_SBYTE: return &UA_TYPES[UA_TYPES_SBYTE];
    case UA_DATATYPEKIND_BYTE: return &UA_TYPES[UA_TYPES_BYTE];
    case UA_DATATYPEKIND_INT16: return &UA_TYPES[UA_TYPES_INT16];
    case UA_DATATYPEKIND_UINT16: return &UA_TYPES[UA_TYPES_UINT16];
    case UA_DATATYPEKIND_INT32: return &UA_TYPES[UA_TYPES_INT32];
    case UA_DATATYPEKIND_UINT32: return &UA_TYPES[UA_TYPES_UINT32];
    case UA_DATATYPEKIND_INT64: return &UA_TYPES[UA_TYPES_INT64];
    case UA_DATATYPEKIND_UINT64: return &UA_TYPES[UA_TYPES_UINT64];
    case UA_DATATYPEKIND_FLOAT: return &UA_TYPES[UA_TYPES_FLOAT];
    case UA_DATATYPEKIND_DOUBLE: return &UA_TYPES[UA_TYPES_DOUBLE];
    case UA_DATATYPEKIND_STRING: return &UA_TYPES[UA_TYPES_STRING];
    case UA_DATATYPEKIND_DATETIME: return &UA_TYPES[UA_TYPES_DATETIME];
    default: return &UA_TYPES[UA_TYPES_INT32];
    }
}

// ============================================================================
// 辅助函数 - 数组相关
// ============================================================================

ArrayType OPCUAClient::detectArrayType(const UA_Variant* variant) {
    if (!variant || !variant->type) return ArrayType::UNKNOWN;

    // 使用显式转换
    UA_DataTypeKind kind = static_cast<UA_DataTypeKind>(variant->type->typeKind);
    switch (kind) {
    case UA_DATATYPEKIND_BOOLEAN: return ArrayType::BOOLEAN;
    case UA_DATATYPEKIND_SBYTE: return ArrayType::INT8;
    case UA_DATATYPEKIND_BYTE: return ArrayType::UINT8;
    case UA_DATATYPEKIND_INT16: return ArrayType::INT16;
    case UA_DATATYPEKIND_UINT16: return ArrayType::UINT16;
    case UA_DATATYPEKIND_INT32: return ArrayType::INT32;
    case UA_DATATYPEKIND_UINT32: return ArrayType::UINT32;
    case UA_DATATYPEKIND_INT64: return ArrayType::INT64;
    case UA_DATATYPEKIND_UINT64: return ArrayType::UINT64;
    case UA_DATATYPEKIND_FLOAT: return ArrayType::FLOAT;
    case UA_DATATYPEKIND_DOUBLE: return ArrayType::DOUBLE;
    case UA_DATATYPEKIND_STRING: return ArrayType::STRING;
    case UA_DATATYPEKIND_BYTESTRING: return ArrayType::BYTE;
    case UA_DATATYPEKIND_DATETIME: return ArrayType::DATETIME;
    case UA_DATATYPEKIND_GUID: return ArrayType::GUID;
    default: return ArrayType::UNKNOWN;
    }
}

UA_DataType* OPCUAClient::getDataTypeForArray(ArrayType type) {
    switch (type) {
    case ArrayType::BOOLEAN: return &UA_TYPES[UA_TYPES_BOOLEAN];
    case ArrayType::INT8: return &UA_TYPES[UA_TYPES_SBYTE];
    case ArrayType::UINT8: return &UA_TYPES[UA_TYPES_BYTE];
    case ArrayType::INT16: return &UA_TYPES[UA_TYPES_INT16];
    case ArrayType::UINT16: return &UA_TYPES[UA_TYPES_UINT16];
    case ArrayType::INT32: return &UA_TYPES[UA_TYPES_INT32];
    case ArrayType::UINT32: return &UA_TYPES[UA_TYPES_UINT32];
    case ArrayType::INT64: return &UA_TYPES[UA_TYPES_INT64];
    case ArrayType::UINT64: return &UA_TYPES[UA_TYPES_UINT64];
    case ArrayType::FLOAT: return &UA_TYPES[UA_TYPES_FLOAT];
    case ArrayType::DOUBLE: return &UA_TYPES[UA_TYPES_DOUBLE];
    case ArrayType::STRING: return &UA_TYPES[UA_TYPES_STRING];
    case ArrayType::BYTE: return &UA_TYPES[UA_TYPES_BYTESTRING];
    case ArrayType::DATETIME: return &UA_TYPES[UA_TYPES_DATETIME];
    case ArrayType::GUID: return &UA_TYPES[UA_TYPES_GUID];
    default: return nullptr;
    }
}

ArrayType OPCUAClient::getArrayTypeFromDataType(const UA_DataType* dataType) {
    if (!dataType) return ArrayType::UNKNOWN;

    // 使用显式转换
    UA_DataTypeKind kind = static_cast<UA_DataTypeKind>(dataType->typeKind);
    switch (kind) {
    case UA_DATATYPEKIND_BOOLEAN: return ArrayType::BOOLEAN;
    case UA_DATATYPEKIND_SBYTE: return ArrayType::INT8;
    case UA_DATATYPEKIND_BYTE: return ArrayType::UINT8;
    case UA_DATATYPEKIND_INT16: return ArrayType::INT16;
    case UA_DATATYPEKIND_UINT16: return ArrayType::UINT16;
    case UA_DATATYPEKIND_INT32: return ArrayType::INT32;
    case UA_DATATYPEKIND_UINT32: return ArrayType::UINT32;
    case UA_DATATYPEKIND_INT64: return ArrayType::INT64;
    case UA_DATATYPEKIND_UINT64: return ArrayType::UINT64;
    case UA_DATATYPEKIND_FLOAT: return ArrayType::FLOAT;
    case UA_DATATYPEKIND_DOUBLE: return ArrayType::DOUBLE;
    case UA_DATATYPEKIND_STRING: return ArrayType::STRING;
    case UA_DATATYPEKIND_BYTESTRING: return ArrayType::BYTE;
    case UA_DATATYPEKIND_DATETIME: return ArrayType::DATETIME;
    case UA_DATATYPEKIND_GUID: return ArrayType::GUID;
    default: return ArrayType::UNKNOWN;
    }
}
bool OPCUAClient::convertVariantToArray(const UA_Variant* variant, void* values,
    size_t elementSize, size_t& elementCount,
    std::vector<size_t>& dimensions) {
    if (!variant || !UA_Variant_isArray(variant)) return false;

    elementCount = variant->arrayLength;

    dimensions.clear();
    if (variant->arrayDimensionsSize > 0) {
        for (size_t i = 0; i < variant->arrayDimensionsSize; ++i) {
            dimensions.push_back(variant->arrayDimensions[i]);
        }
    }
    else {
        dimensions.push_back(elementCount);
    }

    if (values && elementCount > 0) {
        memcpy(values, variant->data, elementCount * elementSize);
    }

    return true;
}

bool OPCUAClient::createArrayVariant(const void* values, size_t elementSize, size_t count,
    const std::vector<size_t>& dimensions, UA_DataType* dataType,
    UA_Variant& variant) {
    if (!values || count == 0 || !dataType) return false;

    void* data = UA_Array_new(count, dataType);
    if (!data) return false;

    memcpy(data, values, count * elementSize);
    UA_Variant_setArray(&variant, data, count, dataType);

    if (!dimensions.empty() && dimensions.size() > 1) {
        variant.arrayDimensionsSize = dimensions.size();
        variant.arrayDimensions = (UA_UInt32*)UA_Array_new(dimensions.size(), &UA_TYPES[UA_TYPES_UINT32]);
        for (size_t i = 0; i < dimensions.size(); ++i) {
            variant.arrayDimensions[i] = static_cast<UA_UInt32>(dimensions[i]);
        }
    }

    return true;
}

bool OPCUAClient::checkConnection(UA_Client* client) {
    if (!client) return false;

    UA_SecureChannelState channelState;
    UA_SessionState sessionState;
    UA_StatusCode connectStatus;

    UA_Client_getState(client, &channelState, &sessionState, &connectStatus);

    return (connectStatus == UA_STATUSCODE_GOOD && sessionState != UA_SESSIONSTATE_CLOSED);
}

size_t OPCUAClient::calculateTotalElements(const std::vector<size_t>& dimensions) {
    if (dimensions.empty()) return 0;
    size_t total = 1;
    for (size_t dim : dimensions) {
        total *= dim;
    }
    return total;
}

void OPCUAClient::updateLatencyStats(long long latencyUs, bool isRead) {
    if (isRead) {
        long long currentAvg = avgReadLatencyUs_.load();
        avgReadLatencyUs_ = (currentAvg * 99 + latencyUs) / 100;
        if (latencyUs > maxReadLatencyUs_.load()) {
            maxReadLatencyUs_ = latencyUs;
        }
    }
    else {
        long long currentAvg = avgWriteLatencyUs_.load();
        avgWriteLatencyUs_ = (currentAvg * 99 + latencyUs) / 100;
        if (latencyUs > maxWriteLatencyUs_.load()) {
            maxWriteLatencyUs_ = latencyUs;
        }
    }
}

// ============================================================================
// 重连
// ============================================================================

bool OPCUAClient::reconnectConnection(Connection* conn) {
    if (conn->reconnecting) return false;
    conn->reconnecting = true;

    for (int i = 0; i < config_.maxReconnectAttempts || config_.maxReconnectAttempts == -1; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(config_.reconnectIntervalMs));

        if (!running_) break;

        UA_StatusCode status = UA_Client_connect(conn->client, conn->url.c_str());
        if (status == UA_STATUSCODE_GOOD) {
            conn->connected = true;
            conn->reconnecting = false;

            for (auto session : conn->sessions) {
                if (session->client) {
                    UA_Client_disconnect(session->client);
                    UA_Client_delete(session->client);
                }
                session->client = UA_Client_new();
                UA_ClientConfig_setDefault(UA_Client_getConfig(session->client));

                UA_ClientConfig* cfg = UA_Client_getConfig(session->client);
                cfg->timeout = config_.timeoutMs;
                cfg->secureChannelLifeTime = 60000;
                cfg->requestedSessionTimeout = 60000;

                UA_Client_connect(session->client, conn->url.c_str());
            }

            state_ = ConnectionState::CONNECTED;
            if (stateCallback_) {
                stateCallback_(state_, "Reconnected successfully");
            }

            return true;
        }

        if (config_.maxReconnectAttempts != -1 && i >= config_.maxReconnectAttempts - 1) {
            state_ = ConnectionState::CONNECTION_ERROR;
            if (stateCallback_) {
                stateCallback_(state_, "Max reconnect attempts reached");
            }
            break;
        }
    }

    conn->reconnecting = false;
    return false;
}

// ============================================================================
// 状态和统计
// ============================================================================

std::string OPCUAClient::getStateString() const {
    switch (state_.load()) {
    case ConnectionState::DISCONNECTED: return "DISCONNECTED";
    case ConnectionState::CONNECTING: return "CONNECTING";
    case ConnectionState::CONNECTED: return "CONNECTED";
    case ConnectionState::RECONNECTING: return "RECONNECTING";
    case ConnectionState::CONNECTION_ERROR: return "CONNECTION_ERROR";
    default: return "UNKNOWN";
    }
}

void OPCUAClient::setStateCallback(StateChangeCallback callback) {
    std::lock_guard<std::mutex> lock(callbackMutex_);
    stateCallback_ = callback;
}

void OPCUAClient::getStatistics(std::map<std::string, long long>& stats) {
    stats["totalReads"] = totalReads_.load();
    stats["totalWrites"] = totalWrites_.load();
    stats["totalArrayReads"] = totalArrayReads_.load();
    stats["totalArrayWrites"] = totalArrayWrites_.load();
    stats["failedReads"] = failedReads_.load();
    stats["failedWrites"] = failedWrites_.load();
    stats["failedArrayReads"] = failedArrayReads_.load();
    stats["failedArrayWrites"] = failedArrayWrites_.load();
    stats["avgReadLatencyUs"] = avgReadLatencyUs_.load();
    stats["avgWriteLatencyUs"] = avgWriteLatencyUs_.load();
    stats["maxReadLatencyUs"] = maxReadLatencyUs_.load();
    stats["maxWriteLatencyUs"] = maxWriteLatencyUs_.load();
    stats["totalArrayBytesRead"] = totalArrayBytesRead_.load();
    stats["totalArrayBytesWritten"] = totalArrayBytesWritten_.load();
}

std::string OPCUAClient::getPoolStatus() const {
    std::string status = "=== Connection Pool Status ===\n";
    int totalSessions = 0;
    int activeSessions = 0;
    int connectedConns = 0;

    for (size_t i = 0; i < connectionPool_.size(); ++i) {
        auto conn = connectionPool_[i];
        status += "Connection " + std::to_string(i) + ": ";
        status += conn->connected ? "[Connected]" : "[Disconnected]";
        status += " Sessions: " + std::to_string(conn->sessions.size());

        int active = 0;
        for (auto session : conn->sessions) {
            if (session->inUse) active++;
            totalSessions++;
            if (session->inUse) activeSessions++;
        }
        status += " Active: " + std::to_string(active) + "\n";
        if (conn->connected) connectedConns++;
    }

    status += "Connected Connections: " + std::to_string(connectedConns) + "/" +
        std::to_string(connectionPool_.size()) + "\n";
    status += "Total Sessions: " + std::to_string(totalSessions) + "\n";
    status += "Active Sessions: " + std::to_string(activeSessions) + "\n";
    status += "Task Queue Size: " + std::to_string(taskQueue_.size()) + "\n";
    status += "State: " + getStateString() + "\n";

    return status;
}

// ============================================================================
// 配置
// ============================================================================

void OPCUAClient::setConfig(const Config& config) {
    config_ = config;
}

void OPCUAClient::setTimeout(int timeoutMs) {
    config_.timeoutMs = timeoutMs;
    for (auto conn : connectionPool_) {
        UA_ClientConfig* cfg = UA_Client_getConfig(conn->client);
        if (cfg) cfg->timeout = timeoutMs;
        for (auto session : conn->sessions) {
            UA_ClientConfig* sessionCfg = UA_Client_getConfig(session->client);
            if (sessionCfg) sessionCfg->timeout = timeoutMs;
        }
    }
}

void OPCUAClient::setReconnectInterval(int intervalMs) {
    config_.reconnectIntervalMs = intervalMs;
}

void OPCUAClient::setMaxArraySize(size_t maxSize) {
    config_.maxArraySize = maxSize;
}

void OPCUAClient::resetStatistics() {
    totalReads_ = 0;
    totalWrites_ = 0;
    totalArrayReads_ = 0;
    totalArrayWrites_ = 0;
    failedReads_ = 0;
    failedWrites_ = 0;
    failedArrayReads_ = 0;
    failedArrayWrites_ = 0;
    avgReadLatencyUs_ = 0;
    avgWriteLatencyUs_ = 0;
    maxReadLatencyUs_ = 0;
    maxWriteLatencyUs_ = 0;
    totalArrayBytesRead_ = 0;
    totalArrayBytesWritten_ = 0;
}

// ============================================================================
// 订阅
// ============================================================================

bool OPCUAClient::subscribe(const std::string& nodeName,
    std::function<void(UA_Variant*)> callback,
    int intervalMs) {
    (void)intervalMs;

    UA_NodeId nodeId = parseNodeId(nodeName);
    if (UA_NodeId_isNull(&nodeId)) return false;

    std::unique_lock<std::shared_mutex> lock(subscriptionMutex_);

    Session* session = acquireSession();
    if (!session) {
        UA_NodeId_clear(&nodeId);
        return false;
    }

    auto sub = std::make_shared<Subscription>();
    sub->nodeId = nodeId;
    sub->client = session->client;
    sub->callback = callback;
    sub->active = true;
    sub->isArray = false;

    subscriptions_[nodeName] = sub;

    return true;
}

bool OPCUAClient::subscribeArray(const std::string& nodeName,
    std::function<void(UA_Variant*)> callback,
    int intervalMs) {
    (void)intervalMs;

    UA_NodeId nodeId = parseNodeId(nodeName);
    if (UA_NodeId_isNull(&nodeId)) return false;

    std::unique_lock<std::shared_mutex> lock(subscriptionMutex_);

    Session* session = acquireSession();
    if (!session) {
        UA_NodeId_clear(&nodeId);
        return false;
    }

    auto sub = std::make_shared<Subscription>();
    sub->nodeId = nodeId;
    sub->client = session->client;
    sub->callback = callback;
    sub->active = true;
    sub->isArray = true;

    subscriptions_[nodeName] = sub;

    return true;
}

bool OPCUAClient::unsubscribe(const std::string& nodeName) {
    std::unique_lock<std::shared_mutex> lock(subscriptionMutex_);
    auto it = subscriptions_.find(nodeName);
    if (it == subscriptions_.end()) return false;

    it->second->active = false;
    UA_NodeId_clear(&it->second->nodeId);
    subscriptions_.erase(it);

    return true;
}

// ============================================================================
// 数组信息
// ============================================================================

bool OPCUAClient::getArrayInfo(const std::string& nodeName,
    size_t& totalSize,
    std::vector<size_t>& dimensions,
    ArrayType& dataType,
    int timeoutMs) {
    UA_NodeId nodeId = parseNodeId(nodeName);
    if (UA_NodeId_isNull(&nodeId)) return false;

    Session* session = acquireSession();
    if (!session) return false;

    UA_Variant variant;
    UA_Variant_init(&variant);

    UA_ClientConfig* config = UA_Client_getConfig(session->client);
    int originalTimeout = config->timeout;
    config->timeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    UA_StatusCode status = UA_Client_readValueAttribute(session->client, nodeId, &variant);

    config->timeout = originalTimeout;
    UA_NodeId_clear(&nodeId);

    if (status != UA_STATUSCODE_GOOD || !UA_Variant_isArray(&variant)) {
        UA_Variant_clear(&variant);
        releaseSession(session);
        return false;
    }

    totalSize = variant.arrayLength;
    dataType = detectArrayType(&variant);

    dimensions.clear();
    if (variant.arrayDimensionsSize > 0) {
        for (size_t i = 0; i < variant.arrayDimensionsSize; ++i) {
            dimensions.push_back(variant.arrayDimensions[i]);
        }
    }
    else {
        dimensions.push_back(totalSize);
    }

    UA_Variant_clear(&variant);
    releaseSession(session);

    return true;
}