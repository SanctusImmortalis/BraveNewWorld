#ifndef CAMERA_H
#define CAMERA_H

#include "Base.h"

enum Movimento_Cam{
  FRENTE,
  TRAS,
  ESQUERDA,
  DIREITA
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float VELOCIDADE = 10.0f;
const float SENSITIVIDADE = 0.1f;
const float Zoom = 45.0f;

class Camera
{
public:
  glm::vec3 Posicao;
  glm::vec3 Frente;
  glm::vec3 Up; // Da câmera
  glm::vec3 Direita;
  glm::vec3 UpMundo; //Self-Explanatory
  float Yaw; //Rotação Horizontal
  float Pitch;//Rotação Vertical
  float VelocidadeMovimento;
  float Sensitividade;
  float Zoom;

//Ele dá um construtor com vetor e um com os escalares
  Camera(glm::vec3 posicao = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Frente(glm::vec3(0.0f,0.0f, -1.0f)), VelocidadeMovimento(VELOCIDADE), Sensitividade(SENSITIVIDADE), Zoom(Zoom)
  {
    Posicao = posicao;
    UpMundo = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCamVectors();
  }


  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Frente(glm::vec3(0.0f, 0.0f, -1.0f)), VelocidadeMovimento(VELOCIDADE), Sensitividade(SENSITIVIDADE), Zoom(Zoom)
{
    Posicao = glm::vec3(posX, posY, posZ);
    UpMundo = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCamVectors();
}

glm::mat4 GetViewMatrix()
{
  return glm::lookAt(Posicao, Posicao + Frente, Up);
}

void InputTeclado(Movimento_Cam direcao, float deltaTime)
{
  float velocidade = VelocidadeMovimento * deltaTime;
  if(direcao == FRENTE)
    Posicao += glm::normalize(glm::vec3(Frente.x, 0.0f, Frente.z)) * velocidade;
  if(direcao == TRAS)
    Posicao -= glm::normalize(glm::vec3(Frente.x, 0.0f, Frente.z)) * velocidade;
  if(direcao == ESQUERDA)
    Posicao -= Direita * velocidade;
  if(direcao == DIREITA)
    Posicao += Direita * velocidade;
}

void InputMouse(float xoffset, float yoffset, GLboolean constrainPitch = true)
{
  xoffset *= Sensitividade;
  yoffset *= Sensitividade;
  Yaw += xoffset;
  Pitch += yoffset;

  if(constrainPitch)//Limita a câmera na sua rotação vertical para não bugar
  {
    if(Pitch > 89.0f)
      Pitch = 89.0f;
    if(Pitch < -89.0f)
      Pitch = -89.0f;
  }

  updateCamVectors();
}


void InputRodaMouse(float yoffset)
{
  if(Zoom >= 1.0f && Zoom <= 45.0f)
    Zoom -= yoffset;
  if(Zoom <= 1.0f)
    Zoom = 1.0f;
  if(Zoom >= 45.0f)
    Zoom = 45.0f;
}

private:
  void updateCamVectors()
  {
    glm::vec3 frente;
    frente.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    frente.y = sin(glm::radians(Pitch));
    frente.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    //Recalcula a direita e o Up da câmera
    Frente = glm::normalize(frente);
    Direita = glm::normalize(glm::cross(Frente, UpMundo));
    Up = glm::normalize(glm::cross(Direita, Frente));
  }


};
#endif
