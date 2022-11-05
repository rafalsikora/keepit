#include "AnalyzerAlgorithmFactory.h"

#include <memory>

#include "AlgorithmThreadWrapper.h"

std::vector<IAnalyzerAlgorithmPtr> AnalyzerAlgorithmFactory::Create(const ProgramSettingsPtrConst& programSettings, const FileHandlerPtr& fileHandler)
{
	std::vector<IAnalyzerAlgorithmPtr> vAlgorithms;

	for (unsigned int i=0; i<programSettings->m_nThreads; ++i)
	{
		IAnalyzerAlgorithmPtr algorithmObj = std::make_shared<AlgorithmThreadWrapper>(fileHandler);
//		switch() //ALERT here different type will be produced depending on th setting
		vAlgorithms.push_back(algorithmObj);
	}

	return vAlgorithms;
}
