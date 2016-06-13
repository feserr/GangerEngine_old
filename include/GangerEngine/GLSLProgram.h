// GLSLProgram.h

#pragma once

#include <string>
#include <GL/glew.h>

namespace GangerEngine
{
    //This class handles the compilation, linking, and usage of a GLSL shader program.
    //Reference: http://www.opengl.org/wiki/Shader_Compilation
    class GLSLProgram
    {
    public:
        GLSLProgram();
        ~GLSLProgram();

        void CompileShaders(const std::string& vertexShaderFile,
            const std::string& fragmentShaderFile);
        void LinkShaders();
        void AddAttribute(const std::string& attributeName);

        GLint GetUniformLocation(const std::string& uniformName);

        void Use();
        void Unuse();

        void Dispose();

    private:
        void CompileShader(const std::string& filePath, GLuint& id);

        GLuint m_programID;
        GLuint m_vertexShaderID;
        GLuint m_fragmentShaderID;

        int m_numAttribute;
    };
}
