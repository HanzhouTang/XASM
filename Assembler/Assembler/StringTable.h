#pragma once
#include<map>
#include<vector>
class StringTable {
public:
	int AddString(std::wstring str);
	inline static StringTable& Instance() {
		static StringTable stable;
		return stable;
	}
private:
	static std::map<std::wstring,int> StrMap;
	static std::vector<std::wstring> StrTable;
	StringTable(){}
	StringTable(const StringTable&){}
	StringTable& operator=(const StringTable&){}
};