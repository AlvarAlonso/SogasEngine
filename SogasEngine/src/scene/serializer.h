#pragma once

#include "defines.h"

#include <memory>
#include <string>

namespace Sogas
{
	class Scene;

	// TODO: should this be exported?
	class SGS Serializer
	{
	public:

		Serializer(std::shared_ptr<Scene>& scene);

		bool serialize(const std::string& filename);

		bool deserialize(const std::string& filename);

	private:
		std::shared_ptr<Scene> m_pScene;
	};
}