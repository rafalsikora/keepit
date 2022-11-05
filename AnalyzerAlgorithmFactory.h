#pragma once

#include <vector>

#include "FileHandler.h"
#include "IAnalyzerAlgorithm.h"
#include "ProgramSettings.h"

struct AnalyzerAlgorithmFactory
{
	static std::vector<IAnalyzerAlgorithmPtr> Create(const ProgramSettingsPtrConst& programSettings, const FileHandlerPtr& fileHandler);
};
