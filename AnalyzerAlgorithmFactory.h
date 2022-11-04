#pragma once

#include <vector>

#include "IAnalyzerAlgorithm.h"
#include "ProgramSettings.h"

struct AnalyzerAlgorithmFactory
{
	static std::vector<IAnalyzerAlgorithmPtr> Create(const ProgramSettingsPtr& programSettings);
};
