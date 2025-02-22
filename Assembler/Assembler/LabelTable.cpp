#include"LabelTable.h"

std::tuple<bool,LabelTable::LabelNode> LabelTable::GetLabelByIdent(std::wstring name, int funcindex) {
	if (_LabelMap.find(name) != _LabelMap.end() &&
		_LabelTable[_LabelMap[name]].iFuncIndex==funcindex)
		return std::make_tuple(true, _LabelTable[_LabelMap[name]]);
	return std::make_tuple(false,LabelNode());
}

std::tuple<bool, LabelTable::LabelNode> LabelTable::operator[](std::size_t i) {
	if(i>=_LabelTable.size()) return std::make_tuple(false, LabelNode());
	return std::make_tuple(true, _LabelTable[i]);
}

int LabelTable::AddLabel(std::wstring name, int targetindex, int funcindex) {
	auto tempIndex = GetLabelByIdent(name, funcindex);
	if (std::get<0>(tempIndex)) {
		if(std::get<1>(tempIndex).iFuncIndex==funcindex)
		return -1;
	}
	LabelNode labelNode;
	labelNode.iFuncIndex = funcindex;
	labelNode.iTargetIndex = targetindex;
	labelNode.wstrIndent = name;
	labelNode.iIndex = _LabelTable.size();
	_LabelMap[name] = labelNode.iIndex;
	_LabelTable.push_back(labelNode);
	return labelNode.iIndex;
}

std::vector<LabelTable::LabelNode> LabelTable::_LabelTable = {};
std::map<std::wstring, int> LabelTable::_LabelMap = {};