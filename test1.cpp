#include <iostream>
#include <fstream>
#include <string>
#include "../spanir.pb.h"

using namespace std;


span::spill::Loc *loc;	//location
map<string, span::spill::VarExpr *> allVars; //list for all the global and local variables (in the form of a map)
vector<span::spill::AssignI *> assignInstrs; //all assignment instructions
map<int, span::spill::BB *> basicBlocks; //all basic blocks

void InitializeLoc()
{
	//Initializing the location object
	loc = new span::spill::Loc();
	loc->set_line(10);
	loc->set_col(11);
}
void InitializeVars()
{
	//Initializating the list with global and local variables
	allVars["v:main:argc"] = new span::spill::VarExpr();
	allVars["v:main:argc"]->set_type("INT32");
	allVars["v:main:argc"]->set_name("v:main:argc");
	allVars["v:main:argc"]->set_allocated_loc(loc);

	allVars["v:main:argv"] = new span::spill::VarExpr();
	allVars["v:main:argv"]->set_type("p-p-CHAR");
	allVars["v:main:argv"]->set_name("v:main:argv");
	allVars["v:main:argv"]->set_allocated_loc(loc);

	allVars["v:main:x"] = new span::spill::VarExpr();
	allVars["v:main:x"]->set_type("INT32");
	allVars["v:main:x"]->set_name("v:main:x");
	allVars["v:main:x"]->set_allocated_loc(loc);

	allVars["v:main:y"] = new span::spill::VarExpr();
 	allVars["v:main:y"]->set_type("INT32");
	allVars["v:main:y"]->set_name("v:main:y");
	allVars["v:main:y"]->set_allocated_loc(loc);

	allVars["v:main:z"] = new span::spill::VarExpr();
	allVars["v:main:z"]->set_type("INT32");
	allVars["v:main:z"]->set_name("v:main:z");
	allVars["v:main:z"]->set_allocated_loc(loc);

	allVars["v:g"] = new span::spill:VarExpr();
	allVars["v:g"]->set_type("INT32");
	allVars["v:g"]->set_name("v:g");
	allVars["v:g"]->set_allocated_loc(loc);
}

void InitializeAssignInstrs() {
	//Assignment Instruction: x = 10;
	assignInstrs.push_back(new span::spill::AssignI());
	assignInstrs[0]->set_type("ASSIGN_INSTR");
	assignInstrs[0]->set_allocated_varlhs(allVars["v:main:x"]);
	span::spill::LitExpr litExpr1 = new span::spill::LitExpr();
	litExpr1->set_type("LIT_EXPR");
	litExpr1->set_intval(10);
	litExpr1->set_allocated_loc(loc);
	assignInstrs[0]->set_allocated_litrhs(litExpr1);
	assignInstrs[0]->set_allocated_loc(loc);

	//Assignment Instruction: y = 20;
	assignInstrs.push_back(new span::spill::AssignI());
	assignInstrs[1]->set_type("ASSIGN_INSTR");
	assignInstrs[1]->set_allocated_varlhs(allVars["v:main:y"]);
	span::spill::LitExpr litExpr2 = new span::spill::LitExpr();
	litExpr2->set_type("LIT_EXPR");
	litExpr2->set_intval(20);
	litExpr2->set_allocated_loc(loc);
	assignInstrs[1]->set_allocated_litrhs(litExpr2);
	assignInstrs[1]->set_allocated_loc(loc);

	//Assignment Instruction: z = y;
	assignInstrs.push_back(new span::spill::AssignI());
	assignInstrs[2]->set_type("ASSIGN_INSTR");
	assignInstrs[2]->set_allocated_varlhs(allVars["v:main:z"]);
	assignInstrs[2]->set_allocated_varrhs(allVars["v:main:y"]);
	assignInstrs[2]->set_allocated_loc(loc);

	//Assignment Instruction: g = z;
	assignInstrs.push_back(new span::spill::AssignI());
	assignInstrs[3]->set_type("ASSIGN_INSTR");
	assignInstrs[3]->set_allocated_varlhs(allVars["v:g"]);
	assignInstrs[3]->set_allocated_varrhs(allVars["v:main:z"]);
	assignInstrs[3]->set_allocated_loc(loc);
}

int main(int argc, char *argv[])
{
	if (argc != 2){
		cerr << "Usage: " << argv[0] << " SPAN_IR_FILE" << endl;
		return -1;
	} 

	InitializeLoc();
	InitializeVars();
	InitializeAssignInstrs();

	basicBlocks[-1] = new span::spill::BB();
	basicBlocks[-1]->
	return 0;
}