#include "Game.h"

//Brush brushes[1];

const char* bvertexShaderSource = "#version 330 core\n"
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

const char* bfragmentShaderSource = "#version 330 core\n"
"in vec2 tc;\n"
"in vec3 norm;\n"
"in vec3 FragPos;\n"
"out vec4 FragColor;\n"
"uniform sampler2D diff;\n"
"uniform sampler2D spec;\n"
"layout (std140) uniform Lights{\n"
"vec3 light1;\nvec3 light2;\nvec3 light3;\nvec3 light4;\nvec3 light5;\n"
"vec3 viewPos;\n"
"};\n"
"vec4 samples[2];\n"
"void main(){\n"
"samples[0] = texture(diff, tc);\nsamples[1] = texture(spec, tc);\n"
"vec3 normie = normalize(norm);\n"

"float distance1    = length(light1 - FragPos);"
"float distance2    = length(light2 - FragPos);"
"float distance3    = length(light3 - FragPos);"
"float distance4    = length(light4 - FragPos);"
"float distance5    = length(light5 - FragPos);"
"float attenuation1 = 1.0 / (1.0 + 0.022 * distance1 + 0.0019 * (distance1 * distance1));"
"float attenuation2 = 1.0 / (1.0 + 0.022 * distance2 + 0.0019 * (distance2 * distance2));"
"float attenuation3 = 1.0 / (1.0 + 0.022 * distance3 + 0.0019 * (distance3 * distance3));"
"float attenuation4 = 1.0 / (1.0 + 0.022 * distance4 + 0.0019 * (distance4 * distance4));"
"float attenuation5 = 1.0 / (1.0 + 0.022 * distance5 + 0.0019 * (distance5 * distance5));"
"vec3 ambient = vec3(samples[0]) * 0.01;\n"

"vec3 light1Dir = normalize(light1 - FragPos);\n"
"vec3 light2Dir = normalize(light2 - FragPos);\n"
"vec3 light3Dir = normalize(light3 - FragPos);\n"
"vec3 light4Dir = normalize(light4 - FragPos);\n"
"vec3 light5Dir = normalize(light5 - FragPos);\n"
"vec3 viewDir = normalize(viewPos - FragPos);\n"
"vec3 reflect1Dir = reflect(-light1Dir, normie);\n"
"vec3 reflect2Dir = reflect(-light2Dir, normie);\n"
"vec3 reflect3Dir = reflect(-light3Dir, normie);\n"
"vec3 reflect4Dir = reflect(-light4Dir, normie);\n"
"vec3 reflect5Dir = reflect(-light5Dir, normie);\n"
"vec3 diffuseLight = (max(dot(normie, light1Dir), 0.0) * attenuation1) * vec3(samples[0]) + (max(dot(normie, light2Dir), 0.0) * attenuation2) * vec3(samples[0]) + (max(dot(normie, light3Dir), 0.0) * attenuation3) * vec3(samples[0]) + (max(dot(normie, light4Dir), 0.0) * attenuation4) * vec3(samples[0]) + (max(dot(normie, light5Dir), 0.0) * attenuation5) * vec3(samples[0]);\n"
"vec3 specularLight = (pow(max(dot(viewDir, reflect1Dir), 0.0), 64) * attenuation1) * vec3(samples[1]) + (pow(max(dot(viewDir, reflect2Dir), 0.0), 64) * attenuation2) * vec3(samples[1]) + (pow(max(dot(viewDir, reflect3Dir), 0.0), 64) * attenuation3) * vec3(samples[1]) + (pow(max(dot(viewDir, reflect4Dir), 0.0), 64) * attenuation4) * vec3(samples[1]) + (pow(max(dot(viewDir, reflect5Dir), 0.0), 64) * attenuation5) * vec3(samples[1]);"

"FragColor = vec4(ambient + diffuseLight*0.7 + specularLight*0.5, 1.0);\n}\0";

const char* geometryShaderSource = "#version 330 core\n"
"layout (triangles) in;\n"
"layout (line_strip, max_vertices = 2) out;\n"
"in vec3 norm[];\n"
"void main() {"
    "gl_Position = gl_in[0].gl_Position;"
    "EmitVertex();"
    "gl_Position = gl_in[0].gl_Position + vec4( normalize(norm[0]), 0.0) * 0.15;"
    "EmitVertex();"
    "EndPrimitive();"
    "gl_Position = gl_in[1].gl_Position;"
    "EmitVertex();"
    "gl_Position = gl_in[1].gl_Position + vec4( normalize(norm[1]), 0.0) * 0.15;"
    "EmitVertex();"
    "EndPrimitive();"
    "gl_Position = gl_in[2].gl_Position;"
    "EmitVertex();"
    "gl_Position = gl_in[2].gl_Position + vec4( normalize(norm[2]), 0.0) * 0.15;"
    "EmitVertex();"
    "EndPrimitive();"
"}";

const char* fSS = "#version 330 core\n"
"out vec4 FragColor;"
"void main(){"
    "FragColor = vec4(0.6, 0.6, 0.6, 1.0);"
"}";

