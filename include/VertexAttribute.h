#include <glad.h>

class VertexAttribute
{
private:
    uint32_t mAttributeIndex;
    uint32_t mAttributeIndexNumOfComponents;
    uint32_t mAttributeType;
    bool mHaveOpenGlNormalize;
public:
    VertexAttribute(uint32_t attributeIndex, uint32_t numOfComponents, uint32_t attributeType, bool haveOpenGlNormalize=false);
    ~VertexAttribute();

    uint32_t getNumberOfComponents();
    uint32_t getAttributeType();
    bool getHaveOpenGlNormalize();
};