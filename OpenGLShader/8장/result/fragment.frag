#version 410

uniform vec3 lightDir; 
uniform vec3 lightCol;
uniform vec3 meshCol;
uniform vec3 cameraPos;

in vec3 fragNrm;
in vec3 fragWorldPos;


out vec4 outCol;

void main(){
	vec3 normal = normalize(fragNrm);

	float lightAmt = max(0.2, dot(normal, lightDir));
	vec3 fragLight = lightCol * lightAmt;

	outCol = vec4(meshCol * fragLight, 1.0);
}

	vec3 toCam = normalize(cameraPos - fragWorldPos);

	float rimAmt = 1.0 - max(0.0, dot(normal, toCam));
	rimAmt = pow(rimAmt, 2);