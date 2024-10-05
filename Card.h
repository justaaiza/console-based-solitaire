#ifndef CARD_H
#define CARD_H
#include<iostream>
#include<string> 

using namespace std;

#include "List.h"
#include "Stack.h"
#include "Command.h"

class Card {

	friend ostream& operator<<(ostream& out, const Card& obj);

private:
	string suit;
	int rank;
	bool flipped;

public:
	Card(string c = " ", int r = -1)
	{
		suit = c;
		rank = r;
		flipped = true;
	}

	void setState(bool flag)
	{
		flipped = flag;
	}

	bool aceCondition()
	{
		if (rank == 1)
			return true;
		else
			return false;
	}

	bool isFoundMoveValid(Card* dest)
	{
		if (dest)
		{
			if (rank == (dest->rank+1))
			{
				if (suit == dest->suit)
					return true;
			}
		}

		return false;
	}

	bool isMoveValid(Card* dest)
	{
		if(dest)
		{
			if (rank < dest->rank)
			{
				if ((suit == "spade" && (dest->suit == "heart" || dest->suit == "diamond"))
					|| (suit == "club" && (dest->suit == "heart" || dest->suit == "diamond"))
					|| (suit == "heart" && (dest->suit == "spade" || dest->suit == "club"))
					|| (suit == "diamond" && (dest->suit == "spade" || dest->suit == "club")))
					return true;
				else
					return false;
			}
			else
				return false;
		}
	}

	bool isKing()
	{
		if (rank == 13)
			return true;
		else
			return false;
	}

};

inline ostream& operator<<(ostream& out, const Card& obj)
{
	if (obj.flipped)
		out<<"\033[34m" << "[ ]" << "\033[0m" << " ";

	else
	{
		if (obj.suit == "spade")
		{
			if(obj.rank==1)
				out  << "A" << char(6) << " ";
			else if(obj.rank==11)
				out << "J" << char(6) << " ";
			else if (obj.rank == 12)
				out << "Q" << char(6) << " ";
			else if (obj.rank == 13)
				out << "K" << char(6) << " ";
			else 
				out << obj.rank<< char(6) << " ";
		}
		else if (obj.suit == "diamond")
		{
			out << "\033[31m"; 
			if (obj.rank == 1)
				out << "A" << char(4) << " ";
			else if (obj.rank == 11)
				out << "J" << char(4) << " ";
			else if (obj.rank == 12)
				out << "Q" << char(4) << " ";
			else if (obj.rank == 13)
				out << "K" << char(4) << " ";
			else
				out << obj.rank << char(4) << " ";
			cout<< "\033[0m";
		}
		else if (obj.suit == "heart")
		{
			out << "\033[31m";
			if (obj.rank == 1)
				out << "A" << char(3) << " ";
			else if (obj.rank == 11)
				out << "J" << char(3) << " ";
			else if (obj.rank == 12)
				out << "Q" << char(3) << " ";
			else if (obj.rank == 13)
				out << "K" << char(3) << " ";
			else
				out << obj.rank << char(3) << " ";
			cout << "\033[0m";
		}
		else if (obj.suit == "club")
		{
			if (obj.rank == 1)
				out << "A" << char(5) << " ";
			else if (obj.rank == 11)
				out << "J" << char(5) << " ";
			else if (obj.rank == 12)
				out << "Q" << char(5) << " ";
			else if (obj.rank == 13)
				out << "K" << char(5) << " ";
			else
				out << obj.rank << char(5) << " ";
		}
	}

	return out;
}

#endif
