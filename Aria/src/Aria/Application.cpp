#include "ariapch.h"
#include "Application.h"

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Log.h"

namespace Aria {

Application::Application() {
  m_Window = std::unique_ptr<Window>(Window::Create());
}

Application::~Application() {}

void Application::Run() {


  while (true) {
    while (m_Running) {
      m_Window->OnUpdate();
    }
  }
}
}  // namespace Aria
