#pragma once
#include<map>
#include<tuple>
#include<vector>
class FunctionTable {
public:
	typedef struct FuncNode{
		std::wstring wstrName; //function name
		int iIndex; // index of vector
		int iEntryPoint; // the entry point of function
		int iParamCOunt; // the amount of parameters in the function
		int iLocalDataSize; // the size of storage this function needed
	};
	int AddFunc(std::wstring name, int Entry);
	void SetFuncInfo(std::wstring name, int paramcount, int local);
	std::tuple<bool,FuncNode> GetFunctionByName(std::wstring name);
private:
	std::map<std::wstring,int> FuncMap;
	std::vector<FuncNode> FuncTable;

};