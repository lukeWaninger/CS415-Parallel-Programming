config const N = 8;
var D: domain(2) = {0..N-1, 0..N-1};
var A, B, C: [D] int;         // setup A, B, and C to use the same square domain

[(m,n) in D] A(m,n) = m + n;  // set each for of A to ints from 0..N-1
B = 1;                        // set B's elements equal to A's

// perform the multiplication
// [(m,n) in D] C(m,n) = + reduce ([k in 0..N-1] A(m,k) * B(k,n));
forall (m,n) in D do
  C(m,n) = + reduce ([k in 0..N-1] A(m,k) * B(k,n));

writeln((+ reduce([(m,n) in D] C(m,n))) + " (should be 3584)");
