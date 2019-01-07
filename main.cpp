/*******************************************************************************
**Name: Michael Andrews
**Date: 8/3/18
**Program Filename: main.cpp
**Description: This is the main for the final assignment, which is the Infocom
**sort of game thing.
*******************************************************************************/

#include <iostream>
#include <cstdlib>
#include <limits>
#include <string>
#include <fstream>
#include "Room.hpp"
#include "Holodeck.hpp"
#include "Corridor.hpp"
#include "Object.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Console.hpp"
#include "Deactivated.hpp"
#include "LastRoom.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::numeric_limits;
using std::streamsize;
using std::string;
using std::ifstream;
using std::istreambuf_iterator;


/*******************************************************************************
**Function: char take_input(Player&)
**Description: Simply handles taking input from the player via cin; returns a
**single character and flushes the input buffer.
*******************************************************************************/
char take_input(Player&);


/*******************************************************************************
**Function: void shuffle_world()
**Description: shuffle_world() causes all Holodeck objects in main's array
**to re-randomize themselves, flushing any data about Items they may have,
**retrieving new descriptive text, deleting any Objects in their vectors,
**flushing any data bout the Console, and spawning new objects.  It then
**passes its Room**, Item**, Console* and bool* arguments to scatter_tools().
**Parameters: Player&; Room** which is main's holodeck array, Item** which is
**main's tool array, Console*, bool* which is main's tools_found array.
*******************************************************************************/
void shuffle_world(Player&, Room**, Item**, Console*, bool*);


/*******************************************************************************
**Function: void scatter_tools()
**Description: Scatter tools uses its bool* parameter, which is main's
**tools_found array, to add any tools which remain unrecovered to random
**holodeck rooms.  It then also places the console in a random holodeck room.
**As such, it is passed the array of Holodecks, the array of Items, the console,
**and the aforementioned array of bools.
*******************************************************************************/
void scatter_tools(Room**, Item**, Console*, bool*);


/*******************************************************************************
**Function: void enter_to_continue();
**Description: Simply uses cin.ignore to pause output until the player hits
**enter.
*******************************************************************************/
void enter_to_continue();


/*******************************************************************************
**Function: string process_selection()
**Description: Takes the input obtained by take_input and processes it to
**determine outcome and set any flags needed for the next turn.
**In many ways, it is the actual turn loop, with main() only handling those
**events which do not happen on every single turn, such as shuffling the world
**or losing the game.
**Parameters: 
**	Player&: the player.
**	char: the player's input.
**	string&: the combined results of last turn.
**	bool&: increment_turn, which is used to decide whether anything actually
**		happened on the previous turn.  It's necessary to avoid typos
**		causing repeated world shuffles, etc.
**	bool&: tools_just_taken, which is used to force the calling of
**		scatter_tools().  This avoids tools being unavailable for
**		recovery until the next regularly scheduled shuffling.
**	bool&: day_saved, which is used to break out of the primary game loop
**		and allow the player to proceed to the endgame.
**	bool*: tools_found[4], which is passed to shuffle_world() so that it can
**		then be passed on to scatter_tools.
*******************************************************************************/
string process_selection(Player&, char, string&, bool&, bool&, bool&, bool*, string&);

