#include "sgspch.h"
#include "cameraComponent.h"

namespace Sogas
{
	const char* CameraComponent::s_name = "CameraComponent";
	bool CameraComponent::init()
	{
		camera = std::make_shared<Camera>();
		return true;
	}
}