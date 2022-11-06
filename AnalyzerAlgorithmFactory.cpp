#include "AnalyzerAlgorithmFactory.h"

#include <memory>

#include "AlgorithmThreadWrapper.h"
#include "AnalyzerAlgorithmUnorderedSet.h"

std::vector<AlgorithmThreadWrapperPtr> AnalyzerAlgorithmFactory::Create(const ProgramSettingsPtrConst& programSettings, const FileHandlerPtr& fileHandler)
{
	std::vector<AlgorithmThreadWrapperPtr> vAlgorithms;

	for (unsigned int i=0; i<programSettings->m_nThreads; ++i)
	{
		IAnalyzerAlgorithmPtr algorithmObj = std::make_shared<AnalyzerAlgorithmUnorderedSet>();
		AlgorithmThreadWrapperPtr algoThreadWrapper = std::make_shared<AlgorithmThreadWrapper>(fileHandler, algorithmObj);
		vAlgorithms.push_back(algoThreadWrapper);
	}

	return vAlgorithms;
}
