#pragma once
#include<map>
#include<tuple>
#include<vector>
#include<string>
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
	std::tuple<bool, SymboleNode> operator[](std::size_t i);
	int GetStackIndexByIndent(std::wstring name, int funcindex);
	int GetSizeByIndent(std::wstring name, int funcindex);
	std::size_t size() { return IdentTable.size(); }
	inline static SymbolTable& Instance() {
		static SymbolTable symboltable;
		return symboltable;
	}
private:
	static std::map<std::wstring,int> IdentMap;
	static std::vector<SymboleNode> IdentTable;
	SymbolTable(){}
	SymbolTable(const SymbolTable&){}
	SymbolTable& operator=(const SymbolTable&){}
	
};