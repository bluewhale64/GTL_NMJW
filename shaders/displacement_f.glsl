#version 330 core
in vec2 UV_coords;
in vec2 UV_dismap;
in flat int v_texnum;
in flat int v_disnum;
out vec4 colour;
uniform sampler2D u_texarray[32];
void main(){
  vec4 dmp = texture(u_texarray[v_disnum], UV_dismap);
  vec2 disp = vec2(dmp.r, dmp.g);
  colour = texture(u_texarray[int(v_texnum)], (UV_coords + disp));
}