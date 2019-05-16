

#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <SFML\Graphics.hpp>

using namespace sf;
using namespace std;

// Trạng Thái Của Con Rắn
enum STATUS {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

// Tọa Độ Cho Một Đốt Của Con Rắn
struct POSITION
{
	float Column;
	float Line;
};

class SNAKE
{
private:
	CircleShape sprite;
	STATUS Status;
	POSITION Position[120];
	int Amount;       // Số Đốt Của Con Rắn
	int Mana;         // Thang Điểm Thưởng
public:
	// Constructor
	SNAKE();

	bool Die;               // True Là Rắn Chết
							// False Là Rắn Chưa Chết

	void ShowSnake(RenderWindow &, Texture &, Texture &);

	// Hàm Để Xử Lí Di Chuyển
	bool Process(float, float);           // Trả Vể True Là Ăn Được Mồi
										  // Trả Về False Là Chưa Ăn Được Mồi

	void setStatus(STATUS);
	STATUS getStatus();
};

#endif