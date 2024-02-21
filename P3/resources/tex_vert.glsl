#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
layout(location = 3) in vec3 lightPos;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec2 vTexCoord;
out vec3 fragNor;
out vec3 lightDir;
out vec3 EPos; // intercept point

void main() {

  /* First model transforms */
  gl_Position = P * V * M * vertPos;

  // fragNor = (M * vec4(vertNor, 0.0)).xyz;
  fragNor = (V * M * vec4(vertNor, 0.0)).xyz; 
	lightDir = (V*vec4(lightPos, 1.0)).xyz - (V*M*vertPos).xyz;
	EPos = (V*M*vertPos).xyz;

  /* pass through the texture coordinates to be interpolated */
  vTexCoord = vertTex;
}