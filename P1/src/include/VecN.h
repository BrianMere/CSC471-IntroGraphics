#ifndef VECN_H
#define VECN_H

#include <array>
#include <iostream>
#include <tuple>
#include <cmath>
#include <algorithm>

/*
    A static vector implementation class. Makes it so that we only deal with N-dimensional vectors
*/
template <typename T, int N>
class VecN
{
private:
public:

    std::array<T,N> m_data;
    // default constructor
    VecN()
    {
        this->m_data = std::array<T,N>();
        for (size_t i = 0; i < this->m_data.size(); i++)
        {
            this->m_data[i] = T(0);
        }
    };
    // pass in an array of appropriate size and make a vector with that size...
    VecN(std::vector<T> vec) {std::copy(std::begin(vec), std::end(vec), this->m_data.begin());}; 
    VecN(std::array<T,N> & data) : m_data(data) {};
    // default deconstructor
    ~VecN() {};

    // Operator Overrides...

    T operator[](int i) const { return this->m_data.at(i); };
    T& operator[](int i) { return this->m_data.at(i); };
    VecN<T,N>& operator *(const VecN<T,N>& v) { return *this; };
    // define scalar multiplication
    VecN<T,N> operator *(double alpha) const
    {
        std::array<T,N> r_data = std::array<T,N>();
        for (size_t i = 0; i < this->m_data.size(); i++)
        {
            r_data[i] = alpha * this->m_data.at(i);
        }
        return VecN<T,N>(r_data);
    }
    // define vector addition
    VecN<T,N> operator +( VecN<T,N> & o_v) const
    {
        std::array<T,N> r_data = std::array<T,N>();
        for (size_t i = 0; i < this->m_data.size(); i++)
        {
            r_data[i] = this->m_data.at(i) + o_v[i];
        }
        return VecN<T,N>(r_data);
    }

    // Now the following can be consructed from the two definitions above!

    VecN<T,N> operator -( VecN<T,N> & o_v) const
    {
        VecN<T,N> in = o_v * -1.0;
        return *this + in;
    }

    double length_squared() const
    {
        double sum = 0.0;
        for (T elem : this->m_data)
        {
            sum += ((double)elem * (double)elem);
        }
        return sum;
    }
    double length() const { return sqrt(this->length_squared());}


    VecN<T,N>& operator+=(const VecN<T,N> & o_v) 
    {
        for (size_t i = 0; i < N; i++)
        {
            (*this)[i] = (*this)[i] + o_v[i];
        }
        return *this;
    }
    VecN<T,N>& operator*=(double alpha) 
    {
        for (size_t i = 0; i < N; i++)
        {
            (*this)[i] *= alpha;
        }
        return *this;
    }
    VecN<T,N>& operator-=(const VecN<T,N> & o_v)
    {
        *this += o_v * (-1.0);
        return *this;
    }


};

template <typename T, int N>
inline std::ostream& operator <<(std::ostream &out, const VecN<T,N> &v) 
{
    out << "[";
    for (size_t i = 0; i < v.m_data.size(); i++)
    {
        T elem = v.m_data.at(i);
        out << std::to_string(elem);
        // only add "," if we aren't at the end
        if(i != v.m_data.size() - 1)
        {
            out << ", ";
        }
    }
    out << "]";
    return out;
    //return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];

}

typedef VecN<u_int8_t, 3> Color; // define Color as just a vector of 3 colors with 8 bits (max 0 to 255 is what we want)
typedef VecN<double, 3> Vec3; // define a Vec3 (world coords) as our VecN with a double floating point values and 3 values
typedef VecN<double, 4> Vec4; // same as Vec3 but with an extra coordinate (for math we do later on...)

/**
 * Ignore the top 8 bits of hex and create a Color object from the other values...
*/
inline Color color_from_hex(u_int32_t hex) 
{
    return Color({(hex & 0xFF0000U) >> 16, (hex & 0x00FF00U) >> 8, (hex & 0x0000FFU) >> 0});
}

/**
 * Calculate the determinant of two, 2-dimensional vectors (ie: in 2D, find the area that they encompass on their spanning parallelogram
*/
template <typename T>
inline double calc_2determinant(VecN<T, 2> & v1, VecN<T, 2> & v2)
{
    return (v1[0] * v2[1]) - (v2[0] * v1[1]);
}

/**
 * Calculate the Area of a triangle, given by 3 vertices...
*/
template <typename T>
inline double calc_TriArea(VecN<T,2> & v1, VecN<T,2> & v2, VecN<T,2> & v3)
{
    VecN<T, 2> vT = v1 - v3;
	VecN<T, 2> vP = v2 - v3;
	return 0.5 * calc_2determinant(vT, vP);
}


// some default colors and vectors, for you to have fun with!!!
Color RED = color_from_hex(0xFF0000);
Color GREEN = color_from_hex(0x00FF00);
Color BLUE = color_from_hex(0x0000FF);
Color BLACK = color_from_hex(0x000000);
Color YELLOW = color_from_hex(0xFFFF00);
Color ORANGE = color_from_hex(0xFF8000);
Color PURPLE = color_from_hex(0x7F00FF);
Color WHITE = color_from_hex(0xFFFFFF);

#endif