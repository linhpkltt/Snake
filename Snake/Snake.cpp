

#include "Snake.hpp"

SNAKE::SNAKE()
{
	Die = false;

	Status = RIGHT;

	sprite.setRadius(5);
	Amount = 2;
	Position[0].Column = 100;
	Position[0].Line = 100;
	Position[1].Column = 90;// kh
	Position[1].Line = 100;

	for (int i = 2; i < 120; i++)
	{
		Position[i].Column = NULL;
		Position[i].Line = NULL;
	}
}

void SNAKE::ShowSnake(RenderWindow &window, Texture & Head, Texture & Body)
{

	// Vẽ Cái Đầu
	sprite.setTexture(&Head);
	sprite.setPosition(Position[0].Column, Position[0].Line);
	window.draw(sprite);

	// Vẽ Thân
	sprite.setTexture(&Body);

	for (int i = 1; i < Amount; i++)
	{
		sprite.setPosition(Position[i].Column, Position[i].Line);
		window.draw(sprite);
	}
}

bool SNAKE::Process(float Column, float Line)
{
	for (int i = Amount - 1; i > 0; i--)
		Position[i] = Position[i - 1];

	switch (Status)
	{
	case UP:
		Position[0].Line -= 10;
		break;
	case DOWN:
		Position[0].Line += 10;
		break;
	case LEFT:
		Position[0].Column -= 10;
		break;
	case RIGHT:
		Position[0].Column += 10;
		break;
	}

	// Xử Lí Chạm Thân

	for (int i = 1; i < Amount; i++)
	{
		if (Position[0].Column == Position[i].Column && Position[0].Line == Position[i].Line)
		{
			Die = true;
			break;
		}
	}

	if (Position[0].Column < 50.0 || Position[0].Column > 440.0) Die = true;
	if (Position[0].Line < 50.0 || Position[0].Line > 340.0) Die = true;

	if (Position[0].Column == Column && Position[0].Line == Line)
	{
		Amount++;
		for (int i = Amount - 1; i > 0; i--)
			Position[i] = Position[i - 1];
		return true;
	}
	else return false;
}

void SNAKE::setStatus(STATUS Tus)
{
	Status = Tus;
}

STATUS SNAKE::getStatus()
{
	return Status;
}