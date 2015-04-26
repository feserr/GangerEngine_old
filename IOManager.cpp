
#include "IOManager.h"

#include <fstream>

bool IOManager::readFileToBuffer (std::string filePath, std::vector<unsigned>& buffer)
{
    std::ifstream file(filePath, std::ios::binary);
    if (file.fail())
    {
        perror(filePath.c_str());
        return false;
    }

    // Seek to the end
    file.seekg(0, std::ios::end);

    // Get the size of the file
    int fileSize = file.tellg();

    // Return to the begining
    file.seekg(0, std::ios::beg);

    // Reduce the file size by any header bytes if present
    fileSize -= file.tellg();

    buffer.resize(fileSize);
    file.read((char *)&buffer[0], fileSize);
    file.close;

    return true;
}