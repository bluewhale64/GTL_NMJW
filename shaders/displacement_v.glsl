#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 UV;
layout(location = 2) in vec2 texdisind;
uniform mat4 MVP;
out vec2 UV_coords;
out vec2 UV_dismap;
out flat int v_texnum;
out flat int v_disnum;
void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1.0);
    UV_coords = UV.xy;
    UV_dismap = UV.zw;
    v_texnum = int(texdisind.x);
    v_disnum = int(texdisind.y);
}