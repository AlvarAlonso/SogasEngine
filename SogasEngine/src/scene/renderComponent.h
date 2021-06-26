#pragma once

#include "defines.h"
#include "entityComponent.h"

#include "renderer/resources/mesh.h"

namespace Sogas
{
	class RenderComponent : public EntityComponent
	{
	public:
		Mesh* getMesh(void) const { return m_pMesh; }
		void setMesh(Mesh* newMesh) { m_pMesh = newMesh; }

		static const char* s_name;
		//virtual const char* getName() override { return s_name; }
	private:
		Mesh* m_pMesh;
	};
}