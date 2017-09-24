#pragma once
#include<map>
#include<tuple>
#include<vector>
class SymbolTable {
public:
	struct SymboleNode {
		int iIndex; // index of vector
		std::wstring wstrIdent; // name of identifier
		int iSize; //size of indetifier 1 for variable, n for array
		int iStackIndex; // index of stack
		int iFuncIndex; // the scope of identifier
	};
	int AddSymbol(std::wstring indet, int size, int stackindex, int functionindex);
	std::tuple<bool,SymboleNode> GetSymbolByIndent(std::wstring name, int functindex);
	int GetStackIndexByIndent(std::wstring name, int funcindex);
	int GetSizeByIndent(std::wstring name, int funcindex);
private:
	std::map<std::wstring,int> IdentMap;
	std::vector<SymboleNode> IdentTable;
	
};