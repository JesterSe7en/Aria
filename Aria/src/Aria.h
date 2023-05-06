#pragma once

// For use by ARIA applications

#include <stdio.h>
#include "Aria/Core/Base.h"
#include "Aria/Core/Application.h"
#include "Aria/Core/Layer.h"
#include "Aria/ImGui/ImGuiLayer.h"
#include "Aria/Core/Log.h"
#include "Aria/Core/Timestep.h"

// -------- Input --------
#include "Aria/Core/Input.h"
#include "Aria/Core/KeyCodes.h"

// -------- Renderer --------
#include "Aria/Renderer/Renderer.h"
#include "Aria/Renderer/RenderCommand.h"
#include "Aria/Renderer/Buffer.h"
#include "Aria/Renderer/Camera.h"
#include "Aria/Renderer/Shader.h"
#include "Aria/Renderer/VertexArray.h"
#include "Aria/Renderer/Texture.h"