const char* mgeometryShaderSource = "#version 330 core\n"
"layout (triangles) in;\n"
"layout (line_strip, max_vertices = 2) out;\n"
"in vec3 norm[];\n"
"in vec2 tc[];\n"
"out vec2 tec;"
"void main() {"
    "gl_Position = gl_in[0].gl_Position;"
    "tec = tc[0];"
    "EmitVertex();"
    "gl_Position = gl_in[1].gl_Position;"
    "tec = tc[1];"
    "EmitVertex();"
    "EndPrimitive();"
    "gl_Position = gl_in[1].gl_Position;"
    "tec = tc[1];"
    "EmitVertex();"
    "gl_Position = gl_in[2].gl_Position;"
    "tec = tc[2];"
    "EmitVertex();"
    "EndPrimitive();"
    "gl_Position = gl_in[2].gl_Position;"
    "tec = tc[2];"
    "EmitVertex();"
    "gl_Position = gl_in[0].gl_Position;"
    "tec = tc[0];"
    "EmitVertex();"
    "EndPrimitive();"
"}";

const char* mfSS = "#version 330 core\n"
"in vec2 tec;\n"
"in vec3 norm;\n"
"in vec3 FragPos;\n"
"out vec4 FragColor;\n"
"uniform sampler2D diff;\n"
"uniform sampler2D spec;\n"
"layout (std140) uniform Lights{\n"
"vec3 light1;\nvec3 light2;\nvec3 light3;\nvec3 light4;\nvec3 light5;\n"
"vec3 viewPos;\n"
"};\n"
"void main(){\n"
"FragColor = texture(diff, tec);\n}\0";
/*
const char* efragmentShaderSource = "#version 330 core\n"
"in vec2 tc;\n"
"in vec3 norm;\n"
"in vec3 FragPos;\n"
"out vec4 FragColor;\n"
//"struct Material{sampler2D diffuse[16];\nsampler2D specular[16];};\n"
//"uniform Material material;\n"
//"uniform int nrDiff;\nuniform int nrSpec;\n"
"layout (std140) uniform Lights{\n"
"vec3 light1;\nvec3 light2;\nvec3 light3;\nvec3 light4;\nvec3 light5;\n"
"vec3 viewPos;\n"
"};\n"
"vec4 samples[2];\n"
"void main(){\n"
"vec3 normie = normalize(norm);\n"

"float distance1    = length(light1 - FragPos);"
"float distance2    = length(light2 - FragPos);"
"float distance3    = length(light3 - FragPos);"
"float distance4    = length(light4 - FragPos);"
"float distance5    = length(light5 - FragPos);"
"float attenuation1 = 1.0 / (1.0 + 1.5 * distance1 + 2.7 * (distance1 * distance1));"
"float attenuation2 = 1.0 / (1.0 + 1.5 * distance2 + 2.7 * (distance2 * distance2));"
"float attenuation3 = 1.0 / (1.0 + 1.5 * distance3 + 2.7 * (distance3 * distance3));"
"float attenuation4 = 1.0 / (1.0 + 1.5 * distance4 + 2.7 * (distance4 * distance4));"
"float attenuation5 = 1.0 / (1.0 + 1.5 * distance5 + 2.7 * (distance5 * distance5));"
"vec3 ambient = vec3(samples[0]) * 0.1;\n"
"float alpha = 0.0;\n"
"vec3 light1Dir = normalize(light1 - FragPos);\n"
"vec3 light2Dir = normalize(light2 - FragPos);\n"
"vec3 light3Dir = normalize(light3 - FragPos);\n"
"vec3 light4Dir = normalize(light4 - FragPos);\n"
"vec3 light5Dir = normalize(light5 - FragPos);\n"
"vec3 viewDir = normalize(viewPos - FragPos);\n"
"vec3 reflect1Dir = reflect(-light1Dir, normie);\n"
"vec3 reflect2Dir = reflect(-light2Dir, normie);\n"
"vec3 reflect3Dir = reflect(-light3Dir, normie);\n"
"vec3 reflect4Dir = reflect(-light4Dir, normie);\n"
"vec3 reflect5Dir = reflect(-light5Dir, normie);\n"
"vec3 diffuseLight = vec3(0.0, 0.0, 0.0);\n"
"vec3 specularLight = vec3(0.0, 0.0, 0.0);\n"
"for(int i=0;i<nrDiff;i++){"
"samples[0] = texture(material.diffuse[i], tc);"
//"alpha += samples[0].w;\n"
"diffuseLight += (max(dot(normie, light1Dir), 0.0) * attenuation1) * vec3(samples[0]) + (max(dot(normie, light2Dir), 0.0) * attenuation2) * vec3(samples[0]) + (max(dot(normie, light3Dir), 0.0) * attenuation3) * vec3(samples[0]) + (max(dot(normie, light4Dir), 0.0) * attenuation4) * vec3(samples[0]) + (max(dot(normie, light5Dir), 0.0) * attenuation5) * vec3(samples[0]);\n"
"}\n"
"for(int i=0;i<nrSpec;i++){"
"samples[1] = texture(material.specular[i], tc);"
"specularLight += (pow(max(dot(viewDir, reflect1Dir), 0.0), 64) * attenuation1) * vec3(samples[1]) + (pow(max(dot(viewDir, reflect2Dir), 0.0), 64) * attenuation2) * vec3(samples[1]) + (pow(max(dot(viewDir, reflect3Dir), 0.0), 64) * attenuation3) * vec3(samples[1]) + (pow(max(dot(viewDir, reflect4Dir), 0.0), 64) * attenuation4) * vec3(samples[1]) + (pow(max(dot(viewDir, reflect5Dir), 0.0), 64) * attenuation5) * vec3(samples[1]);"
"}\n"
"FragColor = vec4(0.7, 0.0, 0.5, 1.0);\n}\0";
*/

