// Domain and array examples.
//

var D = {1..6};        // declare a domain
writeln("D = ", D);

var a: [D] int;        // declare an int array over domain
a = [i in D] i;        // a = [1,2,3,4,5,6]
writeln("a = ", a);

D = {2..4};            // now, redefine domain D
writeln("a = ", a);    // what happens?

D = {4..8};            // redefine domain D again
writeln("a = ", a);    // what happens?

