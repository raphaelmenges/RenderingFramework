#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <vector>
#include <sstream>

class Shader
{
public:

    // TODO: maybe super class and subclass is standard shader pipeline and compute shader

    // Constructor
    Shader(std::string vsFilepath, std::string fsFilepath, std::string gsFilepath = "");

    // Destructor
    virtual ~Shader();

    // Bind
    void bind() const;

    // Update uniform
    void updateUniform(std::string name, float value) const;

    // TODO: uniform buffer

protected:

    std::string readShaderFile(std::string fullpath, std::vector<std::string> defines) const;
};

#endif // SHADER_H_
