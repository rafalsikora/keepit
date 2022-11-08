#pragma once

#include <atomic>
#include <string>
#include <future>
#include <memory>
#include <unordered_set>

#include "IAnalyzerAlgorithm.h"
#include "FileHandler.h"

class AlgorithmThreadWrapper
{
public:
    explicit AlgorithmThreadWrapper(const FileHandlerPtr& fileHandler, const IAnalyzerAlgorithmPtr& algorithm);
    ~AlgorithmThreadWrapper() {};

    std::future<bool> StartNewThreadAndRun(std::atomic_bool& abort);
    const std::unordered_set<std::string>& GetResult() const { return m_algorithm->GetResult(); }

private:
    bool GetNextWord(std::string& word);

    FileHandlerPtr 					      m_fileHandler;
    IAnalyzerAlgorithmPtr		      	m_algorithm;
    const char*						      m_text;
    size_t						         	m_textTotalSize;
    const char*					      	m_lastNonWhiteChar;
    bool							            m_isNewText;
    size_t							         m_textIterator;
};

typedef std::shared_ptr<AlgorithmThreadWrapper> AlgorithmThreadWrapperPtr;
