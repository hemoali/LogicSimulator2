#ifndef CONST_H
#define CONST_H

//This header file contains some defenitions to be used all over the application

//All possible actions
enum ActionType
{
	ADD_Buff,			//Add 1-input Buffer gate
	ADD_INV,			//Add 1-input Inverter gate
	ADD_AND_GATE_2,		//Add 2-input AND gate
	ADD_OR_GATE_2,		//Add 2-input OR gate
	ADD_NAND_GATE_2,	//Add 2-input NAND gate
	ADD_NOR_GATE_2,		//Add 2-input NOR gate
	ADD_XOR_GATE_2,		//Add 2-input XOR gate
	ADD_XNOR_GATE_2,	//Add 2-input XNOR gate
	ADD_AND_GATE_3,		//Add 3-input AND gate
	ADD_NOR_GATE_3,		//Add 3-input NOR gate
	ADD_XOR_GATE_3,		//Add 3-input XOR gate
	ADD_Switch,			//Add Switch
	ADD_LED,			//Add LED
	ADD_CONNECTION,		//Add Wire Connection
	ADD_Buff_H,			//Add 1-input Buffer gate
	ADD_INV_H,			//Add 1-input Inverter gate
	ADD_AND_GATE_2_H,		//Add 2-input AND gate
	ADD_OR_GATE_2_H,		//Add 2-input OR gate
	ADD_NAND_GATE_2_H,	//Add 2-input NAND gate
	ADD_NOR_GATE_2_H,		//Add 2-input NOR gate
	ADD_XOR_GATE_2_H,		//Add 2-input XOR gate
	ADD_XNOR_GATE_2_H,	//Add 2-input XNOR gate
	ADD_AND_GATE_3_H,		//Add 3-input AND gate
	ADD_NOR_GATE_3_H,		//Add 3-input NOR gate
	ADD_XOR_GATE_3_H,		//Add 3-input XOR gate
	ADD_Switch_H,			//Add Switch
	ADD_LED_H,			//Add LED
	ADD_CONNECTION_H,		//Add Wire Connection

	ADD_Label,			//Add Label to a Component, a Connection

	Create_TruthTable,	//Create Truth Table of the Circuit
	Simulate,
	Validate,

	Change_Switch,		//Change Switch Status in Simulation Mode // ????? TODO:

	SELECT,		//Select a Component, a Connection
	MULTI_SELECT,		//Select many Components, a Connection
	RIGHT_CLICKSELECT,
			
	MOVE,		//Move a Component, a Connection

	CUT,
	COPY,
	PASTE,
	EDIT_Label,	//Edit Label of a Component, a Connection
	DEL, //Delete a Component, a Connection

	SAVE,		//Save the whole Circuit to a file
	LOAD,		//Load a Circuit from a file

	UNDO,		//Undo the last Action preformed
	REDO,		//Redo the last Action canceled

	DSN_MODE,	//Switch to Design mode
	SIM_MODE,	//Switch to Simulatiom mode

	EXIT,		//Exit the application

	STATUS_BAR,		//A click on the status bar
	DSN_TOOL,	//A click on an empty place in the design tool bar

};
//Possible Status for the pin
enum STATUS
{
	LOW,
	HIGH
};

enum  MODE	//Modes of operation
{
	DESIGN,
	SIMULATION
};


enum DsgnMenuItem //The items of the design menu (you should add more items)
{
	//Note: Items are ordered here as they appear in menu
	//If you want to change the menu items order, change the order here
	D2AND,		//AND gate item in menu
	D3AND,
	D2OR,
	D2XOR,
	D3XOR,
	DBUFFER,
	DNOT,
	D2NAND,
	D2NOR,
	D3NOR,
	D2XNOR,
	DSWITCH,
	DLED,
	DCONNECTION,
	DSIMULATION,
	DSAVE,
	DLOAD,
	DEXIT,
	DITEMSCOUNT		//no. of design menu items ==> This should be the last line in this enum

};


enum SimMenuItem //The items of the simulation menu (you should add more items)
{
	//Note: Items are ordered here as they appear in menu
	SVALIDATE,	//Simulate menu item
	SSIMULATE,
	STT,	//Truth table menu item
	SDESIGN,
	SSAVE,
	SLOAD,
	SEXIT,
	SITEMSCOUNT		//no. of simulation menu items ==> This should be the last line in this enum

};
enum CellType{
	EMPTY,
	GATE,
	VERTICAL,
	HORIZONTAL,
	INTERSECTION,
	END_CONNECTION,
	PIN
};

//Maximum number of input pins that can be connected to any output pin
#define MAX_CONNS 20

//assume fan out is 5 for now it can be read from the user or can be predefined as const

#define	FANOUT 3

//A structure to contain drawing parameters for each component
//Each component occupies a rectangluar area so it needs 2 points to represent that area
//this structure can be extended if desired
struct GraphicsInfo
{
	int x1,y1, x2, y2;
};
struct Cell {
	int x, y;
	CellType cellType;
};
#ifndef NULL
#define NULL 0
#endif

#endif