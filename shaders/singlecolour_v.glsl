#version 450 core
layout(location = 0) in vec3 vertexPosition_modelspace;
uniform vec4 u_colour;
uniform mat4 MVP;
out vec4 fragmentColour;
void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    fragmentColour = u_colour;
}