#pragma once
// ---- Instruction Opcodes -----------------------------------------------------------

#define INSTR_MOV               0

#define INSTR_ADD               1
#define INSTR_SUB               2
#define INSTR_MUL               3
#define INSTR_DIV               4
#define INSTR_MOD               5
#define INSTR_EXP               6
#define INSTR_NEG               7
#define INSTR_INC               8
#define INSTR_DEC               9

#define INSTR_AND               10
#define INSTR_OR                11
#define INSTR_XOR               12
#define INSTR_NOT               13
#define INSTR_SHL               14
#define INSTR_SHR               15

#define INSTR_CONCAT            16
#define INSTR_GETCHAR           17
#define INSTR_SETCHAR           18

#define INSTR_JMP               19
#define INSTR_JE                20
#define INSTR_JNE               21
#define INSTR_JG                22
#define INSTR_JL                23
#define INSTR_JGE               24
#define INSTR_JLE               25

#define INSTR_PUSH              26
#define INSTR_POP               27

#define INSTR_CALL              28
#define INSTR_RET               29
#define INSTR_CALLHOST          30

#define INSTR_PAUSE             31
#define INSTR_EXIT              32

// ---- Operand Type Bitfield Flags ---------------------------------------------------
#define OP_FLAG_TYPE_INT           1           //int
#define OP_FLAG_TYPE_FLOAT         2           //float
#define OP_FLAG_TYPE_STRING        4           //string
#define OP_FLAG_TYPE_MEM_REF       8           //variable or array
#define OP_FLAG_TYPE_LINE_LABEL    16          //line label 
#define OP_FLAG_TYPE_FUNC_NAME     32          //function name
#define OP_FLAG_TYPE_HOST_API_CALL 64          //host api call
#define OP_FLAG_TYPE_REG           128         //register
// ---- Assembled Instruction Stream ------------------------------------------------------

#define OP_TYPE_INT                 0           // Integer literal value
#define OP_TYPE_FLOAT               1           // Floating-point literal value
#define OP_TYPE_STRING_INDEX        2           // String literal value
#define OP_TYPE_ABS_STACK_INDEX     3           // Absolute array index
#define OP_TYPE_REL_STACK_INDEX     4           // Relative array index
#define OP_TYPE_INSTR_INDEX         5           // Instruction index
#define OP_TYPE_FUNC_INDEX          6           // Function index
#define OP_TYPE_HOST_API_CALL_INDEX 7           // Host API call index
#define OP_TYPE_REG                 8           // Register

// ---- Lexer -----------------------------------------------------------------------------


#define LEX_STATE_NO_STRING         0           // Lexemes are scanned as normal
#define LEX_STATE_IN_STRING         1           // Lexemes are scanned as strings
#define LEX_STATE_END_STRING        2           // Lexemes are scanned as normal, and thenext state is LEXEME_STATE_NOSTRING

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

// -----------------------------MAIN FUNCTION -------------------------
#define MAIN_FUNCTION_NAME          L"main"

// ---- ------------------------.XSE Header ---------------------------
#define XSE_ID_STRING               L"XSE0"      // Written to the file to state it's validity
#define VERSION_MAJOR               L"0"           // Major version number
#define VERSION_MINOR               L"4"           // Minor version number

// ------------------------------Output Width---------------------------
#define IDENTIFIER_WIDTH               4
#define VERSION_WIDTH                  1
#define STACK_SIZE_WIDTH               5
//-------------------------------File Extension-------------------------
#define XSE_FILE_EXTENSION             L".xse"
#define XASM_FILE_EXTENSION             L".xasm"
