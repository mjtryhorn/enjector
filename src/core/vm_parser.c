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

#include <enjector/core/xmemory.h>
#include <enjector/core/text.h>

#include <enjector/core/vm_parser.h>
#include "vm_parser.tab.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

extern VM_PARSER_STYPE vm_parser_lval;
size_t  vm_parser_state_input_lineno;

extern int vm_parser_parse();
extern void vm_parser_set_in(FILE* in_str);

script* _vm_parser_ctx_current_script = 0;

script* vm_parser_create() {
    script* s = xmemory_new(script);

    vm_parser_clear(s);

    return s;
}

bool vm_parser_load(script* script, const char* text) {
    assert(script);
    assert(text);

    vm_parser_set_in(0);
    vm_parser_clear(script);

    script->vm = vm_create();
    script->vm_parser_state_input = text;	// TODO: copy?
    script->vm_parser_state_input_index = 0;
    script->vm_parser_state_input_len = text_length(text);
    script->vm_parser_state_input_lineno = 1;
    script->vm_parser_state_labels = queue_create();
    script->vm_parser_state_vars = list_create();

    _vm_parser_ctx_current_script = script;

    int result = vm_parser_parse();

    switch (result) {
    case 0:
        // The value returned by yyparse is 0 if parsing was successful (return is due to end-of-input).
        return true;

    case 1:

    // The value is 1 if parsing failed because of invalid input, i.e., input that contains a syntax error or that causes YYABORT to be invoked.

    case 2:

    // The value is 2 if parsing failed due to memory exhaustion.
    default:
        break;
    }

    return false;
}

void vm_parser_clear(script* script) {
    int i;
    assert(script);

    if (script->vm) {
        vm_free(script->vm);
    }

    if (script->vm_parser_state_labels) {
        int label_count = queue_count(script->vm_parser_state_labels);

        while (label_count-- > 0) {
            void* item = queue_pop(script->vm_parser_state_labels);

            if (item) {
                xmemory_free(item);
            }
        }

        queue_free(script->vm_parser_state_labels);
    }

    if (script->vm_parser_state_vars) {
        list_item** items = list_enumerable(script->vm_parser_state_vars);

        for (i = 0; i < list_count(script->vm_parser_state_vars); i++) {
            list_item* item = items[i];

            if (item->value) {
                xmemory_free(item->value);
            }
        }

        list_free(script->vm_parser_state_vars);
    }

    script->vm = NULL;
    script->vm_parser_state_input = NULL;
    script->vm_parser_state_input_index = 0;
    script->vm_parser_state_input_len = 0;
    script->vm_parser_state_input_lineno = 0;
    script->vm_parser_state_labels = NULL;
}

void vm_parser_dispose(script* script) {
    assert(script);

    vm_parser_clear(script);

    xmemory_free(script);
}

int vm_parser_parse_state_char_next() {
    script* script = _vm_parser_ctx_current_script;
    assert(script);

    int c = 0;

    if (script->vm_parser_state_input_index < script->vm_parser_state_input_len) {
        c = (int)script->vm_parser_state_input[script->vm_parser_state_input_index++];
    }

    return c;
}

#define ERROR_BUFFER_SIZE 120

void vm_parser_error(char* error) {
    // TODO: show the line
    script* script = _vm_parser_ctx_current_script;
    assert(script);

    // Show the offending line
    // - How much to show?
    const char* text = script->vm_parser_state_input;
    size_t text_len = script->vm_parser_state_input_len;
    size_t error_pos = script->vm_parser_state_input_index;
    size_t error_start = error_pos;
    size_t error_end = error_pos;

    // Got back to a newline or start of the buffer
    char c;

    while(c = text[error_start--], error_start != 0 && c != '\n') {
    }

    // Got to end of newline or end of the buffer
    while(c = text[error_end++], error_end < text_len && c != '\n') {
    }

    // Limit the error line to the following
    char buffer[ERROR_BUFFER_SIZE + 1];
    size_t error_len = error_end - error_start;

    if (error_len > ERROR_BUFFER_SIZE) {
        // Shrink both sides
        size_t buffer_overflow_len = error_len - ERROR_BUFFER_SIZE;
        size_t adjustment = buffer_overflow_len / 2;
        error_start	+= adjustment;
        error_end -= adjustment;
        error_len = ERROR_BUFFER_SIZE;
    }

    text_copy_length(buffer, &text[error_start], error_len);
    buffer[error_len] = 0;

    // Now show the offending line
    fprintf(stderr, "\n>>> script: %s @ %ld:%ld\n    %s\n", error, (unsigned long)script->vm_parser_state_input_lineno, script->vm_parser_state_input_index, buffer);

    //	fprintf(stderr, "     %s ...\n", buffer);

    // Now show the position where the error occurred
    size_t error_pos_virtual = error_pos - error_start;
    size_t left_spaces = error_pos_virtual;

    while(left_spaces-->0) {
        buffer[left_spaces] = ' ';
    }

    buffer[error_pos_virtual] = '^';
    buffer[error_pos_virtual + 1] = 0;
    fprintf(stderr, "   %s\n", buffer);

}

