#pragma once

#include "rendererAPI.h"

namespace Sogas
{
	class RenderCommand
	{
	public:

		inline static void clear()
		{
			s_rendererAPI->clear();
		}

		inline static void setClearColor(const glm::vec4& color)
		{
			s_rendererAPI->setClearColor(color);
		}

		inline static void setDepthBuffer(const bool& setOn)
		{
			s_rendererAPI->setDepthBuffer(setOn);
		}

		inline static void draw(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive = Primitive::TRIANGLES)
		{
			s_rendererAPI->draw(vertexArray, primitive);
		}

		inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray, const Primitive primitive = Primitive::TRIANGLES)
		{
			s_rendererAPI->drawIndexed(vertexArray, primitive);
		}

		inline static void setBlendFunc(bool blend)
		{
			s_rendererAPI->setBlendFunc(blend);
		}

		inline static void enableBlend(bool blend)
		{
			s_rendererAPI->enableBlend(blend);
		}

		inline static void setLineWidth(const f32 width)
		{
			s_rendererAPI->setLineWidth(width);
		}

	private:
		static RendererAPI* s_rendererAPI;
	};
}