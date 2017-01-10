// The sum program (Chapel version).
//
//
use BlockDist;
config const N = 1000;	/* problem domain size */
const D = {0..N-1};
const BlockD = D dmapped Block(boundingBox=D);
var sum: int;

proc compute(i: int): int { 
  return i*i;
}
sum = + reduce [i in BlockD] (compute(i));
writeln("The result sum is " + sum);
