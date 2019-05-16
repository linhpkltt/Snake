#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include "Snake.hpp"

using namespace std;
using namespace sf;

const string PathImages = "Resource/Images/";
const string PathFonts = "Resource/Fonts/MarkerFelt.ttf";
const string PathMusics = "Resource/Musics/";
const string PathFiles = "Resource/Files/HighScore.txt";
unsigned int Score;


// tạo hoa quả
struct FRUIT
{
	int x;
	int y;
	CircleShape sprite;
	FRUIT(Texture &texture)
	{
		sprite.setTexture(&texture);
		x = (rand() % 39 + 1) * 10 + 50;
		y = (rand() % 29 + 1) * 10 + 50;
		sprite.setPosition(x, y);
		sprite.setRadius(5);
	}
};


// xử lý file ghi điểm cao 
void HighScore()
{
	// Đọc File
	ifstream Read;
	Read.open(PathFiles, ios_base::in);

	unsigned int Score[5];

	for (int i = 0; i < 5; i++)
		Read >> Score[i];

	Read.close();

	if (Score[4] < ::Score)
	{
		Score[4] = ::Score;
		for (int i = 0; i < 5; i++)
			for (int j = i; j < 5; j++)
				if (Score[i] < Score[j])
				{
					Score[i] = Score[i] + Score[j];
					Score[j] = Score[i] - Score[j];
					Score[i] = Score[i] - Score[j];
				}
		//viết đè lên file điểm mới chơi
		ofstream Write;
		Write.open(PathFiles, ios_base::out);
		Write.clear();
		for (int i = 0; i < 5; i++)
		{
			Write << Score[i];
			Write << "\n";
		}

		Write.close();
	}
}

//tạo nút highscore
void HighScore(RenderWindow &window)
{
	// Đọc File
	ifstream Read;
	Read.open(PathFiles, ios_base::in);

	unsigned int * Score = new unsigned int[5];
	string SCORE[5];
	for (int i = 0; i < 5; i++)
	{
		Read >> *(Score + i);
		SCORE[i] = to_string(*(Score + i));
	}
	delete Score;
	Read.close();

	Texture Background;
	Background.loadFromFile(PathImages + "HighScore.png");
	Sprite MainBackground(Background);

	Font font;
	font.loadFromFile(PathFonts);
	Text Point;
	Point.setFont(font);
	Point.setCharacterSize(18);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		for (int i = 0; i < 5; i++)
		{
			Point.setString(SCORE[i]);
			Point.setPosition(280, i * 40 + 150);
			window.draw(Point);
		}
		window.display();
		sleep(microseconds(50));
		if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= 600 && Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= 400) break;
		if (Keyboard::isKeyPressed(Keyboard::Escape)) break;
	}
}

//tạo nút tác giả
void Author(RenderWindow &window)
{
	Texture Background;
	Background.loadFromFile(PathImages + "Author.png");
	Sprite MainBackground(Background);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		window.display();
		sleep(microseconds(50));
		if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= 600 && Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= 400) break;
		if (Keyboard::isKeyPressed(Keyboard::Escape)) break;
	}
}

// xử lý game
void PlayGame(RenderWindow &window)
{
	Texture Head, Body, Fruits, Background;
	Head.loadFromFile(PathImages + "Head.png");
	switch (rand() % 3)
	{
	case 0:
		Body.loadFromFile(PathImages + "Red.png");
		break;
	case 1:
		Body.loadFromFile(PathImages + "Blue.png");
		break;
	case 2:
		Body.loadFromFile(PathImages + "Green.png");
		break;
	}
	Fruits.loadFromFile(PathImages + "Fruit.png");
	Background.loadFromFile(PathImages + "MainBackground.png");

	Sprite MainBackground(Background);

	SNAKE Snake;
	FRUIT Fruit(Fruits);

	Score = 0;

	Font font;
	font.loadFromFile(PathFonts);

	Text SCORE;
	SCORE.setFont(font);
	SCORE.setString("Score: " + to_string(Score));
	SCORE.setFillColor(Color::Yellow);
	SCORE.setPosition(490, 40);
	SCORE.setCharacterSize(18);

	// xử lý âm thanh
	SoundBuffer start, eatfruit, end, beep;
	start.loadFromFile(PathMusics + "Start.wav");
	eatfruit.loadFromFile(PathMusics + "EatFruit.wav");
	end.loadFromFile(PathMusics + "EndGame.wav");
	beep.loadFromFile(PathMusics + "Beep.wav");

	Sound Start(start), End(end), Beep(beep), EatFruit(eatfruit);
	Start.play();

	while (!Snake.Die)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		window.draw(SCORE);
		window.draw(Fruit.sprite);

		if (Snake.Process(Fruit.x, Fruit.y))
		{
			Fruit.x = (rand() % 39 + 1) * 10 + 50;
			Fruit.y = (rand() % 29 + 1) * 10 + 50;
			Fruit.sprite.setPosition(Fruit.x, Fruit.y);
			Score += 10;
			SCORE.setString("Score: " + to_string(Score));
			EatFruit.play();
		}

		// điều khiển rắn
		Snake.ShowSnake(window, Head, Body);

		if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A)) && Snake.getStatus() != RIGHT)
		{
			Snake.setStatus(LEFT);
			Beep.play();
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)) && Snake.getStatus() != LEFT)
		{
			Snake.setStatus(RIGHT);
			Beep.play();
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)) && Snake.getStatus() != DOWN)
		{
			Snake.setStatus(UP);
			Beep.play();
		}
		if ((Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::S)) && Snake.getStatus() != UP)
		{
			Snake.setStatus(DOWN);
			Beep.play();
		}

		window.display();
		sleep(milliseconds(70));
		
	}

	End.play();

	while (End.getStatus() == End.Playing && window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
	}

	Background.loadFromFile(PathImages + "GameOver.png");
	MainBackground.setTexture(Background);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(MainBackground);
		window.display();
		if (Mouse::isButtonPressed(Mouse::Left) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= 600 && Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= 400) break;
		sleep(microseconds(50));
		
	}

	HighScore();
}