int main()
{
	srand(time(0));

	
	/*
	 * Creating the objects we need for the game's initial state.
	 */
	Room* holodeck[9];
	Item* tools[4];
	Console* console = new Console();

	for (int i = 0; i < 4; i++)
	{
		tools[i] = new Item(i);
	}

	/*
	 * blanker is used to clear the screen between turns.
	 */
	string blanker("\033[2J");
	string help_text;
	ifstream help_file("helpfile.txt");
	
	if (help_file.fail())
	{
		cout << "ERROR: Help file not openable!" << endl;
		exit(1);
	}

	help_text.assign((istreambuf_iterator<char>(help_file)),
		  (istreambuf_iterator<char>()));
	
	help_file.close();



	for (int i = 0; i < 9; i++)
	{
		holodeck[i] = new Holodeck;
	}
	
	/*
	 * We need to link up all the rooms in the holodeck grid.  It would be
	 * nice to embed this in the constructors, but of course all the rooms
	 * actually need to exist before we can link them to each other.
	 * Initially, I used a 2d array which simplified this linking, but
	 * added a lot of nested for loops later in the code.  A flat array
	 * might involve more typing now, but it results in more efficiency
	 * later.
	 */
	holodeck[0]->add_exits(holodeck[6], holodeck[3], 
			holodeck[1], holodeck[2]);
	holodeck[1]->add_exits(holodeck[7], holodeck[4],
			holodeck[2], holodeck[0]);
	holodeck[2]->add_exits(holodeck[8], holodeck[5],
			holodeck[0], holodeck[1]);
	holodeck[3]->add_exits(holodeck[0], holodeck[6],
			holodeck[4], holodeck[5]);
	holodeck[4]->add_exits(holodeck[1], holodeck[7],
			holodeck[5], holodeck[3]);
	holodeck[5]->add_exits(holodeck[2], holodeck[8],
			holodeck[3], holodeck[4]);
	holodeck[6]->add_exits(holodeck[3], holodeck[0],
			holodeck[7], holodeck[8]);
	holodeck[7]->add_exits(holodeck[4], holodeck[1],
			holodeck[8], holodeck[6]);
	holodeck[8]->add_exits(holodeck[5], holodeck[2],
			holodeck[6], holodeck[7]);

	Room* start = new Corridor(holodeck[5]);
	Player player(start, tools);

	/*
	 * Flags we will need, and a variable to hold input.
	 */
	char choice;
	bool day_saved = false;
	bool tools_just_taken = false;
	bool found_tools[4] {false, false, false, false};

	int turn_counter = -1;
	bool increment_turn = true;
	string last_turn;
	while(!day_saved && turn_counter <= 30)
	{
		cout << blanker;
		/*
		 * Increment the turn count if the previous action was valid.
		 */

		if (increment_turn)
		{
			turn_counter++;
		}
		/*
		 * The tools_just_taken flag is set by the starting room's
		 * action() method.  It's also passed to every other room's
		 * action(), but isn't actually modified or used.
		 */
		if (tools_just_taken)
		{
			scatter_tools(holodeck, tools, console, found_tools);
			tools_just_taken = false;
			cout << help_text;
			enter_to_continue();
		}
		/*
		 * These are messages warning the player about the time limit.
		 */
		if(increment_turn && turn_counter == 0)
		{
			cout << "Your communicator chirps to life.  \"Ensign, ";
			cout << "the holodeck is consuming all the ship's ";
			cout << "power! If it isn't fixed soon, the spacial ";
			cout << "anomaly will collapse the shields, ";
			cout << "and everyone will die!  Get it fixed, stat!\"";
			cout << endl;
			enter_to_continue();
		}
		if(increment_turn && turn_counter == 10)
		{
			cout << "Your communicator chirps to life. \"Ensign, ";
			cout << "not to bug you or anything, I mean, I know ";
			cout << "you're busy with standing around with your ";
			cout << "thumb up your ass, but if you don't get this ";
			cout << "fixed, we're all going to die.\"" << endl;
			enter_to_continue();
		}
		if(increment_turn && turn_counter == 20)
		{
			cout << "Your communicator chirps to life. \"Okay, ";
			cout << "shields are at 20%.  I realize it's not ";
			cout << "your fault that the holodeck power supply ";
			cout << "doesn't have a fusebox, but it's still on ";
			cout << "you to fix it.\"" << endl;
			enter_to_continue();
		}
		if(increment_turn && turn_counter == 25)
		{
			cout << "Your communicator chirps to life. \"5%";
			cout << " shields, buddy.  Nice knowing you.\"" << endl;
			enter_to_continue();
		}
		
		/*
		 * Display last turn's results.
		 */
		cout << last_turn;
		if (!last_turn.empty())
		{
			enter_to_continue();
			last_turn.clear();
		}
		
		/*
		 * This will shuffle the holodeck every tenth turn.
		 * It happens after the last turn's result so that the player
		 * knows, if they scanned, that their brand new tricorder
		 * results are bunk.
		 */
		if (increment_turn && turn_counter % 10 == 0)
		{
			shuffle_world(player, holodeck, tools, console, 
				      found_tools);
		}
		cout << player.get_location()->print_room();
		cout << endl;
		/*
		 * We're creating two player input variables, one to handle
		 * cardinal movement, which is a character, and one to handle
		 * interactions, which is a number.
		 */
		choice = take_input(player);
		process_selection(player, choice, last_turn, increment_turn,
				  tools_just_taken, day_saved, found_tools,
				  help_text);
	}

	/*
	 * We want to do this here, outside the if/else below, to make sure
	 * that the pointer is scoped to main itself so we can safely get rid
	 * of it at the end.
	 */
	Room* deactivated = new Deactivated();
	Room* last_room = new LastRoom();
	deactivated->add_exits(last_room, nullptr, nullptr, nullptr);
	last_room->add_exits(nullptr, deactivated, nullptr, nullptr);
	player.set_location(deactivated);

	/*
	 * Here we'll handle losing the game.
	 */
	if(!day_saved)
	{
		cout << blanker;
		cout << "Your communicator chirps to life. \"Ensign!  What's ";
		cout << "happening down there, the shields are abou--\"";
		cout << endl;
		cout << "And you know no more." << endl;
		enter_to_continue();
	}
	else
	{
		cout << blanker;
		cout << last_turn;
		if (!last_turn.empty())
		{
			enter_to_continue();
			last_turn.clear();
		}

		cout << "Your communicator chirps to life. \"Ensign, whatever ";
		cout << "you did down there seems to have worked.  Power is ";
		cout << "coming back all over the ship, and the shields are ";
		cout << "recovering.  Good work.  This will be reflected on ";
		cout << "your next performance review.\"" << endl;
		enter_to_continue();

		bool game_ended = false;
		while (!game_ended)
		{
			cout << blanker;
			cout << last_turn;
			if (!last_turn.empty())
			{
				enter_to_continue();
				last_turn.clear();
			}

			cout << player.get_location()->print_room();
			cout << endl;
			
			choice = take_input(player);
			process_selection(player, choice, last_turn, 
					  increment_turn, tools_just_taken, 
					  day_saved, found_tools, help_text);
			if (player.get_location() == last_room)
			{
				game_ended = true;
			}
		}

	/*
	 * The victory message is just the description of the final room.
	 * We use the print_room() function, and truncate the results to get
	 * rid of the action list.
	 */
	string game_over = last_room->print_room();
	game_over.erase(game_over.find('%'),
			numeric_limits<streamsize>::max());
	game_over += "\e[0m";
	cout << game_over << endl;
	}


	/*for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			delete holodeck[i][j];
		}
	}*/
	
	/*
	 * Here we just delete all the dynamic memory we've allocated.
	 * Only main ever actually allocates things, and main's pointers to
	 * the objects allocated are never allowed to point anywhere else.
	 */
	for (int i = 0; i < 9; i++)
	{
		delete holodeck[i];
	}
	for (int i = 0; i < 4; i++)
	{
		delete tools[i];
	}
	delete console;
	delete start;
	delete deactivated;
	delete last_room;

	return 0;
}

