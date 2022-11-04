#include "TextAnalyzer.h"

#include <iostream>
#include <string>
#include <unordered_set>

#include "AnalyzerAlgorithmFactory.h"

TextAnalyzer::TextAnalyzer(const ProgramSettingsPtr& programSettings)
    : m_programSettings{programSettings},
	  m_analyzers{},
	  m_nUniqueWords{}
{
	m_analyzers = AnalyzerAlgorithmFactory::Create(m_programSettings);
}

bool TextAnalyzer::Run()
{
	const bool status = RunAllAnalyzers();
	if (status)
	{
		MergeResultsFromAllAnalyzers();
	}
	else
	{
		std::cerr << "Error during text analysis." << std::endl;
	}
	return status;
}

bool TextAnalyzer::RunAllAnalyzers() // MULTITHREADING HERE //ALERT
{
	bool status = true;
	for (auto& analyzer : m_analyzers)
	{
		bool result = analyzer->Run();
		status = status && result;
	}

	return status;
}

void TextAnalyzer::MergeResultsFromAllAnalyzers()
{
	std::unordered_set<std::string> mergedResults;
	for (const auto& analyzer : m_analyzers)
	{
		std::unordered_set<std::string> singleResult{analyzer->GetResult()};
		mergedResults.merge(singleResult);
	}
	m_nUniqueWords = static_cast<long>(mergedResults.size());
}

void TextAnalyzer::PrintResults() const
{
	std::cout << "UNIQUE WORDS:\t" << m_nUniqueWords << std::endl;
}
