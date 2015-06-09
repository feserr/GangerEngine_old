
#include <GangerEngine/GLSLProgram.h>
#include <GangerEngine/Errors.h>

#include <vector>
#include <fstream>

namespace GangerEngine
{
    GLSLProgram::GLSLProgram() : _programID(0), _vertexShaderID(0), _fragmentShaderID(0),
        _numAttribute(0)
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
        _programID = glCreateProgram();

        // Create the vertex shader object, and store its ID
        _vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
        if(_vertexShaderID == 0) {
            FatalError("Vertex shader failed to be created!");
        }

        // Create the fragment shader object, and store its ID
        _fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
        if(_fragmentShaderID == 0) {
            FatalError("Fragment shader failed to be created!");
        }

        // Compile each shader
        CompileShader(vertexShaderFile, _vertexShaderID);
        CompileShader(fragmentShaderFile, _fragmentShaderID);
    }

    void GLSLProgram::LinkShaders()
    {
        // Attach our shaders to our program
        glAttachShader(_programID, _vertexShaderID);
        glAttachShader(_programID, _fragmentShaderID);

        // Link our program
        glLinkProgram(_programID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
        if(isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<char> errorLog(maxLength);
            glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(_programID);
            //Don't leak shaders either.
            glDeleteShader(_vertexShaderID);
            glDeleteShader(_fragmentShaderID);

            // Use the infoLog as you see fit.

            // In this simple program, we'll just leave
            std::printf("%s\n", &(errorLog[0]));
            FatalError("Shaders: Failed to link it");
        }

        // Always detach shaders after a successful link.
        glDetachShader(_programID, _vertexShaderID);
        glDetachShader(_programID, _fragmentShaderID);

        glDeleteShader(_vertexShaderID);
        glDeleteShader(_fragmentShaderID);
    }

    void GLSLProgram::AddAttribute(const std::string& attributeName)
    {
        glBindAttribLocation(_programID, _numAttribute++, attributeName.c_str());
    }

    GLint GLSLProgram::getUniformLocation(const std::string& uniformName)
    {
        GLuint location = glGetUniformLocation(_programID, uniformName.c_str());

        if(location == GL_INVALID_INDEX)
        {
            FatalError("Uniform " + uniformName + " not found in shader");
        }

        return location;
    }

    void GLSLProgram::use()
    {
        glUseProgram(_programID);
        for(int i = 0; i < _numAttribute; i++)
        {
            glEnableVertexAttribArray(i);
        }
    }

    void GLSLProgram::unuse()
    {
        glUseProgram(0);
        for(int i = 0; i < _numAttribute; i++)
        {
            glDisableVertexAttribArray(i);
        }
    }
}
