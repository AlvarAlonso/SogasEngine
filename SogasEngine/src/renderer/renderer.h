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
	class Scene;
	class Camera;
	class RenderComponent;
	class Mesh;
	class Environment;

	class SGS Renderer
	{
	public:

		enum class API
		{
			None = 0, OpenGL = 1, Vulkan = 2
		};

		inline static API getAPI() { return s_API; }

		static Renderer* get()
		{
			if (!m_handle) {
				m_handle = new Renderer();
			}
			return m_handle;
		};


		bool init(); // TODO pass necessary information to init renderer module.
		void beginEditorScene(std::shared_ptr<Scene>& scene, std::shared_ptr<Camera>& camera);
		void beginRuntimeScene(std::shared_ptr<Scene>& scene);
		void draw();
		void endScene();
		void shutdown();

		// TODO render grid should be private and called inside draw(), not in the application layer
		void renderGrid(std::shared_ptr<Mesh>& grid); // Maybe grid should be a default render component?
		void renderEditorUI();

	private:
		Renderer() = default; // TODO make user able to chose rendering API.
		void submit(const std::shared_ptr<RenderComponent>& renderComponent, const glm::mat4& transform);
		void renderEnvironment(std::weak_ptr<Environment> environment);

	private:
		static API s_API; // TODO: Hide the API to the rest of the application
		static RendererAPI* s_RendererAPI;

		static Renderer* m_handle;

		static bool environment;

		struct sceneData
		{
			glm::mat4 viewprojectionMatrix;
			glm::vec3 cameraPosition;
		};

		static std::shared_ptr<Scene> s_pScene;
		static std::unique_ptr<sceneData> s_sceneData;
	};
}