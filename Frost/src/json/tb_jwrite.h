#ifndef TB_JWRITE_INCLUDE_H
#define TB_JWRITE_INCLUDE_H
//
// A *really* simple JSON writer in C
// - a collection of functions to generate JSON semi-automatically
//
// The idea is to simplify writing native C values into a JSON string and
// to provide some error trapping to ensure that the result is valid JSON.
//
// Example:
//  tb_jwrite_control jwc;
//
//  tb_jwrite_open(&jwc, buffer, buflen, TB_JWRITE_OBJECT, 1);    // open root node as object
//  tb_jwrite_obj_string(&jwc, "key", "value");
//  tb_jwrite_obj_int(&jwc, "int", 1);
//  tb_jwrite_obj_array(&jwc, "anArray");
//      tb_jwrite_array_int(&jwc, 0);
//      tb_jwrite_array_int(&jwc, 1);
//      tb_jwrite_array_int(&jwc, 2);
//  tb_jwrite_end(&jwc);
//  tb_jwrite_error err = tb_jwrite_close(&jwc);          // close root object
//
// results in:
//
//  {
//      "key": "value",
//      "int": 1,
//      "anArray": [
//          0,
//          1,
//          2
//      ]
//  }
//
// Note that tb_jwrite handles string quoting and getting commas in the right place.
// If the sequence of calls is incorrect
// e.g.
//      tb_jwrite_open(buffer, buflen, TB_JWRITE_OBJECT, 1);
//      tb_jwrite_obj_string("key", "value");
//          tb_jwrite_array_int(0);
//      ...
//
// then the error code returned from jwClose() would indicate that you attempted to
// put an array element into an object (instead of a key:value pair)
// To locate the error, the supplied buffer has the JSON created upto the error point
// and a call to jwErrorPos() would return the function call at which the error occurred
// - in this case 3, the 3rd function call "tb_jwrite_array_int(0)" is not correct at this point.
//
// The root JSON type can be TB_JWRITE_OBJECT or TB_JWRITE_ARRAY.
//
// For more information on each function, see the prototypes below.
//

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>

// -----------------------------------------------------------------------------
// ----| Version |--------------------------------------------------------------
// -----------------------------------------------------------------------------

#define TB_JWRITE_VERSION_MAJOR    1
#define TB_JWRITE_VERSION_MINOR    1

#define TB_JWRITE_STACK_DEPTH 32   // max nesting depth of objects/arrays

// output string control for tb_jwrite_open()
typedef enum
{
    TB_JWRITE_COMPACT,
    TB_JWRITE_INLINE,
    TB_JWRITE_NEWLINE
} tb_jwrite_style;

// Error Codes
// -----------
typedef enum
{
    TB_JWRITE_OK,
    TB_JWRITE_NOT_ARRAY,   // tried to write Array value into Object
    TB_JWRITE_NOT_OBJECT,  // tried to write Object key/value into Array
    TB_JWRITE_STACK_FULL,  // array/object nesting > TB_JWRITE_STACK_DEPTH
    TB_JWRITE_STACK_EMPTY, // stack underflow error (too many 'end's)
    TB_JWRITE_NEST_ERROR,   // nesting error, not all objects closed when tb_jwrite_close() called
    TB_JWRITE_WRITE_ERROR
} tb_jwrite_error;

typedef enum
{
    TB_JWRITE_OBJECT,
    TB_JWRITE_ARRAY
} tb_jwrite_node_type;

typedef struct
{
    tb_jwrite_node_type type;
    int element;
} tb_jwrite_node;

typedef struct
{
    FILE* file;
    char tmpbuf[32];        // local buffer for int/double convertions
    tb_jwrite_error error;  // error code
    int call;               // call on which error occurred
    tb_jwrite_node nodes[TB_JWRITE_STACK_DEPTH]; // stack of array/object nodes
    int stackpos;
    tb_jwrite_style style;
} tb_jwrite_control;


// tb_jwrite_open
// - initialises tb_jwrite with the application supplied 'buffer' of length 'buflen'
//   in operation, the buffer will always contain a valid '\0'-terminated string
// - tb_jwrite will not overrun the buffer (it returns an "output buffer full" error)
// - rootType is the base JSON type: TB_JWRITE_OBJECT or TB_JWRITE_ARRAY
// - style controls 'prettifying' the output: TB_JWRITE_PRETTY or TB_JWRITE_COMPACT
void tb_jwrite_open(tb_jwrite_control* jwc, const char* target, tb_jwrite_node_type root_type, tb_jwrite_style style);

// tb_jwrite_close
// - closes the element opened by tb_jwrite_open()
// - returns error code (if everything was fine: TB_JWRITE_OK)
// - after an error, all following tb_jwrite calls are skipped internally
//   so the error code is for the first error detected
tb_jwrite_error tb_jwrite_close(tb_jwrite_control* jwc);

void tb_jwrite_set_style(tb_jwrite_control* jwc, tb_jwrite_style style);

// Object insertion functions
// - used to insert "key":"value" pairs into an object
void tb_jwrite_string(tb_jwrite_control* jwc, char* key, char* value);
void tb_jwrite_int(tb_jwrite_control* jwc, char* key, int value);
void tb_jwrite_float(tb_jwrite_control* jwc, char* key, float value);
void tb_jwrite_null(tb_jwrite_control* jwc, char* key);
void tb_jwrite_object(tb_jwrite_control* jwc, char* key);
void tb_jwrite_array(tb_jwrite_control* jwc, char* key);

// Array insertion functions
// - used to insert "value" elements into an array
void tb_jwrite_array_string(tb_jwrite_control* jwc, char* value);
void tb_jwrite_array_int(tb_jwrite_control* jwc, int value);
void tb_jwrite_array_float(tb_jwrite_control* jwc, float value);
void tb_jwrite_array_null(tb_jwrite_control* jwc);
void tb_jwrite_array_object(tb_jwrite_control* jwc);
void tb_jwrite_array_array(tb_jwrite_control* jwc);

// tb_jwrite_end
// - defines the end of an Object or Array definition
tb_jwrite_error tb_jwrite_end(tb_jwrite_control* jwc);

// these 'raw' routines write the JSON value as the contents of rawtext
// i.e. enclosing quotes are not added
// - use if your app. supplies its own value->string functions
void tb_jwrite_object_raw(tb_jwrite_control* jwc, char* key, char* rawtext);
void tb_jwrite_array_raw(tb_jwrite_control* jwc, char* rawtext);

// tb_jwrite_error_pos
// - if tb_jwrite_close returned an error, this function returns the number of the jWrite function call
//   which caused that error.
int tb_jwrite_error_pos(tb_jwrite_control* jwc);

// Returns '\0'-termianted string describing the error (as returned by tb_jwrite_close())
char* tb_jwrite_error_string(tb_jwrite_error err);

#ifdef __cplusplus
}
#endif

//
//
////   end header file   /////////////////////////////////////////////////////
#endif // TB_JWRITE_INCLUDE_H


/*
------------------------------------------------------------------------------
This software is available under the MIT License.
------------------------------------------------------------------------------
Copyright (c) 2019 Oliver Jakobs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
*/