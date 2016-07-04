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

#ifndef _GLSLPROGRAM_H_
#define _GLSLPROGRAM_H_

#include <GL/glew.h>
#include <string>

namespace GangerEngine {
/**
 * \brief      This class handles the compilation, linking, and usage of a GLSL shader
               program. Reference: http://www.opengl.org/wiki/Shader_Compilation
 */
class GLSLProgram {
 public:
    /// Default constructor.
    GLSLProgram();
    /// Default destructor.
    ~GLSLProgram();

    /**
     * \brief      Compile the shader with the given file path.
     *
     * \param[in]  vertexShaderFilePath    The vertex shader file path
     * \param[in]  fragmentShaderFilePath  The fragment shader file path
     */
    void CompileShaders(const std::string& vertexShaderFilePath,
        const std::string& fragmentShaderFilePath);

    /**
     * \brief      Compile the shader.
     *
     * \param[in]  vertexSource    The vertex source
     * \param[in]  fragmentSource  The fragment source
     */
    void CompileShadersFromSource(const char* vertexSource,
        const char* fragmentSource);

    /// Link the shader.
    void LinkShaders();

    /**
     * \brief      Adds an attribute to a variable of the shader.
     *
     * \param[in]  attributeName  The attribute name
     */
    void AddAttribute(const std::string& attributeName);

    /**
     * \brief      Gets the uniform location.
     *
     * \param[in]  uniformName  The uniform name
     *
     * \return     The uniform location.
     */
    GLint GetUniformLocation(const std::string& uniformName);

    /// Use the shader.
    void Use();

    /// Unuse the shader.
    void Unuse();

    /// Terminates the shader.
    void Dispose();

 private:
    int m_numAttributes;

    void CompileShader(const char* source, const std::string& name,
        GLuint id);

    GLuint m_programID;

    GLuint m_vertexShaderID;
    GLuint m_fragmentShaderID;
};
}  // namespace GangerEngine

#endif  // _GLSLPROGRAM_H_
