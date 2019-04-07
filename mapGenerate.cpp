#include "mapGenerate.h"
#include "point.h"
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Map::Map(std::vector<Point> v, unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT){

    this->SCR_WIDTH = SCR_WIDTH;
    this->SCR_HEIGHT = SCR_HEIGHT;
    this->ctrlPoints = v;
    glGenVertexArrays(1, &VAO);
    std::cout<<"szia"<< "\n";
    glGenVertexArrays(1, &VAOapprox);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBOapprox);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(clickedPoints), clickedPoints, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(VAOapprox);
    glBindBuffer(GL_ARRAY_BUFFER, VBOapprox);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

double Map::L( int i, double tt, float knotVector[], int addedPoints ) {
    double Li = 1.0;
    for (int j = 0; j < addedPoints; j++) {
        if (i != j){
            //std::cout << "tt: " << tt << " | knotvector[j]: " << knotVector[j] << " | knotvetor[i]: " << knotVector[i] << "\n";
            Li *= (tt - knotVector[j]) / (knotVector[i] - knotVector[j]);
        }
    }
    //std::cout << Li << "\n";
    return Li;
}

Point Map::CalcLagrangePoint (float t, std::vector<Point> &ctrlPoint, float knotVector[], int addedPoints) {
    //std::cout << "t: " << t << "\n";
    Point actPT(0,0);
    for(int i = 0; i < addedPoints; i++)
        actPT = actPT + ctrlPoint[i]*L(i,t, knotVector, addedPoints);
    return actPT;
}

void Map::Lagrange(unsigned int addedPoints, unsigned int approx, std::vector<Point> ctrlPoints, std::vector<float> &vertices){
    addedPoints++;
    float knotVector[addedPoints];
    for(unsigned int i = 0; i < addedPoints; i++){
        knotVector[i] = (float) i / (float) (addedPoints - 1);
    }
    ctrlPoints.push_back(ctrlPoints[0]);
    for(unsigned int i = 0; i < approx; i++){
        float t = 1.0f / ((float) approx - 1) * (float)i;
        Point p = CalcLagrangePoint(t, ctrlPoints, knotVector, addedPoints);
        vertices[i * 6] = p.x;
        vertices[i * 6 + 1] = p.y;
        vertices[i * 6 + 2] = 0.0f;
        vertices[i * 6 + 3] = 1.0f;
        vertices[i * 6 + 4] = 1.0f;
        vertices[i * 6 + 5] = 1.0f;
    }
}

void Map::DrawMap(){

    this->Lagrange(addedPoints, approx, ctrlPoints, vertices);
    glBindVertexArray(VAOapprox);
    glBindBuffer(GL_ARRAY_BUFFER, VBOapprox);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());
    glDrawArrays(GL_LINE_STRIP, 0, approx);

}

void Map::grab(float xpos, float ypos){
    bool found = false;
    float minimum_radius = 0.01f;
    float minimum_distance = 1.0f;
    unsigned int minimum_pos = 0;
    for(unsigned int i = 0; i < addedPoints; i++){
        float distance = pow((xpos - ctrlPoints[i].x), 2) + pow((ypos - ctrlPoints[i].y), 2);
        if (distance < minimum_radius){
            found = true;
            if(distance < minimum_distance){
                minimum_distance = distance;
                minimum_pos = i;
            }
        }
    }
    if(found){
        ctrlPoints[minimum_pos].x = xpos;
        ctrlPoints[minimum_pos].y = ypos;

        float vertice[6] = {
            (float) xpos, (float) ypos, 0.0f, 1.0f, 1.0f, 1.0f
        };
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 6*sizeof(float)*minimum_pos, sizeof(vertice), &vertice);
    }
}

void Map::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    else if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        addedPoints = 0;
        ctrlPoints.clear();
    }

    else if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        glLineWidth(1);
    }
    else if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        glLineWidth(2);
    }
    else if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
        glLineWidth(3);
    }
    else if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
        glLineWidth(4);
    }
    else if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS){
        glLineWidth(5);
    }
    else if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS){
        glLineWidth(6);
    }
    else if(glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS){
        glLineWidth(7);
    }
    else if(glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
        glLineWidth(8);
    }
    else if(glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS){
        glLineWidth(9);
    }
}


void Map::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        changeInCourse = true;
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);
        float normalizerX = SCR_WIDTH / 2.0;
        float normalizerY = SCR_HEIGHT / 2.0;
        xpos = (xpos - normalizerX) / normalizerX;
        ypos = -1 * (ypos - normalizerY) / normalizerY;
        float vertice[6] = {
            (float) xpos, (float) ypos, 0.0f, 1.0f, 1.0f, 1.0f
        };

        ctrlPoints.push_back(Point((float) xpos, (float) ypos));
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 6*sizeof(float)*addedPoints, sizeof(vertice), &vertice);
        addedPoints++;

        std::cout << addedPoints << ".: " <<"Cursor Position at " << xpos << " : " << ypos << "\n";
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        grabbing = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
        grabbing = false;
    }
}

