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
int main() {
	FunctionTable& funcTable = FunctionTable::Instance();
	InstrLookupTable& instrLookTable = InstrLookupTable::Instance();
    LabelTable& labelTable = LabelTable::Instance();
	SymbolTable& symbolTable = SymbolTable::Instance();
	auto temp = instrLookTable.GetInstrByMnemonic(L"Call");
	if (std::get<0>(temp))
	{
		auto t = std::get<1>(temp);
		std::wcout << t.wstrMnemonic <<"\t"<< t.iOpcode << std::endl;
	
	}
	system("pause");
}



