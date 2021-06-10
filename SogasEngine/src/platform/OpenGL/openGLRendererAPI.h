#pragma once

#include "renderer/rendererAPI.h"

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void clear() override;

	virtual void setClearColor(const glm::vec4& colour) override;

	virtual void setDepthBuffer(const bool& setOn) override;

	virtual void draw(const std::shared_ptr<VertexArray>& vertexArray) override;

	virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
};