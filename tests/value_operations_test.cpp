#include "val.h"
#include <iostream>
#include <limits>

int main() {
    int failed = 0;
    auto check = [&](bool condition, const char* name) {
        std::cout << (condition ? "PASS " : "FAIL ") << name << '\n';
        if (!condition) ++failed;
    };
    Value a = Value(25.7) % Value(3.5);
    check(a.IsNum() && a.GetNum() == 1.0, "decimal operands truncate before remainder");
    Value b = Value(47.0) % Value(3.5);
    check(b.IsNum() && b.GetNum() == 2.0, "integer remainder differs from floating remainder");
    Value c = Value(std::string("25.7")) % Value(3.5);
    check(c.IsNum() && c.GetNum() == 1.0, "numeric string is converted before remainder");
    Value d = Value(-25.7) % Value(3.5);
    check(d.IsNum() && d.GetNum() == -1.0, "negative dividend truncates toward zero");
    check((Value(5.0) % Value(0.5)).IsErr(), "divisor that truncates to zero is rejected");
    check((Value(5.0) % Value(0.0)).IsErr(), "zero divisor is rejected");
    check((Value(std::numeric_limits<double>::infinity()) % Value(3.0)).IsErr(), "nonfinite operand is rejected");
    check((Value(std::numeric_limits<double>::max()) % Value(3.0)).IsErr(), "out-of-range integer conversion is rejected");
    return failed == 0 ? 0 : 1;
}
