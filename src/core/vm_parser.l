/*
* vm.parser.l
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

%option prefix="vm_parser_"
%option nounistd 

%{
	#include <enjector/core/text.h>
	#include <enjector/core/vm_parser.h>
	
	// Generated tokens
	#include "vm_parser.tab.h"
	
	#undef YY_INPUT
	#define YY_INPUT(buf,result,max_size) \
	{ \
		int c = vm_parser_parse_state_char_next(); \
		result = (c == 0) ? YY_NULL : (buf[0] = c, 1); \
	}
	
	#undef vm_parser_stype
	#define vm_parser_stype VM_PARSER_STYPE

	extern vm_parser_stype	vm_parser_lval;
	extern size_t			vm_parser_state_input_lineno;
	
	/*{name}		{ vm_parser_lval.s = text_clone(yytext); return NAME; }
	 {float}		{ vm_parser_lval.d = atof(yytext); return DECIMAL; }
	 {number}		{ vm_parser_lval.i = atoi(yytext); return NUMBER; }
	 */
	extern script*			_vm_parser_ctx_current_script;
%}

nl					(\r\n|\r|\n)
ws					[ \t]+
name				[a-zA-Z_][0-9a-zA-Z_]*
number				[0-9][0-9]*
float				{number}\.[0-9]+
comment				"//"([^\r|^\n])*
string				(\'(\\.|[^'])*\'|\"(\\.|[^"])*\")

condition			(">="|"=>"|"<="|"=<"|">"|"<"|"!="|"==")
conditional_operand	("&&"|"||")

%%

function	{ return FUNCTION; }
return		{ return RETURN; }
if			{ return IF; }
while		{ return WHILE; }
doevents	{ return DOEVENTS; }

{name}		{ vm_parser_lval.string = text_clone(yytext); return NAME; }
{float}		{ vm_parser_lval.string = text_clone(yytext); return DECIMAL; }
{number}	{ vm_parser_lval.string = text_clone(yytext); return NUMBER; }
{string}	{ vm_parser_lval.string = text_clone(yytext); return STRING; }

{condition}				{ vm_parser_lval.string = text_clone(yytext); return CONDITION; }
{conditional_operand}	{ vm_parser_lval.string = text_clone(yytext); return CONDITIONAL_OPERAND; }

{comment}	{/* skip */}
{ws}		{/* skip */}
{nl}		{ _vm_parser_ctx_current_script->vm_parser_state_input_lineno++; }

.			{ return yytext[0]; }
