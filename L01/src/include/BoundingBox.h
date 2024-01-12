#include <vector>
#include <iostream>
#include <stdexcept>

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

// Class item to represent a bounding box, with dimension N
template <typename T, int N>
class BoundingBox
{
private:
    std::vector<std::vector<T>> m_list; // list of Vertices (as vectors)
    std::vector<T> maximums;
    std::vector<T> minimums;
public:
    // nothing in the constructor!!!
    BoundingBox()
    {
        this->m_list.clear();
    }
    ~BoundingBox(){};

    // add vertex to BoundingBox...
    void add_vertex(std::vector<T> v)
    {
        if (v.size() == N)
        {
            this->m_list.push_back(v);
        }
        else
            std::cerr << "Invalid dimensional vertex passed in." << std::endl;
    
        this->refresh_maxmins();
    }

    void refresh_maxmins()
    {
        // initialize the first vertex as the max AND the min
        if (this->m_list.size() <= 0)
        {
            std::cerr << "No vertices exist to define the bounding box." << std::endl;
        }
        else 
        {
            //initial values are just to use the first vertex in our list. 
            this->maximums = this->m_list.at(0);
            this->minimums = this->m_list.at(0);

            // for each vertex, replace it's maximums and minimum entries correspondingly.
            for(int i = 0; i < N; i++)
            {
                // i is the n-th dimension we are looking at
                for(std::vector<T> vertex : this->m_list)
                {
                    if (this->maximums.at(i) < vertex.at(i))
                    {
                        this->maximums[i] = vertex.at(i);
                    }
                    if (this->minimums.at(i) > vertex.at(i))
                    {
                        this->minimums[i] = vertex.at(i);
                    }
                }
            }
        }
    }

    // determine if a point is within the BoundingBox
    bool in_box(std::vector<T> v);
};

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

#endif

