#pragma once

#include "Aria/Core/Log.h"
#include "vulkan/vk_enum_string_helper.h"

#define ARIA_VK_CHECK_RESULT_AND_ERROR(err, ...)                                                                       \
  if (err != VK_SUCCESS) { ARIA_CORE_ERROR("Vulkan API Error: {0} - {1}", __VA_ARGS__, string_VkResult(err)); }
#define ARIA_VK_CHECK_RESULT_AND_WARN(err, ...)                                                                        \
  if (err != VK_SUCCESS) { ARIA_CORE_WARN("Vulkan API Warning: {0} - {1}", __VA_ARGS__, string_VkResult(err)); }
