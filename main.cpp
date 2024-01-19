#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <glm/glm.hpp> //core glm functionality
#include <glm/gtc/matrix_transform.hpp> //glm extension for generating common transformation matrices
#include <glm/gtc/matrix_inverse.hpp> //glm extension for computing inverse matrices
#include <glm/gtc/type_ptr.hpp> //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"
#include "SkyBox.hpp"

#include <iostream>

// window
int glWindowWidth = 1550;
int glWindowHeight = 890;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;
const unsigned int SHADOW_WIDTH = 8192;
const unsigned int SHADOW_HEIGHT = 8192;
// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;
glm::mat3 lightDirMatrix;
GLuint lightDirMatrixLoc;

// light 
glm::vec3 lightDir;
glm::vec3 lightColor;

// shader uniform locations
GLint modelLoc;
GLint viewLoc;
GLint projectionLoc;
GLint normalMatrixLoc;
GLint lightDirLoc;
GLint lightColorLoc;

glm::vec3 pointLightPos = glm::vec3(0.219997f, 0.071731f, 6.52909f);
glm::vec3 pointLightPos1 = glm::vec3(1.62866f, 0.071731f, 6.60784f);
glm::vec3 pointLightPos2 = glm::vec3(1.91494f, 0.071731f, 5.48713f);
glm::vec3 pointLightPos3 = glm::vec3(3.05665f, 0.071731f, 5.64258f);
glm::vec3 pointLightPos4 = glm::vec3(3.24058f, 0.071731f, 4.15488f);
glm::vec3 pointLightPos5 = glm::vec3(4.06874f, 0.071731f, 3.29229f);
glm::vec3 pointLightPos6 = glm::vec3(0.612628f, 0.495077f, 7.95335f);
glm::vec3 pointLightPos7 = glm::vec3(15.2125f, 0.55954f, 8.825f);
glm::vec3 pointLightPos8 = glm::vec3(15.1111f, 0.559545f, 10.3608f);

glm::vec3 pointLightColor;
float pointLightConstant = 1.0f;
float pointLightLinear = 0.22f;
float pointLightQuadratic = 0.20f;

GLuint positionLoc;


// camera
gps::Camera myCamera(
    glm::vec3(0.0f, 2.0f, 5.5f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));

GLfloat cameraSpeed = 0.05f;

GLboolean pressedKeys[1024];

// models
gps::Model3D MainScene;
gps::Model3D teapot;
gps::Model3D boat;
gps::Model3D casa;
gps::Model3D casaMare;
gps::Model3D animale;
gps::Model3D stall;
gps::Model3D lumina;


// skybox
gps::SkyBox mySkyBox;

GLfloat angle;
GLfloat lightAngle = 0.0f;
GLuint shadowMapFBO;
GLuint depthMapTexture;
bool showDepthMap;

//fog
glm::vec3 ceata;
GLint ceataLoc;
bool ceataOn = false;

// shaders
gps::Shader myBasicShader;
gps::Shader skyboxShader;
gps::Shader lightCube;
gps::Shader depthMapShader;


 

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
    //TODO
    //for RETINA display
    glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

    myBasicShader.useShaderProgram();

    //set projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
    //send matrix data to shader
    GLint projLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    lightCube.useShaderProgram();

    glUniformMatrix4fv(glGetUniformLocation(lightCube.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    //set Viewport transform
    glViewport(0, 0, retina_width, retina_height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_M && action == GLFW_PRESS)
        showDepthMap = !showDepthMap;

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            pressedKeys[key] = true;
        else if (action == GLFW_RELEASE)
            pressedKeys[key] = false;
    }
}
//float yaw = 0.0f;
//float pitch = 0.0f;
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 1550.0f;
float lastY = 890.0f;
float fov = 90.0f;
float boatX = 0.0f;
float boatZ = 0.0f;

float teapotX = 0.0f;
float teapotZ = 0.0f;
float teapotY = 0.0f;
bool up, down, left, right;
const float sensitivity = 0.1f;

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // y de sus in jos
    lastX = xpos;
    lastY = ypos;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    myCamera.rotate(pitch, yaw);
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

}



