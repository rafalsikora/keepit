#pragma once

#include <string>
#include <unordered_set>

#include "IAnalyzerAlgorithm.h"

class AnalyzerAlgorithmUnorderedSet : public IAnalyzerAlgorithm
{
public:
	AnalyzerAlgorithmUnorderedSet();
    ~AnalyzerAlgorithmUnorderedSet() override {}

    bool Run() override;
    const std::unordered_set<std::string>& GetResult() const override { return m_uniqueWords; }

private:
    std::unordered_set<std::string> m_uniqueWords;
};
