/*******************************************************************************
**Name: Michael Andrews
**Date: 8/3/18
**Program Filename: Holodeck.hpp
**Description: This is the header file for the Holodeck class, which is a
**child class of Room.  The Holodeck class is chiefly distinguished by having
**a randomized, rather than fixed, description.
*******************************************************************************/

#ifndef HOLODECK
#define HOLODECK


#include "Room.hpp"

class Object;

class Holodeck : public Room
{
	private:

		/*
		 * The two static members are only ever changed by Class methods
		 * and even then, only the first time a constructor of the Class
		 * is called.
		 */

		static int max_descriptor;
		static int max_sky;
	public:
		/***************************************************************
		**Function: Holodeck()
		**Description: Constructor.  It differs from the base class
		**constructor in that it calls randomize() to assign a random
		**description to the room upon creation.
		***************************************************************/
		Holodeck();
		
		
		/***************************************************************
		**Function: find_max_descriptor()
		**Description: Holodeck room descriptions are pulled randomly
		**from a file called rooms.txt.  find_max_descriptor scans
		**through that file to determine the index of the last valid
		**description it contains.  The purpose is to allow additional
		**descriptions to be added to the file without needing to alter
		**the code and recompile to change the range of valid 
		**descriptions.
		***************************************************************/
		int find_max_descriptor();


		/***************************************************************
		**Function: find_max_sky()
		**Description: As with find_max_descriptor(), Holodecks have a
		**randomized sky string which is concatenated with the main room
		**description.  find_max_sky() scans the textfile containing
		**sky strings to determine how many valid strings are available
		**for randomization.
		***************************************************************/
		int find_max_sky();
		

		/***************************************************************
		**Function: randomize()
		**Description: Randomize sets a random description for the room,
		**which consists of three components; a description and matching
		**distant description from rooms.txt, and a sky description from
		**skies.txt.  It does this by scanning through the files for a
		**specific marker, $XX$, where XX is the random number generated
		**and then reading to the next $.  For rooms.txt, the format is
		**$XX$DESCRIPTION$DISTANT_DESCRIPTION$.  For skies.txt, the
		**format is $XX$SKY_DESCRIPTION$.
		**Precondition: rooms.txt and skies.txt *must* exist and be
		**readable, or randomize will print an error message and exit
		**the program.  Additionally, if randomize hits the end-of-file
		**marker without having found the target strings, it will also
		**print an error and quit.
		***************************************************************/
		void randomize();
		

		/***************************************************************
		**Function: action()
		**Description: The action for Holodeck rooms is a tricorder
		**scan.  It follows all four pointers in the exit array, and
		**calls get_has_tool() and get_has_console() on each of them.
		**It also checks the calling room's tool and console flags.
		**It then assembles and returns a string tabulating the results.
		**Note that it should gracefully handle being asked about the
		**contents of a nullptr exit, but if that happens the game
		**has gone very, very seriously off the rails.
		***************************************************************/
		std::string action(Player&, bool&);
};

#endif
