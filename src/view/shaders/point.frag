#version 410 core
uniform vec3 u_color;
uniform int u_vertex_display;
out vec4 fragColor;
void main() {
  if (u_vertex_display == 1) {
    vec2 coord = gl_PointCoord.xy - vec2(0.5);
    if (dot(coord, coord) > 0.25) discard;
  }
  fragColor = vec4(u_color, 1.0);
}