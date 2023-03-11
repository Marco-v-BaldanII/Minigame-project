#include <math.h>
#include <iostream>
using namespace std;

#include "Entity.h"

Entity::Entity()
{
	is_alive = false;
}

Entity::~Entity() {};

void Entity::Init(int posx, int posy, int w, int h, int s) 
{
	x = posx;
	y = posy;
	width = w;
	height = h;
	speed = s;
	is_alive = true;
}

bool Entity::IsAlive() 
{
	return is_alive;
}

void Entity::ShutDown() 
{
	is_alive = false;
}

void Entity::GetRect(int* posx, int* posy, int* w, int* h) {
	*posx = x;
	*posy = y;
	*w = width;
	*h = height;
}

void Entity::SetY(int posy) { y = posy; }

int Entity::GetY() { return y; }

int Entity::GetHeight() { return height; }

void Entity::Move(int dx, int dy) {
	x += dx * speed;
	y += dy * speed;
}