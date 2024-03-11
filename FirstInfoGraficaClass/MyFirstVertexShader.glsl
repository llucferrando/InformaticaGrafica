#version 440 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 aleatorio;

uniform vec2 offset;

void main(){
	
	gl_Position = vec4(position + offset, 0.0, 1.0);
	
}