void processMovement() {
    if (pressedKeys[GLFW_KEY_W]) {
        myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_S]) {
        myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_A]) {
        myCamera.move(gps::MOVE_LEFT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }

    if (pressedKeys[GLFW_KEY_D]) {
        myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    }
    if (pressedKeys[GLFW_KEY_DOWN])
    {
        boatX += 0.01f;
        down = true;
    }
    else
    {
        down = false;
    }

    if (pressedKeys[GLFW_KEY_UP])
    {
        boatX -= 0.01f;
        up = true;
    }
    else
    {

        up = false;
    }


    if (pressedKeys[GLFW_KEY_LEFT])
    {
        boatZ += 0.01f;
        left = true;
    }
    else {
        left = false;
    }


    if (pressedKeys[GLFW_KEY_RIGHT])
    {
        boatZ -= 0.01f;
        right = true;
    }
    else {
        right = false;
    }

    if (pressedKeys[GLFW_KEY_E]) {
        angle += 0.1f;
        if (angle > 360.0f)
            angle -= 360.0f;
    }

    if (pressedKeys[GLFW_KEY_Q]) {
        angle -= 0.1f;
        if (angle < 0.0f)
            angle += 360.0f;
    }

    if (pressedKeys[GLFW_KEY_O]) { // SMOOTH VIEW
        glDisable(GL_MULTISAMPLE);
        glEnable(GL_POINT_SMOOTH);
    }


    if (pressedKeys[GLFW_KEY_K]) {

        lightAngle += 0.3f;
        if (lightAngle > 360.0f)
            lightAngle -= 360.0f;
        glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
        myBasicShader.useShaderProgram();
        glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
    }

    if (pressedKeys[GLFW_KEY_L]) {
        lightAngle -= 0.3f;
        if (lightAngle < 0.0f)
            lightAngle += 360.0f;
        glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
        myBasicShader.useShaderProgram();
        glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
    }

    if (pressedKeys[GLFW_KEY_I])
    {
        lightColor = glm::vec3(0.0f, 0.0f, 0.0f); //white light
        glm::vec3 pointLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

    }
    if (pressedKeys[GLFW_KEY_U])
    {
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
        glm::vec3 pointLightColor = glm::vec3(0.0f, 0.0f, 0.0f);
        lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

    }
  
    if (pressedKeys[GLFW_KEY_G])
    {
        ceata.x = 0.0f;
        glUniform3fv(ceataLoc, 1, glm::value_ptr(ceata));
    }
    if (pressedKeys[GLFW_KEY_F])
    {
            ceata.x = 0.05f;
            glUniform3fv(ceataLoc, 1, glm::value_ptr(ceata));
            ceataOn = true;
    }
    
    //wireframe
    if (pressedKeys[GLFW_KEY_COMMA])
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //point
    if (pressedKeys[GLFW_KEY_PERIOD])
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    //fill
    if (pressedKeys[GLFW_KEY_SLASH])
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

bool initOpenGLWindow()
{
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    //window scaling for HiDPI displays
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    //for sRBG framebuffer
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

    //for antialising
    glfwWindowHint(GLFW_SAMPLES, 4);

    glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
    if (!glWindow) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
    glfwSetKeyCallback(glWindow, keyboardCallback);
    glfwSetCursorPosCallback(glWindow, mouseCallback);
    glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(glWindow);

    glfwSwapInterval(1);

#if not defined (__APPLE__)
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    //for RETINA display
    glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

    return true;
}


void initOpenGLState()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, retina_width, retina_height);

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

    glEnable(GL_FRAMEBUFFER_SRGB);
}


void initModels() {

    MainScene.LoadModel("models/Scena/ScenaFinala.obj");
    teapot.LoadModel("models/teapot/teapotFinalFinal.obj");
    boat.LoadModel("models/OldBoat_obj/barcaIntoarsa.obj");
    stall.LoadModel("models/Stall/standFinal.obj");
    casa.LoadModel("models/Casa/casa.obj");
    animale.LoadModel("models/animale/animaleFinal.obj");
    casaMare.LoadModel("models/animale/Casa/casaMare.obj");
    lumina.LoadModel("models/Sfera/lumina.obj");
}


