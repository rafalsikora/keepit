#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <unordered_set>

#include "IAnalyzerAlgorithm.h"

class AnalyzerAlgorithmUnorderedSet : public IAnalyzerAlgorithm
{
public:
	AnalyzerAlgorithmUnorderedSet();
    ~AnalyzerAlgorithmUnorderedSet() override;

    bool StartNewThreadAndRun(std::atomic_int& done, std::atomic_bool& abort) override;
    const std::unordered_set<std::string>& GetResult() const override { return m_uniqueWords; }

private:
    void StopAndJoinThread() override;

    std::thread m_thread;
    std::unordered_set<std::string> m_uniqueWords;
};
