#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;
layout(location = 3) in vec2 a_uv;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_model;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_worldPosition;
out vec2 v_uv;
out vec3 v_color;

void main()
{
	mat4 model = u_model;
	v_position = a_position;
	v_normal = (model * vec4(a_normal, 0.0)).xyz;
	v_worldPosition = (model * vec4(a_position, 1.0)).xyz;
	v_uv = a_uv;
	v_color = a_color;

	gl_Position = u_viewProjectionMatrix * model * vec4(a_position, 1.0);
};

#shader fragment
#version 330 core

vec3 specularBRDF(float roughness, vec3 F0, float NdotH, float NdotV, float NdotL, float LdotH);
float GeometrySmith(float NdotV, float NdotL, float roughness);
vec3 FresnelSchlick(float VdotH, vec3 F0);
float DistributionGGX(float NdotH, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);

in vec3 v_position;
in vec3 v_normal;
in vec3 v_worldPosition;
in vec2 v_uv;
in vec3 v_color;

layout(location = 0) out vec4 outColor;
layout(location = 1) out int idColor;

uniform sampler2D u_texture;

uniform vec3 u_lightPosition;
uniform vec3 u_lightColor;
uniform vec4 u_color;
uniform vec3 u_cameraPosition;
uniform float u_maxLightDistance;
uniform float u_lightIntensity;
uniform int u_entityID;
uniform float u_metalness;
uniform float u_roughness;

void main()
{
	vec3 L				= u_lightPosition - v_worldPosition;
	float lightDistance = length(L);
	L					= normalize(L);

	vec3 V				= normalize(u_cameraPosition - v_worldPosition);
	vec3 H				= normalize(V + L);
	vec3 N				= normalize(v_normal);
	vec2 uv				= v_uv;

	// Compute all necessary dot product
	float NdotL = clamp(dot(N, L), 0.0, 1.0);
	float NdotV = clamp(dot(N, V), 0.0, 1.0);
	float NdotH = clamp(dot(N, H), 0.0, 1.0);
	float VdotH = clamp(dot(V, H), 0.0, 1.0);

	float attenuation	= u_maxLightDistance - lightDistance;
	attenuation			/= u_maxLightDistance;
	attenuation			= max(attenuation, 0.0);
	attenuation			= attenuation * attenuation;

	vec3 textureColor = texture(u_texture, uv).xyz;
	vec3 baseColor = v_color * u_color.xyz * textureColor;

	vec3 light = u_lightColor * u_lightIntensity * attenuation;

	// We compute the ratio of diffuse and specular light based on the metalness of the material.
	// A pure metal reflects all incoming light, thus it has no diffuse light (kd == 0).
	float kd = (1.0 - u_metalness);
	float ks = 1.0 - kd;

	// We compute the Lambertian diffuse equation
	vec3 diffuse = kd * NdotL * baseColor * light;

	// We compute the reflection factor F0 taking into account the base colour and metalness
	vec3 F0 = mix(vec3(0.5), baseColor, u_metalness);

	// Compute the Cook-Torrance specular
	vec3 specular = ks * specularBRDF(u_roughness, F0, NdotH, NdotV, NdotL, VdotH);

	/*outColor = vec4(diffuse, 1.0) + vec4(specular, 1.0);
	if (DistributionGGX(NdotH, u_roughness) > 0.0)
		outColor += vec4(1, 0, 0, 1);
	vec3 s = FresnelSchlick(VdotH, F0);
	if (s.x < 0.0 && s.y < 0.0 && s.z < 0.0)
		outColor += vec4(0, 1, 0, 1);*/

	//if (GeometrySmith(NdotV, NdotL, 0.0) > 0.90)
	//	outColor += vec4(0, 0, 1, 1);
	//else
	//	outColor += vec4(1, 0, 0, 1);

	//if (GeometrySchlickGGX(NdotV, u_roughness) == 0.0)
	//	outColor += vec4(0, 0, 1, 1);

	if (u_roughness == 0.0)
		outColor == vec4(1, 0, 0, 1);
	if (u_roughness < 0.00001)
		outColor == vec4(0, 1, 0, 1);
	if (u_roughness > 0.5)
		outColor == vec4(0, 0, 1, 1);
	/*if (specular.x > 0.0 && specular.y > 0.0 && specular.z > 0.0)
		outColor = vec4(1, 0, 0, 1);
	else
		outColor = vec4(0, 1, 0, 1);*/

	idColor = u_entityID;
};

/*
* PBR functions
* -------------
* From now on, all functions below are used to calculate the Physically-Based Rendering
* 
* The reflectance Equation is as follows:
* Lo(p, wo) = Integral fr(p, wi, wo) * Li(p, wi) * dor(N, wi) * dW;
* 
* Cook-Torrance BRDF
* fr = kd * flamber + ks * fcook-torrance where
* 
* kd: ratio of incoming light energy that gets refracted
* ks: ratio of light getting reflected
* 
* The lambertian diffuse used is defined as follows:
* flambert = c / pi where c is the albedo or surface colour
* 
* The specular part of the BRDF is the Cook-Torrance specular defined as follows:
* fcook-torrance = DFG / (4 * dot(wo, N) * dot(wi, N))
* 
* D: Normal Distribution Function: approximates the amount of surface's microfacets are aligned to the halfway vector.
* F: Fresnel Equation: describes the ratio of surface reflection.
* G: Geometry Function: describes the self-shadowing property of microfacets.
*/

const float PI = 3.1415926535897932384626433832795;

/*
* Normal Distribution Function
*/

float DistributionGGX(float NdotH, float roughness)
{
	float a = roughness * roughness;
	float NdotH2 = NdotH * NdotH;

	float nom = a;
	float denom = (NdotH2 * (a - 1.0) + 1.0);
	denom = PI * denom * denom;
	
	return nom / denom;
}

/*
* Fresnel Equation using the Fresnel-Schlick approximation
*/

vec3 FresnelSchlick(float VdotH, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
}

/*
* Geometry Schlick Function GGX
*/

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float nom = NdotV;
	float denom = NdotV * (1.0 - roughness) + roughness;

	return nom / denom;
}

/*
* Smith method is used since we have to take into account the view direction (geometry obstruction) and
* the light direction (geometry shadowing)
*/

float GeometrySmith(float NdotV, float NdotL, float roughness)
{
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
	/*float k = pow(roughness + 1.0, 2.0) / 8.0;
	float ggx1 = GeometrySchlickGGX(NdotL, k);
	float ggx2 = GeometrySchlickGGX(NdotV, k);

	return ggx1 * ggx2;*/
}

/*
* Compute specular BRDF
*/

vec3 specularBRDF(float roughness, vec3 F0, float NdotH, float NdotV, float NdotL, float LdotH)
{
	// Normal Distribution Function
	float D = DistributionGGX(NdotH, roughness);

	// Fresnel Equation
	vec3 F = FresnelSchlick(LdotH, F0);

	// Geometry Function
	float G = GeometrySmith(NdotV, NdotL, roughness);

	vec3 specular = D * F * G;
	specular /= (4.0 * NdotL * NdotV);

	return specular;
}