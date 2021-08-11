#pragma once

#include "defines.h"
#include <memory>

namespace Sogas
{
	class TextureCubeMap;
	class Mesh;

	class SGS Environment
	{
	public:

		Environment();
		void setMesh(std::shared_ptr<Mesh> mesh) { m_pMesh = mesh; }
		void setTexture(std::shared_ptr<TextureCubeMap> texture) { m_pTexture = texture; }

		std::shared_ptr<Mesh> getMesh(void) { return m_pMesh; }
		std::shared_ptr<TextureCubeMap> getTexture(void) { return m_pTexture; }

	private:
		std::shared_ptr<Mesh> m_pMesh;
		std::shared_ptr<TextureCubeMap> m_pTexture;
	};
}