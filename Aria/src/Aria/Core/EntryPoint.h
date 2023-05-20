#pragma once

#include "Base.h"
#include "Application.h"
#include "Log.h"

#ifdef ARIA_PLATFORM_WINDOWS

extern aria::Application *aria::CreateApplication();

int main(int argc, char **argv) {
  aria::Log::Init();

  ARIA_CORE_WARN("Initialized log!")
  int a = 4;
  ARIA_INFO("Hello var={0}", a)
  ARIA_CRITICAL("CRITICAL ERROR~!")
  ARIA_ERROR("Oh noes! An error!")
  ARIA_WARN("Warning! Warning!")

  auto app = aria::CreateApplication();
  app->Run();
  delete app;
}

#endif