/*******************************************************************************
**Name: Michael Andrews
**Date: 8/3/18
**Description: This is the implementation file for the Object class, which is
**a pure virtual class intended as a base for interactable objects which
**populate the Rooms.
*******************************************************************************/

#include <fstream>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <string>
#include "Holodeck.hpp"
#include "Object.hpp"
#include "Item.hpp"
#include "Player.hpp"

using std::ifstream;
using std::numeric_limits;
using std::streamsize;
using std::string;
using std::cout;
using std::endl;

int Object::max_descriptor = 0;

Object::Object()
{
	if (max_descriptor == 0)
	{
		max_descriptor = find_max_descriptor();
	}

	int target = (rand() % (max_descriptor + 1));
	ifstream descs("object_descs.txt");
	item = nullptr;
	if (!descs)
	{
		cout << "Error: could not open objects_descs.txt." << endl;
		exit(1);
	}
	bool descfound = false;
	string desc_index;
	int converted_index;
	while (!descfound)
	{
		/*
		 * This scans through the file looking for the $ delimiter,
		 * which is used to break up descriptions.
		 */
		descs.ignore(numeric_limits<streamsize>::max(), '$');
		
		/*
		 * The scan should never be allowed to hit the end of the file,
		 * so it it does something has gone very wrong.
		 */
		if (descs.eof())
		{
			cout << "Error: Object description " << target;
			cout << " could not be found." << endl;
			exit(1);
		}
		/*
		 * Here we're checking to see if what comes after the $ is the
		 * number of the description we want. We take it as a
		 * string, and then convert it to integers.
		 */
		getline(descs, desc_index, '$');
		converted_index = atoi(desc_index.c_str());

		/*
		 * If it's the right description, we read in what comes before
		 * the next $.
		 */
		if (converted_index == target)
		{
			getline(descs, description, '$');
			getline(descs, interact_no_tool, '$');
			getline(descs, interact_has_tool, '$');
			descfound = true;
		}
		/*
		 * And if it's not, we just bypass the next $ and set
		 * up to repeat the process from there.
		 */
		else
		{
			descs.ignore(numeric_limits<streamsize>::max(), '$');
			descs.ignore(numeric_limits<streamsize>::max(), '$');
			descs.ignore(numeric_limits<streamsize>::max(), '$');
		}
			
	}
	descs.close();

}

int Object::find_max_descriptor()
{
	ifstream descs("object_descs.txt");
	int max_found = 0;
	int found = 0;
	string holder;
	if (!descs)
	{
		cout << "Error: object_descs.txt could not be opened.";
	       	cout << endl;
		exit(1);
	}

	while(!descs.eof())
	{
		descs.ignore(numeric_limits<streamsize>::max(), '$');
		getline(descs, holder, '$');
		found = atoi(holder.c_str());
		if (found > max_found)
		{
			max_found = found;
		}
		descs.ignore(numeric_limits<streamsize>::max(), '$');
		descs.ignore(numeric_limits<streamsize>::max(), '$');
		descs.ignore(numeric_limits<streamsize>::max(), '$');
	}
	descs.close();
	
	return max_found;
}

string Object::interact(Player &player, bool* found_list)
{
	string result;

	if (item != nullptr)
	{
		interact_has_tool.replace(interact_has_tool.find('%'), 1,
					  item->get_name());
		result += interact_has_tool;
	       	result += '\n';
		found_list[item->get_id()] = true;
		player.add_tool(item);
		item = nullptr;
	}
	else
	{
		result += interact_no_tool;
		result += '\n';
	}
	
	return result;
}

void Object::give_tool(Item* tool)
{
	item = tool;
}
