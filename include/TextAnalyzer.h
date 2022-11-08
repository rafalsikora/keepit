#pragma once

#include <atomic>
#include <future>
#include <vector>
#include <unordered_set>

#include "FileHandler.h"
#include "AlgorithmThreadWrapper.h"
#include "ProgramSettings.h"

class FileHandler;

class TextAnalyzer
{

public:
    explicit TextAnalyzer(const ProgramSettingsPtrConst& programSettingsPtr);
    ~TextAnalyzer() = default;
    
    [[nodiscard]] bool Run();
    void PrintResults() const;
    long GetNumberOfUniqueWords() const { return static_cast<long>(m_uniqueWords.size()); };
    const std::unordered_set<std::string>& GetUniqueWords() const { return m_uniqueWords; };

private:
    bool RunAllAnalyzers();
    void StartNewThreadsAndRun();
    bool WaitUntilThreadsExecute();
    void MergeResultsFromAllAnalyzers();

    std::atomic_uint 						      m_nThreadsReady;
    std::atomic_bool 						      m_abortFlag;
    const ProgramSettingsPtrConst 			   m_programSettings;
    FileHandlerPtr							      m_fileHandler;
    std::vector<AlgorithmThreadWrapperPtr> 	m_analyzers;
    std::vector<std::future<bool>>           m_analyzerReturns;
    std::unordered_set<std::string>          m_uniqueWords;
};
