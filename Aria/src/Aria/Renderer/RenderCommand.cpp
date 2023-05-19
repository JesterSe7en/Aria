#include "ariapch.h"

#include "Platform/OpenGL/OpenGlRendererApi.h"

#include "RenderCommand.h"

namespace aria {

RendererApi* RenderCommand::p_renderer_api_ = new OpenGlRendererApi;

}