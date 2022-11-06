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

const int TextAnalyzer::m_waitLoopSleepMs{10};

TextAnalyzer::TextAnalyzer(const ProgramSettingsPtrConst& programSettings)
    :	m_nThreadsReady{},
		m_abortFlag{false},
		m_programSettings{programSettings},
		m_fileHandler{},
		m_analyzers{},
		m_nUniqueWords{}
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
	bool status = StartNewThreadsAndRun();
	WaitUntilThreadsExecute();
	StopThreadsAndJoin();
	if (status)
	{
		double executionTime = duration_cast<milliseconds>(steady_clock::now()-start).count()/1000.;
		std::cout << "Execution time [s]: " << executionTime << std::endl;
	}
	return status && !m_abortFlag;
}

bool TextAnalyzer::StartNewThreadsAndRun()
{
	for (auto& analyzer : m_analyzers)
	{
		if (!analyzer->StartNewThreadAndRun(m_nThreadsReady, m_abortFlag))
		{
			std::cerr << "Error while running the analyzer. The text analysis will now be stopped." << std::endl;
			return false;
		}
	}
	return true;
}

void TextAnalyzer::WaitUntilThreadsExecute() const
{
	while (m_nThreadsReady.load() < m_programSettings->m_nThreads)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(m_waitLoopSleepMs));
	}
}

void TextAnalyzer::StopThreadsAndJoin()
{
	for (auto& analyzer : m_analyzers)
	{
		analyzer->StopAndJoinThread();
	}
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
