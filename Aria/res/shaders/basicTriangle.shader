#shader vertex
#version 330 core

// refers to the layout defined in our vertex attribute
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec3 v_position;
out vec4 v_color;

void main() {
  v_position = a_Position;
  v_color = a_Color;
  gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_pos;
in vec4 v_color;


void main() {
  //color = vec4(v_pos * 0.5 + 0.5, 1.0);
  color = v_color;
};
