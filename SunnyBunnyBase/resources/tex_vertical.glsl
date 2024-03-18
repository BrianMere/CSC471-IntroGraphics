#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;
uniform int texWidth;
uniform int texHeight;

void main(){

	vec2 filteredTexCoord;
	filteredTexCoord.x = texCoord.x;
	filteredTexCoord.y = texCoord.y;

	// strip-like effect
	if (mod(float(texWidth) * texCoord.x, 30.0) < 15.0)
	{ // shift up if that's the case
		filteredTexCoord.y += (9.0 / float(texHeight));
	}

	vec3 texColor = texture( texBuf, filteredTexCoord ).rgb;
	//TODO modify to show this is a 2D image
	color = vec4(texColor, 1.0);

	// blue checkerboard pattern aftereffects...
	// if (
	// 	mod(float(texWidth) * texCoord.x, 20.0) < 10.0 && 
	// 	mod(float(texHeight) * texCoord.y, 20.0) < 10.0
	// )
	// {
	// 	color.b = 1.0;
	// }
}
