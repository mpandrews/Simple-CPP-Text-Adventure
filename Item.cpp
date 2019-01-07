/*******************************************************************************
**Name: Michael Andrews
**Date: 8/3/18
**Description: This is the implementation file for the Item class, which is used
**to represent the tools the player needs to recover.
*******************************************************************************/

#include "Item.hpp"
#include <string>

using std::string;

Item::Item(int type)
{
	name += "\e[36m";

	if (type == 0)
	{
		name += "an interocitor";
		item_id = 0;
	}
	else if (type == 1)
	{
		name += "a samoflange";
		item_id = 1;
	}
	else if (type == 2)
	{
		name += "a turobencabulator";
		item_id = 2;
	}
	else if (type == 3)
	{
		name += "a length of Jeffries Tube";
		item_id = 3;
	}

	name += "\e[0m";
}
