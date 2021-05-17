#pragma once

#include "../external/GLEW/glew-2.1.0/include/GL/glew.h"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "shader.h"

class renderer
{
public:
	void clear() const;
	void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
};

