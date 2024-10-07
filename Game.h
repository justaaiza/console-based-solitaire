#ifndef GAME_H
#define GAME_H

#include <iostream>
#include<iomanip>
#include<string>
#include <cstdlib>
#include <limits> //for cin.fail
using namespace std;
#include <algorithm> //for shuffle
#include <chrono>
#include <random>


#include "List.h"
#include "Card.h"
#include "Command.h"
#include "Stack.h"

int globalVar = 0;

enum column {
	c1 = 0,
	c2 = 1,
	c3 = 2,
	c4 = 3,
	c5 = 4,
	c6 = 5,
	c7 = 6
};

int getColumnFromString(const string& src)
{
	if (src == "c1") return c1;
	if (src == "c2") return c2;
	if (src == "c3") return c3;
	if (src == "c4") return c4;
	if (src == "c5") return c5;
	if (src == "c6") return c6;
	if (src == "c7") return c7;
}

enum foundation {
	f1 = 0,
	f2 = 1,
	f3 = 2,
	f4 = 3
	
};

int getFounFromString(const string& str)
{
	if (str == "f1") return f1;
	if (str == "f2") return f2;
	if (str == "f3") return f3;
	if (str == "f4") return f4;
	
}


template<class T>
class Stack;
class Command;
class List;
class Card;

class Game
{
private:
	Card* deck[52];
	List tableau[7];
	Stack<Card*> foundation[4];
	Stack<Card*> stock;
	Stack<Card*> wpile;
	Stack<Command*> cStack;

public:
	Game()
	{
		intializeDeck();
		shuffleDeck();
		populate();
	}

	void intializeDeck()
	{
		for (int i = 0; i < 13; i++)
		{
			deck[i] = new Card("spade", i + 1);
			deck[i + 13] = new Card("diamond", i + 1);
			deck[i + 26] = new Card("club", i + 1);
			deck[i + 39] = new Card("heart", i + 1);
		}

	}

	void shuffleDeck()
	{
		random_device rd;
		mt19937 generator(rd());
		shuffle(deck, deck + 52, generator);
	}

