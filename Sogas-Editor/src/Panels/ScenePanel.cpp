#include "ScenePanel.h"

#include "scene/components/renderComponent.h"
#include "scene/components/cameraComponent.h"
#include "scene/components/transformComponent.h"
#include "scene/components/lightComponent.h"
#include "scene/components/entityScriptComponent.h"

#include "core/utils.h"
#include "platform/utils/platformUtils.h"
#include "renderer/shader.h"
#include "renderer/resources/texture.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>
#include <filesystem>

namespace Sogas
{
	extern const std::filesystem::path g_assetsDirectory;

	ScenePanel::ScenePanel(const std::shared_ptr<Scene>& scene)
	{
		setContext(scene);
	}

	/*
	* @brief Pass a scene to be stored as the context of the panel.
	* @param scene A const shared_ptr with the scene to be stored.
	* @return void.
	*/
	void ScenePanel::setContext(const std::shared_ptr<Scene>& scene)
	{
		m_context = scene;
		m_selectedEntity = {};
	}

	void ScenePanel::onImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		//ImGui::ShowDemoWindow();

		EntityId entityToDestroy = 0;
		for (auto &entity : m_context.get()->getRootEntities())
		{
			EntityId id = drawEntityNode(entity);
			
			if(id != 0)
				entityToDestroy = id;

			if (ImGui::BeginDragDropSource()) {
				EntityId entityDraggedID = entity->getId();
				ImGui::SetDragDropPayload("ENTITY", &entityDraggedID, sizeof(EntityId));
				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget()) {

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
				{
					SGSASSERT(payload->DataSize == sizeof(EntityId));
					EntityId childID = *(const EntityId*)payload->Data;
					entity->addChild(m_context->findEntityById(childID));
					m_context->removeEntity(childID);
				}
			}
		}

		if(entityToDestroy != 0)
		{
			m_context->destroyEntity(entityToDestroy);
		}

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

		ImGui::Begin("Environment");
		ImGui::Checkbox("Show Environment", &m_context->m_renderEnvironment);

