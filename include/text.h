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
#include "libtype.h"

typedef struct
{
    STRING keyword;
    int permi_tag;
    LIST_HANDLE line_node_handle;
    int line_num;
    int word_index_begin;
    int word_index_end;
}MATCH_WORD_POSITION,*MATCH_WORD_POSITION_PTR;

typedef struct  
{
    int line_len;
    char* line_ptr;
}LINE_NODE,*LINE_NODE_PTR;

LIST_HANDLE read_text(STRING text_filename);

int find_keyword( 
                 LIST_HANDLE text_handle, /* A handle represents the target text. */ 
                 STRING keyword, /* Keyword for match */ 
                 MATCH_WORD_POSITION_PTR str_posi_ptr, 
                 int permi_tag /* Permission tag */ 
                 );

int find_line_num( 
                  LIST_HANDLE text_handle, /* A handle represents the target text. */ 
                  int line_num, /* line number */ 
                  LIST_HANDLE line_node, /* A handle represents matched line */ 
                  int permi_tag /* Permission tag */ 
                  );

int replace_line_num( 
                     LIST_HANDLE text_handle, 
                     int line_num, 
                     STRING new_line_str 
                     );

int replace_line_kw( 
                    LIST_HANDLE text_handle, 
                    STRING keyword, 
                    STRING new_line_str 
                    );

int replace_kw( 
               LIST_HANDLE text_handle, 
               STRING keyword, 
               STRING new_string 
               );

int insert_line( 
                LIST_HANDLE text_handle, 
                STRING keyword, 
                STRING ins_line, 
                int ins_posi 
                );

int text_save(
              STRING config_filename, 
              LIST_HANDLE text_handle
              );
