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

    // Compile
    void compile();

    // Bind (MUST BE CALLED BEFORE UPDATING UNIFORMS!!!)
    void bind() const;

    // Getter for program handle
    GLuint getProgram() const;

    // Update uniform
    void updateUniform(std::string name, const float& rValue) const;
    void updateUniform(std::string name, const int& rValue) const;
    void updateUniform(std::string name, const glm::vec3& rValue) const;
    void updateUniform(std::string name, const glm::mat4& rValue) const;

protected:

    std::string readShaderFile(std::string filepath, const std::set<std::string>& rDefines) const;

    void logShaderInfo(GLuint shaderHandle) const;

    // Members
    bool mProgramLinked;
    GLuint mProgram;
    std::string mVertexShaderFilepath;
    std::string mGeometryShaderFilepath;
    std::string mFragmentShaderFilepath;
};

#endif // SHADER_PROGRAM_H_
