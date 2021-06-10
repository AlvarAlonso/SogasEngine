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

// Renderer includes
#include "renderer/shader.h"
#include "renderer/vertexArray.h"
#include "renderer/renderer.h"
#include "renderer/texture.h"
// TODO: platform specific should be done in engine
#include "platform/openGL/openGLShader.h"

#include "imgui/imguiLayer.h"

// ------ Entry Point ---------
#include "core/entryPoint.h"