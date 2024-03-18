#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;
uniform int texWidth;
uniform int texHeight;

void main(){

    float max = max(texWidth, texHeight);
    max = 512.0f;

    float offset[5] = float[](0.0, 1.0, 2.0, 3.0, 4.0);
    float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216,
                                  0.0540540541, 0.0162162162);

	vec4 texColor = texture( texBuf, texCoord) * weight[0]; //  / max, * weight[0]

    // do 5x5 gaussian blur...
    for (int i=1; i<5; i++) {
        texColor +=
            texture2D(texBuf, (vec2(texCoord) + vec2(0.0, offset[i]) / max ) )
                * weight[i];
        texColor +=
            texture2D(texBuf, (vec2(texCoord) - vec2(0.0, offset[i]) / max ) )
                * weight[i];
    }

	color = vec4(texColor.rgb, 1.0);
}