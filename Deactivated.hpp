/*******************************************************************************
**Name: Michael Andrews
**Date: 8/5/18
**Program Filename: Deactivated.hpp
**Description: This is the header file for the Deactivated class, which is a
**child class of Room.  When the player interacts with the console with all
**tools, the crisis is resolved and the player is teleported to an instance
**of this class.
*******************************************************************************/

#ifndef DEACTIVATED
#define DEACTIVATED

#include <string>
#include "Room.hpp"

class Player;

class Deactivated : public Room
{
	private:
		bool already_tugged_shirt;
	public:
		/***************************************************************
		**Function: Deactivated()
		**Description: Constructor.  Sets the descriptive strings to
		**predetermined values from rooms.txt, and sets
		**already_tugged_shirt to false.
		***************************************************************/
		Deactivated();


		/***************************************************************
		**Function: action()
		**Description: The Deactivated version of action() returns a
		**string about tugging the player's tunic, if
		**already_tugged_shirt is false.  It also sets the flag to true.
		**If the player attempts additional tugs, a different message
		**is returned.
		***************************************************************/
		std::string action(Player&, bool&);
};

#endif
