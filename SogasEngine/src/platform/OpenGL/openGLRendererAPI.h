#pragma once

#include "renderer/rendererAPI.h"

namespace Sogas 
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual bool init() override { return true; }
		virtual void shutdown() override {};

		virtual void clear() override;
		virtual void setClearColor(const glm::vec4& colour) override;
		virtual void enableDepthBuffer(const bool& depth) override;
		virtual void setDepthFunc(const DepthTypes factor) override;
		virtual void draw(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive = Primitive::TRIANGLES) override;
		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive = Primitive::TRIANGLES) override;
		virtual void setBlendFunc(const BlendTypes sfactor, const BlendTypes dfactor) override;
		virtual void enableBlend(const bool blend) override;
		virtual void setLineWidth(const f32 width) override;
	};
}