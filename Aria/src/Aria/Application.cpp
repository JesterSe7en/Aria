#include "Application.h"

#include "Aria/Events/ApplicationEvent.h"
#include "Aria/Events/KeyEvent.h"
#include "Aria/Log.h"

namespace Aria {

	Application::Application() {}

	Application::~Application() {}

	void Application::Run() {
		WindowResizeEvent e(1280, 800);
		ARIA_TRACE(e);

		KeyPressedEvent kp(100, 23);
		ARIA_TRACE(kp);

		KeyReleasedEvent kr(100);
		ARIA_TRACE(kr);

		while (true) {}
	}
} // namespace Aria
