#version 330

uniform vec4 u_color;
uniform vec3 u_cam_pos;
uniform vec3 u_light_pos;
uniform vec3 u_light_intensity;

in vec4 v_position;
in vec4 v_normal;
in vec2 v_uv;

out vec4 out_color;

void main() {
  // YOUR CODE HERE
  
  // (Placeholder code. You will want to replace it.)

  float cosine1 = dot(v_normal, normalize(vec4(u_light_pos, 0)));
  float cosine2 = dot(v_normal, normalize(vec4(u_light_pos, 0) + v_position));
  out_color = vec4(0.12 + normalize(u_light_intensity) * max(0, cosine1) + normalize(u_light_intensity) * pow(max(0, cosine2), 10), 1) * u_color;
}

