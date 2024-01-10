#include <vector>
#include <iostream>
#include <stdexcept>

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
        this->refresh_maxmins();

        if (v.size() == N)
        {
            this->m_list.insert(v);
        }
        else
            std::cerr << "Invalid dimensional vertex passed in." << std::endl;
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



