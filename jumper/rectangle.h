#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "globals.h"

#include <iostream>
#include <stack>

class Rectangle {
public:
	Rectangle() {}

	Rectangle(int x, int y, int width, int height) :
		_x(x),
		_y(y),
		_width(width),
		_height(height)
	{}

	const int getCenterX() const { return this->_x + this->_width / 2; }
	const int getCenterY() const { return this->_y + this->_height / 2; }

	const int getLeft() const { return this->_x; }
	const int getRight() const { return this->_x + this->_width; }
	const int getTop() const { return this->_y; }
	const int getBottom() const { return this->_y + this->_height; }

	const int getWidth() const { return this->_width; }
	const int getHeight() const { return this->_height; }

	const int getSide(const sides::Side side) const {
		return
			side == sides::LEFT ? this->getLeft() :
			side == sides::RIGHT ? this->getRight() :
			side == sides::TOP ? this->getTop() :
			side == sides::BOTTOM ? this->getBottom() :
			sides::NONE;
	}

	//bool collidesWith
	//Takes in another Rectangle and checks if the two are colliding
	const bool collidesWith(const Rectangle& other) const {
		return
			this->getRight() > other.getLeft() &&
			this->getLeft() < other.getRight() &&
			this->getTop() < other.getBottom() &&
			this->getBottom() > other.getTop();
	}

	const bool isValidRectangle() const {
		return (this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
	}

	void print() const {
		printf("x: %d, y: %d, width: %d, height: %d\n", _x, _y, _width, _height);
	}

private:
	int _x, _y, _width, _height;
};

class Ticket {
public:
	Ticket() {};
	~Ticket() {};

	int insertTicket() 
	{
		this->_ticket.push(curr);
		if (curr == INT_MAX) {
			curr = 1;
		}
		else {
			++curr;
		}
		//std::cout << "insert " << _ticket.top() << std::endl;
		return this->_ticket.top();
	}
	
	int pollTicket()
	{
		if (this->_ticket.empty()) {
			return 0;
		}
		int topTicket = this->_ticket.top();
		//std::cout << "remove " << topTicket << std::endl;
		this->_ticket.pop();
		return topTicket;
	}
	
private:
	std::stack<int> _ticket;
	int curr = 1;
};

#endif
