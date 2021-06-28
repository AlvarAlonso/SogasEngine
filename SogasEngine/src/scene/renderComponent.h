#pragma once

#include "defines.h"
#include "entityComponent.h"

#include "renderer/resources/mesh.h"

namespace Sogas
{
	class SGS RenderComponent : public EntityComponent
	{
	public:
		static const char* s_name;

		RenderComponent() {}
		virtual bool init() override;
		virtual const char* getName() const override { return s_name; }

		Mesh* getMesh(void) const { return m_pMesh; }
		void setMesh(const char* filename) { m_pMesh = new Mesh; m_pMesh->load(filename); }

	private:
		Mesh* m_pMesh;
	};
}