#version 330 core
uniform sampler2D Texture0;

uniform vec3 MatAmb; // normal "color" of the object
uniform vec3 MatDif; // diffuse "color"
uniform vec3 MatSpec; // specular "color"
uniform float MatShine; // specular "shinniness" factor

uniform float mode;

in vec2 vTexCoord;

//interpolated normal and light vector in camera space
in vec3 fragNor;
in vec3 lightDir;
//position of the vertex in camera space
in vec3 EPos;

out vec4 color;

void main() {
  	vec4 texColor0 = texture(Texture0, vTexCoord);

  	//to set the out color as the texture color 
  	color = texColor0;

  	//to set the outcolor as the texture coordinate (for debugging)
	// color = vec4(vTexCoord.s, vTexCoord.t, 0, 1);

	// Lighting 

	vec3 normal = normalize(fragNor);
	vec3 light = normalize(lightDir);

	//vec3 V = normalize(-1 * EPos);
	vec3 V = normalize(-1 * EPos);
	vec3 H = normalize(V + light);

	float dC = max(dot(normal, light), 0);
	float sC = pow(max(dot(normal, H), 0), MatShine);

	// note that color.a is our opacity, which we need to be 1!!!
	color = vec4(texColor0.xyz, 0.0);
	color += dC * vec4(MatDif, 0.0); // diffuse light
	color += sC * vec4(MatSpec, 0.0); // specular light
	color += vec4(MatAmb, 1.0); // ambient light

	// mode == 0 is just to do default lighting with textures
	if(mode == 1)
	{
		// mode == 1 is defined as clearing all black pixels
		if (texColor0.r <= 0.01 && texColor0.g <= 0.01 && texColor0.b <= 0.01)
		{
			discard;
		}
	}
	if(mode == 2)
	{
		// mode == 2 is defined as disabling lighting calculations
		color = texColor0;
	}
}