/*******************************************************************************
**Name: Michael Andrews
**Date: 8/8/18
**Program Filename: Console.hpp
**Description: Header file for the Console class, which is used to win the game
**after the player has recovered all their tools.
*******************************************************************************/

#ifndef CONSOLE
#define CONSOLE

#include <string>

class Player;

class Console
{
	private:
		std::string descriptor;
		std::string action_descriptor_all_tools;
		std::string action_descriptor_missing_tools;

	public:
		/***************************************************************
		**Function: Console()
		**Description: Constructor.  Initializes all the members to
		**hardcoded values.
		***************************************************************/
		Console();


		/***************************************************************
		**Function: Interact()
		**Description: interact() allows the player to interact with the
		**Console object.  As arguments, it takes the player and a bool.
		**The bool is set to true if the player has recovered all four
		**tools; it's used to break out of the crisis loop.
		***************************************************************/
		std::string interact(Player&, bool& day_saved);


		/***************************************************************
		**Function: get_description()
		**Description: This is used by the Room::print_room() function
		**to let the player know that the console is present and can
		**be interacted with.
		***************************************************************/
		std::string get_description() {return descriptor;}

};

#endif
