#pragma once

#include "IAnalyzerAlgorithm.h"

#include <memory>
#include <string>
#include <unordered_set>

class AnalyzerAlgorithm_Trie : public IAnalyzerAlgorithm
{
public:
    AnalyzerAlgorithm_Trie();
    ~AnalyzerAlgorithm_Trie() override = default;

    void operator+=(const std::string& word) override;
    const std::unordered_set<std::string>& GetResult() const override;

private:
    struct Node
    {
       // now covering all letters (small and capitals), don't care about intermediate signs
       // if were to work solely on small letters then FIRST_INDEX {'a'};
       static constexpr unsigned int FIRST_INDEX {'A'};
       static constexpr unsigned int LAST_INDEX {'z'};
       static_assert( static_cast<int>(LAST_INDEX) - static_cast<int>(FIRST_INDEX) > 0, "Making sure array size is non-negative." );
       static constexpr unsigned int MAX_N_CHILDREN { LAST_INDEX - FIRST_INDEX };

       Node();
       std::unique_ptr<Node> m_children[MAX_N_CHILDREN];
       bool m_representsFullWord;

    private:
       Node(const Node&);
       Node& operator=(const Node&);
    };

    Node m_root;
    std::unordered_set<std::string> m_uniqueWords;
};
