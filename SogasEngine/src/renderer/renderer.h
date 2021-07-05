#pragma once

#include "rendererAPI.h"
#include "renderCommand.h"

#include "scene/types.h"


// TODO: add statistics to print in Sogas-Editor
struct statistics
{
	u32 drawCalls = 0;
};

namespace Sogas 
{
	class Shader;
	class Scene;
	class Camera;
	class Material;

	class SGS Renderer
	{
	public:

		enum class API
		{
			None = 0, OpenGL = 1, Vulkan = 2
		};

		inline static API getAPI() { return s_API; }

		static void beginScene(std::shared_ptr<Scene>& scene, std::shared_ptr<Camera>& camera);
		static void endScene();
		//static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform);
		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, std::shared_ptr<Material>& material);

	private:
		static API s_API; // TODO: Hide the API to the rest of the application
		static RendererAPI* s_RendererAPI;

		struct sceneData
		{
			glm::mat4 viewprojectionMatrix;

			std::vector<StrongEntityPtr> lights;
		};

		static std::unique_ptr<sceneData> s_sceneData;
	};
}