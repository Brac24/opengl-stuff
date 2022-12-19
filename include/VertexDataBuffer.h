#include <glad.h>
#include <vector>
#include <VertexAttribute.h>

class VertexDataBuffer
{
private:
    uint32_t mVao;
    uint32_t mVbo;
    uint32_t mIbo;
    std::vector<VertexAttribute> mVertexAttributes;
    
public:
/**
 * @brief Construct a new Vertex Data Buffer object
 * 
 * @param vertexAttributes 
 * @param totalComponentsPerVertex 
 * @param vertexData 
 * @param vertexDataSize size of the vertex data. This is needed because the correct size cannot be retrieved by calling sizeof(vertexData) in the constructor.
 *                       it instead gives us the size of the pointer address.
 */
    VertexDataBuffer(std::vector<VertexAttribute> vertexAttributes, int totalComponentsPerVertex, const float* vertexData, GLsizeiptr vertexDataSize);

    VertexDataBuffer(std::vector<VertexAttribute> vertexAttributes, int totalComponentsPerVertex, const float* vertexData, GLsizeiptr vertexDataSize,
                     const uint32_t* indices, GLsizeiptr indicesSize);
    ~VertexDataBuffer();
    void setVertexDataFloat(float* vertexData);
    void bind();
};