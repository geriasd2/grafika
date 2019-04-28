#ifndef LINE_H
#define LINE_H

#include <vector>
#include <glm/glm.hpp>


glm::vec3 closest_point(glm::vec3 &cameraFront, glm::vec3 &cameraPos, glm::ve3 &cubeCenter){
    glm::vec3 otherPoint = cameraPos + cameraFront;
    glm::vec3 t_weights = cameraFront;

    float t = 0.0f;
    float c = 0.0f;
    for(int i = 0; i < 3; i++){
        t += cameraFront[i] * cameraFront[i];
        c += cameraFront[i] * (cameraPos[i] - cubeCenter[i]);
    }
    t = -c / t;
    return cameraFront * t + cameraPos;
}

bool insideCube(glm::vec3 &point, std::vector<float> &vertices){
}

#endif
