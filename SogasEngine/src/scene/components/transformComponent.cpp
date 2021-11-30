
#include "sgspch.h"
#include "transformComponent.h"
#include "scene/serializerUtils.h"
#include "../entity.h"
#include "scene/scene.h"

namespace Sogas
{
	const char* TransformComponent::s_name = "TransformComponent";

	bool TransformComponent::init()
	{
		m_translation			= glm::vec3();
		m_rotation				= glm::vec3();
		m_scale					= glm::vec3(1);
		m_localMatrix			= glm::mat4(1);
		m_globalMatrix			= glm::mat4(1);
		m_inverseGlobalMatrix	= glm::mat4(1);
		return true;
	}

	/*
	* @brief Given the properties of translation, rotation and scale stored, build and return the
	* global matrix, this is the representation in the world.
	* @param void
	* @return glm::mat4 The global matrix of the component.
	*/
	glm::mat4 TransformComponent::getGlobalTransform()
	{
		if (m_pOwner.lock()->hasParent()) {
			m_globalMatrix = m_pOwner.lock()->getParent()->getComponent<TransformComponent>().lock()->getGlobalTransform() * m_localMatrix;
		}
		else {
			m_globalMatrix = m_localMatrix;
		}
		return m_globalMatrix;
	}

	/*
	* @brief Calculate and return the matrix that represents the position of the entity respective
	* to the parent. 
	* @param void
	* @return glm::mat4 the local matrix.
	*/
	glm::mat4 TransformComponent::getLocalTransform()
	{
		return m_localMatrix;
	}

	/*
	* @brief Fill the translation, rotation and scale properties from a given glm::mat4.
	* @param transform A const glm::mat4& with the transform matrix.
	* @return void. 
	*/
	void TransformComponent::setTransform(const glm::mat4& transform)
	{
		m_localMatrix	= transform;
		m_translation	= glm::vec3(m_localMatrix[3].x, m_localMatrix[3].y, m_localMatrix[3].z);// (glm::vec3)m_localMatrix[3][0];
		m_rotation		= glm::radians(glm::eulerAngles(glm::quat_cast(m_localMatrix)));
		m_scale			= glm::vec3(m_localMatrix[0][0], m_localMatrix[1][1], m_localMatrix[2][2]);
		//updateMatrix();
	}

	void TransformComponent::setTranslation(glm::vec3& translation)
	{
		EntityId id = m_pOwner.lock()->getId();
		std::shared_ptr<Scene> scene = m_pOwner.lock()->getScene();
		m_translation = translation; 
		updateMatrix();

		scene->updateSceneGraphNode(id, SceneNodeType::TRANSFORM, (void*)&translation);
	}

	// ----------------------------------------------
	// 	   Serialization
	// ----------------------------------------------

	/*
	* @brief Converts the class TransformComponent data to json format.
	* @param j The reference to a json value to be filled.
	* @return void
	*/
	void TransformComponent::to_json(json& j)
	{
		json translation, rotation, scale;
		Sogas::to_json(translation, getTranslation());
		Sogas::to_json(rotation, getRotation());
		Sogas::to_json(scale, getScale());

		j["Translation"] = translation;
		j["Rotation"] = rotation;
		j["Scale"] = scale;

	}

	/*
	* @brief Retrieve data from a json value and store it to the TransforComponent class.
	* @param jsonTransform The json value to be read with transform data.
	* @return void
	*/
	void TransformComponent::from_json(const json& jsonTransform)
	{
		if (jsonTransform.contains("Translation") && !jsonTransform["Translation"].is_null())
		{
			auto jsonPosition = jsonTransform["Translation"];

			f32 x = jsonPosition.contains("x") ? jsonPosition["x"].get<f32>() : 0.0f;
			f32 y = jsonPosition.contains("y") ? jsonPosition["y"].get<f32>() : 0.0f;
			f32 z = jsonPosition.contains("z") ? jsonPosition["z"].get<f32>() : 0.0f;

			glm::vec3 position = glm::vec3(x, y, z);
			setTranslation(position);
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
			setRotation(rotation);
		}
		if (jsonTransform.contains("Scale") && !jsonTransform["Scale"].is_null())
		{
			auto jsonScale = jsonTransform["Scale"];

			f32 x = jsonScale.contains("x") ? jsonScale["x"].get<f32>() : 0.0f;
			f32 y = jsonScale.contains("y") ? jsonScale["y"].get<f32>() : 0.0f;
			f32 z = jsonScale.contains("z") ? jsonScale["z"].get<f32>() : 0.0f;

			glm::vec3 scale = glm::vec3(x, y, z);
			setScale(scale);
		}
	}

	// ----------------------------------------------
	// 	   Lua
	// ----------------------------------------------

	LuaPlus::LuaObject TransformComponent::toLuaObject(LuaPlus::LuaObject self) const
	{
		LuaPlus::LuaObject transform = self.GetByName("transform");

		SGSASSERT(transform.IsTable());

		// TODO: return all the information of the transform component, not only the position
		transform.SetNumber("x", m_translation.x);
		transform.SetNumber("y", m_translation.y);
		transform.SetNumber("z", m_translation.z);

		return transform;
	}

	void TransformComponent::fromLuaObject(LuaPlus::LuaObject self)
	{
		LuaPlus::LuaObject transform = self.GetByName("transform");

		f32 translation[3];

		translation[0] = transform.GetByName("x").GetFloat();
		translation[1] = transform.GetByName("y").GetFloat();
		translation[2] = transform.GetByName("z").GetFloat();
		
		m_translation = glm::vec3(translation[0], translation[1], translation[2]);
	}
}
