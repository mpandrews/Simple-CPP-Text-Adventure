/*******************************************************************************
**Name: Michael Andrews
**Date: 8/3/18
**Program Filename: Room.cpp
**Description: This is the implementation file for the pure virtual Room class,
**which is the parent class from which the actual useable room-types are derived
*******************************************************************************/

#include "Room.hpp"
#include "Object.hpp"
#include "Console.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <cstdlib>

using std::ifstream;
using std::numeric_limits;
using std::streamsize;
using std::string;
using std::cout;
using std::endl;

Room::Room()
{
	has_tool = false;
	tool = nullptr;
	has_console = false;
	console = nullptr;
	for (int i = 0; i < 4; i++)
	{
		exits[i] = nullptr;
	}
}



string Room::print_room() const
{
	string result;

	result += "\e[93m";
	result +=  description;
	result += '\n';
	result +=  ceiling;
       	result += '\n';
	result +=  "\e[0m";

	result += "What would you like to do? \n";
	result += "Enter \e[35mN,S,E,W\e[0m to move, or a \e[35mnumber\e[0m ";
	result += "to perform an action. \n";
	if (exits[0] != nullptr)
	{
		result += "To the \e[35mn\e[0morth you see ";
		result += exits[0]->get_dist_desc();
		result += '\n';
	}
	if (exits[1] != nullptr)
	{
		result += "To the \e[35ms\e[0mouth you see ";
		result += exits[1]->get_dist_desc();
		result += '\n';
	}
	if (exits[2] != nullptr)
	{
		result += "To the \e[35me\e[0mast you see ";
		result += exits[2]->get_dist_desc();
		result += '\n';
	}
	if (exits[3] != nullptr)
	{
		result += "To the \e[35mw\e[0mest you see ";
		result += exits[3]->get_dist_desc();
		result += '\n';
	}
	result += "\e[35m0\e[0m: View the helpfile. \n";
	result += "\e[35m1\e[0m: Rummage through your toolbox. \n";
	result += "\e[35m2\e[0m: ";
	result += action_description;
	result += "\n";
	result += list_objects();

	return result;
}


void Room::add_exits(Room *n, Room *s, Room *e, Room *w)
{
	exits[0] = n;
	exits[1] = s;
	exits[2] = e;
	exits[3] = w;
}

Room::~Room()
{
	while(!contents.empty())
	{
		if (contents.back() != nullptr)
		{
			delete contents.back();
		}
		contents.pop_back();
	}
}

void Room::assign_description(int min, int max, int action)
{
	ifstream rooms("rooms.txt");
	if (!rooms)
	{
		cout << "Error: rooms.txt could not be opened.";
		exit(1);
	}
	bool descfound = false;
	bool actionfound = false;
	string desc_index;
	int converted_index;
	int target;

	/*
	 * Certain rooms have fixed descriptions, and so will have a min and max
	 * descriptor range that are identical.  Others are randomized, and so
	 * have a range.
	 */
	if (min == max)
	{
		target = min;
	}
	else
	{
		target = (rand() % (max - min)) + min;
	}

	/*
	 * We want to check each descriptor in the library until we find one
	 * that matches our targets, whether hardcoded or random.
	 */
	while (!descfound || !actionfound)
	{
		rooms.ignore(numeric_limits<streamsize>::max(), '$');

		/*
		 * The targets should always be in a valid range, so hitting eof
		 * is a definite error that should terminate the program.
		 */
		if (rooms.eof())
		{
			cout << "Error: room description " << target;
			cout << " could not be found." << endl;
			exit(1);
		}

		/*
		 * getline to the first $ will put us at the identifier for the
		 * first string block.
		 */
		getline(rooms, desc_index, '$');
		converted_index = atoi(desc_index.c_str());
		/*
		 * There are three strings in each descriptor block, each
		 * terminated with a $.
		 */
		if (converted_index == target)
		{
			getline(rooms, description, '$');
			getline(rooms, distant_description, '$');
			getline(rooms, why_player_cant_move, '$');
			descfound = true;
		}		
		/*
		 * Once again, there are three strings in each action block,
		 * each terminated with a $.
		 */
		else if (converted_index == action)
		{
			getline(rooms, action_description, '$');
			getline(rooms, action_first_outcome, '$');
			getline(rooms, action_second_outcome, '$');
			actionfound = true;
		}		
		/*
		 * This simply skips past any block that doesn't match one of
		 * the two targets.
		 */
		else
		{
			rooms.ignore(numeric_limits<streamsize>::max(), '$');
			rooms.ignore(numeric_limits<streamsize>::max(), '$');
			rooms.ignore(numeric_limits<streamsize>::max(), '$');
		}
	}
	rooms.close();
}

void Room::set_tool(Item* t)
{
	/*
	 * If we're assigning a tool value of nullptr, the room needs to go
	 * through and strip any tools that have been assigned to its owned
	 * Objects.
	 */
	if (t == nullptr)
	{
		has_tool = false;
		if (contents.size() > 0)
		{
			for (int i = 0; 
			     i < static_cast<int>(contents.size()); i++)
			{
				contents.at(i)->give_tool(nullptr);
			}
		}
	}

	/*
	 * Conversely, if we're adding a tool, the Room needs to go through
	 * and issue a tool to a random owned Object.  Normally, this is
	 * redundant, since after the tool distribution is randomized, so are
	 * the objects, but it's necessary because when the player first
	 * steps onto the holodeck and loses the tools, they would not actually
	 * be assigned to any objects until the next world shuffle.
	 */
	else
	{
		has_tool = true;
		if (contents.size() > 0)
		{
			int random = (rand() % contents.size());
			contents.at(random)->give_tool(t);
		}
	}
	tool = t;
}

void Room::set_console(Console* c)
{
	if (c == nullptr)
	{
		has_console = false;
	}
	else
	{
		has_console = true;
	}
	console = c;
}


string Room::interact_room_object(int index, Player& p, bool* b)
{
	/*
	 * If the object has a tool that the player will recover via the
	 * interaction, we want to set the room's tool-possession flag to false
	 * so that tricorder scans won't return a false positive.
	 */
	if (contents.at(index)->get_tool() != nullptr)
	{
		has_tool = false;
	}
	string result = contents.at(index)->interact(p, b);
	delete contents.at(index);
	contents.erase(contents.begin() + index);
	return result;
}

string Room::interact_room_console(Player &p, bool& b)
{
	return console->interact(p, b);
}

string Room::list_objects() const
{
	string result;
	for (int i = 0; i < static_cast<int>(contents.size()); i++)
	{
		result += "\e[35m";
		result += static_cast<char>(i + '3');
		result += "\e[0m: ";
		result +=  contents.at(i)->get_description();
		result += '\n';
	}
	if (has_console)
	{
		result += "\e[35m";
		result += static_cast<char>(contents.size() + '3');
		result += "\e[0m: ";
		result +=  console->get_description();
		result += '\n';
	}
	return result;
}
