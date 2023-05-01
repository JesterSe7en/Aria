set(PROJECT_NAME Aria)

################################################################################
# Source groups
################################################################################
set(src
    "src/Aria.h"
    "src/ariapch.cpp"
    "src/ariapch.h"
)
source_group("src" FILES ${src})

set(src__Aria
    "src/Aria/Application.cpp"
    "src/Aria/Application.h"
    "src/Aria/Core.h"
    "src/Aria/EntryPoint.h"
    "src/Aria/Input.h"
    "src/Aria/KeyCodes.h"
    "src/Aria/Layer.cpp"
    "src/Aria/Layer.h"
    "src/Aria/LayerStack.cpp"
    "src/Aria/LayerStack.h"
    "src/Aria/Log.cpp"
    "src/Aria/Log.h"
    "src/Aria/Window.h"
)
source_group("src\\Aria" FILES ${src__Aria})

set(src__Aria__Events
    "src/Aria/Events/ApplicationEvent.h"
    "src/Aria/Events/Event.h"
    "src/Aria/Events/KeyEvent.h"
    "src/Aria/Events/MouseEvent.h"
)
source_group("src\\Aria\\Events" FILES ${src__Aria__Events})

set(src__Aria__ImGui
    "src/Aria/ImGui/ImGuiLayer.cpp"
    "src/Aria/ImGui/ImGuiLayer.h"
)
source_group("src\\Aria\\ImGui" FILES ${src__Aria__ImGui})

set(src__Aria__Renderer
    "src/Aria/Renderer/Buffer.cpp"
    "src/Aria/Renderer/Buffer.h"
    "src/Aria/Renderer/Camera.cpp"
    "src/Aria/Renderer/Camera.h"
    "src/Aria/Renderer/GraphicsContext.h"
    "src/Aria/Renderer/RenderCommand.cpp"
    "src/Aria/Renderer/RenderCommand.h"
    "src/Aria/Renderer/Renderer.cpp"
    "src/Aria/Renderer/Renderer.h"
    "src/Aria/Renderer/RendererAPI.cpp"
    "src/Aria/Renderer/RendererAPI.h"
    "src/Aria/Renderer/Shader.cpp"
    "src/Aria/Renderer/Shader.h"
    "src/Aria/Renderer/VertexArray.cpp"
    "src/Aria/Renderer/VertexArray.h"
)
source_group("src\\Aria\\Renderer" FILES ${src__Aria__Renderer})

set(src__Platform__OpenGL
    "src/Platform/OpenGL/OpenGLBuffer.cpp"
    "src/Platform/OpenGL/OpenGLBuffer.h"
    "src/Platform/OpenGL/OpenGLContext.cpp"
    "src/Platform/OpenGL/OpenGLContext.h"
    "src/Platform/OpenGL/OpenGLRendererAPI.cpp"
    "src/Platform/OpenGL/OpenGLRendererAPI.h"
    "src/Platform/OpenGL/OpenGLVertexArray.cpp"
    "src/Platform/OpenGL/OpenGLVertexArray.h"
)
source_group("src\\Platform\\OpenGL" FILES ${src__Platform__OpenGL})

set(src__Platform__Windows
    "src/Platform/Windows/WindowsInput.cpp"
    "src/Platform/Windows/WindowsInput.h"
    "src/Platform/Windows/WindowsWindow.cpp"
    "src/Platform/Windows/WindowsWindow.h"
)
source_group("src\\Platform\\Windows" FILES ${src__Platform__Windows})

set(vendor__glm__glm
    "vendor/glm/glm/common.hpp"
    "vendor/glm/glm/exponential.hpp"
    "vendor/glm/glm/ext.hpp"
    "vendor/glm/glm/fwd.hpp"
    "vendor/glm/glm/geometric.hpp"
    "vendor/glm/glm/glm.hpp"
    "vendor/glm/glm/integer.hpp"
    "vendor/glm/glm/mat2x2.hpp"
    "vendor/glm/glm/mat2x3.hpp"
    "vendor/glm/glm/mat2x4.hpp"
    "vendor/glm/glm/mat3x2.hpp"
    "vendor/glm/glm/mat3x3.hpp"
    "vendor/glm/glm/mat3x4.hpp"
    "vendor/glm/glm/mat4x2.hpp"
    "vendor/glm/glm/mat4x3.hpp"
    "vendor/glm/glm/mat4x4.hpp"
    "vendor/glm/glm/matrix.hpp"
    "vendor/glm/glm/packing.hpp"
    "vendor/glm/glm/trigonometric.hpp"
    "vendor/glm/glm/vec2.hpp"
    "vendor/glm/glm/vec3.hpp"
    "vendor/glm/glm/vec4.hpp"
    "vendor/glm/glm/vector_relational.hpp"
)
source_group("vendor\\glm\\glm" FILES ${vendor__glm__glm})

