#version 330 core
uniform sampler2D Texture0;
uniform int flip; 

in vec2 vTexCoord;
//interpolated normal and light vector in camera space
in vec3 fragNor;
in vec3 lightDir;
//position of the vertex in camera space
in vec3 EPos;



out vec4 Outcolor;

void main() {

  	vec4 texColor0 = texture(Texture0, vTexCoord);

	//to set the out color as the texture color 
	Outcolor = texColor0;

	//to set the outcolor as the texture coordinate (for debugging)
	// Outcolor = vec4(vTexCoord.s, vTexCoord.t, 0, 1);

	/* Lighting Part */

  	//you will need to work with these for lighting
	if(flip != 0)
	{
		vec3 normal = normalize(fragNor);
		vec3 light = normalize(lightDir);

		vec3 V = normalize(-1 * EPos);
		vec3 H = normalize(V + light);

		float dC = max(dot(normal, light), 0);
		// float sC = pow(max(dot(normal, H), 0), MatShine);

		// note that color.a is our opacity, which we need to be 1!!!
		Outcolor = dC * vec4(Outcolor); // diffuse light

		// if the color is mostly blue, clear the opacity
		if ((Outcolor.z > Outcolor.x && Outcolor.z > Outcolor.y) || (flip != 2 && dC == 0.0f))
		{
			discard;
		}
	}
	

}

