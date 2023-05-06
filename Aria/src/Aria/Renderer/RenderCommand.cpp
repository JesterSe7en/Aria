#include "ariapch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

#include "RenderCommand.h"


namespace ARIA {

  RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;

}