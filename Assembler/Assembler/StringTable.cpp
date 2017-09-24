#include"StringTable.h"
int StringTable::AddString(std::wstring str) {
	if (StrMap.find(str) != StrMap.end()) return StrMap[str];
	StrMap[str] = StrTable.size();
	StrTable.push_back(str);
	return StrTable.size() - 1;
}
