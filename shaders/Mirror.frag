#version 330


uniform vec3 u_cam_pos;

uniform samplerCube u_texture_cubemap;

in vec4 v_position;
in vec4 v_normal;
in vec4 v_tangent;

out vec4 out_color;

void main() {
  // YOUR CODE HERE
  vec3 w_o = v_position.xyz - u_cam_pos;
  vec3 w_i = w_o - 2 * dot(v_normal.xyz, w_o) * v_normal.xyz;

  out_color = texture(u_texture_cubemap, w_i);
}
