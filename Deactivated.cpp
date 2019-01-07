/*******************************************************************************
**Name: Michael Andrews
**Date: 8/5/18
**Program Filename: Deactivated.cpp
**Description: This is the implementation file for the Deactivated class, which
**is a derived class of Room.  The player is teleported to an object of this
**class when the holodeck is fixed.
*******************************************************************************/

#include "Deactivated.hpp"
#include <string>

using std::string;

Deactivated::Deactivated() : Room()
{
	assign_description(11, 11, 2);
	already_tugged_shirt = false;
}

string Deactivated::action(Player &player, bool &unused)
{
	string result;
	if (!already_tugged_shirt)
	{
		result += action_first_outcome;
	}
	else
	{
		result += action_second_outcome;
	}
	result += '\n';
	already_tugged_shirt = true;
	return result;
}
