// This is just to suppress a warning message from Houdini.
// The plugin works without this.

#include <SYS/SYS_Version.h>
#include <OP/OP_OperatorTable.h>

extern "C" {
    // An empty function so houdini won't complain about missing symbols.
    void newDriverOperator(OP_OperatorTable* operators) 
    {
    }
}

