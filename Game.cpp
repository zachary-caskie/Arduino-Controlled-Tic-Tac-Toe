#include "Game.h"
//Input Vars
sf::Clock Clock;
float inputDelay = 0.2;

sf::Color player1(147, 173, 244);
sf::Color grid1(48, 94, 226);
sf::Color player2(244, 147, 154);
sf::Color grid2(226, 48, 48);

std::vector<sf::Color> hColours = { player1, player2 };
std::vector<sf::Color> gColours = { grid1, grid2 };

//vectors to store grid squares and states
std::vector<sf::RectangleShape> grid;
std::vector<int> gridData;



int turn = 1;

bool isGame = true;
int winner = 0;

int input(sf::Vector2i dir)//check WASD inputs, with delay
{
	if (Clock.getElapsedTime().asSeconds() > inputDelay) {
		//return a number to move through grid vector by
		if (dir.y < 0) { Clock.restart();  return -3; }
		if (dir.x < 0) { Clock.restart();  return -1; }
		if (dir.y > 0) { Clock.restart();  return 3; }
		if (dir.x > 0) { Clock.restart();  return 1; }
		return 0;
	}
	else
	{
		return 0;
	}
}

void swap()//swaps turns
{
	if (turn == 0)
	{
		turn = 1;
		return;
	}
	if (turn == 1)
	{
		turn = 0;
		return;
	}
}

sf::Clock iClock;//clock for select square delay
float iDelay = 0.5;

bool interact(bool press)//check if spacebar is pressed
{
	if (iClock.getElapsedTime().asSeconds() > iDelay)
	{
		if (press) { iClock.restart(); return true; }
	}
	
	return false;
}

//Grid vars
float size = 250.f;
int selected = 0;
void startGame()//recreate vectors and vars -> restarts game
{
	grid.clear();
	gridData.clear();
	winner = 0;
	isGame = true;
	selected = 4;
	turn = 0;

	for (int row = 0; row < 3; row++)//loop 3 times, once per row
	{
		for (int col = 0; col < 3; col++)//loop 9 times, 3 per row
		{
			float x = 660.f + col * 300.f;//position grid square by row and collum
			float y = 240.f + row * 300.f;

			sf::RectangleShape unit({ size, size });//create grid squares
			unit.setFillColor(sf::Color::White);
			unit.setOrigin({ size / 2, size / 2 });
			unit.setPosition({ x, y });
			unit.setOutlineThickness(20.f);

			grid.push_back(unit);//add all 9 squares to grid vector
			gridData.push_back(0);//add an int to store data for each square

		}
	}
}

int getWinner()
{
	return winner;
}

bool isWon()//check if game is active
{
	return !isGame;
}

void staleMate()
{
	isGame = 0;
	winner = 0;
}

//-----Check Win Condition-----
void check(int player)//very inefficient check for every win condition
{
	if (
		//Check rows:
		(gridData[0] == player &&
		gridData[1] == player &&
		gridData[2] == player)
		||
		(gridData[3] == player &&
		gridData[4] == player &&
		gridData[5] == player)
		||
		(gridData[6] == player &&
		gridData[7] == player &&
		gridData[8] == player)
		||
		(gridData[0] == player &&
		gridData[3] == player &&
		gridData[6] == player)
		||
		//Check Collums
		(gridData[1] == player &&
		gridData[4] == player &&
		gridData[7] == player)
		||
		(gridData[2] == player &&
		gridData[5] == player &&
		gridData[8] == player)
		||
		//Check diagonals
		(gridData[0] == player &&
		gridData[4] == player &&
		gridData[8] == player)
		||
		(gridData[2] == player &&
		gridData[4] == player &&
		gridData[6] == player)
		)
	{
		isGame = false;
		winner = player;
	}
	else if (//check for stalemate
		gridData[0] != 0 &&
		gridData[1] != 0 &&
		gridData[2] != 0 &&
		gridData[3] != 0 &&
		gridData[4] != 0 &&
		gridData[5] != 0 &&
		gridData[6] != 0 &&
		gridData[7] != 0 &&
		gridData[8] != 0
		)
	{
		staleMate();
	}
}

void update(sf::Vector2i dir, bool press)
{
	if (isGame)
	{
		selected += input(dir);

		if (selected < 0)
		{
			selected += 9;
		}
		if (selected > 8)
		{
			selected -= 9;
		}
		for (auto& u : grid)
		{
			u.setOutlineColor(sf::Color::Transparent);
		}
		grid[selected].setOutlineColor(hColours[turn]);

		if (interact(press) && gridData[selected] == 0)
		{
			grid[selected].setFillColor(gColours[turn]);
			gridData[selected] = turn + 1;
			swap();
		}

		check(2);
		check(1);
	}
}

void drawGame(sf::RenderWindow& window)
{
	for (auto& i : grid)
	{
		window.draw(i);
	}
}