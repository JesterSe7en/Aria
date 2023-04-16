#pragma once

#ifdef ARIA_PLATFORM_WINDOWS

extern Aria::Application* Aria::CreateApplication();

int main(int argc, char** argv) {
  Aria::Log::Init();

  ARIA_CORE_WARN("Initialized log!");
  int a = 4;
  ARIA_INFO("Hello var={0}", a);
  ARIA_CRITICAL("CRITICAL ERROR~!");
  ARIA_ERROR("Oh noes! An error!");
  ARIA_WARN("Warning! Warning!");

  auto app = Aria::CreateApplication();
  app->Run();
  delete app;
}

#endif