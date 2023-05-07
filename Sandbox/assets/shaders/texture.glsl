#type vertex
#version 330 core

// refers to the layout defined in our vertex attribute
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_position;
out vec2 v_texCoord;

void main() {
  v_position = a_Position;
  v_texCoord = a_TexCoord;
  gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform sampler2D u_Texture;

void main() { 
  color = texture(u_Texture, v_texCoord);
}
