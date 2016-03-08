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
    mVertexShaderFilepath = vsFilepath;
    mGeometryShaderFilepath = gsFilepath;
    mFragmentShaderFilepath = fsFilepath;
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

void ShaderProgram::compile()
{
    // Create defines (TODO: should be filled somehow)
    std::set<std::string> vertexDefines;
    std::set<std::string> geometryDefines;
    std::set<std::string> fragmentDefines;

    // Vertex shader
    std::string vertexData = readShaderFile(mVertexShaderFilepath, vertexDefines);
    std::vector<char> vertexBuffer(vertexData.size() + 1);
    copy(vertexData.begin(), vertexData.end(), vertexBuffer.begin());
    const char* vertexShaderSource = &vertexBuffer[0];
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    logShaderInfo(vertexShader);

    // Geometry shader
    GLuint geometryShader = 0;
    if (!mGeometryShaderFilepath.empty())
    {
        std::string geometryData = readShaderFile(mGeometryShaderFilepath, geometryDefines);
        std::vector<char> geometryBuffer(geometryData.size() + 1);
        copy(geometryData.begin(), geometryData.end(), geometryBuffer.begin());
        const char* geometryShaderSource = &geometryBuffer[0];

        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);
        logShaderInfo(geometryShader);
    }

    // Fragment shader
    std::string fragmentData = readShaderFile(mFragmentShaderFilepath, fragmentDefines);
    std::vector<char> fragmentBuffer(fragmentData.size() + 1);
    copy(fragmentData.begin(), fragmentData.end(), fragmentBuffer.begin());
    const char* fragmentShaderSource = &fragmentBuffer[0];
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    logShaderInfo(fragmentShader);

    // Delete old program if there exists one
    if (mProgramLinked)
    {
        glDeleteProgram(mProgram);
        mProgramLinked = false;
    }

    // Create program
    mProgram = glCreateProgram();
    glAttachShader(mProgram, vertexShader);
    if (!mGeometryShaderFilepath.empty())
    {
        glAttachShader(mProgram, geometryShader);
    }
    glAttachShader(mProgram, fragmentShader);
    glLinkProgram(mProgram);

    // Delete shaders
    glDeleteShader(vertexShader);
    if (!mGeometryShaderFilepath.empty())
    {
        glDeleteShader(geometryShader);
    }
    glDeleteShader(fragmentShader);

    // Remember, that program was linked
    mProgramLinked = true;
}

void ShaderProgram::bind() const
{
    glUseProgram(mProgram);
}

GLuint ShaderProgram::getProgram() const
{
    return mProgram;
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

std::string ShaderProgram::readShaderFile(std::string filepath, const std::set<std::string>& rDefines) const
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
    bool check = rDefines.empty(); // True when empty, otherwise false

    while (getline(ss, line, '\n'))
    {
        if (line.compare(DEFINES_PHRASE) == 0)
        {
            // Add defines instead of DEFINES_PHRASE
            for (std::string define : rDefines)
            {
                dataWithDefines += define + "\n";
            }

            // Set check to true because DEFINES_PHRASE was found replaced
            check = true;
        }
        else
        {
            // Copy code from shader file
            dataWithDefines += line + "\n";
        }
    }

    // TODO: log something when check is false

    // Return content of shader file including defines
    return dataWithDefines;
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
