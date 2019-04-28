#ifndef LINE_H
#define LINE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


glm::vec3 closest_point(glm::vec3 &cameraFront, glm::vec3 &cameraPos, glm::vec3 &cubeCenter){
    float t = 0.0f;
    float c = 0.0f;
    for(int i = 0; i < 3; i++){
        t += cameraFront[i] * cameraFront[i];
        c += cameraFront[i] * (cameraPos[i] - cubeCenter[i]);
    }
    t = -c / t;
    if(t < 0){
        return cameraPos;
    }
    return cameraFront * t + cameraPos;

}
/*
bool insideCube(glm::vec3 &point, std::vector<float> &vertices){
}
*/
#endif
