#pragma once
#include"FunctionTable.h"
#include"HostCallTable.h"
#include"InstrLookupTable.h"
#include"InstructionOutStream.h"
#include"LabelTable.h"
#include"StringTable.h"
#include"SymbolTable.h"
#include <sstream>
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
	typedef int Token;
	void LoadSourceCode(std::wstring name);
	std::wstring& StripComment(std::wstring& code);
	std::wstring& LeftTrim(std::wstring& code);
	std::wstring& RightTrim(std::wstring& code);
	std::wstring& Trim(std::wstring& code);
	void OutPut(std::wostream& = std::wcout);
	inline bool IsCharWhitespace(wint_t ch) {
		if (ch == L' ' || ch == L'\t') return true;
		return false;
	}
	inline bool IsCharDelimiter(wint_t ch) {
		if (ch == L':' || ch == L'"' || ch == L',' ||
			ch == L'[' || ch == L']' ||
			ch == L'{' || ch == L'}' ||
			IsCharWhitespace(ch))
			return true;
		return false;
	}
	inline bool IsStringFloat(std::wstring str) {
		std::wistringstream iss(str);
		float f;
		iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
							  // Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();

	}
	inline bool IsStringInt(std::wstring str) {
		std::wistringstream iss(str);
		int f;
		iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
								   // Check the entire string was consumed and if either failbit or badbit is set
		return iss.eof() && !iss.fail();
	}

	inline bool IsStringIdent(std::wstring str) {
		if (str.length() == 0) return false;
		std::wstring temp;
		std::copy_if(str.begin(), str.end(), std::back_inserter(temp), [](wint_t ch) {
			return (ch == L'_' || (ch >= L'a'&&ch <= L'z') ||
				   (ch >= L'A'&&ch <= L'Z') || (ch >= L'0'&&ch <= L'9')); });
		return temp.length() == str.length();
	}
	Token GetNextToken();
	void ResetLexer();
	std::wstring GetTokenName(Token t);
private:
	
	struct Lexer {
		std::size_t uiCurrentSourceLine;// current line in codes
		std::wstring::iterator ptrIndex0, ptrIndex1;// indices of string
		std::wstring CurrentLexeme;
		Token CurrentToken;
		int iCurrentLexState;
	};
	Lexer lexer;
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
