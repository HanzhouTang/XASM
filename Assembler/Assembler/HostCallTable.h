#pragma once
#include<vector>
#include<map>
class HostCallTable {
public:
	int AddHostCall(std::wstring hostcall);
	inline static HostCallTable& Instance() {
		static HostCallTable hostcalltable;
		return hostcalltable;
	}
private:
	static std::map<std::wstring, int> _HostCallMap;
	static std::vector<std::wstring> _HostCallTable;
	HostCallTable() {}
	HostCallTable(const HostCallTable&) {}
	HostCallTable& operator=(const HostCallTable&){}
};