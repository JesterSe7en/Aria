//
// Created by alyxc on 5/23/2023.
//

#ifndef ARIA_ARIA_SRC_PLATFORM_VULKAN_VULKANERROR_HPP_
#define ARIA_ARIA_SRC_PLATFORM_VULKAN_VULKANERROR_HPP_

#include "Aria/Core/Log.h"

#define ARIA_VK_CHECK_ERROR_AND_LOG(err, ...)                                                                               \
  if (err != VK_SUCCESS) { ARIA_CORE_ERROR("Vulkan Error: {0}", __VA_ARGS__); }

#endif//ARIA_ARIA_SRC_PLATFORM_VULKAN_VULKANERROR_HPP_
