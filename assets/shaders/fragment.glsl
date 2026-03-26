#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;

uniform sampler2D sampler;

uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform vec3 cameraPosition;


void main()
{
	//获取当前像素的颜色值
	vec3 ObjColor = texture(sampler,uv).xyz;

	//准备diffuse(漫反射)相关数据
	vec3 normalN=normalize(normal);
	vec3 lightDirN=normalize(lightDirection);

	//
	float diffuse= clamp(dot(-lightDirN,normalN),0.0,1.0);

	vec3 finalColor = lightColor* diffuse* ObjColor;

	//FragColor = vec4(finalColor,1.0);

	vec3 viewDir=normalize(worldPosition-cameraPosition);
	viewDir=clamp(-viewDir,0.0,1.0);
	FragColor = vec4(viewDir,1.0);

}