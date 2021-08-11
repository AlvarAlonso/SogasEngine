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
		RenderCommand::setDepthFunc(DepthTypes::LEQUAL);
		RenderCommand::enableDepthBuffer(true);

		RenderCommand::setBlendFunc(BlendTypes::ZERO, BlendTypes::ZERO);
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

			if(renderComponent->getMesh() && renderComponent->getMaterial())
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
		shader->setUniform("u_viewProjectionMatrix", s_sceneData->viewprojectionMatrix);
		shader->setUniform("u_cameraPosition", s_sceneData->cameraPosition);
		shader->setUniform("u_model", transform);
		shader->setUniform("u_color", renderComponent->getMaterial()->getMaterialProperties().color);
		shader->setUniform("u_metalness", renderComponent->getMaterial()->getMaterialProperties().metallicFactor);
		shader->setUniform("u_roughness", renderComponent->getMaterial()->getMaterialProperties().roughnessFactor);
		shader->setUniform("u_entityID", static_cast<int>(renderComponent->getId()));

		if (material->getColorTexture())
		{
			material->getColorTexture()->bind(0);
			shader->setUniform("u_texture", 0);
		}
		else
			shader->setUniform("u_texture", 0);	// TODO: use default texture ??

		auto lights = s_pScene->getByComponent<LightComponent>();

		if (lights.empty())
		{
			RenderCommand::enableBlend(false);
			renderComponent->getMesh()->m_vertexArray->bind();
			if (renderComponent->getMesh()->m_vertexArray->getIndexBuffer())
				RenderCommand::drawIndexed(renderComponent->getMesh()->m_vertexArray, renderComponent->getPrimitive());
			else
				RenderCommand::draw(renderComponent->getMesh()->m_vertexArray, renderComponent->getPrimitive());
		}
		else
		{
			for (u32 i = 0; i < lights.size(); i++)
			{
				(i == 0) ? RenderCommand::enableBlend(false) : RenderCommand::enableBlend(true);

				auto& light = lights.at(i);

				// Set light position
				glm::vec3 lightPosition = makeStrongPtr(light->getComponent<TransformComponent>())->getTranslation();
				shader->setUniform("u_lightPosition", lightPosition);

				// Set light colour
				auto lightComponent = makeStrongPtr(light->getComponent<LightComponent>());
				shader->setUniform("u_lightColor", lightComponent->getColor());
				shader->setUniform("u_lightIntensity", lightComponent->getIntensity());
				shader->setUniform("u_maxLightDistance", lightComponent->getMaxDistance());

				renderComponent->getMesh()->m_vertexArray->bind();
				if (renderComponent->getMesh()->m_vertexArray->getIndexBuffer())
					RenderCommand::drawIndexed(renderComponent->getMesh()->m_vertexArray, renderComponent->getPrimitive());
				else
					RenderCommand::draw(renderComponent->getMesh()->m_vertexArray, renderComponent->getPrimitive());
			}
		}
	}

	void Renderer::renderGrid(std::shared_ptr<Mesh>& grid)
	{
		RenderCommand::setLineWidth(1);
		RenderCommand::enableBlend(false);

		auto shader = Shader::GETDefault("grid");
		shader->bind();
		shader->setUniform("u_color", glm::vec4(0.7f));
		shader->setUniform("u_model", glm::mat4(1));
		shader->setUniform("u_camera_position", s_sceneData->cameraPosition);
		shader->setUniform("u_viewprojection", s_sceneData->viewprojectionMatrix);

		grid->m_vertexArray->bind();
		RenderCommand::draw(grid->m_vertexArray, Primitive::LINES);
		
		RenderCommand::enableBlend(false);
		shader->unbind();
	}
}