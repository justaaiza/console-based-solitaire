#ifndef STACK_H
#define STACK_H
#include<iostream>
#include<iomanip>
using namespace std;

#include "List.h"
#include "Card.h"
#include "Command.h"

template<class T>
class Stack {
private:
	class Node {
	public:
		T data;
		Node* next;

	public:
		Node(T d, Node* n = nullptr)
		{
			data = d;
			next = n;
		}
	};
	Node* head;
	Node* tail;
	int size;

public:
	Stack()
	{
		head = nullptr;
		tail = nullptr;
		size = 0;
	}
	void clear()
	{
		Node* temp = head;

		while (temp != nullptr)
		{
			temp = head->next;
			delete head;
			head = temp;
		}

		size = 0;
	}
	bool isEmpty()
	{
		return (head == nullptr);
	}
	void push(T data)
	{
		if (head == nullptr)
			head = tail =  new Node(data, nullptr);
		else
		{
			Node* temp = new Node(data, head);
			head = temp;
			
		}
		size++;
	}
	void pop()
	{
		if(head)
		{
			Node* temp = head;
			if (head->next)
				head = head->next;
			else
				head = nullptr;
			size--;
		}

	}

	T topItem()
	{
		if(head)
		{
			return head->data;
		}
	}
	int giveSize()
	{
		return size;
	}

	

};

#endif

