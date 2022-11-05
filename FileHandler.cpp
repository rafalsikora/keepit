#include "FileHandler.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

const std::tuple<const char*, size_t> 	FileHandler::FATAL_ERROR {nullptr, std::numeric_limits<size_t>::max()};
const std::tuple<const char*, size_t> 	FileHandler::END_OF_FILE {nullptr, 0};

FileHandler::FileHandler(const ProgramSettingsPtrConst& programSettingsPtr)
	:	m_programSettings{programSettingsPtr},
		m_memoryPageSize{static_cast<size_t>(sysconf(_SC_PAGESIZE))},
		m_isInitialized{},
		m_fileDescriptor{-1},
		m_fileLengthTotal{},
		m_fileLengthProcessed{},
		m_singleChunkDefaultLength{}
{
}

FileHandler::~FileHandler()
{
	ReleaseMemoryAll();
	Close();
}

bool FileHandler::Initialize()
{
	bool status = Open();
	if (status)
	{
		status = DetermineResourcesManagementPolicy();
		if (!status)
		{
			std::cerr << "Problem occurred when determining the resource management policy." << std::endl;
		}
	}
	else{
		std::cerr << "Problem occurred when opening the file " << m_programSettings->m_fileName << " during initialization." << std::endl;
	}
	m_isInitialized = status;
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
		m_singleChunkDefaultLength = m_fileLengthTotal / m_programSettings->m_nThreads;
		m_singleChunkDefaultLength -= m_singleChunkDefaultLength % m_memoryPageSize;
		if (m_singleChunkDefaultLength < m_memoryPageSize)
		{
			m_singleChunkDefaultLength = m_memoryPageSize;
		}
		return true;
	}
}

std::tuple<const char*, size_t> FileHandler::GetPartOfText(const char* memoryToUnmap)
{
	std::lock_guard<std::mutex> guard(m_mutex);
	std::cout << "Getting part of text" << std::endl;
	ReleaseMemory(memoryToUnmap);

	if (m_mappedMemory.size() >= m_programSettings->m_nThreads)
	{
		std::cerr << "FATAL ERROR: abnormal file read (max count exceeded)!" << std::endl;
		return FATAL_ERROR;
	}

	if (m_fileLengthProcessed == m_fileLengthTotal)
	{
		return END_OF_FILE;
	}

	size_t currentChunkLength = GetCurrentChunkLength();
	const char* addr = static_cast<const char*>(mmap(NULL,
													 currentChunkLength,
													 PROT_READ,
													 MAP_PRIVATE,
													 m_fileDescriptor,
													 static_cast<off_t>(m_fileLengthProcessed)));
	if (addr == MAP_FAILED)
	{
		std::cerr << "DEBUG: Map failed" << std::endl;
		return FATAL_ERROR;
	}
	else
	{
		std::cout << "DEBUG: Map succeeded" << std::endl;
		size_t textSizeUntilTheLastWhitespace{currentChunkLength};
		RemoveTrailingTruncatedWords(textSizeUntilTheLastWhitespace);
		UpdateFileRead(addr, currentChunkLength, textSizeUntilTheLastWhitespace);
		return {addr, textSizeUntilTheLastWhitespace};
	}
}

size_t FileHandler::GetCurrentChunkLength() const
{
	size_t currentChunkLength{m_singleChunkDefaultLength};
	if (m_fileLengthProcessed + currentChunkLength > m_fileLengthTotal)
	{
		currentChunkLength = m_fileLengthTotal - m_fileLengthProcessed;
	}
	return currentChunkLength;
}

void FileHandler::RemoveTrailingTruncatedWords(size_t& textSizeUntilTheLastWhitespace) const
{

}

void FileHandler::UpdateFileRead(const char* addr, const size_t& currentChunkLength, const size_t& textLengthInCurrentChunk)
{
	m_fileLengthProcessed += textLengthInCurrentChunk;
	m_mappedMemory[addr] = currentChunkLength;
}

void FileHandler::ReleaseMemoryAll()
{
	for(const auto& [ptr, size] : m_mappedMemory)
	{
		ReleaseMemory(ptr, size);
	}
}

void FileHandler::ReleaseMemory(const char* memoryToUnmap)
{
	if(!memoryToUnmap)
	{
		return;
	}
	if (auto it = m_mappedMemory.find(memoryToUnmap); it == m_mappedMemory.end()) {
		std::cerr << "Error - trying to release memory not reserved by the file handler." << std::endl;
	} else {
		const size_t size = m_mappedMemory[memoryToUnmap];
		ReleaseMemory(memoryToUnmap, size);
		m_mappedMemory.erase(it);
	}
}

void FileHandler::ReleaseMemory(const char* memoryToUnmap, const size_t size) const
{
	if (munmap(const_cast<char*>(memoryToUnmap), size) == -1)
	{
		std::cerr << "Unexpected error during mapped file memory release. Address = " << memoryToUnmap << std::endl;
	}
}
