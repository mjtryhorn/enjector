/*
* vm.parser.y
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

%define api.prefix vm_parser_
%{	
	#include <enjector/core/vm_parser.h>
	
	extern char yytext[];
		
	char* pszTempVar = 0;
	char* pszLabelName = 0;
	
	#define emit(opcode, p1, p2, p3, p4) vm_parser_function_emit(opcode, p1, p2, p3, p4)
	#define var_create					 vm_parser_generator_temp_var_create
	#define labels_pop					 vm_parser_generator_temp_labels_pop
	#define labels_push					 vm_parser_generator_temp_labels_push
	#define var_all_reset				 vm_parser_generator_temp_var_all_reset
	#define label_create				 vm_parser_generator_temp_label_create
%}

/*
 * Declare the type of values in the grammar
 */

%union
{
	int number;
	double decimal;
	char* string;
}

/*
 * Token types: These are returned by the lexer
 */
/*
%token <number> NUMBER
%token <decimal> DECIMAL
%token <string> NAME
*/
%token <string> NAME, NUMBER, DECIMAL, STRING
%token <string> CONDITION, CONDITIONAL_OPERAND
%token FUNCTION RETURN IF WHILE DOEVENTS

%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

/*
 * These are production names:
 */

%type <string> code
%type <string> function, function_call, function_header, parameters, expression_params
%type <string> statements, statement
%type <string> if_condition, while, while_condition, conditional_expressions, conditional_expression
%type <string> expression

%%

/*
 * Production starts here
 */

/*
 * [==- Code definition -----------------------------------------------==]
 */
code
: functions	{}
;

/*
 * [==- function definition ----------------------------------------------==]
 */
functions
: functions function	{}
|						{}
;

/* End of the new function. This will cause a new Function to be created
 */
function
: function_header '{' statements '}'	{ vm_parser_function_end(); }
;

/* Create a new function
 */
function_header
: FUNCTION NAME '(' parameters ')'		{ vm_parser_function_create($2); }
;

/* To get function parameters to populate they are poped from the stack and
 * placed into the variable names which is the parameter name
 */
parameters
: parameters ',' NAME		{ emit(opPOP, 0, 0, 0, $3);  /*$$ = $3;*/ }
| NAME						{ emit(opPOP, 0, 0, 0, $1);  /*$$ = $1;*/ }
|							{}
;

/*
 * [==- Statement definition -----------------------------------------------==]
 */
statements
: statements statement	{}
|						{}
;

/* A statement can be one of the following:
 *	<variable name> = <expression>					assigns a variable to the value of an expression
 *	<object name>.<property name> = <expression>	assigns the property of an object to the value of an expression
 *	<function name>([<parameters>])					calls a function with optional parameters
 */
statement
: NAME '=' expression ';'			{ emit(opSETVAR,    $3,  0, 0, $1); var_all_reset(); }
| NAME '.' NAME '=' expression ';'	{ emit(opSETOBJVAR, $1, $3, 0, $5); var_all_reset(); }
| function_call	';'					{ var_all_reset(); }
| RETURN expression ';'				{ emit(opRET, $2, 0, 0, 0); var_all_reset(); }
| RETURN ';'						{ emit(opRET, "0", 0, 0, 0); var_all_reset(); }
| if_condition statement			{ pszLabelName = $1; emit(opLABEL, pszLabelName, 0, 0, 0); }
| if_condition '{' statements '}'	{ pszLabelName = $1; emit(opLABEL, pszLabelName, 0, 0, 0); }
| while_condition statement			{ pszLabelName = labels_pop(); emit(opJUMP, labels_pop(), 0, 0, 0); emit(opLABEL, pszLabelName, 0, 0, 0); }
| while_condition '{' statements '}'{ pszLabelName = labels_pop(); emit(opJUMP, labels_pop(), 0, 0, 0); emit(opLABEL, pszLabelName, 0, 0, 0); }
| DOEVENTS ';'						{ emit(opDOEVENTS, 0, 0, 0, 0); }
;

if_condition
: IF '(' conditional_expressions ')'	{ pszLabelName = label_create(); emit(opIFCONDITION, $3, pszLabelName, 0, 0); $$ = pszLabelName; }
;

