#pragma once

#include "entityComponent.h"
#include "core/camera.h"

namespace Sogas
{
	class SGS CameraComponent : public EntityComponent
	{
	public:
		static const char* s_name;

		CameraComponent() = default;

		virtual const char* getName() const override { return s_name; }
		virtual bool init() override;
		virtual void to_json(json& j) override {};

		std::shared_ptr<Camera> camera;
		bool primary = false;
	};
}