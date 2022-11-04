#pragma once

#include <memory>
#include <string>
#include <unordered_set>

class IAnalyzerAlgorithm
{
public:
    virtual ~IAnalyzerAlgorithm() = default;

    [[nodiscard]] virtual bool Run() = 0;
    virtual const std::unordered_set<std::string>& GetResult() const = 0;
};

typedef std::shared_ptr<IAnalyzerAlgorithm> IAnalyzerAlgorithmPtr;
