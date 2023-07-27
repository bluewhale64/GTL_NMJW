#version 330 core
in vec2 UV_coords;
flat in int v_texnum;
out vec4 colour;
uniform sampler2D u_texarray[8];
void main(){
  if(v_texnum == 0){
    colour = texture(u_texarray[0], UV_coords);
  }
  if(v_texnum == 1){
    colour = texture(u_texarray[1], UV_coords);
  }
  if(v_texnum == 2){
    colour = texture(u_texarray[2], UV_coords);
  }
  if(v_texnum == 3){
    colour = texture(u_texarray[3], UV_coords);
  }
  if(v_texnum == 4){
    colour = texture(u_texarray[4], UV_coords);
  }
  if(v_texnum == 5){
    colour = texture(u_texarray[5], UV_coords);
  }
  if(v_texnum == 6){
    colour = texture(u_texarray[6], UV_coords);
  }
  if(v_texnum == 7){
    colour = texture(u_texarray[7], UV_coords);
  }
  if(v_texnum < 0 || v_texnum > 7){
    colour = vec4(0.25f, 0.25f, 0.25f, 0.25f);
  }
}