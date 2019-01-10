#ifndef __LIGHTSCRIPT_DEFINES_H__
#define __LIGHTSCRIPT_DEFINES_H__

#include <lightscript/token.h>

// enable for debugging the parser and not to execute statements
//#define PARSER_DEBUGGING

// enable for testing in main.c
// #define MAIN_DEBUGGING

#define LS_IS_NUMBER(str) (str[0] >= '0' && str[0] <= '9')

#define LS_IS_ASSIGN_OPERATOR(type) ( \
type > ls_token_type_assign_ops_start && \
type < ls_token_type_assign_ops_end\
)

#define LS_IS_OPERATOR(type) (\
type == ls_token_type_or_op ||\
type == ls_token_type_and_op ||\
type == ls_token_type_greater_than ||\
type == ls_token_type_less_than ||\
type == ls_token_type_greater_equal ||\
type == ls_token_type_less_equal ||\
type == ls_token_type_equal_equal ||\
type == ls_token_type_not_equal ||\
type == ls_token_type_plus ||\
type == ls_token_type_minus ||\
type == ls_token_type_multiply ||\
type == ls_token_type_divide ||\
type == ls_token_type_percent ||\
type == ls_token_type_colon ||\
type == ls_token_type_power ||\
type == ls_token_type_log_or ||\
type == ls_token_type_log_and ||\
type == ls_token_type_double_colon ||\
(type > ls_token_type_assign_ops_start &&\
type < ls_token_type_assign_ops_end)\
)

// type != ls_token_type_comma && 
#define LS_IS_VALID_EXPR_TOKEN(type) ( \
type != ls_token_type_semicolon && \
type != ls_token_type_do_keyword && \
type != ls_token_type_in_keyword \
)

#define LS_IS_CHAR_TOKEN(type) ( \
type == ls_token_type_semicolon || \
type == ls_token_type_comma || \
type == ls_token_type_colon || \
type == ls_token_type_plus || \
type == ls_token_type_minus || \
type == ls_token_type_multiply || \
type == ls_token_type_divide || \
type == ls_token_type_percent || \
type == ls_token_type_lbrace || \
type == ls_token_type_rbrace || \
type == ls_token_type_lparenth || \
type == ls_token_type_rparenth || \
type == ls_token_type_lbracket || \
type == ls_token_type_rbracket || \
type == ls_token_type_equal || \
type == ls_token_type_dollar || \
type == ls_token_type_at || \
type == ls_token_type_hash || \
type == ls_token_type_less_than || \
type == ls_token_type_greater_than || \
type == ls_token_type_exclamation || \
type == ls_token_type_tilde || \
type == ls_token_type_log_or || \
type == ls_token_type_log_and || \
type == ls_token_type_unknown \
)

// TODO ==> decide if '.' is a token or use it for double values
#define LS_IS_TOKEN(c) (\
c == '#' || c == '^' || c == '$' || c == '=' || \
c == '-' || c == '+' || c == '%' || c == '/' || \
c == '*' || c == '&' || c == '|' || c == ';' || \
c == '\"' || c == '@' || c == '!' || c == '\'' || \
c == '~' || c == '<' || c == '>' || c == '(' || \
c == ')' || c == '[' || c == ']' || c == '{' || \
c == '}' || c == ' ' || c == '\t' || c == '\n' || \
c == ',' || c == ':' \
)

#define LS_VAR_GET_FINAL_VALUE(var) while(LS_VAR_IS_REFERENCE(var)) {\
  var = \
  ls_var_get_reference_value(var);\
}

#define LS_IS_DOUBLE_TOKEN(c, next) ( \
((c == '+' || c == '-' || c == '=' || c == '*' || c == '/' || c == '&' \
|| c == '|' || c == ':') && next == c) || \
((c == '+' || c == '-' || c == '=' || c == '*' || c == '/' || c == '&' || \
c == '|' || c == '!' || c == '<' || c == '>' || c == ':') && next == '=') \
)

#define LS_VAR_IS_INT(var) (var->type >= ls_var_type_s8 &&\
var->type <= ls_var_type_u64\
)
#define LS_VAR_IS_DOUBLE(var) (var->type == ls_var_type_double)
#define LS_VAR_IS_REFERENCE(var) (var->type == ls_var_type_reference)
#define LS_VAR_IS_STRING(var) (var->type == ls_var_type_string)
#define LS_VAR_IS_BOOLEAN(var) (var->type == ls_var_type_boolean)
#define LS_VAR_IS_MODULE(var) (var->type == ls_var_type_module)
#define LS_VAR_IS_OBJECT(var) (var->type == ls_var_type_object)
#define LS_VAR_IS_ARRAY(var) (var->type == ls_var_type_array)
#define LS_VAR_IS_FUNCTION(var) (var->type == ls_var_type_function)

#define LS_IS_SPACE(c) (c == ' ' || c == '\t' || c == '\n' || c == '\r')

#define LS_IS_VALID_EXPR_NODE(node) (node->children &&\
node->type != ls_node_type_function_call &&\
node->type != ls_node_type_array &&\
node->type != ls_node_type_array_element &&\
node->type != ls_node_type_object_property)

#define ls_boolean_false 0
#define ls_boolean_true 1

#endif