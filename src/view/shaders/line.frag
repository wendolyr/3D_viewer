#version 410 core
in vec3 g_position;
flat in vec3 start;
flat in vec3 end;
in vec2 screen_pos;
flat in vec2 screen_start;
flat in vec2 screen_end;
uniform vec3 u_color;
uniform float u_dash_size;
uniform float u_gap_size;
uniform bool use_dashing;
out vec4 fragColor;
void main() {
  if (use_dashing) {
    vec2 dir = normalize(screen_end - screen_start);
    float pos = dot(screen_pos - screen_start, dir);
    float cycle = u_dash_size + u_gap_size;
    if (fract(pos / cycle) > (u_dash_size / cycle)) {
      discard;
    }
  }
  fragColor = vec4(u_color, 1.0);
}