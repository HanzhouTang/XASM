#include"InstrLookupTable.h"
std::tuple<bool, InstrLookupTable::InstrLookupNode> InstrLookupTable::
GetInstrByMnemonic(std::wstring name) {
	if (InstrLookupMap.find(name) != InstrLookupMap.end())
		return std::make_tuple(true, _InstrLookupTable[InstrLookupMap[name]]);
	return std::make_tuple(false, InstrLookupNode());
}

void InstrLookupTable::AddInstrLookup(std::wstring name, int opcode, int opcount) {
	auto temp = GetInstrByMnemonic(name);
	if (std::get<0>(temp)) return;
	InstrLookupNode t;
	t.iOpcode = opcode;
	t.iOpCount = opcount;
	t.wstrMnemonic = name;
	t.OpList = std::vector<OpTypes>(opcount);
	InstrLookupMap[name] = _InstrLookupTable.size();
	_InstrLookupTable.push_back(t);
}

void InstrLookupTable::SetOpType(std::wstring name, int opindex, OpTypes optype) {
	if (InstrLookupMap.find(name) != InstrLookupMap.end()) {
		_InstrLookupTable[InstrLookupMap[name]].OpList[opindex] = optype;
	}
}

std::vector<InstrLookupTable::InstrLookupNode> InstrLookupTable::_InstrLookupTable = {};
std::map<std::wstring, int> InstrLookupTable::InstrLookupMap = {};

