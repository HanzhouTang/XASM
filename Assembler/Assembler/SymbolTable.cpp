#include"SymbolTable.h"

std::tuple<bool,SymbolTable::SymboleNode> SymbolTable::GetSymbolByIndent(std::wstring name, int funcindex) {
	using std::to_wstring;
	if (IdentMap.find(name+to_wstring(funcindex)) != IdentMap.end() )
		return std::make_tuple(true, IdentTable[IdentMap[name]]);
	return std::make_tuple(false, SymboleNode());
}
std::tuple<bool, SymbolTable::SymboleNode> SymbolTable::operator [](std::size_t i) {
	if (i >= IdentTable.size()) return std::make_tuple(false, SymboleNode());
	return std::make_tuple(true, IdentTable[i]);
}

int SymbolTable::AddSymbol(std::wstring name, int size, int stackindex, int funcindex) {
	using std::to_wstring;
	auto tempIndex = GetSymbolByIndent(name, funcindex);
	if (std::get<0>(tempIndex)) return -1;
	SymboleNode tempSymbol;
	tempSymbol.iFuncIndex = funcindex;
	tempSymbol.iSize = size;
	tempSymbol.iStackIndex = stackindex;
	tempSymbol.wstrIdent = name;
	tempSymbol.iIndex = IdentTable.size();
	IdentMap[name+to_wstring(funcindex)] = tempSymbol.iIndex;
	IdentTable.push_back(tempSymbol);
	return tempSymbol.iIndex;
}

int SymbolTable::GetSizeByIndent(std::wstring name, int funcindex) {
	auto tempIndex = GetSymbolByIndent(name, funcindex);
	if (!std::get<0>(tempIndex)) return - 1;
	return std::get<1>(tempIndex).iSize;
}

int SymbolTable::GetStackIndexByIndent(std::wstring name, int funcindex) {
	auto tempIndex = GetSymbolByIndent(name, funcindex);
	if (!std::get<0>(tempIndex)) return -1;
	return std::get<1>(tempIndex).iStackIndex;
}

std::vector<SymbolTable::SymboleNode> SymbolTable::IdentTable = {};
std::map<std::wstring, int> SymbolTable::IdentMap = {};
