#include "AnalyzerAlgorithm_Trie.h"

void AnalyzerAlgorithm_Trie::operator+=(const std::string& word)
{
}

const std::unordered_set<std::string>& AnalyzerAlgorithm_Trie::GetResult() const
{
	return m_uniqueWords;
}
