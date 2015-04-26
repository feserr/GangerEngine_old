#include "IOFiles.h"
#include "Errors.h"

#include <fstream>
#include <string>

std::string GetFileContents(const char *filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        return(std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()));
    }
    else
    {
        FatalError("Failed to read " + *filename);
    }
}
