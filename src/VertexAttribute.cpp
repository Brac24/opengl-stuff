#include <VertexAttribute.h>

VertexAttribute::VertexAttribute(uint32_t attributeIndex, uint32_t numOfComponents, uint32_t attributeType, bool haveOpenGlNormalize):
mAttributeIndex(attributeIndex),
mAttributeIndexNumOfComponents(numOfComponents),
mAttributeType(attributeType),
mHaveOpenGlNormalize(haveOpenGlNormalize)
{
    
}

VertexAttribute::~VertexAttribute(){}

uint32_t VertexAttribute::getNumberOfComponents()
{
    return mAttributeIndexNumOfComponents;
}

uint32_t VertexAttribute::getAttributeType()
{
    return mAttributeType;
}

bool VertexAttribute::getHaveOpenGlNormalize()
{
    return mHaveOpenGlNormalize;
}
