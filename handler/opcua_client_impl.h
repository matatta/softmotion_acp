#ifndef OPCUA_CLIENT_IMPL_H
#define OPCUA_CLIENT_IMPL_H

#include "opcua_client.h"
#include <chrono>
#include <iostream>
#include <cstring>

// ============================================================================
// 标量读写模板实现
// ============================================================================

template<typename T>
ReadResult<T> OPCUAClient::readSync(const std::string& nodeName, int timeoutMs) {
    auto startTime = std::chrono::steady_clock::now();
    totalReads_++;

    ReadResult<T> result;
    int actualTimeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    Session* session = acquireSession();
    if (!session) {
        failedReads_++;
        result.success = false;
        result.errorMsg = "No available session";
        return result;
    }

    try {
        bool success = readInternal(session->client, nodeName, result.value, actualTimeout);
        result.success = success;
        if (!success) {
            result.errorMsg = "Read failed";
            failedReads_++;
        }
    }
    catch (const std::exception& e) {
        result.success = false;
        result.errorMsg = e.what();
        failedReads_++;
    }

    releaseSession(session);

    auto endTime = std::chrono::steady_clock::now();
    auto latency = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    result.latencyUs = latency.count();
    updateLatencyStats(latency.count(), true);

    return result;
}

template<typename T>
bool OPCUAClient::writeSync(const std::string& nodeName, const T& value, int timeoutMs) {
    auto startTime = std::chrono::steady_clock::now();
    totalWrites_++;

    int actualTimeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    Session* session = acquireSession();
    if (!session) {
        failedWrites_++;
        return false;
    }

    bool success = writeInternal(session->client, nodeName, value, actualTimeout);
    if (!success) {
        failedWrites_++;
    }

    releaseSession(session);

    auto endTime = std::chrono::steady_clock::now();
    auto latency = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    updateLatencyStats(latency.count(), false);

    return success;
}

template<typename T>
void OPCUAClient::readAsync(const std::string& nodeName,
    std::function<void(const ReadResult<T>&)> callback,
    int timeoutMs) {
    auto task = std::make_shared<AsyncTask>();
    task->type = AsyncTask::READ;
    task->nodeName = nodeName;
    task->timeoutMs = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;
    task->enqueueTime = std::chrono::steady_clock::now();
    task->promise = std::make_shared<std::promise<void>>();

    task->readCallback = [callback, this](bool success, void* data, size_t size, size_t arraySize) {
        ReadResult<T> result;
        result.success = success;
        if (success && data) {
            result.value = *(T*)data;
        }
        callback(result);
        };

    std::lock_guard<std::mutex> lock(queueMutex_);
    taskQueue_.push(task);
    queueCond_.notify_one();
}

template<typename T>
void OPCUAClient::writeAsync(const std::string& nodeName, const T& value,
    std::function<void(bool)> callback,
    int timeoutMs) {
    auto task = std::make_shared<AsyncTask>();
    task->type = AsyncTask::WRITE;
    task->nodeName = nodeName;
    task->data = new T(value);
    task->dataSize = sizeof(T);
    task->timeoutMs = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;
    task->enqueueTime = std::chrono::steady_clock::now();
    task->writeCallback = callback;
    task->promise = std::make_shared<std::promise<void>>();

    std::lock_guard<std::mutex> lock(queueMutex_);
    taskQueue_.push(task);
    queueCond_.notify_one();
}

// ============================================================================
// 数组读写模板实现
// ============================================================================

template<typename T>
ArrayReadResult<T> OPCUAClient::readArray(const std::string& nodeName, int timeoutMs) {
    auto startTime = std::chrono::steady_clock::now();
    totalArrayReads_++;

    ArrayReadResult<T> result;
    int actualTimeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    Session* session = acquireSession();
    if (!session) {
        failedArrayReads_++;
        result.success = false;
        result.errorMsg = "No available session";
        return result;
    }

    try {
        std::vector<T> values;
        std::vector<size_t> dimensions;

        bool success = readArrayInternal(session->client, nodeName, values, dimensions, actualTimeout);

        if (success) {
            result.success = true;
            result.values = std::move(values);
            result.dimensions = std::move(dimensions);
            result.isMultiDimensional = (result.dimensions.size() > 1);
        }
        else {
            result.success = false;
            result.errorMsg = "Read array failed";
            failedArrayReads_++;
        }
    }
    catch (const std::exception& e) {
        result.success = false;
        result.errorMsg = e.what();
        failedArrayReads_++;
    }

    releaseSession(session);

    auto endTime = std::chrono::steady_clock::now();
    auto latency = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    result.latencyUs = latency.count();
    updateLatencyStats(latency.count(), true);

    totalArrayBytesRead_ += static_cast<long long>(result.values.size() * sizeof(T));

    return result;
}

