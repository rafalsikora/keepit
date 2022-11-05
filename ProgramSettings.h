#pragma once

#include <memory>
#include <string>
#include <thread>

namespace
{
	static const int DEFAUTL_NUM_THREADS = std::thread::hardware_concurrency();
	static const std::string DEFAULT_FILENAME = "defaultFile.txt";
}

struct ProgramSettings
{
    ProgramSettings() : m_nThreads(DEFAUTL_NUM_THREADS),
    					m_fileName(DEFAULT_FILENAME) {}
    
    int m_nThreads;
    std::string m_fileName;
};

typedef std::shared_ptr<ProgramSettings> ProgramSettingsPtr;
