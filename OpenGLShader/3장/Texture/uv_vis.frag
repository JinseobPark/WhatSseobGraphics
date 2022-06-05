#version 410

uniform sampler2D jordyTex; 
in vec2 fragUV;
out vec4 outCol;

void main()
{
	outCol = texture(jordyTex,fragUV);
}