#pragma once

#include "ProgramSettings.h"

#include <map>
#include <mutex>
#include <tuple>

class FileHandler
{

public:
    explicit FileHandler(const ProgramSettingsPtrConst& programSettingsPtr);
    ~FileHandler();

    bool Initialize();
    bool IsInitialized() const { return m_isInitialized; }
    void ReleaseMemoryAll();
    std::tuple<const char*, size_t> GetPartOfText(const char* memoryToUnmap);

    static const std::tuple<const char*, size_t> FATAL_ERROR;
    static const std::tuple<const char*, size_t> END_OF_FILE;

private:
    bool Open();
    bool Close();
    bool DetermineResourcesManagementPolicy();
    size_t GetCurrentChunkLength() const;
    void RemoveTrailingTruncatedWords(size_t& textSizeUntilTheLastWhitespace) const;
    void UpdateFileRead(const char* addr, const size_t& currentChunkLength, const size_t& textLengthInCurrentChunk);
    void ReleaseMemory(const char* memoryToUnmap);
    void ReleaseMemory(const char* memoryToUnmap, const size_t size) const;

    std::mutex					m_mutex;
    const ProgramSettingsPtrConst  	m_programSettings;
    const size_t				m_memoryPageSize;
    bool						m_isInitialized;
    int							m_fileDescriptor;
    size_t						m_fileLengthTotal;
    size_t						m_fileLengthProcessed;
    size_t						m_singleChunkDefaultLength;
    std::map<const char*, size_t> m_mappedMemory;
};

typedef std::shared_ptr<FileHandler> FileHandlerPtr;
