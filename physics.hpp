#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#define PHYSICS_FRAME (0.020)
#define MOVE_SPEED ( 4.5)
#define TURN_SPEED ( 1)
void moveUnits(Unit* us, int n, const Area& a,double dt=PHYSICS_FRAME);

#endif
