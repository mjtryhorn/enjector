/*
* vm_parser.h
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

#include <enjector/core/vm.h>
#include <enjector/core/queue.h>
#include <enjector/core/list.h>

typedef struct script_t {
    vm*				vm;

    // Parser state
    const char*		vm_parser_state_input;
    size_t			vm_parser_state_input_len;
    size_t			vm_parser_state_input_index;
    size_t			vm_parser_state_input_lineno;

    int				vm_parser_generator_temp_var_count;
    queue*			vm_parser_state_labels;
    list*			vm_parser_state_vars;

    vm_function*	vm_parser_state_current_function;
} script;

script* vm_parser_create();
bool vm_parser_load(script* script, const char* text);
void	vm_parser_clear(script* script);
void	vm_parser_dispose(script* script);

char*	vm_parser_generator_temp_var_create();
void	vm_parser_generator_temp_var_all_reset();

char*	vm_parser_generator_temp_label_create();
void	vm_parser_generator_temp_labels_push(char* label);
char*	vm_parser_generator_temp_labels_pop();
void	vm_parser_generator_temp_labels_clear();

vm_function*	vm_parser_function_create(char* name);
void			vm_parser_function_end();
void			vm_parser_function_emit(vm_opcode opcode, char* param1, char* param2, char* param3, char* param4);
