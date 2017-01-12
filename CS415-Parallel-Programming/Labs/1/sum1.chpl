// The sum program (Chapel version).
//
//
config const N = 1000;	/* problem domain size */
const D = {0..N-1};
var sum: int;

proc compute(i: int): int { 
  return i*i;
}
sum = + reduce [i in D] (compute(i));
writeln("The result sum is " + sum);
