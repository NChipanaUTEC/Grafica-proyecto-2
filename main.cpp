#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "camera.h"
#include "Objeto.h"
// #include "BoundingVolume.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// variables globales
vector<Objeto*> pObjetos;
Esfera esfera_plantilla(vec3(0),2., 100, 100);
Esfera* esfera_prota = new Esfera(vec3(0,0,0),true); 
float esfera_angulo = 0.0;
float mouse_distancia = 0.0;

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(vec3(esfera_prota->centro.x, esfera_prota->centro.y, esfera_prota->centro.z+20.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float tiempoInicial = 0.0f, tiempoTranscurrido= 0.0f;

// lighting
glm::vec3 lightPos(30.2f, 10.0f, 2.0f);
GLint POSITION_ATTRIBUTE=0, NORMAL_ATTRIBUTE=1, TEXCOORD0_ATTRIBUTE=8;


void Escena1(){
    esfera_prota->vao = esfera_plantilla.vao;
    esfera_prota->indices_size = esfera_plantilla.indices_size;
    esfera_prota->radius = esfera_plantilla.radius;
    esfera_prota->calcularBoundingBox();
    esfera_prota->afectaGravedad = false;
    pObjetos.emplace_back(esfera_prota);

    Caja* caja = new Caja(vec3(-10,-103,-20),100.0f);
    caja->vao = caja->setup();
    caja->calcularBoundingBox();
    caja->afectaGravedad = false;
    pObjetos.emplace_back(caja);

    Esfera* esfera1 = new Esfera(vec3(80,-4,0));
    esfera1->vao = esfera_plantilla.vao;
    esfera1->indices_size = esfera_plantilla.indices_size;
    esfera1->radius = esfera_plantilla.radius;
    esfera1->calcularBoundingBox();
    esfera1->afectaGravedad = false;
    pObjetos.emplace_back(esfera1);

    Esfera* esfera2 = new Esfera(vec3(76,-4,0));
    esfera2->vao = esfera_plantilla.vao;
    esfera2->indices_size = esfera_plantilla.indices_size;
    esfera2->radius = esfera_plantilla.radius;
    esfera2->calcularBoundingBox();
    esfera2->afectaGravedad = false;
    pObjetos.emplace_back(esfera2);

    Esfera* esfera3 = new Esfera(vec3(84,-4,0));
    esfera3->vao = esfera_plantilla.vao;
    esfera3->indices_size = esfera_plantilla.indices_size;
    esfera3->radius = esfera_plantilla.radius;
    esfera3->calcularBoundingBox();
    esfera3->afectaGravedad = false;
    pObjetos.emplace_back(esfera3);

    Esfera* esfera4 = new Esfera(vec3(78,0,0));
    esfera4->vao = esfera_plantilla.vao;
    esfera4->indices_size = esfera_plantilla.indices_size;
    esfera4->radius = esfera_plantilla.radius;
    esfera4->calcularBoundingBox();
    esfera4->afectaGravedad = false;
    pObjetos.emplace_back(esfera4);

    Esfera* esfera5 = new Esfera(vec3(82,0,0));
    esfera5->vao = esfera_plantilla.vao;
    esfera5->indices_size = esfera_plantilla.indices_size;
    esfera5->radius = esfera_plantilla.radius;
    esfera5->calcularBoundingBox();
    esfera5->afectaGravedad = false;
    pObjetos.emplace_back(esfera5);

    Esfera* esfera6 = new Esfera(vec3(80,4,0));
    esfera6->vao = esfera_plantilla.vao;
    esfera6->indices_size = esfera_plantilla.indices_size;
    esfera6->radius = esfera_plantilla.radius;
    esfera6->calcularBoundingBox();
    esfera6->afectaGravedad = false;
    pObjetos.emplace_back(esfera6);
}

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("./2.2.basic_lighting.vs", "./2.2.basic_lighting.fs");

    esfera_plantilla.vao = esfera_plantilla.setup();
    Escena1();

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        tiempoTranscurrido = currentFrame - tiempoInicial; //static_cast<float>(glfwGetTime());
        processInput(window);
        // render
        glClearColor(128.0f/255.0f, 223.0f/255.0f, 255.0f/255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        if(pObjetos.size() > 2)
        lightingShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
        else lightingShader.setVec3("objectColor", 0.0f, 1.0f, 0.0f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // cuboShader.setMat4("projection", projection);
        // cuboShader.setMat4("view", view);
        for (auto &obj : pObjetos ) {
            obj->actualizarDatos(tiempoTranscurrido);
            obj->calcularColision(pObjetos);
            obj->display(lightingShader);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, luna_vao);
    //glDeleteVertexArrays(1, &lightCubeVAO);
    //glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && camera.Position.x > 0.0f)
        camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.MovementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && camera.Position.x < 40.0f)
        camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * camera.MovementSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && camera.Position.x == 0.0f && !esfera_prota->proyectil_lanzado){
        if (!esfera_prota->proyectil_listo){
            esfera_prota->calcularBoundingBox();
            esfera_prota->proyectil_listo = true;
            esfera_prota->proyectil_lanzado = true;
            esfera_prota->vel_ini = vec3(50*mouse_distancia,50*mouse_distancia,0);
            esfera_prota->pos_ini = esfera_prota->centro;
            esfera_prota->ang_ini = esfera_angulo - 180;
            esfera_prota->afectaGravedad = true;
            tiempoInicial = static_cast<float>(glfwGetTime());
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE){
        esfera_prota->proyectil_listo = false;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        esfera_prota->centro = vec3(0,0,0);
        esfera_prota->vel_ini = vec3(0,0,0);
        esfera_prota->pos_ini = vec3(0,0,0);
        esfera_prota->afectaGravedad = false;
        esfera_prota->proyectil_lanzado = false;
        esfera_prota->proyectil_listo = false;
    }
    //cout << "Posicion: " << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    ypos = SCR_HEIGHT - ypos;
    if (firstMouse) firstMouse = false;

    // get distance of mouse pointer to center of window
    mouse_distancia = sqrt(pow(xpos - SCR_WIDTH / 2, 2) + pow(ypos - SCR_HEIGHT / 2, 2)) / (sqrt(SCR_WIDTH * SCR_WIDTH + SCR_HEIGHT * SCR_HEIGHT)/2);
    esfera_angulo = atan2(ypos-SCR_HEIGHT/2, xpos-SCR_WIDTH/2)*180/M_PI;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
