#include "ProgramOptionsParser.h"

#include <iostream>
#include <unistd.h>
#include <experimental/filesystem>

ProgramOptionsParser::ProgramOptionsParser()
    : m_programSettings{}, m_isHelpRequested{false}
{
    AddOptions();
}

bool ProgramOptionsParser::Run(int argc, const char* argv[])
{
    ProgramSettings localSettings;
    ReadArguments(argc, argv);
    bool status = ParseArguments(localSettings);
    if (status)
    {
    	status = ValidateProgramSettings(localSettings);
    	if (status)
    	{
    		m_programSettings.reset(new const ProgramSettings(localSettings));
    	}
    }
    return status;
}

bool ProgramOptionsParser::ParseArguments(ProgramSettings& settings)
{
    bool status = true;
    for (unsigned int i=0; i<m_programArguments.size(); ++i)
    {
        const auto arg = m_programArguments[i];
        const auto option = m_optionsMap[arg];
        switch(option.m_id)
        {
            case OPT_HELP:
                m_isHelpRequested = true;
                break;
            case OPT_NTHREADS:
            	settings.m_nThreads = static_cast<unsigned int>(std::stoi(m_programArguments[++i]));
                break;
            case OPT_FILENAME:
            	settings.m_fileName = m_programArguments[++i];
                break;
            case OPT_ALGORITHM:
            	settings.m_algorithmId = static_cast<ALGORITHM>(std::stoi(m_programArguments[++i]));
            	break;
            default:
                std::cerr << "Unrecognized option: \"" << arg << "\"" << std::endl;
                status = false;
        }
    }
    return status;
}

void ProgramOptionsParser::ReadArguments(int argc, const char* argv[])
{
    for (int i=1; i<argc; ++i)
    {
        m_programArguments.push_back(std::string(argv[i]));
    }
}

void ProgramOptionsParser::AddOptions()
{
    m_optionsMap["-h"] = {"-h", "Display help message.", OPT_HELP};
    m_optionsMap["-j"] = {"-j", "The number of threads to be used (defined by the number which follows e.g. -j 5)", OPT_NTHREADS};
    m_optionsMap["-f"] = {"-f", "The path to the file (the string which follows) e.g. -f /home/user/file.txt", OPT_FILENAME};
    m_optionsMap["-a"] = {"-a", "The id of the words duplicate search algorithm (1 = hashing in std::unordered_set, 2 = trie algorithm) e.g. -a 2", OPT_ALGORITHM};
}

void ProgramOptionsParser::PrintHelp() const
{
    std::cout << "Options available:" << std::endl;
    for (const auto& [optString, optObject] : m_optionsMap)
    {
        std::cout << optString << '\t' << optObject.m_description << std::endl;
    }
}

bool ProgramOptionsParser::ValidateProgramSettings(ProgramSettings& settings)
{
	if (!std::experimental::filesystem::exists(settings.m_fileName))
	{
		std::cerr << "FATAL ERROR: File \"" << settings.m_fileName << "\" does not exists." << std::endl;
		return false;
	}

	if (settings.m_algorithmId != ALGORITHM::ALGO_UNORDERED_SET && settings.m_algorithmId != ALGORITHM::ALGO_TRIE)
	{
	   std::cerr << "FATAL ERROR: Unrecognized algorithm option." << std::endl;
	   return false;
	}

	if (settings.m_nThreads > DEFAUTL_NUM_THREADS)
	{
		std::cout << "Warning: asked to use n=" << settings.m_nThreads << \
				" threads while hardware supports n_max=" << DEFAUTL_NUM_THREADS << \
				" concurrent threads. Setting n to n_max." << std::endl;
		settings.m_nThreads = DEFAUTL_NUM_THREADS;
	}

	return true;
}
