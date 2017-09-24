#pragma once
#include<vector>
#include"constant.h"
class InstructionOutStream {
public:
	struct Op {
		int iType;
		union {
			int iIntLiteral;
			float fFloatLiteral;
			int iStringTableIndex;
			int iStackIndex;
			int iFuncIndex;
			int iHostAPICallIndex;
			int IReg;
		};
		int iOffsetIndex; // needed for array
	};
	struct Instr {
		int iOpcode;
		int iCount;
		Op* pOpList;
	};
private:
	static std::vector<Instr> InstrStream;
};