/*******************************************************************************
**Name: Michael Andrews
**Date: 8/3/18
**Program Filename: Object.hpp
**Description: This is the header file for the Object pure virtual class.
**The various objects which populate the Rooms are child classes of Object.
*******************************************************************************/

#ifndef OBJECT
#define OBJECT

#include <string>

class Item;
class Player;

class Object
{
	protected:
		std::string description;
		std::string interact_no_tool;
		std::string interact_has_tool;
		Item* item;
		static int max_descriptor;
	public:
		/***************************************************************
		**Function: Object(int)
		**Description: The base class constructor handles finding the
		**appropriate strings for the descriptor members in the text
		**file object_descs.txt, and assigning them.
		***************************************************************/
		Object();


		/***************************************************************
		**Function: find_max_descriptor()
		**Description: Object description strings are pulled from a file
		**called object_descs.txt.  This function scans through the file
		**and determines the number of description blocks it contains,
		**so that the Object constructor knows the valid range to pull
		**from at random.  This allows an arbitrary number of blocks
		**to be added to the textfile without necessitating altering
		**the code for the class, though it does come at the cost of
		**some overhead in file i/o.
		***************************************************************/
		int find_max_descriptor();


		/***************************************************************
		**Function: interact()
		**Description: interact essentially allows the player to
		**investigate an object with the hope of recovering a tool.
		**If the object has a tool, that is to say, if its Item* is not
		**nullptr, that tool will be removed from the Object and
		**added to the player's toolbox.  The bool* is intended to be
		**the array found_list, which is a quartet of flags indicating
		**whether each particular tool has been recovered.
		**If the Object does not have a tool, it will return a different
		**string, indicating the failure of the player to find anything.
		***************************************************************/
		std::string interact(Player&, bool*);

		
		/***************************************************************
		**Function: give_tool(Item*)
		**Description: This assigns its parameter to the Object's item
		**member.  If the argument passed is an Item pointer, the
		**has_tool flag will be set to true.  If the argument passed is
		**nullptr, the has_tool flag will be set to false.
		***************************************************************/
		void give_tool(Item*);


		/***************************************************************
		**Function: get_tool()
		**Description: Exactly what it looks like.
		***************************************************************/
		inline Item* get_tool() const {return item;}


		/***************************************************************
		**Function: get_description()
		**Description: Gets description.  This is used by Room's
		**print_room() function to tell the player what objects are
		**present and can be interacted with.
		***************************************************************/
		std::string get_description() const {return description;}
};

#endif
