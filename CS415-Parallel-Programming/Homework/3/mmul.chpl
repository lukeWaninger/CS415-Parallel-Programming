config const N = 8;
var D: domain(2) = {0..N-1, 0..N-1};
var A, B, C: [D] int;

[(m,n) in D] A(m,n) = m + n;  // set elements of A to ints from 0..N-1
B = 1;                        // set all of B's elements to 1s

// perform the multiplication
forall (m,n) in D do
  C(m,n) = + reduce ([k in 0..N-1] A(m,k) * B(k,n));

writeln((+ reduce([(m,n) in D] C(m,n))) + " (should be 3584)");
