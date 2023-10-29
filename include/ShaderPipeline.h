#include <glad.h>
#include <string>
#include <glm/glm.hpp>

#pragma once

class ShaderPipeline
{
private:
    uint32_t mProgramId;
public:
    ShaderPipeline(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~ShaderPipeline();
    void activate();
    void setUniformFloat(const std::string& uniformName, const glm::vec3& value);
    uint32_t getProgramId();
};