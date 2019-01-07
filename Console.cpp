/*******************************************************************************
**Name: Michael Andrews
**Date: 8/5/18
**Program Filename: Console.cpp
**Description: This is the implementation file for the Console class, which
**the player interacts with to end the game's crisis.
*******************************************************************************/

#include <string>
#include <iostream>
#include "Console.hpp"
#include "Player.hpp"

using std::string;
using std::cout;
using std::endl;

Console::Console()
{
	descriptor = "You see a holodeck control console, which for those keeping score at home is what you need to fix.";
	action_descriptor_missing_tools = "You open the console and take a look, but there's no way you can actually finish the job without all of your tools.";
	action_descriptor_all_tools = "You open the console and get to work.  In a matter of minutes, the holodeck is repaired.  There's a sort of sighing sound, and everything except the console vanishes.  Isn't it weird how the problem is never, like, somewhere inside the wall where you can't get to it?";
}

string Console::interact(Player& player, bool& day_saved)
{
	string result;
	bool all_tools = true;
	for (int i = 0; i < 4; i++)
	{
		if (player.get_inventory_item(i) == nullptr)
		{
			all_tools = false;
		}
	}

	if (!all_tools)
	{
		result += action_descriptor_missing_tools;
		result += '\n';
	}
	else
	{
		result +=  action_descriptor_all_tools;
		result += '\n';
		day_saved = true;
	}
	return result;
}
