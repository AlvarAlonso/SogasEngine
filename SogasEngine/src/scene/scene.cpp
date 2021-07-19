
#include "sgspch.h"
#include "scene.h"

#include "renderer/shader.h"
#include "components/transformComponent.h"
#include "components/renderComponent.h"
#include "components/lightComponent.h"
#include "renderer/resources/texture.h"

#include "glm/gtc/quaternion.hpp"

namespace Sogas
{
	Scene::Scene()
	{
		m_pEntityFactory.reset();
		m_pEntityFactory = std::make_unique<EntityFactory>();
	}

	Scene::Scene(const char* filename)
	{

		std::ifstream file(filename);
		json j;
		file >> j;

		m_pEntityFactory.reset();
		m_pEntityFactory = std::make_unique<EntityFactory>();

		from_json(j);
	}

	Scene::~Scene()
	{
		
	}

	/*
	* TODO: Should probably receive the path to the json to deserialize, name should not be
	* added at creation. 
	* @brief Create automatically an entity to the scene
	* @param const std::string& name
	* @return StrongEntityPtr
	*/
	StrongEntityPtr Scene::createEntity(const std::string& name)
	{
		StrongEntityPtr entity = m_pEntityFactory->createEntity(name.c_str());
		m_entities.push_back(entity);
		return entity;
	}

	void Scene::destroyEntity(EntityId entityId)
	{
		// TODO: Optimize insertion/deletion of entities

		i32 index = 0;
		for(auto& currentEntity : m_entities)
		{
			if (currentEntity->getId() == entityId)
			{
				currentEntity->destroy();
				currentEntity = nullptr;
				m_entities.erase(m_entities.begin() + index);
				break; // break loop after finding the entity to be deleted
			}

			index++;
		}

		// TODO: some way to destroy an entity deleting all the references to it automatically
	}

	// TODO: scene onUpdate should update and submit commands to the renderer, thus the renderer needs to have access to the shader
	// and textures to bind them when updating/rendering
	void Scene::onUpdate(f32 dt)
	{

		//for (auto& entity : m_entities)
		//{
		//	auto tComponent = entity->getComponent<TransformComponent>(TransformComponent::s_name);
		//}

	}

	/*
	* @brief Search for the entity with the correspondant id and returns it
	* @param EntityId
	* @return StrongEntityPtr (shared_ptr<Entity>)
	*/

	StrongEntityPtr Scene::findEntityById(EntityId entityId)
	{
		for (i32 index = 0; index < m_entities.size(); index++)
		{
			if(m_entities[index]->getId() == entityId)
			{
				return m_entities[index];
			}
		}

		return nullptr;
	}

	/*
	* @brief Serialize all data in the scene to json
	* @param json&
	* @return void
	*/
	void Scene::to_json(json& j)
	{
		json entArray = json::array();
		for (auto& entity : getEntities())
		{
			json ent;
			entity->to_json(ent);
			entArray.push_back(ent);
		}
		j = json{ {"Entities", entArray} };
	}

