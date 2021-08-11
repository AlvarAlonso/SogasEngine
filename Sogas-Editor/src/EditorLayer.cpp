#include "EditorLayer.h"

#include "core/input.h"
#include "core/keyCodes.h"
#include "core/mouseButtonCodes.h"
#include "core/logger.h"
#include "core/cameraController.h"

#include "platform/utils/platformUtils.h"

#include <ImGuizmo.h>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "renderer/renderer.h"
#include "renderer/framebuffer.h"
#include "renderer/resources/mesh.h" // For grid in editor. Rethink if it should be in here

// TODO: find a better place to define components
#include "scene/entityFactory.h"
#include "scene/entity.h"
#include "scene/scene.h"
#include "scene/serializer.h"
#include "scene/components/transformComponent.h"
#include "scene/components/renderComponent.h"
#include "scene/components/cameraComponent.h"
#include "scene/components/lightComponent.h"

#include "Panels/ScenePanel.h"

namespace Sogas 
{
	extern const std::filesystem::path g_assetsDirectory;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::onAttach()
	{
		Sogas::FramebufferSpecs specs;
		specs.attachments = { FramebufferTextureFormat::R8G8B8A8_FLOAT, FramebufferTextureFormat::R32_INT, FramebufferTextureFormat::D24S8_FLOAT };
		specs.width = 1280; // Application::getInstance()->getWindow().getWidth();
		specs.height = 720; // Application::getInstance()->getWindow().getHeight();

		m_framebuffer = Framebuffer::create(specs);

		m_pCamera = std::make_shared<Camera>();

		this->newScene();
		Serializer serializer(m_pScene);
		serializer.deserialize("../Assets/scenes/basic.json");

		// TODO: add scripting for camera movement/behavior
		m_cameraController.reset(new CameraController(m_pCamera));
		//m_pCamera->lookat({ 20.0f, 10.0f, -50.0f }, { 0.0f, 0.0f, 0.0f });
		//m_pCamera->lookat({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 10.0f });
		m_pCamera->setPosition(glm::vec3{ 0.0f, 3.0f, -15.0f });

		mouse_pos = { Application::getInstance()->getWindow().getWidth(), Application::getInstance()->getWindow().getHeight() };

		m_scenePanel.setContext(m_pScene);

		u32 texData = 0xffffffff;
		std::shared_ptr<Texture2D> whiteTexture = Texture2D::create(1, 1, &texData);
		Texture2D::loadToMap(whiteTexture, "Default white");
	}

	void EditorLayer::onDetach()
	{
	}

	void EditorLayer::onUpdate(f32 dt)
	{
		// TODO: Framebuffer resize function

		if (m_viewportFocused)
		{
			m_cameraController->onUpdate(dt);
		}

		// TODO: implement onUpdate func
		m_pScene->onUpdate(dt);

		// TODO: Should grid be directly implemented by editor layer?

		m_framebuffer->bind();


		Renderer::beginScene(m_pScene, m_pCamera);
		m_framebuffer->clearAttachment(1, -1);
		Renderer::render();
		if (!m_pGrid)
		{
			m_pGrid = std::make_shared<Mesh>();
			m_pGrid->createGrid();
		}
		Renderer::renderGrid(m_pGrid);
		if (m_pScene->m_renderEnvironment)
		{
			Renderer::renderEnvironment(m_pScene->getEnvironment());
		}
		Renderer::endScene();


		// TODO: Mouse picking

		ImGui::SetCurrentContext(Application::getImguiContext());

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_viewportBounds[0].x;
		my -= m_viewportBounds[0].y;
		glm::vec2 viewportSize = m_viewportBounds[1] - m_viewportBounds[0];
		my = viewportSize.y - my;
		i32 mouseX = (i32)mx;
		i32 mouseY = (i32)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (i32)viewportSize.x && mouseY < (i32)viewportSize.y)
		{
			// TODO: this has a high computational cost, in the future it probably needs a refactor
			i32 pixelData = m_framebuffer->readPixel(1, mouseX, mouseY);

			if (pixelData == -1)
			{
				m_hoveredEntity.lock() = nullptr;
			}
			else
			{
				// TODO: likely to be a bug
				m_hoveredEntity = m_pScene->findEntityById(pixelData);
			}

			m_entityIdHovered = pixelData;
		}

