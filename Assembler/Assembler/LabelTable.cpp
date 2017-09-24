#include"LabelTable.h"

std::tuple<bool,LabelTable::LabelNode> LabelTable::GetLabelByIdent(std::wstring name, int funcindex) {
	if (_LabelMap.find(name) != _LabelMap.end() &&
		_LabelTable[_LabelMap[name]].iFuncIndex==funcindex)
		return std::make_tuple(true, _LabelTable[_LabelMap[name]]);
	return std::make_tuple(false,LabelNode());
}

int LabelTable::AddLabel(std::wstring name, int targetindex, int funcindex) {
	auto tempIndex = GetLabelByIdent(name, funcindex);
	if (std::get<0>(tempIndex)) return std::get<1>(tempIndex).iIndex;
	LabelNode labelNode;
	labelNode.iFuncIndex = funcindex;
	labelNode.iTargetIndex = targetindex;
	labelNode.wstrIndent = name;
	labelNode.iIndex = _LabelTable.size();
	_LabelMap[name] = labelNode.iIndex;
	_LabelTable.push_back(labelNode);
	return labelNode.iIndex;
}