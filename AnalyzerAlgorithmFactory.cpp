#include "AnalyzerAlgorithmFactory.h"

#include <memory>

#include "AnalyzerAlgorithmUnorderedSet.h"

std::vector<IAnalyzerAlgorithmPtr> AnalyzerAlgorithmFactory::Create(const ProgramSettingsPtr& programSettings)
{
	std::vector<IAnalyzerAlgorithmPtr> vAlgorithms;

	for (int i=0; i<programSettings->m_nThreads; ++i)
	{
		IAnalyzerAlgorithmPtr algorithmObj = std::make_shared<AnalyzerAlgorithmUnorderedSet>();
//		switch() //ALERT here different type will be produced depending on th setting
		vAlgorithms.push_back(algorithmObj);
	}

	return vAlgorithms;
}
