/*
* vm.c
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

#include <enjector/core/vm.h>
#include <enjector/core/xmemory.h>
#include <enjector/core/map.h>
#include <enjector/core/list.h>

#include <assert.h>

vm* vm_create() {
    vm* v = xmemory_new(vm);
    v->functions = map_create();
    return v;
}

void vm_free(vm* v) {
    assert(v);

    if (v->functions) {
        map_item**	items = map_enumerable(v->functions);

        for (int i = 0; i < map_count(v->functions); i++) {
            map_item* item = items[i];

            if (item->value) {
                vm_function* function = item->value;
                assert(function);
                assert(function->name);
                assert(function->instructions);

                xmemory_free(function->name);
                list_free(function->instructions);

                xmemory_free(function);
            }
        }

        map_free(v->functions);
    }

    xmemory_free(v);
}

//// Pointer to the function that is being created
//Function *pFunction;
//
//// Temporary store for the byte code generated. This list will be transformed
//// into an array of byte code that will be stored in the Function class
//List<ScriptByteCode*> byteCodes;

// Signal: Create a new function in the Script class
vm_function* vm_function_create(vm* v, char* name) {
    assert(v);
    assert(name);

    vm_function* function = xmemory_new(vm_function);
    function->name = name; // name is already text cloned
    function->instructions = list_create();

    map_set(v->functions, name, function);

    return function;
}

// Signal: End of the newly created function - this will now transfer the byte code
// to the Function class.
// Phase 1 - was the syntax directed code transformation
// Phase 2 - creates a label index
void vm_function_end(vm_function* fn) {
    assert(fn);
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
void vm_function_emit(vm_function* fn, vm_opcode opcode, char* param1, char* param2, char* param3, char* param4) {
    assert(fn);

    /*	if (param1) xmemory_free(param1);
    	if (param2) xmemory_free(param2);
    	if (param3) xmemory_free(param3);
    	if (param4) xmemory_free(param4);
    */
    //	ScriptByteCode *pByteCode = ScriptVM_bytecode_create(opCode, param1, param2, param3, param4);
    //	byteCodes.add(pByteCode);
}
