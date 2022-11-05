#include "AnalyzerAlgorithmUnorderedSet.h"

#include <exception>
#include <iostream>

AnalyzerAlgorithmUnorderedSet::AnalyzerAlgorithmUnorderedSet()
	: m_thread{}, m_uniqueWords{}
{
}

AnalyzerAlgorithmUnorderedSet::~AnalyzerAlgorithmUnorderedSet()
{
	StopAndJoinThread();
}

bool AnalyzerAlgorithmUnorderedSet::StartNewThreadAndRun(std::atomic_int& done, std::atomic_bool& abort)
{
	try
	{
		m_thread = std::thread([&]()
		{
			m_uniqueWords.emplace("test");
			m_uniqueWords.emplace("test2");
			m_uniqueWords.emplace("test");
			++done;
		});
		return true;
	}
	catch(const std::exception& e)
	{
		std::cerr << "New thread in " << typeid(this).name() << " could not be started. Exception thrown: " << e.what() << std::endl;
		abort = true;
		++done;
		return false;
	}
}

void AnalyzerAlgorithmUnorderedSet::StopAndJoinThread()
{
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}