set(vendor__glm__glm__detail
    "vendor/glm/glm/detail/_features.hpp"
    "vendor/glm/glm/detail/_fixes.hpp"
    "vendor/glm/glm/detail/_noise.hpp"
    "vendor/glm/glm/detail/_swizzle.hpp"
    "vendor/glm/glm/detail/_swizzle_func.hpp"
    "vendor/glm/glm/detail/_vectorize.hpp"
    "vendor/glm/glm/detail/compute_common.hpp"
    "vendor/glm/glm/detail/compute_vector_relational.hpp"
    "vendor/glm/glm/detail/func_common.inl"
    "vendor/glm/glm/detail/func_common_simd.inl"
    "vendor/glm/glm/detail/func_exponential.inl"
    "vendor/glm/glm/detail/func_exponential_simd.inl"
    "vendor/glm/glm/detail/func_geometric.inl"
    "vendor/glm/glm/detail/func_geometric_simd.inl"
    "vendor/glm/glm/detail/func_integer.inl"
    "vendor/glm/glm/detail/func_integer_simd.inl"
    "vendor/glm/glm/detail/func_matrix.inl"
    "vendor/glm/glm/detail/func_matrix_simd.inl"
    "vendor/glm/glm/detail/func_packing.inl"
    "vendor/glm/glm/detail/func_packing_simd.inl"
    "vendor/glm/glm/detail/func_trigonometric.inl"
    "vendor/glm/glm/detail/func_trigonometric_simd.inl"
    "vendor/glm/glm/detail/func_vector_relational.inl"
    "vendor/glm/glm/detail/func_vector_relational_simd.inl"
    "vendor/glm/glm/detail/qualifier.hpp"
    "vendor/glm/glm/detail/setup.hpp"
    "vendor/glm/glm/detail/type_float.hpp"
    "vendor/glm/glm/detail/type_half.hpp"
    "vendor/glm/glm/detail/type_half.inl"
    "vendor/glm/glm/detail/type_mat2x2.hpp"
    "vendor/glm/glm/detail/type_mat2x2.inl"
    "vendor/glm/glm/detail/type_mat2x3.hpp"
    "vendor/glm/glm/detail/type_mat2x3.inl"
    "vendor/glm/glm/detail/type_mat2x4.hpp"
    "vendor/glm/glm/detail/type_mat2x4.inl"
    "vendor/glm/glm/detail/type_mat3x2.hpp"
    "vendor/glm/glm/detail/type_mat3x2.inl"
    "vendor/glm/glm/detail/type_mat3x3.hpp"
    "vendor/glm/glm/detail/type_mat3x3.inl"
    "vendor/glm/glm/detail/type_mat3x4.hpp"
    "vendor/glm/glm/detail/type_mat3x4.inl"
    "vendor/glm/glm/detail/type_mat4x2.hpp"
    "vendor/glm/glm/detail/type_mat4x2.inl"
    "vendor/glm/glm/detail/type_mat4x3.hpp"
    "vendor/glm/glm/detail/type_mat4x3.inl"
    "vendor/glm/glm/detail/type_mat4x4.hpp"
    "vendor/glm/glm/detail/type_mat4x4.inl"
    "vendor/glm/glm/detail/type_mat4x4_simd.inl"
    "vendor/glm/glm/detail/type_quat.hpp"
    "vendor/glm/glm/detail/type_quat.inl"
    "vendor/glm/glm/detail/type_quat_simd.inl"
    "vendor/glm/glm/detail/type_vec1.hpp"
    "vendor/glm/glm/detail/type_vec1.inl"
    "vendor/glm/glm/detail/type_vec2.hpp"
    "vendor/glm/glm/detail/type_vec2.inl"
    "vendor/glm/glm/detail/type_vec3.hpp"
    "vendor/glm/glm/detail/type_vec3.inl"
    "vendor/glm/glm/detail/type_vec4.hpp"
    "vendor/glm/glm/detail/type_vec4.inl"
    "vendor/glm/glm/detail/type_vec4_simd.inl"
)
source_group("vendor\\glm\\glm\\detail" FILES ${vendor__glm__glm__detail})

