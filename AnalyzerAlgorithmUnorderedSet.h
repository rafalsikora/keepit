#pragma once

#include "IAnalyzerAlgorithm.h"

#include <string>
#include <unordered_set>

class AnalyzerAlgorithmUnorderedSet : public IAnalyzerAlgorithm
{
public:
    ~AnalyzerAlgorithmUnorderedSet() override = default;

    void operator+=(const std::string& word) override { m_uniqueWords.insert(word); };
    const std::unordered_set<std::string>& GetResult() const override { return m_uniqueWords; }

private:
    std::unordered_set<std::string> m_uniqueWords;
};
