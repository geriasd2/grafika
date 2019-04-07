#ifndef MAPGENERATE_H
#define MAPGENERATE_H

#include "point.h"
#include <vector>
//#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Map {
public:
    std::vector<Point> ctrlPoints;
    std::vector<Point> mapPoints;
    unsigned int VBO;
    GLuint VAO;
    unsigned int VBOapprox;
    unsigned int VAOapprox;
    unsigned int approx = 100;
    float clickedPoints[600] = {0};
    std::vector<float> vertices = std::vector<float>(approx * 6);
    unsigned int addedPoints = 0;
    bool changeInCourse = false;
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    bool grabbing = false;

public:

    Map(std::vector<Point> v, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT);

    double L( int i, double tt, float knotVector[], int addedPoints );

    Point CalcLagrangePoint (float t, std::vector<Point> &ctrlPoint, float knotVector[], int addedPoints);

    void Lagrange(unsigned int addedPoints, unsigned int approx, std::vector<Point> ctrlPoints, std::vector<float> &vertices);

    void DrawMap();

    void grab(float xpos, float ypos);

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};


#endif
