#version 460 core

#define POINT_LIGHT_NUMBER 1

out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;
uniform sampler2D specular_mask_sampler;

uniform vec3 lightPosition;
uniform vec3 targetDirection;

uniform vec3 ambientColor;

uniform float shiness;

//相机位置
uniform vec3 cameraPosition;

struct DirectionalLight
{
	vec3 direction;
	vec3 color;
	float specularIntensity;
};

struct PointLight
{
	vec3 position;
	vec3 color;
	float specularIntensity;

	float k1;
	float k2; 
	float kc; 
};

struct SpotLight
{
	vec3 position;
	vec3 targetDirection;
	vec3 color;
	float outerLine;
	float innerLine;
	float specularIntensity;
};


uniform SpotLight spotLight;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[POINT_LIGHT_NUMBER];

//计算diffuse(漫反射)相关数据
vec3 calculateDiffuse(vec3 lightColor, vec3 objectColor, vec3 lightDirN, vec3 normalN)
{
	float diffuse= clamp(dot(-lightDirN,normalN),0.0,1.0);
	vec3 diffuseColor = lightColor* diffuse* objectColor;

	return diffuseColor;
}

//计算specular(镜面反射)
vec3 calculateSpecular(vec3 lightColor, vec3 lightDirN, vec3 normalN, vec3 viewDir, float intensity)
{
	//1 防止背面光的照入
	float dotResult= dot(-lightDirN,normalN);
	float flag=step(0.0,dotResult);
	vec3 lightReflect = normalize(reflect(lightDirN,normalN));

	//2 计算specular
	float specular=clamp(dot(lightReflect,-viewDir),0.0,1.0);
	
	//3 控制光斑大小
	specular = pow(specular,shiness);

	//float specularMask= texture(specular_mask_sampler,uv).r;
	//vec3 specularColor=spotLight.color*specular*flag*spotLight.specularIntensity*specularMask;

	//4 计算最终颜色
	vec3 specularColor=lightColor*specular*flag*intensity;

	return specularColor;
}

//探照光
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
	//计算光照的通用数据
	vec3 ObjColor = texture(sampler,uv).xyz;
	vec3 lightDir=normalize(worldPosition-light.position);
	vec3 targetDir = normalize(light.targetDirection);

	//计算spotlight的照射范围
	float cGamma = dot(lightDir,targetDir);
	float intensity = clamp((cGamma-light.outerLine)/(light.innerLine-light.outerLine),0.0,1.0);

	//1 计算diffuse(漫反射)
	vec3 diffuse = calculateDiffuse(light.color,ObjColor,lightDir,normal);
	
	//2 计算specular(镜面反射)
	vec3 specular = calculateSpecular(light.color,lightDir,normal,viewDir,light.specularIntensity);

	return (diffuse+specular)* intensity;
}

//平行光
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
	vec3 ObjColor = texture(sampler,uv).xyz;
	vec3 lightDir=normalize(light.direction);

	//1 计算diffuse(漫反射)
	vec3 diffuse = calculateDiffuse(light.color,ObjColor,lightDir,normal);
	
	//2 计算specular(镜面反射)
	vec3 specular = calculateSpecular(light.color,lightDir,normal,viewDir,light.specularIntensity);
	
	return (diffuse+specular);
}

//点光源
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 ObjColor = texture(sampler,uv).xyz;
	vec3 lightDir = normalize(worldPosition-light.position);

	//计算衰减值
	float dist = length(worldPosition - light.position);
	float attenuation = 1.0/(light.k2*dist*dist+light.k1*dist+light.kc);

	//1 计算diffuse(漫反射)
	vec3 diffuse = calculateDiffuse(light.color,ObjColor,lightDir,normal);
	
	//2 计算specular(镜面反射)
	vec3 specular = calculateSpecular(light.color,lightDir,normal,viewDir,light.specularIntensity);
	
	return (diffuse + specular) * attenuation;
}

void main()
{
	vec3 result =  vec3(0.0);

	//计算光照的通用数据
	vec3 ObjColor = texture(sampler,uv).xyz;
	vec3 normalN=normalize(normal);
	vec3 viewDir=normalize(worldPosition-cameraPosition);

	result +=calculateSpotLight(spotLight,normalN,viewDir);
	result +=calculateDirectionalLight(directionalLight,normalN,viewDir);

	for(int i=0;i<POINT_LIGHT_NUMBER;i++)
	{
		result +=calculatePointLight(pointLights[i],normalN,viewDir);
	}

	//环境光计算
	vec3 ambient=ObjColor*ambientColor;
	vec3 finalColor = result +ambient;

	FragColor = vec4(finalColor,1.0);
}