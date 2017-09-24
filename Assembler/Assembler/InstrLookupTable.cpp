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