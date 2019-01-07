/*******************************************************************************Name: Michael Andrews
**Date: 8/5/18
**Program Filename: LastRoom.hpp
**Description: This is the header file for the LastRoom class, which is a
**child class of Room.  LastRoom is intended to be only partially used; the
**game should end as soon as the player enters it.
*****************************************************************************/

#ifndef LASTROOM
#define LASTROOM

#include "Room.hpp"
#include <string>

class LastRoom : public Room
{
	public:
		
		/*************************************************************
		**Function: LastRoom();
		**Description: Simply sets a pre-baked set of descriptors from
		**rooms.txt.
		*************************************************************/
		LastRoom();


		/*************************************************************
		**Function: action()
		**Description: LastRoom's version of action() is a
		**placeholder.  It simply returns a string saying that said
		**string should never been seen in normal game operation.
		*************************************************************/
		std::string action(Player&, bool&);
};

#endif
