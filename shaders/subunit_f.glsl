#version 450 core
in vec2 UV_coords;
flat in vec4 bounds;
out vec4 colour;
uniform sampler2D u_texture;
void main(){
    vec2 size = vec2(bounds.z - bounds.x, bounds.w - bounds.y);
    vec2 coords = mod(UV_coords * size, size) + bounds.xy;
    colour = texture(u_texture, coords);
}