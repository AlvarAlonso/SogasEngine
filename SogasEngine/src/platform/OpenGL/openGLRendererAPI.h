#pragma once

#include "../../renderer/rendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void clear() override;

	virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};