#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_color;
layout(location = 3) in vec2 a_uv;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_model;
uniform vec3 u_cameraPosition;

out vec3 v_normal;
out vec3 v_worldPosition;
out vec2 v_uv;
out vec3 v_color;
out float v_angle;

void main()
{
	mat4 model = u_model;
	// TODO matrix inverse is a costly operation, should be performed in CPU.
	v_normal = mat3(transpose(inverse(model))) * a_normal;
	v_worldPosition = (model * vec4(a_position, 1.0)).xyz;
	v_uv = a_uv;
	v_color = a_color;

	gl_Position = u_viewProjectionMatrix * model * vec4(a_position, 1.0);

	// Calculating the cosine between the camera vector to the shaded point and the normal vector
	vec3 V = normalize(u_cameraPosition - v_worldPosition);
	vec3 N = normalize(v_normal);
	v_angle = dot(V, N);
};

#shader fragment
#version 330 core

float DistributionGGX(float NdotH);
float GeometrySmith(float NdotV, float NdotL);
vec3 FresnelSchlick(float VdotH, vec3 F0);

in vec3 v_normal;
in vec3 v_worldPosition;
in vec2 v_uv;
in vec3 v_color;
in float v_angle;

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

const float PI = 3.1415926535897932384626433832795;

void main()
{
	vec3 L				= u_lightPosition - v_worldPosition;
	float lightDistance = length(L);
	L					= normalize(L);

	vec3 V				= normalize(u_cameraPosition - v_worldPosition);
	vec3 N				= normalize(v_normal);
	vec3 H				= normalize(V + L);
	vec2 uv				= v_uv;

	// Compute all necessary dot product
	float NdotL = clamp(dot(N, L), 0.0, 1.0);
	float NdotV = clamp(dot(N, V), 0.0, 1.0);
	float NdotH = clamp(dot(N, H), 0.0, 1.0);
	float HdotV = clamp(dot(H, V), 0.0, 1.0);

	float attenuation	= u_maxLightDistance - lightDistance;
	attenuation			/= u_maxLightDistance;
	attenuation			= max(attenuation, 0.0);
	attenuation			= attenuation * attenuation;

	vec3 textureColor = texture(u_texture, uv).xyz;
	vec3 baseColor = v_color * u_color.xyz * textureColor;

	vec3 light = u_lightColor * u_lightIntensity * attenuation;

	// We compute the reflection factor F0 taking into account the base colour and metalness
	vec3 F0 = mix(vec3(0.04), baseColor, u_metalness);

	// Normal Distribution Function
	float D = DistributionGGX(NdotH);

	// Fresnel Equation
	vec3 F = FresnelSchlick(HdotV, F0);

	// Geometry Function
	float G = GeometrySmith(NdotV, NdotL);

	// Compute the Cook-Torrance specular
	vec3 specular = D * F * G;
	specular /= max(4.0 * NdotL * NdotV, 0.0001);

	// We compute the ratio of diffuse and specular light based on the metalness of the material.
	// A pure metal reflects all incoming light, thus it has no diffuse light (kd == 0).
	vec3 ks = F;
	vec3 kd = vec3(1.0) - F;
	kd *= 1.0 - u_metalness;

	// We compute the Lambertian diffuse equation
	vec3 diffuse = ( kd * baseColor ) / PI;

	vec3 direct = (diffuse + specular) * light * NdotL;
	// TODO ambient should be done based on IBL
	vec3 ambient = vec3(0.1) * baseColor;

	vec4 shadingColor = vec4(direct + ambient, 1);
	vec4 highlightColor = vec4(1, 0, 0, 1);
	//outColor = (1.0f - v_angle) * vec4(1, 0, 0, 1) + v_angle * shadingColor;
	outColor = shadingColor;
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


/*
* Normal Distribution Function
*/

float DistributionGGX(float NdotH)
{
	float a = u_roughness * u_roughness;
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

float GeometrySchlickGGX(float NdotV, float k)
{
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

/*
* Smith method is used since we have to take into account the view direction (geometry obstruction) and
* the light direction (geometry shadowing)
*/

float GeometrySmith(float NdotV, float NdotL)
{

	float r = u_roughness + 1.0;
	float k = (r * r) / 8.0;
	float ggx1 = GeometrySchlickGGX(NdotV, k);
	float ggx2 = GeometrySchlickGGX(NdotL, k);

	return ggx1 * ggx2;
}

/*
* Compute specular BRDF
*/
//
//vec3 specularBRDF(float roughness, vec3 F0, float NdotH, float NdotV, float NdotL, float HdotV)
//{
//	// Normal Distribution Function
//	float D = DistributionGGX(NdotH, roughness);
//
//	// Fresnel Equation
//	vec3 F = FresnelSchlick(HdotV, F0);
//
//	// Geometry Function
//	float G = GeometrySmith(NdotV, NdotL, roughness);
//
//	vec3 specular = D * F * G;
//	specular /= max(4.0 * NdotL * NdotV, 0.0001);
//
//	return specular;
//}