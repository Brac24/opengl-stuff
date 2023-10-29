#include "glad.h"
#include <GLFW/glfw3.h>

//#include "pch.h"
#include <iostream>
#include <math.h>
#include <ShaderPipeline.h>
#include <fstream>
#include <sstream>
#include <VertexDataBuffer.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "Texture.h"
#include <Instrumentor.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 768;
const unsigned int SCR_HEIGHT = 1024;

const uint16_t FRAG_SAMPLES = 8;

// Reading contents from a file
// Credit to stack overflow answer:
// https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
std::string get_file_contents(const char* filename)
{
	std::ifstream t(filename);
    std::stringstream buffer;
    buffer << t.rdbuf();

    return buffer.str();
}

int main()
{
    PROFILE_BEGIN_SESSION("opengl-profiling", "/tmp/myopengl-profile");
    // Put contents of vertex and frag shaders into strings
    auto vertexShaderString = get_file_contents("../assets/glsl/plainVertexShader.vs");
    auto fragmentShaderString = get_file_contents("../assets/glsl/plainFragmentShader.fs");
    auto textureVertexShaderString = get_file_contents("../assets/glsl/textureVertexShader.vs");
    auto textureFragmentShaderString = get_file_contents("../assets/glsl/textureFragmentShader.fs");

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, FRAG_SAMPLES); // Tells glfw how many samples to use in a pixel to figure out if a pixel should be colored or not.

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Bracamontes OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // initialize triangle vertices in Normalized Device Coordinates (NDC) for OpenGL
    // In real applications, these come into shaders as inputs but are not already in NDC as they are here.
    // *Note that by vertices in this code we mean vertex positions since other kinds of information can be
    // attached to a single vertex such as normals, color, etc. each of these pieces of information attached
    // to a vertex is called a vertex attribute. Color is a vertex attribute, position is a vertex attribute, etc.
    // Below are the 3 vertex positions that make up the triangle we want to draw.
    // Keep in mind that middle of the opengl window is the origin considered (0, 0)
    // where -1 <= x <= 1 and -1 <= y <= 1 and -1 <= z <= 1
    float vertices[] = {
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  // Bottom right  - Red   (RGBA - 1001)   index 0
       -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   // Bottom left  - Green (RGBA - 0101)   index 1
        0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,    // Top          - Blue  (RGBA = 0011)  index 2
    };

    float squareVertices[] = {
        // Defining vertices for square
        //--- Position --    |---- Color -----------    |---Texture Coordinate-----|
       -0.25f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f, 0.5f,     0.0f, 0.0f, // bottom left corner   index 0
        0.0f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f, 0.5f,     1.0f, 0.0f, // bottom right corner  index 1
       -0.25f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 0.5f,     0.0f, 1.0f, // top left             index 2
        0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 0.5f,     1.0f, 1.0f  // top right            index 3
    };

    uint16_t triangleIndices[] = {0,1,2};
    uint32_t squareIndices[] = {0,1,2, 1,2,3};

    // Create a shader pipeline object that will compile and link our shaders into a 
    // shader program in our GPU
    ShaderPipeline shaderPipeline(vertexShaderString, fragmentShaderString);

    // Shader program used for textures
    ShaderPipeline textureShaderPipeline(textureVertexShaderString, textureFragmentShaderString);

    // Define available vertex attributes. These attributes are the attributes in the glsl programs labeledl with the layout =  command
    VertexAttribute positionAttribute(0, 3, GL_FLOAT, GL_FALSE);
    VertexAttribute colorAttribute(1, 4, GL_FLOAT, GL_FALSE);
    VertexAttribute texCoordinateAttribute(2, 2, GL_FLOAT, GL_FALSE);

    // Creates a VBO and VAO and sets the vertex data to the VBO
    VertexDataBuffer objectDataBuffer(std::vector<VertexAttribute>{positionAttribute, colorAttribute}, 7, vertices, sizeof(vertices));

    // Creates vertex buffer object containing square. This also creates the index buffer used to draw the square
    VertexDataBuffer squareDataBuffer(std::vector<VertexAttribute>{positionAttribute, colorAttribute, texCoordinateAttribute}, 9, squareVertices, sizeof(squareVertices), squareIndices, sizeof(squareIndices));
    

    // Texture Implementation
    // load the image data of the container crate
    Texture containerTexture("../assets/textures/container.jpg"); // This must be called after the OpenGL context has been initialized with glfw or else the glGenTextures in the constructor will seg fault.
    containerTexture.bind();
    containerTexture.setDefaultTextureParams();
    containerTexture.loadToGpu();
    //containerTexture.generateMipmap(); // Used to use this because setDefaultTextureParams() set up use of mip maps but that was removed
    //PROFILE_FUNCTION();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        {

        PROFILE_SCOPE("RenderLoop Clear Screen");
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.2f, 0.3f, 0.6f, 1.0f); // sets the color to use when clearing the color buffer
        glClear(GL_COLOR_BUFFER_BIT);         // clear the color buffer using the color set above
        }

{
        PROFILE_SCOPE("RenderLoop Process Input");
        // input
        // -----
        processInput(window);
}
float timeValue;
float attenuateValue;
int vertexColorLocation;
{
        PROFILE_SCOPE("RenderLoop Drawing Triangle");        
        shaderPipeline.activate();
        timeValue = glfwGetTime();
        attenuateValue = (cos(timeValue)/2.0f) + 0.5f;
        vertexColorLocation = glGetUniformLocation(shaderPipeline.getProgramId(), "attenuate");
        glUniform3f(vertexColorLocation, attenuateValue, attenuateValue, attenuateValue);
       
        // Bind the triangle data which exists in objectDataBuffer and draw triangle
        objectDataBuffer.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3); // Draw the currently bound buffer.
}
{
        PROFILE_SCOPE("RenderLoop Drawing Texture");
        // Bind and draw square
        textureShaderPipeline.activate();                 // activate the shader program that contains the glsl that samples textures
        vertexColorLocation = glGetUniformLocation(shaderPipeline.getProgramId(), "attenuate");
        glUniform3f(vertexColorLocation, attenuateValue, attenuateValue, attenuateValue);
        squareDataBuffer.bind();                          // bind our vertex data which include texture coordinates for each vertex
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
{
        PROFILE_SCOPE("RenderLoop Swap Buffers and Poll Events");
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
       glfwPollEvents();
}
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    PROFILE_END_SESSION();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		std::cout << "W Pressed" << std::endl;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height); // calling this will redraw symbols to fit the new window size
}