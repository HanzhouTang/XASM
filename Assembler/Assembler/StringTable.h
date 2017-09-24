#pragma once
#include<map>
#include<vector>
class StringTable {
public:
	int AddString(std::wstring str);
private:
	std::map<std::wstring,int> StrMap;
	std::vector<std::wstring> StrTable;
};