template<typename T>
void OPCUAClient::readArrayAsync(const std::string& nodeName,
    std::function<void(const ArrayReadResult<T>&)> callback,
    int timeoutMs) {
    auto task = std::make_shared<AsyncTask>();
    task->type = AsyncTask::READ_ARRAY;
    task->nodeName = nodeName;
    task->timeoutMs = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;
    task->enqueueTime = std::chrono::steady_clock::now();
    task->promise = std::make_shared<std::promise<void>>();

    task->arrayReadCallback = [callback, this](bool success, void* data, size_t elementSize,
        size_t elementCount, const std::vector<size_t>& dimensions) {
            ArrayReadResult<T> result;
            result.success = success;
            result.dimensions = dimensions;
            result.isMultiDimensional = (dimensions.size() > 1);

            if (success && data && elementCount > 0) {
                T* ptr = static_cast<T*>(data);
                result.values.assign(ptr, ptr + elementCount);
            }
            callback(result);
        };

    std::lock_guard<std::mutex> lock(queueMutex_);
    taskQueue_.push(task);
    queueCond_.notify_one();
}

template<typename T>
bool OPCUAClient::writeArray(const std::string& nodeName,
    const ArrayWriteData<T>& data,
    int timeoutMs) {
    auto startTime = std::chrono::steady_clock::now();
    totalArrayWrites_++;

    int actualTimeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    Session* session = acquireSession();
    if (!session) {
        failedArrayWrites_++;
        return false;
    }

    bool success = writeArrayInternal(session->client, nodeName, data.values,
        data.dimensions, data.isMultiDimensional, actualTimeout);

    if (!success) {
        failedArrayWrites_++;
    }

    releaseSession(session);

    auto endTime = std::chrono::steady_clock::now();
    auto latency = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    updateLatencyStats(latency.count(), false);

    if (success) {
        totalArrayBytesWritten_ += static_cast<long long>(data.values.size() * sizeof(T));
    }

    return success;
}

template<typename T>
bool OPCUAClient::writeArray(const std::string& nodeName,
    const std::vector<T>& values,
    int timeoutMs) {
    ArrayWriteData<T> data(values);
    return writeArray(nodeName, data, timeoutMs);
}

template<typename T>
void OPCUAClient::writeArrayAsync(const std::string& nodeName,
    const ArrayWriteData<T>& data,
    std::function<void(bool)> callback,
    int timeoutMs) {
    auto task = std::make_shared<AsyncTask>();
    task->type = AsyncTask::WRITE_ARRAY;
    task->nodeName = nodeName;
    task->data = new std::vector<T>(data.values);
    task->dataSize = data.values.size() * sizeof(T);
    task->arraySize = data.values.size();
    task->dimensions = data.dimensions;
    task->timeoutMs = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;
    task->enqueueTime = std::chrono::steady_clock::now();
    task->writeCallback = callback;
    task->promise = std::make_shared<std::promise<void>>();

    std::lock_guard<std::mutex> lock(queueMutex_);
    taskQueue_.push(task);
    queueCond_.notify_one();
}

template<typename T>
ArrayReadResult<T> OPCUAClient::readArrayRange(const std::string& nodeName,
    size_t startIndex,
    size_t count,
    int timeoutMs) {
    auto startTime = std::chrono::steady_clock::now();
    totalArrayReads_++;

    ArrayReadResult<T> result;
    int actualTimeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    if (count > config_.maxArraySize) {
        result.success = false;
        result.errorMsg = "Requested count exceeds max array size";
        return result;
    }

    Session* session = acquireSession();
    if (!session) {
        failedArrayReads_++;
        result.success = false;
        result.errorMsg = "No available session";
        return result;
    }

    try {
        std::vector<T> values;
        bool success = readArrayRangeInternal(session->client, nodeName, startIndex, count, values, actualTimeout);

        if (success) {
            result.success = true;
            result.values = std::move(values);
            result.dimensions = { values.size() };
        }
        else {
            result.success = false;
            result.errorMsg = "Read array range failed";
            failedArrayReads_++;
        }
    }
    catch (const std::exception& e) {
        result.success = false;
        result.errorMsg = e.what();
        failedArrayReads_++;
    }

    releaseSession(session);

    auto endTime = std::chrono::steady_clock::now();
    auto latency = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    result.latencyUs = latency.count();
    updateLatencyStats(latency.count(), true);

    return result;
}

