#include "GamePlayer.h"

GamePlayer::GamePlayer(irr::core::vector3df sp, Timer pl_t, Timer pl_in, GamePlayer_Type ty) : spawnPoint(sp), pl_time(pl_t), pl_inv(pl_in), type(ty), physicsObject(){}