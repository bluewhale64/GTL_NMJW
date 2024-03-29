#version 330 core
in vec2 UV_coords;
in flat int v_texnum;
out vec4 colour;
uniform sampler2D u_texarray[32];
void main(){
  colour = texture(u_texarray[v_texnum], UV_coords);
}