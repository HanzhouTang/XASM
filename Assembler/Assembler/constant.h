#pragma once
#define OP_FLAG_TYPE_INT 1//int
#define OP_FLAG_TYPE_FLOAT 2 //float
#define OP_FLAG_TYPE_STRING 4 //string
#define OP_FLAG_TYPE_MEM_REF 8 //variable or array
#define OP_FLAG_TYPE_LINE_LABEL 16 //line label 
#define OP_FLAG_TYPE_FUNC_NAME 32 //function name
#define OP_FLAG_TYPE_HOST_API_CALL 64 //host api call
#define OP_FLAG_TYPE_REG 128 //register