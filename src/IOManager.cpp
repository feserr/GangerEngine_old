/*
    Copyright [2016] [Elías Serrano]

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <GangerEngine/IOManager.h>

#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <fstream>

// Namespace alias
namespace fs = boost::filesystem;

namespace GangerEngine {
    bool IOManager::ReadFileToBuffer(std::string filePath,
        std::vector<unsigned char>& buffer) {
        std::ifstream file(filePath, std::ios::binary);
        if (file.fail()) {
            perror(filePath.c_str());
            return false;
        }

        // Seek to the end
        file.seekg(0, std::ios::end);

        // Get the file size
        unsigned int fileSize = (unsigned int)file.tellg();
        file.seekg(0, std::ios::beg);

        // Reduce the file size by any header bytes that might be present
        fileSize -= (unsigned int)file.tellg();

        buffer.resize(fileSize);
        file.read(reinterpret_cast<char*>(&buffer[0]), fileSize);
        file.close();

        return true;
    }

    bool IOManager::ReadFileToBuffer(std::string filePath,
        std::string& buffer) {
        std::ifstream file(filePath, std::ios::binary);
        if (file.fail()) {
            perror(filePath.c_str());
            return false;
        }

        // Seek to the end
        file.seekg(0, std::ios::end);

        // Get the file size
        unsigned int fileSize = (unsigned int)file.tellg();
        file.seekg(0, std::ios::beg);

        // Reduce the file size by any header bytes that might be present
        fileSize -= (unsigned int)file.tellg();

        buffer.resize(fileSize);
        file.read(reinterpret_cast<char *>(&(buffer[0])), fileSize);
        file.close();

        return true;
    }

    bool IOManager::GetDirectoryEntries(const char* path,
        std::vector<DirEntry>* rvEntries) {
        auto dpath = fs::path(path);
        // Must be directory
        if (!fs::is_directory(dpath)) return false;

        for (auto it = fs::directory_iterator(dpath); it !=
            fs::directory_iterator(); ++it) {
            rvEntries->emplace_back();
            rvEntries->back().path = it->path().string();
            if (is_directory(it->path())) {
                rvEntries->back().isDirectory = true;
            } else {
                rvEntries->back().isDirectory = false;
            }
        }
        return true;
    }

    bool IOManager::MakeDirectory(const char* path) {
        return fs::create_directory(fs::path(path));
    }
}  // namespace GangerEngine
