#include "WorldToPixelConverter.h"

WorldToPixelConverter::WorldToPixelConverter(unsigned int width, unsigned int height)
{
    // get viewing plane values
    float temp = ((float) width)/((float) height);
    if(width >= height)
    {
        this->L = -temp;
        this->R = temp; 
        this->B = -1.0;
        this->T = 1.0;
    }
    else 
    {
        float i_temp = 1/temp;

        this->L = -1.0;
        this->R = 1.0;
        this->B = -i_temp;
        this->T = i_temp;
    }

    // get constants for scaling and shifting world objects to the window
    // when we shift to pixel coords, we get the L => 0 and R => W - 1
    // and similar for the y-direction
    this->C = ((float)(width - 1))/(this->R - this->L);
    this->D = -(this->C * this->L);
    this->E = ((float)(height - 1))/(this->T - this->B);
    this->F = -(this->E * this->B);
}

inline unsigned int WorldToPixelConverter::W2PX(float xw)
{
    return (unsigned int)(this->C * xw + this->D);
}

inline unsigned int WorldToPixelConverter::W2PY(float yw)
{
    return (unsigned int)(this->E * yw + this->F);
}