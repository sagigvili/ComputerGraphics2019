#version 330 core

struct Material
{
	vec4 AmbientColor;
	vec4 DiffuseColor;
	vec4 SpecualrColor;

	float KA;
	float KD;
	float KS;
	int KSE;
};

struct Fog
{
	int fogType;
	vec4 fogColor;
	float density;
	float start;
	float end;
};

// We set this field's properties from the C++ code
uniform Fog fog;
uniform Material material;

uniform vec4 modelColor;
uniform vec4 wirmframe_color;
uniform vec3 eye;
uniform bool wirmframe;

uniform vec4 SceneAmbient;
uniform vec4 lightPos[10];
uniform vec4 lightColor[10];
uniform int lightsCount;

// Inputs from vertex shader (after interpolation was applied)
in vec4 fragPos;
in vec4 fragNormal;


// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	vec4 AmbientColor = material.AmbientColor;
	vec4 DiffuseColor = material.DiffuseColor;
	vec4 SpecualrColor = material.SpecualrColor;

	vec3 N = normalize(fragNormal.xyz / fragNormal.w);
	vec3 V = normalize(eye - fragPos.xyz / fragPos.w);	//assuming camera is always at 0,0,0
	// ambient is only needed once
	vec4 ac = material.KA * AmbientColor;
	vec4 IA = clamp(ac*SceneAmbient, 0.0f, 1.0f);

	vec4 ID = vec4(0.0f);
	vec4 IS = vec4(0.0f);

	for (int i=0; i<lightsCount; i++) {
		vec4 lightColor = lightColor[i];
		vec3 pos = lightPos[i].xyz / lightPos[i].w;

		vec3 L = normalize(pos - (fragPos.xyz / fragPos.w));
		vec3 R = normalize(reflect(-L, N));

		float LN = max(dot(N, L), 0.0f);
		vec4 dc = material.KD * LN * DiffuseColor;
		ID = ID + clamp(dc*lightColor, 0.0f, 1.0f);

		float RV = max(dot(R, V), 0.0f);
		vec4 sc = material.KS * pow(RV,material.KSE) * SpecualrColor;
		IS = IS + clamp(sc*lightColor, 0.0f, 1.0f);

	}
	if (lightsCount != 0)
		frag_color = clamp(IA + ID + IS + modelColor, 0.0f, 1.0f);
	else
		frag_color = modelColor;

	// fog coloring
	float dist = abs(fragPos.z / fragPos.w);
	float fogFactor = 0.0f;
	// linear
	if (fog.fogType == 1) {
		fogFactor = clamp((fog.end-dist)/(fog.end-fog.start), 0.0f, 1.0f);
		frag_color = mix(fog.fogColor, frag_color, fogFactor);
	}
	else if (fog.fogType == 2) {	// exp
		fogFactor = clamp((1.0f/exp(dist * fog.density)), 0.0f, 1.0f);
		frag_color = mix(fog.fogColor, frag_color, fogFactor);
	}
	else if (fog.fogType == 3) {	//exp squared
		fogFactor = clamp(1.0 /exp((dist * fog.density) * (dist * fog.density)), 0.0f, 1.0f);
		frag_color = mix(fog.fogColor, frag_color, fogFactor);
	}

	if (wirmframe)
		frag_color = wirmframe_color;
}