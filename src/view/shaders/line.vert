#version 410 core
layout(location = 0) in vec3 a_position;
out vec3 v_position;
uniform mat4 u_mvp;
void main() {
  v_position = a_position;
  gl_Position = u_mvp * vec4(a_position, 1.0);
}