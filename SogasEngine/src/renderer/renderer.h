#pragma once

#include "rendererAPI.h"

class Renderer
{
public:
	enum class API 
	{
		None = 0, OpenGL = 1
	};
public:
	inline static API getAPI() { return s_API; }

	inline static void clear()
	{
		s_RendererAPI->clear();
	}

	inline static void draw(const std::shared_ptr<VertexArray>& vertexArray)
	{
		s_RendererAPI->draw(vertexArray);
	}

	inline static void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		s_RendererAPI->drawIndexed(vertexArray);
	}

private:
	static API s_API; // TODO: Hide the API to the rest of the application
	static RendererAPI* s_RendererAPI;
};