int vm_parser_wrap() {
    return 1;
}
//
// [==--- Parsed data processing -------------------------------------------==]
//

//
//// Pointer to the function that is being created
//Function *pFunction;
//
//// Temporary store for the byte code generated. This list will be transformed
//// into an array of byte code that will be stored in the Function class
//List<ScriptByteCode*> byteCodes;

// Signal: Create a new function in the Script class
vm_function* vm_parser_function_create(char* name) {
    script* script = _vm_parser_ctx_current_script;
    assert(script);
    assert(name);

    vm_function* function = vm_function_create(script->vm, name);
//	function->name = name; // name is already text cloned
//	function->instructions = list_create();

//	map_set(script->vm->functions, name, function);

    script->vm_parser_state_current_function = function;

    return function;
}

// Signal: End of the newly created function - this will now transfer the byte code
// to the Function class.
// Phase 1 - was the syntax directed code transformation
// Phase 2 - creates a label index
void vm_parser_function_end(vm_function* function) {
    //	// Phase 1 - already completed at this point
    //
    //	// Phase 2 - Build a list of label indexes
    //	List<int> labelAddresses;
    //	int address = 0;
    //	if (byteCodes.moveToHead())
    //	{
    //		do
    //		{
    //			ScriptByteCode *pByteCode = byteCodes.get();
    //			if (pByteCode->opCode == opLABEL)
    //			{
    //				// Store the label address
    //				labelAddresses.add(address, pByteCode->param1);
    //
    //				// Remove this label as an instruction
    //				byteCodes.remove();
    //			}
    //			else
    //			{
    //				address++;
    //			}
    //		}
    //		while(byteCodes.moveToNext());
    //	}
    //
    //	// Phase 3 - replace instructions which refer to labels by the actual addresses
    //	int iByteCodesLength = byteCodes.count();
    //	ScriptByteCode **pByteCodes = new ScriptByteCode*[iByteCodesLength];
    //
    //	int index = 0;
    //	char bufAddress[5];
    //	if (byteCodes.moveToHead())
    //	{
    //		do
    //		{
    //			ScriptByteCode *pByteCode = byteCodes.get();
    //
    //			switch(pByteCode->opCode)
    //			{
    //				case opIFCONDITION:
    //
    //					xitoa(labelAddresses[pByteCode->param2], bufAddress);
    //					if (pByteCode->param2) free(pByteCode->param2);
    //					pByteCode->param2 = String_duplicate(bufAddress);
    //					//                        pByteCode->setParam2(bufAddress);
    //					break;
    //
    //				case opJUMP:
    //
    //					xitoa(labelAddresses[pByteCode->param1], bufAddress);
    //					if (pByteCode->param1) free(pByteCode->param1);
    //					pByteCode->param1 = String_duplicate(bufAddress);
    //					///                     pByteCode->setParam1(bufAddress);
    //					break;
    //			}
    //
    //			pByteCodes[index++]  = pByteCode;
    //		}
    //		while(byteCodes.remove());
    //	}
    //
    //	pFunction->setByteCodes(pByteCodes, iByteCodesLength);
}

// Add a byte code to the function
void vm_parser_function_emit(vm_opcode opcode, char* param1, char* param2, char* param3, char* param4) {
    script* script = _vm_parser_ctx_current_script;
    vm_function* function = script->vm_parser_state_current_function;
    assert(function);

    if (param1) xmemory_free(param1);

    if (param2) xmemory_free(param2);

    if (param3) xmemory_free(param3);

    if (param4) xmemory_free(param4);

    //	ScriptByteCode *pByteCode = ScriptVM_bytecode_create(opCode, param1, param2, param3, param4);
    //	byteCodes.add(pByteCode);
}

char* vm_parser_generator_temp_var_create() {
    script* script = _vm_parser_ctx_current_script;

    char tmp[5];
    sprintf(tmp, "v%d", script->vm_parser_generator_temp_var_count++);

    char* var = text_clone(tmp);
    list_add(script->vm_parser_state_vars, var);

    return var;
}

void vm_parser_generator_temp_var_all_reset() {
    script* script = _vm_parser_ctx_current_script;
    script->vm_parser_generator_temp_var_count = 0;
}

char* vm_parser_generator_temp_label_create() {
    script* script = _vm_parser_ctx_current_script;
    unsigned int count = queue_count(script->vm_parser_state_labels);
    char tmp[5];
    sprintf(tmp, "L%d", count);
    return text_clone(tmp);
}

void vm_parser_generator_temp_labels_push(char* label) {
    script* script = _vm_parser_ctx_current_script;
    queue_push(script->vm_parser_state_labels, label);
}

char* vm_parser_generator_temp_labels_pop() {
    script* script = _vm_parser_ctx_current_script;
    return queue_pop(script->vm_parser_state_labels);
}

void vm_parser_generator_temp_labels_clear() {
    script* script = _vm_parser_ctx_current_script;
    //	script->vm_parser_generator_temp_label_count = 0;
    queue_clear(script->vm_parser_state_labels);
}

