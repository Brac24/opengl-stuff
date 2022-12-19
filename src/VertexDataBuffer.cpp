#include <VertexDataBuffer.h>
#include <string>
#include <iostream>

VertexDataBuffer::VertexDataBuffer(std::vector<VertexAttribute> vertexAttributes, int totalComponentsPerVertex, const float* vertexData, GLsizeiptr vertexDataSize):
mVertexAttributes(vertexAttributes)
{
    // Generates a buffer object and assigns the buffer ID to the VBO variable. 
    // I believe this is just a general buffer object not specific to being a vertex buffer object.
    // First parameter of glGenBuffers is how many buffers we want to generate
    glGenBuffers(1, &mVbo);
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);
    // This essentially directly binds the general buffer object to the vertex buffer and now VBO is considered a vertex buffer object.
    // GL_ARRAY_BUFFER is the buffer type that refers to a vertext buffer object
    glBindBuffer(GL_ARRAY_BUFFER, mVbo); 

    glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_DYNAMIC_DRAW);

    auto attributeStart = 0;
    for(int i=0; i<mVertexAttributes.size(); i++)
    {
        auto attributePointerIncrement = i > 0 ? mVertexAttributes[i-1].getNumberOfComponents() : 0;
        attributeStart += attributePointerIncrement;

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
        // then this stride parameter would the sum of all components for each attribute in that vector so 3 components for position + 4 components for color = 7
        // multiplied by the number of bytes in each component so 7*4 = 28 bytes. So there would be a total of 28 bytes
        // for a complete vertex that included its position and color attributes.
        // In our case we only specify one attribute that is a 3D position vector so it would be 3*4 = 12 bytes
        // The final parameter specifies the offset of where this specific attribute is.
        // If we had the 3D position and a 4D color vector for our vertex, the first attribute being position would have an
        // offset of 0 since it is our first attribute. The color vector would have an offset of 3*4 = 12 bytes.
        // We only have a position attribute so we have a 0 offset/
        glVertexAttribPointer(i, mVertexAttributes[i].getNumberOfComponents(), GL_FLOAT, GL_FALSE, totalComponentsPerVertex*sizeof(float), (void*)(attributeStart*sizeof(float))); // This API call is tied to the VAO
        glEnableVertexAttribArray(i);
    }
}

VertexDataBuffer::VertexDataBuffer(std::vector<VertexAttribute> vertexAttributes, int totalComponentsPerVertex, const float* vertexData, GLsizeiptr vertexDataSize,
                     const uint32_t* indices, GLsizeiptr indicesSize):
                     mVertexAttributes(vertexAttributes)
{
    // Generates a buffer object and assigns the buffer ID to the VBO variable. 
    // I believe this is just a general buffer object not specific to being a vertex buffer object.
    // First parameter of glGenBuffers is how many buffers we want to generate
    glGenBuffers(1, &mVbo);
    glGenVertexArrays(1, &mVao);
    glBindVertexArray(mVao);
    // This essentially directly binds the general buffer object to the vertex buffer and now VBO is considered a vertex buffer object.
    // GL_ARRAY_BUFFER is the buffer type that refers to a vertext buffer object
    glBindBuffer(GL_ARRAY_BUFFER, mVbo); 

    glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_DYNAMIC_DRAW);

    auto attributeStart = 0;
    for(int i=0; i<mVertexAttributes.size(); i++)
    {
        auto attributePointerIncrement = i > 0 ? mVertexAttributes[i-1].getNumberOfComponents() : 0;
        attributeStart += attributePointerIncrement;

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
        // then this stride parameter would the sum of all components for each attribute in that vector so 3 components for position + 4 components for color = 7
        // multiplied by the number of bytes in each component so 7*4 = 28 bytes. So there would be a total of 28 bytes
        // for a complete vertex that included its position and color attributes.
        // In our case we only specify one attribute that is a 3D position vector so it would be 3*4 = 12 bytes
        // The final parameter specifies the offset of where this specific attribute is.
        // If we had the 3D position and a 4D color vector for our vertex, the first attribute being position would have an
        // offset of 0 since it is our first attribute. The color vector would have an offset of 3*4 = 12 bytes.
        // We only have a position attribute so we have a 0 offset/
        glVertexAttribPointer(i, mVertexAttributes[i].getNumberOfComponents(), GL_FLOAT, GL_FALSE, totalComponentsPerVertex*sizeof(float), (void*)(attributeStart*sizeof(float))); // This API call is tied to the VAO
        glEnableVertexAttribArray(i);
    }

    // Create Index Buffer object
    glGenBuffers(1, &mIbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_DYNAMIC_DRAW);
}

VertexDataBuffer::~VertexDataBuffer(){}

void VertexDataBuffer::setVertexDataFloat(float* vertexData)
{
    //glBindVertexArray(mVao);
    //glBindBuffer(GL_ARRAY_BUFFER, mVbo); 
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_DYNAMIC_DRAW);
}

void VertexDataBuffer::bind()
{
    glBindVertexArray(mVao);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
}