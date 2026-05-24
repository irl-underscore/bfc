#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "dyn_array.h"
#include "type.h"

char *assemble(dyn_array *operations, ArcType target);

#endif /* ASSEMBLER_H  */
