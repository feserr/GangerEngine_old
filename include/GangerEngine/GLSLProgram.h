
#pragma once

#include <string>
#include <GL/glew.h>

namespace GangerEngine
{
    class GLSLProgram
    {
    public:
        GLSLProgram ();
        ~GLSLProgram ();

        void CompileShaders (const std::string& vertexShaderFile,
            const std::string& fragmentShaderFile);
        void LinkShaders ();
        void AddAttribute (const std::string& attributeName);

        GLint getUniformLocation (const std::string& uniformName);

        void use ();
        void unuse ();

    private:
        void CompileShader (const std::string& filePath, GLuint& id);

        GLuint _programID;
        GLuint _vertexShaderID;
        GLuint _fragmentShaderID;

        int _numAttribute;
    };
}
