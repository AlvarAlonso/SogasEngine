#include "sgspch.h"
#include "cameraComponent.h"

#include "scene/scene.h"
#include "glm/glm/gtc/matrix_transform.hpp"

namespace Sogas
{
	const char* CameraComponent::s_name = "CameraComponent";
	bool CameraComponent::init()
	{
		recalculateProjection();
		return true;
	}

	void CameraComponent::postInit()
	{
		std::weak_ptr<Scene> scene = m_pOwner.lock()->getScene();
		setViewportSize(scene.lock()->m_viewportWidth, scene.lock()->m_viewportHeight);
		recalculateProjection();
	}

	/*
	* @brief Set the necessary information to construct a perspective camera.
	* @param f32 fov, f32 near, f32 far
	* @return void
	*/
	void CameraComponent::setPerspective(f32 fov, f32 near, f32 far)
	{
		m_projectionType	= eProjectionType::Perspective;
		m_perspectiveFOV	= fov;
		m_perspectiveNear	= near;
		m_perspectiveFar	= far;
		recalculateProjection();
	}

	/*
	* @brief Set the necessary information to construct an orthographic camera.
	* @param f32 size, f32 near, f32 far
	* @return void
	*/
	void CameraComponent::setOrtographic(f32 size, f32 near, f32 far)
	{
		m_projectionType	= eProjectionType::Orthographic;
		m_orthographicSize	= size;
		m_orthographicNear	= near;
		m_orthographicFar	= far;
		recalculateProjection();
	}

	/*
	* @brief Pass the width and height of the window and calculate the aspect ratio. Call recalculateProjection to
	* calculate the matrix with the new aspect ratio.
	* @param u32 width, u32 height
	* @return void
	*/
	void CameraComponent::setViewportSize(u32 width, u32 height)
	{
		SGSASSERT(width > 0 && height > 0)
		m_aspectRatio = (f32)width / (f32)height;
		recalculateProjection();
	}

	/*
	* @brief Given the data in the camera component, store the resulting matrix in the projection variable.
	* @param void
	* @return void
	*/
	void CameraComponent::recalculateProjection()
	{
		switch (m_projectionType)
		{
			case Sogas::CameraComponent::eProjectionType::Perspective:
			{
				m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
				break;
			}
			case Sogas::CameraComponent::eProjectionType::Orthographic:
			{
				f32 left	= -m_orthographicSize * m_aspectRatio * 0.5f;
				f32 right	=  m_orthographicSize * m_aspectRatio * 0.5f;
				f32 bottom	= -m_orthographicSize * 0.5f;
				f32 top		=  m_orthographicSize * 0.5f;

				m_projection = glm::ortho(left, right, bottom, top, m_orthographicNear, m_orthographicFar);

				break;
			}
		}
	}

	/*
	* @brief Converts the class CameraComponent data to json format.
	* @param json& j The reference variable json to be filled.
	* @return void
	*/
	void CameraComponent::to_json(json& j)
	{
		switch (m_projectionType)
		{
			case Sogas::CameraComponent::eProjectionType::Perspective:
			{
				j["Type"]	= "Perspective";
				j["FOV"]	= m_perspectiveFOV;
				j["Near"]	= m_perspectiveNear;
				j["Far"]	= m_perspectiveFar;
				break;
			}
			case Sogas::CameraComponent::eProjectionType::Orthographic:
			{
				j["Type"]	= "Orthographic";
				j["Size"]	= m_orthographicSize;
				j["Near"]	= m_orthographicNear;
				j["Far"]	= m_orthographicFar;
				break;
			}
		}
	}

	/*
	* @brief Deserialize the CameraComponent from json data to the proper class.
	* @param const json& j Json variable containing the data to be deserialized.
	* @return void
	*/
	void CameraComponent::from_json(const json& jsonComponent)
	{
		if (jsonComponent.contains("Type") && !jsonComponent["Type"].is_null())
		{
			std::string typeString = jsonComponent["Type"].get<std::string>();

			if (typeString == "Perspective")
			{
				m_projectionType = eProjectionType::Perspective;
				
				if (jsonComponent.contains("FOV") && !jsonComponent["FOV"].is_null())
					m_perspectiveFOV = jsonComponent["FOV"];

				if (jsonComponent.contains("Near") && !jsonComponent["Near"].is_null())
					m_perspectiveNear = jsonComponent["Near"];

				if (jsonComponent.contains("Far") && !jsonComponent["Far"].is_null())
					m_perspectiveFar = jsonComponent["Far"];
			}

			if (typeString == "Orthographic")
			{
				m_projectionType = eProjectionType::Orthographic;

				if (jsonComponent.contains("Size") && !jsonComponent["Size"].is_null())
					m_orthographicSize = jsonComponent["Size"];

				if (jsonComponent.contains("Near") && !jsonComponent["Near"].is_null())
					m_orthographicNear = jsonComponent["Near"];

				if (jsonComponent.contains("Far") && !jsonComponent["Far"].is_null())
					m_orthographicFar = jsonComponent["Far"];
			}
		}
		else
		{
			SGSWARN("Camera item has no type assigned when deserializing.");
		}
	}
}