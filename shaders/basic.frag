#version 410 core

in vec4 fPosEye;
in vec3 fNormal;
in vec3 fPosition;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;

out vec4 fColor;

//lighting
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 lightPosition; 
uniform int controlPunctiforma;

//ceata
uniform vec3 ceata;
//matrici
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;


//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 25.0f;


void computeLightComponents()
{		
	//transform normal
	vec3 normalEye = normalize(normalMatrix*fNormal);	
	
	//compute light direction
	vec3 lightDirN = vec3(normalize(view * vec4(lightDir, 0.0f)));
	
	//compute view direction 
	vec3 viewDirN = normalize( - fPosEye.xyz);
		
	//compute ambient light
	ambient = ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), shininess);
	specular = specularStrength * specCoeff * lightColor;
}
uniform vec3 pointLightPos;
uniform vec3 pointLightPos1;
uniform vec3 pointLightPos2;
uniform vec3 pointLightPos3;
uniform vec3 pointLightPos4;
uniform vec3 pointLightPos5;
uniform vec3 pointLightPos6;
uniform vec3 pointLightPos7;
uniform vec3 pointLightPos8;

uniform vec3 pointLightColor;
uniform vec3 pointLightColor1;
uniform vec3 pointLightColor2;
uniform vec3 pointLightColor3;
uniform vec3 pointLightColor4;
uniform vec3 pointLightColor5;
uniform vec3 pointLightColor6;
uniform vec3 pointLightColor7;
uniform vec3 pointLightColor8;

uniform float pointLightConstant;
uniform float pointLightLinear;
uniform float pointLightQuadratic;

