/*******************************************************************************
**Name: Michael Andrews
**Date: 8/5/18
**Program Filename: Corridor.cpp
**Description: This is the implementation for the Corridor class, which is a
**derived class of Room.  The Corridor is used as the player's starting
**location.
*******************************************************************************/


#include "Room.hpp"
#include "Corridor.hpp"
#include "Object.hpp"
#include "Player.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <string>

using std::ifstream;
using std::numeric_limits;
using std::streamsize;
using std::string;
using std::cout;
using std::endl;

Corridor::Corridor(Room* connection) : Room()
{
	arch_connection = connection;
	assign_description(10, 10, 0);
}

string Corridor::action(Player &player, bool& tools_lost)
{
	string result;

	result += action_first_outcome;
	result.replace(result.find('%'), 1,
			arch_connection->get_dist_desc());
	result += '\n';
	player.set_location(arch_connection);
	for (int i = 0; i < 4; i++)
	{
		player.remove_tool(i);
	}
	tools_lost = true;
	return result;
}