set(vendor__glm__glm__ext
    "vendor/glm/glm/ext/_matrix_vectorize.hpp"
    "vendor/glm/glm/ext/matrix_clip_space.hpp"
    "vendor/glm/glm/ext/matrix_clip_space.inl"
    "vendor/glm/glm/ext/matrix_common.hpp"
    "vendor/glm/glm/ext/matrix_common.inl"
    "vendor/glm/glm/ext/matrix_double2x2.hpp"
    "vendor/glm/glm/ext/matrix_double2x2_precision.hpp"
    "vendor/glm/glm/ext/matrix_double2x3.hpp"
    "vendor/glm/glm/ext/matrix_double2x3_precision.hpp"
    "vendor/glm/glm/ext/matrix_double2x4.hpp"
    "vendor/glm/glm/ext/matrix_double2x4_precision.hpp"
    "vendor/glm/glm/ext/matrix_double3x2.hpp"
    "vendor/glm/glm/ext/matrix_double3x2_precision.hpp"
    "vendor/glm/glm/ext/matrix_double3x3.hpp"
    "vendor/glm/glm/ext/matrix_double3x3_precision.hpp"
    "vendor/glm/glm/ext/matrix_double3x4.hpp"
    "vendor/glm/glm/ext/matrix_double3x4_precision.hpp"
    "vendor/glm/glm/ext/matrix_double4x2.hpp"
    "vendor/glm/glm/ext/matrix_double4x2_precision.hpp"
    "vendor/glm/glm/ext/matrix_double4x3.hpp"
    "vendor/glm/glm/ext/matrix_double4x3_precision.hpp"
    "vendor/glm/glm/ext/matrix_double4x4.hpp"
    "vendor/glm/glm/ext/matrix_double4x4_precision.hpp"
    "vendor/glm/glm/ext/matrix_float2x2.hpp"
    "vendor/glm/glm/ext/matrix_float2x2_precision.hpp"
    "vendor/glm/glm/ext/matrix_float2x3.hpp"
    "vendor/glm/glm/ext/matrix_float2x3_precision.hpp"
    "vendor/glm/glm/ext/matrix_float2x4.hpp"
    "vendor/glm/glm/ext/matrix_float2x4_precision.hpp"
    "vendor/glm/glm/ext/matrix_float3x2.hpp"
    "vendor/glm/glm/ext/matrix_float3x2_precision.hpp"
    "vendor/glm/glm/ext/matrix_float3x3.hpp"
    "vendor/glm/glm/ext/matrix_float3x3_precision.hpp"
    "vendor/glm/glm/ext/matrix_float3x4.hpp"
    "vendor/glm/glm/ext/matrix_float3x4_precision.hpp"
    "vendor/glm/glm/ext/matrix_float4x2.hpp"
    "vendor/glm/glm/ext/matrix_float4x2_precision.hpp"
    "vendor/glm/glm/ext/matrix_float4x3.hpp"
    "vendor/glm/glm/ext/matrix_float4x3_precision.hpp"
    "vendor/glm/glm/ext/matrix_float4x4.hpp"
    "vendor/glm/glm/ext/matrix_float4x4_precision.hpp"
    "vendor/glm/glm/ext/matrix_int2x2.hpp"
    "vendor/glm/glm/ext/matrix_int2x2_sized.hpp"
    "vendor/glm/glm/ext/matrix_int2x3.hpp"
    "vendor/glm/glm/ext/matrix_int2x3_sized.hpp"
    "vendor/glm/glm/ext/matrix_int2x4.hpp"
    "vendor/glm/glm/ext/matrix_int2x4_sized.hpp"
    "vendor/glm/glm/ext/matrix_int3x2.hpp"
    "vendor/glm/glm/ext/matrix_int3x2_sized.hpp"
    "vendor/glm/glm/ext/matrix_int3x3.hpp"
    "vendor/glm/glm/ext/matrix_int3x3_sized.hpp"
    "vendor/glm/glm/ext/matrix_int3x4.hpp"
    "vendor/glm/glm/ext/matrix_int3x4_sized.hpp"
    "vendor/glm/glm/ext/matrix_int4x2.hpp"
    "vendor/glm/glm/ext/matrix_int4x2_sized.hpp"
    "vendor/glm/glm/ext/matrix_int4x3.hpp"
    "vendor/glm/glm/ext/matrix_int4x3_sized.hpp"
    "vendor/glm/glm/ext/matrix_int4x4.hpp"
    "vendor/glm/glm/ext/matrix_int4x4_sized.hpp"
    "vendor/glm/glm/ext/matrix_integer.hpp"
    "vendor/glm/glm/ext/matrix_integer.inl"
    "vendor/glm/glm/ext/matrix_projection.hpp"
    "vendor/glm/glm/ext/matrix_projection.inl"
    "vendor/glm/glm/ext/matrix_relational.hpp"
    "vendor/glm/glm/ext/matrix_relational.inl"
    "vendor/glm/glm/ext/matrix_transform.hpp"
    "vendor/glm/glm/ext/matrix_transform.inl"
    "vendor/glm/glm/ext/matrix_uint2x2.hpp"
    "vendor/glm/glm/ext/matrix_uint2x2_sized.hpp"
    "vendor/glm/glm/ext/matrix_uint2x3.hpp"
    "vendor/glm/glm/ext/matrix_uint2x3_sized.hpp"
    "vendor/glm/glm/ext/matrix_uint2x4.hpp"
    "vendor/glm/glm/ext/matrix_uint2x4_sized.hpp"
    "vendor/glm/glm/ext/matrix_uint3x2.hpp"
    "vendor/glm/glm/ext/matrix_uint3x2_sized.hpp"
    "vendor/glm/glm/ext/matrix_uint3x3.hpp"
    "vendor/glm/glm/ext/matrix_uint3x3_sized.hpp"
    "vendor/glm/glm/ext/matrix_uint3x4.hpp"
    "vendor/glm/glm/ext/matrix_uint3x4_sized.hpp"
    "vendor/glm/glm/ext/matrix_uint4x2.hpp"
    "vendor/glm/glm/ext/matrix_uint4x2_sized.hpp"
    "vendor/glm/glm/ext/matrix_uint4x3.hpp"
    "vendor/glm/glm/ext/matrix_uint4x3_sized.hpp"
    "vendor/glm/glm/ext/matrix_uint4x4.hpp"
    "vendor/glm/glm/ext/matrix_uint4x4_sized.hpp"
    "vendor/glm/glm/ext/quaternion_common.hpp"
    "vendor/glm/glm/ext/quaternion_common.inl"
    "vendor/glm/glm/ext/quaternion_common_simd.inl"
    "vendor/glm/glm/ext/quaternion_double.hpp"
    "vendor/glm/glm/ext/quaternion_double_precision.hpp"
    "vendor/glm/glm/ext/quaternion_exponential.hpp"
    "vendor/glm/glm/ext/quaternion_exponential.inl"
    "vendor/glm/glm/ext/quaternion_float.hpp"
    "vendor/glm/glm/ext/quaternion_float_precision.hpp"
    "vendor/glm/glm/ext/quaternion_geometric.hpp"
    "vendor/glm/glm/ext/quaternion_geometric.inl"
    "vendor/glm/glm/ext/quaternion_relational.hpp"
    "vendor/glm/glm/ext/quaternion_relational.inl"
    "vendor/glm/glm/ext/quaternion_transform.hpp"
    "vendor/glm/glm/ext/quaternion_transform.inl"
    "vendor/glm/glm/ext/quaternion_trigonometric.hpp"
    "vendor/glm/glm/ext/quaternion_trigonometric.inl"
    "vendor/glm/glm/ext/scalar_common.hpp"
    "vendor/glm/glm/ext/scalar_common.inl"
    "vendor/glm/glm/ext/scalar_constants.hpp"
    "vendor/glm/glm/ext/scalar_constants.inl"
    "vendor/glm/glm/ext/scalar_int_sized.hpp"
    "vendor/glm/glm/ext/scalar_integer.hpp"
    "vendor/glm/glm/ext/scalar_integer.inl"
    "vendor/glm/glm/ext/scalar_packing.hpp"
    "vendor/glm/glm/ext/scalar_packing.inl"
    "vendor/glm/glm/ext/scalar_reciprocal.hpp"
    "vendor/glm/glm/ext/scalar_reciprocal.inl"
    "vendor/glm/glm/ext/scalar_relational.hpp"
    "vendor/glm/glm/ext/scalar_relational.inl"
    "vendor/glm/glm/ext/scalar_uint_sized.hpp"
    "vendor/glm/glm/ext/scalar_ulp.hpp"
    "vendor/glm/glm/ext/scalar_ulp.inl"
    "vendor/glm/glm/ext/vector_bool1.hpp"
    "vendor/glm/glm/ext/vector_bool1_precision.hpp"
    "vendor/glm/glm/ext/vector_bool2.hpp"
    "vendor/glm/glm/ext/vector_bool2_precision.hpp"
    "vendor/glm/glm/ext/vector_bool3.hpp"
    "vendor/glm/glm/ext/vector_bool3_precision.hpp"
    "vendor/glm/glm/ext/vector_bool4.hpp"
    "vendor/glm/glm/ext/vector_bool4_precision.hpp"
    "vendor/glm/glm/ext/vector_common.hpp"
    "vendor/glm/glm/ext/vector_common.inl"
    "vendor/glm/glm/ext/vector_double1.hpp"
    "vendor/glm/glm/ext/vector_double1_precision.hpp"
    "vendor/glm/glm/ext/vector_double2.hpp"
    "vendor/glm/glm/ext/vector_double2_precision.hpp"
    "vendor/glm/glm/ext/vector_double3.hpp"
    "vendor/glm/glm/ext/vector_double3_precision.hpp"
    "vendor/glm/glm/ext/vector_double4.hpp"
    "vendor/glm/glm/ext/vector_double4_precision.hpp"
    "vendor/glm/glm/ext/vector_float1.hpp"
    "vendor/glm/glm/ext/vector_float1_precision.hpp"
    "vendor/glm/glm/ext/vector_float2.hpp"
    "vendor/glm/glm/ext/vector_float2_precision.hpp"
    "vendor/glm/glm/ext/vector_float3.hpp"
    "vendor/glm/glm/ext/vector_float3_precision.hpp"
    "vendor/glm/glm/ext/vector_float4.hpp"
    "vendor/glm/glm/ext/vector_float4_precision.hpp"
    "vendor/glm/glm/ext/vector_int1.hpp"
    "vendor/glm/glm/ext/vector_int1_sized.hpp"
    "vendor/glm/glm/ext/vector_int2.hpp"
    "vendor/glm/glm/ext/vector_int2_sized.hpp"
    "vendor/glm/glm/ext/vector_int3.hpp"
    "vendor/glm/glm/ext/vector_int3_sized.hpp"
    "vendor/glm/glm/ext/vector_int4.hpp"
    "vendor/glm/glm/ext/vector_int4_sized.hpp"
    "vendor/glm/glm/ext/vector_integer.hpp"
    "vendor/glm/glm/ext/vector_integer.inl"
    "vendor/glm/glm/ext/vector_packing.hpp"
    "vendor/glm/glm/ext/vector_packing.inl"
    "vendor/glm/glm/ext/vector_reciprocal.hpp"
    "vendor/glm/glm/ext/vector_reciprocal.inl"
    "vendor/glm/glm/ext/vector_relational.hpp"
    "vendor/glm/glm/ext/vector_relational.inl"
    "vendor/glm/glm/ext/vector_uint1.hpp"
    "vendor/glm/glm/ext/vector_uint1_sized.hpp"
    "vendor/glm/glm/ext/vector_uint2.hpp"
    "vendor/glm/glm/ext/vector_uint2_sized.hpp"
    "vendor/glm/glm/ext/vector_uint3.hpp"
    "vendor/glm/glm/ext/vector_uint3_sized.hpp"
    "vendor/glm/glm/ext/vector_uint4.hpp"
    "vendor/glm/glm/ext/vector_uint4_sized.hpp"
    "vendor/glm/glm/ext/vector_ulp.hpp"
    "vendor/glm/glm/ext/vector_ulp.inl"
)
source_group("vendor\\glm\\glm\\ext" FILES ${vendor__glm__glm__ext})

