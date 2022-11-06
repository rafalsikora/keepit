#include "AnalyzerAlgorithmFactory.h"

#include <iostream>
#include <memory>

#include "AlgorithmThreadWrapper.h"
#include "AnalyzerAlgorithm_UnorderedSet.h"
#include "AnalyzerAlgorithm_Trie.h"

std::vector<AlgorithmThreadWrapperPtr> AnalyzerAlgorithmFactory::Create(const ProgramSettingsPtrConst& programSettings, const FileHandlerPtr& fileHandler)
{
	std::vector<AlgorithmThreadWrapperPtr> vAlgorithms;
	for (unsigned int i=0; i<programSettings->m_nThreads; ++i)
	{
		IAnalyzerAlgorithmPtr algorithmObj{};
		switch (programSettings->m_algorithmId)
		{
			case ALGORITHM::ALGO_UNORDERED_SET:
			{
				algorithmObj = std::make_shared<AnalyzerAlgorithm_UnorderedSet>();
				break;
			}
			case ALGORITHM::ALGO_TRIE:
			{
				algorithmObj = std::make_shared<AnalyzerAlgorithm_Trie>();
				break;
			}
			default: std::cerr << "FATAL ERROR: Uncovered algorithm option." << std::endl;
		}
		AlgorithmThreadWrapperPtr algoThreadWrapper = std::make_shared<AlgorithmThreadWrapper>(fileHandler, algorithmObj);
		vAlgorithms.push_back(algoThreadWrapper);
	}

	return vAlgorithms;
}
