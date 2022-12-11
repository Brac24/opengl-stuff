#include <glad.h>
#include <string>

#pragma once

class ShaderPipeline
{
private:
    uint32_t mProgramId;
public:
    ShaderPipeline(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~ShaderPipeline();
    void activate();
    uint32_t getProgramId();
};