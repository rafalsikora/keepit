#include "AlgorithmThreadWrapper.h"

#include <cctype>
#include <exception>
#include <iostream>

AlgorithmThreadWrapper::AlgorithmThreadWrapper(const FileHandlerPtr& fileHandler, const IAnalyzerAlgorithmPtr& algorithm)
	: m_fileHandler{fileHandler},
	  m_algorithm{algorithm},
	  m_text{},
	  m_textTotalSize{},
	  m_lastNonWhiteChar{},
	  m_isNewText{},
	  m_textIterator{}
{
}

std::future<bool> AlgorithmThreadWrapper::StartNewThreadAndRun(std::atomic_bool& abort)
{
	try
	{
	   return std::async(std::launch::async, [&]()
		{
			while (!abort)
			{
				const auto data = m_fileHandler->GetPartOfText();
				if (data == FileHandler::END_OF_FILE)
				{
					break;
				}

				m_isNewText = true;
				m_text = std::get<const char*>(data);
				m_textTotalSize = std::get<size_t>(data);

				std::string word{};
				while (GetNextWord(word) && !abort)
				{
					*m_algorithm += word;
				}
			}
			return true;
		});
	}
	catch (const std::exception& e)
	{
		std::cerr << "New thread in " << typeid(this).name() << " could not be started. Exception thrown: " << e.what() << std::endl;
		abort = true;
		return std::future<bool>();
	}
}

bool AlgorithmThreadWrapper::GetNextWord(std::string& word)
{
	if (m_isNewText)
	{
		m_lastNonWhiteChar = m_text;
		m_textIterator = 0;
		m_isNewText = false;
	}

	for(size_t wordSize{}; m_textIterator<=m_textTotalSize; ++m_textIterator)
	{
	   const auto isCharSpace { isspace(m_text[m_textIterator]) };
	   const auto isCharLastInText { m_textIterator==m_textTotalSize };
		if (isCharSpace || isCharLastInText)
		{
			if (wordSize)
			{
				word = std::string(m_lastNonWhiteChar, wordSize);
				return true;
			}
			m_lastNonWhiteChar = &m_text[m_textIterator+1];
			wordSize = 0;
		}
		else
		{
			++wordSize;
		}
	}
	return false;
}
