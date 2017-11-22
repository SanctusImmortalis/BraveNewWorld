#include "Game.h"

//Brush brushes[1];

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormals;\n"
"layout (location = 2) in vec2 aTexCoords;\n"
"uniform mat4 model;\n"
"uniform mat3 normalMat;\n"
"layout (std140) uniform Matrices{"
"mat4 view;\n"
"mat4 projection;\n"
"};\n"
"out vec2 tc;\n"
"out vec3 norm;\n"
"out vec3 FragPos;\n"
"void main(){\n"
"gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
"FragPos = vec3(model * vec4(aPos, 1.0f));\n"
"norm = normalMat * aNormals;\n"
"tc = aTexCoords;\n}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"in vec2 tc;\n"
"in vec3 norm;\n"
"in vec3 FragPos;\n"
"out vec4 FragColor;\n"
"uniform sampler2D diff;\n"
"uniform sampler2D spec;\n"
"layout (std140) uniform Lights{\n"
"vec3 light1;\nvec3 light2;\nvec3 light3;\nvec3 light4;\nvec3 light5;\n"
"};\n"
"vec4 samples[2];\n"
"void main(){\n"
"samples[0] = texture(diff, tc);\nsamples[1] = texture(spec, tc);\n"
"float distance1    = length(light1 - FragPos);"
"float distance2    = length(light2 - FragPos);"
"float distance3    = length(light3 - FragPos);"
"float distance4    = length(light4 - FragPos);"
"float distance5    = length(light5 - FragPos);"
"float attenuation1 = 1.0 / (1.0 + 0.35 * distance1 + 0.44 * (distance1 * distance1));"
"float attenuation2 = 1.0 / (1.0 + 0.35 * distance2 + 0.44 * (distance2 * distance2));"
"float attenuation3 = 1.0 / (1.0 + 0.35 * distance3 + 0.44 * (distance3 * distance3));"
"float attenuation4 = 1.0 / (1.0 + 0.35 * distance4 + 0.44 * (distance4 * distance4));"
"float attenuation5 = 1.0 / (1.0 + 0.35 * distance5 + 0.44 * (distance5 * distance5));"
"vec3 ambient = samples[0] * 0.1;\n"
"vec3 light1Dir = normalize(light1 - FragPos);\n"
"vec3 light2Dir = normalize(light2 - FragPos);\n"
"vec3 light3Dir = normalize(light3 - FragPos);\n"
"vec3 light4Dir = normalize(light4 - FragPos);\n"
"vec3 light5Dir = normalize(light5 - FragPos);\n"
"vec3 viewDir = normalize(viewPos - FragPos);\n"
"vec3 reflect1Dir = reflect(-light1Dir, norm);\n"
"vec3 reflect2Dir = reflect(-light2Dir, norm);\n"
"vec3 reflect3Dir = reflect(-light3Dir, norm);\n"
"vec3 reflect4Dir = reflect(-light4Dir, norm);\n"
"vec3 reflect5Dir = reflect(-light5Dir, norm);\n"
"vec3 diffuseLight = (max(dot(norm, light1Dir), 0.0) + attenuation1) * samples[0] + (max(dot(norm, light2Dir), 0.0) + attenuation2) * samples[0] + (max(dot(norm, light3Dir), 0.0) + attenuation3) * samples[0] + (max(dot(norm, light4Dir), 0.0) + attenuation4) * samples[0] + (max(dot(norm, light5Dir), 0.0) + attenuation5) * samples[0];\n"
"vec3 specularLight = (pow(max(dot(viewDir, reflect1Dir), 0.0), 32) + attenuation1) * samples[1] + (pow(max(dot(viewDir, reflect2Dir), 0.0), 32) + attenuation2) * samples[1] + (pow(max(dot(viewDir, reflect3Dir), 0.0), 32) + attenuation3) * samples[1] + (pow(max(dot(viewDir, reflect4Dir), 0.0), 32) + attenuation4) * samples[1] + (pow(max(dot(viewDir, reflect5Dir), 0.0), 32) + attenuation5) * samples[1];"
"FragColor = vec4(ambient + diffuseLight + specularLight*0.5, 1.0);\n}\0";

