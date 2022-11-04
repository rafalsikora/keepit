#include <iostream>

#include "ProgramOptionsParser.h"
#include "TextAnalyzer.h"

int RunTextAnalysis(const ProgramOptionsParser& parser);

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
        	return RunTextAnalysis(parser);
        }
    }
    else
    {
        std::cerr << "Program options parser error. Exiting." << std::endl;
        return 1;
    }
}

int RunTextAnalysis(const ProgramOptionsParser& parser)
{
	ProgramSettingsPtr settings {parser.GetSettings()};
	TextAnalyzer analyzer(settings);
	bool status = analyzer.Run();
	if (status)
	{
		analyzer.PrintResults();
		return 0;
	}
	else
	{
		std::cerr << "Text analyzer error. Exiting." << std::endl;
		return 2;
	}
}
