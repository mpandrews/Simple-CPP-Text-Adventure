/*******************************************************************************
**Name: Michael Andrews
**Date: 8/3/18
**Program Filename: Room.hpp
**Description: This is the header file for the Room class, which is a pure
**virtual class from which the actual room-type classes are derived.
*******************************************************************************/

#ifndef ROOM
#define ROOM

#include <string>
#include <vector>

class Object;
class Player;
class Item;
class Console;

class Room
{
	protected:
		std::string description;
		std::string ceiling;
		std::string distant_description;
		std::string action_description;
		std::string action_first_outcome;
		std::string action_second_outcome;
		std::string why_player_cant_move;
		
		bool has_tool;
		Item* tool;
		bool has_console;
		Console* console;
		Room* exits[4];
		std::vector<Object*> contents;
	public:
		/***************************************************************
		**Function: Room()
		**Description: Just sets the pointers in exit to nullptr.
		***************************************************************/
		Room();


		/***************************************************************
		**Function: print_room()
		**Description: This causes the object to print its descriptions
		**and exits to the console.  It will also follow all its exit
		**pointers and include the distant_description member of all
		**adjacent Room objects.
		***************************************************************/
		std::string print_room() const;


		/***************************************************************
		**Function: action()
	        **Description: This is a pure virtual function.  It will be
		**defined in child classes, but has no function here.
		***************************************************************/
		virtual std::string action(Player&, bool&) = 0;


		/***************************************************************
		**Function: get_dist_desc()
		**Description: This simply returns the distant description
		**member.  Its intended use is for Rooms to be able to provide
		**descriptions of nearby rooms.
		***************************************************************/
		inline std::string get_dist_desc() const 
			{return distant_description;}

		
		/***************************************************************
		**Function: string get_immobility_reason()
		**Description: Returns a string telling the player why they
		**can't engage in orthogonal movement from their current
		**location.
		***************************************************************/
		inline std::string get_immobility_reason() const
			{return why_player_cant_move;}

		
		/***************************************************************
		**Function: string get_action_desc()
		**Description:Returns a string which describes how the room
		**should appear from adjacent rooms.
		***************************************************************/
		inline std::string get_action_desc() const
			{return action_description;}

		
		/***************************************************************
		**Function: int get_contents_size()
		**Description: Returns the size of the vector of Objects owned
		**by the Room.
		***************************************************************/
		inline int get_contents_size() const
			{return contents.size();}

		
		/***************************************************************
		**Function: string list_objects()
		**Description: Returns a formatted and ready-to-use string
		**describing the objects and/or console in the room.
		***************************************************************/
		std::string list_objects() const;
		

		/***************************************************************
		**Function: string interact_room()
		**Description: The first parameter is an index; it passes the
		**other two parameters to the interact() method of the Object
		**at that position in the contents vector.  It then deletes the
		**Object, and passes the return of its interact() method back
		**up.
		***************************************************************/
		std::string interact_room_object(int, Player&, bool*);

		
		/***************************************************************
		**Function: string interact_room_console()
		**Description: Passes its parameters to the interact() method of
		**the Console class, if and only if there's a console in the
		**Room.  Passes the Console.interact() return back up.
		***************************************************************/
		std::string interact_room_console(Player&, bool&);


		/***************************************************************
		**Function: add_exits()
		**Description: This simply allows defining the adjacencies for
		**the Room object; it will populate the four pointers in the
		**exits array, but does nothing else.
		***************************************************************/
		void add_exits(Room* n, Room* s, Room* e, Room* w);
		

		/***************************************************************
		**Function: get_exit()
		**Description: This simply returns the exit at the specified
		**index, which is a pointer-to-Room.
		***************************************************************/
		inline Room* get_exit(int i) const 
			{return exits[i];}

		
		/***************************************************************
		**Function: set_tool()
		**Description: Assigns its parameter to the Room's tool member.
		**If the argument passed is nullptr, it sets has_tool to false,
		**otherwise it sets it to true.
		***************************************************************/
		void set_tool(Item* t);

		
		/***************************************************************
		**Function: set_console()
		**Description: Assigns its parameter to the Room's console
		**member.  If the argument passed is nullptr, it sets
		**has_console to false, otherwise it sets it to true.
		***************************************************************/
		void set_console(Console* c);
		

		/***************************************************************
		**Function: get_has_tool()
		**Description: Simply returns has_tool.  Used during
		**randomization to decide whether an Object needs to be given
		**a tool.
		***************************************************************/
		inline bool get_has_tool() {return has_tool;}


		/***************************************************************
		**Function: get_has_console()
		**Description: Simply returns the has_console flag.
		***************************************************************/
		inline bool get_has_console() {return has_console;}


		/***************************************************************
		**Function: randomize()
		**Description: Just a stub; exists so that Holodeck::randomize()
		**can be called through a Room pointer.
		***************************************************************/
		virtual void randomize() {};		


		/***************************************************************
		**Function: ~Room()
		**Description: Empty destructor; the class does not allocate
		**dynamic memory, but a destructor is still required because it
		**is a parent class.
		***************************************************************/
		virtual ~Room();

		
		/***************************************************************
		**Function: assign_description()
		**Description: Opens rooms.txt and looks for specially formatted
		**strings, which are then assigned to the Room's various string
		**members.  Some rooms have fixed descriptions, and so are
		**passed identical min and max arguments; other have random
		**description, so min and max represent the range of valid 
		**string blocks to choose from.  Action is always fixed.
		***************************************************************/
		void assign_description(int min, int max, int action);
};

#endif
