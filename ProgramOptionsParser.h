#pragma once

#include <memory>
#include <map>
#include <utility>
#include <vector>

#include "ProgramSettings.h"


enum OPTIONS { OPT_HELP=1, OPT_NTHREADS=2, OPT_FILENAME=3 };

struct ProgramOption
{
	std::string 	m_option;
	std::string 	m_description;
	OPTIONS 		m_id;
};

class ProgramOptionsParser
{

public:
    ProgramOptionsParser();
    ~ProgramOptionsParser() = default;

    bool Run(int argc, const char* argv[]);
    ProgramSettingsPtrConst GetSettings() const { return m_programSettings; }
    bool IsHelpRequested() const { return m_isHelpRequested; }
    void PrintHelp() const;
    
private:
    void ReadArguments(int argc, const char* argv[]);
    bool ParseArguments(ProgramSettings& settings);
    bool ValidateProgramSettings(ProgramSettings& settings);
    void AddOptions();
    
    ProgramSettingsPtrConst  				m_programSettings;
    bool                					m_isHelpRequested;
    std::map<std::string, ProgramOption> 	m_optionsMap;
    std::vector<std::string> 				m_programArguments;
};
