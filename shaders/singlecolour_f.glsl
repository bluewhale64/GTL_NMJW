#version 450 core
in vec4 fragmentColour;
out vec4 colour;
void main(){
  colour = fragmentColour;
}