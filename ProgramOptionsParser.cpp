#include "ProgramOptionsParser.h"

#include <iostream>

ProgramOptionsParser::ProgramOptionsParser()
    : m_programSettings(), m_isHelpRequested(false)
{
    AddOptions();
}

bool ProgramOptionsParser::Run(int argc, const char* argv[])
{
    m_programSettings = std::make_shared<ProgramSettings>();
    ReadArguments(argc, argv);
    return ParseArguments();
}

bool ProgramOptionsParser::ParseArguments()
{
    bool status = true;
    for (unsigned int i=0; i<m_programArguments.size(); ++i)
    {
        const auto arg = m_programArguments[i];
        const auto option = m_optionsDescription[arg];
        switch(option.second)
        {
            case OPT_HELP:
                m_isHelpRequested = true;
                break;
            case OPT_NTHREADS:
                m_programSettings->m_nThreads = std::stoi(m_programArguments[++i]);
                break;
            case OPT_FILENAME:
                m_programSettings->m_fileName = m_programArguments[++i];
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
    m_optionsDescription["-h"] = {"Display help message.", OPT_HELP};
    m_optionsDescription["-j"] = {"The number of threads to be used (defined by the number which follows e.g. -j 5)", OPT_NTHREADS};
    m_optionsDescription["-f"] = {"The path to the file (the string which follows) e.g. -f /home/user/file.txt", OPT_FILENAME};
}

void ProgramOptionsParser::PrintHelp() const
{
    std::cout << "Options available:" << std::endl;
    for (const auto& [opt, val] : m_optionsDescription)
    {
        std::cout << opt << '\t' << val.first << std::endl;
    }
}
