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

#include "scene/types.h"

namespace Sogas 
{
	// TODO: This is valid as long as we render one mesh at a time. It will be invalid once we start optimizing using batch rendering, etc...
	struct RenderState
	{
		glm::mat4 transform;
	} currentRenderState;

	std::unique_ptr<Renderer::sceneData>Renderer::s_sceneData = std::make_unique<Renderer::sceneData>();
	std::shared_ptr<Scene> Renderer::s_pScene;

	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	RendererAPI* Renderer::s_RendererAPI = new OpenGLRendererAPI;

	bool Renderer::environment = true;
	Renderer* Renderer::m_handle = nullptr;

	struct RenderStateInfo
	{
		std::weak_ptr<Shader> currentShader;
	} renderStateInfo;

	bool Renderer::init()
	{
		return s_RendererAPI->init();
	}

	void Renderer::beginScene(std::shared_ptr<Scene>& scene, std::shared_ptr<Camera>& pCamera)
	{
		RenderCommand::setClearColor(glm::vec4(1.0f, 0.2f, 0.2f, 1.0f));
		RenderCommand::clear();
		RenderCommand::setDepthFunc(DepthTypes::LEQUAL);
		RenderCommand::enableDepthBuffer(true);

		RenderCommand::setBlendFunc(BlendTypes::ZERO, BlendTypes::ZERO);
		RenderCommand::enableBlend(true);

		s_pScene							= scene;
		s_sceneData->viewprojectionMatrix	= pCamera->getViewProjection();
		s_sceneData->cameraPosition			= pCamera->getPosition();
	}
	/*
	void Renderer::draw()
	{
		std::vector<StrongEntityPtr> renderables = s_pScene->getByComponent<RenderComponent>();

//#ifdef SOGAS_EDITOR
		// TODO This should be after all renderables have been drown, the problem is that the objects highlighted outline would not be seen.
		if (s_pScene->m_renderEnvironment) {
			renderEnvironment(s_pScene->getEnvironment());
		}
//#endif

		// TODO Make rendering child nodes cleaner
		for (const auto& renderable : renderables)
		{
			auto renderComponent = makeStrongPtr(renderable->getComponent<RenderComponent>());
			glm::mat4 model = makeStrongPtr(renderable->getComponent<TransformComponent>())->getGlobalTransform();

			if(renderComponent->getMesh() && renderComponent->getMaterial())
				submit(renderComponent, model);
		}
	}
	*/
	void Renderer::endScene()
	{
		
	}