set(vendor__glm__glm__gtc
    "vendor/glm/glm/gtc/bitfield.hpp"
    "vendor/glm/glm/gtc/bitfield.inl"
    "vendor/glm/glm/gtc/color_space.hpp"
    "vendor/glm/glm/gtc/color_space.inl"
    "vendor/glm/glm/gtc/constants.hpp"
    "vendor/glm/glm/gtc/constants.inl"
    "vendor/glm/glm/gtc/epsilon.hpp"
    "vendor/glm/glm/gtc/epsilon.inl"
    "vendor/glm/glm/gtc/integer.hpp"
    "vendor/glm/glm/gtc/integer.inl"
    "vendor/glm/glm/gtc/matrix_access.hpp"
    "vendor/glm/glm/gtc/matrix_access.inl"
    "vendor/glm/glm/gtc/matrix_integer.hpp"
    "vendor/glm/glm/gtc/matrix_inverse.hpp"
    "vendor/glm/glm/gtc/matrix_inverse.inl"
    "vendor/glm/glm/gtc/matrix_transform.hpp"
    "vendor/glm/glm/gtc/matrix_transform.inl"
    "vendor/glm/glm/gtc/noise.hpp"
    "vendor/glm/glm/gtc/noise.inl"
    "vendor/glm/glm/gtc/packing.hpp"
    "vendor/glm/glm/gtc/packing.inl"
    "vendor/glm/glm/gtc/quaternion.hpp"
    "vendor/glm/glm/gtc/quaternion.inl"
    "vendor/glm/glm/gtc/quaternion_simd.inl"
    "vendor/glm/glm/gtc/random.hpp"
    "vendor/glm/glm/gtc/random.inl"
    "vendor/glm/glm/gtc/reciprocal.hpp"
    "vendor/glm/glm/gtc/round.hpp"
    "vendor/glm/glm/gtc/round.inl"
    "vendor/glm/glm/gtc/type_aligned.hpp"
    "vendor/glm/glm/gtc/type_precision.hpp"
    "vendor/glm/glm/gtc/type_precision.inl"
    "vendor/glm/glm/gtc/type_ptr.hpp"
    "vendor/glm/glm/gtc/type_ptr.inl"
    "vendor/glm/glm/gtc/ulp.hpp"
    "vendor/glm/glm/gtc/ulp.inl"
    "vendor/glm/glm/gtc/vec1.hpp"
)
source_group("vendor\\glm\\glm\\gtc" FILES ${vendor__glm__glm__gtc})

