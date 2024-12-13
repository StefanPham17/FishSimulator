#version 330

uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

uniform vec4 u_color;

uniform sampler2D u_texture_4;
uniform vec2 u_texture_4_size;

uniform float u_normal_scaling;
uniform float u_height_scaling;

in vec4 v_position;
in vec4 v_normal;
in vec4 v_tangent;
in vec2 v_uv;

out vec4 out_color;

float h(vec2 uv) {
  // You may want to use this helper function...
  return texture(u_texture_4, uv).r;
}

void main() {
  vec3 b = cross(v_normal.xyz, v_tangent.xyz);
  float dU = (h(vec2(v_uv.x + 1/u_texture_4_size.x, v_uv.y)) - h(v_uv)) * u_normal_scaling * u_height_scaling;
  float dV = (h(vec2(v_uv.x, v_uv.y + 1/u_texture_4_size.y)) - h(v_uv)) * u_normal_scaling * u_height_scaling;
  vec3 n_d = normalize(-1 * dU * v_tangent.xyz + -1 * dV * b + v_normal.xyz);

  float cosine1 = dot(n_d, normalize(u_light_pos));
  float cosine2 = dot(n_d, normalize(u_light_pos + v_position.xyz));
  out_color = vec4(normalize(u_light_intensity) * max(0, cosine1) + normalize(u_light_intensity) * 0.5 * pow(max(0, cosine2), 10), 1) * u_color;
}