// ============================================================================
// 批量操作模板实现
// ============================================================================

template<typename T>
std::vector<ReadResult<T>> OPCUAClient::readBatch(const std::vector<std::string>& nodeNames,
    int timeoutMs) {
    std::vector<ReadResult<T>> results;
    results.reserve(nodeNames.size());

    int actualTimeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    Session* session = acquireSession();
    if (!session) {
        for (size_t i = 0; i < nodeNames.size(); ++i) {
            ReadResult<T> result;
            result.success = false;
            result.errorMsg = "No available session";
            results.push_back(result);
        }
        return results;
    }

    for (const auto& name : nodeNames) {
        T value;
        bool success = readInternal(session->client, name, value, actualTimeout);
        ReadResult<T> result;
        result.success = success;
        if (success) {
            result.value = value;
        }
        else {
            result.errorMsg = "Read failed";
        }
        results.push_back(result);
    }

    releaseSession(session);
    return results;
}

template<typename T>
std::vector<bool> OPCUAClient::writeBatch(const std::vector<std::string>& nodeNames,
    const std::vector<T>& values,
    int timeoutMs) {
    if (nodeNames.size() != values.size()) {
        return std::vector<bool>(nodeNames.size(), false);
    }

    std::vector<bool> results(nodeNames.size(), false);
    int actualTimeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    Session* session = acquireSession();
    if (!session) return results;

    for (size_t i = 0; i < nodeNames.size(); ++i) {
        results[i] = writeInternal(session->client, nodeNames[i], values[i], actualTimeout);
    }

    releaseSession(session);
    return results;
}

template<typename T>
std::vector<ArrayReadResult<T>> OPCUAClient::readArrayBatch(const std::vector<std::string>& nodeNames,
    int timeoutMs) {
    std::vector<ArrayReadResult<T>> results;
    results.reserve(nodeNames.size());

    int actualTimeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    Session* session = acquireSession();
    if (!session) {
        for (size_t i = 0; i < nodeNames.size(); ++i) {
            ArrayReadResult<T> result;
            result.success = false;
            result.errorMsg = "No available session";
            results.push_back(result);
        }
        return results;
    }

    for (const auto& name : nodeNames) {
        std::vector<T> values;
        std::vector<size_t> dimensions;

        bool success = readArrayInternal(session->client, name, values, dimensions, actualTimeout);

        ArrayReadResult<T> result;
        result.success = success;
        if (success) {
            result.values = std::move(values);
            result.dimensions = std::move(dimensions);
            result.isMultiDimensional = (result.dimensions.size() > 1);
        }
        else {
            result.errorMsg = "Read array failed";
        }
        results.push_back(std::move(result));
    }

    releaseSession(session);
    return results;
}

template<typename T>
std::vector<bool> OPCUAClient::writeArrayBatch(const std::vector<std::string>& nodeNames,
    const std::vector<ArrayWriteData<T>>& datas,
    int timeoutMs) {
    if (nodeNames.size() != datas.size()) {
        return std::vector<bool>(nodeNames.size(), false);
    }

    std::vector<bool> results(nodeNames.size(), false);
    int actualTimeout = (timeoutMs > 0) ? timeoutMs : config_.timeoutMs;

    Session* session = acquireSession();
    if (!session) return results;

    for (size_t i = 0; i < nodeNames.size(); ++i) {
        results[i] = writeArrayInternal(session->client, nodeNames[i],
            datas[i].values, datas[i].dimensions,
            datas[i].isMultiDimensional,
            actualTimeout);
    }

    releaseSession(session);
    return results;
}

// ============================================================================
// 内部读写实现
// ============================================================================

