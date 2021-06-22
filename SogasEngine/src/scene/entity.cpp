#include "sgspch.h"

#include "entity.h"
#include "core/application.h"

#include "../external/imgui/imgui.h"
#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>

namespace Sogas 
{
	Entity::Entity() : m_model(glm::mat4(1))
	{
	}

	Renderable::Renderable() : Entity()
	{
		m_name = "Renderable";
	}

	Renderable::Renderable(const std::string& name) : Entity()
	{
		m_name = name;
	}

	void Renderable::OnImguiRender()
	{
		ImGui::Text("Name %s", m_name.c_str());

		if (ImGui::Button("Selected"))
			Application::m_guizmoEntity = this;

		if(ImGui::TreeNode((void*)this, "Model"))
		{
			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(m_model), matrixTranslation, matrixRotation, matrixScale);
			ImGui::DragFloat3("Position", matrixTranslation, 0.1f);
			ImGui::DragFloat3("Rotation", matrixRotation, 0.1f);
			ImGui::DragFloat3("Scale", matrixScale, 0.1f);
			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(m_model));

			ImGui::TreePop();
		}
	}

	Light::Light() : Entity()
	{
		m_name = "Light";
	}

	Light::Light(const std::string& name) : Entity()
	{
		m_name = name;
	}

	void Light::OnImguiRender()
	{
		ImGui::Text("Name: %s", m_name.c_str());

		if (ImGui::Button("Selected"))
			Application::m_guizmoEntity = this;

		if(ImGui::TreeNode((void*)this, "Model"))
		{
			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(m_model), matrixTranslation, matrixRotation, matrixScale);
			ImGui::DragFloat3("Position", glm::value_ptr(m_model[3]), 0.1f);
			ImGui::DragFloat3("Rotation", matrixRotation, 0.1f);
			ImGui::DragFloat3("Scale", matrixScale, 0.1f);
			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(m_model));

			ImGui::TreePop();
		}
	}
}
