#pragma once
#include"constant.h"
#include<vector>
#include<map>
#include<tuple>
class InstrLookupTable{
public:
	typedef int OpTypes;
	struct InstrLookupNode {
		std::wstring wstrMnemonic; //Mnemonic string
		int iOpcode; 
		int iOpCount; 
		std::vector<OpTypes> OpList;
	};
	void AddInstrLookup(std::wstring name, int opcode, int opcount);
	void SetOpType(std::wstring name, int opIndex, OpTypes optype);
	std::tuple<bool, InstrLookupNode> GetInstrByMnemonic(std::wstring name);
	inline static InstrLookupTable& Instance(){
		static InstrLookupTable instr;
		return instr;
	}
private:
	static std::map<std::wstring, int> InstrLookupMap;
	static std::vector<InstrLookupNode> _InstrLookupTable;
	InstrLookupTable();
	InstrLookupTable(const InstrLookupTable&){}
	InstrLookupTable& operator=(const InstrLookupTable&){}
};
