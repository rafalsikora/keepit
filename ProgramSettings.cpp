#include "ProgramSettings.h"

#include <iostream>

ProgramSettings::ProgramSettings()
	:	m_nThreads(DEFAUTL_NUM_THREADS),
    	m_fileName(DEFAULT_FILENAME)
{
}

void ProgramSettings::Print() const
{
	std::cout << "Program settings:";
	std::cout << " nThreads=" << m_nThreads;
	std::cout << " fileName=" << m_fileName;
	std::cout << "\n------------------" << std::endl;
}
