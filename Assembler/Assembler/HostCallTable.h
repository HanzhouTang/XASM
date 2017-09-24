#pragma once
#include<vector>
#include<map>
class HostCallTable {
public:
	int AddHostCall(std::wstring hostcall);
private:
	std::map<std::wstring, int> _HostCallMap;
	std::vector<std::wstring> _HostCallTable;
};