/*******************************************************************************
**Name: Michael Andrews
**Date: 8/3/18
**Description: This is the header file for the Item class.  It's a very simple
**class, used to define the MacGuffins the player has to collect.
*******************************************************************************/

#ifndef ITEM
#define ITEM

#include <string>

class Item
{
	private:
		int item_id;
		std::string name;
	public:
		
		/***************************************************************
		**Function: Item(int)
		**Description: The constructor.  It simply sets the item's
		**name to a specific value, determined from the int parameter.
		***************************************************************/
		Item(int);


		/***************************************************************
		**Function: get_name
		**Description: Exactly what it looks like.
		***************************************************************/
		inline std::string get_name() const {return name;}


		/***************************************************************
		**Function: get_id()
		**Description: Returns an int representing the tool's identity.
		**This is used for various functions which need to know which
		**tool is being interacted with, and avoids having to parse
		**the name string to find out.
		***************************************************************/
		inline int get_id() const {return item_id;}
};

#endif
