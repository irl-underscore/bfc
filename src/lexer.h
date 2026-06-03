#ifndef LEXER_H
#define LEXER_H
#include "dyn_array.h"
#include "file.h"

#include <stdint.h>

dyn_array *apply_o1_optimization(dyn_array *operations);
dyn_array *parse_file(file_buf *buf);

#endif /* LEXER_H */
