#pragma once

#include "defines.h"
#include "renderer/VertexArray.h"
#include <string>

namespace Sogas 
{
	class SGS Mesh
	{
	public:

		Mesh();

		void load(const std::string& filename);

		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<IndexBuffer> m_indexBuffer;

	private:

		std::shared_ptr<VertexBuffer> m_vertexBuffer;
	};
}

