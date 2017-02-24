config const N = 8;
var D = {0..N-1};
var a,b,c: [D][D] int;

a = [i in D][i in D] i;
b = [i in D][i in D] i;

forall m in D do
  forall n in D do
    forall k in D do
      c[m][n] += a[m][k] * b[k][n];

forall m in D do
  writeln(c[m]);
