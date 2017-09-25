#include"Assembler.h"
std::vector<std::wstring> Assembler::codes = {};
void Assembler::LoadSourceCode(std::wstring name) {
	std::wifstream read(name);
	if (!read.is_open()) {
		ExitOnError(L"Could not open file #" + name);
	}
	wchar_t str[256];
	while (!read.eof()) {
		read.getline(str,256);
		std::wstring temp(str);
		IgnoreComment(temp);
		Trim(temp);
		if (!temp.empty())
			codes.push_back(temp+L"\n");
	}
	read.close();
}

void Assembler::OutPut(std::wostream& stream) {
	for (std::size_t i = 0; i < codes.size(); i++) {
		stream << i<<"\t" << codes[i];
	}
}

std::wstring& Assembler::IgnoreComment(std::wstring& code) {
	bool instring = false;
	std::wstring::iterator ptr;
	for ( ptr = code.begin(); ptr != code.end(); ptr++) {
		if (*ptr == L'"') instring = !instring;
		if (*ptr == L';' && !instring) break;
	}
	code.erase(ptr, code.end());
	return code;
}

std::wstring& Assembler::LeftTrim(std::wstring& code) {
	code.erase(code.begin(), std::find_if(code.begin(),code.end(),
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
















//avoid using windows API