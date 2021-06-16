#pragma once

#include "defines.h"

#include "core/application.h"
#include "core/layer.h"
#include "core/logger.h"
#include "core/assertions.h"

#include "core/input.h"
#include "core/keyCodes.h"
#include "core/mouseButtonCodes.h"

#include "core/camera.h"

#include "../external/glm/glm/glm.hpp"
#include "../external/glm/glm/gtc/matrix_transform.hpp"

// Renderer includes
#include "renderer/shader.h"
#include "renderer/vertexArray.h"
#include "renderer/renderer.h"
#include "renderer/framebuffer.h"

#include "renderer/resources/texture.h"
#include "renderer/resources/mesh.h"
// TODO: platform specific should be done in engine
#include "platform/openGL/openGLShader.h"

// TODO: check if that should be included
//#define IMGUI_IMPL_OPENGL_LOADER_GLEW
//#include "imgui/imgui_impl_glfw.h"
//#include "imgui/imgui_impl_opengl3.h"

// ------ Entry Point ---------
#include "core/entryPoint.h"