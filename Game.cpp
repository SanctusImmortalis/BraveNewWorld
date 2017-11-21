#include "Game.h"

//Brush brushes[1];

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormals;\n"
"layout (location = 2) in vec2 aTexCoords;\n"
"uniform mat4 model;\n"
"layout (std140) uniform Matrices{"
"mat4 view;\n"
"mat4 projection;\n"
"};\n"
"out vec2 tc;\n"
"out vec3 norm;\n"
"void main(){\n"
"gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
"norm = mat3(transpose(inverse(model))) * aNormals;\n"
"tc = aTexCoords;\n}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"in vec2 tc;\n"
"out vec4 FragColor;\n"
"uniform sampler2D tex;\n"
"void main(){\n"
"FragColor = texture(tex, tc);\n}\0";

Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 7.0f));
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float ultX = SCR_WIDTH / 2.0;
float ultY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool updateView = true, updateProj = true;

void initMap(GameMap* m){
  glGenBuffers(1, &(m->matrices));
  glBindBuffer(GL_UNIFORM_BUFFER, m->matrices);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, m->matrices, 0, 2 * sizeof(glm::mat4));
  std::vector<Vertex> v;
  std::vector<GLuint> i;
  Texture td, ts;
  int w, h, c;
  unsigned char* img = loadImg("wall.png", &w, &h, &c);
  glGenTextures(1, &(td.texHnd));
  glBindTexture(GL_TEXTURE_2D, td.texHnd);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    freeImg(img);

  Vertex vt;
  vt.position = glm::vec3(-5.0f, -5.0f, 0.0f);
  vt.texCoords = glm::vec2(0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, 0.0f);
  vt.texCoords = glm::vec2(5.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, 0.0f);
  vt.texCoords = glm::vec2(0.0f, 5.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, 0.0f);
  vt.texCoords = glm::vec2(5.0f, 5.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, -1.0f);
  vt.texCoords = glm::vec2(5.0f, 6.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, -1.0f);
  vt.texCoords = glm::vec2(5.0f, 1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, -5.0f, -1.0f);
  vt.texCoords = glm::vec2(5.0f, 1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, -1.0f);
  vt.texCoords = glm::vec2(5.0f, 6.0f);
  v.push_back(vt);


  i.push_back(0);
  i.push_back(1);
  i.push_back(2);

  i.push_back(1);
  i.push_back(3);
  i.push_back(2);

  i.push_back(1);
  i.push_back(4);
  i.push_back(3);

  i.push_back(1);
  i.push_back(5);
  i.push_back(4);

  i.push_back(5);
  i.push_back(6);
  i.push_back(4);

  i.push_back(4);
  i.push_back(6);
  i.push_back(7);

  i.push_back(0);
  i.push_back(2);
  i.push_back(6);

  i.push_back(2);
  i.push_back(7);
  i.push_back(6);

  i.push_back(2);
  i.push_back(3);
  i.push_back(7);

  i.push_back(7);
  i.push_back(3);
  i.push_back(4);

  i.push_back(0);
  i.push_back(1);
  i.push_back(6);

  i.push_back(6);
  i.push_back(1);
  i.push_back(5);

  //const char* vertexShaderSource = "#version 330 core\nlayout (location = 0) in vec3 aPos;layout (location = 1) in vec3 normals;layout (location = 2) in vec2 texCoords;out vec2 tc;uniform mat4 model;layout (std140) uniform Matrices{mat4 view; mat4 projection;};\nvoid main(){gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);tc = vec2(texCoords.x, texCoords.y);}";
  //const char* fragmentShaderSource = "#version 330 core\nin vec2 tc;out vec4 FragColor;uniform sampler2D tex;\nvoid main(){FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);}";
    Shader vs(0, false, vertexShaderSource);
  Shader fs(2, false, fragmentShaderSource);
  ShaderProgram* brushShader = new ShaderProgram(&vs, NULL, &fs);
  brushShader->use();

  brushShader->setBlockIndex("Matrices", 0);

  m->shaders[0] = brushShader;
  m->shadernum = 1;
  m->entnum = 0;

  //Brush* obj = new Brush(v, td, ts, i, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 0.0f, 45.0f), glm::vec3(0.5f, 0.5f, 0.5f));
  m->brushes[0] = new Brush(v, td, ts, i, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
  //m->brushes[1] = new Brush(v, td, ts, i, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));
  m->brushnum = 1;
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->InputTeclado(FRENTE, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->InputTeclado(TRAS, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->InputTeclado(ESQUERDA, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->InputTeclado(DIREITA, deltaTime);

        updateView = true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    updateProj = true;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
  {
      ultX = xpos;
      ultY = ypos;
      firstMouse = false;
  }

  float xoffset = xpos - ultX;
  float yoffset = ultY - ypos; // reversed since y-coordinates go from bottom to top

  ultX = xpos;
  ultY = ypos;

  camera->InputMouse(xoffset, yoffset);
  updateView = true;
}

Game::Game(){
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfw window creation
  // --------------------
  this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Brave New World", NULL, NULL);
    if (this->window == NULL)
    {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    this->m = new GameMap(initMap);
}

Game::~Game(){
  glfwTerminate();
}

void Game::mainLoop(){
  while(!glfwWindowShouldClose(this->window)){
    float frameAtual = glfwGetTime();
    deltaTime = frameAtual - lastFrame;
    lastFrame = frameAtual;

    if(updateView){
      glBindBuffer(GL_UNIFORM_BUFFER, this->m->matrices);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
    }
    if(updateProj){
      glBindBuffer(GL_UNIFORM_BUFFER, this->m->matrices);
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(glm::perspective(glm::radians(45.0f), ((float)SCR_WIDTH)/((float)SCR_HEIGHT), 0.1f, 100.0f)));
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    processInput(this->window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(int i = 0;i<(this->m)->entnum;i++){
      (this->m)->ents[i]->draw();
    }
    for(int i = 0;i<(this->m)->brushnum;i++){
      (this->m)->brushes[i]->draw(((this->m)->shaders[0]));
    }

    glfwPollEvents();
    glfwSwapBuffers(this->window);
  }
}
