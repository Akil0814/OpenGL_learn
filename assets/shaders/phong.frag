#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;

uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform vec3 ambientColor;


//相机位置
uniform vec3 cameraPosition;
uniform float specularIntensity;


void main()
{
	//计算光照的通用数据
	vec3 ObjColor = texture(sampler,uv).xyz;
	vec3 normalN=normalize(normal);
	vec3 lightDirN=normalize(lightDirection);
	vec3 viewDir=normalize(worldPosition-cameraPosition);

	//计算diffuse(漫反射)相关数据
	float diffuse= clamp(dot(-lightDirN,normalN),0.0,1.0);
	vec3 diffuseColor = lightColor* diffuse* ObjColor;

	//计算specular
	//防止背面光的照入
	float dotResult= dot(-lightDirN,normalN);
	float flag=step(0.0,dotResult);

	vec3 lightReflect = normalize(reflect(lightDirN,normalN));

	float specular=clamp(dot(lightReflect,-viewDir),0.0,1.0);
	
	//控制光斑大小
	specular = pow(specular,64);

	vec3 specularColor=lightColor*specular*flag*specularIntensity;

	//环境光计算
	vec3 ambientColor=ObjColor*ambientColor;

	vec3 finalColor = diffuseColor+specularColor+ambientColor;

	//FragColor = vec4(diffuseColor,1.0);
	//FragColor = vec4(specularColor,1.0);
	FragColor = vec4(finalColor,1.0);

}