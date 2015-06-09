
#include <GangerEngine/IOManager.h>

#include <fstream>

namespace GangerEngine
{
    bool IOManager::ReadFileToBuffer (std::string filePath, std::vector<unsigned char>& buffer)
    {
        std::ifstream file (filePath, std::ios::binary);
        if (file.fail ())
        {
            perror (filePath.c_str ());
            return false;
        }

        // Seek to the end
        file.seekg (0, std::ios::end);

        // Get the size of the file
        int fileSize = (int)file.tellg ();

        // Return to the beginning
        file.seekg (0, std::ios::beg);

        // Reduce the file size by any header bytes if present
        fileSize -= (int)file.tellg ();

        buffer.resize (fileSize);
        file.read ((char *)&buffer[0], fileSize);
        file.close ();

        return true;
    }
}