		m_framebuffer->unbind();
	}

	void EditorLayer::onImguiRender()
	{
		ImGui::SetCurrentContext(Application::getImguiContext());

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					newScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					openScene();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					saveScene();

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					saveSceneAs();

				if (ImGui::MenuItem("Exit", NULL, false))
					Application::getInstance()->close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_scenePanel.onImGuiRender();
		m_assetsPanel.onImGuiRender();

		// Hovered Entity
		ImGui::Begin("Hovered Info");
		std::string name = "none";
		if (m_hoveredEntity.lock())
			name = m_hoveredEntity.lock()->getName();
		ImGui::Text("Hovered Entity: %s", name.c_str());
		ImGui::Text("ID hovered: %d", m_entityIdHovered);
		ImGui::End();

		// Stats panel
		ImGui::Begin("Stats");
		ImGui::Text("Frame Count: %i", ImGui::GetFrameCount());
		ImGui::Text("Delta time: %f ms", io.DeltaTime);
		ImGui::Text("Framerate %.2f fps", io.Framerate);
		ImGui::End();

		// Viewport panel
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0, 0.0));
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::getInstance()->getImGuiLayer()->blockEvents(!m_viewportFocused || !m_viewportHovered);

		//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		//m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// TODO: Should also resize the aspect ratio of the camera
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_viewportSize != *((glm::vec2*) & viewportPanelSize))
		{
			m_framebuffer->resize((u32)viewportPanelSize.x, (u32)viewportPanelSize.y);
			m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
			m_cameraController.get()->setViewportSize(m_viewportSize.x, m_viewportSize.y);
		}
		u64 textureId = m_framebuffer->getColorAttachment();
		ImGui::Image((ImTextureID)textureId, ImVec2{ m_viewportSize.x, m_viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		
		if(ImGui::BeginDragDropTarget())
		{
			if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS_PANEL_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				// TODO: open the scene
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		StrongEntityPtr selectedEntity = m_scenePanel.getSelectedEntity().lock();
		if(selectedEntity && m_gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_viewportBounds[1].x - m_viewportBounds[0].x, m_viewportBounds[1].y - m_viewportBounds[0].y);
		
			// Editor camera
			const glm::mat4& cameraProjection = m_pCamera->getProjection();
			glm::mat4 cameraView = m_pCamera->getView();

			// Entity transform
			//std::weak_ptr<TransformComponent> transformComponent = selectedEntity->getComponent<TransformComponent>(TransformComponent::s_name);
			std::weak_ptr<TransformComponent> transformComponent = selectedEntity->getComponent<TransformComponent>();

			glm::mat4 transform = transformComponent.lock()->getTransform();

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, nullptr);

			if(ImGuizmo::IsUsing())
			{
				f32 matrixTranslation[3], matrixRotation[3], matrixScale[3];
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), matrixTranslation, matrixRotation, matrixScale);

				// apply transformation here
				glm::vec3 deltaRotation = glm::make_vec3(matrixRotation) - transformComponent.lock()->getRotation();
				
				glm::vec3 newTranslation = glm::make_vec3(matrixTranslation);
				transformComponent.lock()->setTranslation(newTranslation);

				glm::vec3 rot = glm::eulerAngles(glm::quat_cast(transform));
				//glm::vec3 newRotation = glm::make_vec3(matrixRotation) + deltaRotation;
				transformComponent.lock()->setRotation(rot);

				glm::vec3 newScale = glm::make_vec3(matrixScale);
				transformComponent.lock()->setScale(newScale);
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::onEvent(Event& event)
	{
		if (m_viewportFocused) {
			m_cameraController.get()->onEvent(event);
		}

		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FUNC(EditorLayer::onKeyPressed));
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNC(EditorLayer::onMouseButtonPressed));
	}

	bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
	{
		if (e.getRepeatCount() > 0)
			return false;

		bool ctrl = Input::isKeyPressed(SGS_KEY_LEFT_CONTROL) || Input::isKeyPressed(SGS_KEY_RIGHT_CONTROL);
		bool shift = Input::isKeyPressed(SGS_KEY_LEFT_SHIFT) || Input::isKeyPressed(SGS_KEY_RIGHT_SHIFT);

		switch(e.getKeyCode())
		{
			// Gizmos
			case SGS_KEY_Q:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = -1;
				break;
			}
			case SGS_KEY_W:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case SGS_KEY_E:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case SGS_KEY_R:
			{
				if (!ImGuizmo::IsUsing())
					m_gizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
			case SGS_KEY_N:
			{
				if(ctrl)
					newScene();
				break;
			}
			case SGS_KEY_S:
			{
				if (ctrl && shift) {
					saveSceneAs();
					break;
				}
				if (ctrl)
					saveScene();
				break;
			}
			case SGS_KEY_O:
			{
				if (ctrl)
					openScene();
				break;
			}
		}
	}

	bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if(e.getMouseButton() == SGS_MOUSE_BUTTON_LEFT)
		{

			// TODO: The m_entities of the scene fails when the hovered entity is set to be selected. The entity selected is the one that fails

			if (m_viewportHovered && !ImGuizmo::IsOver() && !Input::isKeyPressed(SGS_KEY_LEFT_ALT))
			{
				m_scenePanel.setSelectedEntity(m_hoveredEntity.lock());
			}
		}

		return false;
	}

	/*
	* @brief Command the application to create a new scene
	* @param void
	* @return void
	*/
	void Sogas::EditorLayer::newScene()
	{
		m_pScene = std::make_shared<Scene>();
		m_scenePanel.setContext(m_pScene);
	}

	/*
	* @brief Allows the user to search a json file where scene is saved and deserialize it.
	* @param void
	* @return void
	*/
	void Sogas::EditorLayer::openScene()
	{
		std::string filepath = FileDialog::openFile("Sogas Scene (*.sgs)\0*.sgs\0Sogas Scene (*.json)\0*.json\0");
		
		if (!filepath.empty())
		{
			m_pScene = std::make_shared<Scene>();
			m_scenePanel.setContext(m_pScene);
			
			Serializer serializer(m_pScene);
			serializer.deserialize(filepath);
		}

		SGSINFO("Open scene function");
	}

	/*
	*	@brief Function called when saving an scene. If scene has been previously saved it automatically
	*	saves it to the same path and name. Call the saveSceneAs() function otherwise.
	*	@param void
	*	@return void
	*/
	void EditorLayer::saveScene()
	{
		if (m_savePath.empty())
		{
			saveSceneAs();
		}
		else
		{
			Serializer serializer(m_pScene);
			serializer.serialize(m_savePath);
		}
	}

	/*
	* @brief Used to open a file explorer and save the file where the scene will be deserialized
	* @param void
	* @return void
	*/
	void Sogas::EditorLayer::saveSceneAs()
	{
		m_savePath = FileDialog::saveFile("Sogas Scene (*.sgs)\0*.sgs\0Sogas Scene (*json)\0*.json\0");

		if (!m_savePath.empty())
		{
			Serializer serializer(m_pScene);
			serializer.serialize(m_savePath);
			SGSINFO("Save scene function");
		}
	}
}