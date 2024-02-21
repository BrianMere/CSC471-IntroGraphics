#include "function_generator.h"

// Calculate the normal at a point on the surface defined by the function
glm::vec3 calculateNormal(
    float x, float y,
    std::function<float (float, float)> func
    ) {
    // Approximate the normal by finding gradients in x and y directions
    float epsilon = 0.01f;
    glm::vec3 normal(
        func(x + epsilon, y) - func(x - epsilon, y),
        func(x, y + epsilon) - func(x, y - epsilon),
        2 * epsilon
    );
    return glm::normalize(normal);
}

/**
 * Generate an obj for a given function of two floats. 
 * 
 * Pass in a directory, and a filename to dictate where to store our function obj.
 * numSamplesDir is the number of samples to use. Higher resolution means more points. Uses the square number of samples
 * 
*/
void funcToObj(
    const std::string &directory, 
    const std::string &filename,
    const int numSamplesDir, 
    const std::function<float (float, float)> func,
    const float xmin, 
    const float xmax, 
    const float ymin, 
    const float ymax
) {
    // Define sampling parameters
    float minX = xmin, maxX = xmax;
    float minY = ymin, maxY = ymax;
    float delX = (maxX - minX) / numSamplesDir;
    float delY = (maxY - minY) / numSamplesDir;

    // Open OBJ file for writing
    std::ofstream objFile(directory + "/" + filename);
    if (!objFile.is_open()) {
        std::cerr << "Failed to open output file." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Sample the function and write vertices to OBJ file
    unsigned int count = 0;
    for (int i = 0; i < numSamplesDir; ++i) {
        for (int j = 0; j < numSamplesDir; ++j) {

            glm::vec3 bl = glm::vec3(
                minX + i * delX, 
                minY + j * delY, 
                func(minX + i*delX, minY + j * delY)
            );

            glm::vec3 tl = glm::vec3(
                minX + i * delX, 
                minY + (j + 1) * delY, 
                func(minX + i*delX, minY + (j+1) * delY)
            );

            glm::vec3 br = glm::vec3(
                minX + (i+1) * delX, 
                minY + j * delY, 
                func(minX + (i+1)*delX, minY + j * delY)
            );

            glm::vec3 tr = glm::vec3(
                minX + (i+1) * delX, 
                minY + (j + 1) * delY, 
                func(minX + (i+1)*delX, minY + (j+1) * delY)
            );

            // Add each vertex, then add the top left, bottom right triangles
            objFile << "v " << bl.x << " " << bl.y << " " << bl.z << std::endl;
            objFile << "v " << br.x << " " << br.y << " " << br.z << std::endl;
            objFile << "v " << tl.x << " " << tl.y << " " << tl.z << std::endl;
            objFile << "v " << tr.x << " " << tr.y << " " << tr.z << std::endl;

            // Support texture coordinates
            objFile << "vt " << std::to_string((float) i / numSamplesDir) 
                    << " " << std::to_string((float) j / numSamplesDir) 
                    << std::endl;

            objFile << "vt " << std::to_string((float) (i+1) / numSamplesDir) 
                    << " " << std::to_string((float) (j) / numSamplesDir) 
                    << std::endl;

            objFile << "vt " << std::to_string((float) (i) / numSamplesDir) 
                    << " " << std::to_string((float) (j+1) / numSamplesDir) 
                    << std::endl;

            objFile << "vt " << std::to_string((float) (i+1) / numSamplesDir) 
                    << " " << std::to_string((float) (j+1) / numSamplesDir) 
                    << std::endl;

            objFile << "f " << std::to_string(count + 1) << "/"
                            << std::to_string(count + 1) << " "
                            << std::to_string(count + 2) << "/"
                            << std::to_string(count + 2) << " "
                            << std::to_string(count + 3) << "/"
                            << std::to_string(count + 3) << " "
                            << std::endl;

            objFile << "f " << std::to_string(count + 2) << "/"
                            << std::to_string(count + 2) << " "
                            << std::to_string(count + 3) << "/"
                            << std::to_string(count + 3) << " "
                            << std::to_string(count + 4) << "/"
                            << std::to_string(count + 4) << " "
                            << std::endl;

            count += 4;
        }
    }

    // Close the OBJ file
    objFile.close();

    std::cout << "OBJ file generated successfully." << std::endl;

}
