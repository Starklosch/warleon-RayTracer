#version 330 core

uniform sampler2D tex;

out vec4 fragColor;

void main(){
	fragColor = texture(tex, gl_FragCoord.xy / vec2(800, -600));
}