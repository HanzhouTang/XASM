#pragma once
#include<map>
#include<tuple>
#include<vector>
class LabelTable {
public:
	struct  LabelNode
	{
		std::wstring wstrIndent;
		int iTargetIndex; // index of InstructionOutStream's vector
		int iFuncIndex; //scope of label
		int iIndex;// index of vector
	};
	int AddLabel(std::wstring name, int targetindex, int funcindex);
	std::tuple<bool,LabelNode> GetLabelByIdent(std::wstring name, int funcindex);
	std::tuple<bool, LabelNode> operator[](std::size_t i);
	inline static LabelTable& Instance() {
		static LabelTable labeltable;
		return labeltable;
	}
private:
	static std::map<std::wstring,int> _LabelMap;
	static std::vector<LabelNode> _LabelTable;
	LabelTable(){}
	LabelTable(const LabelTable&){}
	LabelTable& operator=(const LabelTable&){}
};