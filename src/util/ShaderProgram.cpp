#include "ShaderProgram.h"

#include "externals/glm/glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// TODO: delete
#include <iostream>

const std::string DEFINES_PHRASE = "#defines";

ShaderProgram::ShaderProgram(std::string vsFilepath, std::string fsFilepath) : ShaderProgram(vsFilepath, "", fsFilepath)
{
    // Piped to other constructor
}

ShaderProgram::ShaderProgram(std::string vsFilepath, std::string gsFilepath, std::string fsFilepath)
{
    mVertexShader.filepath = vsFilepath;
    mGeometryShader.filepath = gsFilepath;
    mFragmentShader.filepath = fsFilepath;
    mProgramLinked = false;
}

ShaderProgram::~ShaderProgram()
{
    if (mProgramLinked)
    {
        // Delete linked program
        glDeleteProgram(mProgram);
    }
}

void ShaderProgram::compile(bool bind)
{
    // Vertex shader
    std::string vertexData = readShaderFile(mVertexShader.filepath);
    GLuint vertexShader = compileShader(vertexData, GL_VERTEX_SHADER);

    // Geometry shader
    GLuint geometryShader = 0;
    if (!mGeometryShader.filepath.empty())
    {
        std::string geometryData = readShaderFile(mGeometryShader.filepath);
        geometryShader = compileShader(geometryData, GL_GEOMETRY_SHADER);
    }

    // Fragment shader
    std::string fragmentData = readShaderFile(mFragmentShader.filepath);
    GLuint fragmentShader = compileShader(fragmentData, GL_FRAGMENT_SHADER);

    // Delete old program if there exists one
    if (mProgramLinked)
    {
        glDeleteProgram(mProgram);
        mProgramLinked = false;
    }

    // Create program
    mProgram = glCreateProgram();
    glAttachShader(mProgram, vertexShader);
    if (!mGeometryShader.filepath.empty())
    {
        glAttachShader(mProgram, geometryShader);
    }
    glAttachShader(mProgram, fragmentShader);
    glLinkProgram(mProgram);

    // Delete shaders
    glDeleteShader(vertexShader);
    if (!mGeometryShader.filepath.empty())
    {
        glDeleteShader(geometryShader);
    }
    glDeleteShader(fragmentShader);

    // Remember, that program was linked
    mProgramLinked = true;

    // Bind if wished
    if(bind)
    {
        this->bind();
    }
}

void ShaderProgram::bind() const
{
    glUseProgram(mProgram);
}

GLuint ShaderProgram::getProgram() const
{
    return mProgram;
}

void ShaderProgram::addDefine(std::string define)
{
    mDefines.insert(define);
}

void ShaderProgram::removeDefine(std::string define)
{
    mDefines.erase(define);
}

bool ShaderProgram::findDefine(std::string define) const
{
    return mDefines.find(define) != mDefines.end();
}

void ShaderProgram::updateUniform(std::string name, const float& rValue) const
{
    GLuint location = glGetUniformLocation(mProgram, name.c_str());
    glUniform1f(location, rValue);
}

void ShaderProgram::updateUniform(std::string name, const int& rValue) const
{
    GLuint location = glGetUniformLocation(mProgram, name.c_str());
    glUniform1i(location, rValue);
}

void ShaderProgram::updateUniform(std::string name, const glm::vec3& rValue) const
{
    GLuint location = glGetUniformLocation(mProgram, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(rValue));
}


void ShaderProgram::updateUniform(std::string name, const glm::mat4& rValue) const
{
    GLuint location = glGetUniformLocation(mProgram, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(rValue));
}

std::string ShaderProgram::readShaderFile(std::string filepath) const
{
    // Create full path
    std::string fullpath = std::string(SHADERS_PATH) + "/" + filepath;

    // Open file
    std::ifstream is(fullpath.c_str(), std::ifstream::in);

    // Check file
    if (!is)
    {
        // TODO: log error
    }

    // Read data from file
    std::string data((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

    // Find DEFINES_PHRASE
    std:: stringstream ss(data);
    std::string line;
    std::string dataWithDefines;
    bool check = mDefines.empty(); // True when empty, otherwise false

    while (getline(ss, line, '\n'))
    {
        if (line.compare(DEFINES_PHRASE) == 0)
        {
            // Add defines instead of DEFINES_PHRASE
            for (std::string define : mDefines)
            {
                dataWithDefines += "#define " + define + "\n";
            }

            // Set check to true because DEFINES_PHRASE was found and replaced
            check = true;
        }
        else
        {
            // Copy code from shader file
            dataWithDefines += line + "\n";
        }
    }

    // TODO: log something when check is false
    if(!check)
    {
        std::cout << "Define phrase was not found." << filepath << std::endl;
    }

    // Return content of shader file including defines
    return dataWithDefines;
}

GLuint ShaderProgram::compileShader(const std::string& rData, GLenum shaderType) const
{
    std::vector<char> buffer(rData.size() + 1);
    copy(rData.begin(), rData.end(), buffer.begin());
    const char* source = &buffer[0];
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle, 1, &source, NULL);
    glCompileShader(shaderHandle);
    logShaderInfo(shaderHandle);
    return shaderHandle;
}

void ShaderProgram::logShaderInfo(GLuint shaderHandle) const
{
    // Get length of log
    GLint logLength = 0;
    if (glIsShader(shaderHandle))
    {
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
    }

    // If there is log, write it to debug logging output
    if (logLength > 1)
    {
        // Copy log to chars
        char* log = new char[logLength];
        glGetShaderInfoLog(shaderHandle, logLength, NULL, log);

        // TODO: use logger
        std::cout << log << std::endl;

        // Delete chars
        delete[] log;
    }
}
