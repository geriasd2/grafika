
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include <vector>

#include <iostream>
#include <cmath>
#include <math.h>

#define approx 500
#define PI 3.14159265


class Point{

public:

    float x;
    float y;
    bool ctrlpoint;

    Point(float x, float y){
        this->x = x;
        this->y = y;
        ctrlpoint = false;
    }

    Point operator*(double L){
        Point p(this->x, this->y);
        p.x *= L;
        p.y *= L;
        return p;
    }

    Point operator+(Point p){
        Point p2(this->x + p.x, this->y + p.y);
        return p2;
    }

    Point operator/(double n){
        Point p2(this->x / n, this->y / n);
        return p2;
    }

    float distance(Point p){
        float xDist = this->x - p.x;
        float yDist = this->y - p.y;
        return xDist * xDist + yDist * yDist;
    }
};

class Snail{

public:
    glm::mat4 trans = glm::mat4(1.0f);
    double lastFrame = 0;
    double actFrame = 0;
    unsigned int actPtIdx = 0;
    float numOfVertices = 72;
    float direction = 0;
    float speed = 0.0;
    glm::vec2 position = glm::vec2(0.0, 0.0);
    float vertices[432] = {
        0.0, 0.0, 0.0, 0.6, 0.4, 0.2,
        -0.065, -0.54, 0.0, 0.6, 0.4, 0.2,
        -0.095119, -0.497507, 0.0, 0.6, 0.4, 0.2,
        -0.124625, -0.4541, 0.0, 0.6, 0.4, 0.2,
        -0.152904, -0.408866, 0.0, 0.6, 0.4, 0.2,
        -0.179292, -0.361235, 0.0, 0.6, 0.4, 0.2,
        -0.203127, -0.310632, 0.0, 0.6, 0.4, 0.2,
        -0.22374, -0.257064, 0.0, 0.6, 0.4, 0.2,
        -0.240462, -0.200533, 0.0, 0.6, 0.4, 0.2,
        -0.252804, -0.141643, 0.0, 0.6, 0.4, 0.2,
        -0.260274, -0.0809978, 0.0, 0.6, 0.4, 0.2,
        -0.262821, -0.0195675, 0.0, 0.6, 0.4, 0.2,
        -0.260392, 0.0416779, 0.0, 0.6, 0.4, 0.2,
        -0.253572, 0.10181, 0.0, 0.6, 0.4, 0.2,
        -0.242945, 0.159902, 0.0, 0.6, 0.4, 0.2,
        -0.229685, 0.215444, 0.0, 0.6, 0.4, 0.2,
        -0.214968, 0.267927, 0.0, 0.6, 0.4, 0.2,
        -0.200194, 0.317402, 0.0, 0.6, 0.4, 0.2,
        -0.186763, 0.363918, 0.0, 0.6, 0.4, 0.2,
        -0.175713, 0.407924, 0.0, 0.6, 0.4, 0.2,
        -0.168087, 0.449864, 0.0, 0.6, 0.4, 0.2,
        -0.16406, 0.49022, 0.0, 0.6, 0.4, 0.2,
        -0.163811, 0.529469, 0.0, 0.6, 0.4, 0.2,
        -0.166541, 0.567704, 0.0, 0.6, 0.4, 0.2,
        -0.171454, 0.605018, 0.0, 0.6, 0.4, 0.2,
        -0.177169, 0.640794, 0.0, 0.6, 0.4, 0.2,
        -0.182305, 0.674412, 0.0,0.6, 0.4, 0.2,
        -0.185609, 0.704512, 0.0, 0.6, 0.4, 0.2,
        -0.185825, 0.729732, 0.0, 0.6, 0.4, 0.2,
        -0.182434, 0.748419, 0.0, 0.6, 0.4, 0.2,
        -0.174915, 0.758921, 0.0, 0.6, 0.4, 0.2,
        -0.163601, 0.760198, 0.0, 0.6, 0.4, 0.2,
        -0.148825, 0.751213, 0.0, 0.6, 0.4, 0.2,
        -0.131356, 0.732489, 0.0, 0.6, 0.4, 0.2,
        -0.111965, 0.704546, 0.0, 0.6, 0.4, 0.2,
        -0.0913056, 0.669823, 0.0, 0.6, 0.4, 0.2,
        -0.0700317, 0.630758, 0.0, 0.6, 0.4, 0.2,
        -0.0484571, 0.591022, 0.0, 0.6, 0.4, 0.2,
        -0.0268953, 0.55429, 0.0, 0.6, 0.4, 0.2,
        -0.00546516, 0.523902, 0.0, 0.6, 0.4, 0.2,
        0.0157144, 0.503198, 0.0, 0.6, 0.4, 0.2,
        0.0365588, 0.493559, 0.0, 0.6, 0.4, 0.2,
        0.0569832, 0.496366, 0.0, 0.6, 0.4, 0.2,
        0.0769529, 0.510359, 0.0, 0.6, 0.4, 0.2,
        0.0964333, 0.534279, 0.0, 0.6, 0.4, 0.2,
        0.115517, 0.564945, 0.0, 0.6, 0.4, 0.2,
        0.134297, 0.599175, 0.0, 0.6, 0.4, 0.2,
        0.153001, 0.633382, 0.0, 0.6, 0.4, 0.2,
        0.171855, 0.663978, 0.0,0.6, 0.4, 0.2,
        0.190887, 0.687945, 0.0, 0.6, 0.4, 0.2,
        0.210124, 0.702268, 0.0, 0.6, 0.4, 0.2,
        0.228655, 0.70482, 0.0, 0.6, 0.4, 0.2,
        0.245566, 0.693476, 0.0, 0.6, 0.4, 0.2,
        0.25885, 0.668485, 0.0, 0.6, 0.4, 0.2,
        0.266502, 0.630093, 0.0,0.6, 0.4, 0.2,
        0.267285, 0.581822, 0.0, 0.6, 0.4, 0.2,
        0.25996, 0.527192, 0.0, 0.6, 0.4, 0.2,
        0.247576, 0.469, 0.0, 0.6, 0.4, 0.2,
        0.233179, 0.410041, 0.0, 0.6, 0.4, 0.2,
        0.222704, 0.348805, 0.0, 0.6, 0.4, 0.2,
        0.222086, 0.28378, 0.0, 0.6, 0.4, 0.2,
        0.228515, 0.212583, 0.0, 0.6, 0.4, 0.2,
        0.239181, 0.132831, 0.0, 0.6, 0.4, 0.2,
        0.244235, 0.0422385, 0.0, 0.6, 0.4, 0.2,
        0.233827, -0.0614775, 0.0, 0.6, 0.4, 0.2,
        0.208734, -0.168485, 0.0,0.6, 0.4, 0.2,
        0.169733, -0.26895, 0.0, 0.6, 0.4, 0.2,
        0.120107, -0.357745, 0.0, 0.6, 0.4, 0.2,
        0.0631389, -0.429739, 0.0,0.6, 0.4, 0.2,
        0.00890972, -0.482851, 0.0,0.6, 0.4, 0.2,
        -0.0325, -0.515, 0.0, 0.6, 0.4, 0.2,
        -0.0675, -0.536667, 0.0, 0.6, 0.4, 0.2
    };



