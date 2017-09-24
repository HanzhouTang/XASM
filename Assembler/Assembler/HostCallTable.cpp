#include"HostCallTable.h"
int HostCallTable::AddHostCall(std::wstring str) {
	if (_HostCallMap.find(str) != _HostCallMap.end()) return _HostCallMap[str];
	_HostCallMap[str] = _HostCallTable.size();
	_HostCallTable.push_back(str);
	return _HostCallTable.size() - 1;
}