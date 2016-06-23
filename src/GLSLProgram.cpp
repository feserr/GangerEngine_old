/*
    Copyright [2016] [Ganger Games]

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

#include <GangerEngine/GLSLProgram.h>
#include <GangerEngine/GangerErrors.h>
#include <GangerEngine/IOManager.h>

#include <string>
#include <vector>
#include <fstream>

namespace GangerEngine {
    // The : m_numAttributes(0) ect. is an initialization list. It is a better
    // way to initialize variables, since it avoids an extra copy.
    GLSLProgram::GLSLProgram() : m_numAttributes(0), m_programID(0),
        m_vertexShaderID(0), m_fragmentShaderID(0) {
    }


    GLSLProgram::~GLSLProgram() {
    }

    // Compiles the shaders into a form that your GPU can understand
    void GLSLProgram::CompileShaders(const std::string& vertexShaderFilePath,
        const std::string& fragmentShaderFilePath) {
        std::string vertSource;
        std::string fragSource;

        IOManager::ReadFileToBuffer(vertexShaderFilePath, vertSource);
        IOManager::ReadFileToBuffer(fragmentShaderFilePath, fragSource);

        CompileShadersFromSource(vertSource.c_str(), fragSource.c_str());
    }

    void GLSLProgram::CompileShadersFromSource(const char* vertexSource,
        const char* fragmentSource) {
        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        m_programID = glCreateProgram();

        // Create the vertex shader object, and store its ID
        m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        if (m_vertexShaderID == 0) {
            FatalError("Vertex shader failed to be created!");
        }

        // Create the fragment shader object, and store its ID
        m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if (m_fragmentShaderID == 0) {
            FatalError("Fragment shader failed to be created!");
        }

        // Compile each shader
        CompileShader(vertexSource, "Vertex Shader", m_vertexShaderID);
        CompileShader(fragmentSource, "Fragment Shader", m_fragmentShaderID);
    }

    void GLSLProgram::LinkShaders() {
        // Attach our shaders to our program
        glAttachShader(m_programID, m_vertexShaderID);
        glAttachShader(m_programID, m_fragmentShaderID);

        // Link our program
        glLinkProgram(m_programID);

        // Note the different functions here: glGetProgram* instead of
        // glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_programID, GL_LINK_STATUS, reinterpret_cast<int*>(
            &isLinked));

        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetProgramInfoLog(m_programID, maxLength, &maxLength,
                &errorLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_programID);
            // Don't leak shaders either.
            glDeleteShader(m_vertexShaderID);
            glDeleteShader(m_fragmentShaderID);

            // print the error log and quit
            std::printf("%s\n", &(errorLog[0]));
            FatalError("Shaders failed to link!");
        }

        // Always detach shaders after a successful link.
        glDetachShader(m_programID, m_vertexShaderID);
        glDetachShader(m_programID, m_fragmentShaderID);
        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);
    }

    // Adds an attribute to our shader. SHould be called between compiling and
    // linking.
    void GLSLProgram::AddAttribute(const std::string& attributeName) {
        glBindAttribLocation(m_programID, m_numAttributes++,
            attributeName.c_str());
    }

    GLint GLSLProgram::GetUniformLocation(const std::string& uniformName) {
        GLint location = glGetUniformLocation(m_programID, uniformName.c_str());
        if (location == GL_INVALID_INDEX) {
            FatalError("Uniform " + uniformName + " not found in shader!");
        }
        return location;
    }

    // Enable the shader, and all its attributes
    void GLSLProgram::Use() {
        glUseProgram(m_programID);
        // Enable all the attributes we added with addAttribute
        for (int i = 0; i < m_numAttributes; i++) {
            glEnableVertexAttribArray(i);
        }
    }

    // Disable the shader
    void GLSLProgram::Unuse() {
        glUseProgram(0);
        for (int i = 0; i < m_numAttributes; i++) {
            glDisableVertexAttribArray(i);
        }
    }

    void GLSLProgram::Dispose() {
        if (m_programID)
            glDeleteProgram(m_programID);
    }

    // Compiles a single shader file
    void GLSLProgram::CompileShader(const char* source, const std::string& name,
        GLuint id) {
        // Tell opengl that we want to use fileContents as the contents of the
        // shader file
        glShaderSource(id, 1, &source, nullptr);

        // Compile the shader
        glCompileShader(id);

        // Check for errors
        GLint success = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

            // Provide the infolog in whatever manor you deem best.
            // Exit with failure.
            glDeleteShader(id);  // Don't leak the shader.

            // Print error log and quit
            std::printf("%s\n", &(errorLog[0]));
            FatalError("Shader " + name + " failed to compile");
        }
    }
}  // namespace GangerEngine
