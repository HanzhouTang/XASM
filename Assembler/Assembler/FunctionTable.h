#pragma once
#include<map>
#include<tuple>
#include<vector>
class FunctionTable {
public:
	struct FuncNode{
		std::wstring wstrName; //function name
		int iIndex; // index of vector
		int iEntryPoint; // the entry point of function
		int iParamCOunt; // the amount of parameters in the function
		int iLocalDataSize; // the size of storage this function needed
	};
	int AddFunc(std::wstring name, int Entry);
	void SetFuncInfo(std::wstring name, int paramcount, int local);
	std::tuple<bool,FuncNode> GetFunctionByName(std::wstring name);
	std::tuple<bool, FuncNode> operator[](std::size_t i);
	inline static FunctionTable& Instance() {
		static FunctionTable functable;
		return functable;
	}
private:
	FunctionTable(){}
	FunctionTable(const FunctionTable&){}
	FunctionTable& operator=(const FunctionTable&){}
	static std::map<std::wstring,int> FuncMap;
	static std::vector<FuncNode> FuncTable;
};