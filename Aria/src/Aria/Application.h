#pragma once

#include "Core.h"

namespace Aria {

	class ARIA_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};
}