#include"Assembler.h"
std::vector<std::wstring> Assembler::codes = {};
void Assembler::LoadSourceCode(std::wstring name) {
	std::wifstream read(name);
	if (!read.is_open()) {
		ExitOnError(L"Could not open file #" + name);
	}
	wchar_t str[256];
	while (!read.eof()) {
		read.getline(str, 256);
		std::wstring temp(str);
		StripComment(temp);
		Trim(temp);
		if (!temp.empty())
			codes.push_back(temp + L"\n");
	}
	read.close();
}

void Assembler::OutPut(std::wostream& stream) {
	for (std::size_t i = 0; i < codes.size(); i++) {
		stream << i << "\t" << codes[i];
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
	if (lexer.ptrIndex0 == codes[lexer.uiCurrentSourceLine].end()) {
		lexer.iCurrentLexState = LEX_STATE_NO_STRING;
		lexer.uiCurrentSourceLine++;
		if (lexer.uiCurrentSourceLine == codes.size()) {
			lexer.CurrentToken = END_OF_TOKEN_STREAM;
			return lexer.CurrentToken;
		}
		lexer.ptrIndex0 = lexer.ptrIndex1 = codes[lexer.uiCurrentSourceLine].begin();
	}
	auto ptrEnd = codes[lexer.uiCurrentSourceLine].end();
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
	lexer.ptrIndex0 = lexer.ptrIndex1 = codes[0].begin();
	lexer.CurrentToken = TOKEN_TYPE_INVALID;
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