    Snail(){}

    void loadToGpu(unsigned int &VAO, unsigned int &VBO){
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    void draw(unsigned int &VAO, unsigned int &VBO, Shader &shader, std::vector<Point> &points){

        this->calcTransform(points);
        this->trans = glm::scale(this->trans, glm::vec3(0.1, 0.1, 0.1));
        shader.setMat4("trans", this->trans);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, this->numOfVertices);
        glBindVertexArray(0);
    }

    void setEllapsedTime(){
        this->lastFrame = this->actFrame;
        this->actFrame = glfwGetTime();
    }

    void calcTransform(std::vector<Point> &points){
        if(points.size() < 5){
            this->trans = glm::mat4(1.0f);
            if(points.size()){
                this->trans = glm::translate(this->trans, glm::vec3(points[0].x, points[0].y, 0.0));
                //this->trans = glm::rotate(this->trans, -glm::radians(36.0f), glm::vec3(0.0, 0.0, 1.0));
            }
            return;
        }

        this->setEllapsedTime();
        Point nextPt = this->setNextPt(points);
        if(this->checkDistance(nextPt)){
            //we can just shift position with the normalized direction vector * ellapsed time
            this->setDisplacementRotation(nextPt, this->actFrame - this->lastFrame);
            return;
        }
        this->actPtIdx++;
        this->calcTransform(points);
    }

    Point setNextPt(std::vector<Point> &points){
        if(this->actPtIdx == points.size() - 1){
            this->actPtIdx = 0;
        }
        return points[this->actPtIdx + 1];
    }

    bool checkDistance(Point p){
        double ellapsedTime = this->actFrame - this->lastFrame;
        double length = glm::length(glm::vec2(this->position[0] - p.x, this->position[1] - p.y));
        return ellapsedTime < length;
    }

