#pragma once

#include <memory>
#include <string>
#include <thread>

enum ALGORITHM { ALGO_UNORDERED_SET = 1, ALGO_TRIE };

namespace
{
	static const unsigned int DEFAUTL_NUM_THREADS = std::thread::hardware_concurrency();
	static const std::string DEFAULT_FILENAME = "defaultFile.txt";
	static const ALGORITHM DEFAULT_ALGORITHM = ALGORITHM::ALGO_UNORDERED_SET;
}

struct ProgramSettings
{
    ProgramSettings();

    void Print() const;

    unsigned int m_nThreads;
    std::string m_fileName;
    ALGORITHM m_algorithmId;
};

typedef std::shared_ptr<const ProgramSettings> ProgramSettingsPtrConst;