while_condition
: while '(' conditional_expressions ')'	{	pszLabelName = label_create(); emit(opIFCONDITION, $3, pszLabelName, 0, 0); labels_push(pszLabelName);	/* Do conditional check here */ }
;

while
: WHILE { pszLabelName = label_create(); emit(opLABEL, pszLabelName, 0, 0, 0); labels_push(pszLabelName);	/* Loop back to this point */ }
;

conditional_expressions
: conditional_expressions CONDITIONAL_OPERAND conditional_expression	{ pszTempVar = var_create(); emit(opCONDITION, $1, $2, $3, pszTempVar); $$ = pszTempVar; }
| conditional_expression												{ $$ = $1; }
;

conditional_expression
: expression CONDITION expression	{ pszTempVar = var_create(); emit(opCONDITION, $1, $2, $3, pszTempVar); $$ = pszTempVar; }
| expression						{ pszTempVar = var_create(); emit(opCONDITION, $1, "!=", "0", pszTempVar); $$ = pszTempVar; }
;

/*
 * [==- expression definition -----------------------------------------------==]
 */
expression
: expression '+' expression		{ pszTempVar = var_create(); emit(opADD, $1, $3, 0, pszTempVar); $$ = pszTempVar; }
| expression '-' expression		{ pszTempVar = var_create(); emit(opSUB, $1, $3, 0, pszTempVar); $$ = pszTempVar; }
| expression '*' expression		{ pszTempVar = var_create(); emit(opMUL, $1, $3, 0, pszTempVar); $$ = pszTempVar; }
| expression '/' expression		{ pszTempVar = var_create(); emit(opDIV, $1, $3, 0, pszTempVar); $$ = pszTempVar; }
| '-' expression %prec UMINUS	{ pszTempVar = var_create(); emit(opNEG, $2,  0, 0, pszTempVar); $$ = pszTempVar; }
| '(' expression ')'			{ $$ = $2; }
| NUMBER						{ $$ = $1; }
| DECIMAL						{ $$ = $1; }
| STRING						{ $$ = $1; }
| NAME							{ pszTempVar = var_create(); emit(opGETVAR,    $1,  0, 0, pszTempVar); $$ = pszTempVar; }
| NAME '.' NAME					{ pszTempVar = var_create(); emit(opGETOBJVAR, $1, $3, 0, pszTempVar); $$ = pszTempVar; }
| function_call					{ pszTempVar = var_create(); emit(opPOP,	    0,  0, 0, pszTempVar); $$ = pszTempVar; }
| xpath_expression				{ pszTempVar = var_create(); /*TODO*/ $$ = pszTempVar; }
| json_expression				{ pszTempVar = var_create(); /*TODO*/ $$ = pszTempVar; }
;

/*
 * [==- Function call definition -------------------------------------------==]
 */

/* Function calls involve the parameters to be placed onto the stack including
 * a temp variable for any results
 */
function_call
: NAME '(' expression_params ')'			{ emit(opCALL, $1, 0, 0, 0); }
| NAME '.' NAME '(' expression_params ')'	{ emit(opCALLOBJMETHOD, $1, $3, 0, 0); }
;

/* These are the function's parameters which are placed on the stack which
 * will be poped by the function.
 */
expression_params
: expression ',' expression_params	{ emit(opPUSH, $1, 0, 0, 0); }
| expression						{ emit(opPUSH, $1, 0, 0, 0); }
|									{}
;


/*
 * [==- XPath expression definition -------------------------------------------==]
 */

xpath_expression
: xpath_fragments
;

xpath_fragments
: xpath_fragment xpath_fragments
| xpath_fragment
|
;

xpath_fragment
: '/' NAME
| '/' NAME '[' conditional_expressions ']'

/*
 * [==- JSON expression definition -------------------------------------------==]
 */
json_expression
: '[' json_array_items ']'
| '{' json_tuples '}'
;

json_value
: NUMBER
| DECIMAL
| STRING
| NAME
| json_expression
;

json_tuples
: json_tuple
| json_tuple ',' json_tuple
|
;

json_tuple
: NAME ':' json_value
;

json_array_items
: json_value
| json_value ',' json_value
|
;

%%