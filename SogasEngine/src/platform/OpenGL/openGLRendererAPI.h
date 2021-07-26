#pragma once

#include "renderer/rendererAPI.h"

namespace Sogas 
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void clear() override;
		virtual void setClearColor(const glm::vec4& colour) override;
		virtual void setDepthBuffer(const bool& setOn) override;
		virtual void draw(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive = Primitive::TRIANGLES) override;
		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive = Primitive::TRIANGLES) override;
		virtual void setBlendFunc(bool blend) override;
		virtual void enableBlend(const bool blend) override;
		virtual void setLineWidth(const f32 width) override;
	};
}