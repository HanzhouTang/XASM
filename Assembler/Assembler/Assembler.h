#pragma once
#include"FunctionTable.h"
#include"HostCallTable.h"
#include"InstrLookupTable.h"
#include"InstructionOutStream.h"
#include"LabelTable.h"
#include"StringTable.h"
#include"SymbolTable.h"
#include<iostream>
#include<stdlib.h>
#include<string>
#include<fstream>
#include <algorithm>
#include <iterator>
#include <cwctype>
class Assembler {
public:
	inline static Assembler& Instance() {
		static Assembler assem;
		return assem;
	}
	inline void ExitOnError(std::wstring errormsg) {
		std::wcout << errormsg << std::endl;
		exit(-1);
	}
	void LoadSourceCode(std::wstring name);
	std::wstring& IgnoreComment(std::wstring& code);
	std::wstring& LeftTrim(std::wstring& code);
	std::wstring& RightTrim(std::wstring& code);
	std::wstring& Trim(std::wstring& code);
	void OutPut(std::wostream& = std::wcout);
private:
	static std::vector<std::wstring> codes;
	Assembler() {}
	Assembler(const Assembler&) {}
	Assembler& operator=(const Assembler&) {}
	inline FunctionTable& GetFunctionTable() {
		return FunctionTable::Instance();
	}
	inline InstrLookupTable& GetInstrLookupTable() {
		return InstrLookupTable::Instance();
	}
	inline LabelTable& GetLabelTable() {
		return LabelTable::Instance();
	}
	inline SymbolTable& GetSymbolTable() {
		return SymbolTable::Instance();
	}
	inline HostCallTable& GetHostCallTable() {
		return HostCallTable::Instance();
	}
	inline StringTable& GetStringTable() {
		return StringTable::Instance();
	}
};