void computePointLight()
{
    vec3 normalEye = normalize(normalMatrix * fNormal);

    vec3 lightDir2 = normalize(pointLightPos - fPosition);
	float distance2 = length(pointLightPos - fPosition);
	float attenuation2 = 1.0 / (pointLightConstant + pointLightLinear * distance2 + pointLightQuadratic * distance2 * distance2);
	ambient += ambientStrength * pointLightColor * attenuation2;
    diffuse += max(dot(normalEye, lightDir2), 0.0f) * pointLightColor * attenuation2;
    diffuse += max(dot(normalEye, lightDir2), 0.0f) * pointLightColor * attenuation2;
    specular += specularStrength * pow(max(dot(normalEye, reflect(-lightDir2, normalEye)), 0.0f), 15) * pointLightColor * attenuation2;


	vec3 lightDir3 = normalize(pointLightPos1 - fPosition);
    float distance3 = length(pointLightPos1 - fPosition);
    float attenuation3 = 1.0 / (pointLightConstant + pointLightLinear * distance3 + pointLightQuadratic * distance3 * distance3);
	ambient += ambientStrength * pointLightColor1 * attenuation3;
	diffuse += max(dot(normalEye, lightDir3), 0.0f) * pointLightColor1 * attenuation3;
    diffuse += max(dot(normalEye, lightDir3), 0.0f) * pointLightColor1 * attenuation3;
    specular += specularStrength * pow(max(dot(normalEye, reflect(-lightDir3, normalEye)), 0.0f), 15) * pointLightColor1 * attenuation3;

	vec3 lightDir4 = normalize(pointLightPos2 - fPosition);
    float distance4 = length(pointLightPos2 - fPosition);
    float attenuation4 = 1.0 / (pointLightConstant + pointLightLinear * distance4 + pointLightQuadratic * distance4 * distance4);
	ambient += ambientStrength * pointLightColor2 * attenuation4;
	diffuse += max(dot(normalEye, lightDir4), 0.0f) * pointLightColor2 * attenuation4;
    diffuse += max(dot(normalEye, lightDir4), 0.0f) * pointLightColor2 * attenuation4;
    specular += specularStrength * pow(max(dot(normalEye, reflect(-lightDir4, normalEye)), 0.0f), 15) * pointLightColor2 * attenuation4;

	vec3 lightDir5 = normalize(pointLightPos3 - fPosition);
    float distance5 = length(pointLightPos3 - fPosition);
    float attenuation5 = 1.0 / (pointLightConstant + pointLightLinear * distance5 + pointLightQuadratic * distance5 * distance5);
	ambient += ambientStrength * pointLightColor3 * attenuation5;
	diffuse += max(dot(normalEye, lightDir5), 0.0f) * pointLightColor3 * attenuation5;
    diffuse += max(dot(normalEye, lightDir5), 0.0f) * pointLightColor3 * attenuation5;
    specular += specularStrength * pow(max(dot(normalEye, reflect(-lightDir5, normalEye)), 0.0f), 15) * pointLightColor3 * attenuation5;

	vec3 lightDir6 = normalize(pointLightPos4 - fPosition);
    float distance6 = length(pointLightPos4 - fPosition);
    float attenuation6 = 1.0 / (pointLightConstant + pointLightLinear * distance6 + pointLightQuadratic * distance6 * distance6);
	ambient += ambientStrength * pointLightColor4 * attenuation6;
	diffuse += max(dot(normalEye, lightDir6), 0.0f) * pointLightColor4 * attenuation6;
    diffuse += max(dot(normalEye, lightDir6), 0.0f) * pointLightColor4 * attenuation6;
    specular += specularStrength * pow(max(dot(normalEye, reflect(-lightDir6, normalEye)), 0.0f), 15) * pointLightColor4 * attenuation6;

	vec3 lightDir7 = normalize(pointLightPos5 - fPosition);
    float distance7 = length(pointLightPos5 - fPosition);
    float attenuation7 = 1.0 / (pointLightConstant + pointLightLinear * distance7 + pointLightQuadratic * distance7 * distance7);
	ambient += ambientStrength * pointLightColor5 * attenuation7;
	diffuse += max(dot(normalEye, lightDir7), 0.0f) * pointLightColor5 * attenuation7;
    diffuse += max(dot(normalEye, lightDir7), 0.0f) * pointLightColor5 * attenuation7;
    specular += specularStrength * pow(max(dot(normalEye, reflect(-lightDir7, normalEye)), 0.0f), 15) * pointLightColor5 * attenuation7;

	vec3 lightDir8 = normalize(pointLightPos6 - fPosition);
    float distance8 = length(pointLightPos6 - fPosition);
    float attenuation8 = 1.0 / (pointLightConstant + pointLightLinear * distance8 + pointLightQuadratic * distance8 * distance8);
	ambient += ambientStrength * pointLightColor6 * attenuation8;
	diffuse += max(dot(normalEye, lightDir8), 0.0f) * pointLightColor6 * attenuation8;
    diffuse += max(dot(normalEye, lightDir8), 0.0f) * pointLightColor6 * attenuation8;
    specular += specularStrength * pow(max(dot(normalEye, reflect(-lightDir8, normalEye)), 0.0f), 15) * pointLightColor6 * attenuation8;

	vec3 lightDir9 = normalize(pointLightPos7 - fPosition);
    float distance9 = length(pointLightPos7 - fPosition);
    float attenuation9 = 1.0 / (pointLightConstant + pointLightLinear * distance9 + pointLightQuadratic * distance9 * distance9);
	ambient += ambientStrength * pointLightColor7 * attenuation9;
	diffuse += max(dot(normalEye, lightDir9), 0.0f) * pointLightColor7 * attenuation9;
    diffuse += max(dot(normalEye, lightDir9), 0.0f) * pointLightColor7 * attenuation9;
    specular += specularStrength * pow(max(dot(normalEye, reflect(-lightDir9, normalEye)), 0.0f), 15) * pointLightColor7 * attenuation9;

	vec3 lightDir10 = normalize(pointLightPos8 - fPosition);
    float distance10 = length(pointLightPos8 - fPosition);
    float attenuation10 = 1.0 / (pointLightConstant + pointLightLinear * distance10 + pointLightQuadratic * distance10 * distance10);
	ambient += ambientStrength * pointLightColor8 * attenuation10;
	diffuse += max(dot(normalEye, lightDir10), 0.0f) * pointLightColor8 * attenuation10;
    diffuse += max(dot(normalEye, lightDir10), 0.0f) * pointLightColor8 * attenuation10;
    specular += specularStrength * pow(max(dot(normalEye, reflect(-lightDir10, normalEye)), 0.0f), 15) * pointLightColor8 * attenuation10;
   
   
}


float creareCeata()
{
	float fragmentDistance = length(fPosEye);
	float ceataFactor = exp(-pow(fragmentDistance * ceata.x, 2));
	return clamp(ceataFactor, 0.0f, 1.0f);
}

float computeShadow()
{

	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	normalizedCoords = normalizedCoords * 0.5 + 0.5;
	if (normalizedCoords.z > 1.0f) return 0.0f;
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;
	float currentDepth = normalizedCoords.z;
	float bias = max(0.05f * (1.0f - dot(fNormal,lightDir)), 0.05f);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;	
	if (normalizedCoords.z > 1.0f)
		return 0.0f;
	return shadow;
}

void main() 
{	
	vec4 colorFromTexture= texture(diffuseTexture, fTexCoords);

	if(colorFromTexture.a < 0.1f)
		discard;

	ambient = vec3(0.0f);
	diffuse = vec3(0.0f);
	specular = vec3(0.0f);
	computeLightComponents();
	computePointLight();
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;
	

   vec3 color = min((ambient + diffuse) + specular , 1.0f);


	float shadow = computeShadow();
	color = min((ambient + (1.0f - shadow)*diffuse) + (1.0f - shadow)*specular, 1.0f);

	float fogFact = creareCeata();
	vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vec4 specialColorForFog = vec4(color, 0.0f);
	fColor = mix(fogColor,specialColorForFog, fogFact);
    //fColor = vec4(color, 1.0f);
}
