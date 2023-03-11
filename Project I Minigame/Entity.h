#pragma once

class Entity {
	
public:
	Entity();
	~Entity();

	void Init(int posx, int posy, int w, int h, int s);
	void ShutDown();
	void GetRect(int* posx, int* posy, int* w, int* h);
	void SetY(int posy);
	void Move(int dx, int dy);
	int GetY();
	int GetHeight();
	bool IsAlive();

private:
	int x, y;
	int width, height;
	int speed;
	bool is_alive;
};