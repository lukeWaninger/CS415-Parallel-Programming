// More domain and array examples.
//

var D = {1..6} by 2;   // domain striding
writeln("D = ", D);

var a: [D] int = [i in D] i;
writeln("a = ", a);

D = {1..8} by 2;       // a superset of original D
writeln("D = ", D);
writeln("a = ", a);    // what happens?

D = {1..8} by 1;       // another superset
writeln("D = ", D);
writeln("a = ", a);    // what happens?

D = {2..7} by 2;       // a disjoint striding
writeln("D = ", D);
writeln("a = ", a);    // what happens?

