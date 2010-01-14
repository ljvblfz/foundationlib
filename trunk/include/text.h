/*
*	text.c
*	text modification tool C code file.
*	
*	Infrastructure, Text Analysis Module.
*	Based on SDK5.RC16, Linux 2.6.20, ARM Host CPU.
*	
*	MG3500 (MAXIM, mobilygen, Inc.) HD project
*   Copyright (c) Artificial Intelligence Infinity
*
*	2009.08.06 Solution designed by shaobin, Programmed by shaobin.
*/

#include "list.h"

typedef struct
{
    LIST_HANDLE line_node_handle;
    int line_num;
    int char_index;
}STRING_POSITION,*STRING_POSITION_PTR;

LIST_HANDLE read_text(STRING text_filename, void* line_cmp_fun);

static int find_keyword( 
                        LIST_HANDLE text_handle, /* A handle represents the target text. */ 
                        STRING keyword, /* Keyword for match */ 
                        LIST_HANDLE line_node, /* A handle represents matched line */ 
                        int PermissionTag /* Permission tag */ 
                        );

int find_line_num( 
                  LIST_HANDLE text_handle, /* A handle represents the target text. */ 
                  int line_num, /* line number */ 
                  LIST_HANDLE line_node, /* A handle represents matched line */ 
                  int PermissionTag /* Permission tag */ 
                  );

int replace_line_num( 
                     LIST_HANDLE text_handle, 
                     int line_num, 
                     STRING line_str, 
                     int line_len, 
                     STRING* old_line 
                     );

int replace_line_kw( 
                    LIST_HANDLE text_handle, 
                    STRING keyword, 
                    STRING line_str, 
                    int line_len, 
                    STRING* old_line 
                    );
int ins_line( 
             LIST_HANDLE text_handle, 
             STRING keyword, 
             STRING ins_line, 
             int length, 
             int ins_posi );

int text_save(
              STRING config_filename, 
              LIST_HANDLE text_handle
              );