		ImGui::End();
	}

	/*
	* @brief Set selected entity with the entity passed.
	* @param entity A shared_ptr<Entity> with the entity to be selected.
	* @return void.
	*/
	void ScenePanel::setSelectedEntity(StrongEntityPtr entity)
	{
		m_selectedEntity = entity;
	}

	EntityId ScenePanel::drawEntityNode(StrongEntityPtr entity)
	{
		ImGuiTreeNodeFlags flags = ((m_selectedEntity.lock() == entity) ? ImGuiTreeNodeFlags_Selected : 0);
		flags |= entity->hasChild() ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(u64)(u32)entity->getId(), flags, entity->getName().c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			if (ImGui::MenuItem("Create Child Entity")) {
				m_context->createEntity("Child Empty Entity", entity->getId());
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			for (const auto& child : entity->getChildList()) {
				i32 entityID = drawEntityNode(child);
				if (entityID != 0) {
					m_context->destroyEntity(entityID);
				}
			}
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			if (m_selectedEntity.lock() == entity)
				m_selectedEntity = {};

			return entity->getId();
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
				uiFuntion(component, entity->getScene()->m_sceneGraph);
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
					m_context->addComponent<RenderComponent>(pEntity);
				}
				else
				{
					SGSWARN("This entity already has a render component.");
				}

				ImGui::CloseCurrentPopup();
			}
			
			if(ImGui::MenuItem("Script"))
			{
				m_context->addComponent<EntityScriptComponent>(pEntity);
			}
			
			if(ImGui::MenuItem("Light"))
			{
				if(!m_selectedEntity.lock()->has<LightComponent>())
				{
					m_context->addComponent<LightComponent>(pEntity);
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

		drawComponent<TransformComponent>("Transform", entity.lock(), [](auto& component, std::shared_ptr<SceneGraph>sceneGraph = nullptr)
			{
				DrawVec3Control("Translation", component.lock()->getTranslation());
				glm::vec3 rotation = glm::degrees(component.lock()->getRotation());
				DrawVec3Control("Rotation", rotation);
				rotation = glm::radians(rotation);
				component.lock()->setRotation(rotation);
				DrawVec3Control("Scale", component.lock()->getScale(), 1.0f);
			});

		drawComponent<CameraComponent>("Camera", entity.lock(), [](auto& component, std::shared_ptr<SceneGraph> sceneGraph = nullptr)
			{
				ImGui::Text(CameraComponent::s_name);
			});

		drawComponent<LightComponent>("Light", entity.lock(), [](auto& component, std::shared_ptr<SceneGraph> sceneGraph = nullptr)
			{
				ImGui::ColorEdit3("Colour", &component.lock()->getColor().x);
				ImGui::SliderFloat("Intensity", &component.lock()->getIntensity(), 0.0f, 100.0f);
				ImGui::SliderFloat("Maximum Distance", &component.lock()->getMaxDistance(), 0.0f, 500.0f);
			});

		
		drawComponent<EntityScriptComponent>("Script", entity.lock(), [](auto& component, std::shared_ptr<SceneGraph> sceneGraph = nullptr)
			{
				ImGui::Text("Using an entity script");
				
				ScriptVariablesMap scriptVariables = component.lock()->getEntityScriptVariables();

				for (auto it = scriptVariables.begin(); it != scriptVariables.end(); it++)
				{
					if (it->second.type == VariableType::INTEGER)
					{
						i32 value = it->second.getInteger();
						std::string text = std::string(it->first.c_str()).append(": ").append(std::to_string(value));
						ImGui::Text(text.c_str());
					}
					else if (it->second.type == VariableType::FLOAT)
					{
						f32 value = it->second.getFloat();
						std::string text = std::string(it->first.c_str()).append(": ").append(std::to_string(value));
						ImGui::Text(text.c_str());
					}
					else if (it->second.type == VariableType::STRING)
					{
						std::string value = it->second.getString();
						std::string text = std::string(it->first.c_str()).append(": ").append(value);
						ImGui::Text(text.c_str());
					}
					else if (it->second.type == VariableType::BOOLEAN)
					{
						bool value = it->second.getBoolean();
						std::string text = std::string(it->first.c_str()).append(": ").append(std::to_string(value));
						ImGui::Text(text.c_str());
					}
				}
			});

		drawComponent<RenderComponent>("Renderer", entity.lock(), [](auto& component, std::shared_ptr<SceneGraph> sceneGraph = nullptr)
			{
				ImGui::Columns(3);
				ImGui::SetColumnWidth(0, 75.0f);
				ImGui::SetColumnWidth(1, 150.0f);
				ImGui::Text("Mesh");
				ImGui::NextColumn();
				ImGui::Text(component.lock()->getMesh() ? component.lock()->getMesh()->getMeshName().c_str() : "Null");
				ImGui::NextColumn();
				ImGui::PushID("MeshButton");
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS_PANEL_MESH"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path meshPath = std::filesystem::path(g_assetsDirectory) / path;
						std::string meshName = takeNameFromPath(meshPath.string());
						component.lock()->setMesh(meshName.c_str());
						sceneGraph->updateNode(component.lock()->getOwner().lock()->getId(), SceneNodeType::GEOMETRY, (void*)component.lock()->getMesh().get());
						sceneGraph->updateNode(component.lock()->getOwner().lock()->getId(), SceneNodeType::MATERIAL, (void*)component.lock()->getMaterial().get());
					}
					ImGui::EndDragDropTarget();
				}

				if (ImGui::Button("..."))
				{
					std::string meshPath = FileDialog::openFile("Meshes (*.obj)\0*.obj\0");
					std::string meshName = takeNameFromPath(meshPath);
					if (!meshName.empty()) {
						component.lock()->setMesh(meshName.c_str());
						sceneGraph->updateNode(component.lock()->getOwner().lock()->getId(), SceneNodeType::GEOMETRY, (void*)component.lock()->getMesh().get());
						sceneGraph->updateNode(component.lock()->getOwner().lock()->getId(), SceneNodeType::MATERIAL, (void*)component.lock()->getMaterial().get());
					}
				}
				ImGui::EndColumns();
				ImGui::PopID();

				if (component.lock()->getMesh())
				{
					static u32 selectedPrimitive = 4;
					const char* names[] = { "Points", "Lines", "Lines Strip", "Lines Fan", "Triangles", "Triangles Strip", "Triangles Fan" };

					if (ImGui::Button("Primitive"))
					{
						ImGui::OpenPopup("select_primitive");
					}
					ImGui::SameLine();
					ImGui::Text(names[selectedPrimitive]);

					if (ImGui::BeginPopup("select_primitive"))
					{
						for (u32 i = 0; i < IM_ARRAYSIZE(names); i++)
						{
							if (ImGui::Selectable(names[i])) {
								selectedPrimitive = i;
								component.lock()->setPrimitive((Primitive)selectedPrimitive);
							}
						}
						ImGui::EndPopup();
					}

					if (ImGui::CollapsingHeader("Material"))
					{
						ImGui::Columns(3);
						ImGui::Text("Shader");
						ImGui::NextColumn();
						ImGui::Text(component.lock()->getShader() ? component.lock()->getShader()->getName().c_str() : "Null");
						ImGui::NextColumn();
						
						ImGui::PushID("ShaderButton");
						if (ImGui::Button("..."))
						{
							std::string shaderName = FileDialog::openFile("Shader (*.shader)\0*.shader\0");
							if (!shaderName.empty())
								component.lock()->getMaterial()->setMaterialShader(Shader::GET(takeNameFromPath(shaderName)));
						}
						ImGui::PopID();

						ImGui::EndColumns();

						auto& materialProperties = component.lock()->getMaterial()->getMaterialProperties();
						ImGui::ColorEdit4("", &materialProperties.color.x);
						ImGui::SliderFloat("Metallic", &materialProperties.metallicFactor, 0.0f, 1.0f);
						ImGui::SliderFloat("Roughness", &materialProperties.roughnessFactor, 0.0f, 1.0f);
						ImGui::SliderFloat("Tilling", &materialProperties.tillingFactor, 0.0f, 100.0f);

						DrawVec3Control("Emission", materialProperties.emissiveFactor);

						ImGui::Dummy(ImVec2(0.0f, 10.0f));

						const char* colorTextureName = materialProperties.colorTexture ? materialProperties.colorTexture->getName().c_str() : "None";
						const char* normalTextureName = materialProperties.normalTexture ? materialProperties.normalTexture->getName().c_str() : "None";
						const char* emissiveTextureName = materialProperties.emissiveTexture ? materialProperties.emissiveTexture->getName().c_str() : "None";
						const char* metallicRoughnessTextureName = materialProperties.metallicRoughnessTexture ?
							materialProperties.metallicRoughnessTexture->getName().c_str() : "None";
						
						static f32 itemRightPadding = 16.0f;
						static f32 thumbnailSize = 128.0f;
						f32 cellSize = thumbnailSize + itemRightPadding;

						f32 panelWidth = ImGui::GetContentRegionAvail().x;
						i32 columnCount = (i32)(panelWidth / cellSize);
						if (columnCount < 1)
							columnCount = 1;

						ImGui::Columns(columnCount, 0, false);

						// COLOR TEXTURE
						static std::string tName;

						if (ImGui::BeginPopup("select_texture"))
						{
							if (ImGui::Selectable("Default white texture"))
							{
								tName = "Default white";
							}
							if (ImGui::Selectable("Load Textures"))
							{
								tName = FileDialog::openFile("Texture Files (*.png; *.jpg)\0*.png;*.jpg\0");
							}
							ImGui::EndPopup();
						}

						std::string textureName;
						if (materialProperties.colorTexture)
						{
							if(ImGui::ImageButton((ImTextureID)materialProperties.colorTexture->getID(), { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
								if (!tName.empty()) {
									materialProperties.colorTexture = Texture2D::GET(takeNameFromPath(tName));
									tName.clear();
								}
							}
						}
						else
						{
							if(ImGui::Button("Color Texture", { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
								if (!tName.empty()) {
									materialProperties.colorTexture = Texture2D::GET(takeNameFromPath(tName));
									tName.clear();
								}
							}
						}

						//if (ImGui::BeginPopup("select_color_texture"))
						//{
						//	if (ImGui::Selectable("Default white texture"))
						//	{
						//		materialProperties.colorTexture = Texture2D::GET("Default white");
						//	}

						//	if (ImGui::Selectable("Load textures"))
						//	{
						//		std::string textureName = FileDialog::openFile("Texture Files (*.png; *.jpg)\0*.png;*.jpg\0");
						//		if (!textureName.empty())
						//			materialProperties.colorTexture = Texture2D::GET(takeNameFromPath(textureName));
						//	}
						//	ImGui::EndPopup();
						//}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS_PANEL_TEXTURE"))
							{
								const wchar_t* path = (const wchar_t*)payload->Data;
								std::filesystem::path texturePath = std::filesystem::path(g_assetsDirectory) / path;
								materialProperties.colorTexture = Texture2D::GET(texturePath.string());
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::Text("Color Texture");


						// EMISSIVE TEXTURE
						
						ImGui::NextColumn();

						if (materialProperties.emissiveTexture)
						{
							if(ImGui::ImageButton((ImTextureID)materialProperties.emissiveTexture->getID(), { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
							}
						}
						else
						{
							if(ImGui::Button("Emissive Texture", { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
							}
						}

						//if (ImGui::BeginPopup("select_emissive_texture"))
						//{
						//	if (ImGui::Selectable("Default white texture"))
						//	{
						//		materialProperties.emissiveTexture = Texture2D::GET("Default white");
						//	}

						//	if (ImGui::Selectable("Load textures"))
						//	{
						//		std::string textureName = FileDialog::openFile("Texture (*.png)\0*.png\0Texture (*.jpg)\0*.jpg\0");
						//		if (!textureName.empty())
						//			materialProperties.emissiveTexture = Texture2D::GET(textureName);
						//	}
						//	ImGui::EndPopup();
						//}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS_PANEL_TEXTURE"))
							{
								const wchar_t* path = (const wchar_t*)payload->Data;
								std::filesystem::path texturePath = std::filesystem::path(g_assetsDirectory) / path;
								materialProperties.emissiveTexture = Texture2D::GET(texturePath.string());
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::Text("Emissive Texture");
						
						
						// METAL-ROUGHNESS TEXTURE

						ImGui::NextColumn();

						if (materialProperties.metallicRoughnessTexture)
						{
							if(ImGui::ImageButton((ImTextureID)materialProperties.metallicRoughnessTexture->getID(), { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
							}
						}
						else
						{
							if(ImGui::Button("Metal-Roughness Texture", { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
							}
						}

						//if (ImGui::BeginPopup("select_metal_roughness_texture"))
						//{
						//	if (ImGui::Selectable("Default white texture"))
						//	{
						//		materialProperties.metallicRoughnessTexture = Texture2D::GET("Default white");
						//	}

						//	if (ImGui::Selectable("Load textures"))
						//	{
						//		// TODO load the texture name to path, reduce it to only the name and pass it to GET (It will use the utils::findFile to find the path to the texture)
						//		std::string texturePath = FileDialog::openFile("Texture (*.png)\0*.png\0Texture (*.jpg)\0*.jpg\0");
						//		std::string textureName = takeNameFromPath(texturePath);

						//		if (!textureName.empty())
						//			materialProperties.metallicRoughnessTexture = Texture2D::GET(textureName);
						//	}
						//	ImGui::EndPopup();
						//}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS_PANEL_TEXTURE"))
							{
								const wchar_t* path = (const wchar_t*)payload->Data;
								std::filesystem::path texturePath = std::filesystem::path(g_assetsDirectory) / path;
								materialProperties.metallicRoughnessTexture = Texture2D::GET(texturePath.string());
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::Text("Metal-Roughness Texture");


						// OCCLUSION TEXTURE

						ImGui::NextColumn();

						if (materialProperties.occlusionTexture)
						{
							if(ImGui::ImageButton((ImTextureID)materialProperties.occlusionTexture->getID(), { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
							}
						}
						else
						{
							if(ImGui::Button("Occlusion Texture", { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
							}
						}

						//if (ImGui::BeginPopup("select_occlusion_texture"))
						//{
						//	if (ImGui::Selectable("Default white texture"))
						//	{
						//		materialProperties.occlusionTexture = Texture2D::GET("Default white");
						//	}

						//	if (ImGui::Selectable("Load textures"))
						//	{
						//		std::string textureName = FileDialog::openFile("Texture (*.png)\0*.png\0Texture (*.jpg)\0*.jpg\0");
						//		if (!textureName.empty())
						//			materialProperties.occlusionTexture = Texture2D::GET(textureName);
						//	}
						//	ImGui::EndPopup();
						//}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS_PANEL_TEXTURE"))
							{
								const wchar_t* path = (const wchar_t*)payload->Data;
								std::filesystem::path texturePath = std::filesystem::path(g_assetsDirectory) / path;
								materialProperties.occlusionTexture = Texture2D::GET(texturePath.string());
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::Text("Occlusion Texture");


						// NORMAL TEXTURE

						ImGui::NextColumn();

						if (materialProperties.normalTexture)
						{
							if(ImGui::ImageButton((ImTextureID)materialProperties.normalTexture->getID(), { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
							}
						}
						else
						{
							if(ImGui::Button("Normal Texture", { 128, 128 }))
							{
								ImGui::OpenPopup("select_texture");
							}
						}

						//if (ImGui::BeginPopup("select_normal_texture"))
						//{
						//	if (ImGui::Selectable("Default white texture"))
						//	{
						//		materialProperties.normalTexture = Texture2D::GET("Default white");
						//	}

						//	if (ImGui::Selectable("Load textures"))
						//	{
						//		std::string textureName = FileDialog::openFile("Texture (*.png)\0*.png\0Texture (*.jpg)\0*.jpg\0");
						//		if (!textureName.empty())
						//			materialProperties.normalTexture = Texture2D::GET(textureName);
						//	}
						//	ImGui::EndPopup();
						//}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS_PANEL_TEXTURE"))
							{
								const wchar_t* path = (const wchar_t*)payload->Data;
								std::filesystem::path texturePath = std::filesystem::path(g_assetsDirectory) / path;
								materialProperties.normalTexture = Texture2D::GET(texturePath.string());
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::Text("Normal Texture");
						

						ImGui::NextColumn();

						ImGui::Columns(1);
					}
				}
			});
	}
}