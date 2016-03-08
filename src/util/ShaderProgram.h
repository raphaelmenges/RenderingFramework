#ifndef SHADER_PROGRAM_H_
#define SHADER_PROGRAM_H_

#include "externals/gl3w/include/GL/gl3w.h"
#include "externals/glm/glm/glm.hpp"

#include <string>
#include <set>

class ShaderProgram
{
public:

    // TODO: maybe super class and subclass is standard shader pipeline and compute shader
    // TODO: cache for uniforms and / or use location directly
    // TODO: log shader compilation log at error
    // TODO: uniform buffer

    // Constructor (pathes relative to SHADER_PATH)
    ShaderProgram(std::string vsFilepath, std::string fsFilepath);
    ShaderProgram(std::string vsFilepath, std::string gsFilepath, std::string fsFilepath);

    // Destructor
    virtual ~ShaderProgram();

    // Compile (if already compiled, rebind is necessary! Handle may change)
    void compile(bool bind = true);

    // Bind (MUST BE CALLED BEFORE UPDATING UNIFORMS!!!)
    void bind() const;

    // Getter for program handle
    GLuint getProgram() const;

    // Add define
    void addDefine(std::string define);

    // Remove define
    void removeDefine(std::string define);

    // Find define
    bool findDefine(std::string define) const;

    // Update uniform
    void updateUniform(std::string name, const float& rValue) const;
    void updateUniform(std::string name, const int& rValue) const;
    void updateUniform(std::string name, const glm::vec3& rValue) const;
    void updateUniform(std::string name, const glm::mat4& rValue) const;

protected:

    // Struct for shader (no OpenGL handle, since only locally compiled)
    struct Shader
    {
        std::string filepath; // if empty, shader is not used
    };

    std::string readShaderFile(std::string filepath) const;

    GLuint compileShader(const std::string& rData, GLenum shaderType) const;

    void logShaderInfo(GLuint shaderHandle) const;

    // Members
    bool mProgramLinked;
    GLuint mProgram;
    Shader mVertexShader;
    Shader mGeometryShader;
    Shader mFragmentShader;
    std::set<std::string> mDefines;
};

#endif // SHADER_PROGRAM_H_
