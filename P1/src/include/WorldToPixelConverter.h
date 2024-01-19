
/**
 * A converter function to handle all the math regarding converting 
 * world coordinates to pixel coordinates. 
*/
class WorldToPixelConverter
{
private:
    // view plane definitions
    float L;
    float R;
    float B;
    float T;

    // Cx_w + D = x_p, Ey_w + F = y_p
    float C;
    float D;
    float E;
    float F;
public:
    WorldToPixelConverter(unsigned int width, unsigned int height);
    ~WorldToPixelConverter() {};

    // Convert x world coordinates to x pixel coordinates
    inline unsigned int W2PX(float xw);
    // Convert y world coordinates to y pixel coordinates
    inline unsigned int W2PY(float yw);
};

