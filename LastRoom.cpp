/*******************************************************************************
**Name: Michael Andrews
**Date: 8/5/18
**Program Filename: LastRoom.cpp
**Description: This is the implementation file for the LastRoom class, which is
**a child class of Room.  LastRoom is the most stripped down of the child
**classes, and is only intended for partial use; the game will end as soon as
**the player enters it.
*******************************************************************************/

#include "LastRoom.hpp"
#include <string>

using std::string;

LastRoom::LastRoom() : Room()
{
	assign_description(12, 12, 3);
}

string LastRoom::action(Player &player, bool &unused)
{
	string result;
	result += action_first_outcome;
	return result;
}
