#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Aria {

	class ARIA_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}