Camera* camera = new Camera(glm::vec3(-7.0f, 0.0f, 35.0f));
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

  glm::vec3 lightPositions[5];
  lightPositions[0] = glm::vec3(0.0f, 4.8f, 0.0f);
  lightPositions[1] = glm::vec3(0.0f, 4.8f, 27.5f);
  lightPositions[2] = glm::vec3(0.0f, 4.8f, -27.5f);
  lightPositions[3] = glm::vec3(27.5f, 4.8f, 0.0f);
  lightPositions[4] = glm::vec3(-27.5f, 4.8f, 0.0f);
  glGenBuffers(1, &(m->lights));
  glBindBuffer(GL_UNIFORM_BUFFER, m->lights);
  glBufferData(GL_UNIFORM_BUFFER, 80, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), lightPositions);
  glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(glm::vec3), lightPositions + 1);
  glBufferSubData(GL_UNIFORM_BUFFER, 32, sizeof(glm::vec3), lightPositions + 2);
  glBufferSubData(GL_UNIFORM_BUFFER, 48, sizeof(glm::vec3), lightPositions + 3);
  glBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(glm::vec3), lightPositions + 4);


  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(GL_UNIFORM_BUFFER, 1, m->lights, 0, 80);
  std::vector<Vertex> v;
  std::vector<GLuint> i;
  Texture tdw, tsw, tdf, tsf;
  int w, h, c;
  unsigned char* img = loadImg("wall.png", &w, &h, &c);
  unsigned char img1[] = {30, 30, 30, 255};
  unsigned char img2[] = {240, 240, 240, 255};
  glGenTextures(1, &(tdw.texHnd));
  glBindTexture(GL_TEXTURE_2D, tdw.texHnd);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    freeImg(img);
    glGenTextures(1, &(tsw.texHnd));
    glBindTexture(GL_TEXTURE_2D, tsw.texHnd);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, img1);
      glGenerateMipmap(GL_TEXTURE_2D);

    img = loadImg("floor.png", &w, &h, &c);
    glGenTextures(1, &(tdf.texHnd));
    glBindTexture(GL_TEXTURE_2D, tdf.texHnd);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
      glGenerateMipmap(GL_TEXTURE_2D);

      glBindTexture(GL_TEXTURE_2D, 0);

      freeImg(img);
      glGenTextures(1, &(tsf.texHnd));
      glBindTexture(GL_TEXTURE_2D, tsf.texHnd);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, img2);
        glGenerateMipmap(GL_TEXTURE_2D);



  Vertex vt;
  vt.position = glm::vec3(-5.0f, -5.0f, 0.5f);
  vt.texCoords = glm::vec2(0.0f, 0.0f);
  vt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, 0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 3.5f);
  vt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, 0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, -0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 3.5f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, -0.5f);
  vt.texCoords = glm::vec2(0.0f, 0.0f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, -0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, -0.5f);
  vt.texCoords = glm::vec2(0.0f, 0.0f);
  vt.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, -5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, -0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, -0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, -5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 3.5f);
  vt.normal = glm::vec3(0.0f, 0.0f, -1.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, -5.0f, 0.5f);
  vt.texCoords = glm::vec2(0.0f, 0.0f);
  vt.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, 0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, -5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, 0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 3.5f);
  vt.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, -5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, 0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 3.5f);
  vt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 3.5f);
  vt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, 5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 3.5f);
  vt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 3.5f);
  vt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, 5.0f, -0.5f);
  vt.texCoords = glm::vec2(0.0f, 3.5f);
  vt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, -5.0f, 0.5f);
  vt.texCoords = glm::vec2(0.0f, 0.0f);
  vt.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, -5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, 0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(-5.0f, -5.0f, -0.5f);
  vt.texCoords = glm::vec2(3.5f, 0.0f);
  vt.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  v.push_back(vt);
  vt.position = glm::vec3(5.0f, -5.0f, -0.5f);
  vt.texCoords = glm::vec2(0.0f, 0.0f);
  vt.normal = glm::vec3(0.0f, -1.0f, 0.0f);
  v.push_back(vt);


  i.push_back(0);
  i.push_back(1);
  i.push_back(2);
  i.push_back(3);
  i.push_back(4);
  i.push_back(5);
  i.push_back(6);
  i.push_back(7);
  i.push_back(8);
  i.push_back(9);
  i.push_back(10);
  i.push_back(11);
  i.push_back(12);
  i.push_back(13);
  i.push_back(14);
  i.push_back(15);
  i.push_back(16);
  i.push_back(17);
  i.push_back(18);
  i.push_back(19);
  i.push_back(20);
  i.push_back(21);
  i.push_back(22);
  i.push_back(23);
  i.push_back(24);
  i.push_back(25);
  i.push_back(26);
  i.push_back(27);
  i.push_back(28);
  i.push_back(29);
  i.push_back(30);
  i.push_back(31);
  i.push_back(32);
  i.push_back(33);
  i.push_back(34);
  i.push_back(35);

  //const char* vertexShaderSource = "#version 330 core\nlayout (location = 0) in vec3 aPos;layout (location = 1) in vec3 normals;layout (location = 2) in vec2 texCoords;out vec2 tc;uniform mat4 model;layout (stdw140) uniform Matrices{mat4 view; mat4 projection;};\nvoid main(){gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);tc = vec2(texCoords.x, texCoords.y);}";
  //const char* fragmentswhaderSource = "#version 330 core\nin vec2 tc;out vec4 FragColor;uniform sampler2D tex;\nvoid main(){FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);}";
    Shader vs(0, false, vertexShaderSource);
  Shader fs(2, false, fragmentShaderSource);
  ShaderProgram* brushShader = new ShaderProgram(&vs, NULL, &fs);
  brushShader->use();

  brushShader->setBlockIndex("Matrices", 0);
  brushShader->setBlockIndex("Lights", 1);

  m->shaders[0] = brushShader;
  m->shadernum = 1;
  m->entnum = 0;

  //Brush* obj = new Brush(v, tdw, tsw, i, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 0.0f, 45.0f), glm::vec3(0.5f, 0.5f, 0.5f));
  m->brushes[0] = new Brush(v, tdf, tsf, i, glm::vec3(0.0f, -5.5f, 0.0f), glm::vec3(90.0f, 00.0f, 0.0f), glm::vec3(7.5f, 7.5f, 1.0f));
  m->brushes[1] = new Brush(v, tdf, tsf, i, glm::vec3(0.0f, 5.5f, 0.0f), glm::vec3(90.0f, 00.0f, 0.0f), glm::vec3(7.5f, 7.5f, 1.0f));

  m->brushes[2] = new Brush(v, tdw, tsw, i, glm::vec3(0.0f, 0.0f, 38.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(3.0f, 1.0f, 1.0f));
  m->brushes[3] = new Brush(v, tdw, tsw, i, glm::vec3(-15.5f, 0.0f, 27.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  m->brushes[4] = new Brush(v, tdw, tsw, i, glm::vec3(15.5f, 0.0f, 27.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  m->brushes[5] = new Brush(v, tdw, tsw, i, glm::vec3(-11.25f, 0.0f, 17.0f), glm::vec3(00.0f, 00.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  m->brushes[6] = new Brush(v, tdw, tsw, i, glm::vec3(11.25f, 0.0f, 17.0f), glm::vec3(00.0f, 00.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));

  m->brushes[7] = new Brush(v, tdw, tsw, i, glm::vec3(0.0f, 0.0f, -38.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(3.0f, 1.0f, 1.0f));
  m->brushes[8] = new Brush(v, tdw, tsw, i, glm::vec3(-15.5f, 0.0f, -27.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  m->brushes[9] = new Brush(v, tdw, tsw, i, glm::vec3(15.5f, 0.0f, -27.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  m->brushes[10] = new Brush(v, tdw, tsw, i, glm::vec3(-11.25f, 0.0f, -17.0f), glm::vec3(00.0f, 00.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  m->brushes[11] = new Brush(v, tdw, tsw, i, glm::vec3(11.25f, 0.0f, -17.0f), glm::vec3(00.0f, 00.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));

  m->brushes[12] = new Brush(v, tdw, tsw, i, glm::vec3(-38.0f, 0.0f, 0.0f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(3.0f, 1.0f, 1.0f));
  m->brushes[13] = new Brush(v, tdw, tsw, i, glm::vec3(-27.5f, 0.0f, -15.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  m->brushes[14] = new Brush(v, tdw, tsw, i, glm::vec3(-27.5f, 0.0f, 15.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  m->brushes[15] = new Brush(v, tdw, tsw, i, glm::vec3(-17.0f, 0.0f, -11.25f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  m->brushes[16] = new Brush(v, tdw, tsw, i, glm::vec3(-17.0f, 0.0f, 11.25f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));

  m->brushes[17] = new Brush(v, tdw, tsw, i, glm::vec3(38.0f, 0.0f, 0.0f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(3.0f, 1.0f, 1.0f));
  m->brushes[18] = new Brush(v, tdw, tsw, i, glm::vec3(27.5f, 0.0f, -15.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  m->brushes[19] = new Brush(v, tdw, tsw, i, glm::vec3(27.5f, 0.0f, 15.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  m->brushes[20] = new Brush(v, tdw, tsw, i, glm::vec3(17.0f, 0.0f, -11.25f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  m->brushes[21] = new Brush(v, tdw, tsw, i, glm::vec3(17.0f, 0.0f, 11.25f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));

  m->brushes[22] = new Brush(v, tdw, tsw, i, glm::vec3(-8.0f, 0.0f, 12.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.8f, 1.0f, 1.0f));
  m->brushes[23] = new Brush(v, tdw, tsw, i, glm::vec3(8.0f, 0.0f, 12.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.8f, 1.0f, 1.0f));
  m->brushes[24] = new Brush(v, tdw, tsw, i, glm::vec3(-8.0f, 0.0f, -12.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.8f, 1.0f, 1.0f));
  m->brushes[25] = new Brush(v, tdw, tsw, i, glm::vec3(8.0f, 0.0f, -12.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.8f, 1.0f, 1.0f));

  m->brushes[26] = new Brush(v, tdw, tsw, i, glm::vec3(12.0f, 0.0f, -8.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.9f, 1.0f, 1.0f));
  m->brushes[27] = new Brush(v, tdw, tsw, i, glm::vec3(12.0f, 0.0f, 8.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.9f, 1.0f, 1.0f));
  m->brushes[28] = new Brush(v, tdw, tsw, i, glm::vec3(-12.0f, 0.0f, -8.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.9f, 1.0f, 1.0f));
  m->brushes[29] = new Brush(v, tdw, tsw, i, glm::vec3(-12.0f, 0.0f, 8.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.9f, 1.0f, 1.0f));

  //m->brushes[1] = new Brush(v, td, tsw, i, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));
  m->brushnum = 30;
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
