1. add "set" and "dict" types:
    implement "set" data structure with hash-table (like Python with dummy values) or b-tree structures.
    implement "dict" data structure with hash-table structure.
2. cleaning code:
    clean up code.
    fix names of variables and functions.
    fix headers and avoid headers including duplicates.
3. fix position and context system (Traceback process in runtime_error):
    fix the positioning system for strings and variables.
    specifically, the problem can be seen in "interpreter.c" >> interpret_for >> function // ill defined.
    also, fix misaddressing caused by removing position and context from values.
4. fix problems in parser grammar:
    fix some issues with multiline statements in the parser (if, switch, for, while, and try statements).
    specifically, to accept semicolons as newline (sometimes).
5. delete copy system from language:
    restrict copying values in the interpreter even further to avoid any unnecessary time and memory investment.
    change methods for assigning values into variables and variable pointers.
    change function methods for copying to avoid context conflicts.
7. change function algorithms:
    change functions to achieve first-class functions properly.
    upgrade generating method for function body to increase performance and avoid memory wasting.
8. upgrade loop algorithms:
    upgrade generating method for loop bodies (like functions) to increase performance.
9. upgrade "struct" data-structure:
    currently, "struct" accepts any kind of content for its body.
    this should be restricted to some level to keep the language readable.
    also, add constructors for structures to increase the ease of coding.
