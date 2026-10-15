# Reference output erratum: testprog7

The original `testprog7.correct` is preserved without edits. Its first result is `res_1 = 25.0`
for the expression `"25.7" % 3.5`.

The course's language description specifies conversion to integers for the remainder operator.
Truncation yields `25 % 3`, which is `1`, not `25`. The second result, `47 % 3.5`, is `47 % 3 = 2`.

`testprog7.verified.correct` changes only the first result to `res_1 = 1.0`. All subsequent
diagnostics and other output remain identical to the original reference. The runner explicitly
labels this case and reports how many original reference outputs match separately.

The original implementation used a floating-point remainder function. The October 2026 maintenance
pass implements integer conversion, rejects zero/unsafe integer operands, and adds eight direct
checks in `value_operations_test.cpp`, including the numeric-string example above.
