#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 myMatrix;
uniform mat4 cameraMat;

out vec2 fragUV;
out vec3 loc;
//out vec3 lightLoc;
out vec3 cameraLoc;
out vec3 fragLoc;
out vec3 modelNorm;


void main()
{
	vec4 vertPos = myMatrix * vec4(position, 1.0f);
	loc = vec3(vertPos) / vertPos.w;
	fragUV = uv;

	vec3 lightLoc = vec3(0,1,2);

	mat4 normalMatrix = transpose(inverse(myMatrix));
	modelNorm = vec3(normalMatrix * vec4(normal, 0.0f));
	gl_Position = cameraMat * myMatrix * vec4(position, 1.0);

}