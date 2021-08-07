#include "ScenePanel.h"

#include "scene/components/renderComponent.h"
#include "scene/components/cameraComponent.h"
#include "scene/components/transformComponent.h"
#include "scene/components/lightComponent.h"

#include "platform/utils/platformUtils.h"
#include "renderer/shader.h"
#include "renderer/resources/texture.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>

namespace Sogas
{
	extern const std::filesystem::path g_assetsDirectory;

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

		ImGui::ShowDemoWindow();

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
			{
				//TODO List the components and nodes of every entity and allow their destruction if wanted
				ImGui::TreePop();
			}
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
					m_context->addComponent<RenderComponent>(pEntity);
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

		drawComponent<TransformComponent>("Transform", entity.lock(), [](auto& component)
			{
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
				ImGui::ColorEdit3("Colour", &component.lock()->getColor().x);
				ImGui::SliderFloat("Intensity", &component.lock()->getIntensity(), 0.0f, 100.0f);
				ImGui::SliderFloat("Maximum Distance", &component.lock()->getMaxDistance(), 0.0f, 500.0f);
			});

		drawComponent<RenderComponent>("Renderer", entity.lock(), [](auto& component)
			{
				ImGui::Columns(3);
				ImGui::SetColumnWidth(0, 50.0f);
				ImGui::SetColumnWidth(1, 250.0f);
				ImGui::Text("Mesh");
				ImGui::NextColumn();
				ImGui::Text(component.lock()->getMesh() ? component.lock()->getMesh()->getMeshName().c_str() : "Null");
				ImGui::NextColumn();
				if (ImGui::Button("..."))
				{
					std::string meshName = FileDialog::openFile("Meshes (*.obj)\0*.obj\0");
					if (!meshName.empty())
						component.lock()->setMesh(meshName.c_str());
				}
				ImGui::EndColumns();

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
						
						if (ImGui::Button("...!"))
						{
							std::string shaderName = FileDialog::openFile("Shader (*.shader)\0*.shader\0");
							if (!shaderName.empty())
								component.lock()->getMaterial()->setMaterialShader(Shader::GET(shaderName));
						}

						ImGui::EndColumns();

						auto& materialProperties = component.lock()->getMaterial()->getMaterialProperties();
						ImGui::ColorEdit4("", &materialProperties.color.x);
						if(materialProperties.colorTexture)
						{
							ImGui::ImageButton((ImTextureID)materialProperties.colorTexture->getID(), { 128, 128 });
						}
						else
						{
							ImGui::Button("Color Texture", { 128, 128 });
						}

						if(ImGui::BeginDragDropTarget())
						{
							if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS_PANEL_ITEM"))
							{
								const wchar_t* path = (const wchar_t*)payload->Data;
								std::filesystem::path texturePath = std::filesystem::path(g_assetsDirectory) / path;
								materialProperties.colorTexture = Texture2D::create(texturePath.string());
							}
							ImGui::EndDragDropTarget();
						}

						ImGui::SliderFloat("Metallic", &materialProperties.metallicFactor, 0.0f, 1.0f);
						ImGui::SliderFloat("Roughness", &materialProperties.roughnessFactor, 0.0f, 1.0f);
						ImGui::SliderFloat("Tilling", &materialProperties.tillingFactor, 0.0f, 100.0f);

						DrawVec3Control("Emission", materialProperties.emissiveFactor);

						const char* colorTextureName = materialProperties.colorTexture ? materialProperties.colorTexture->getName().c_str() : "None";
						const char* normalTextureName = materialProperties.normalTexture ? materialProperties.normalTexture->getName().c_str() : "None";
						const char* emissiveTextureName = materialProperties.emissiveTexture ? materialProperties.emissiveTexture->getName().c_str() : "None";
						const char* metallicRoughnessTextureName = materialProperties.metallicRoughnessTexture ?
							materialProperties.metallicRoughnessTexture->getName().c_str() : "None";

						ImGui::Columns(2);
						ImGui::Text("Colour Texture");
						ImGui::NextColumn();

						if (ImGui::Button(colorTextureName))
						{
							ImGui::OpenPopup("select_texture");
						}
						ImGui::EndColumns();

						if (ImGui::BeginPopup("select_texture"))
						{
							if (ImGui::Selectable("Default white texture"))
							{
								materialProperties.colorTexture = Texture2D::GET("Default white");
							}

							if (ImGui::Selectable("Load textures"))
							{
								std::string textureName = FileDialog::openFile("Texture (*.png)\0*.png\0Texture (*.jpg)\0*.jpg\0");
								if (!textureName.empty())
									materialProperties.colorTexture = Texture2D::GET(textureName);
							}
							ImGui::EndPopup();
						}


						ImGui::Columns(2);
						ImGui::Text("Normal Texture");
						ImGui::NextColumn();

						if (ImGui::Button(normalTextureName))
						{
							std::string textureName = FileDialog::openFile("Texture (*.png)\0*.png\0");
							if (!textureName.empty())
								materialProperties.normalTexture = Texture2D::GET(textureName);
						}
						ImGui::EndColumns();

						ImGui::Columns(2);
						ImGui::Text("Emissive Texture");
						ImGui::NextColumn();

						if (ImGui::Button(emissiveTextureName))
						{
							std::string textureName = FileDialog::openFile("Texture (*.png)\0*.png\0");
							if(!textureName.empty())
								materialProperties.emissiveTexture = Texture2D::GET(textureName);
						}
						ImGui::EndColumns();

						ImGui::Columns(2);
						ImGui::Text("MetallicRoughness Texture");
						ImGui::NextColumn();

						if (ImGui::Button(metallicRoughnessTextureName))
						{
							std::string textureName = FileDialog::openFile("Texture (*.png)\0*.png\0");
							if (!textureName.empty())
								materialProperties.metallicRoughnessTexture = Texture2D::GET(textureName);
						}
						ImGui::EndColumns();
					}
				}
			});
	}
}