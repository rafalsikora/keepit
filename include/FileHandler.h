#pragma once

#include "ProgramSettings.h"

#include <mutex>
#include <tuple>

class FileHandler
{

public:
    explicit FileHandler(const ProgramSettingsPtrConst& programSettingsPtr);
    ~FileHandler();

    bool Initialize();
    bool IsInitialized() const { return m_isInitialized; }
    void Finalize();
    std::tuple<const char*, size_t> GetPartOfText();

    static constexpr std::tuple<const char*, size_t> END_OF_FILE {nullptr, 0};

private:
    bool Open();
    bool Close();
    const char* MapFile();
    int UnmapFile();
    bool DetermineResourcesManagementPolicy();
    void UpdateText(const size_t& currentTextChunkLength);

    bool ReachedEOF() const;
    const char* GetChunkHead() const;
    size_t GetChunkLengthUntilNextWhitespaceOrEOF() const;

    std::mutex					         m_mutex;
    const ProgramSettingsPtrConst  	m_programSettings;
    bool						            m_isInitialized;
    int							         m_fileDescriptor;
    const char*					      m_fileData;
    size_t						         m_fileLengthTotal;
    size_t						         m_fileLengthProcessed;
    size_t						         m_singleChunkDefaultLength;
};

typedef std::shared_ptr<FileHandler> FileHandlerPtr;
