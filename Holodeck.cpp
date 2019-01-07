/*******************************************************************************
**Name: Michael Andrews
**Date: 8/3/18
**Description: This is the implementation file for the Holodeck class, which is
**a child class of Room.  The bulk of the game takes places in Holodeck rooms,
**which have randomized descriptions and contents.
*******************************************************************************/

#include <fstream>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <string>
#include "Holodeck.hpp"
#include "Object.hpp"

using std::ifstream;
using std::numeric_limits;
using std::streamsize;
using std::string;
using std::cout;
using std::endl;

Holodeck::Holodeck() : Room()
{
	if (max_descriptor == -1)
	{
		max_descriptor = find_max_descriptor();
	}
	if (max_sky == -1)
	{
		max_sky = find_max_sky();
	}
	randomize();
}

int Holodeck::max_descriptor = -1;

int Holodeck::max_sky = -1;

int Holodeck::find_max_descriptor()
{
	ifstream rooms("rooms.txt");
	int max_found = 0;
	int found = 0;
	string holder;
	if (rooms.fail())
	{
		cout << "Error: rooms.txt could not be opened." << endl;
		exit(1);
	}

	while(!rooms.eof())
	{
		rooms.ignore(numeric_limits<streamsize>::max(), '$');
		getline(rooms, holder, '$');
		found = atoi(holder.c_str());
		if (found > max_found)
		{
			max_found = found;
		}
		rooms.ignore(numeric_limits<streamsize>::max(), '$');
		rooms.ignore(numeric_limits<streamsize>::max(), '$');
		rooms.ignore(numeric_limits<streamsize>::max(), '$');
	}
	rooms.close();

	return max_found;
}

int Holodeck::find_max_sky()
{
	ifstream skies("skies.txt");
	int max_found = 0;
	int found = 0;
	string holder;
	if (skies.fail())
	{
		cout << "Error: skies.txt could not be opened." << endl;
		exit(1);
	}

	while(!skies.eof())
	{
		skies.ignore(numeric_limits<streamsize>::max(), '$');
		getline(skies, holder, '$');
		found = atoi(holder.c_str());
		if (found > max_found)
		{
			max_found = found;
		}
		skies.ignore(numeric_limits<streamsize>::max(), '$');
	}
	skies.close();

	return max_found;
}


void Holodeck::randomize()
{
	for (int i = 0; i < static_cast<int>(contents.size()); i++)
	{
		delete contents.at(i);
		contents.at(i) = nullptr;
	}

	contents.clear();
	assign_description(20, max_descriptor + 1, 1);

	
	ifstream skies("skies.txt");
	if (skies.fail())
	{
		cout << "Error: could not open skies.txt." << endl;
		exit(1);
	}
	int target;
	int found;
	bool sky_found = false;
	string holder;
	string concatenator;
	target = rand() % (max_sky + 1);
	while(!sky_found)
	{
		if (skies.eof())
		{
			cout << "Error: sky description " << target;
			cout << " could not be found." << endl;
			exit(1);
		}
		skies.ignore(numeric_limits<streamsize>::max(), '$');
		getline(skies, holder, '$');
		found = atoi(holder.c_str());
		if (found == target)
		{
			getline(skies, concatenator, '$');
			description += concatenator;
			sky_found = true;
		}
		else
		{
			skies.ignore(numeric_limits<streamsize>::max(), '$');
		}
	}
	skies.close();

	for (int i = (rand() % 3); i < 3; i++)
	{
		contents.emplace_back(new Object());
	}
	if (has_tool)
	{
		int random = rand() % contents.size();
		contents.at(random)->give_tool(tool);
	}

}

string Holodeck::action(Player& player, bool& unused)
{
	string result;
	result += action_first_outcome;
	result += '\n';
	int counter = 0;

	for (int i = 0; i < 4; i++)
	{
		if (get_exit(i) == nullptr)
		{
			cout << "ERROR: Adjacency check failed in";
			cout << "Holodeck::action()!" << endl;
			exit(1);
		}
	}	
	for (int i = 0; i < 4; i++)
	{
		if (get_exit(i)->get_has_tool())
		{
			counter++;
		}
	}
	if (has_tool)
	{
		counter++;
	}

	/*
	 * We're going to append a description of the number of nearby tools
	 * located, with wildcards we're going to replace with directions.
	 */
	if (counter == 0)
	{
		result += "The scan doesn't find any tools, ";
		result += "either where you are or orthogonally nearby.";
	}
	else if (counter == 1)
	{
		result += "The scan turns up one tool: somewhere %.";
	}
	else if (counter == 2)
	{
		result += "The scan turns up two tools, one %, and the other";
		result += " %.";
	}
	else if (counter == 3)
	{
		result += "The scan turns up three tools, one %, the second ";
		result += "%, and the third %.";
	}
	else if (counter == 4)
	{
		result += "The scan finds all four of your tools nearby, ";
		result += "one %, the second %, the third %, and the last %.";
	}
	/*
	 * Because we should have a number of wildcards equal to the number of
	 * tools present or adjacent, we can just blindly replace them with
	 * appropriate substrings.  The numbers should always match up.
	 */
	if (has_tool)
	{
		result.replace(result.find('%'), 1,
				"in your immediate vicinity");
	}
	if (get_exit(0)->get_has_tool())
	{
		result.replace(result.find('%'), 1,
				"to the north");
	}
	if (get_exit(1)->get_has_tool())
	{
		result.replace(result.find('%'), 1,
				"to the south");
	}
	if (get_exit(2)->get_has_tool())
	{
		result.replace(result.find('%'), 1,
				"to the east");
	}
	if (get_exit(3)->get_has_tool())
	{
		result.replace(result.find('%'), 1,
				"to the west");
	}
	result += '\n';

	if (has_console)
	{
		result += "The tricorder % indicates that the console you need";
		result += " to repair is right in front of your face.";
		result += '\n';
		if (counter > 0)
		{
			result.replace(result.find('%'), 1, "also");
		}
		else
		{
			result.erase(result.find('%'), 2);
		}
	}

	bool console_adjacent = false;
	string console_direction;
	
	for (int i = 0; i < 4; i++)
	{
		if (get_exit(i)->get_has_console())
		{
			console_adjacent = true;
			if (i == 0)
			{
				console_direction += "north";
			}
			else if (i == 1)
			{
				console_direction += "south";
			}
			else if (i == 2)
			{
				console_direction += "east";
			}
			else if (i == 3)
			{
				console_direction += "west";
			}
		}
	}
	if (console_adjacent)
	{
		result += "The tricorder % indicates that the console you need";
		result += " to repair is somewhere to the %.";
		result += '\n';
		if (counter > 0)
		{
			result.replace(result.find('%'), 1, "also");
		}
		else
		{
			result.erase(result.find('%'), 2);
		}
		result.replace(result.find('%'), 1, console_direction);
	}


	return result;
}
