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
#include <locale>
#include <codecvt>
#include<regex>
#include<iomanip>
class Assembler {
public:
	inline static Assembler& Instance() {
		static Assembler assem;
		return assem;
	}
	inline void ExitOnError(std::wstring errormsg);
	typedef int Token;
	void LoadSourceCode(std::wstring name);
	std::wstring& StripComment(std::wstring& code);
	std::wstring& LeftTrim(std::wstring& code);
	std::wstring& RightTrim(std::wstring& code);
	std::wstring& Trim(std::wstring& code);
	void OutPut(std::wostream& = std::wcout);
	inline bool IsCharWhitespace(wint_t ch);
	inline bool IsCharDelimiter(wint_t ch);
	inline bool IsStringFloat(std::wstring str);
	inline bool IsStringInt(std::wstring str);
	inline bool IsStringIdent(std::wstring str);
	inline int StringToInt(std::wstring str);
	inline float StringToFloat(std::wstring str);
	Token GetNextToken();
	void ResetLexer();
	void Compile();
	void BuildXSE(std::wstring name);
	std::wstring GetTokenName(Token t);
	void Statistic(std::wstring name) {
		using std::wcout;
		using std::endl;
		using std::to_wstring;
		auto& functionTable = GetFunctionTable();
		auto& symbolTable = GetSymbolTable();
		auto& labelTable = GetLabelTable();
		auto& instrLookupTable = GetInstrLookupTable();
		auto& instructionOutStream = GetInstrctionOutStream();
		auto& stringTable = GetStringTable();
		wcout << L"生成：               " + name + L"文件成功!"<<endl;
		wcout << L"共处理了：           " + to_wstring(codes.size()) + L"行汇编" << endl;
		wcout << L"栈大小：             " + to_wstring(scriptHeader.stackSize) << endl;
		wcout << L"生成指令流大小为：   " << to_wstring(instructionOutStream.size()) << endl;
		wcout << L"变量：               " << to_wstring(symbolTable.size()) << L"个" << endl;
		wcout << L"字符串：             " << to_wstring(stringTable.size()) << L"条" << endl;
		wcout << L"函数：               " << to_wstring(functionTable.size()) << L"个" << endl;
		if (scriptHeader.isMainFuncPresent) {
			wcout << "Main函数：            " << to_wstring(scriptHeader.mainFuncIndex) << endl;
		}
	}
	wchar_t GetLookAheadChar();
private:

	struct Lexer {
		std::size_t uiCurrentSourceLine;// current line in codes
		std::wstring::iterator ptrIndex0, ptrIndex1;// indices of string
		std::wstring CurrentLexeme;
		Token CurrentToken;
		int iCurrentLexState;
	};
	struct Code {
		std::size_t lineNumber;
		std::wstring code;
		Code(std::size_t n, std::wstring s) {
			lineNumber = n;
			code = s;
		}
	};
	struct ScriptHeader {
		std::size_t stackSize;
		std::size_t globalDataSize;
		bool isMainFuncPresent;
		std::size_t mainFuncIndex;
		ScriptHeader() {
			stackSize = 1024;
			globalDataSize = 0;
			isMainFuncPresent = false;
			mainFuncIndex = 0;
		}
	};
	ScriptHeader scriptHeader;
	Lexer lexer;
	static std::vector<Code> codes;
	bool isSetStackSizeFound;
	bool isInAFunction;

	Assembler() {
		std::wcout.imbue(std::locale("chs"));
		system("chcp 936");
	}
	Assembler(const Assembler&) {}
	Assembler& operator=(const Assembler&) {}
	inline FunctionTable& GetFunctionTable();
	inline InstrLookupTable& GetInstrLookupTable();
	inline LabelTable& GetLabelTable();
	inline SymbolTable& GetSymbolTable();
	inline HostCallTable& GetHostCallTable();
	inline StringTable& GetStringTable();
	inline InstructionOutStream& GetInstrctionOutStream();
	inline void ExitOnCodeError(std::wstring msg) {
		std::cout << "Line: " << codes[lexer.uiCurrentSourceLine].lineNumber << std::endl;
		std::wcout << codes[lexer.uiCurrentSourceLine].code;
		for (auto ptr = codes[lexer.uiCurrentSourceLine].code.begin(); ptr != lexer.ptrIndex0; ptr++) {
			std::cout << " ";
		}
		std::cout << "^" << std::endl;
		std::wcout << L"Error: " << msg << std::endl;
		exit(-1);
	}
	bool SkipToNextLine() {
		lexer.uiCurrentSourceLine++;
		if (lexer.uiCurrentSourceLine >= codes.size()) return false;
		lexer.ptrIndex0 = codes[lexer.uiCurrentSourceLine].code.begin();
		lexer.ptrIndex1 = lexer.ptrIndex0;
		lexer.iCurrentLexState = LEX_STATE_NO_STRING;
		return true;
	}
};
