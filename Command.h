#ifndef COMMAND_H
#define COMMAND_H

#include<iostream>
#include<string>
using namespace std;

#include "List.h"
#include "Card.h"
#include "Stack.h"

class Command {
private:
	string command;
	string src;
	string dest;
	int numb;

public:
	Command(string c)
	{
		command = c;
		src = '\0';
		dest = '\0';
		numb = 0;
	}

	void setSource(string c)
	{
			src = c;
	}

	void setDestination(string c)
	{
			dest = c;
	}

	void setNumber(int n)
	{
			numb = n;
	}

	bool validMove()
	{
		if ((src == "c1" || src == "c2" || src == "c3" || src == "c4" || src == "c5" || src == "c6" || src == "c7"
			|| src == "f1" || src == "f2" || src == "f3" || src == "f4" || src == "w") &&
			((dest == "c1" || dest == "c2" || dest == "c3" || dest == "c4" || dest == "c5" || dest == "c6" || dest == "c7"
				|| dest == "f1" || dest == "f2" || dest == "f3" || dest == "f4" || dest == "w")) &&
			(numb > 0 && numb < 13))
			return true;

		else
			return false;

	}

	string getSource()
	{
		return src;
	}

	string getDestination()
	{
		return dest;
	}

	string getCommand()
	{
		return command;
	}

	int getNumber()
	{
		return numb;
	}
};

#endif 
