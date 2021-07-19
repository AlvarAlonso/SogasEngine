#include "ScenePanel.h"

#include "scene/components/renderComponent.h"
#include "scene/components/cameraComponent.h"
#include "scene/components/transformComponent.h"
#include "scene/components/lightComponent.h"

#include <imgui.h>
#include <imgui_internal.h>

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
			EntityId id = drawEntityNode(entity);
			
			if(id != 0)
				entityToDestroy = id;
		}

		if(entityToDestroy != 0)
		{
			m_context->destroyEntity(entityToDestroy);
		}

		// TODO: If a mesh renderer is added to the empty entity, because mesh is nullptr the program breaks
		if(ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_context->createEntity("Empty Entity");

			ImGui::EndPopup();
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			SGSINFO("Mouse is down");
			m_selectedEntity = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if(m_selectedEntity.lock() != nullptr)
		{
			drawEntityComponents(m_selectedEntity);
		}

		ImGui::End();
	}

	void ScenePanel::setSelectedEntity(StrongEntityPtr entity)
	{
		m_selectedEntity = entity;
	}

	EntityId ScenePanel::drawEntityNode(StrongEntityPtr entity)
	{
		ImGuiTreeNodeFlags flags = ((m_selectedEntity.lock() == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(u64)(u32)entity->getId(), flags, entity->getName().c_str());
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
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, entity->getName().c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if(entityDeleted)
		{
			if (m_selectedEntity.lock() == entity)
				m_selectedEntity = {};

			return entity->getId();
			//m_context->destroyEntity(entity); // TODO: delete all references
		}

		return 0;
	}

	template<typename T, typename UIFunction>
	void ScenePanel::drawComponent(const std::string& name, StrongEntityPtr entity, UIFunction uiFuntion)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | 
			ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		
		if(entity->has<T>())
		{
			auto component = entity->getComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x);
			if (ImGui::Button("+", ImVec2{ 1.0f, 1.0f }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if(ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFuntion(component);
				ImGui::TreePop();
			}

			if(removeComponent)
			{
				entity->removeComponent(T::s_name);
			}
		}
	}
	
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth(); // TODO: It breaks here

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	
	void ScenePanel::drawEntityComponents(std::weak_ptr<Entity> entity)
	{
		StrongEntityPtr pEntity = entity.lock();

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, pEntity->getName().c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			pEntity->setName(std::string(buffer));
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if(ImGui::MenuItem("Mesh Render"))
			{
				if(!m_selectedEntity.lock()->has<RenderComponent>())
				{
					// TODO: An entity should add its own components
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
				if(!m_selectedEntity.lock()->has<LightComponent>())
				{
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

		drawComponent<TransformComponent>("Transform", entity.lock(), [](auto& component)
			{
				ImGui::Text(TransformComponent::s_name);
				DrawVec3Control("Translation", component.lock()->getTranslation());
				glm::vec3 rotation = glm::degrees(component.lock()->getRotation());
				DrawVec3Control("Rotation", rotation);
				rotation = glm::radians(rotation);
				component.lock()->setRotation(rotation);
				DrawVec3Control("Scale", component.lock()->getScale(), 1.0f);
			});

		drawComponent<CameraComponent>("Camera", entity.lock(), [](auto& component)
			{
				ImGui::Text(CameraComponent::s_name);
			});

		drawComponent<LightComponent>("Light", entity.lock(), [](auto& component)
			{
				ImGui::Text(LightComponent::s_name);
			});

		drawComponent<RenderComponent>("Renderer", entity.lock(), [](auto& component)
			{
				ImGui::Text(RenderComponent::s_name);
			});
	}
}