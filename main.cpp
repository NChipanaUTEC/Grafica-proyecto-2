#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Objeto.h"
#include "camera.h"
#include "shader_m.h"
// #include "BoundingVolume.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1240;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float tiempoInicial = 0.0f, tiempoTranscurrido = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
GLuint luna_vao;
int luna_numIndices;
GLint POSITION_ATTRIBUTE = 0, NORMAL_ATTRIBUTE = 1, TEXCOORD0_ATTRIBUTE = 8;

vector<Objeto *> pObjetos;
Esfera esfera_plantilla(vec3(0), 2., 100, 100);
Esfera *esfera_prota = new Esfera(vec3(0, 0, 0));
float esfera_angulo = 0.0;
float mouse_distancia = 0.0;
bool proyectil_listo = false;
bool proyectil_lanzado = false;

void Escena1() {
  esfera_prota->vao = esfera_plantilla.vao;
  esfera_prota->indices_size = esfera_plantilla.indices_size;
  esfera_prota->radius = esfera_plantilla.radius;
  esfera_prota->calcularBoundingBox();
  esfera_prota->afectaGravedad = false;
  pObjetos.emplace_back(esfera_prota);
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
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
  // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
  // Shader lightCubeShader("../2.2.light_cube.vs", "../2.2.light_cube.fs");

  esfera_plantilla.vao = esfera_plantilla.setup();
  Escena1();

  // render loop
  while (!glfwWindowShouldClose(window)) {
    // per-frame time logic
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    tiempoTranscurrido =
        currentFrame - tiempoInicial; // static_cast<float>(glfwGetTime());
    processInput(window);
    // render
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader.use();
    lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3("lightPos", lightPos);
    lightingShader.setVec3("viewPos", camera.Position);

    // view/projection transformations
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);

    // world transformation
    /*glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5));
    lightingShader.setMat4("model", model);
    if (aparece) {
        glBindVertexArray(luna_vao);
        glDrawElements(GL_TRIANGLES, luna_numIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }*/
    for (auto &esf : pObjetos) {
      esf->actualizarDatos(tiempoTranscurrido);
      // calcular si hay colision
      esf->calcularColision(pObjetos);
      esf->display(lightingShader);
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  // glDeleteVertexArrays(1, luna_vao);
  // glDeleteVertexArrays(1, &lightCubeVAO);
  // glDeleteBuffers(1, &VBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    if (!proyectil_listo && !proyectil_lanzado) {
      esfera_prota->calcularBoundingBox();
      proyectil_listo = true;
      proyectil_lanzado = true;
      esfera_prota->vel_ini =
          vec3(50 * mouse_distancia, 50 * mouse_distancia, 0);
      esfera_prota->pos_ini = esfera_prota->centro;
      esfera_prota->ang_ini = esfera_angulo - 180;
      esfera_prota->afectaGravedad = true;
      tiempoInicial = static_cast<float>(glfwGetTime());
    }
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
    proyectil_listo = false;
  }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  ypos = SCR_HEIGHT - ypos;
  if (firstMouse)
    firstMouse = false;

  // get distance of mouse pointer to center of window
  mouse_distancia =
      sqrt(pow(xpos - SCR_WIDTH / 2, 2) + pow(ypos - SCR_HEIGHT / 2, 2)) /
      (sqrt(SCR_WIDTH * SCR_WIDTH + SCR_HEIGHT * SCR_HEIGHT) / 2);
  esfera_angulo =
      atan2(ypos - SCR_HEIGHT / 2, xpos - SCR_WIDTH / 2) * 180 / M_PI;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
