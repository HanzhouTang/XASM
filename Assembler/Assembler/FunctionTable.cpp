#include"FunctionTable.h"

std::tuple<bool,FunctionTable::FuncNode> FunctionTable::GetFunctionByName(std::wstring name) {
	if (FuncMap.find(name) != FuncMap.end()) return std::make_tuple(true, FuncTable[FuncMap[name]]);
	return std::make_tuple(false, FuncNode());
}

std::tuple<bool, FunctionTable::FuncNode> FunctionTable::operator[](std::size_t i) {
	if (i >= FuncTable.size()) return std::make_tuple(false, FuncNode());
	return std::make_tuple(true, FuncTable[i]);
}


void FunctionTable::SetFuncInfo(std::wstring name, int patamCount, int LocalDataSize) {
	auto temp = GetFunctionByName(name);
	if (std::get<0>(temp)) {
		FuncTable[FuncMap[name]].iLocalDataSize = LocalDataSize;
		FuncTable[FuncMap[name]].iParamCount = patamCount;
	}
}
int FunctionTable::AddFunc(std::wstring name, int entry) {
	auto tempIndex = GetFunctionByName(name);
	if (std::get<0>(tempIndex)) return -1;
	FuncNode temp;
	temp.wstrName = name;
	temp.iEntryPoint = entry;
	temp.iIndex = FuncTable.size();
	FuncMap[name] = temp.iIndex;
	FuncTable.push_back(temp);
	return temp.iIndex;
}

std::map<std::wstring, int> FunctionTable::FuncMap = {};
std::vector<FunctionTable::FuncNode> FunctionTable::FuncTable = {};
