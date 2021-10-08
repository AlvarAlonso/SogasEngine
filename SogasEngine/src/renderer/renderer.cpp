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

#include "glm/gtx/quaternion.hpp"

#include "scene/types.h"

namespace Sogas 
{
	std::unique_ptr<Renderer::sceneData>Renderer::s_sceneData = std::make_unique<Renderer::sceneData>();
	std::shared_ptr<Scene> Renderer::s_pScene;

	Renderer::API Renderer::s_API = Renderer::API::OpenGL;
	RendererAPI* Renderer::s_RendererAPI = new OpenGLRendererAPI;

	bool Renderer::environment = true;
	Renderer* Renderer::m_handle = nullptr;

	bool Renderer::init()
	{
		return s_RendererAPI->init();
	}

	void Renderer::beginEditorScene(std::shared_ptr<Scene>& scene, std::shared_ptr<Camera>& pCamera)
	{
		RenderCommand::setClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
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
	* @brief Render the ingame scene. Search for the main camera component and render it from its view.
	* @param shared_ptr<Scene>& scene
	* @return void
	*/
	void Renderer::beginRuntimeScene(std::shared_ptr<Scene>& scene)
	{
		RenderCommand::setClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		RenderCommand::clear();
		RenderCommand::setDepthFunc(DepthTypes::LEQUAL);
		RenderCommand::enableDepthBuffer(true);

		RenderCommand::setBlendFunc(BlendTypes::ZERO, BlendTypes::ZERO);
		RenderCommand::enableBlend(true);

		s_pScene = scene;

		glm::mat4 view, projection;
		glm::vec3 cameraPosition = glm::vec3(0.0f);
		for (const auto& ent : scene->getEntities()) {
			if (ent->has<CameraComponent>()) {
				if (ent->getComponent<CameraComponent>().lock()->isPrimary()) 
				{
					projection		= ent->getComponent<CameraComponent>().lock()->getProjection();
					view			= ent->getComponent<TransformComponent>().lock()->getGlobalTransform();
					cameraPosition	= ent->getComponent<TransformComponent>().lock()->getTranslation();
					break;
				}
			}
		}

		SGSDEBUG("%f, %f, %f", cameraPosition.x, cameraPosition.y, cameraPosition.z);

		s_sceneData->viewprojectionMatrix	= projection * view;
		s_sceneData->cameraPosition			= -cameraPosition;
	}

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

	void Renderer::endScene()
	{
		
	}

	void Renderer::submit(const std::shared_ptr<RenderComponent>& renderComponent, const glm::mat4& transform)
	{

		auto& material = renderComponent->getMaterial();
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
			//renderComponent->getMesh()->m_vertexArray->unbind();
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
		//grid->m_vertexArray->unbind();
		
		RenderCommand::enableBlend(true);
		shader->unbind();
	}

	/*
	* @brief Function with the goal of rendering the environment as a cubemap.
	* @param std::weak_ptr<Environment> environment
	* @return void
	*/
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
		//mesh->m_vertexArray->unbind();

		RenderCommand::enableDepthBuffer(true);
		RenderCommand::enableBlend(true);
		shader->unbind();
	}

	/*
	* @brief Render the icons for items like lights or cameras.
	* @param void
	* @return void
	*/
	void Renderer::renderEditorUI()
	{
		RenderCommand::enableBlend(false);

		// Do lights at the moment
		for (const auto& entity : s_pScene->getEntities())
		{
			if (entity->has<LightComponent>())
			{
				glm::mat4 model = entity->getComponent<TransformComponent>().lock()->getGlobalTransform();
				//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
				glm::vec3 position			= entity->getComponent<TransformComponent>().lock()->getTranslation();
				glm::vec3 targetVector		= glm::normalize(s_sceneData->cameraPosition - position);
				glm::quat rotationQuat		= glm::rotation(glm::vec3(0, 1, 0), -targetVector);
				glm::mat4 rotationMatrix	= glm::toMat4(rotationQuat);
				model = glm::scale(model, glm::vec3(0.25f));

				model *= rotationMatrix;

				std::shared_ptr<Mesh>		mesh = Mesh::GET("plane.obj");
				std::shared_ptr<Shader>		shader = Shader::GET("editorUI.shader");
				std::shared_ptr<Texture2D>	texture = Texture2D::GET("../Sogas-Editor/resources/lightbulb-icon.png");
				texture->bind(1);

				shader->bind();
				shader->setUniform("u_model", model);
				shader->setUniform("u_viewprojectionMatrix", s_sceneData->viewprojectionMatrix);
				shader->setUniform("u_texture", 1);

				mesh->m_vertexArray->bind();
				RenderCommand::drawIndexed(mesh->m_vertexArray);
			}

			if (entity->has<CameraComponent>())
			{
				std::weak_ptr<CameraComponent> cameraComponent = entity->getComponent<CameraComponent>();

				std::shared_ptr<Mesh> camera = std::make_shared<Mesh>();
				camera->createCameraMesh(cameraComponent);

				glm::mat4 model = entity->getComponent<TransformComponent>().lock()->getGlobalTransform();

				std::shared_ptr<Shader> shader = Shader::GET("editorCamera.shader");

				shader->bind();
				shader->setUniform("u_model", model);
				shader->setUniform("u_viewprojectionMatrix", s_sceneData->viewprojectionMatrix);
				shader->setUniform("u_color", glm::vec3(0.0f));

				camera->m_vertexArray->bind();
				RenderCommand::setLineWidth(2.0f);
				RenderCommand::drawIndexed(camera->m_vertexArray, Primitive::LINE_LOOP);
				RenderCommand::setLineWidth(1.0f);

				if (cameraComponent.lock()->getShowFrustrum())
				{
					std::shared_ptr frustrum = std::make_shared<Mesh>();
					frustrum->getFrustrumFromCamera(cameraComponent);

					glm::mat4 model = entity->getComponent<TransformComponent>().lock()->getGlobalTransform();
					std::shared_ptr<Shader> shader = Shader::GET("editorCamera.shader");

					shader->bind();
					shader->setUniform("u_model", model);
					shader->setUniform("u_viewprojectionMatrix", s_sceneData->viewprojectionMatrix);
					shader->setUniform("u_color", glm::vec3(0.5f));

					frustrum->m_vertexArray->bind();
					RenderCommand::drawIndexed(frustrum->m_vertexArray, Primitive::LINES);
				}
			}
		}
	}

	void Renderer::shutdown()
	{
		s_RendererAPI->shutdown();
	}
}