	/*
	* @brief Deserialize all data from a json file to a scene
	* @param const json&
	* @return void
	*/
	void Scene::from_json(const json& j)
	{
		if (j.contains("Scene"))
		{
			auto scene = j["Scene"];

			if (scene.contains("Entities"))
			{
				auto jsonEntities = scene["Entities"];
				for (auto& jsonEntity : jsonEntities)
				{
					std::string name = jsonEntity["Name"].get<std::string>();

					auto entity = m_pEntityFactory->createEntity(name.c_str());

					// -----------------------
					// Check if entity contains Transform Component
					// -----------------------
					if (jsonEntity.contains(TransformComponent::s_name) && !jsonEntity[TransformComponent::s_name].is_null())
					{
						// TODO: add transform
						auto jsonTransform = jsonEntity[TransformComponent::s_name];
						auto transformComponent = makeStrongPtr<TransformComponent>(entity->getComponent<TransformComponent>());
						glm::mat4 model = glm::mat4(1);

						if (jsonTransform.contains("Translation") && !jsonTransform["Translation"].is_null())
						{
							auto jsonPosition = jsonTransform["Translation"];

							f32 x = jsonPosition.contains("x") ? jsonPosition["x"].get<f32>() : 0.0f;
							f32 y = jsonPosition.contains("y") ? jsonPosition["y"].get<f32>() : 0.0f;
							f32 z = jsonPosition.contains("z") ? jsonPosition["z"].get<f32>() : 0.0f;

							glm::vec3 position = glm::vec3(x, y, z);
							transformComponent->setTranslation(position);
						}
						if (jsonTransform.contains("Rotation") && !jsonTransform["Rotation"].is_null())
						{
							auto jsonRotation = jsonTransform["Rotation"];

							f32 x = jsonRotation.contains("x") ? jsonRotation["x"].get<f32>() : 0.0f;
							f32 y = jsonRotation.contains("y") ? jsonRotation["y"].get<f32>() : 0.0f;
							f32 z = jsonRotation.contains("z") ? jsonRotation["z"].get<f32>() : 0.0f;
							//f32 w = jsonRotation.contains("w") ? jsonRotation["w"].get<f32>() : 0.0f;

							// TODO: Add support for quaternions

							//glm::vec3 rotation = glm::eulerAngles(glm::quat(x, y, z, w));
							glm::vec3 rotation = glm::vec3(x, y, z);
							transformComponent->setRotation(rotation);
						}
						if (jsonTransform.contains("Scale") && !jsonTransform["Scale"].is_null())
						{
							auto jsonScale = jsonTransform["Scale"];

							f32 x = jsonScale.contains("x") ? jsonScale["x"].get<f32>() : 0.0f;
							f32 y = jsonScale.contains("y") ? jsonScale["y"].get<f32>() : 0.0f;
							f32 z = jsonScale.contains("z") ? jsonScale["z"].get<f32>() : 0.0f;

							glm::vec3 scale = glm::vec3(x, y, z);
							transformComponent->setScale(scale);
						}
					}

					// -----------------------
					// Check if entity contains Render Component
					// -----------------------
					if (jsonEntity.contains(RenderComponent::s_name) && !jsonEntity[RenderComponent::s_name].is_null())
					{
						auto jsonComponent = jsonEntity[RenderComponent::s_name];
						entity->addComponent(m_pEntityFactory->createComponent(RenderComponent::s_name));

						std::shared_ptr<RenderComponent> renderComponent = makeStrongPtr(entity->getComponent<RenderComponent>());

						// -----------------------
						// Check for the mesh data - it should contain a Mesh, otherwise the render component loses its purpose
						// TODO: Maybe should include an error handling for when loading a render component without a mesh or add a default one
						// such as a cube
						// -----------------------
						if (jsonComponent.contains("Mesh") && !jsonComponent["Mesh"].is_null())
						{
							renderComponent->setMesh(jsonComponent["Mesh"].get<std::string>().c_str());
						}
						else {
							SGSWARN("Loading a render component with no valid Mesh!");
						}

						// -----------------------
						// Check for the material data
						// For the material data we create a default material if none is given. The default material should load the basic shader too
						// -----------------------
						if (jsonComponent.contains("Material"))
						{
							// Create the standard material, this will be inflated with the data parsed if any
							std::shared_ptr<Material> material = std::make_shared<Material>();

							if (!jsonComponent["Material"].is_null())
							{
								auto jsonMaterial = jsonComponent["Material"];

								// Check if a valid shader is given
								if (jsonMaterial.contains("Shader") &&
									!jsonMaterial["Shader"].is_null() &&
									jsonMaterial["Shader"].is_string())
								{
									material->setMaterialShader(Shader::create(jsonMaterial["Shader"].get<std::string>()));
								}
								else
								{
									material->setMaterialShader(Shader::create("../SogasEngine/shaders/basic.shader"));
								}

								// Get the material properties
								if (jsonMaterial.contains("ColorTexture") && jsonMaterial["ColorTexture"].is_string())
								{
									auto& properties = material->getMaterialProperties();
									properties.colorTexture = Texture2D::create(jsonMaterial["ColorTexture"].get<std::string>());
									material->setMaterialProperties(properties);
								}
							}
							else {
								material->setMaterialShader(Shader::create("../SogasEngine/shaders/basic.shader"));
								auto& properties = material->getMaterialProperties();
								properties.colorTexture = Sogas::Texture2D::create(1, 1);
								u32 defaultTextureData = 0xffffffff;
								properties.colorTexture->setData(&defaultTextureData, sizeof(u32));
								material->setMaterialProperties(properties);
							}
							renderComponent->setMaterial(material);
						}
						else
						{
							SGSWARN("Render component with no Material defined!");
						}

					}

					// -----------------------
					// Check if entity contains Light Component
					// -----------------------
					if (jsonEntity.contains(LightComponent::s_name) && !jsonEntity[LightComponent::s_name].is_null())
					{
						auto jsonLight = jsonEntity["LightComponent"];
						entity->addComponent(m_pEntityFactory->createComponent(LightComponent::s_name));

						std::shared_ptr<LightComponent> lightComponent = makeStrongPtr(entity->getComponent<LightComponent>());

						if (jsonLight.contains("Color") && !jsonLight["Color"].is_null())
						{
							auto jsonColor = jsonLight["Color"];

							f32 r = jsonColor.contains("r") ? jsonColor["r"].get<f32>() : 0.0f;
							f32 g = jsonColor.contains("g") ? jsonColor["g"].get<f32>() : 0.0f;
							f32 b = jsonColor.contains("b") ? jsonColor["b"].get<f32>() : 0.0f;

							lightComponent->setColor(glm::vec3(r, g, b));
						}
					}

					m_entities.push_back(entity);
				}
			}
		}
	}
}