template<typename T>
bool OPCUAClient::readInternal(UA_Client* client, const std::string& nodeName, T& value, int timeoutMs) {
    UA_NodeId nodeId;
    {
        std::shared_lock<std::shared_mutex> lock(cacheMutex_);
        auto it = nodeIdCache_.find(nodeName);
        if (it != nodeIdCache_.end()) {
            nodeId = it->second;
        }
    }

    if (UA_NodeId_isNull(&nodeId)) {
        nodeId = parseNodeId(nodeName);
        if (UA_NodeId_isNull(&nodeId)) return false;

        std::unique_lock<std::shared_mutex> lock(cacheMutex_);
        if (config_.enableCache) {
            nodeIdCache_[nodeName] = nodeId;
        }
    }

    UA_Variant variant;
    UA_Variant_init(&variant);

    UA_ClientConfig* config = UA_Client_getConfig(client);
    int originalTimeout = config->timeout;
    config->timeout = timeoutMs;

    UA_StatusCode status = UA_Client_readValueAttribute(client, nodeId, &variant);

    config->timeout = originalTimeout;

    if (status != UA_STATUSCODE_GOOD) {
        UA_Variant_clear(&variant);
        return false;
    }

    bool success = false;
    if (variant.type) {
        success = convertVariantToValue(&variant, &value,
            getDataTypeForTypeKind(variant.type->typeKind));
    }

    UA_Variant_clear(&variant);
    return success;
}

template<typename T>
bool OPCUAClient::writeInternal(UA_Client* client, const std::string& nodeName,
    const T& value, int timeoutMs) {
    UA_NodeId nodeId = parseNodeId(nodeName);
    if (UA_NodeId_isNull(&nodeId)) return false;

    UA_Variant variant;
    UA_Variant_init(&variant);

    UA_DataType* dataType = nullptr;
    if constexpr (std::is_same_v<T, bool>) dataType = &UA_TYPES[UA_TYPES_BOOLEAN];
    else if constexpr (std::is_same_v<T, int8_t>) dataType = &UA_TYPES[UA_TYPES_SBYTE];
    else if constexpr (std::is_same_v<T, uint8_t>) dataType = &UA_TYPES[UA_TYPES_BYTE];
    else if constexpr (std::is_same_v<T, int16_t>) dataType = &UA_TYPES[UA_TYPES_INT16];
    else if constexpr (std::is_same_v<T, uint16_t>) dataType = &UA_TYPES[UA_TYPES_UINT16];
    else if constexpr (std::is_same_v<T, int32_t>) dataType = &UA_TYPES[UA_TYPES_INT32];
    else if constexpr (std::is_same_v<T, uint32_t>) dataType = &UA_TYPES[UA_TYPES_UINT32];
    else if constexpr (std::is_same_v<T, int64_t>) dataType = &UA_TYPES[UA_TYPES_INT64];
    else if constexpr (std::is_same_v<T, uint64_t>) dataType = &UA_TYPES[UA_TYPES_UINT64];
    else if constexpr (std::is_same_v<T, float>) dataType = &UA_TYPES[UA_TYPES_FLOAT];
    else if constexpr (std::is_same_v<T, double>) dataType = &UA_TYPES[UA_TYPES_DOUBLE];
    else if constexpr (std::is_same_v<T, std::string>) dataType = &UA_TYPES[UA_TYPES_STRING];
    else return false;

    UA_Variant_setScalar(&variant, (void*)&value, dataType);

    UA_ClientConfig* config = UA_Client_getConfig(client);
    int originalTimeout = config->timeout;
    config->timeout = timeoutMs;

    UA_StatusCode status = UA_Client_writeValueAttribute(client, nodeId, &variant);

    config->timeout = originalTimeout;
    UA_NodeId_clear(&nodeId);

    return status == UA_STATUSCODE_GOOD;
}

// ============================================================================
// 内部数组读写实现
// ============================================================================

