/*
* vm.h
*
* Copyright 2004-2018 Enjector Software, Ltd.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#include <enjector/core/list.h>
#include <enjector/core/map.h>

typedef enum vm_opcode_t {
    opADD,
    opMUL,
    opDIV,
    opSUB,
    opNEG,
    opGETVAR,
    opSETVAR,
    opGETOBJVAR,
    opSETOBJVAR,
    opPOP,
    opPUSH,
    opCALL,
    opCALLOBJMETHOD,
    opRET,
    opIFCONDITION,
    opCONDITION,
    opJUMP,
    opLABEL,
    opDOEVENTS,
    opXPATH,
    opJSON
} vm_opcode;

typedef struct vm_instruction_t {
    vm_opcode	opcode;
    char*		param1;
    char*		param2;
    char*		param3;
    char*		param4;
} vm_instruction;

typedef struct vm_function_t {
    char* name;
    list* instructions;
} vm_function;

typedef struct vm_t {
    map* functions;
} vm;

vm*				vm_create();
void			vm_free(vm* vm);

vm_function*	vm_function_create(vm* v, char* name);
void			vm_function_emit(vm_function* fn, vm_opcode opcode, char* param1, char* param2, char* param3, char* param4);
void			vm_function_end(vm_function* fn);
