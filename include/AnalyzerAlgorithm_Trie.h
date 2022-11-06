#pragma once

#include "IAnalyzerAlgorithm.h"

#include <string>
#include <unordered_set>

class AnalyzerAlgorithm_Trie : public IAnalyzerAlgorithm
{
public:
    ~AnalyzerAlgorithm_Trie() override = default;

    void operator+=(const std::string& word) override;
    const std::unordered_set<std::string>& GetResult() const override;

private:
    std::unordered_set<std::string> m_uniqueWords;
};