char take_input(Player &player)
{
	char input;
	cin >> input;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return tolower(input);
}

void shuffle_world(Player &player, Room** holodeck, Item** tools, 
		   Console* console, bool* found)
{
	bool player_in_holodeck = false;
	for (int i = 0; i < 9; i++)
	{
		if (player.get_location() == holodeck[i])
		{
			player_in_holodeck = true;
		}
	}
	if (player_in_holodeck)
	{
		cout << "A terrible groaning sound courses through the";
	        cout << " holodeck, and";
		cout << " everything changes before your eyes." << endl;
		cout << "Everything you know about where anything is is now ";
		cout << "out of date." << endl;
		enter_to_continue();
	}
	else
	{
		cout << "You hear an unearthly groan from through the archway ";
		cout << "doors and the deck beneath your feet vibrates.";
		cout << endl;
		enter_to_continue();
	}
	
	scatter_tools(holodeck, tools, console, found);
	for (int i = 0; i < 9; i++)
	{
		holodeck[i]->randomize();
	}	
}

void scatter_tools(Room** holodeck, Item** tools, Console* console, 
		   bool* found)
{
	int roll;
	bool placed[4];
	
	/*
	 * Blank existing tool status for holodeck.
	 */
	for (int i = 0; i < 9; i++)
	{
		holodeck[i]->set_tool(nullptr);
	}
	
	/*
	 * Populate the array that tracks placement with the array of 
	 * things needing to be placed.
	 */
	for (int i = 0; i < 4; i++)
	{
		placed[i] = found[i];
	}
	/*
	 * Place each item, avoiding double-placement.
	 */
	for (int i = 0; i < 4; i++)
	{
		while (!placed[i])
		{
			roll = rand() % 9;
			if (!holodeck[roll]->get_has_tool())
			{
				holodeck[roll]->set_tool(tools[i]);
				placed[i] = true;
			}
		}
	}
	/*
	 * Blank placement of the console (which is unique) and then place it.
	 */
	for (int i = 0; i < 9; i++)
	{
		holodeck[i]->set_console(nullptr);
	}
	holodeck[rand() % 9]->set_console(console);
}

