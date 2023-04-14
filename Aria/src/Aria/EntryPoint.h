#pragma once

#ifdef ARIA_PLATFORM_WINDOWS

extern Aria::Application* Aria::CreateApplication();

int main(int argc, char** argv) {
	printf("Aria Engine\n");
	auto app = Aria::CreateApplication();
	app->Run();
	delete app;
}

#endif