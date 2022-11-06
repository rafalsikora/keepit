#pragma once

#include <vector>

#include "FileHandler.h"
#include "AlgorithmThreadWrapper.h"
#include "ProgramSettings.h"

struct AnalyzerAlgorithmFactory
{
	static std::vector<AlgorithmThreadWrapperPtr> Create(const ProgramSettingsPtrConst& programSettings, const FileHandlerPtr& fileHandler);
};
