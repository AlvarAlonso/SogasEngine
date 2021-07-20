#include "sgspch.h"

#include "renderer.h"
#include "shader.h"
#include "platform/OpenGL/openGLRendererAPI.h"
#include "platform/openGL/openGLShader.h"
#include "scene/scene.h"
#include "renderer/resources/material.h"
#include "renderer/resources/texture.h"

#include "scene/components/cameraComponent.h"
#include "scene/components/lightComponent.h"
#include "scene/components/transformComponent.h"
#include "scene/components/renderComponent.h"

namespace Sogas 
{
	std::unique_ptr<Renderer::sceneData>Renderer::s_sceneData = std::make_unique<Renderer::sceneData>();
	std::shared_ptr<Scene> Renderer::s_pScene;

	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	RendererAPI* Renderer::s_RendererAPI = new OpenGLRendererAPI;

	void Renderer::beginScene(std::shared_ptr<Scene>& scene, std::shared_ptr<Camera>& pCamera)
	{
		RenderCommand::setClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		RenderCommand::clear();
		RenderCommand::setDepthBuffer(true);

		// TODO Create enum with values to pass to depth and blend functions
		glDepthFunc(GL_LEQUAL);

		RenderCommand::setBlendFunc(true);
		RenderCommand::enableBlend(true);

		s_pScene = scene;
		s_sceneData->viewprojectionMatrix = pCamera->getViewProjection();
		s_sceneData->cameraPosition = pCamera->getPosition();
	}

	void Renderer::render()
	{
		std::vector<StrongEntityPtr> renderables = s_pScene->getByComponent<RenderComponent>();

		for (const auto& renderable : renderables)
		{
			auto renderComponent = makeStrongPtr(renderable->getComponent<RenderComponent>());
			glm::mat4 model = makeStrongPtr(renderable->getComponent<TransformComponent>())->getTransform();

			Renderer::submit(renderComponent, model);
		}
	}

	void Renderer::endScene()
	{

	}

	void Renderer::submit(const std::shared_ptr<RenderComponent>& renderComponent, const glm::mat4& transform)
	{
		auto& shader = renderComponent->getShader();
		auto& material = renderComponent->getMaterial();
		shader->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_viewProjectionMatrix", s_sceneData->viewprojectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_view", s_sceneData->cameraPosition);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_model", transform);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_entityID", static_cast<int>(renderComponent->getId()));

		if (material->getColorTexture())
		{
			material->getColorTexture()->bind(0);
			std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_texture", 0);
		}
		else
			std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_texture", 0);	// TODO: use default texture ??

		auto lights = s_pScene->getByComponent<LightComponent>();

		if (lights.empty())
		{
			RenderCommand::enableBlend(false);
			RenderCommand::drawIndexed(renderComponent->getMesh()->m_vertexArray);
		}
		else
		{
			for (u32 i = 0; i < lights.size(); i++)
			{
				(i == 0) ? RenderCommand::enableBlend(false) : RenderCommand::enableBlend(true);

				auto& light = lights.at(i);

				// Set light position
				glm::vec3 lightPosition = makeStrongPtr(light->getComponent<TransformComponent>())->getTranslation();
				std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_lightPosition", lightPosition);

				// Set light colour
				auto lightComponent = makeStrongPtr(light->getComponent<LightComponent>());
				std::dynamic_pointer_cast<OpenGLShader>(shader)->setUniform("u_lightColor", lightComponent->getColor());

				renderComponent->getMesh()->m_vertexArray->bind();
				RenderCommand::drawIndexed(renderComponent->getMesh()->m_vertexArray);
			}
		}
	}
}