void initShaders() {
    myBasicShader.loadShader("shaders/basic.vert", "shaders/basic.frag");
    myBasicShader.useShaderProgram();
    skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
    skyboxShader.useShaderProgram();
    lightCube.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
    depthMapShader.loadShader("shaders/umbre.vert", "shaders/umbre.frag");
}
void initFBOs()
{
    //generate FBO ID
    glGenFramebuffers(1, &shadowMapFBO);

    //create depth texture for FBO
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //attach texture to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix()
{
    const GLfloat near_plane = 1.0f, far_plane = 10.0f;
    glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
    glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
    glm::mat4 lightView = glm::lookAt(lightDirTr, myCamera.getCameraTarget(), glm::vec3(0.0f, 1.0f, 0.0f));
    return lightProjection * lightView;
}

void initUniforms() {
    myBasicShader.useShaderProgram();

    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

    view = myCamera.getViewMatrix();
    viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

    lightDirMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDirMatrix");

    projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 100.0f);
    projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //set the light direction (direction towards the light)
    lightDir = glm::vec3(0.0f, 1.0f, 2.0f);

    lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
    glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

    //set light color - pentru lumina directionala , o trimit si pentru lumina punctiforma
    lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
    lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

    
    //ceata
    ceata = glm::vec3(0.0f, 0.0f, 0.0f);
    ceataLoc = glGetUniformLocation(myBasicShader.shaderProgram, "ceata");
    glUniform3fv(ceataLoc, 1, glm::value_ptr(ceata));
    pointLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPos"), 1, glm::value_ptr(pointLightPos));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightColor"), 1, glm::value_ptr(pointLightColor));
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightConstant"), pointLightConstant);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightLinear"), pointLightLinear);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightQuadratic"), pointLightQuadratic);

    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPos1"), 1, glm::value_ptr(pointLightPos1));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightColor1"), 1, glm::value_ptr(pointLightColor));
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightConstant"), pointLightConstant);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightLinear"), pointLightLinear);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightQuadratic"), pointLightQuadratic);

    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPos2"), 1, glm::value_ptr(pointLightPos2));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightColor2"), 1, glm::value_ptr(pointLightColor));
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightConstant"), pointLightConstant);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightLinear"), pointLightLinear);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightQuadratic"), pointLightQuadratic);

    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPos3"), 1, glm::value_ptr(pointLightPos3));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightColor3"), 1, glm::value_ptr(pointLightColor));
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightConstant"), pointLightConstant);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightLinear"), pointLightLinear);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightQuadratic"), pointLightQuadratic);

    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPos4"), 1, glm::value_ptr(pointLightPos4));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightColor4"), 1, glm::value_ptr(pointLightColor));
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightConstant"), pointLightConstant);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightLinear"), pointLightLinear);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightQuadratic"), pointLightQuadratic);

    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPos5"), 1, glm::value_ptr(pointLightPos5));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightColor5"), 1, glm::value_ptr(pointLightColor));
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightConstant"), pointLightConstant);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightLinear"), pointLightLinear);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightQuadratic"), pointLightQuadratic);

    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPos6"), 1, glm::value_ptr(pointLightPos6));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightColor6"), 1, glm::value_ptr(pointLightColor));
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightConstant"), pointLightConstant);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightLinear"), pointLightLinear);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightQuadratic"), pointLightQuadratic);

    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPos7"), 1, glm::value_ptr(pointLightPos7));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightColor7"), 1, glm::value_ptr(pointLightColor));
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightConstant"), pointLightConstant);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightLinear"), pointLightLinear);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightQuadratic"), pointLightQuadratic);

    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightPos8"), 1, glm::value_ptr(pointLightPos8));
    glUniform3fv(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightColor8"), 1, glm::value_ptr(pointLightColor));
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightConstant"), pointLightConstant);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightLinear"), pointLightLinear);
    glUniform1f(glGetUniformLocation(myBasicShader.shaderProgram, "pointLightQuadratic"), pointLightQuadratic);

 
}

void renderMainScene(gps::Shader shader) {

    shader.useShaderProgram();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    view = myCamera.getViewMatrix();
    normalMatrix = view * model;
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glDisable(GL_CULL_FACE);

    MainScene.Draw(shader);
    glEnable(GL_CULL_FACE);
}