template<typename T>
bool OPCUAClient::readArrayInternal(UA_Client* client, const std::string& nodeName,
    std::vector<T>& values,
    std::vector<size_t>& dimensions,
    int timeoutMs) {
    UA_NodeId nodeId;
    {
        std::shared_lock<std::shared_mutex> lock(cacheMutex_);
        auto it = nodeIdCache_.find(nodeName);
        if (it != nodeIdCache_.end()) {
            nodeId = it->second;
        }
    }

    if (UA_NodeId_isNull(&nodeId)) {
        nodeId = parseNodeId(nodeName);
        if (UA_NodeId_isNull(&nodeId)) return false;

        std::unique_lock<std::shared_mutex> lock(cacheMutex_);
        if (config_.enableCache) {
            nodeIdCache_[nodeName] = nodeId;
        }
    }

    UA_Variant variant;
    UA_Variant_init(&variant);

    UA_ClientConfig* config = UA_Client_getConfig(client);
    int originalTimeout = config->timeout;
    config->timeout = timeoutMs;

    UA_StatusCode status = UA_Client_readValueAttribute(client, nodeId, &variant);

    config->timeout = originalTimeout;

    if (status != UA_STATUSCODE_GOOD || !UA_Variant_isArray(&variant)) {
        UA_Variant_clear(&variant);
        return false;
    }

    size_t arraySize = variant.arrayLength;
    if (arraySize > config_.maxArraySize) {
        UA_Variant_clear(&variant);
        return false;
    }

    values.clear();
    values.reserve(arraySize);

    UA_DataType* dataType = variant.type;
    if (!dataType) {
        UA_Variant_clear(&variant);
        return false;
    }

    bool success = true;
    try {
        switch (dataType->typeKind) {
        case UA_DATATYPEKIND_BOOLEAN: {
            UA_Boolean* arr = (UA_Boolean*)variant.data;
            for (size_t i = 0; i < arraySize; ++i) {
                if constexpr (std::is_same_v<T, bool>) {
                    values.push_back(arr[i] ? true : false);
                }
                else {
                    values.push_back(static_cast<T>(arr[i]));
                }
            }
            break;
        }
        case UA_DATATYPEKIND_INT32: {
            UA_Int32* arr = (UA_Int32*)variant.data;
            for (size_t i = 0; i < arraySize; ++i) {
                if constexpr (std::is_same_v<T, int32_t>) {
                    values.push_back(arr[i]);
                }
                else {
                    values.push_back(static_cast<T>(arr[i]));
                }
            }
            break;
        }
        case UA_DATATYPEKIND_UINT32: {
            UA_UInt32* arr = (UA_UInt32*)variant.data;
            for (size_t i = 0; i < arraySize; ++i) {
                if constexpr (std::is_same_v<T, uint32_t>) {
                    values.push_back(arr[i]);
                }
                else {
                    values.push_back(static_cast<T>(arr[i]));
                }
            }
            break;
        }
        case UA_DATATYPEKIND_INT64: {
            UA_Int64* arr = (UA_Int64*)variant.data;
            for (size_t i = 0; i < arraySize; ++i) {
                if constexpr (std::is_same_v<T, int64_t>) {
                    values.push_back(arr[i]);
                }
                else {
                    values.push_back(static_cast<T>(arr[i]));
                }
            }
            break;
        }
        case UA_DATATYPEKIND_UINT64: {
            UA_UInt64* arr = (UA_UInt64*)variant.data;
            for (size_t i = 0; i < arraySize; ++i) {
                if constexpr (std::is_same_v<T, uint64_t>) {
                    values.push_back(arr[i]);
                }
                else {
                    values.push_back(static_cast<T>(arr[i]));
                }
            }
            break;
        }
        case UA_DATATYPEKIND_FLOAT: {
            UA_Float* arr = (UA_Float*)variant.data;
            for (size_t i = 0; i < arraySize; ++i) {
                if constexpr (std::is_same_v<T, float>) {
                    values.push_back(arr[i]);
                }
                else {
                    values.push_back(static_cast<T>(arr[i]));
                }
            }
            break;
        }
        case UA_DATATYPEKIND_DOUBLE: {
            UA_Double* arr = (UA_Double*)variant.data;
            for (size_t i = 0; i < arraySize; ++i) {
                if constexpr (std::is_same_v<T, double>) {
                    values.push_back(arr[i]);
                }
                else {
                    values.push_back(static_cast<T>(arr[i]));
                }
            }
            break;
        }
        case UA_DATATYPEKIND_STRING: {
            UA_String* arr = (UA_String*)variant.data;
            for (size_t i = 0; i < arraySize; ++i) {
                if constexpr (std::is_same_v<T, std::string>) {
                    values.push_back(std::string((char*)arr[i].data, arr[i].length));
                }
            }
            break;
        }
        default:
            success = false;
            break;
        }
    }
    catch (...) {
        success = false;
    }

    dimensions.clear();
    if (variant.arrayDimensionsSize > 0) {
        for (size_t i = 0; i < variant.arrayDimensionsSize; ++i) {
            dimensions.push_back(variant.arrayDimensions[i]);
        }
    }
    else {
        dimensions.push_back(arraySize);
    }

    UA_Variant_clear(&variant);
    return success;
}