const char* ppVS = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\nlayout (location = 1) in vec2 aTexCoords;\n"
"out vec2 tc;"
"void main(){"
"gl_Position = vec4(aPos, 1.0);\ntc = aTexCoords;}\n";

const char* ppFS0 = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 tc;\n"
"uniform sampler2D samp;\n"
"void main(){"
"FragColor = texture(samp, tc);}\n";

const char* ppFS1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 tc;\n"
"uniform sampler2D samp;\n"
"uniform float radius;"
"void main(){"
"vec3 sample = vec3(texture(samp, tc));"
"vec2 texC = tc - vec2(0.5, 0.5);"
"if(texC.x * texC.x + texC.y * texC.y < radius * radius){"
"float deriv = 1/(radius * sqrt(texC.x * texC.x + texC.y * texC.y));"
"texC = texC + vec2(0.5, 0.5);"
"sample = (1.0, 1.0, 1.0) - vec3(texture(samp, texC));"
"}"
"FragColor = vec4(sample, 1.0);}\n";

const char* ppFS2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 tc;\n"
"uniform sampler2D samp;\n"
"void main(){"
"vec3 sample = vec3(texture(samp, tc));"
"float avg = (sample.x + sample.y + sample.z) / 3;"
"FragColor = vec4(vec3(avg), 1.0);}\n";

const float sqrt2 = glm::sqrt(2.0f) / 2.0f;
ShaderProgram* ppSP[3];
GLuint FBOms, FBO, RBO, TEXms, TEX;
GLuint VAO, VBO, EBO;
float vt[] = {
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
  -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 0.0f, 1.0f, 1.0f
};
GLuint ind[] = {0, 1, 2, 1, 3, 2};
GLuint scrShader = 0;
float radius = 0.0f;
float elapsedTime = 0.0f;
unsigned int fur = 0;
unsigned int wiref = 0;
unsigned int geo = 0;
Camera* camera = new Camera(glm::vec3(-7.0f, 0.0f, 35.0f));
unsigned int SCR_WIDTH = 900;
unsigned int SCR_HEIGHT = 900;
float ultX = SCR_WIDTH / 2.0;
float ultY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool updateView = true, updateProj = true;

AABB bbs[32];

void checkError(){
  GLenum err = glGetError();
  switch(err){
    case GL_INVALID_ENUM:
    std::cerr << "Invalid enum error." << std::endl;
    break;
    case GL_INVALID_VALUE:
    std::cerr << "Invalid value error." << std::endl;
    break;
    case GL_INVALID_OPERATION:
    std::cerr << "Invalid op error." << std::endl;
    break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
    std::cerr << "Invalid FB error." << std::endl;
    break;
    case GL_OUT_OF_MEMORY:
    std::cerr << "Out of memory." << std::endl;
    break;
    /*
    case GL_STACK_UNDERFLOW:
    std::cerr << "Underflow." << std::endl;
    break;
    case GL_STACK_OVERFLOW:
    std::cerr << "Overflow." << std::endl;
    break;
*/
    case GL_NO_ERROR:
    //std::cerr << "Everything is fine." << std::endl;
    break;

    default:
    std::cerr << "Idk." << std::endl;
  }
}

