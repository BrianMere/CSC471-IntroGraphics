#include "Drawing.h"

template <typename T, int N>
bool BoundingBox<T, N>::in_box(std::vector<T> v)
{

    // check the dimension of the vector. 
    if (v.size() != this->minimums.size() ||
        v.size() != this->maximums.size()
    )
    {
        std::cerr << "The input vector doesn't match the dimension of this box" << std::endl;
        return false;
    }

    // check for boundedness...
    for(int i = 0; i < v.size(); i++)
    {
        if(v.at(i) < this->minimums.at(i) || 
            v.at(i) > this->maximums.at(i))
        {
            return false;
        }
    }
    return true;
};
