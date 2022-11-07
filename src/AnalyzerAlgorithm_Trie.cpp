#include "AnalyzerAlgorithm_Trie.h"

#include <iostream>

AnalyzerAlgorithm_Trie::AnalyzerAlgorithm_Trie()
   :  m_root{}, m_uniqueWords{}
{
}

void AnalyzerAlgorithm_Trie::operator+=(const std::string& word)
{
   Node* currentNode = &m_root;

   for (const auto& ch : word)
   {
      const auto index = static_cast<unsigned int>(ch) - AnalyzerAlgorithm_Trie::Node::FIRST_INDEX;
      currentNode = currentNode->m_children[index].get() ?
                     currentNode->m_children[index].get() :
                     (currentNode->m_children[index] = std::make_unique<Node>()).get();
   }

   if( !currentNode->m_representsFullWord )
   {
      currentNode->m_representsFullWord = true;
      m_uniqueWords.emplace(word);
   }
}

const std::unordered_set<std::string>& AnalyzerAlgorithm_Trie::GetResult() const
{
	return m_uniqueWords;
}

AnalyzerAlgorithm_Trie::Node::Node()
   :  m_children{}, m_representsFullWord{false}
{
}


