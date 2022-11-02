#include <iostream>

#include "ProgramOptionsParser.h"

int main(int argc, const char* argv[])
{
    ProgramOptionsParser parser;
    
    if (parser.Run(argc, argv))
    {
        if (parser.IsHelpRequested())
        {
            parser.PrintHelp();
            return 0;
        }
        else
        {
            ProgramSettingsPtr settings {parser.GetSettings()};
        }
    }
    else
    {
        std::cerr << "Program options parser error." << std::endl;
        return 1;
    }
	return 0;
}