InstrLookupTable::InstrLookupTable() {


	// ---- Main

	// Mov          Destination, Source

	AddInstrLookup(L"Mov", INSTR_MOV, 2);
	SetOpType(L"Mov", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"Mov", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// ---- Arithmetic

	// Add         Destination, Source

	AddInstrLookup(L"Add", INSTR_ADD, 2);
	SetOpType(L"Add", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"Add", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Sub          Destination, Source

	AddInstrLookup(L"Sub", INSTR_SUB, 2);
	SetOpType(L"Sub", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"Sub", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Mul          Destination, Source

	AddInstrLookup(L"Mul", INSTR_MUL, 2);
	SetOpType(L"Mul", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"Mul", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Div          Destination, Source

	AddInstrLookup(L"Div", INSTR_DIV, 2);
	SetOpType(L"Div", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"Div", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Mod          Destination, Source

	AddInstrLookup(L"Mod", INSTR_MOD, 2);
	SetOpType(L"Mod", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"Mod", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Exp          Destination, Source

	AddInstrLookup(L"Exp", INSTR_EXP, 2);
	SetOpType(L"Exp", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"Exp", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Neg          Destination

	AddInstrLookup(L"Neg", INSTR_NEG, 1);
	SetOpType(L"Neg", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Inc          Destination

	AddInstrLookup(L"Inc", INSTR_INC, 1);
	SetOpType(L"Inc", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Dec          Destination

	AddInstrLookup(L"Dec", INSTR_DEC, 1);
	SetOpType(L"Dec", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// ---- Bitwise

	// And          Destination, Source

	AddInstrLookup(L"And", INSTR_AND, 2);
	SetOpType(L"And", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"And", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Or           Destination, Source

	AddInstrLookup(L"Or", INSTR_OR, 2);
	SetOpType(L"Or", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"Or", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// XOr          Destination, Source

	AddInstrLookup(L"XOr", INSTR_XOR, 2);
	SetOpType(L"XOr", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"XOr", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Not          Destination

	AddInstrLookup(	L"Not", INSTR_NOT, 1);
	SetOpType(L"Not", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// ShL          Destination, Source

	AddInstrLookup(L"ShL", INSTR_SHL, 2);
	SetOpType(L"ShL", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"ShL", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// ShR          Destination, Source

	AddInstrLookup(L"ShR", INSTR_SHR, 2);
	SetOpType(L"ShR", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"ShR", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// ---- String Manipulation

	// Concat       String0, String1

    AddInstrLookup(L"Concat", INSTR_CONCAT, 2);
	SetOpType(L"Concat", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"Concat", 1, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG |
		OP_FLAG_TYPE_STRING);

	// GetChar      Destination, Source, Index

	AddInstrLookup(L"GetChar", INSTR_GETCHAR, 3);
	SetOpType(L"GetChar", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"GetChar", 1, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG |
		OP_FLAG_TYPE_STRING);
	SetOpType(L"GetChar", 2, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG |
		OP_FLAG_TYPE_INT);

	// SetChar      Destination, Index, Source

	AddInstrLookup(L"SetChar", INSTR_SETCHAR, 3);
	SetOpType(L"SetChar", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"SetChar", 1, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG |
		OP_FLAG_TYPE_INT);
	SetOpType(L"SetChar", 2, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG |
		OP_FLAG_TYPE_STRING);

	// ---- Conditional Branching

	// Jmp          Label

	AddInstrLookup(L"Jmp", INSTR_JMP, 1);
	SetOpType(L"Jmp", 0, OP_FLAG_TYPE_LINE_LABEL);

	// JE           Op0, Op1, Label

	AddInstrLookup(L"JE", INSTR_JE, 3);
	SetOpType(L"JE", 0, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JE", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JE", 2, OP_FLAG_TYPE_LINE_LABEL);

	// JNE          Op0, Op1, Label

	AddInstrLookup(L"JNE", INSTR_JNE, 3);
	SetOpType(L"JNE", 0, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JNE", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JNE", 2, OP_FLAG_TYPE_LINE_LABEL);

	// JG           Op0, Op1, Label

	AddInstrLookup(L"JG", INSTR_JG, 3);
	SetOpType(L"JG", 0, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JG", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JG", 2, OP_FLAG_TYPE_LINE_LABEL);

	// JL           Op0, Op1, Label

	AddInstrLookup(L"JL", INSTR_JL, 3);
	SetOpType(L"JL", 0, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JL", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JL", 2, OP_FLAG_TYPE_LINE_LABEL);

	// JGE          Op0, Op1, Label

	AddInstrLookup(L"JGE", INSTR_JGE, 3);
	SetOpType(L"JGE", 0, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JGE", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JGE", 2, OP_FLAG_TYPE_LINE_LABEL);

	// JLE           Op0, Op1, Label

	AddInstrLookup(L"JLE", INSTR_JLE, 3);
	SetOpType(L"JLE", 0, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JLE", 1, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);
	SetOpType(L"JLE", 2, OP_FLAG_TYPE_LINE_LABEL);

	// ---- The Stack Interface

	// Push          Source

	AddInstrLookup(L"Push", INSTR_PUSH, 1);
	SetOpType(L"Push", 0, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Pop           Destination

	AddInstrLookup(L"Pop", INSTR_POP, 1);
	SetOpType(L"Pop", 0, OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// ---- The Function Interface

	// Call          FunctionName

	AddInstrLookup(L"Call", INSTR_CALL, 1);
	SetOpType(L"Call", 0, OP_FLAG_TYPE_FUNC_NAME);

	// Ret

	AddInstrLookup(L"Ret", INSTR_RET, 0);

	// CallHost      FunctionName

	AddInstrLookup(L"CallHost", INSTR_CALLHOST, 1);
	SetOpType(L"CallHost", 0, OP_FLAG_TYPE_HOST_API_CALL);

	// ---- Miscellaneous

	// Pause        Duration

	AddInstrLookup(L"Pause", INSTR_PAUSE, 1);
	SetOpType(L"Pause", 0, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);

	// Exit         Code

	AddInstrLookup(L"Exit", INSTR_EXIT, 1);
	SetOpType(L"Exit", 0, OP_FLAG_TYPE_INT |
		OP_FLAG_TYPE_FLOAT |
		OP_FLAG_TYPE_STRING |
		OP_FLAG_TYPE_MEM_REF |
		OP_FLAG_TYPE_REG);


}