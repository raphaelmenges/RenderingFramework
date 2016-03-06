#ifndef SHADER_H_
#define SHADER_H_

#include "externals/gl3w/include/GL/gl3w.h"

#include <string>
#include <vector>

class Shader
{
public:

    // TODO: maybe super class and subclass is standard shader pipeline and compute shader
	// TODO: cache for uniforms and / or use location directly
	// TODO: log shader compilation log at error
	// TODO: uniform buffer

    // Constructor (pathes relative to SHADER_PATH)
    Shader(std::string vsFilepath, std::string fsFilepath, std::string gsFilepath = "");

    // Destructor
    virtual ~Shader();

	// Compile
	void compile();

    // Bind (MUST BE CALLED BEFORE UPDATING UNIFORMS!!!)
    void bind() const;

	// Getter for program handle
	GLuint getProgram() const;

    // Update uniform
    void updateUniform(std::string name, float value) const; 

protected:

    std::string readShaderFile(std::string filepath, const std::vector<std::string>& rDefines) const;

	void logShaderInfo(GLuint shaderHandle) const;

	// Members
	bool mProgramLinked;
	GLuint mProgram;
	std::string mVertexShaderFilepath;
	std::string mGeometryShaderFilepath;
	std::string mFragmentShaderFilepath;
};

#endif // SHADER_H_
