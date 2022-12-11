#include <ShaderPipeline.h>
#include <iostream>


ShaderPipeline::ShaderPipeline(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{

    // Create vertex and fragment shader object with OpenGL. Below will return ID's for each shader
    uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    auto vSrc = vertexShaderSource.c_str();
    auto fSrc = fragmentShaderSource.c_str();
    // Associate the shader string with shader object created on the GPU
    glShaderSource(vertexShader, 1, &vSrc, NULL);
    glShaderSource(fragShader, 1, &fSrc, NULL);
    
    // Compile the shaders on the GPU
    glCompileShader(vertexShader);
    glCompileShader(fragShader);

    // Verify successful compilation of both shaders
    auto success = 0;
    auto shaderProgramError = 0; // used to know if at any point in compilation in linking of program there was an error
    char errorLog[512];

    //TODO: Extract into a shader compilation checker function
    // check vertex shader compilation status
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        shaderProgramError = 1;
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cout << "Error in vertex shader compilation" << errorLog << std::endl;
    }

    // Check fragment shader compilation status
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        shaderProgramError = 1;
        glGetShaderInfoLog(fragShader, 512, NULL, errorLog);
        std::cout << "Error in fragment shader compilation" << errorLog << std::endl;
    }

    // Create a program object on our GPU and attach shader objects to it and link the program
    mProgramId = glCreateProgram();
    glAttachShader(mProgramId, vertexShader);
    glAttachShader(mProgramId, fragShader);
    glLinkProgram(mProgramId);

    // Check successful linking of program
    glGetProgramiv(mProgramId, GL_LINK_STATUS, &success);

    if(!success)
    {
        shaderProgramError = 1;
        glGetProgramInfoLog(mProgramId, 512, NULL, errorLog);
        std::cout << "Error linking program " << errorLog << std::endl;
    }

    if(!shaderProgramError)
    {
        std::cout << "Compilation and Linking Successful" << std::endl;
    }

    // Delete shader objects from our GPU since they are already stored inside the program object
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

ShaderPipeline::~ShaderPipeline(){}

void ShaderPipeline::activate()
{
    glUseProgram(mProgramId);
}

uint32_t ShaderPipeline::getProgramId()
{
    return mProgramId;
}