void enter_to_continue()
{
	cout << "\e[35mPress enter to continue.\e[0m" << endl;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string process_selection(Player& player, char choice, string& last_turn, 
			 bool& increment_turn, bool& tools_just_taken,
			 bool& day_saved, bool* found_tools,
			 string& help_text)
{
	int int_choice = choice - '0' - 3;

	if (choice == 'n')
	{
		if (player.get_location()->get_exit(0) != nullptr)
		{
			player.move(0);
			increment_turn = true;
		}
		else
		{
			last_turn += player.get_location()->
			get_immobility_reason();
			last_turn += '\n';
			increment_turn = false;
		}
	}
	else if (choice == 's')
	{
		if (player.get_location()->get_exit(1) != nullptr)
		{
			player.move(1);
			increment_turn = true;
		}
		else
		{
			last_turn += player.get_location()->
				get_immobility_reason();
			last_turn += '\n';
			increment_turn = false;
		}
				

	}
	else if (choice == 'e')
	{
		if (player.get_location()->get_exit(2) != nullptr)
		{
			player.move(2);
			increment_turn = true;
		}
		else
		{
			last_turn += player.get_location()->
				get_immobility_reason();
			last_turn += '\n';
			increment_turn = false;
		}				
	}		
	else if (choice == 'w')
	{
		if (player.get_location()->get_exit(3) != nullptr)
		{
			player.move(3);
			increment_turn = true;
		}
		else
		{
			last_turn += player.get_location()->
				get_immobility_reason();
			last_turn += '\n';
			increment_turn = false;
		}
	}
	/*
	 * This handles the player looking at their inventory.
	 */
	else if (choice == '0')
	{
		cout << help_text;
		enter_to_continue();
		increment_turn = false;
	}
	else if (choice == '1')
	{
		last_turn += player.check_inventory();
		increment_turn = true;
	}
	/*
	 * This handles room special actions.
	 */
	else if (choice == '2')
	{
		last_turn += player.get_location()->
				action(player, tools_just_taken);
		increment_turn = true;
	}
	/*
	 * This handles interacting with an object in the room.  The way
	 * this works is that we've decremented the choice by 2, to
	 * account for the two permanent options of inventory and room
	 * action, and then we're seeing if the new result refers to a
	 * valid element of the local room's object vector.  This is
	 * necessary because rooms have a random number of objects in
	 * them.
	 */
	else if (int_choice >= 0 && int_choice 
		 < player.get_location()->get_contents_size())
	{
		last_turn += player.get_location()->
			interact_room_object((int_choice), 
				              player, found_tools);
		increment_turn = true;
	}
	/*
	 * Finally, this handles the player interacting with the repair
	 * console.  The console exists in some random holodeck room,
	 * and its menu index is always the integer immediately after
	 * the last object.  So, if the console is there, *and* the
	 * player's input is exactly equal to the size of the local
	 * room object vector, then we know the player wants to use
	 * the console.  We pass the day_saved flag to the console
	 * by reference because it's the item that actually toggles it.
	 */
	else if (player.get_location()->get_has_console() 
		 && int_choice == 
		 player.get_location()->get_contents_size())
	{
		last_turn += player.get_location()->
			     interact_room_console(player,
					           day_saved);
		increment_turn = true;
	}	
	else
	{
		last_turn += "I didn't understand that.";
		last_turn += '\n';
		increment_turn = false;
	}
	return last_turn;
}
