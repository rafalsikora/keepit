#include "TextAnalyzer.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_set>

#include "AnalyzerAlgorithmFactory.h"
#include "FileHandler.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

TextAnalyzer::TextAnalyzer(const ProgramSettingsPtrConst& programSettings)
    :	m_nThreadsReady{},
		m_abortFlag{false},
		m_programSettings{programSettings},
		m_fileHandler{},
		m_analyzers{},
		m_analyzerReturns{},
		m_uniqueWords{}
{
	m_fileHandler.reset(new FileHandler(m_programSettings));
	m_analyzers = AnalyzerAlgorithmFactory::Create(m_programSettings, m_fileHandler);
}

bool TextAnalyzer::Run()
{
	bool status = m_fileHandler->Initialize();
	if (status)
	{
		status = RunAllAnalyzers();

		if (status)
		{
			MergeResultsFromAllAnalyzers();
		}
		else
		{
			std::cerr << "Error during text analysis." << std::endl;
		}
	}
	else
	{
		std::cerr << "Error during file handler initialization." << std::endl;
	}
	m_fileHandler->Finalize();
	return status;
}

bool TextAnalyzer::RunAllAnalyzers()
{
	auto start = steady_clock::now();
	StartNewThreadsAndRun();
	bool status = WaitUntilThreadsExecute();
	if (status)
	{
		double executionTime = duration_cast<milliseconds>(steady_clock::now()-start).count()/1000.;
		std::cout << "Execution time [s]: " << executionTime << std::endl;
	}
	return status && !m_abortFlag;
}

void TextAnalyzer::StartNewThreadsAndRun()
{
	for (auto& analyzer : m_analyzers)
	{
	   m_analyzerReturns.push_back(analyzer->StartNewThreadAndRun(m_abortFlag));
	}
}

bool TextAnalyzer::WaitUntilThreadsExecute()
{
   bool status = true;
   for (auto& returnValue : m_analyzerReturns)
   {
      returnValue.wait();
      status = status && returnValue.get();
   }
   if (!status)
   {
      std::cerr << "Error while running the analyzer. The text analysis will now be stopped." << std::endl;
   }
   return status;
}

void TextAnalyzer::MergeResultsFromAllAnalyzers()
{
	for (const auto& analyzer : m_analyzers)
	{
		std::unordered_set<std::string> singleResult{analyzer->GetResult()};
		m_uniqueWords.merge(singleResult);
	}
}

void TextAnalyzer::PrintResults() const
{
	std::cout << "UNIQUE WORDS:\t" << GetNumberOfUniqueWords() << std::endl;
}
