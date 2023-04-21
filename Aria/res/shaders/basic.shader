#shader vertex
#version 330 core

// refers to the layout defined in our vertex attribute
layout(location = 0) in vec3 position;

//out vec2 v_TexCoord;

//uniform mat4 u_MVP;  // model view project matrix

void main() {
  // https://docs.gl/sl4/gl_Position
  gl_Position = vec4(position, 1.0);
  //gl_Position = u_MVP * position;
  //v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

//in vec2 v_TexCoord;

//uniform vec4 u_Color;
//uniform sampler2D u_Texture;

void main() {
  //vec4 texColor = texture(u_Texture, v_TexCoord);
  color = vec4(0.8, 0.2, 0.3, 1.0);
};