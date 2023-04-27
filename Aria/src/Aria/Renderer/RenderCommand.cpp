#include "ariapch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ARIA {

  RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;

}