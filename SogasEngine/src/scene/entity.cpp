#include "sgspch.h"

#include "entity.h"

#include "../external/imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

namespace Sogas 
{
	Entity::Entity() : m_model(glm::mat4(1))
	{
	}

	Renderable::Renderable() : Entity()
	{
	}

	Renderable::Renderable(const std::string& name) : Entity()
	{
		m_name = name;
	}

	void Renderable::OnImguiRender()
	{
		ImGui::Text("Name %s", m_name.c_str());

		if(ImGui::TreeNode((void*)this, "Model"))
		{
			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGui::DragFloat3("Position", glm::value_ptr(m_model[3]), 0.1f);
			ImGui::DragFloat3("Rotation", matrixRotation, 0.1f);
			ImGui::DragFloat3("Scale", matrixScale, 0.1f);

			ImGui::TreePop();
		}
	}

	Light::Light() : Entity()
	{
	}

	Light::Light(const std::string& name) : Entity()
	{
		m_name = name;
	}
	void Light::OnImguiRender()
	{
	}
}
