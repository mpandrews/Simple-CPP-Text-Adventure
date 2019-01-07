/*******************************************************************************
**Name: Michael Andrews
**Date: 8/8/18
**Program Filename: Player.cpp
**Description: This is the implementation file for the Player class, which is
**used to represent the player's avatar and tracks the player's location and
**inventory.
*******************************************************************************/

#include "Player.hpp"
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

Player::Player(Room* pos, Item** tools)
{
	location = pos;

	for (int i = 0; i < 4; i++)
	{
		inventory[i] = tools[i];
	}
}

string Player::check_inventory() const
{
	int count = 0;
	string result;
	for (int i = 0; i < 4; i++)
	{
		if (inventory[i] != nullptr)
		{
			count++;
		}
	}
	if (count == 0)
	{
		result += "Your toolbox is empty, save for a few cobwebs.";
		result += '\n';
		result += "Four tools are left to recover.\n";
	}
	else if (count == 1)
	{
		result += "The only thing in your toolbox is ";
		result += inventory[0]->get_name();
		result += '.';
		result += '\n';
		result += "Three tools are left to recover.\n";
	}
	else if(count == 2)
	{
		result += "Your toolbox contains ";
		result += inventory[0]->get_name();
	       	result += " and ";
		result += inventory[1]->get_name();
		result += '.';
		result += '\n';
		result += "Two tools are left to recover.\n";
	}
	else if (count == 3)
	{
		result += "Your toolbox contains ";
		result += inventory[0]->get_name();
		result += ", ";
		result += inventory[1]->get_name();
		result += ", and ";
		result += inventory[2]->get_name();
		result += '.';
		result += '\n';
		result += "One tool is left to recover.\n";
	}
	else
	{
		result += "Your toolbox contains ";
		result += inventory[0]->get_name();
	        result += ", ";
		result += inventory[1]->get_name();
	       	result += ", ";
		result += inventory[2]->get_name();
		result += ", and ";
		result += inventory[3]->get_name();
		result += ".";
		result += '\n';
		result += "There are no tools missing.";
		result += '\n';
	}
	return result;
}

void Player::remove_tool(int id)
{
	for (int i = 0; i < 4; i++)
	{
		if(inventory[i] != nullptr)
		{
			if (inventory[i]->get_id() == id)
			{
				inventory[i] = nullptr;
			}
		}
	}
}

void Player::add_tool(Item* newtool)
{
	if (inventory[0] == nullptr)
	{
		inventory[0] = newtool;
	}
	else if (inventory[1] == nullptr)
	{
		inventory[1] = newtool;
	}
	else if (inventory[2] == nullptr)
	{
		inventory [2] = newtool;
	}
	else if (inventory[3] == nullptr)
	{
		inventory[3] = newtool;
	}
}
