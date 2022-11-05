#pragma once

#include <memory>
#include <string>
#include <thread>

namespace
{
	static const unsigned int DEFAUTL_NUM_THREADS = std::thread::hardware_concurrency();
	static const std::string DEFAULT_FILENAME = "defaultFile.txt";
}

struct ProgramSettings
{
    ProgramSettings();
    
    void Print() const;

    unsigned int m_nThreads;
    std::string m_fileName;
};

typedef std::shared_ptr<const ProgramSettings> ProgramSettingsPtrConst;
