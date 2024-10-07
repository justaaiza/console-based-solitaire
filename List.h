#ifndef LIST_H
#define LIST_H

#include<iostream>
using namespace std;

#include "Card.h"
#include "Stack.h"
#include "Command.h"

class List {
	class Node {
	public:
		Card* data;
		Node* prev;
		Node* next;

		Node(Card* c = nullptr, Node* p = nullptr, Node* n = nullptr)
		{
			data = c;
			prev = p;
			next = n;
		}
	};
	class listIterator {

		friend class List;

	private:
		Node* iptr;

	public:
		listIterator(Node* ptr = nullptr)
		{
			iptr = ptr;
		}
		listIterator& operator++()
		{
			if (iptr)
				iptr = iptr->next;

			return (*this);
		}

		listIterator& operator--()
		{
			if (iptr)
			{
				iptr = iptr->prev;

				return (*this);
			}

			return(*this);
		}

		void operator*()
		{
			cout << iptr->data;
		}

		bool operator==(listIterator& ptr)
		{
			if (iptr == ptr.iptr)
				return true;

			else
				return false;
		}

		listIterator operator++(int)
		{

			listIterator old = *this;
			++(*this);
			return old;
		}

		bool operator!=(listIterator& ptr)
		{
			if (iptr != ptr.iptr)
				return true;

			else
				return false;
		}

		bool operator!=(Node* ptr)
		{
			if (iptr != ptr)
				return true;

			else
				return false;
		}
	};

	Node* head;
	Node* tail;

public:

	typedef listIterator iterator;


	List() { head = nullptr, tail = nullptr; }

	~List()
	{
		Node* temp = head;

		while (temp != nullptr)
		{
			temp = head->next;
			delete head;
			head = temp;
		}
	}


	Card* begin()
	{
		if(head)
		{
			return head->data;
		}
	}

	Card* end()
	{
		if(tail)
		{
			return tail->data;
		}
	}

	iterator getPtr(int numb)
	{
		Node* temp = tail;
		int i = 0;
		while (i < numb-1)
		{
			temp = temp->prev;
			i++;
		}

		return temp;
	}

	bool isMoveValid(iterator itr, Card* dest)
	{
		if(itr.iptr)
			return itr.iptr->data->isMoveValid(dest);
	}

	void moveSublist(List& src, iterator& itr)
	{
		if (itr.iptr) 
		{
			iterator prev = itr.iptr->prev;

			if (tail)      //if destination's tail exist
			{
				tail->next = itr.iptr;  // point dest tail->next to desired card of src
				itr.iptr->prev = tail;  

				while (itr.iptr->next != nullptr)   //traverse till the end of src list
					itr++;

	
				tail = itr.iptr;  //update the tail
			}
			else  //incase the dest list is empty
			{
				if(itr.iptr->data->isKing())
				{
					head = itr.iptr;
					while (itr.iptr->next != nullptr)
						itr++;
					tail = itr.iptr;
				}
			}

			
			if (prev != nullptr)  //updating the tail of src
			{
				src.tail = prev.iptr;  
				prev.iptr->next = nullptr;  

				prev.iptr->data->setState(false);
			}
			else  
			{
				src.head = src.tail = nullptr;  
			}
		}
	}

	void moveToHead(List& src, iterator& itr)
	{
		if(itr.iptr)
		{

			iterator prev = itr.iptr->prev;

			head = itr.iptr;
			while (itr.iptr->next != nullptr)
				itr++;
			tail = itr.iptr;

			
			if (prev != nullptr)  //updating the tail of src
			{
				src.tail = prev.iptr;
				prev.iptr->next = nullptr;

				prev.iptr->data->setState(false);
			}
			else
			{
				src.head = src.tail = nullptr;
			}
		}
	}

	bool singleElement()
	{
		if (head == tail)
			return true;
		else
			return false;
	}

	void insertAtTail(Card* card)
	{
		if (head == nullptr)
		{
			head = tail = new Node(card, nullptr, nullptr);
		}
		else
		{
			Node* temp = new Node(card, tail, nullptr);
			tail->next = temp;
			tail = temp;
		}
	}

	void deleteFromTail()
	{
		if (tail)
		{
			if (tail == head)
			{
				delete tail;
				head = tail = nullptr;
			}

			else
			{
				Node* temp = tail;
				tail = tail->prev;
				tail->next = nullptr;
				delete temp;
			}
		}
	}

	void printList()
	{
		Node* temp = head;

		while (temp != nullptr)
		{
			cout << *(temp->data)<<" ";
			temp = temp->next;
		}
	}

	bool isEmpty()
	{
		if (head == nullptr)
			return true;
		else
			return false;
	}

	bool elementExit(int i) //checking if data at a specific index exist
	{
		Node* temp = head;
		int x = 0;

		while (temp && x < i)
		{
			temp = temp->next;
			x++;
		}

		if (temp)
			return true;
		else
			return false;
	}

	void printElement(int i) //printing data at a specific index
	{
		Node* temp = head;
		int x = 0;

		while (x < i)
		{
			temp = temp->next;
			x++;
		}
		cout << *(temp->data);
	}

	
};


#endif