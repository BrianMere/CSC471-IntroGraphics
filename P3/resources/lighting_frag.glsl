#version 330 core 

uniform vec3 MatAmb; // normal "color" of the object
uniform vec3 MatDif; // diffuse "color"
uniform vec3 MatSpec; // specular "color"
uniform float MatShine; // specular "shinniness" factor

//interpolated normal and light vector in camera space
in vec3 fragNor;
in vec3 lightDir;
//position of the vertex in camera space
in vec3 EPos;


out vec4 color;

void main()
{
	vec3 normal = normalize(fragNor);
	vec3 light = normalize(lightDir);

	//vec3 V = normalize(-1 * EPos);
	vec3 V = normalize(-1 * EPos);
	vec3 H = normalize(V + light);

	float dC = max(dot(normal, light), 0);
	float sC = pow(max(dot(normal, H), 0), MatShine);

	// note that color.a is our opacity, which we need to be 1!!!
	color = dC * vec4(MatDif, 0.0); // diffuse light
	color += sC * vec4(MatSpec, 0.0); // specular light
	color += vec4(MatAmb, 1.0); // ambient light
}
