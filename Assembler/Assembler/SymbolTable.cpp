#include"SymbolTable.h"

std::tuple<bool,SymbolTable::SymboleNode> SymbolTable::GetSymbolByIndent(std::wstring name, int funcindex) {
	if (IdentMap.find(name) != IdentMap.end() &&
		IdentTable[IdentMap[name]].iFuncIndex == funcindex)
		return std::make_tuple(true, IdentTable[IdentMap[name]]);
	return std::make_tuple(false, SymboleNode());
}
std::tuple<bool, SymbolTable::SymboleNode> SymbolTable::operator [](std::size_t i) {
	if (i >= IdentTable.size()) return std::make_tuple(false, SymboleNode());
	return std::make_tuple(true, IdentTable[i]);
}

int SymbolTable::AddSymbol(std::wstring name, int size, int stackindex, int funcindex) {
	auto tempIndex = GetSymbolByIndent(name, funcindex);
	if (std::get<0>(tempIndex)) return std::get<1>(tempIndex).iIndex;
	SymboleNode tempSymbol;
	tempSymbol.iFuncIndex = funcindex;
	tempSymbol.iSize = size;
	tempSymbol.iStackIndex = stackindex;
	tempSymbol.wstrIdent = name;
	tempSymbol.iIndex = IdentTable.size();
	IdentMap[name] = tempSymbol.iIndex;
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
//未来或许会改