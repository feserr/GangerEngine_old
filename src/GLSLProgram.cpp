// GLSLProgram.cpp

#include <GangerEngine/GLSLProgram.h>
#include <GangerEngine/GangerErrors.h>

#include <vector>
#include <fstream>

namespace GangerEngine
{
    GLSLProgram::GLSLProgram() : m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0),
        m_numAttribute(0)
    {
    }

    GLSLProgram::~GLSLProgram()
    {
    }

    void GLSLProgram::CompileShader(const std::string& filePath, GLuint& id)
    {
        std::ifstream shaderFile(filePath);
        if(shaderFile.fail()) {
            perror(filePath.c_str());
            FatalError("Failed to open " + filePath);
        }

        // File contents stores all the text in the file
        std::string fileContents = "";
        // Line is used to grab each line of the file
        std::string line;

        //Get all the lines in the file and add it to the contents
        while(std::getline(shaderFile, line))
        {
            fileContents += line + "\n";
        }

        shaderFile.close();

        // Get a pointer to our file contents c string;
        const char* contentsPtr = fileContents.c_str();
        // Tell OpenGL that we want to use fileContents as the contents of the shader file
        glShaderSource(id, 1, &contentsPtr, nullptr);

        // Compile the shader
        glCompileShader(id);

        GLint isCompiled = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

            // Provide the info log in whatever manor you deem best.
            // Exit with failure.
            glDeleteShader(id); // Don't leak the shader.

            std::printf("%s\n", &(errorLog[0]));
            FatalError("Shader " + filePath + ": Failed to compile it");
        }
    }

    void GLSLProgram::CompileShaders(const std::string& vertexShaderFile,
        const std::string& fragmentShaderFile)
    {
        /* Vertex and fragment shaders are successfully compiled.
         * Now is time to link them together into a program.
         * Get a program object.*/
        m_programID = glCreateProgram();

        // Create the vertex shader object, and store its ID
        m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        if(m_vertexShaderID == 0) {
            FatalError("Vertex shader failed to be created!");
        }

        // Create the fragment shader object, and store its ID
        m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if(m_fragmentShaderID == 0) {
            FatalError("Fragment shader failed to be created!");
        }

        // Compile each shader
        CompileShader(vertexShaderFile, m_vertexShaderID);
        CompileShader(fragmentShaderFile, m_fragmentShaderID);
    }

    void GLSLProgram::LinkShaders()
    {
        // Attach our shaders to our program
        glAttachShader(m_programID, m_vertexShaderID);
        glAttachShader(m_programID, m_fragmentShaderID);

        // Link our program
        glLinkProgram(m_programID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
        if(isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_programID);
            //Don't leak shaders either.
            glDeleteShader(m_vertexShaderID);
            glDeleteShader(m_fragmentShaderID);

            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
            std::printf("%s\n", &(errorLog[0]));
            FatalError("Shaders: Failed to link it");
        }

        // Always detach shaders after a successful link.
        glDetachShader(m_programID, m_vertexShaderID);
        glDetachShader(m_programID, m_fragmentShaderID);

        glDeleteShader(m_vertexShaderID);
        glDeleteShader(m_fragmentShaderID);
    }

    void GLSLProgram::AddAttribute(const std::string& attributeName)
    {
        glBindAttribLocation(m_programID, m_numAttribute++, attributeName.c_str());
    }

    GLint GLSLProgram::GetUniformLocation(const std::string& uniformName)
    {
        GLuint location = glGetUniformLocation(m_programID, uniformName.c_str());

        if(location == GL_INVALID_INDEX)
        {
            FatalError("Uniform " + uniformName + " not found in shader");
        }

        return location;
    }

    void GLSLProgram::Use()
    {
        glUseProgram(m_programID);
        for(int i = 0; i < m_numAttribute; i++)
        {
            glEnableVertexAttribArray(i);
        }
    }

    void GLSLProgram::Unuse()
    {
        glUseProgram(0);
        for(int i = 0; i < m_numAttribute; i++)
        {
            glDisableVertexAttribArray(i);
        }
    }

    void GLSLProgram::Dispose()
    {
        if(m_programID)
            glDeleteProgram(m_programID);
    }
}
