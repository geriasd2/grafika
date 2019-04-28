#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#include "stb_image.h"
#include "shader.h"
#include "Camera.h"
#include "line.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool forward, backward, left, right = false;
bool shoot = false;
// camera
//Camera camera(glm::vec3(1.3f, 1.1f, 1.0f));
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

bool firstMouse = true; // csak lekezeljuk az eslo eger mozgatast
float lastX =  800.0f / 2.0;  // kezdeti eger pozicio
float lastY =  600.0 / 2.0;   // kezdeti eger pozicio

// timing // kamera mozgas gyorsasagahoz
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

//////////////////////////////////////////////////////
// lighting
// position of the light source
glm::vec3 lightPos(1.2f, 10.0f, -3.0f);
//////////////////////////////////////////////////////

float a = 0;
float d = 0;
float s = 0;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // tell GLFW to capture our mouse
    // nem latjuk a cursort es nem tudja elhagyni az ablakot
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    //////////////////////////////////////////////////////
    // build and compile our shader program
    Shader lightingShader("4.0.shader.vs", "4.0.shader.fs");

    // kulon shader mert ha valtoztatni akarunk a vertexeken akkor legyen fuggetlen
    // a light es a targyak rajzolasa
    // es nem akarjuk h a feny hatassal legyen a lampankra csak a megvilagitott targyra
    Shader lampShader("4.0.lamp.vs", "4.0.lamp.fs");

    Shader ourShader("4.0.floor.vs", "4.0.floor.fs");
    //////////////////////////////////////////////////////

    // set up vertex data (and buffer(s)) and configure vertex attributes
    /// normal vectors
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    /*
    float floorVertices [] = {

        -50, 0.0, -50, 0.0, 0.0, 0.0, 0.0f, 1.0f,
        -50, 0.0, 50, 0.0, 0.0, 0.0, 0.0f, 0.0f,
        50, 0.0, -50, 0.0, 0.0, 0.0, 1.0f, 1.0f,
        50, 0.0, 50, 0.0, 0.0, 0.0, 1.0f, 0.0f,
    };
    */
    float floorVertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // top left
    };

    float floor_indices[] = {
        0, 1, 2,
        1, 2, 3
    };
    //////////////////////////////////////////////////////
    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //////////////////////////////////////////////////////

    //////////////////////////////////////////////////////
    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data

    ///////// IT IS 6 kell legyen a stride mert nem hasznaljuk a lampanal a normalt
    // ez nem baj met mar ugyis a GPU n van az adat
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int floorVAO, floorVBO, floorEBO;

    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glGenBuffers(1, &floorEBO);

    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floor_indices), floor_indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    std::string path2 = "texture_01.jpg";
    unsigned char *data = stbi_load(path2.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);

    std::vector <glm::vec3> cubePositions;
    int lastmix = -1;

    // render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // per-frame time logic
        // Kamera mozgas gyorsasagahoz
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if(forward){
            camera.ProcessKeyboard(FORWARD, deltaTime);
        } else if(backward){
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if(left){
            camera.ProcessKeyboard(LEFT, deltaTime);
        } else if(right){
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        camera.Position[1] = 1.1f;

        if(shoot){
            for(unsigned int i = 0; i < cubePositions.size(); i++){
                glm::vec3 closest = closest_point(camera.Front, camera.Position, cubePositions[i]);
                if(closest == camera.Position){
                    continue;
                }
                if(glm::distance(closest, cubePositions[i]) < 1){
                    std::cout << "hit " << i << "\n";
                    cubePositions.erase(cubePositions.begin() + i);
                    break;
                }
            }
        }

        if((int) currentFrame % 10 == 0 && int(currentFrame) != lastmix){
            lastmix = (int) currentFrame;
            cubePositions.clear();
            for(unsigned int i = 0; i < 10; i++ ){
                float xpos = rand() % 101 - 50;
                float zpos = rand() % 101 - 50;
                cubePositions.push_back(glm::vec3(xpos, 1.1f, zpos));
            }
        }
        //////////////////////////////////////////////////////
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);
        //////////////////////////////////////////////////////

        lightingShader.setFloat("a", a);
        lightingShader.setFloat("d", d);
        lightingShader.setFloat("s", s);

        //////////////////////////////////////////////////////
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        //////////////////////////////////////////////////////

        //////////////////////////////////////////////////////
        // render the cube
        // kockak rajzolasa
        glBindVertexArray(cubeVAO);

        for (unsigned int i = 0; i < cubePositions.size(); i++) {
            glm::mat4 model = glm::mat4(1.0f);
            cubePositions[i][1] += deltaTime * 1.0f;
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        {

            glm::mat4 model = glm::mat4(1.0f);

            glm::vec3 trans = camera.Position + camera.Front;
            model = glm::translate(model, trans);
            float dot = glm::dot(glm::normalize(camera.Front), glm::vec3(1.0f, 0.0f, 0.0f));
            float angle = glm::acos(dot);
            if(camera.Front[2] > 0){
                angle = glm::radians(360.0f) - angle;
            }
            //std::cout << glm::degrees(angle) << " -> " << dot << "\n";
            model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

            model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f) * 0.05f);
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);

        }
        //////////////////////////////////////////////////////

        //////////////////////////////////////////////////////
        // lampa rajzolasa
        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", glm::mat4(1.0f));
        glBindVertexArray(floorVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //////////////////////////////////////////////////////

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        if(a == 0)
            a = 1;
        else
            a = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        if(d == 0)
            d = 1;
        else
            d = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        if(s == 0)
            s = 1;
        else
            s = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        forward = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        backward = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        left = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        right = true;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        forward = false;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
        backward = false;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
        left = false;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
        right = false;
}

// FPS like kamera mozgas
// 1) Calculate the mouse's offset since the last frame.
// 2) Add the offset values to the camera's yaw and pitch values.
// 3) Add some constraints to the maximum/minimum pitch values
// 4) Calculate the direction vector

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        shoot = true;
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE){
        shoot = false;
    }
}

// zooming
// amikor a fov kisebb lesz akkor a projektalt space is kisebb lesz ami olyan hatast kelt mintha zoom olnank
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