void initMap(GameMap* m){
  glGenBuffers(1, &(m->matrices));
  glBindBuffer(GL_UNIFORM_BUFFER, m->matrices);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, m->matrices, 0, 2 * sizeof(glm::mat4));

  glm::vec3 lightPositions[5];
  lightPositions[0] = glm::vec3(0.0f, 3.5f, 0.0f);
  lightPositions[1] = glm::vec3(0.0f, 3.5f, 27.5f);
  lightPositions[2] = glm::vec3(0.0f, 3.5f, -27.5f);
  lightPositions[3] = glm::vec3(27.5f, 3.5f, 0.0f);
  lightPositions[4] = glm::vec3(-27.5f, 3.5f, 0.0f);
  glGenBuffers(1, &(m->lights));
  glBindBuffer(GL_UNIFORM_BUFFER, m->lights);
  glBufferData(GL_UNIFORM_BUFFER, 96, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), lightPositions);
  glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(glm::vec3), lightPositions + 1);
  glBufferSubData(GL_UNIFORM_BUFFER, 32, sizeof(glm::vec3), lightPositions + 2);
  glBufferSubData(GL_UNIFORM_BUFFER, 48, sizeof(glm::vec3), lightPositions + 3);
  glBufferSubData(GL_UNIFORM_BUFFER, 64, sizeof(glm::vec3), lightPositions + 4);
  //  glBufferSubData(GL_UNIFORM_BUFFER, 80, sizeof(glm::vec4), &(camera->Posicao));


  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(GL_UNIFORM_BUFFER, 1, m->lights, 0, 96);
  std::vector<Vertex> v;
  std::vector<GLuint> i;
  Texture tdw, tsw, tdf, tsf, tdm, tsm;
  Texture ttd1, tts1, ttd2, tts2, ttd3, tts3, ttd4, ttd5, ttd6;
  Texture lenna, lemmy, ednaldo;
  int w, h, c;
  unsigned char* img = loadImg("wall.png", &w, &h, &c);
  unsigned char img1[] = {30, 30, 30, 255};
  unsigned char img2[] = {240, 240, 240, 255};
  unsigned char img3[] = {255, 255, 255, 255};
  unsigned char td1[] = {160, 160, 0, 255};
  unsigned char ts1[] = {239, 239, 117, 255};
  unsigned char td2[] = {24, 130, 82, 255};
  unsigned char ts2[] = {255, 255, 255, 255};
  unsigned char td3[] = {94, 94, 94, 255};
  unsigned char ts3[] = {10, 10, 10, 255};
  unsigned char td4[] = {0, 255, 255, 255};
  unsigned char td5[] = {255, 255, 94, 255};
  unsigned char td6[] = {255, 0, 255, 255};
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
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

        glGenTextures(1, &(ttd1.texHnd));
        glBindTexture(GL_TEXTURE_2D, ttd1.texHnd);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, td1);
          glGenerateMipmap(GL_TEXTURE_2D);

          glGenTextures(1, &(tts1.texHnd));
          glBindTexture(GL_TEXTURE_2D, tts1.texHnd);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, ts1);
            glGenerateMipmap(GL_TEXTURE_2D);

            glGenTextures(1, &(ttd2.texHnd));
            glBindTexture(GL_TEXTURE_2D, ttd2.texHnd);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, td2);
              glGenerateMipmap(GL_TEXTURE_2D);

              glGenTextures(1, &(tts2.texHnd));
              glBindTexture(GL_TEXTURE_2D, tts2.texHnd);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, ts2);
                glGenerateMipmap(GL_TEXTURE_2D);

                glGenTextures(1, &(ttd3.texHnd));
                glBindTexture(GL_TEXTURE_2D, ttd3.texHnd);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, td3);
                  glGenerateMipmap(GL_TEXTURE_2D);

                  glGenTextures(1, &(tts3.texHnd));
                  glBindTexture(GL_TEXTURE_2D, tts3.texHnd);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, ts3);
                    glGenerateMipmap(GL_TEXTURE_2D);

                    img = loadImg("marble.png", &w, &h, &c);
                    glGenTextures(1, &(tdm.texHnd));
                    glBindTexture(GL_TEXTURE_2D, tdm.texHnd);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
                      glGenerateMipmap(GL_TEXTURE_2D);

                      glBindTexture(GL_TEXTURE_2D, 0);

                      freeImg(img);
                      glGenTextures(1, &(tsm.texHnd));
                      glBindTexture(GL_TEXTURE_2D, tsm.texHnd);
                      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, img3);
                        glGenerateMipmap(GL_TEXTURE_2D);

                        glGenTextures(1, &(ttd4.texHnd));
                        glBindTexture(GL_TEXTURE_2D, ttd4.texHnd);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, td4);
                          glGenerateMipmap(GL_TEXTURE_2D);

                          glGenTextures(1, &(ttd5.texHnd));
                          glBindTexture(GL_TEXTURE_2D, ttd5.texHnd);
                          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                          glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, td5);
                            glGenerateMipmap(GL_TEXTURE_2D);

                            glGenTextures(1, &(ttd6.texHnd));
                            glBindTexture(GL_TEXTURE_2D, ttd6.texHnd);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, td6);
                              glGenerateMipmap(GL_TEXTURE_2D);

                              float borderColor[] = { (float)79/(float)255, (float)51/(float)255, (float)19/(float)255, 1.0f };

                              img = loadImg("Lenna.png", &w, &h, &c);
                              glGenTextures(1, &(lenna.texHnd));
                              glBindTexture(GL_TEXTURE_2D, lenna.texHnd);
                              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
                                glGenerateMipmap(GL_TEXTURE_2D);
                                glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

                                glBindTexture(GL_TEXTURE_2D, 0);

                                freeImg(img);

                                img = loadImg("lemmy-vodka.png", &w, &h, &c);
                                glGenTextures(1, &(lemmy.texHnd));
                                glBindTexture(GL_TEXTURE_2D, lemmy.texHnd);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                              glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
                                  glGenerateMipmap(GL_TEXTURE_2D);
                                  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

                                  glBindTexture(GL_TEXTURE_2D, 0);

                                  freeImg(img);

                                  img = loadImg("Ednaldo_Pereira_3.png", &w, &h, &c);
                                  glGenTextures(1, &(ednaldo.texHnd));
                                  glBindTexture(GL_TEXTURE_2D, ednaldo.texHnd);
                                  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
                                    glGenerateMipmap(GL_TEXTURE_2D);
                                    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

                                    glBindTexture(GL_TEXTURE_2D, 0);

                                    freeImg(img);

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
    Shader bvs(0, false, bvertexShaderSource);
  Shader bfs(2, false, bfragmentShaderSource);
  Shader gs(1, false, geometryShaderSource);
  Shader fs(2, false, fSS);
  Shader mgs(1, false, mgeometryShaderSource);
  Shader mfs(2, false, mfSS);
  //Shader efs(2, false, efragmentShaderSource);
  ShaderProgram* brushShader = new ShaderProgram(&bvs, NULL, &bfs);
  ShaderProgram* bunnyShader = new ShaderProgram(&bvs, &gs, &fs);
  ShaderProgram* monkeyShader = new ShaderProgram(&bvs, &mgs, &mfs);
  //ShaderProgram* entityShader = new ShaderProgram(&bvs, NULL, &efs);
  brushShader->use();

  brushShader->setBlockIndex("Matrices", 0);
  brushShader->setBlockIndex("Lights", 1);
  bunnyShader->use();
  bunnyShader->setBlockIndex("Matrices", 0);
  monkeyShader->use();
  monkeyShader->setBlockIndex("Matrices", 0);

  m->shaders[0] = brushShader;
  m->shaders[1] = bunnyShader;
  m->shaders[2] = monkeyShader;
  //m->shaders[1] = entityShader;
  m->shadernum = 3;

  Model* tp1 = new Model("wt_teapot.obj", NULL);
  Model* tp2 = new Model("bunny.obj", NULL);
  Model* tp3 = new Model("monkey_head2.obj", NULL);
  /*
  std::vector<Vertex> v2;
  std::vector<GLuint> i2;
  for(int j=0;j<(car->meshes.size());j++){
    v2.insert(v2.end(), car->meshes[j]->vertices.begin(), car->meshes[j]->vertices.end());
    i2.insert(i2.end(), car->meshes[j]->indices.begin(), car->meshes[j]->indices.end());
  }
  */
  m->ents[0] = new Entity(tp1, m->shaders, ttd1, tts1, glm::vec3(27.5f, -2.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.7f, 1.7f, 1.7f));

  m->ents[1] = new Entity(tp1, m->shaders, ttd2, tts2, glm::vec3(27.5f, -2.0f, 5.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.7f, 1.7f, 1.7f));

  m->ents[2] = new Entity(tp1, m->shaders, ttd3, tts3, glm::vec3(27.5f, -2.0f, -5.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.7f, 1.7f, 1.7f));

  m->ents[3] = new Entity(tp2, m->shaders, ttd3, tts3, glm::vec3(-27.5f, -2.1f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(7.0f, 7.0f, 7.0f));

  m->ents[3]->setShaders(0, 1, 0);

  m->ents[4] = new Entity(tp3, m->shaders, ttd4, tts2, glm::vec3(0.0f, -2.0f, -27.5f), glm::vec3(00.0f, 00.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

  m->ents[5] = new Entity(tp3, m->shaders, ttd5, tts2, glm::vec3(-5.0f, -2.0f, -27.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

  m->ents[6] = new Entity(tp3, m->shaders, ttd6, tts2, glm::vec3(5.0f, -2.0f, -27.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

  m->entnum = 7;

  //Brush* obj = new Brush(v, tdw, tsw, i, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(30.0f, 0.0f, 45.0f), glm::vec3(0.5f, 0.5f, 0.5f));
  m->brushes[0] = new Brush(v, tdf, tsf, i, glm::vec3(0.0f, -5.5f, 0.0f), glm::vec3(90.0f, 00.0f, 0.0f), glm::vec3(7.5f, 7.5f, 1.0f));
  m->brushes[1] = new Brush(v, tdf, tsf, i, glm::vec3(0.0f, 5.5f, 0.0f), glm::vec3(90.0f, 00.0f, 0.0f), glm::vec3(7.5f, 7.5f, 1.0f));

  m->brushes[2] = new Brush(v, tdw, tsw, i, glm::vec3(0.0f, 0.0f, 38.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(3.0f, 1.0f, 1.0f));
  bbs[0].size = glm::vec2(30.0f, 1.0f);
  bbs[0].position = glm::vec2(0.0f, 38.0f) - bbs[0].size * 0.5f;
  m->brushes[3] = new Brush(v, tdw, tsw, i, glm::vec3(-15.5f, 0.0f, 27.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  bbs[1].size = glm::vec2(1.0f, 20.0f);
  bbs[1].position = glm::vec2(-15.5f, 27.5f) - bbs[1].size * 0.5f;
  m->brushes[4] = new Brush(v, tdw, tsw, i, glm::vec3(15.5f, 0.0f, 27.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  bbs[2].size = glm::vec2(1.0f, 20.0f);
  bbs[2].position = glm::vec2(15.5f, 27.5f) - bbs[1].size * 0.5f;
  m->brushes[5] = new Brush(v, tdw, tsw, i, glm::vec3(-11.25f, 0.0f, 17.0f), glm::vec3(00.0f, 00.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  bbs[3].size = glm::vec2(7.5f, 1.0f);
  bbs[3].position = glm::vec2(-11.25f, 17.0f) - bbs[3].size * 0.5f;
  m->brushes[6] = new Brush(v, tdw, tsw, i, glm::vec3(11.25f, 0.0f, 17.0f), glm::vec3(00.0f, 00.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  bbs[4].size = glm::vec2(7.5f, 1.0f);
  bbs[4].position = glm::vec2(11.25f, 17.0f) - bbs[4].size * 0.5f;

  m->brushes[7] = new Brush(v, tdw, tsw, i, glm::vec3(0.0f, 0.0f, -38.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(3.0f, 1.0f, 1.0f));
  bbs[5].size = glm::vec2(30.0f, 1.0f);
  bbs[5].position = glm::vec2(0.0f, -38.0f) - bbs[5].size * 0.5f;
  m->brushes[8] = new Brush(v, tdw, tsw, i, glm::vec3(-15.5f, 0.0f, -27.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  bbs[6].size = glm::vec2(1.0f, 20.0f);
  bbs[6].position = glm::vec2(-15.5f, -27.5f) - bbs[6].size * 0.5f;
  m->brushes[9] = new Brush(v, tdw, tsw, i, glm::vec3(15.5f, 0.0f, -27.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  bbs[7].size = glm::vec2(1.0f, 20.0f);
  bbs[7].position = glm::vec2(15.5f, -27.5f) - bbs[7].size * 0.5f;
  m->brushes[10] = new Brush(v, tdw, tsw, i, glm::vec3(-11.25f, 0.0f, -17.0f), glm::vec3(00.0f, 00.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  bbs[8].size = glm::vec2(7.5f, 1.0f);
  bbs[8].position = glm::vec2(-11.25f, -17.0f) - bbs[8].size * 0.5f;
  m->brushes[11] = new Brush(v, tdw, tsw, i, glm::vec3(11.25f, 0.0f, -17.0f), glm::vec3(00.0f, 00.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  bbs[9].size = glm::vec2(7.5f, 1.0f);
  bbs[9].position = glm::vec2(11.25f, -17.0f) - bbs[9].size * 0.5f;

  m->brushes[12] = new Brush(v, tdw, tsw, i, glm::vec3(-38.0f, 0.0f, 0.0f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(3.0f, 1.0f, 1.0f));
  bbs[10].size = glm::vec2(1.0f, 30.0f);
  bbs[10].position = glm::vec2(-38.0f, 0.0f) - bbs[10].size * 0.5f;
  m->brushes[13] = new Brush(v, tdw, tsw, i, glm::vec3(-27.5f, 0.0f, -15.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  bbs[11].size = glm::vec2(20.0f, 1.0f);
  bbs[11].position = glm::vec2(-27.5f, -15.5f) - bbs[11].size * 0.5f;
  m->brushes[14] = new Brush(v, tdw, tsw, i, glm::vec3(-27.5f, 0.0f, 15.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  bbs[12].size = glm::vec2(20.0f, 1.0f);
  bbs[12].position = glm::vec2(-27.5f, 15.5f) - bbs[12].size * 0.5f;
  m->brushes[15] = new Brush(v, tdw, tsw, i, glm::vec3(-17.0f, 0.0f, -11.25f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  bbs[13].size = glm::vec2(1.0f, 7.5f);
  bbs[13].position = glm::vec2(-17.0f, -11.25f) - bbs[13].size * 0.5f;
  m->brushes[16] = new Brush(v, tdw, tsw, i, glm::vec3(-17.0f, 0.0f, 11.25f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  bbs[14].size = glm::vec2(1.0f, 7.5f);
  bbs[14].position = glm::vec2(-17.0f, 11.25f) - bbs[14].size * 0.5f;

  m->brushes[17] = new Brush(v, tdw, tsw, i, glm::vec3(38.0f, 0.0f, 0.0f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(3.0f, 1.0f, 1.0f));
  bbs[15].size = glm::vec2(1.0f, 30.0f);
  bbs[15].position = glm::vec2(38.0f, 0.0f) - bbs[15].size * 0.5f;
  m->brushes[18] = new Brush(v, tdw, tsw, i, glm::vec3(27.5f, 0.0f, -15.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  bbs[16].size = glm::vec2(20.0f, 1.0f);
  bbs[16].position = glm::vec2(27.5f, -15.5f) - bbs[16].size * 0.5f;
  m->brushes[19] = new Brush(v, tdw, tsw, i, glm::vec3(27.5f, 0.0f, 15.5f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f));
  bbs[17].size = glm::vec2(20.0f, 1.0f);
  bbs[17].position = glm::vec2(27.5f, 15.5f) - bbs[17].size * 0.5f;
  m->brushes[20] = new Brush(v, tdw, tsw, i, glm::vec3(17.0f, 0.0f, -11.25f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  bbs[18].size = glm::vec2(1.0f, 7.5f);
  bbs[18].position = glm::vec2(17.0f, -11.25f) - bbs[18].size * 0.5f;
  m->brushes[21] = new Brush(v, tdw, tsw, i, glm::vec3(17.0f, 0.0f, 11.25f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.75f, 1.0f, 1.0f));
  bbs[19].size = glm::vec2(1.0f, 7.5f);
  bbs[19].position = glm::vec2(17.0f, 11.25f) - bbs[19].size * 0.5f;

  m->brushes[22] = new Brush(v, tdw, tsw, i, glm::vec3(-8.0f, 0.0f, 12.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.8f, 1.0f, 1.0f));
  bbs[20].size = glm::vec2(1.0f, 8.0f);
  bbs[20].position = glm::vec2(-8.0f, 12.5f) - bbs[20].size * 0.5f;
  m->brushes[23] = new Brush(v, tdw, tsw, i, glm::vec3(8.0f, 0.0f, 12.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.8f, 1.0f, 1.0f));
  bbs[21].size = glm::vec2(1.0f, 8.0f);
  bbs[21].position = glm::vec2(8.0f, 12.5f) - bbs[21].size * 0.5f;
  m->brushes[24] = new Brush(v, tdw, tsw, i, glm::vec3(-8.0f, 0.0f, -12.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.8f, 1.0f, 1.0f));
  bbs[22].size = glm::vec2(1.0f, 8.0f);
  bbs[22].position = glm::vec2(-8.0f, -12.5f) - bbs[22].size * 0.5f;
  m->brushes[25] = new Brush(v, tdw, tsw, i, glm::vec3(8.0f, 0.0f, -12.5f), glm::vec3(00.0f, 90.0f, 0.0f), glm::vec3(0.8f, 1.0f, 1.0f));
  bbs[23].size = glm::vec2(1.0f, 8.0f);
  bbs[23].position = glm::vec2(8.0f, -12.5f) - bbs[23].size * 0.5f;

  m->brushes[26] = new Brush(v, tdw, tsw, i, glm::vec3(12.0f, 0.0f, -8.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.9f, 1.0f, 1.0f));
  bbs[24].size = glm::vec2(9.0f, 1.0f);
  bbs[24].position = glm::vec2(12.0f, -8.0f) - bbs[24].size * 0.5f;
  m->brushes[27] = new Brush(v, tdw, tsw, i, glm::vec3(12.0f, 0.0f, 8.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.9f, 1.0f, 1.0f));
  bbs[25].size = glm::vec2(9.0f, 1.0f);
  bbs[25].position = glm::vec2(12.0f, 8.0f) - bbs[25].size * 0.5f;
  m->brushes[28] = new Brush(v, tdw, tsw, i, glm::vec3(-12.0f, 0.0f, -8.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.9f, 1.0f, 1.0f));
  bbs[26].size = glm::vec2(9.0f, 1.0f);
  bbs[26].position = glm::vec2(-12.0f, -8.0f) - bbs[26].size * 0.5f;
  m->brushes[29] = new Brush(v, tdw, tsw, i, glm::vec3(-12.0f, 0.0f, 8.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.9f, 1.0f, 1.0f));
  bbs[27].size = glm::vec2(9.0f, 1.0f);
  bbs[27].position = glm::vec2(-12.0f, 8.0f) - bbs[27].size * 0.5f;

  m->brushes[30] = new Brush(v, tdm, tsm, i, glm::vec3(27.5f, -4.0f, 0.0f), glm::vec3(0.0f, 00.0f, 0.0f), glm::vec3(0.3f, 0.4f, 2.5f));
  bbs[28].size = glm::vec2(3.0f, 2.5f);
  bbs[28].position = glm::vec2(27.5f, 0.0f) - bbs[28].size * 0.5f;
  m->brushes[31] = new Brush(v, tdm, tsm, i, glm::vec3(27.5f, -4.0f, 5.0f), glm::vec3(0.0f, 00.0f, 0.0f), glm::vec3(0.3f, 0.4f, 2.5f));
  bbs[29].size = glm::vec2(3.0f, 2.5f);
  bbs[29].position = glm::vec2(27.5f, 5.0f) - bbs[29].size * 0.5f;
  m->brushes[32] = new Brush(v, tdm, tsm, i, glm::vec3(27.5f, -4.0f, -5.0f), glm::vec3(0.0f, 00.0f, 0.0f), glm::vec3(0.3f, 0.4f, 2.5f));
  bbs[30].size = glm::vec2(3.0f, 2.5f);
  bbs[30].position = glm::vec2(27.5f, -5.0f) - bbs[30].size * 0.5f;
  m->brushes[33] = new Brush(v, tdm, tsm, i, glm::vec3(-27.5f, -4.0f, 0.0f), glm::vec3(0.0f, 00.0f, 0.0f), glm::vec3(0.3f, 0.4f, 2.5f));
  bbs[31].size = glm::vec2(3.0f, 2.5f);
  bbs[31].position = glm::vec2(-27.5f, 0.0f) - bbs[31].size * 0.5f;

  std::vector<Vertex> v2;
  std::vector<GLuint> i2;
  vt.position = glm::vec3(0.0f, -1.0f, -1.0f);
  vt.texCoords = glm::vec2(1.05f, -0.05f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v2.push_back(vt);
  vt.position = glm::vec3(0.0f, -1.0f, 1.0f);
  vt.texCoords = glm::vec2(-0.05f, -0.05f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v2.push_back(vt);
  vt.position = glm::vec3(0.0f, 1.0f, -1.0f);
  vt.texCoords = glm::vec2(1.05f, 1.05f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v2.push_back(vt);
  vt.position = glm::vec3(0.0f, 1.0f, 1.0f);
  vt.texCoords = glm::vec2(-0.05f, 1.05f);
  vt.normal = glm::vec3(1.0f, 0.0f, 0.0f);
  v2.push_back(vt);

  i2.push_back(0);
  i2.push_back(1);
  i2.push_back(2);
  i2.push_back(1);
  i2.push_back(3);
  i2.push_back(2);

  m->brushes[34] = new Brush(v2, lenna, tsm, i2, glm::vec3(-14.8f, 0.0f, 27.5f), glm::vec3(0.0f, 00.0f, 0.0f), glm::vec3(1.0f, 2.5f, 2.5f));
  m->brushes[35] = new Brush(v2, lemmy, tsm, i2, glm::vec3(14.8f, 0.0f, 22.5f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.0f, 1.35f, 2.4f));
  m->brushes[36] = new Brush(v2, ednaldo, tsm, i2, glm::vec3(14.8f, 0.0f, 32.5f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.0f, 1.69f, 2.138f));

  //m->brushes[1] = new Brush(v, td, tsw, i, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(00.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f));
  m->brushnum = 37;
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->InputTeclado(FRENTE, deltaTime, bbs);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->InputTeclado(TRAS, deltaTime, bbs);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->InputTeclado(ESQUERDA, deltaTime, bbs);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->InputTeclado(DIREITA, deltaTime, bbs);


        updateView = true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_E && action == GLFW_PRESS && scrShader != 2)
        fur = (fur + 1) % 2;
    if (key == GLFW_KEY_Q && action == GLFW_PRESS && scrShader != 2)
        geo = (geo + 2) % 4;
    if (key == GLFW_KEY_R && action == GLFW_PRESS && scrShader == 0){
        scrShader = 1;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;

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
  GLFWmonitor* mon = glfwGetPrimaryMonitor();
  SCR_WIDTH = glfwGetVideoMode(mon)->width;
  SCR_HEIGHT = glfwGetVideoMode(mon)->height;
  // glfw window creation
  // --------------------
  this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Brave New World", mon, NULL);
    if (this->window == NULL)
    {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return;
    }

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);

    this->m = new GameMap(initMap);
}

Game::~Game(){
  glfwTerminate();
}

void Game::mainLoop(){

  Shader stdVS(0, false, ppVS);
  Shader stdFS(2, false, ppFS0);
  Shader zwFS1(2, false, ppFS1);
  Shader zwFS2(2, false, ppFS2);

  ppSP[0] = new ShaderProgram(&stdVS, NULL, &stdFS);

  ppSP[0]->use();

  glUniform1i(ppSP[0]->getUniform("samp"), 0);

  ppSP[1] = new ShaderProgram(&stdVS, NULL, &zwFS1);

  ppSP[1]->use();

  glUniform1i(ppSP[1]->getUniform("samp"), 0);

  ppSP[2] = new ShaderProgram(&stdVS, NULL, &zwFS2);

  ppSP[2]->use();

  glUniform1i(ppSP[2]->getUniform("samp"), 0);

  glGenFramebuffers(1, &FBOms);
  glBindFramebuffer(GL_FRAMEBUFFER, FBOms);
  glGenTextures(1, &TEXms);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, TEXms);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, TEXms, 0);

  glGenRenderbuffers(1, &RBO);
  glBindRenderbuffer(GL_RENDERBUFFER, RBO);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer 1 is not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);


  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glGenTextures(1, &TEX);
  glBindTexture(GL_TEXTURE_2D, TEX);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEX, 0);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer 2 is not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);


  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);



  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vt), vt, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


  while(!glfwWindowShouldClose(this->window)){
    checkError();
    float frameAtual = glfwGetTime();
    deltaTime = frameAtual - lastFrame;
    lastFrame = frameAtual;

    processInput(this->window);

    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, FBOms);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      m->ents[3]->setActiveShaders(fur + 1);
      m->ents[4]->setShaders(geo, 0, 0);
      m->ents[5]->setShaders(geo, 0, 0);
      m->ents[6]->setShaders(geo, 0, 0);
      wiref = (wiref + 2 - scrShader) % 360;
      m->ents[4]->br->rotation = glm::vec3(0.0f, (float)wiref, 0.0f);
      m->ents[4]->br->updateModel = true;
      m->ents[5]->br->rotation = glm::vec3(0.0f, 0.0f, (float)wiref);
      m->ents[5]->br->updateModel = true;
      m->ents[6]->br->rotation = glm::vec3((float)wiref, 0.0f, 0.0f);
      m->ents[6]->br->updateModel = true;

    if(updateView){
      glBindBuffer(GL_UNIFORM_BUFFER, this->m->matrices);
      glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
      glBindBuffer(GL_UNIFORM_BUFFER, this->m->lights);
      glBufferSubData(GL_UNIFORM_BUFFER, 80, sizeof(glm::vec3), &(camera->Posicao));
    }
    if(updateProj){
      glBindBuffer(GL_UNIFORM_BUFFER, this->m->matrices);
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(glm::perspective(glm::radians(45.0f), ((float)SCR_WIDTH)/((float)SCR_HEIGHT), 0.1f, 100.0f)));
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    for(int i = 0;i<(this->m)->entnum;i++){
      (this->m)->ents[i]->draw();
    }
    for(int i = 0;i<(this->m)->brushnum;i++){
      (this->m)->brushes[i]->draw(((this->m)->shaders[0]));
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBOms);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glDrawBuffer(GL_BACK);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    ppSP[scrShader]->use();

    if(scrShader == 1){
      radius += 0.5f * deltaTime;
      glUniform1f(ppSP[1]->getUniform("radius"), radius);
      if(radius >= sqrt2){
        scrShader = 2;
        radius = 0.0f;
      }
    }

    if(scrShader == 2){
      elapsedTime += deltaTime;
      if(elapsedTime >= 5.0f){
        scrShader = 0;
        elapsedTime = 0.0f;
      }
    }

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TEX);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);


    glfwPollEvents();
    glfwSwapBuffers(this->window);
  }
}
