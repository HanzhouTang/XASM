#include"Assembler.h"
std::vector<Assembler::Code> Assembler::codes = {};
void Assembler::LoadSourceCode(std::wstring name) {
	std::wifstream read(name, std::ios::binary);
	read.imbue(std::locale(std::locale("chs"),
		new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
	int i = 0;
	if (!read.is_open()) {
		ExitOnError(L"Could not open file #" + name);
	}
	wchar_t str[256];
	while (!read.eof()) {
		i++;
		read.getline(str, 256);
		std::wstring temp(str);
		StripComment(temp);
		Trim(temp);
		if (!temp.empty())
			codes.push_back(Code(i, temp + L"\n"));
	}
	read.close();
}

void Assembler::OutPut(std::wostream& stream) {
	for (const auto& x : codes) {
		std::wcout << x.lineNumber << ":\t" << x.code;
	}
}

std::wstring& Assembler::StripComment(std::wstring& code) {
	bool instring = false;
	std::wstring::iterator ptr;
	for (ptr = code.begin(); ptr != code.end(); ptr++) {
		if (*ptr == L'"') instring = !instring;
		if (*ptr == L';' && !instring) break;
	}
	code.erase(ptr, code.end());
	return code;
}

std::wstring& Assembler::LeftTrim(std::wstring& code) {
	code.erase(code.begin(), std::find_if(code.begin(), code.end(),
		[](wint_t ch) {return !std::iswspace(ch); }));
	return code;
}

std::wstring& Assembler::RightTrim(std::wstring& code) {
	code.erase(std::find_if(code.rbegin(), code.rend(),
		[](wint_t ch) { return !std::iswspace(ch); }).base(), code.end());
	return code;
}
std::wstring& Assembler::Trim(std::wstring& code) {
	LeftTrim(code);
	RightTrim(code);
	return code;
}

Assembler::Token Assembler::GetNextToken() {
	std::wstring tempstring;
	lexer.ptrIndex0 = lexer.ptrIndex1;
	if (lexer.ptrIndex0 == codes[lexer.uiCurrentSourceLine].code.end()) {
		lexer.iCurrentLexState = LEX_STATE_NO_STRING;
		lexer.uiCurrentSourceLine++;
		if (lexer.uiCurrentSourceLine == codes.size()) {
			lexer.CurrentToken = END_OF_TOKEN_STREAM;
			return lexer.CurrentToken;
		}
		lexer.ptrIndex0 = lexer.ptrIndex1 = codes[lexer.uiCurrentSourceLine].code.begin();
	}
	auto ptrEnd = codes[lexer.uiCurrentSourceLine].code.end();
	if (lexer.iCurrentLexState == LEX_STATE_IN_STRING) {
		tempstring.clear();
		for (lexer.ptrIndex1 = lexer.ptrIndex0; lexer.ptrIndex1 != ptrEnd && (*lexer.ptrIndex1) != L'"'; lexer.ptrIndex1++) {
			if (*lexer.ptrIndex1 == L'\\') lexer.ptrIndex1++;
			tempstring.push_back(*lexer.ptrIndex1);
		}
		if (lexer.ptrIndex1 == ptrEnd) {
			lexer.CurrentToken = TOKEN_TYPE_INVALID;
			return lexer.CurrentToken;
		}
	}
	else {
		lexer.ptrIndex0 = std::find_if(lexer.ptrIndex0, ptrEnd, [this](wint_t ch) {
			return !IsCharWhitespace(ch);
		});
		lexer.ptrIndex1 = std::find_if(lexer.ptrIndex0, ptrEnd, [this](wint_t ch) {
			return IsCharDelimiter(ch) || ch == L'\n';
		});
		if (lexer.ptrIndex1 == lexer.ptrIndex0) lexer.ptrIndex1++;
	}
	lexer.CurrentLexeme = std::wstring(lexer.ptrIndex0, lexer.ptrIndex1);
	lexer.CurrentToken = TOKEN_TYPE_INVALID;
	if (lexer.iCurrentLexState == LEX_STATE_IN_STRING) {
		lexer.CurrentToken = TOKEN_TYPE_STRING;
		lexer.iCurrentLexState = LEX_STATE_END_STRING;
		lexer.CurrentLexeme = tempstring;
		return lexer.CurrentToken;
	}
	if (lexer.CurrentLexeme.length() == 1) {
		switch (lexer.CurrentLexeme[0])
		{
		case L'"':
			if (lexer.iCurrentLexState == LEX_STATE_NO_STRING) {
				lexer.iCurrentLexState = LEX_STATE_IN_STRING;
			}
			lexer.CurrentToken = TOKEN_TYPE_QUOTE;
			break;
		case L',':
			lexer.CurrentToken = TOKEN_TYPE_COMMA;
			break;
		case L':':
			lexer.CurrentToken = TOKEN_TYPE_COLON;
			break;
		case L'[':
			lexer.CurrentToken = TOKEN_TYPE_OPEN_BRACKET;
			break;
		case L']':
			lexer.CurrentToken = TOKEN_TYPE_CLOSE_BRACKET;
			break;
		case L'{':
			lexer.CurrentToken = TOKEN_TYPE_OPEN_BRACE;
			break;
		case L'}':
			lexer.CurrentToken = TOKEN_TYPE_CLOSE_BRACE;
			break;
		case L'\n':
			lexer.CurrentToken = TOKEN_TYPE_NEWLINE;
			break;
		}
		if (IsStringInt(lexer.CurrentLexeme)) {
			lexer.CurrentToken = TOKEN_TYPE_INT;
		}
		else if (IsStringIdent(lexer.CurrentLexeme)) {
			lexer.CurrentToken = TOKEN_TYPE_IDENT;
		}
	}
	else {
		auto& instLookupTable = GetInstrLookupTable();
		auto ret = instLookupTable.GetInstrByMnemonic(lexer.CurrentLexeme);
		if (IsStringInt(lexer.CurrentLexeme)) {
			lexer.CurrentToken = TOKEN_TYPE_INT;
		}
		else if (IsStringFloat(lexer.CurrentLexeme)) {
			lexer.CurrentToken = TOKEN_TYPE_FLOAT;
		}
		else if (lexer.CurrentLexeme == L"SetStackSize") {
			lexer.CurrentToken = TOKEN_TYPE_SETSTACKSIZE;
		}
		else if (lexer.CurrentLexeme == L"Var") {
			lexer.CurrentToken = TOKEN_TYPE_VAR;
		}
		else if (lexer.CurrentLexeme == L"Func") {
			lexer.CurrentToken = TOKEN_TYPE_FUNC;
		}
		else if (lexer.CurrentLexeme == L"Param") {
			lexer.CurrentToken = TOKEN_TYPE_PARAM;
		}
		else if (lexer.CurrentLexeme == L"_RetVal") {
			lexer.CurrentToken = TOKEN_TYPE_REG_RETVAL;
		}
		else if (std::get<0>(ret)) lexer.CurrentToken = TOKEN_TYPE_INSTR;
		else if (IsStringIdent(lexer.CurrentLexeme)) {
			lexer.CurrentToken = TOKEN_TYPE_IDENT;
		}

	}
	return lexer.CurrentToken;
}

void Assembler::ResetLexer() {
	lexer.uiCurrentSourceLine = 0;
	lexer.iCurrentLexState = LEX_STATE_NO_STRING;
	lexer.ptrIndex0 = lexer.ptrIndex1 = codes[0].code.begin();
	lexer.CurrentToken = TOKEN_TYPE_INVALID;
}


/*
#define TOKEN_TYPE_INT              0           // An integer literal
#define TOKEN_TYPE_FLOAT            1           // An floating-point literal
#define TOKEN_TYPE_STRING           2           // An string literal
#define TOKEN_TYPE_QUOTE            3           // A double-quote
#define TOKEN_TYPE_IDENT            4           // An identifier
#define TOKEN_TYPE_COLON            5           // A colon
#define TOKEN_TYPE_OPEN_BRACKET     6           // An openening bracket
#define TOKEN_TYPE_CLOSE_BRACKET    7           // An closing bracket
#define TOKEN_TYPE_COMMA            8           // A comma
#define TOKEN_TYPE_OPEN_BRACE       9           // An openening curly brace
#define TOKEN_TYPE_CLOSE_BRACE      10          // An closing curly brace
#define TOKEN_TYPE_NEWLINE          11          // A newline

#define TOKEN_TYPE_INSTR			12			// An instruction

#define TOKEN_TYPE_SETSTACKSIZE     13          // The SetStackSize directive
#define TOKEN_TYPE_VAR              14          // The Var/Var [] directives
#define TOKEN_TYPE_FUNC             15          // The Func directives
#define TOKEN_TYPE_PARAM            16          // The Param directives
#define TOKEN_TYPE_REG_RETVAL       17          // The _RetVal directives

#define TOKEN_TYPE_INVALID          18          // Error code for invalid tokens
#define END_OF_TOKEN_STREAM         19          // The end of the stream has been reached
*/


void Assembler::Compile()
{
	//using std::cout;
	using std::endl;
	//using std::wcout;
	using std::wstring;
	typedef InstructionOutStream::Op Op;
	typedef InstrLookupTable::OpTypes OpTypes;
	ResetLexer();
	isSetStackSizeFound = false;
	isInAFunction = false;
	std::size_t instrStreamSize = 0;
	FunctionTable::FuncNode currentFunc;
	int currentParamIndex = 0;
	auto& functionTable = GetFunctionTable();
	auto& symbolTable = GetSymbolTable();
	auto& labelTable = GetLabelTable();
	auto& instrLookupTable = GetInstrLookupTable();
	auto& instructionOutStream = GetInstrctionOutStream();
	auto& stringTable = GetStringTable();
	while (GetNextToken() != END_OF_TOKEN_STREAM) {
		//wcout << GetTokenName(lexer.CurrentToken) << endl;
		switch (lexer.CurrentToken) {
		case TOKEN_TYPE_SETSTACKSIZE: {
			if (isInAFunction) {
				ExitOnCodeError(L"不能在函数内部声明栈大小");
			}
			if (isSetStackSizeFound) {
				ExitOnCodeError(L"重复声明了栈的大小");
			}
			if (GetNextToken() != TOKEN_TYPE_INT) {
				ExitOnCodeError(L"只能用整数声明栈的大小");
			}
			isSetStackSizeFound = true;
			scriptHeader.stackSize = StringToInt(lexer.CurrentLexeme);
			break; }
		case TOKEN_TYPE_FUNC: {// 现在 func 的问题在于 Func Test{ Param test 会错过 test的声明
			if (isInAFunction) {
				ExitOnCodeError(L"不能在函数的内部重定义函数");
			}
			if (GetNextToken() != TOKEN_TYPE_IDENT) {
				ExitOnCodeError(L"需提供函数名");
			}
			isInAFunction = true;
			currentFunc.wstrName = lexer.CurrentLexeme;
			currentFunc.iEntryPoint = instrStreamSize;
			currentFunc.iLocalDataSize = 0;
			currentFunc.iParamCount = 0;
			currentFunc.iIndex = functionTable.AddFunc(currentFunc.wstrName, instrStreamSize);
			if (currentFunc.iIndex == -1) {
				ExitOnCodeError(L"之前已有函数 " + currentFunc.wstrName + L" 的定义");
			}

			if (lexer.CurrentLexeme == MAIN_FUNCTION_NAME) {
				scriptHeader.isMainFuncPresent = true;
				scriptHeader.mainFuncIndex = currentFunc.iIndex;
			}
			while (GetNextToken() == TOKEN_TYPE_NEWLINE);
			if (lexer.CurrentToken != TOKEN_TYPE_OPEN_BRACE) {
				ExitOnCodeError(L"函数 " + currentFunc.wstrName + L" 需要{");
			}
			instrStreamSize++;
			break;
		}
		case TOKEN_TYPE_CLOSE_BRACE: {
			if (!isInAFunction) {
				ExitOnCodeError(L"}只应该在函数结束时使用");
			}
			functionTable.SetFuncInfo(currentFunc.wstrName, currentFunc.iParamCount, currentFunc.iLocalDataSize);
			isInAFunction = false;
			break;
		}
		case TOKEN_TYPE_VAR: {
			if (GetNextToken() != TOKEN_TYPE_IDENT) {
				ExitOnCodeError(L"Var声明的变量需要变量名");
			}
			std::wstring ident = lexer.CurrentLexeme;
			int size = 1;
			if (GetLookAheadChar() == L'[') {
				if (GetNextToken() != TOKEN_TYPE_OPEN_BRACKET) {
					ExitOnCodeError(L"需要[");
				}
				if (GetNextToken() != TOKEN_TYPE_INT) {
					ExitOnCodeError(L"数组的长度必须是整形");
					size = StringToInt(lexer.CurrentLexeme);
				}
				if (GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET) {
					ExitOnCodeError(L"需要]");
				}
			}
			int stackindex;
			if (isInAFunction) {
				stackindex = -(2 + currentFunc.iLocalDataSize);
			}
			else {
				stackindex = scriptHeader.globalDataSize;
			}
			if (symbolTable.AddSymbol(ident, size, stackindex, currentFunc.iIndex) == -1) {
				ExitOnCodeError(L"函数重定义错误");
			}
			if (isInAFunction) currentFunc.iLocalDataSize += size;
			else scriptHeader.globalDataSize += size;
			break;
		}
		case TOKEN_TYPE_PARAM: {
			if (!isInAFunction) {
				ExitOnCodeError(L"必须在函数中定义参数");
			}
			if (GetNextToken() != TOKEN_TYPE_IDENT) {
				ExitOnCodeError(L"声明参数后必须跟随声明的变量名");
			}
			if (GetNextToken() != TOKEN_TYPE_NEWLINE) {
				ExitOnCodeError(L"参数名后有无效的输入(参数不能为数组)");
			}
			currentFunc.iParamCount++;
			break;
		}
		case TOKEN_TYPE_IDENT: {
			wstring labelName = lexer.CurrentLexeme;
			if (GetNextToken() != TOKEN_TYPE_COLON) {
				ExitOnCodeError(labelName + L"必须为跳转标记");
			}
			if (!isInAFunction) {
				ExitOnCodeError(L"跳转标记只能出现在函数中");
			}

			int targetIndex = instrStreamSize - 1;//进入函数后，instrStreamSize +1 ，现在 减去这多加的1
			if (labelTable.AddLabel(labelName, targetIndex, currentFunc.iIndex) == -1) {
				ExitOnCodeError(L"不能在函数中出现重名跳转标记");
			}
			break;
		}
		case TOKEN_TYPE_INSTR: {
			if (!isInAFunction) {
				ExitOnCodeError(L"指令必须出现在函数中");
			}
			instrStreamSize++;
			while (GetNextToken() != TOKEN_TYPE_NEWLINE);
			break;
		}
		default:
			if (lexer.CurrentToken != TOKEN_TYPE_NEWLINE)
				ExitOnCodeError(L"无效的输入");
		}
	}
	instructionOutStream.ReSize(instrStreamSize);
	ResetLexer();
	isInAFunction = false;
	instrStreamSize = 0;
	while (GetNextToken() != END_OF_TOKEN_STREAM) {
		switch (lexer.CurrentToken)
		{
		case TOKEN_TYPE_FUNC: {
			GetNextToken();
			isInAFunction = true;
			currentParamIndex = 0;
			currentFunc = std::get<1>(functionTable.GetFunctionByName(lexer.CurrentLexeme));
			break; }
		case TOKEN_TYPE_CLOSE_BRACE: {
			isInAFunction = false;
			if (currentFunc.wstrName == MAIN_FUNCTION_NAME) {
				instructionOutStream[instrStreamSize].iOpcode = INSTR_EXIT;
				instructionOutStream[instrStreamSize].iCount = 1;
				Op temp;
				temp.iType = OP_TYPE_INT;
				temp.iIntLiteral = 0;
				instructionOutStream[instrStreamSize].OpList.push_back(temp);
			}
			else {
				instructionOutStream[instrStreamSize].iOpcode = INSTR_RET;
				instructionOutStream[instrStreamSize].iCount = 0;
			}
			instrStreamSize++;
			break;
		}
		case TOKEN_TYPE_PARAM: {
			GetNextToken();
			wstring name = lexer.CurrentLexeme;
			int stackIndex = -(currentFunc.iLocalDataSize + 2 + (currentParamIndex + 1));// +2 因为从-2 开始(-1保留) +1 因为参数在返回地址的下面
			if (symbolTable.AddSymbol(name, 1, stackIndex, currentFunc.iIndex) == -1) {
				ExitOnCodeError(L"不能在函数中出现重复的声明");
			}
			currentParamIndex++;
			break;
		}
		case TOKEN_TYPE_INSTR: {
			wstring name = lexer.CurrentLexeme;
			auto currentInstr = std::get<1>(instrLookupTable.GetInstrByMnemonic(name));
			instructionOutStream[instrStreamSize].iOpcode = currentInstr.iOpcode;
			instructionOutStream[instrStreamSize].iCount = currentInstr.iOpCount;
			for (int i = 0; i < currentInstr.iOpCount; i++) {
				OpTypes currentType = currentInstr.OpList[i];
				GetNextToken();
				switch (lexer.CurrentToken) {
				case TOKEN_TYPE_INT: {
					if (currentType&OP_FLAG_TYPE_INT) {
						Op temp;
						temp.iType = OP_TYPE_INT;
						temp.iIntLiteral = StringToInt(lexer.CurrentLexeme);
						instructionOutStream[instrStreamSize].OpList.push_back(temp);
					}
					else {
						ExitOnCodeError(L"无效的参数类型");
					}
					break;
				}
				case TOKEN_TYPE_FLOAT: {
					if (currentType&OP_FLAG_TYPE_FLOAT) {
						Op temp;
						temp.iType = OP_TYPE_FLOAT;
						temp.fFloatLiteral = StringToFloat(lexer.CurrentLexeme);
						instructionOutStream[instrStreamSize].OpList.push_back(temp);
					}
					else {
						ExitOnCodeError(L"无效的参数类型");
					}
					break;
				}
				case TOKEN_TYPE_QUOTE: {
					if (currentType&OP_FLAG_TYPE_STRING) {
						GetNextToken();
						if (lexer.CurrentToken == TOKEN_TYPE_STRING) {
							wstring str = lexer.CurrentLexeme;
							int index = stringTable.AddString(str);
							if (GetNextToken() != TOKEN_TYPE_QUOTE) {
								ExitOnCodeError(L"缺少\"");
							}
							Op temp;
							temp.iType = OP_TYPE_STRING_INDEX;
							temp.iStringTableIndex = index;
							instructionOutStream[instrStreamSize].OpList.push_back(temp);
						}
						else ExitOnCodeError(L"无效的参数类型");
					}
					else ExitOnCodeError(L"无效的参数类型");
					break;
				}
				case TOKEN_TYPE_REG_RETVAL: {
					if (currentType&OP_FLAG_TYPE_REG) {
						Op temp;
						temp.iType = OP_TYPE_REG;
						temp.IReg = 0;
						instructionOutStream[instrStreamSize].OpList.push_back(temp);
					}
					else ExitOnCodeError(L"无效的参数类型");
					break;
				}
				case TOKEN_TYPE_IDENT: {
					if (currentType&OP_FLAG_TYPE_MEM_REF) {// variable or array 
						wstring name = lexer.CurrentLexeme;
						auto temp = symbolTable.GetSymbolByIndent(name, currentFunc.iIndex);
						if (!std::get<0>(temp)) {
							ExitOnCodeError(L"引用了未声明的变量" + name);
						}
						int baseIndex = std::get<1>(temp).iStackIndex;
						if (GetLookAheadChar() != L'[') {
							if (std::get<1>(temp).iSize != 1) {
								ExitOnCodeError(L"数组" + name + L"需要下标");
							}
							Op temp;
							temp.iType = OP_TYPE_ABS_STACK_INDEX;
							temp.iStackIndex = baseIndex;
							instructionOutStream[instrStreamSize].OpList.push_back(temp);
						}
						else {
							if (std::get<1>(temp).iSize == 1) {
								ExitOnCodeError(L"变量" + name + L"不需要下标");
							}
							GetNextToken();
							GetNextToken();
							if (lexer.CurrentToken == TOKEN_TYPE_INT) {
								int offset = StringToInt(lexer.CurrentLexeme);
								if (offset >= std::get<1>(temp).iSize) {
									ExitOnCodeError(L"数组下标越界");
								}
								Op temp;
								temp.iType = OP_TYPE_ABS_STACK_INDEX;
								if (baseIndex >= 0)
									temp.iStackIndex = baseIndex + offset;
								else temp.iStackIndex = baseIndex - offset;// 如果时正数，全局变量，直接相加，否则时局部变量，需要向下走，即相减
								instructionOutStream[instrStreamSize].OpList.push_back(temp);
							}
							else if (lexer.CurrentToken == TOKEN_TYPE_IDENT) {
								wstring offsetName = lexer.CurrentLexeme;
								auto offsetTemp = symbolTable.GetSymbolByIndent(offsetName, currentFunc.iIndex);
								if (!std::get<0>(offsetTemp)) {
									ExitOnCodeError(L"引用了未声明的变量" + offsetName);
								}
								if (std::get<1>(offsetTemp).iSize > 1) {
									ExitOnCodeError(L"此版本中,作为下标的变量不能未数组");
								}
								Op temp;
								temp.iType = OP_TYPE_REL_STACK_INDEX;
								temp.iStackIndex = baseIndex;
								temp.iOffsetIndex = std::get<1>(offsetTemp).iStackIndex;
								instructionOutStream[instrStreamSize].OpList.push_back(temp);
							}
							else {
								ExitOnCodeError(L"无效的数组序号");
							}
							if (GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET) {
								ExitOnCodeError(L"需要]");
							}
						}

					}
					else if (currentType&OP_FLAG_TYPE_LINE_LABEL) {
						wstring name = lexer.CurrentLexeme;
						auto label = labelTable.GetLabelByIdent(name, currentFunc.iIndex);
						if (!std::get<0>(label)) {
							ExitOnCodeError(L"无效的跳转标记");
						}
						Op temp;
						temp.iType = OP_TYPE_INSTR_INDEX;
						temp.iInstrIndex = std::get<1>(label).iTargetIndex;
						instructionOutStream[instrStreamSize].OpList.push_back(temp);
					}
					/*else if (currentType&OP_FLAG_TYPE_HOST_API_CALL) {

					}*/
					else if (currentType&OP_FLAG_TYPE_FUNC_NAME) {
						wstring name = lexer.CurrentLexeme;
						auto func = functionTable.GetFunctionByName(name);
						if (!std::get<0>(func)) {
							ExitOnCodeError(L"未声明函数" + name);
							Op temp;
							temp.iType = OP_TYPE_FUNC_INDEX;
							temp.iFuncIndex = std::get<1>(func).iIndex;
							instructionOutStream[instrStreamSize].OpList.push_back(temp);
						}
					}

					break;
				}
				default: {
					ExitOnCodeError(L"无效的参数类型");
				}
				}
				if (i < currentInstr.iOpCount - 1) {
					if (GetNextToken() != TOKEN_TYPE_COMMA) {
						ExitOnCodeError(L"需要,");
					}
				}
			}
			if (GetNextToken() != TOKEN_TYPE_NEWLINE)
				ExitOnCodeError(L"无效的输入");
			instrStreamSize++;
			break;
		}//	case TOKEN_TYPE_INSTR
		}//switch
		if (!SkipToNextLine())
			break;
	}

}

void Assembler::BuildXSE(std::wstring name) {
	using std::setw;
	using std::wcout;
	auto& functionTable = GetFunctionTable();
	auto& symbolTable = GetSymbolTable();
	auto& labelTable = GetLabelTable();
	auto& instrLookupTable = GetInstrLookupTable();
	auto& instructionOutStream = GetInstrctionOutStream();
	auto& stringTable = GetStringTable();
	name = name + L".xse";
	std::wofstream output(name, std::ios_base::binary | std::ios_base::out);
	output.imbue(std::locale(std::locale("C"),
		new std::codecvt_utf16<wchar_t, 0x10ffff, std::generate_header>));
	output << setw(IDENTIFIER_WIDTH) << XSE_ID_STRING;
	output << setw(VERSION_WIDTH) << VERSION_MINOR;
	output << setw(VERSION_WIDTH) << VERSION_MAJOR;
	output << setw(sizeof(std::size_t)) << scriptHeader.stackSize;
	output << setw(sizeof(std::size_t)) << scriptHeader.globalDataSize;
	output << setw(sizeof(bool)) << scriptHeader.isMainFuncPresent;
	output << setw(sizeof(std::size_t)) << scriptHeader.mainFuncIndex;
	output << setw(sizeof(std::size_t)) << instructionOutStream.size();
	for (auto x : instructionOutStream) {
		output << setw(sizeof(int)) << x.iOpcode;
		output << setw(sizeof(int)) << x.iCount;
		for (auto y : x.OpList) {
			output << setw(sizeof(int)) << y.iType;
			switch (y.iType) {
			case OP_TYPE_INT:
				output << setw(sizeof(int)) << y.iIntLiteral;
				break;
			case OP_TYPE_FLOAT:
				output << setw(sizeof(float)) << y.fFloatLiteral;
				break;
			case OP_TYPE_STRING_INDEX:
				output << setw(sizeof(int)) << y.iStringTableIndex;
				break;
			case OP_TYPE_REG:
				output << setw(sizeof(int)) << y.IReg;
				break;
			case OP_TYPE_ABS_STACK_INDEX:
				output << setw(sizeof(int)) << y.iStackIndex;
				break;
			case OP_TYPE_REL_STACK_INDEX:
				output << setw(sizeof(int)) << y.iStackIndex;
				output << setw(sizeof(int)) << y.iOffsetIndex;
				break;
			case OP_TYPE_INSTR_INDEX:
				output << setw(sizeof(int)) << y.iInstrIndex;
				break;
			case OP_TYPE_FUNC_INDEX:
				output << setw(sizeof(int)) << y.iFuncIndex;
				break;
			default:
				output << setw(sizeof(int)) << y.iInstrIndex;
				wcout << "Caution:" << std::endl;
				break;
			}

		}
	}
	output << setw(sizeof(std::size_t)) << functionTable.size();
	for (auto x : functionTable) {
		output << setw(sizeof(int)) << x.iEntryPoint;
		output << setw(sizeof(int)) << x.iLocalDataSize;
		output << setw(sizeof(int)) << x.iParamCount;
	}

	output << setw(sizeof(std::size_t)) << stringTable.size();
	for (auto x : stringTable) {
		output << setw(sizeof(std::size_t)) << x.length();
		output << x;
	}
	
	output.close();

}


bool Assembler::IsCharWhitespace(wint_t ch) {
	if (ch == L' ' || ch == L'\t') return true;
	return false;
}

bool Assembler::IsCharDelimiter(wint_t ch) {
	if (ch == L':' || ch == L'"' || ch == L',' ||
		ch == L'[' || ch == L']' ||
		ch == L'{' || ch == L'}' ||
		IsCharWhitespace(ch))
		return true;
	return false;
}
bool Assembler::IsStringFloat(std::wstring str) {
	std::wistringstream iss(str);
	float f;
	iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
							   // Check the entire string was consumed and if either failbit or badbit is set
	return iss.eof() && !iss.fail();

}
bool Assembler::IsStringInt(std::wstring str) {
	std::wistringstream iss(str);
	int f;
	iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
							   // Check the entire string was consumed and if either failbit or badbit is set
	return iss.eof() && !iss.fail();
}

inline int Assembler::StringToInt(std::wstring str)
{
	std::wistringstream iss(str);
	int f;
	iss >> std::noskipws >> f;
	return f;
}

inline float Assembler::StringToFloat(std::wstring str)
{
	std::wistringstream iss(str);
	float f;
	iss >> std::noskipws >> f;
	return f;
}

bool Assembler::IsStringIdent(std::wstring str) {
	if (str.length() == 0) return false;
	std::wstring temp;
	std::copy_if(str.begin(), str.end(), std::back_inserter(temp), [](wint_t ch) {
		return (ch == L'_' || (ch >= L'a'&&ch <= L'z') ||
			(ch >= L'A'&&ch <= L'Z') || (ch >= L'0'&&ch <= L'9')); });
	return temp.length() == str.length();
}


FunctionTable& Assembler::GetFunctionTable() {
	return FunctionTable::Instance();
}
InstrLookupTable& Assembler::GetInstrLookupTable() {
	return InstrLookupTable::Instance();
}
LabelTable& Assembler::GetLabelTable() {
	return LabelTable::Instance();
}
SymbolTable& Assembler::GetSymbolTable() {
	return SymbolTable::Instance();
}
HostCallTable& Assembler::GetHostCallTable() {
	return HostCallTable::Instance();
}
StringTable& Assembler::GetStringTable() {
	return StringTable::Instance();
}

inline InstructionOutStream & Assembler::GetInstrctionOutStream()
{
	return InstructionOutStream::Instance();
}


void Assembler::ExitOnError(std::wstring errormsg) {
	std::wcout << errormsg << std::endl;
	exit(-1);
}

std::wstring Assembler::GetTokenName(Token t) {
	switch (t)
	{
	case TOKEN_TYPE_INT:
		return L"TOKEN_TYPE_INT";
	case TOKEN_TYPE_FLOAT:
		return L"TOKEN_TYPE_FLOAT";
	case TOKEN_TYPE_STRING:
		return L"TOKEN_TYPE_STRING";
	case TOKEN_TYPE_QUOTE:
		return L"TOKEN_TYPE_QUOTE";
	case TOKEN_TYPE_IDENT:
		return L"TOKEN_TYPE_IDENT";
	case TOKEN_TYPE_COLON:
		return L"TOKEN_TYPE_COLON";
	case TOKEN_TYPE_OPEN_BRACKET:
		return L"TOKEN_TYPE_OPEN_BRACKET";
	case TOKEN_TYPE_CLOSE_BRACKET:
		return L"TOKEN_TYPE_CLOSE_BRACKET";
	case TOKEN_TYPE_COMMA:
		return L"TOKEN_TYPE_COMMA";
	case TOKEN_TYPE_OPEN_BRACE:
		return L"TOKEN_TYPE_OPEN_BRACE";
	case TOKEN_TYPE_CLOSE_BRACE:
		return L"TOKEN_TYPE_CLOSE_BRACE";
	case TOKEN_TYPE_NEWLINE:
		return L"TOKEN_TYPE_NEWLINE";
	case TOKEN_TYPE_INSTR:
		return L"TOKEN_TYPE_INSTR";
	case TOKEN_TYPE_SETSTACKSIZE:
		return L"TOKEN_TYPE_SETSTACKSIZE";
	case TOKEN_TYPE_VAR:
		return L"TOKEN_TYPE_VAR";
	case TOKEN_TYPE_FUNC:
		return L"TOKEN_TYPE_FUNC";
	case TOKEN_TYPE_PARAM:
		return L"TOKEN_TYPE_PARAM";
	case TOKEN_TYPE_REG_RETVAL:
		return L"TOKEN_TYPE_REG_RETVAL";
	case TOKEN_TYPE_INVALID:
		return L"TOKEN_TYPE_INVALID";
	case END_OF_TOKEN_STREAM:
		return L"END_OF_TOKEN_STREAM";
	}
	return L"Strange Token";
}

wchar_t Assembler::GetLookAheadChar() {
	using std::cout;
	using std::endl;
	std::size_t currentCodeLine = lexer.uiCurrentSourceLine;
	std::wstring::iterator index = lexer.ptrIndex1;
	if (lexer.iCurrentLexState != LEX_STATE_IN_STRING) {
		index = std::find_if(index, codes[currentCodeLine].code.end(), [](wint_t ch) {return !std::iswspace(ch); });
		if (index == codes[currentCodeLine].code.end()) {
			currentCodeLine++;
			if (currentCodeLine >= codes.size()) return 0;
			index = codes[currentCodeLine].code.begin();
		}
	}
	return *index;
}


