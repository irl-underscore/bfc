#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include "syscall.h"
#include "dyn_array.h"

char *assemble(dyn_array *operations, ArcType target);

#endif /* ASSEMBLER_H  */
