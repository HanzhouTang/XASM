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
private:
	std::map<std::wstring,int> _LabelMap;
	std::vector<LabelNode> _LabelTable;
};