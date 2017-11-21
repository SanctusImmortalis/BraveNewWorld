#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include "Base.h"
#include "AABB.h"

class GameObject{
public:
  GameObject() {}
  bool checkCollision(AABB other);
protected:

};

#endif
