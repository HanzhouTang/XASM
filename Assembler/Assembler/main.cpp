#include"Assembler.h"
int main(int argc, char*argv[]) {
	Assembler& assembler = Assembler::Instance();
	//if (argc == 1) assembler.ExitOnError(L"must contain file name");
	//std::string tempstr = std::string(argv[1]);

	
	std::string tempstr = std::string("test_4.xasm");
	std::wstring tempwstr = std::wstring(tempstr.begin(), tempstr.end());
	assembler.LoadSourceCode(tempwstr);
	assembler.Compile();
	assembler.BuildXSE(L"test");
	assembler.Statistic(L"test");
	system("pause");
}



