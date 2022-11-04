#include "AnalyzerAlgorithmUnorderedSet.h"


AnalyzerAlgorithmUnorderedSet::AnalyzerAlgorithmUnorderedSet()
	: m_uniqueWords{}
{
}

bool AnalyzerAlgorithmUnorderedSet::Run()
{
	m_uniqueWords.emplace("test");
	m_uniqueWords.emplace("test2");
	m_uniqueWords.emplace("test");
	return true;
}
