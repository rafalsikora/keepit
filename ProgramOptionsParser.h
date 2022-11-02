#pragma once

#include <memory>
#include <map>
#include <utility>
#include <vector>

#include "ProgramSettings.h"

class ProgramOptionsParser
{

public:
    ProgramOptionsParser();
    ~ProgramOptionsParser() = default;

    bool Run(int argc, const char* argv[]);
    ProgramSettingsPtr GetSettings() const { return m_programSettings; }
    bool IsHelpRequested() const { return m_isHelpRequested; }
    void PrintHelp() const;
    
private:
    enum OPTIONS { OPT_HELP=1, OPT_NTHREADS=2, OPT_FILENAME=3 };
    
    void ReadArguments(int argc, const char* argv[]);
    bool ParseArguments();
    void AddOptions();
    
    ProgramSettingsPtr  m_programSettings;
    bool                m_isHelpRequested;
    std::map<std::string, std::pair<std::string, int>> m_optionsDescription;
    std::vector<std::string> m_programArguments;
};
