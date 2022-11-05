#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <unordered_set>

#include "IAnalyzerAlgorithm.h"
#include "FileHandler.h"

class AlgorithmThreadWrapper : public IAnalyzerAlgorithm
{
public:
	explicit AlgorithmThreadWrapper(const FileHandlerPtr& fileHandler);
    ~AlgorithmThreadWrapper() override;

    bool StartNewThreadAndRun(std::atomic_uint& done, std::atomic_bool& abort) override;
    void StopAndJoinThread() override;
    const std::unordered_set<std::string>& GetResult() const override { return m_uniqueWords; }

private:
    bool GetNextWord(std::string& word);

    FileHandlerPtr 					m_fileHandler;
    std::thread 					m_thread;
    std::unordered_set<std::string> m_uniqueWords;
    const char*						m_text;
    size_t							m_textTotalSize;
    const char*						m_lastNonWhiteChar;
    bool							m_isNewText;
    size_t							m_textIterator;
};