    void setDisplacementRotation(Point nextPt, double distance){
        glm::vec2 directionVec = glm::normalize(glm::vec2(nextPt.x - this->position[0], nextPt.y - this->position[1]));
        this->setSpeed(directionVec);
        float xDisplacement = directionVec[0] * distance * this->speed;
        float yDisplacement = directionVec[1] * distance * this->speed;
        this->position = glm::vec2(this->position[0] + xDisplacement, this->position[1] + yDisplacement);
        this->trans = glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0));
        //std::cout << "y / x = " << directionVec[1] / directionVec[0] << " -> atan = " << glm::atan(directionVec[1] / directionVec[0]) << "\n";
        this->trans = glm::rotate(this->trans, countAngle(directionVec[0], directionVec[1], xDisplacement), glm::vec3(0.0, 0.0, 1.0));
    }

    void setSpeed(glm::vec2 directionVec){
        float steepness = std::abs(directionVec[1] / directionVec[0]);

        steepness = (steepness > 2.0f)? 2.0f : steepness;
        steepness = (steepness < 0.5f)? 0.5f : steepness;
        this->speed = steepness;
    }

    float countAngle(float x, float y, float xDisplacement){
        float angle = glm::atan(y / x);
        angle += (xDisplacement > 0) ? glm::radians(-90.0f) : glm::radians(90.0f);
        return angle;
    }

};

void initGpu(unsigned int &VAO, unsigned int &VBO, unsigned int numOfVertices, unsigned int mode){
    float tmp[numOfVertices * 6];
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfVertices * 6, tmp, mode);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}


std::vector<float> pointsToVertices(std::vector<Point> v){


    std::vector<float> vertices(v.size() * 6);
    for(unsigned int i = 0; i < vertices.size(); i++){
        vertices[i] = v[i].x;
        vertices[i] = v[i].y;
        vertices[i] = 0.0f;
        vertices[i] = 1.0f;
        vertices[i] = 1.0f;
        vertices[i] = 1.0f;

    }
    return vertices;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
bool grabbing = false;

std::vector<Point> points;
unsigned int VBOctrlpoints, VAOctrlpoints, VBOmap, VAOmap, VBOsnail, VAOsnail;
unsigned int addedPoints = 0;
Snail snail;

float r = 0, g = 0, b = 0, speed = 0.1;

void cursorPos(Point &p, GLFWwindow* window){
    double xpos, ypos;
    //getting cursor position
    glfwGetCursorPos(window, &xpos, &ypos);
    float normalizerX = SCR_WIDTH / 2.0;
    float normalizerY = SCR_HEIGHT / 2.0;
    xpos = (xpos - normalizerX) / normalizerX;
    ypos = -1 * (ypos - normalizerY) / normalizerY;
    p.x = xpos;
    p.y = ypos;
    p.ctrlpoint = true;
}

std::vector<Point> filterCtrlPoints(std::vector<Point> &v){
    std::vector<Point> ctrlPoints;
    for(unsigned int i = 0; i < v.size(); i++){
        if(v[i].ctrlpoint){
            ctrlPoints.push_back(v[i]);
        }
    }
    return ctrlPoints;
}

void handleGrab(std::vector<Point> &points, GLFWwindow* window){
    unsigned int idx = 0;
    unsigned int prevIdx = 0;
    unsigned int nextIdx = 0;
    float distance = 5;

    Point cursor(0, 0);
    cursorPos(cursor, window);
    for(unsigned int i = 0; i < points.size(); i++){
        if(!points[i].ctrlpoint){
            continue;
        }
        float distanceFromCursor = points[i].distance(cursor);
        if(distanceFromCursor < distance){
            distance = distanceFromCursor;
            idx = i;
        }
    }
    points[idx] = cursor;

}

void CatmullClark(){
    for(unsigned int i = 0; i + 1 < points.size(); i += 2){
        float half_x = (points[i].x + points[i + 1].x) * 0.5;
        float half_y = (points[i].y + points[i + 1].y) * 0.5;
        Point half(half_x, half_y);
        points.insert(points.begin() + i + 1, half);
    }
    for(unsigned int i = 2; i + 1 < points.size(); i += 2){
        points[i].x = points[i].x * 0.5 + points[i - 1].x * 0.25 + points[i + 1].x * 0.25;
        points[i].y = points[i].y * 0.5 + points[i - 1].y * 0.25 + points[i + 1].y * 0.25;
    }
}


void drawMap(){
    std::vector<Point> ctrlPoints = filterCtrlPoints(points);
    points = ctrlPoints;
    for(unsigned int i = 0; i < 3; i++){
        CatmullClark();
    }
    ctrlPoints = filterCtrlPoints(points);

    float ctrlPointVertices2[ctrlPoints.size() * 6];

    for(unsigned int i = 0; i < ctrlPoints.size(); i++){
        ctrlPointVertices2[i * 6] = ctrlPoints[i].x;
        ctrlPointVertices2[i * 6 + 1] = ctrlPoints[i].y;
        ctrlPointVertices2[i * 6 + 2] = 0.0f;
        ctrlPointVertices2[i * 6 + 3] = 0.0f;
        ctrlPointVertices2[i * 6 + 4] = 0.5f;
        ctrlPointVertices2[i * 6 + 5] = 0.0f;
    }
    glBindVertexArray(VAOctrlpoints);
    glBindBuffer(GL_ARRAY_BUFFER, VBOctrlpoints);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ctrlPointVertices2), &ctrlPointVertices2);

    Point firstPoint = points[0];
    firstPoint.ctrlpoint = false;
    points.push_back(firstPoint);

    float vertice[points.size() * 6];
    for(unsigned int i = 0; i < points.size(); i++){
        vertice[i * 6] = points[i].x;
        vertice[i * 6 + 1] = points[i].y;
        vertice[i * 6 + 2] = 0.0f;
        vertice[i * 6 + 3] = 0.0f;
        vertice[i * 6 + 4] = 0.0f;
        vertice[i * 6 + 5] = 0.8f;
    }
    glBindVertexArray(VAOmap);
    glBindBuffer(GL_ARRAY_BUFFER, VBOmap);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertice), &vertice);

}



