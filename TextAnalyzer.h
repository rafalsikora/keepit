#pragma once

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
    void MergeResultsFromAllAnalyzers();

//    FileHandler				m_fileHandler;
//    OptionsValidator			m_optionsValidator;
    const ProgramSettingsPtr  	m_programSettings;
    std::vector<IAnalyzerAlgorithmPtr>		m_analyzers;
    long						m_nUniqueWords;
};
