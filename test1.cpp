#include <iostream>
#include <fstream>
#include <string>
#include "../spanir.pb.h"

using namespace std;

span::spill::Loc loc;	//location
map<string, span::spill::VarExpr> allVars; //list for all the global and local variables (in the form of a map)
vector<span::spill::AssignI> assignInstrs; //all assignment instructions
map<int, span::spill::BB> basicBlocks; //all basic blocks
vector<span::spill::BBEdge> basicBlockEdges; //list of all edges between basic blocks
vector<span::spill::Instruction> allInstructions; //list of all instructions

void InitializeLoc() {
	//Initializing the location object
	loc.set_line(10);
	loc.set_col(11);
}
void InitializeVars() {
	//Initializating the list with global and local variables

	//int argc (parameter of main function)
	span::spill::VarExpr varExpr1;
	varExpr1.set_type("INT32");
	varExpr1.set_name("v:main:argc");
	*varExpr1.mutable_loc() = loc;
	allVars["v:main:argc"] = varExpr1;
	
	//char *argv[] (parameter of main function)
	span::spill::VarExpr varExpr2;
	varExpr2.set_type("p-p-CHAR");
	varExpr2.set_name("v:main:argv");
	*varExpr2.mutable_loc() = loc;
	allVars["v:main:argv"] = varExpr2;

	//int x (local to main function)
	span::spill::VarExpr varExpr3;
	varExpr3.set_type("INT32");
	varExpr3.set_name("v:main:x");
	*varExpr3.mutable_loc() = loc;
	allVars["v:main:x"] = varExpr3;

	//int y (local to main function)
	span::spill::VarExpr varExpr4;
	varExpr4.set_type("INT32");
	varExpr4.set_name("v:main:y");
	*varExpr4.mutable_loc() = loc;
	allVars["v:main:y"] = varExpr4;

	//int z (local to main function)
	span::spill::VarExpr varExpr5;
	varExpr5.set_type("INT32");
	varExpr5.set_name("v:main:z");
	*varExpr5.mutable_loc() = loc;
	allVars["v:main:z"] = varExpr5;

	//int g (global variable)
	span::spill::VarExpr varExpr6;
	varExpr6.set_type("INT32");
	varExpr6.set_name("v:g");
	*varExpr6.mutable_loc() = loc;
	allVars["v:g"] = varExpr6;
}

void InitializeAssignInstrs() {
	//Assignment Instruction: x = 10;
	span::spill::AssignI assignInstr1;
	assignInstr1.set_type("ASSIGN_INSTR");
	*assignInstr1.mutable_varlhs() = allVars["v:main:x"];
	span::spill::LitExpr litExpr1;
	litExpr1.set_type("LIT_EXPR");
	litExpr1.set_intval(10);
	*litExpr1.mutable_loc() = loc;
	*assignInstr1.mutable_litrhs() = litExpr1;
	*assignInstr1.mutable_loc() = loc;
	assignInstrs.push_back(assignInstr1);

	//Assignment Instruction: y = 20;
	span::spill::AssignI assignInstr2;
	assignInstr1.set_type("ASSIGN_INSTR");
	*assignInstr1.mutable_varlhs() = allVars["v:main:y"];
	span::spill::LitExpr litExpr2;
	litExpr2.set_type("LIT_EXPR");
	litExpr2.set_intval(20);
	*litExpr2.mutable_loc() = loc;
	*assignInstr2.mutable_litrhs() = litExpr2;
	*assignInstr2.mutable_loc() = loc;
	assignInstrs.push_back(assignInstr2);

	//Assignment Instruction: z = y;
	span::spill::AssignI assignInstr3;
	assignInstr3.set_type("ASSIGN_INSTR");
	*assignInstr3.mutable_varlhs() = allVars["v:main:z"];
	*assignInstr3.mutable_varrhs() = allVars["v:main:y"];
	*assignInstr3.mutable_loc() = loc;
	assignInstrs.push_back(assignInstr3);

	//Assignment Instruction: g = z;
	span::spill::AssignI assignInstr4;
	assignInstr4.set_type("ASSIGN_INSTR");
	*assignInstr4.mutable_varlhs() = allVars["v:g"];
	*assignInstr4.mutable_varrhs() = allVars["v:main:z"];
	*assignInstr4.mutable_loc() = loc;
	assignInstrs.push_back(assignInstr4);
}

void InitializeInstrs() {
	//Pushing all the assignment instructions into the list of all instructions
	for (int i = 0; i < assignInstrs.size(); i++)
	{
		span::spill::Instruction instr;
		*instr.mutable_assigninsn() = assignInstrs[i];
		allInstructions.push_back(instr);
	}
}

int main(int argc, char *argv[])
{
  	// Verify that the version of the library that we linked against is
  	// compatible with the version of the headers we compiled against.
  	GOOGLE_PROTOBUF_VERIFY_VERSION;
	
	if (argc != 2){
		cerr << "Usage: " << argv[0] << " SPAN_IR_FILE" << endl;
		return -1;
	} 

	InitializeLoc();
	InitializeVars();
	InitializeAssignInstrs();
	InitializeInstrs();

	//Creating basic blocks
	span::spill::BB bb1, bb2;
	
	//-1 is always the start/entry basic block
	*bb1.mutable_insns() = {allInstructions.begin(), allInstructions.end()};
	bb1.set_id(-1);

	//0 is always end/exit block
	bb2.set_id(0);

	//Creating an edge between basic blocks -1 and 0
	span::spill::BBEdge bbEdge;
	bbEdge.set_start(-1);
	bbEdge.set_end(0);
	bbEdge.set_edgekind(span::spill::EdgeKind::UNCOND_EDGE);
	basicBlockEdges.push_back(bbEdge);

	//The edge (-1, 0) will be a successor for basic block -1
 	*bb1.mutable_successors() = {basicBlockEdges.begin(), basicBlockEdges.end()};

 	//The edge (-1, 0) will be a predecessor for basic block 0
 	*bb2.mutable_predecessors() = {basicBlockEdges.begin(), basicBlockEdges.end()};

 	//Mapping the basic blocks -1 and 0
	basicBlocks[-1] = bb1;
	basicBlocks[0] = bb2;

	//Serializing basic blocks -1 and 0
	fstream output(argv[1], ios::out | ios::trunc | ios::binary);

	if (!basicBlocks[-1].SerializeToOstream(&output)) {
		cerr << "Failed to serialize basic block -1." << endl;
		return -1;
	}

	if (!basicBlocks[0].SerializeToOstream(&output)) {
		cerr << "Failed to serialize basic block 0." << endl;
		return -1;
	}

	//Deleting all the global objects allocated by libprotobuf
	google::protobuf::ShutdownProtobufLibrary();

 	return 0;
}
