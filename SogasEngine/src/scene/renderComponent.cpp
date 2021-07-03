
#include "sgspch.h"
#include "renderComponent.h"

namespace Sogas
{
	const char* RenderComponent::s_name = "RenderComponent";

	bool RenderComponent::init()
	{
		m_pMesh = nullptr;
		return true;
	}
}