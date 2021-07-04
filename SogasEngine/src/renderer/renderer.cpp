#include "sgspch.h"

#include "renderer.h"
#include "shader.h"
#include "platform/OpenGL/openGLRendererAPI.h"
#include "platform/openGL/openGLShader.h"
#include "scene/scene.h"

#include "scene/cameraComponent.h"
#include "scene/lightComponent.h"
#include "scene/transformComponent.h"

namespace Sogas 
{
	std::unique_ptr<Renderer::sceneData>Renderer::s_sceneData = std::make_unique<Renderer::sceneData>();

	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	RendererAPI* Renderer::s_RendererAPI = new OpenGLRendererAPI;

	void Renderer::beginScene(std::shared_ptr<Scene>& scene, StrongEntityPtr pCamera)
	{
		RenderCommand::setClearColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		RenderCommand::clear();
		RenderCommand::setDepthBuffer(true);

		RenderCommand::setBlendFunc(true);

		std::shared_ptr<CameraComponent> camera = makeStrongPtr(pCamera->getComponent<CameraComponent>(CameraComponent::s_name));
		s_sceneData->viewprojectionMatrix = camera->camera->getViewProjection();

		s_sceneData->lights = scene->getByComponent(LightComponent::s_name);
	}

	void Renderer::endScene()
	{

	}

	// TODO: dataScene should be to pass all scene info
	void Renderer::submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_viewProjectionMatrix", s_sceneData->viewprojectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_model", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_texture", 0);

		for (const auto& light : s_sceneData->lights)
		{

			// Set light position
			glm::mat4 lightModel = makeStrongPtr(light->getComponent<TransformComponent>(TransformComponent::s_name))->getTransform();
			std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_lightPosition", (glm::vec3)lightModel[3]);

			// Set light colour
			auto lightComponent = makeStrongPtr(light->getComponent<LightComponent>(LightComponent::s_name));
			std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_lightColor", lightComponent->getColor());

			vertexArray->bind();
			RenderCommand::drawIndexed(vertexArray);
		}

	}
}