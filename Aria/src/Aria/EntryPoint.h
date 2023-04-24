#pragma once

#ifdef ARIA_PLATFORM_WINDOWS

extern ARIA::Application* ARIA::create_application();

int main(int argc, char** argv) {
  ARIA::Log::init();

  ARIA_CORE_WARN("Initialized log!");
  int a = 4;
  ARIA_INFO("Hello var={0}", a);
  ARIA_CRITICAL("CRITICAL ERROR~!");
  ARIA_ERROR("Oh noes! An error!");
  ARIA_WARN("Warning! Warning!");

  auto app = ARIA::create_application();
  app->run();
  delete app;
}

#endif