#pragma once

#include "renderer/rendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void clear() override;

	virtual void draw(const std::shared_ptr<VertexArray>& vertexArray) override;

	virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};