	void Renderer::submit(const std::shared_ptr<RenderComponent>& renderComponent, const glm::mat4& transform)
	{

		auto material = renderComponent->getMaterial();
		std::shared_ptr<Shader> shader;
		// TODO This is a bug right now, first outline is black and all outlines not shown while environmnet is true.
		// Outline pass - Only do this if component is selected
		if (renderComponent->getOwner().lock()->isSelected())
		{
			shader = Shader::GET("outline.shader");
			RenderCommand::enableDepthBuffer(true);
			RenderCommand::enableDepthMask(false);
			shader->bind();
			shader->setUniform("u_viewprojection", s_sceneData->viewprojectionMatrix);
			shader->setUniform("u_model", glm::scale(transform, glm::vec3(1.1f)));

			renderComponent->getMesh()->m_vertexArray->bind();
			if (renderComponent->getMesh()->m_vertexArray->getIndexBuffer())
				RenderCommand::drawIndexed(renderComponent->getMesh()->m_vertexArray, renderComponent->getPrimitive());
			else
				RenderCommand::draw(renderComponent->getMesh()->m_vertexArray, renderComponent->getPrimitive());
			renderComponent->getMesh()->m_vertexArray->unbind();
			shader->unbind();
			RenderCommand::enableDepthBuffer(true);
			RenderCommand::enableDepthMask(true);
		}
		
		// Shading path
		shader = renderComponent->getShader();
		shader->bind();
		shader->setUniform("u_viewProjectionMatrix", s_sceneData->viewprojectionMatrix);
		shader->setUniform("u_cameraPosition", s_sceneData->cameraPosition);
		shader->setUniform("u_model", transform);
		shader->setUniform("u_color", renderComponent->getMaterial()->getMaterialProperties().color);
		shader->setUniform("u_metalness", renderComponent->getMaterial()->getMaterialProperties().metallicFactor);
		shader->setUniform("u_roughness", renderComponent->getMaterial()->getMaterialProperties().roughnessFactor);
		shader->setUniform("u_entityID", static_cast<int>(renderComponent->getOwner().lock()->getId())); 

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

	void Renderer::submit(const glm::mat4& transform)
	{
		currentRenderState.transform = transform;
	}

	void Renderer::submit(Material* pMaterial, const EntityId entityId)
	{
		renderStateInfo.currentShader = pMaterial->getShader();
		std::shared_ptr<Shader> shader = renderStateInfo.currentShader.lock(); // reading purposes

		shader->bind();
		shader->setUniform("u_model", currentRenderState.transform);
		shader->setUniform("u_viewProjectionMatrix", s_sceneData->viewprojectionMatrix);
		shader->setUniform("u_cameraPosition", s_sceneData->cameraPosition);
		shader->setUniform("u_color", pMaterial->getMaterialProperties().color);
		shader->setUniform("u_metalness", pMaterial->getMaterialProperties().metallicFactor);
		shader->setUniform("u_roughness", pMaterial->getMaterialProperties().roughnessFactor);
		shader->setUniform("u_entityID", static_cast<int>(entityId));

		if (pMaterial->getColorTexture())
		{
			pMaterial->getColorTexture()->bind(0);
			shader->setUniform("u_texture", 0);
		}
		else
			shader->setUniform("u_texture", 0);	// TODO: use default texture ??
	}

	// [IMPORTANT] TODO: make a temporary state of the rendering pass (current shader)
	void Renderer::submit(Mesh* pMesh, const Primitive primitive)
	{
		std::shared_ptr<Shader> shader = renderStateInfo.currentShader.lock(); // reading purposes

		// lights set up and drawing of mesh

		auto lights = s_pScene->getByComponent<LightComponent>();

		if (lights.empty())
		{
			RenderCommand::enableBlend(false);
			pMesh->m_vertexArray->bind();
			if (pMesh->m_vertexArray->getIndexBuffer())
				RenderCommand::drawIndexed(pMesh->m_vertexArray, primitive);
			else
				RenderCommand::draw(pMesh->m_vertexArray, primitive);
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

				pMesh->m_vertexArray->bind();
				if (pMesh->m_vertexArray->getIndexBuffer())
					RenderCommand::drawIndexed(pMesh->m_vertexArray, primitive);
				else
					RenderCommand::draw(pMesh->m_vertexArray, primitive);
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
		
		RenderCommand::enableBlend(true);
		shader->unbind();
	}

	void Renderer::renderEnvironment(std::weak_ptr<Environment> environment)
	{
		RenderCommand::enableDepthBuffer(true);
		RenderCommand::setDepthFunc(DepthTypes::LEQUAL);
		RenderCommand::enableBlend(false);

		auto shader = Shader::GET("environment.shader");
		shader->bind();

		shader->setUniform("u_model", glm::translate(glm::mat4(1), s_sceneData->cameraPosition));
		shader->setUniform("u_viewprojection", s_sceneData->viewprojectionMatrix);

		auto texture = environment.lock()->getTexture();
		texture->bind(1);
		shader->setUniform("u_cubemap", 1);

		std::shared_ptr<Mesh> mesh = environment.lock()->getMesh();
		mesh->m_vertexArray->bind();
		if (mesh->m_vertexArray->getIndexBuffer())
			RenderCommand::drawIndexed(mesh->m_vertexArray);
		else
			RenderCommand::draw(mesh->m_vertexArray);

		RenderCommand::enableDepthBuffer(true);
		RenderCommand::enableBlend(true);
		shader->unbind();
	}

	void Renderer::shutdown()
	{
		s_RendererAPI->shutdown();
	}
}