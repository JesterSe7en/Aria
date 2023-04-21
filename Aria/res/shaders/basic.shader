#shader vertex
#version 330 core

// refers to the layout defined in our vertex attribute
layout(location = 0) in vec3 position;

out vec3 v_pos;

//uniform mat4 u_MVP;  // model view project matrix

void main() {
  // https://docs.gl/sl4/gl_Position
  gl_Position = vec4(position, 1.0);
  v_pos = position;
  //gl_Position = u_MVP * position;
  //v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_pos;

//uniform vec4 u_Color;
//uniform sampler2D u_Texture;

void main() {
  //vec4 texColor = texture(u_Texture, v_TexCoord);
  color = vec4(v_pos * 0.5 + 0.5, 1.0);
};