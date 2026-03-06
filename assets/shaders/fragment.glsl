#version 460 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;

uniform sampler2D grass_sampler;
uniform sampler2D land_sampler;
uniform sampler2D noise_sampler;


void main()
{
	vec4 grass_color=texture(grass_sampler,uv);
	vec4 land_color=texture(land_sampler,uv);
	vec4 noise_color=texture(noise_sampler,uv);
	float weight=noise_color.r;

	//vec4 final_color=grass_color*noise_color+land_color*(1.0-noise_color);
	vec4 final_color=mix(grass_color,land_color,weight);

	//FragColor = final_color;
	FragColor = vec4(final_color.rgb,1.0);
}