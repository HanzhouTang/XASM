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
			int iInstrIndex;
			int iFuncIndex;
			int iHostAPICallIndex;
			int IReg;
		};
		int iOffsetIndex; // needed for array
	};
	struct Instr {
		int iOpcode;
		int iCount;
		std::vector<Op> OpList;
	};
	static inline InstructionOutStream& Instance() {
		static InstructionOutStream stream;
		return stream;
	}
	std::vector<Instr>::iterator begin() {
		return InstrStream.begin();
	}
	std::vector<Instr>::iterator end() {
		return InstrStream.end();
	}
	void ReSize(std::size_t size) {
		InstrStream.resize(size);
	}
	std::size_t size() {
		return InstrStream.size();
	}
	Instr& operator[](std::size_t size) {
		return InstrStream[size];
	}
private:
	static std::vector<Instr> InstrStream;
};