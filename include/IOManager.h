
#pragma once

#include <vector>

class IOManager
{
public:
    static bool ReadFileToBuffer (std::string filePath, std::vector<unsigned char>& buffer);
};
