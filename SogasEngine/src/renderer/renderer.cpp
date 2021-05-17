#include "renderer.h"

void renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	shader.bind();
	vertexArray.bind();
	indexBuffer.bind();
	glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
}