set(vendor__glm__glm__gtx
    "vendor/glm/glm/gtx/associated_min_max.hpp"
    "vendor/glm/glm/gtx/associated_min_max.inl"
    "vendor/glm/glm/gtx/bit.hpp"
    "vendor/glm/glm/gtx/bit.inl"
    "vendor/glm/glm/gtx/closest_point.hpp"
    "vendor/glm/glm/gtx/closest_point.inl"
    "vendor/glm/glm/gtx/color_encoding.hpp"
    "vendor/glm/glm/gtx/color_encoding.inl"
    "vendor/glm/glm/gtx/color_space.hpp"
    "vendor/glm/glm/gtx/color_space.inl"
    "vendor/glm/glm/gtx/color_space_YCoCg.hpp"
    "vendor/glm/glm/gtx/color_space_YCoCg.inl"
    "vendor/glm/glm/gtx/common.hpp"
    "vendor/glm/glm/gtx/common.inl"
    "vendor/glm/glm/gtx/compatibility.hpp"
    "vendor/glm/glm/gtx/compatibility.inl"
    "vendor/glm/glm/gtx/component_wise.hpp"
    "vendor/glm/glm/gtx/component_wise.inl"
    "vendor/glm/glm/gtx/dual_quaternion.hpp"
    "vendor/glm/glm/gtx/dual_quaternion.inl"
    "vendor/glm/glm/gtx/easing.hpp"
    "vendor/glm/glm/gtx/easing.inl"
    "vendor/glm/glm/gtx/euler_angles.hpp"
    "vendor/glm/glm/gtx/euler_angles.inl"
    "vendor/glm/glm/gtx/extend.hpp"
    "vendor/glm/glm/gtx/extend.inl"
    "vendor/glm/glm/gtx/extended_min_max.hpp"
    "vendor/glm/glm/gtx/extended_min_max.inl"
    "vendor/glm/glm/gtx/exterior_product.hpp"
    "vendor/glm/glm/gtx/exterior_product.inl"
    "vendor/glm/glm/gtx/fast_exponential.hpp"
    "vendor/glm/glm/gtx/fast_exponential.inl"
    "vendor/glm/glm/gtx/fast_square_root.hpp"
    "vendor/glm/glm/gtx/fast_square_root.inl"
    "vendor/glm/glm/gtx/fast_trigonometry.hpp"
    "vendor/glm/glm/gtx/fast_trigonometry.inl"
    "vendor/glm/glm/gtx/float_notmalize.inl"
    "vendor/glm/glm/gtx/functions.hpp"
    "vendor/glm/glm/gtx/functions.inl"
    "vendor/glm/glm/gtx/gradient_paint.hpp"
    "vendor/glm/glm/gtx/gradient_paint.inl"
    "vendor/glm/glm/gtx/handed_coordinate_space.hpp"
    "vendor/glm/glm/gtx/handed_coordinate_space.inl"
    "vendor/glm/glm/gtx/hash.hpp"
    "vendor/glm/glm/gtx/hash.inl"
    "vendor/glm/glm/gtx/integer.hpp"
    "vendor/glm/glm/gtx/integer.inl"
    "vendor/glm/glm/gtx/intersect.hpp"
    "vendor/glm/glm/gtx/intersect.inl"
    "vendor/glm/glm/gtx/io.hpp"
    "vendor/glm/glm/gtx/io.inl"
    "vendor/glm/glm/gtx/log_base.hpp"
    "vendor/glm/glm/gtx/log_base.inl"
    "vendor/glm/glm/gtx/matrix_cross_product.hpp"
    "vendor/glm/glm/gtx/matrix_cross_product.inl"
    "vendor/glm/glm/gtx/matrix_decompose.hpp"
    "vendor/glm/glm/gtx/matrix_decompose.inl"
    "vendor/glm/glm/gtx/matrix_factorisation.hpp"
    "vendor/glm/glm/gtx/matrix_factorisation.inl"
    "vendor/glm/glm/gtx/matrix_interpolation.hpp"
    "vendor/glm/glm/gtx/matrix_interpolation.inl"
    "vendor/glm/glm/gtx/matrix_major_storage.hpp"
    "vendor/glm/glm/gtx/matrix_major_storage.inl"
    "vendor/glm/glm/gtx/matrix_operation.hpp"
    "vendor/glm/glm/gtx/matrix_operation.inl"
    "vendor/glm/glm/gtx/matrix_query.hpp"
    "vendor/glm/glm/gtx/matrix_query.inl"
    "vendor/glm/glm/gtx/matrix_transform_2d.hpp"
    "vendor/glm/glm/gtx/matrix_transform_2d.inl"
    "vendor/glm/glm/gtx/mixed_product.hpp"
    "vendor/glm/glm/gtx/mixed_product.inl"
    "vendor/glm/glm/gtx/norm.hpp"
    "vendor/glm/glm/gtx/norm.inl"
    "vendor/glm/glm/gtx/normal.hpp"
    "vendor/glm/glm/gtx/normal.inl"
    "vendor/glm/glm/gtx/normalize_dot.hpp"
    "vendor/glm/glm/gtx/normalize_dot.inl"
    "vendor/glm/glm/gtx/number_precision.hpp"
    "vendor/glm/glm/gtx/number_precision.inl"
    "vendor/glm/glm/gtx/optimum_pow.hpp"
    "vendor/glm/glm/gtx/optimum_pow.inl"
    "vendor/glm/glm/gtx/orthonormalize.hpp"
    "vendor/glm/glm/gtx/orthonormalize.inl"
    "vendor/glm/glm/gtx/pca.hpp"
    "vendor/glm/glm/gtx/pca.inl"
    "vendor/glm/glm/gtx/perpendicular.hpp"
    "vendor/glm/glm/gtx/perpendicular.inl"
    "vendor/glm/glm/gtx/polar_coordinates.hpp"
    "vendor/glm/glm/gtx/polar_coordinates.inl"
    "vendor/glm/glm/gtx/projection.hpp"
    "vendor/glm/glm/gtx/projection.inl"
    "vendor/glm/glm/gtx/quaternion.hpp"
    "vendor/glm/glm/gtx/quaternion.inl"
    "vendor/glm/glm/gtx/range.hpp"
    "vendor/glm/glm/gtx/raw_data.hpp"
    "vendor/glm/glm/gtx/raw_data.inl"
    "vendor/glm/glm/gtx/rotate_normalized_axis.hpp"
    "vendor/glm/glm/gtx/rotate_normalized_axis.inl"
    "vendor/glm/glm/gtx/rotate_vector.hpp"
    "vendor/glm/glm/gtx/rotate_vector.inl"
    "vendor/glm/glm/gtx/scalar_multiplication.hpp"
    "vendor/glm/glm/gtx/scalar_relational.hpp"
    "vendor/glm/glm/gtx/scalar_relational.inl"
    "vendor/glm/glm/gtx/spline.hpp"
    "vendor/glm/glm/gtx/spline.inl"
    "vendor/glm/glm/gtx/std_based_type.hpp"
    "vendor/glm/glm/gtx/std_based_type.inl"
    "vendor/glm/glm/gtx/string_cast.hpp"
    "vendor/glm/glm/gtx/string_cast.inl"
    "vendor/glm/glm/gtx/texture.hpp"
    "vendor/glm/glm/gtx/texture.inl"
    "vendor/glm/glm/gtx/transform.hpp"
    "vendor/glm/glm/gtx/transform.inl"
    "vendor/glm/glm/gtx/transform2.hpp"
    "vendor/glm/glm/gtx/transform2.inl"
    "vendor/glm/glm/gtx/type_aligned.hpp"
    "vendor/glm/glm/gtx/type_aligned.inl"
    "vendor/glm/glm/gtx/type_trait.hpp"
    "vendor/glm/glm/gtx/type_trait.inl"
    "vendor/glm/glm/gtx/vec_swizzle.hpp"
    "vendor/glm/glm/gtx/vector_angle.hpp"
    "vendor/glm/glm/gtx/vector_angle.inl"
    "vendor/glm/glm/gtx/vector_query.hpp"
    "vendor/glm/glm/gtx/vector_query.inl"
    "vendor/glm/glm/gtx/wrap.hpp"
    "vendor/glm/glm/gtx/wrap.inl"
)
source_group("vendor\\glm\\glm\\gtx" FILES ${vendor__glm__glm__gtx})