void renderAnimale(gps::Shader shader) {
    shader.useShaderProgram();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    view = myCamera.getViewMatrix();
    normalMatrix = view * model;
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    animale.Draw(shader);

}
void renderCasaMare(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	view = myCamera.getViewMatrix();
	normalMatrix = view * model;
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	casaMare.Draw(shader);

}

void renderLumina(gps::Shader shader) {
    shader.useShaderProgram();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = view * model;
    view = myCamera.getViewMatrix();
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    lumina.Draw(shader);

}

void renderTeapot(gps::Shader shader) {
    // select active shader program

    shader.useShaderProgram();
    //x2.48672 m  y-9.3938 m  z0.191508 m
   
    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    /// view = myCamera.getViewMatrix();
   ///  normalMatrix = view * model;
     //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 original_pos(2.48672f, 0.191508f, 9.3938);

    model = glm::translate(model, original_pos);
    model = glm::rotate(model, glm::radians(teapotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, -original_pos);
    model = glm::translate(model, glm::vec3(teapotX, teapotY, 0.0));

    // draw teapot
    teapot.Draw(shader);
}
void renderCasa(gps::Shader shader)
{
    shader.useShaderProgram();

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    view = myCamera.getViewMatrix();
    normalMatrix = view * model;
    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    casa.Draw(shader);

}
void renderStall(gps::Shader shader)
{
    shader.useShaderProgram();

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    view = myCamera.getViewMatrix();
    normalMatrix = view * model;
    //send teapot normal matrix data to shader
    glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    // draw teapot
    stall.Draw(shader);

}
void renderBoat(gps::Shader shader) {
    // select active shader program

    shader.useShaderProgram();

    //send teapot model matrix data to shader
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    view = myCamera.getViewMatrix();
   // normalMatrix = view * model;
    //send teapot normal matrix data to shader
   // glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));


    glm::mat4 model = glm::mat4(1.0f);
 

    // draw teapot
    boat.Draw(shader);
}
void initSkyBox() {

    std::vector<const GLchar*> faces;
    faces.push_back("skybox/right.tga");
    faces.push_back("skybox/left.tga");
    faces.push_back("skybox/top.tga");
    faces.push_back("skybox/bottom.tga");
    faces.push_back("skybox/back.tga");
    faces.push_back("skybox/front.tga");
    mySkyBox.Load(faces);
}


void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processMovement();
   
    depthMapShader.useShaderProgram();

    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(computeLightSpaceTrMatrix()));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

#pragma region Umbra_Scena

    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
    //send model matrix to shader
    glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    MainScene.Draw(depthMapShader);

#pragma endregion
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //OBIECTE

    myBasicShader.useShaderProgram();
    //send lightSpace matrix to shader
    glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightSpaceTrMatrix"),
        1,
        GL_FALSE,
        glm::value_ptr(computeLightSpaceTrMatrix()));

    //send view matrix to shader
    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "view"),
        1,
        GL_FALSE,
        glm::value_ptr(view));

    //compute light direction transformation matrix
    lightDirMatrix = glm::mat3(glm::inverseTranspose(view));
    //send lightDir matrix data to shader
    glUniformMatrix3fv(lightDirMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightDirMatrix)); //NORMALA

    glViewport(0, 0, retina_width, retina_height);
    myBasicShader.useShaderProgram();

    //bind the depth map
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "shadowMap"), 3);

    renderMainScene(myBasicShader);
    renderAnimale(myBasicShader);
    renderCasaMare(myBasicShader);
    renderStall(myBasicShader);
    renderCasa(myBasicShader);
    renderTeapot(myBasicShader);
    renderBoat(myBasicShader);
    renderLumina(myBasicShader);
    
    mySkyBox.Draw(skyboxShader, view, projection);

}


void cleanup() {
    glDeleteTextures(1, &depthMapTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &shadowMapFBO);
    glfwDestroyWindow(glWindow);
    //close GL context and any other GLFW resources
    glfwTerminate();
}

int main(int argc, const char* argv[]) {

    try {
        initOpenGLWindow();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    initOpenGLState();
    initFBOs();
    initModels();
    initSkyBox();
    initShaders();
    
    initUniforms();
    
    glCheckError();

    while (!glfwWindowShouldClose(glWindow)) {
        processMovement();
        renderScene();

        glfwPollEvents();
        glfwSwapBuffers(glWindow);
    }

    cleanup();

    return 0;

    return EXIT_SUCCESS;
}
