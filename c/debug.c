#include <stdio.h>

#include "debug.h"

void printValue(Value value) {
  if (value == NULL) {
    printf("null");
    return;
  }
  
  switch (value->type) {
    case OBJ_BOOL:
      printf(((ObjBool*)value)->value ? "true" : "false");
      break;
      
    case OBJ_FUNCTION:
      printf("<fn %p>", value);
      break;
      
    case OBJ_NATIVE:
      printf("<native %p>", ((ObjNative*)value)->function);
      break;
      
    case OBJ_NUMBER:
      printf("%g", ((ObjNumber*)value)->value);
      break;
      
    case OBJ_STRING:
      printf("%.*s", ((ObjString*)value)->length, ((ObjString*)value)->chars);
      break;
      
    case OBJ_TABLE:
      printf("table");
      break;
  }
}

void printStack() {
  for (int i = 0; i < vm.frame->stackSize; i++) {
    printf("%d: ", i);
    printValue(vm.frame->stack[i]);
    printf("\n");
  }
}

int printInstruction(ObjFunction* function, int i) {
  printf("%04d ", i);
  switch (function->code[i++]) {
    case OP_CONSTANT: {
      uint8_t constant = function->code[i++];
      printf("%-16s %4d '", "OP_CONSTANT", constant);
      printValue(function->constants.values[constant]);
      printf("'\n");
      break;
    }
      
    case OP_NULL: printf("OP_NULL\n"); break;
    case OP_POP: printf("OP_POP\n"); break;
      
    case OP_GET_LOCAL: {
      uint8_t slot = function->code[i++];
      printf("%-16s %4d\n", "OP_GET_LOCAL", slot);
      break;
    }
      
    case OP_SET_LOCAL: {
      uint8_t slot = function->code[i++];
      printf("%-16s %4d\n", "OP_SET_LOCAL", slot);
      break;
    }

    case OP_GET_GLOBAL: {
      uint8_t name = function->code[i++];
      printf("%-16s %4d '", "OP_GET_GLOBAL", name);
      printValue(function->constants.values[name]);
      printf("'\n");
      break;
    }
      
    case OP_DEFINE_GLOBAL: {
      uint8_t name = function->code[i++];
      printf("%-16s %4d '", "OP_DEFINE_GLOBAL", name);
      printValue(function->constants.values[name]);
      printf("'\n");
      break;
    }
      
    case OP_SET_GLOBAL: {
      uint8_t name = function->code[i++];
      printf("%-16s %4d '", "OP_SET_GLOBAL", name);
      printValue(function->constants.values[name]);
      printf("'\n");
      break;
    }
      
    case OP_EQUAL: printf("OP_EQUAL\n"); break;
    case OP_GREATER: printf("OP_GREATER\n"); break;
    case OP_LESS: printf("OP_LESS\n"); break;
    case OP_ADD: printf("OP_ADD\n"); break;
    case OP_SUBTRACT: printf("OP_SUBTRACT\n"); break;
    case OP_MULTIPLY: printf("OP_MULTIPLY\n"); break;
    case OP_NOT: printf("OP_NOT\n"); break;
    case OP_DIVIDE: printf("OP_DIVIDE\n"); break;
    case OP_NEGATE: printf("OP_NEGATE\n"); break;
    case OP_RETURN: printf("OP_RETURN\n"); break;
      
    case OP_JUMP: {
      uint16_t offset = (uint16_t)(function->code[i++] << 8);
      offset |= function->code[i++];
      printf("%-16s %4d -> %d\n", "OP_JUMP", offset, i + offset);
      break;
    }
      
    case OP_JUMP_IF_FALSE: {
      uint16_t offset = (uint16_t)(function->code[i++] << 8);
      offset |= function->code[i++];
      printf("%-16s %4d -> %d\n", "OP_JUMP_IF_FALSE", offset, i + offset);
      break;
    }
      
    case OP_LOOP: {
      uint16_t offset = (uint16_t)(function->code[i++] << 8);
      offset |= function->code[i++];
      printf("%-16s %4d -> %d\n", "OP_LOOP", offset, i - offset);
      break;
    }
      
    case OP_CALL_0: printf("OP_CALL_0\n"); break;
    case OP_CALL_1: printf("OP_CALL_1\n"); break;
    case OP_CALL_2: printf("OP_CALL_2\n"); break;
    case OP_CALL_3: printf("OP_CALL_3\n"); break;
    case OP_CALL_4: printf("OP_CALL_4\n"); break;
    case OP_CALL_5: printf("OP_CALL_5\n"); break;
    case OP_CALL_6: printf("OP_CALL_6\n"); break;
    case OP_CALL_7: printf("OP_CALL_7\n"); break;
    case OP_CALL_8: printf("OP_CALL_8\n"); break;
  }
  
  return i;
}

void printFunction(ObjFunction* function) {
  for (int i = 0; i < function->codeCount;) {
    i = printInstruction(function, i);
  }
}