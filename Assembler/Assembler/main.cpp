#include"Assembler.h"
int main(int argc, char*argv[]) {
	Assembler& assembler = Assembler::Instance();
	std::wstring name;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	if (argc == 1) assembler.ExitOnError(L"must contain file name"); {
		name = converter.from_bytes(std::string(argv[1]));
	} 
	std::wregex xasm(std::wstring(XASM_FILE_EXTENSION) + L"$");
	if (!std::regex_search(name,xasm)) {
		name += XASM_FILE_EXTENSION;
	}
	assembler.LoadSourceCode(name);
	assembler.Compile();
	name=std::regex_replace(name, xasm, XSE_FILE_EXTENSION);
	assembler.BuildXSE(name);
	assembler.Statistic(name);
}



