#include "sgspch.h"
#include "environment.h"

#include "renderer/resources/mesh.h"
#include "renderer/resources/texture.h"

namespace Sogas
{
	Environment::Environment()
	{
		m_pMesh = Mesh::GET("cube.obj");
		m_pTexture = TextureCubeMap::GET("hardcodedRightNow");
	}
}