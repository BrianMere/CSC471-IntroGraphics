#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;
uniform int texWidth;
uniform int texHeight;

void main(){

	vec4 texColor = texture( texBuf, texCoord);

	color = vec4(texColor.rgb, 1.0);
}