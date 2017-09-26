#include"Assembler.h"
int main(int argc, char*argv[]) {
	Assembler& assembler = Assembler::Instance();
	//if (argc == 1) assembler.ExitOnError(L"must contain file name");
	//std::string tempstr = std::string(argv[1]);
	std::string tempstr = std::string("test_3.xasm");
	std::wstring tempwstr = std::wstring(tempstr.begin(), tempstr.end());
	assembler.LoadSourceCode(tempwstr);
	Assembler::Token t;
	assembler.ResetLexer();
	t = assembler.GetNextToken();
	while (t != END_OF_TOKEN_STREAM) {
		std::wcout << assembler.GetTokenName(t) << std::endl;
		if (t == TOKEN_TYPE_NEWLINE) {
			std::wcout << std::endl;
		}
		t = assembler.GetNextToken();
	}
	system("pause");
}



