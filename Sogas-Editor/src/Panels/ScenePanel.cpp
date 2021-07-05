#include "ScenePanel.h"

#include "scene/renderComponent.h"
#include "scene/cameraComponent.h"
#include "scene/transformComponent.h"
#include "scene/lightComponent.h"

#include <imgui.h>

namespace Sogas
{
	ScenePanel::ScenePanel(const std::shared_ptr<Scene>& scene)
	{
		setContext(scene);
	}

	void ScenePanel::setContext(const std::shared_ptr<Scene>& scene)
	{
		m_context = scene;
		m_selectedEntity = {};
	}

	void ScenePanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		//SGSINFO("Entity Selected %i", m_selectedEntity.getId());
		EntityId entityToDestroy = 0;
		for (auto &entity : m_context.get()->getEntities())
		{
			//ImGui::Text(entity->getName().c_str());
			EntityId id = drawEntityNode(*entity);
			
			if(id != 0)
				entityToDestroy = id;
		}

		if(entityToDestroy != 0)
		{
			m_context->destroyEntity(entityToDestroy);
		}

		// TODO: Add creation of entities

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selectedEntity = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if(m_selectedEntity)
		{
			drawEntityComponents(m_selectedEntity);
		}

		ImGui::End();
	}
	void ScenePanel::setSelectedEntity(Entity entity)
	{
		m_selectedEntity = entity;
	}

	EntityId ScenePanel::drawEntityNode(Entity entity)
	{
		ImGuiTreeNodeFlags flags = ((m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(u64)(u32)entity.getId(), flags, entity.getName().c_str());
		if(ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if(opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, entity.getName().c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if(entityDeleted)
		{
			if (m_selectedEntity == entity)
				m_selectedEntity = {};

			return entity.getId();
			//m_context->destroyEntity(entity); // TODO: delete all references
		}

		return 0;
	}

	// TODO: it must pass a pointer to the real entity, not a copy (selectedContext is a copy)
	void ScenePanel::drawEntityComponents(Entity entity)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, entity.getName().c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			m_context->findEntityById(entity.getId())->setName(std::string(buffer));
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if(ImGui::MenuItem("Mesh Render"))
			{
				if(!m_selectedEntity.has(RenderComponent::s_name))
				{
					// TODO: An entity should add its own components
					StrongEntityPtr pEntity = m_context->findEntityById(entity.getId());
					m_context->addComponent(pEntity, RenderComponent::s_name);
				}
				else
				{
					SGSWARN("This entity already has a render component.");
				}

				ImGui::CloseCurrentPopup();
			}

			if(ImGui::MenuItem("Light"))
			{
				if(!m_selectedEntity.has(LightComponent::s_name))
				{
					StrongEntityPtr pEntity = m_context->findEntityById(entity.getId());
					m_context->addComponent(pEntity, LightComponent::s_name);
				}
				else
				{
					SGSWARN("This entity already has a light component.");
				}

				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
	}
}