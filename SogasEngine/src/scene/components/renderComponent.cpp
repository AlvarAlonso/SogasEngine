
#include "sgspch.h"
#include "renderComponent.h"
#include "renderer/shader.h"

namespace Sogas
{
	const char* RenderComponent::s_name = "RenderComponent";

	RenderComponent::RenderComponent()
	{
		m_pMesh = nullptr;
		m_pMaterial = std::make_shared<Material>(Shader::create("../SogasEngine/shaders/basic.shader"));
	}

	bool RenderComponent::init()
	{
		//m_pMesh = nullptr;
		//m_pMaterial = std::make_shared<Material>(new Material());
		return true;
	}

	/*
	* Serialize RenderComponent to json data
	* @param json&
	* @retun void
	*/
	//void RenderComponent::to_json(json& j)
	//{
	//	j = json{
	//		{"Mesh", getMesh()->getMeshName() },
	//		{"Material", nullptr}
	//	};
	//}
}