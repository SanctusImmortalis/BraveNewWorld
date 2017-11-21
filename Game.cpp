#include "Game.h"

//Brush brushes[1];

void initMap(GameMap* m){
  std::vector<Vertex> v;
  std::vector<GLuint> i;
  Texture td, ts;

  Vertex vt;
  vt.position = glm::vec3(-1.0f, -1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(1.0f, -1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-1.0f, 1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(1.0f, 1.0f, 0.0f);
  v.push_back(vt);

  i.push_back(0);
  i.push_back(1);
  i.push_back(2);
  i.push_back(1);
  i.push_back(3);
  i.push_back(2);

  const char* vertexShaderSource = "#version 330 core\nlayout (location = 0) in vec3 aPos;layout (location = 1) in vec3 normals;layout (location = 2) in vec2 texCoords;out vec2 tc;uniform mat4 model;uniform mat4 view;uniform mat4 projection;\nvoid main(){gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);tc = vec2(texCoords.x, texCoords.y);}";
  const char* fragmentShaderSource = "#version 330 core\nin vec2 tc;out vec4 FragColor;uniform sampler2D tex;\nvoid main(){FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);}";
  glm::mat4 vm = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 pm = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    Shader vs(0, false, vertexShaderSource);
  Shader fs(2, false, fragmentShaderSource);

  ShaderProgram* brushShader = new ShaderProgram(&vs, NULL, &fs);

  brushShader->use();

  glUniformMatrix4fv(brushShader->getUniform("view"), 1, GL_FALSE, glm::value_ptr(vm));
  glUniformMatrix4fv(brushShader->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(pm));

  m->shaders = brushShader;
  m->shadernum = 1;
  m->entnum = 0;

  Brush* obj = new Brush(v, td, ts, i, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 0.0f, 45.0f), glm::vec3(0.5f, 0.5f, 0.5f));

  m->brushes = obj;
  m->brushnum = 1;
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Game::Game(){
  this->SCR_WIDTH = 800;
  this->SCR_HEIGHT = 600;
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfw window creation
  // --------------------
  this->window = glfwCreateWindow(this->SCR_WIDTH, this->SCR_HEIGHT, "Brave New World", NULL, NULL);
    if (this->window == NULL)
    {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return;
    }

    glEnable(GL_DEPTH_TEST);

    this->m = new GameMap(initMap);
}

Game::~Game(){
  glfwTerminate();
}

void Game::mainLoop(){
  while(!glfwWindowShouldClose(this->window)){
    processInput(this->window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0;i<(this->m)->entnum;i++){
      (this->m)->ents[i].draw();
    }
    for(int i = 0;i<(this->m)->brushnum;i++){
      (this->m)->brushes[i].draw(&((this->m)->shaders[0]));
    }

    glfwPollEvents();
    glfwSwapBuffers(this->window);
  }
}