int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // -------------------------

    initGpu(VAOmap, VBOmap, approx, GL_DYNAMIC_DRAW);
    initGpu(VAOctrlpoints, VBOctrlpoints, 60, GL_DYNAMIC_DRAW);
    initGpu(VAOsnail, VBOsnail, sizeof(snail.vertices) / sizeof(float) / 6, GL_STATIC_DRAW);
    snail.loadToGpu(VAOsnail, VBOsnail);

    glPointSize(4);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    Shader ourShader = Shader("4.0.shader_orai.vs", "4.0.shader_orai.fs");
    //Shader lineShader = Shader("4.0.shader_orai_linedraw.vs", "4.0.shader_orai.fs");



    // render loop
    // -----------
    glLineWidth(3);
    glPointSize(9);

    while (!glfwWindowShouldClose(window)) {


        if(grabbing){
            handleGrab(points, window);
            drawMap();
        }

        glClearColor(0.77f, 0.76f, 0.72f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        ourShader.setMat4("trans", glm::mat4(1.0f));

        glBindVertexArray(VAOmap);
        glDrawArrays(GL_LINE_STRIP, 0, points.size());

        glBindVertexArray(VAOctrlpoints);
        glDrawArrays(GL_POINTS, 0, addedPoints);

        snail.draw(VAOsnail, VBOsnail, ourShader, points);

        //glBindVertexArray(0);

        //glDrawArrays(GL_LINE_STRIP, 0, points.size());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAOmap);
    glDeleteVertexArrays(1, &VAOctrlpoints);
    glDeleteVertexArrays(1, &VAOsnail);
    glDeleteBuffers(1, &VBOctrlpoints);
    glDeleteBuffers(1, &VBOmap);
    glDeleteBuffers(1, &VBOsnail);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}




static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    else if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        for(unsigned int i = 0; i < points.size(); i++){
            std::cout << points[i]. x << ", " << points[i].y << ", " << "0.0, 0.0, 0.0, 0.0,\n";
        }
    }
}


static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {


        Point p(0, 0);
        cursorPos(p, window);
        float vertice[6] = {
            p.x, p.y, 0.0f, 1.0f, 0.0f, 0.0f
        };
        points.push_back(p);
        glBindVertexArray(VAOctrlpoints);
        glBindBuffer(GL_ARRAY_BUFFER, VBOctrlpoints);
        glBufferSubData(GL_ARRAY_BUFFER, 6*sizeof(float)*addedPoints, sizeof(vertice), &vertice);

        std::cout << addedPoints << ".: " <<"Cursor Position at " << p.x << " : " << p.y << "\n";
        addedPoints++;
        drawMap();

    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
        grabbing = true;
    }
    else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
        grabbing = false;
    }

}
