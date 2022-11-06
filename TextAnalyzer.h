#pragma once

#include <atomic>
#include <vector>

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

private:
    bool RunAllAnalyzers();
    bool StartNewThreadsAndRun();
    void WaitUntilThreadsExecute() const;
    void StopThreadsAndJoin();
    void MergeResultsFromAllAnalyzers();

    static const int 						m_waitLoopSleepMs;

    std::atomic_uint 						m_nThreadsReady{};
    std::atomic_bool 						m_abortFlag{false};
    const ProgramSettingsPtrConst 			m_programSettings;
    FileHandlerPtr							m_fileHandler;
    std::vector<AlgorithmThreadWrapperPtr> 	m_analyzers;
    long									m_nUniqueWords;
};
