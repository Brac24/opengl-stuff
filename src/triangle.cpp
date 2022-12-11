#include "glad.h"
#include <GLFW/glfw3.h>

//#include "pch.h"
#include <iostream>
#include <math.h>
#include <ShaderPipeline.h>
#include <fstream>
#include <sstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

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
    // Put contents of vertex and frag shaders into strings
    auto vertexShaderString = get_file_contents("../assets/glsl/plainVertexShader.vs");
    auto fragmentShaderString = get_file_contents("../assets/glsl/plainFragmentShader.fs");

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
    // to a vertex is called a vertext attribute. Color is a vertex attribute, position is a vertex attribute, etc.
    // Below are the 3 vertex positions that make up the triangle we want to draw.
    // Keep in mind that middle of the opengl window is the origin considered (0, 0)
    // where -1 <= x <= 1 and -1 <= y <= 1 and -1 <= z <= 1
    float vertices[] = {
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // Bottom right - Red
       -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   // Bottom left  - Green
        0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f    // Top          - Blue
    };

    // Construct shader source code
    //-------------------------------
    // VBO is a variable name that stands for Vertex Buffer Object
    // A VBO is an OpenGL object that allows us to interact with the GPU memory to store vertex data
    // Maybe a good way to think about it is as an interface to the GPU memory specifically for vertex data
    // For example a VBO allows us to assign vertex data to a GPU's memory where a vertex shader can then
    // take that vertex data in the GPU and process it. *Note that there are other kinds of buffer objects.
    unsigned int VBO;
    unsigned int VAO;
    // Generates a buffer object and assigns the buffer ID to the VBO variable. 
    // I believe this is just a general buffer object not specific to being a vertex buffer object.
    // First parameter of glGenBuffers is how many buffers we want to generate
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // This essentially directly binds the general buffer object to the vertex buffer and now VBO is considered a vertex buffer object.
    // GL_ARRAY_BUFFER is the buffer type that refers to a vertext buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    // This will copy the vertex data defined in vertices[] array into the VBO buffer memory
    // GL_STATIC_DRAW specifies to the GPU that the data will most likely not change at all
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Create a shader pipeline object that will compile and link our shaders into a 
    // shader program in our GPU
    ShaderPipeline shaderPipeline(vertexShaderString, fragmentShaderString);

    // A vertex attribute is a piece of information attached to the vertex
    // For example, vertex position is an attribute, vertex color is an attribute.
    // The first parameter specifies which attribute we are talking about. Here it is 0 since we only have a single attribute
    // which is position.
    // If we had a color attribute for the vertex then we would call this function again and that index parameter would
    // then say 1 which refers to the next attribute of the vertex.
    // The second parameter specifies number of components in that attribute (i.e. 3D vector would be 3 and 2D vector would be 2)
    // The third parameter specifies what type the data is which in our case it is just float data
    // Fourth parameter specifies whether we want OpenGL to normalize for us. We have already defined our vertex positions
    // in Normalized Device Coordinates manually so we set this to false
    // Fifth parameter specifies how many bytes are contained within a single vertex.
    // If we had 3D vector vertex position and a 4D vector RGBA vector color and each value is 4 bytes because each is a 32-bit float
    // then this stride parameter would the sum of all components for each attribute in that vector so 3 + 4 = 7
    // multiplied by the number of bytes in each component so 7*4 = 28 bytes. So there would be a total of 28 bytes
    // for a complete vertex that included its position and color attributes.
    // In our case we only specify one attribute that is a 3D position vector so it would be 3*4 = 12 bytes
    // The final parameter specifies the offset of where this specific attribute is.
    // If we had the 3D position and a 4D color vector for our vertex, the first attribute being position would have an
    // offset of 0 since it is our first attribute. The color vector would have an offset of 3*4 = 12 bytes.
    // We only have a position attribute so we have a 0 offset/
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0); // This API call is tied to the VAO
    //I wanted to provide what it might look like if I had a 3D color attribute for the vertex
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(0);
    //If we had that color attribute we would also have to enable attribute 1
    glEnableVertexAttribArray(1);

    glEnable(GL_MULTISAMPLE); // Used for antialiasing. Although Anitaliasing was still working without this. Simply giving the glfw hint was enough

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.2f, 0.3f, 0.6f, 1.0f); // sets the color to use when clearing the color buffer
        glClear(GL_COLOR_BUFFER_BIT);         // clear the color buffer using the color set above
        // input
        // -----
        processInput(window);

        
        //glUseProgram(shaderProgram);
        shaderPipeline.activate();
        float timeValue = glfwGetTime();
        float attenuateValue = (cos(timeValue)/2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderPipeline.getProgramId(), "attenuate");
        glUniform3f(vertexColorLocation, attenuateValue, attenuateValue, attenuateValue);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
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
    glViewport(0, 0, width, height);
}