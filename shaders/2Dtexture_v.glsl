#version 330 core
layout(location = 0) in vec2 vertexPosition_modelspace;
layout(location = 1) in vec2 UV_vertex;
layout(location = 2) in float texnum;
uniform mat4 MVP;
out vec2 UV_coords;
out flat int v_texnum;
void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace,0,1);
    UV_coords = UV_vertex;
    v_texnum = int(texnum);
}