#include "FileHandler.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

const std::tuple<const char*, size_t> 	FileHandler::END_OF_FILE {nullptr, 0};

FileHandler::FileHandler(const ProgramSettingsPtrConst& programSettingsPtr)
	:	m_programSettings{programSettingsPtr},
		m_isInitialized{},
		m_fileDescriptor{-1},
		m_fileData{},
		m_fileLengthTotal{},
		m_fileLengthProcessed{},
		m_singleChunkDefaultLength{}
{
}

FileHandler::~FileHandler()
{
	Finalize();
	Close();
}

bool FileHandler::Initialize()
{
	bool status = Open();
	if (status)
	{
		status = DetermineResourcesManagementPolicy();
		if (status)
		{
			const char* addr = MapFile();
			if (addr == MAP_FAILED)
			{
				std::cerr << "Problem occurred when mapping the file into memory." << std::endl;
				status = false;
			}
			else
			{
				m_fileData = addr;
				m_isInitialized = true;
			}
		}
		else
		{
			std::cerr << "Problem occurred when determining the resource management policy." << std::endl;
		}
	}
	else{
		std::cerr << "Problem occurred when opening the file " << m_programSettings->m_fileName << " during initialization." << std::endl;
	}
	return status;
}

bool FileHandler::Open()
{
	m_fileDescriptor = open(m_programSettings->m_fileName.c_str(), O_RDONLY);
	return m_fileDescriptor != -1;
}

bool FileHandler::Close()
{
	return close(m_fileDescriptor) != -1;
}

bool FileHandler::DetermineResourcesManagementPolicy()
{
	struct stat sb;
	if (fstat(m_fileDescriptor, &sb) == -1)
	{
		return false;
	}
	else
	{
		m_fileLengthTotal = static_cast<size_t>(sb.st_size);
		m_singleChunkDefaultLength = m_fileLengthTotal / m_programSettings->m_nThreads + 1;
		return true;
	}
}

std::tuple<const char*, size_t> FileHandler::GetPartOfText()
{
	std::lock_guard<std::mutex> guard(m_mutex);

	if (ReachedEOF())
	{
		return END_OF_FILE;
	}

	const auto currentTextChunkHead = GetChunkHead();
	const auto currentTextChunkLength = GetChunkLengthUntilNextWhitespaceOrEOF();
	UpdateText(currentTextChunkLength);

	return {currentTextChunkHead, currentTextChunkLength};
}

size_t FileHandler::GetChunkLengthUntilNextWhitespaceOrEOF() const
{
	size_t currentChunkLength{m_singleChunkDefaultLength};
	if (m_fileLengthProcessed + currentChunkLength > m_fileLengthTotal)
	{
		currentChunkLength = m_fileLengthTotal - m_fileLengthProcessed;
	}
	else
	{
		while (m_fileLengthProcessed + currentChunkLength < m_fileLengthTotal)
		{
			bool isSpace = isspace(GetChunkHead()[currentChunkLength]);
			if (!isSpace)
			{
				++currentChunkLength;
			}
			else
			{
				break;
			}
		}
	}
	return currentChunkLength;
}

void FileHandler::UpdateText(const size_t& currentTextChunkLength)
{
	m_fileLengthProcessed += currentTextChunkLength;
}

const char* FileHandler::GetChunkHead() const
{
	return &m_fileData[m_fileLengthProcessed];
}

bool FileHandler::ReachedEOF() const
{
	return m_fileLengthProcessed == m_fileLengthTotal;
}

void FileHandler::Finalize()
{
	if (UnmapFile() == -1)
	{
		std::cerr << "Unexpected error during mapped file memory release. Address = " << m_fileData << std::endl;
	}
	m_isInitialized = false;
}

const char* FileHandler::MapFile()
{
	return static_cast<const char*>(mmap(NULL, m_fileLengthTotal, PROT_READ, MAP_PRIVATE, m_fileDescriptor, 0));
}

int FileHandler::UnmapFile()
{
	return munmap(const_cast<char*>(m_fileData), m_fileLengthTotal);
}
