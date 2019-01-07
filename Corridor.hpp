/*******************************************************************************
**Name: Michael Andrews
**Date: 8/5/18
**Program File: Corridor.hpp
**Description: This is the header file for the Corridor class, which is a child
**class of Room.  Corridor is used for the player's starting location.
*******************************************************************************/

#ifndef CORRIDOR
#define CORRIDOR

#include "Room.hpp"
#include <string>

class Object;

class Corridor : public Room
{
	private:
		Room* arch_connection;

	public:
		/***************************************************************
		**Function: Corridor(Room*)
		**Description: Constructor. It differs from the base class
	       	**constructor in that it takes a pointer-to-Room, which it
		**assigns to the class' arch_connection member.
		**arch_connection is used to allow the room's special action
		**to move the player to another room.
		***************************************************************/
		Corridor(Room*);
		
		
		/***************************************************************
		**Function: action(Player$, bool&)
		**Descrition: action() for the Corridor class teleports the
		**player to another room, 'empties' the player's toolbox, and
		**sets a bool which the main while loop uses to know that
		**the tools should be seeded around the holodeck section of the
		**map.
		***************************************************************/
		virtual std::string action(Player&, bool&);
};

#endif
