#version 430

uniform sampler2D myTexture;

layout(location = 3) uniform vec3 lightLoc;

in vec2 fragUV;
in vec3 loc;
in vec3 cameraLoc;
in vec3 fragLoc;
in vec3 modelNorm;

void main()
{
	vec3 normelle = normalize(modelNorm);
	vec3 L = normalize(lightLoc - fragLoc);
	vec3 E = normalize(cameraLoc - fragLoc);
	vec3 H = normalize(L+E);

	float Ambient = .02f;

	float Diffuse = 1.0f * max(dot(normelle, L),0.0f);
	Diffuse = clamp(Diffuse, 0.0, 1.0);

	float Specular = 0.0f;

	if(Diffuse > 0)
	{
		Specular = 0.5f * pow(max(dot(modelNorm, H),0.0f),16.0f);
		Specular = clamp(Specular, 0.0, 1.0);
	}

	float Brightness = Ambient + Diffuse + Specular;

	vec4 FragColorBeta = texture(myTexture,fragUV);

	gl_FragColor = vec4(FragColorBeta.xyz * Brightness, FragColorBeta.w);
}