set(ALL_FILES
    ${src}
    ${src__Aria}
    ${src__Aria__Events}
    ${src__Aria__ImGui}
    ${src__Aria__Renderer}
    ${src__Platform__OpenGL}
    ${src__Platform__Windows}
    ${vendor__glm__glm}
    ${vendor__glm__glm__detail}
    ${vendor__glm__glm__ext}
    ${vendor__glm__glm__gtc}
    ${vendor__glm__glm__gtx}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/ariapch.h>"
)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Aria)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "Aria"
    TARGET_NAME_DIST    "Aria"
    TARGET_NAME_RELEASE "Aria"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Debug-windows-x86_64/Aria/"
    OUTPUT_DIRECTORY_DIST    "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Dist-windows-x86_64/Aria/"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/Release-windows-x86_64/Aria/"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    $<$<CONFIG:Debug>:
        MultiThreadedDebug
    >
    $<$<CONFIG:Dist>:
        MultiThreaded
    >
    $<$<CONFIG:Release>:
        MultiThreaded
    >
    $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Dist>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui;"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "ARIA_DEBUG"
    ">"
    "$<$<CONFIG:Dist>:"
        "ARIA_DIST"
    ">"
    "$<$<CONFIG:Release>:"
        "ARIA_RELEASE"
    ">"
    "ARIA_PLATFORM_WINDOWS;"
    "ARIA_BUILD_DLL;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Od;
            /ZI
        >
        $<$<CONFIG:Dist>:
            /Gm-;
            /Ox;
            /Oi;
            /GF;
            /Gy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        $<$<CONFIG:Release>:
            /Gm-;
            /Ox;
            /Oi;
            /GF;
            /Gy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /std:c++17;
        /W3;
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Dist>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:WINDOWS
    )
else()
  target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            -O0;
            -g;
            -Wall;
            -Wextra;
            -pedantic;
            -fno-inline-functions;
            -fno-omit-frame-pointer            
        >
        $<$<CONFIG:Dist>:
            -O2;
            -DNDEBUG;
            -fstack-protector-strong;
            -pie;
            -fvisibility=hidden;
            -fdata-sections;
            -ffunction-sections;
       >
        $<$<CONFIG:Release>:
            -O2;
            -g;
            -DNDEBUG;
            -fvisibility=hidden;
            -fdata-sections;
            -ffunction-sections;
        >
        -std=c++17;
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Dist>:
            /OPT:REF;
            /OPT:ICF
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF
        >
        /SUBSYSTEM:WINDOWS
    )
endif()



################################################################################
# Dependencies
################################################################################
# Link with other targets.
target_link_libraries(${PROJECT_NAME} PUBLIC
    GLFW
    Glad
    imgui
)

set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "opengl32"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")