	void populate()
	{
		int index = 0;

		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				tableau[i].insertAtTail(deck[index]);
				index++;
			}

			
			deck[index - 1]->setState(false);

		}

		while (index < 52)
		{
			stock.push(deck[index++]);
		}
	}

	void startGame()
	{
		displayGameState();
	}

	void drawFromStock()
	{
		if(!stock.isEmpty())
		{
			stock.topItem()->setState(false);

			wpile.push(stock.topItem());

			stock.pop();
		}
		else
		{
			while (!wpile.isEmpty())
			{
				Card* data = wpile.topItem();
				data->setState(true);
				stock.push(data);
				wpile.pop();
			}
		}
	}

	void moveFromWpileToCol(List& dest)
	{
		if(!wpile.isEmpty())
		{
			Card* temp = wpile.topItem();
			wpile.pop();
			dest.insertAtTail(temp);
		}
	}

	void moveFromWpileToFoun(Stack<Card*>& dest)
	{
		
		if(!wpile.isEmpty())
		{
			Card* temp = wpile.topItem();
			wpile.pop();
			dest.push(temp);
		}
		
	}

	void moveFromFounToCol(Stack<Card*>& src, List& dest)
	{
		if(!src.isEmpty())
		{
			Card* temp = src.topItem();
			src.pop();
			dest.insertAtTail(temp);
		}
	}

	void moveFromColToFoun(List& src, Stack<Card*>& dest)
	{
		
		Card* temp = src.end();
		dest.push(temp);
		src.deleteFromTail();
		
		if(src.end())
			src.end()->setState(false);  //set the state of now tail as not flipped
	}

	void moveToEmptyCol(List& dest, Stack<Card*>& src)
	{
		if (src.topItem())
		{
			if (src.topItem()->isKing())
			{
				Card* temp = src.topItem();
				dest.insertAtTail(temp);
				src.pop();
			}

		}
	}

	

	bool moveCard(string src, string dest, int numb)
	{
		List::iterator itr;

		//moving from foundations to column
		if ((dest == "c1" || dest == "c2" || dest == "c3" || dest == "c4"
			|| dest == "c5" || dest == "c6" || dest == "c7") && ((src == "f1")
				|| (src == "f2") || (src == "f3") || (src == "f4")))
		{
			int s = getFounFromString(src);
			int d = getColumnFromString(dest);
			if (tableau[d].isEmpty())
			{
				moveToEmptyCol(tableau[d], foundation[s]);
				return true;
			}
			if (foundation[s].topItem()->isMoveValid(tableau[d].end()))
			{
				moveFromFounToCol(foundation[s], tableau[d]);
				return true;
			}

			
		}

		//moving from column to foundation
		else if ((src == "c1" || src == "c2" || src == "c3" || src == "c4"
			|| src == "c5" || src == "c6" || src == "c7") && ((dest == "f1")
				|| (dest == "f2") || (dest == "f3") || (dest == "f4")))
		{
			int s = getColumnFromString(src);
			int d = getFounFromString(dest);

			if (foundation[d].isEmpty())
			{
				if (tableau[s].end()->aceCondition())
				{
					moveFromColToFoun(tableau[s], foundation[d]);
					return true;
				}
				
			}

			else if (tableau[s].end()->isFoundMoveValid(foundation[d].topItem()))
			{
				moveFromColToFoun(tableau[s], foundation[d]);
				return true;
			}

		}

		// moving from coulmn to column
		else if (src[0] == 'c' && dest[0] == 'c')
		{
			int s = getColumnFromString(src);
			itr = tableau[s].getPtr(numb);
			int d = getColumnFromString(dest);

			if (tableau[d].isEmpty())
			{
				tableau[d].moveSublist(tableau[s], itr);
				return true;
			}
			else if (tableau[s].isMoveValid(itr, tableau[d].end()))
			{
				tableau[d].moveSublist(tableau[s], itr);
				return true;
			}

		}


		//moving from wpile to columns
		else if ((dest == "c1" || dest == "c2" || dest == "c3" || dest == "c4"
			|| dest == "c5" || dest == "c6" || dest == "c7") && (src == "w"))
		{
			int d = getColumnFromString(dest);

			if (tableau[d].isEmpty())
			{
				moveToEmptyCol(tableau[d], wpile);
				return true;
			}
			if (wpile.topItem()->isMoveValid(tableau[d].end()))
			{
				moveFromWpileToCol(tableau[d]);
				return true;
			}

		}

		// moving from wpile to foundations
		else if ((dest == "f1" || dest == "f2" || dest == "f3" || dest == "f4") && (src == "w"))
		{
			int d = getFounFromString(dest);
			if (foundation[d].isEmpty())
			{
				if (wpile.topItem()->aceCondition())
				{
					moveFromWpileToFoun(foundation[d]);
					return true;
				}
			}
			else if (wpile.topItem()->isFoundMoveValid(foundation[d].topItem()))
			{
				moveFromWpileToFoun(foundation[d]);
				return true;
			}

		}

		return false;
	
	}

	void undoDrawFromStock()
	{
		if (!wpile.isEmpty())
		{
			wpile.topItem()->setState(true);

			stock.push(wpile.topItem());

			wpile.pop();
		}
		else
		{
			while (!stock.isEmpty())
			{
				Card* data = stock.topItem();
				data->setState(true);
				wpile.push(data);
				stock.pop();
				wpile.topItem()->setState(false);
			}
		}
	}

	void undoMoveCard(Command*& c)
	{
		string src = c->getSource();
		string dest = c->getDestination();
		int numb = c->getNumber();


		//undo the move from col to col
		if (src[0] == 'c' && dest[0] == 'c')
		{
			List::iterator itr;

			int s = getColumnFromString(dest);
			itr = tableau[s].getPtr(numb);
			int d = getColumnFromString(src);


			if (tableau[d].isEmpty())
				tableau[d].moveToHead(tableau[s], itr);

			else
			{
				if (tableau[d].end()->getState() == false)
				{
					if (!tableau[d].singleElement())
					{
						if (tableau[d].end()->getState() != tableau[d].end()->getPrevState())
							tableau[d].end()->setState(true);
					}
				}
				tableau[d].moveSublist(tableau[s], itr);
			}

		}

		//undo the move from wpile to foundation piles
		else if (src[0] == 'w' && dest[0] == 'f')
		{
			int s = getFounFromString(dest);
			Card* data = foundation[s].topItem();
			wpile.push(data);
			foundation[s].pop();
		}

		//undo the move from wpile to columns
		else if (src[0] == 'w' && dest[0] == 'c')
		{
			int s = getColumnFromString(dest);
			Card* data = tableau[s].end();
			wpile.push(data);
			tableau[s].deleteFromTail();
			tableau[s].end()->setState(false);
		}

		//undo the move from column to foundation
		else if (src[0] == 'c' && dest[0] == 'f')
		{
			int s = getFounFromString(dest);
			int d = getColumnFromString(src);

			if (tableau[d].end()->getState() == false)
			{
				if (!tableau[d].singleElement())
				{
					if (tableau[d].end()->getState() != tableau[d].end()->getPrevState())
						tableau[d].end()->setState(true);
				}
			}
			

			Card* data = foundation[s].topItem();
			tableau[d].insertAtTail(data);
			foundation[s].pop();

		}

		//undo the move from foundation to column
		else if (src[0] == 'f' && dest[0] == 'c')
		{
			int s = getColumnFromString(dest);
			int d = getFounFromString(src);

			Card* data = tableau[s].end();
			foundation[d].push(data);
			tableau[s].deleteFromTail();
		}

	}

	void undoCommand()
	{
		Command* c = cStack.topItem();

		if (c->getCommand() == "s")
			undoDrawFromStock();
		else if (c->getCommand() == "m")
			undoMoveCard(c);

		cStack.pop();
		
	}

	bool winningCondition()
	{
		for (int i = 0; i < 4; i++)
		{
			if (foundation[i].giveSize() != 13)
				return false;
		}

		return true;
	}


	bool enterCommand()
	{
		bool isValid = true;
		

		while(isValid)
		{
			string x, y, z;
			int n;
			cout << "\nEnter the desired command: ";
			cin >> x;
			Command* c = new Command(x);
			

			if (c->getCommand() == "s")
			{
				drawFromStock();
				cStack.push(c);
				return true;
			}
			else if (c->getCommand() == "m")
			{
				cout << "Enter source:";
				cin >> y;
				cout << "Enter destination:";
				cin >> z;
				cout << "Enter number:";
				cin >> n;

				if (cin.fail())
				{
					cout << "Invalid input for number! Please enter an integer.\n";
					cin.clear(); // Clear the error state of cin
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
					continue; // Retry input
				}

				c->setSource(y);
				c->setDestination(z);
				c->setNumber(n);

				if (c->validMove())
				{
					if (moveCard(c->getSource(), c->getDestination(), c->getNumber()))
					{
						cStack.push(c);
					}
					return true;
				}
				else
				{
					cout << "\nInvalid move!\n";
					
				}
			}
			else if (c->getCommand() == "z")
			{
				if (!cStack.isEmpty())
				{
					undoCommand();
				}
				return true;
			}

			else if (c->getCommand() == "q")
			{
				isValid = false;
			}

			else
			{
				cout << "\nInvalid move!\n";
				
			}
		}

		return false;

	}

	void displayGameState()
	{

		cout << "\n------------------------------------------------------------------------------------------------------------" << endl;
		cout << "\033[33m" << "GAME INSTRUCTIONS:" << endl;
		cout << "~ enter 'm' to move a card from one column to another\n~ enter 's' to draw a card from the stock pile\n~ enter 'z' to undo a command\n~ enter 'q' to quit the game"<<"\033[0m" << endl;
		cout << "\n------------------------------------------------------------------------------------------------------------" << endl;

		while(!winningCondition())
		{
			cout << "\n------------------------------------------------------------------------------------------------------------" << endl;
			cout <<"\033[35m" << "\t\t\t\t\tCONSOLE-BASED SOLITAIRE" << "\033[0m" << endl;
			cout << "------------------------------------------------------------------------------------------------------------\n\n" << endl;

			cout <<"\033[32m"<<"\033[1m"<< "STOCK:\t\tWASTE:\t\t\t\tFoundation1:\tFoundation2:\tFoundation3:\tFoundation4:" << "\033[0m" << endl;

			cout <<"\033[90m" << "(" << stock.giveSize() << " cards)\t(" << wpile.giveSize() << " cards)\t\t\t(" << foundation[0].giveSize() << " cards)\t("
				<< foundation[1].giveSize() << " cards)\t(" << foundation[2].giveSize() << " cards)\t(" << foundation[3].giveSize() << " cards)" << "\033[0m" << endl;
			
			cout << *(stock.topItem()) << "\t\t"<<*(wpile.topItem())<<"\t\t\t\t"<<*(foundation[0].topItem())<<"\t\t"
				<< *(foundation[1].topItem())<<"\t\t"<< *(foundation[2].topItem()) << "\t\t" << *(foundation[3].topItem())<<endl<<endl;


			cout<< "\033[32m"<<"\033[1m"<< "\nCOLUMN1:\tCOLUMN2:\tCOLUMN3:\tCOULMN4:\tCOLUMN5:\tCOLUMN6:\tCOLUMN7:" << "\033[0m" << endl;

			for (int i = 0; i < 13; i++)
			{
				if (tableau[0].elementExit(i))
					tableau[0].printElement(i);
				cout << "\t\t";
				if (tableau[1].elementExit(i))
					tableau[1].printElement(i);
				cout << "\t\t";
				if (tableau[2].elementExit(i))
					tableau[2].printElement(i);
				cout << "\t\t";
				if (tableau[3].elementExit(i))
					tableau[3].printElement(i);
				cout << "\t\t";
				if (tableau[4].elementExit(i))
					tableau[4].printElement(i);
				cout << "\t\t";
				if (tableau[5].elementExit(i))
					tableau[5].printElement(i);
				cout << "\t\t";
				if (tableau[6].elementExit(i))
					tableau[6].printElement(i);
				
				cout << endl;
			}

			cout << "\n------------------------------------------------------------------------------------------------------------" << endl;
			if (enterCommand())
			{
				system("cls");
				continue;
			}
			else
			{
				cout << "GAME FINISHED!" << endl;
				return;
			}
			cout << "\n\n------------------------------------------------------------------------------------------------------------" << endl;

		}

	
	}

	~Game()
	{
		for (int i = 0; i < 52; i++)
			delete deck[i];
	}

};

#endif 
