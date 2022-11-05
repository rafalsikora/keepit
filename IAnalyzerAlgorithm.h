#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <unordered_set>

class IAnalyzerAlgorithm
{
public:
    virtual ~IAnalyzerAlgorithm() = default;

    [[nodiscard]] virtual bool StartNewThreadAndRun(std::atomic_uint& done, std::atomic_bool& abort) = 0;
    virtual void StopAndJoinThread() = 0;
    virtual const std::unordered_set<std::string>& GetResult() const = 0;
};

typedef std::shared_ptr<IAnalyzerAlgorithm> IAnalyzerAlgorithmPtr;
