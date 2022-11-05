#pragma once

#include <atomic>
#include <vector>

#include "IAnalyzerAlgorithm.h"
#include "ProgramSettings.h"

class TextAnalyzer
{

public:
    explicit TextAnalyzer(const ProgramSettingsPtr& programSettingsPtr);
    ~TextAnalyzer() = default;
    
    [[nodiscard]] bool Run();
    void PrintResults() const;

private:
    bool RunAllAnalyzers();
    bool StartNewThreadsAndRun();
    void WaitUntilThreadsExecute() const;
    void MergeResultsFromAllAnalyzers();

    static const int 			m_waitLoopSleepMs;

    std::atomic_int 			m_nThreadsReady{};
    std::atomic_bool 			m_abortFlag{false};
//    FileHandler				m_fileHandler;
//    OptionsValidator			m_optionsValidator;
    const ProgramSettingsPtr  	m_programSettings;
    std::vector<IAnalyzerAlgorithmPtr>		m_analyzers;
    long						m_nUniqueWords;
};
