#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>
using namespace std;
const int row = 10;
const int col = 10;
char board[10][10];
void menu()
{
	cout << "Welcome to Snake and Ladder game." << endl;
	cout << "1. Play Game\n2. Credits\n3. Instructions\n4. Score\n5. Exit\n";
	cout << "Enter your choice: ";
}
void makeboard()
{
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
			board[i][j] = ' ';
	}
}
void displayboard(int* players, int numplayers, char playerSymbols[])
{
	cout << "  ._________________________________________________________________________________________.\n";
	for (int i = row - 1; i >= 0; --i)
	{
		cout << "  |";
		if (i % 2 == 0)
		{
			for (int j = 0; j < col; ++j)
			{
				char playerSymbol = ' ';
				char obstacle = ' ';
				for (int k = 0; k < numplayers; ++k)
				{
					if ((i * 10 + j + 1) == *(players + k))
						playerSymbol = playerSymbols[k];
				}
				if (board[i][j] == 'S')
				{
					obstacle = 'S';
				}
				else if (board[i][j] == 'L')
				{
					obstacle = 'L';
				}
				cout << "   " << playerSymbol << obstacle << "   |";
			}
		}
		else
		{
			for (int j = col - 1; j >= 0; --j)
			{
				char playerSymbol = ' ';
				char obj = ' ';
				for (int k = 0; k < numplayers; ++k)
				{
					if ((i * 10 + j + 1) == *(players + k))
						playerSymbol = playerSymbols[k];
				}
				if (board[i][j] == 'S')
				{
					obj = 'S';
				}
				else if (board[i][j] == 'L')
				{
					obj = 'L';
				}
				cout << "   " << playerSymbol << obj << "   |";
			}
		}
		cout << "\n  |";
		if (i % 2 == 0)
		{
			for (int j = 0; j < col; ++j)
			{
				if (i * 10 + j + 1 < 10)
				{
					cout << "    " << i * 10 + j + 1 << "   |";
				}
				else
				{
					cout << "    " << i * 10 + j + 1 << "  |";
				}
			}
		}
		else
		{
			for (int j = col - 1; j >= 0; --j)
			{
				if (i == 9)
				{
					cout << "   " << i * 10 + j + 1 << "  | ";
				}
				else if (i * 10 + j + 1 < 10)
				{
					cout << " " << i * 10 + j + 1 << "  |";
				}
				else
				{
					cout << "    " << i * 10 + j + 1 << "  |";
				}
			}
		}
		cout << endl;
		cout << "  |";
		for (int j = 0; j < col; ++j)
		{
			cout << "________|";
		}
		cout << endl;
	}
}
void init()
{
	makeboard();
	board[8][0] = 'L';
	board[8][3] = 'S';
	board[6][2] = 'L';
	board[5][5] = 'S';
	board[4][7] = 'L';
	board[2][4] = 'S';
	board[1][9] = 'L';
}
int movePlayer(int pos, int steps)
{
	int newpos = pos + steps;
	int newrow = (newpos - 1) / 10;
	int newcol;
	if (newrow % 2 == 0)
	{
		newcol = (newpos - 1) % 10;
	}
	else
	{
		newcol = 9 - ((newpos - 1) % 10);
	}
	if (newpos <= 100 && newpos > 0)
	{
		pos = newpos;
		if (newrow % 2 == 0)
		{
			if (newcol == 10)
			{
				pos -= steps;
			}
		}
		else
		{
			if (newcol == -1)
			{
				pos -= steps;
			}
		}
		switch (board[newrow][newcol])
		{
		case 'S':
			cout << "\033[41m\033[37m";
			cout << "Oops! You encountered a snake! Moving back 10 spaces.\n";
			cout << "Screen turns red for 3 seconds...\n";
			this_thread::sleep_for(chrono::seconds(3));
			pos -= 10;
			break;
		case 'L':
			cout << "\033[42m\033[30m";
			cout << "Great! You found a ladder! Climbing up 10 spaces." << endl;
			cout << "Screen turns green for 3 seconds..." << endl;
			this_thread::sleep_for(chrono::seconds(3));
			pos = newpos + 10;
			break;

		default:
			break;
		}
	}
	return pos;
}
void Play_game()
{
	int n = 4;
	int playerpos[4] = { 0, 0, 0, 0 };
	char pSymbol[4] = { '@', '#', '$', '&' };
	int playerswon = 0;
	ofstream write("moves.txt");
	if (write.is_open())
	{
		while (playerswon < 3)
		{
			for (int i = 0; i < n; ++i)
			{
				if (playerpos[i] > 100)
				{
					continue;
				}
				cout << "Player " << pSymbol[i] << ", press Enter to roll the dice...";
				write << "Player " << pSymbol[i] << ", press Enter to roll the dice...";
				cin.ignore();
				cin.get();
				int steps = rand() % 6 + 1;
				cout << "Player " << pSymbol[i] << " rolled a " << steps << "." << endl;
				write << "Player " << pSymbol[i] << " rolled a " << steps << "." << endl;
				cout << "\033[0m";
				if (steps == 6 || playerpos[i] > 1)
				{
					playerpos[i] = movePlayer(playerpos[i], steps);
					displayboard(playerpos, n, pSymbol);
					for (int k = 0; k < n; ++k)
					{
						if (k != i && playerpos[k] == playerpos[i])
						{
							cout << "Player " << pSymbol[i] << " and Player " << pSymbol[k] << " are at the same position! Player " << pSymbol[k] << " goes back to the initial state.\n";
							write << "Player " << pSymbol[i] << " and Player " << pSymbol[k] << " are at the same position! Player " << pSymbol[k] << " goes back to the initial state.\n";
							playerpos[k] = 1;
						}
					}
					if (playerpos[i] >= 100)
					{
						cout << "Player " << pSymbol[i] << " has won!" << endl;
						write << "Player " << pSymbol[i] << " has won!" << endl;
						playerswon++;
						playerpos[i] = 101;
					}
					if (playerswon == 3)
					{
						cout << "Game Over! Three players have won." << endl;
						write << "Game Over! Three players have won." << endl;
						return;
					}
				}
				if (steps == 6 && playerpos[i] > 1)
				{
					cout << "Player " << pSymbol[i] << " rolled a 6! Roll again." << endl;
					write << "Player " << pSymbol[i] << " rolled a 6! Roll again." << endl;
					int steps = rand() % 6 + 1;
					cout << "Player " << pSymbol[i] << " rolled a " << steps << "." << endl;
					write << "Player " << pSymbol[i] << " rolled a " << steps << "." << endl;
					playerpos[i] = movePlayer(playerpos[i], steps);
					displayboard(playerpos, n, pSymbol);
					if (steps >= 2 && steps <= 5)
					{
						cout << "Player " << pSymbol[i] << " rolled a " << steps << " after rolling a 6! Moving " << steps + 6 << " spaces forward." << endl;
						write << "Player " << pSymbol[i] << " rolled a " << steps << " after rolling a 6! Moving " << steps + 6 << " spaces forward." << endl;
						playerpos[i] = movePlayer(playerpos[i], steps + 6);
						displayboard(playerpos, n, pSymbol);
					}
				}
				else
				{
					cout << "Player " << pSymbol[i] << " didn't roll a 6. Waiting for the next turn." << endl;
					write << "Player " << pSymbol[i] << " didn't roll a 6. Waiting for the next turn." << endl;
				}
			}
		}
	}
	write.close();
}
void Credits()
{
	cout << "Game developed by Haisam Lodhi." << endl;
	cout << "Department of Computer Science." << endl;
	cout << "Fast University." << endl;
}
void Instructions()
{
	cout << "Instructions : " << endl;
	cout << "- Roll the dice and move your token forward the number of spaces shown on the dice." << endl;
	cout << "- If you land at the bottom of a ladder, you can move up to the top of the ladder." << endl;
	cout << "- If you land on the head of a snake, you must slide down to the bottom of the snake." << endl;
	cout << "- The first player to reach the final space is the winner." << endl;
	cout << "- If two players are at the same point index, the second player goes back to the initial state." << endl;
	cout << "- Players start their game when they roll a 6. If they don't roll a 6, they will wait for their next move." << endl;
	cout << "- If there's a 6 on the dice, the player gets another turn to roll the dice." << endl;
	cout << "- The first winner can give six moves forward to any other player as a gift." << endl;
	cout << "- As soon as a snake bites a player, the screen color becomes red for a few seconds and comes back to the original color afterward." << endl;
	cout << "- As soon as any player takes a ladder, the screen becomes green and comes back to the original color after a few seconds." << endl;
	cout << "- The game will end when three out of four players complete the game." << endl;
}
void Score(int numPlayers, char playerSymbols[], int playerPositions[])
{
	cout << "Player Scores : " << endl;
	for (int i = 0; i < numPlayers; i++)
	{
		cout << "Player " << playerSymbols[i] << " : " << *(playerPositions + i) << " spaces." << endl;
	}
}
void file_handling(int playerPositions[], int numPlayers)
{
	ofstream file;
	file.open("score.txt");
	if (file.is_open())
	{
		file << "Number of player:" << numPlayers << endl;
		for (int i = 0; i < numPlayers; ++i)
		{
			file << "player" << i + 1 << ":" << playerPositions[i] << endl;
		}
		file.close();
		cout << "Game state saved successfully." << endl;
	}
	else
	{
		cout << "Unable to save game state." << endl;
	}
}

int main()
{
	srand(time(0));
	init();
	char playersym[4] = { '@', '#', '$', '&' };
	int ppos[4] = { 0, 0, 0, 0 };
	while (true)
	{
		menu();


		int choice;
		cin >> choice;

		if (cin.fail())
		{
			cout << "Invalid input. Please enter a number." << endl;
			cin.clear();
			cin.ignore();
			continue;
		}

		switch (choice)
		{
		case 1:
			Play_game();
			break;
		case 2:
			Credits();
			break;
		case 3:
			Instructions();
			break;
		case 4:
			file_handling(ppos, 4);
			return 0;
			break;
		case 5:
			exit;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}
	}
	system("pause");
	return 0;
}
