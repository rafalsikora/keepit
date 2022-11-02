#pragma once

#include <string>
#include <memory>

struct ProgramSettings
{
    ProgramSettings() : m_nThreads(1), m_fileName("") {}
    
    int m_nThreads;
    std::string m_fileName;
};

typedef std::shared_ptr<ProgramSettings> ProgramSettingsPtr;