template<typename T>
bool OPCUAClient::writeArrayInternal(UA_Client* client, const std::string& nodeName,
    const std::vector<T>& values,
    const std::vector<size_t>& dimensions,
    bool isMultiDimensional,
    int timeoutMs) {
    if (values.size() > config_.maxArraySize) {
        return false;
    }

    UA_NodeId nodeId = parseNodeId(nodeName);
    if (UA_NodeId_isNull(&nodeId)) return false;

    UA_DataType* dataType = nullptr;
    if constexpr (std::is_same_v<T, bool>) dataType = &UA_TYPES[UA_TYPES_BOOLEAN];
    else if constexpr (std::is_same_v<T, int8_t>) dataType = &UA_TYPES[UA_TYPES_SBYTE];
    else if constexpr (std::is_same_v<T, uint8_t>) dataType = &UA_TYPES[UA_TYPES_BYTE];
    else if constexpr (std::is_same_v<T, int16_t>) dataType = &UA_TYPES[UA_TYPES_INT16];
    else if constexpr (std::is_same_v<T, uint16_t>) dataType = &UA_TYPES[UA_TYPES_UINT16];
    else if constexpr (std::is_same_v<T, int32_t>) dataType = &UA_TYPES[UA_TYPES_INT32];
    else if constexpr (std::is_same_v<T, uint32_t>) dataType = &UA_TYPES[UA_TYPES_UINT32];
    else if constexpr (std::is_same_v<T, int64_t>) dataType = &UA_TYPES[UA_TYPES_INT64];
    else if constexpr (std::is_same_v<T, uint64_t>) dataType = &UA_TYPES[UA_TYPES_UINT64];
    else if constexpr (std::is_same_v<T, float>) dataType = &UA_TYPES[UA_TYPES_FLOAT];
    else if constexpr (std::is_same_v<T, double>) dataType = &UA_TYPES[UA_TYPES_DOUBLE];
    else if constexpr (std::is_same_v<T, std::string>) dataType = &UA_TYPES[UA_TYPES_STRING];
    else return false;

    UA_Variant variant;
    UA_Variant_init(&variant);

    void* arrayData = UA_Array_new(values.size(), dataType);
    if (!arrayData) return false;

    if constexpr (std::is_same_v<T, std::string>) {
        UA_String* strArray = (UA_String*)arrayData;
        for (size_t i = 0; i < values.size(); ++i) {
            UA_String_clear(&strArray[i]);
            UA_String_allocBuffer(&strArray[i], values[i].size());
            memcpy(strArray[i].data, values[i].c_str(), values[i].size());
            strArray[i].length = values[i].size();
        }
    }
    else {
        memcpy(arrayData, values.data(), values.size() * sizeof(T));
    }

    UA_Variant_setArray(&variant, arrayData, values.size(), dataType);

    if (isMultiDimensional && !dimensions.empty()) {
        variant.arrayDimensionsSize = dimensions.size();
        variant.arrayDimensions = (UA_UInt32*)UA_Array_new(dimensions.size(), &UA_TYPES[UA_TYPES_UINT32]);
        for (size_t i = 0; i < dimensions.size(); ++i) {
            variant.arrayDimensions[i] = static_cast<UA_UInt32>(dimensions[i]);
        }
    }

    UA_ClientConfig* config = UA_Client_getConfig(client);
    int originalTimeout = config->timeout;
    config->timeout = timeoutMs;

    UA_StatusCode status = UA_Client_writeValueAttribute(client, nodeId, &variant);

    config->timeout = originalTimeout;
    UA_NodeId_clear(&nodeId);
    UA_Variant_clear(&variant);

    return status == UA_STATUSCODE_GOOD;
}

template<typename T>
bool OPCUAClient::readArrayRangeInternal(UA_Client* client, const std::string& nodeName,
    size_t startIndex, size_t count,
    std::vector<T>& values, int timeoutMs) {
    std::vector<size_t> dimensions;
    bool success = readArrayInternal(client, nodeName, values, dimensions, timeoutMs);

    if (!success) return false;

    if (startIndex < values.size()) {
        size_t endIndex = std::min(startIndex + count, values.size());
        std::vector<T> sliced;
        sliced.reserve(endIndex - startIndex);
        for (size_t i = startIndex; i < endIndex; ++i) {
            sliced.push_back(values[i]);
        }
        values = std::move(sliced);
        return true;
    }

    values.clear();
    return false;
}

#endif // OPCUA_CLIENT_IMPL_H