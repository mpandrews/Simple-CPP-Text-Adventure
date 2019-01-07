/*******************************************************************************
**Name: Michael Andrews
**Date: 8/6/18
**Program Filename: Player.hpp
**Description: This is the header file for the Player class.  The Player class
**tracks certain game state information, chiefly what the player has and where
**the player is.
*******************************************************************************/

#ifndef PLAYER
#define PLAYER

#include <string>
#include <vector>
#include "Item.hpp"
#include "Room.hpp"

class Player
{
	private:
		Room* location;
		Item* inventory[4];
	public:
		/***************************************************************
		**Function: Player(Room*, Item**)
		**Description: Constructor.  It initializes the player's two
		**data members with the arguments passed.  The Item** must be an
		**array of exactly four elements.
		***************************************************************/
		Player(Room*, Item**);


		/***************************************************************
		**Function: get_location()
		**Description: Exactly what it looks like.  This is probably the
		**most-called function in Player; because it returns a pointer
		**to Room, it can be used to call the location's get_exits()
		**function, which can then call any needed function on any
		**room adjacent via standard exits.
		***************************************************************/
		inline Room* get_location() const {return location;}


		/***************************************************************
		**Function: set_location(Room*)
		**Description: set_location is used to arbitrarily relocate the
		**player to a chosen Room.  It is used twice, when passing
		**through the arch at the game's beginning and when repairing
		**the console to win the game.
		***************************************************************/
		inline void set_location(Room* l) {location = l;}


		/***************************************************************
		**Function: move(int)
		**Description: move() moves the player like set_location does,
		**but only by going through the pointer to the current location
		**to a pointer to an adjacent Room.  This is much simpler to
		**call for normal gameplay movement within the Holodeck areas
		**of the game.
		***************************************************************/
		inline void move(int i) {location = location->get_exit(i);}


		/***************************************************************
		**check_inventory()
		**Description: This returns a string that describes the current
		**content of the player's inventory in natural English.
		**It's called by main() when the player asks to see what they
		**have.
		***************************************************************/
		std::string check_inventory() const;


		/***************************************************************
		**Function: get_inventory(int)
		**Description: This returns a pointer to whatever Item* is
		**located at a specific index in the player's inventory array.
		***************************************************************/
		Item* get_inventory_item(int i) const {return inventory[i];}


		/***************************************************************
		**Function: add_tool(Item*)
		**Description: add_tool adds a tool to the player's inventory
		**array in the first available space.  It does not check IDs,
		**and will happily add a duplicate tool, and will simply fail
		**silently if the array is already full.  It's on the rest
		**of the game to ensure that no duplicate tools are allowed to
		**exist.
		***************************************************************/
		void add_tool(Item*);


		/***************************************************************
		**Function: remove_tool(int)
		**Description: remove_tool will iterate through the tool array,
		**and assign to nullptr any elements whose id matches the
		**parameter.  Note that as implemented, the game only ever
		**wants to remove all tools at once, so the only call to this
		**function is itself a for loop which iterates through all
		**valid arguments to remove_tool; however, this implementation
		**affords more flexibility should the game later be modified.
		***************************************************************/
		void remove_tool(int);

		~Player() {}	
};

#endif
