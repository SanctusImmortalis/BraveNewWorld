#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "Base.h"
#include "Entity.h"

class Brush{
  
};

class GameMap{
private:
  std::vector<Entity> ents;
  std::vector<Brush> brushes;
};

#endif