void Menu(RenderWindow &window)
{
	Texture Back;
	Back.loadFromFile(PathImages + "Main.png");

	Sprite Background(Back);

	Font font;
	font.loadFromFile(PathFonts);

	Text Play, Aut, High, Quit;
	Play.setString("Play");
	Aut.setString("Author");
	High.setString("High Score");
	Quit.setString("Quit");

	Play.setFont(font);
	Aut.setFont(font);
	High.setFont(font);
	Quit.setFont(font);

	Play.setCharacterSize(18);
	Aut.setCharacterSize(18);
	High.setCharacterSize(18);
	Quit.setCharacterSize(18);

	Play.setPosition(100, 220);
	Aut.setPosition(100, 260);
	High.setPosition(100, 300);
	Quit.setPosition(100, 340);

	Play.setFillColor(Color::Yellow);
	Aut.setFillColor(Color::Yellow);
	High.setFillColor(Color::Yellow);
	Quit.setFillColor(Color::Yellow);

	Music music;
	music.openFromFile(PathMusics + "Main.wav");
	music.setLoop(true);
	music.play();

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
		}
		window.clear();
		window.draw(Background);
		window.draw(Play);
		window.draw(Aut);
		window.draw(High);
		window.draw(Quit);
		window.display();
		sleep(microseconds(50));

		if (Mouse::getPosition(window).x >= 100 && Mouse::getPosition(window).x <= 130 && Mouse::getPosition(window).y >= 220 && Mouse::getPosition(window).y <= 236)
		{
			Play.setFillColor(Color::Magenta);
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				music.pause();
				PlayGame(window);
				music.play();
			}
		}
		else Play.setFillColor(Color::Yellow);

		if (Mouse::getPosition(window).x >= 100 && Mouse::getPosition(window).x <= 150 && Mouse::getPosition(window).y >= 260 && Mouse::getPosition(window).y <= 275)
		{
			Aut.setFillColor(Color::Magenta);
			if (Mouse::isButtonPressed(Mouse::Left)) Author(window);
		}
		else Aut.setFillColor(Color::Yellow);

		if (Mouse::getPosition(window).x >= 100 && Mouse::getPosition(window).x <= 177 && Mouse::getPosition(window).y >= 300 && Mouse::getPosition(window).y <= 320)
		{
			High.setFillColor(Color::Magenta);
			if (Mouse::isButtonPressed(Mouse::Left)) HighScore(window);
		}
		else High.setFillColor(Color::Yellow);

		if (Mouse::getPosition(window).x >= 100 && Mouse::getPosition(window).x <= 130 && Mouse::getPosition(window).y >= 340 && Mouse::getPosition(window).y <= 360)
		{
			Quit.setFillColor(Color::Magenta);
			if (Mouse::isButtonPressed(Mouse::Left)) break;
		}
		else Quit.setFillColor(Color::Yellow);
	}
	music.stop();
}

int main()
{
	RenderWindow window(VideoMode(600, 400), "Game Snake - Phung Thi Khanh Linh", Style::Default);

	srand(time(NULL));

	Menu(window);

	return EXIT_SUCCESS;
}