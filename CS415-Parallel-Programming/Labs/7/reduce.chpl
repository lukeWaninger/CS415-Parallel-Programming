//------------------------------------------------------------------------- 
// This is supporting software for CS415P/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A demo program for showing a combined forall + reduce code.
//
//
var D = {1..10};
var a = [i in D] i;
var sum: int = 0;

forall i in D with (+ reduce sum) do
  if a[i] % 2 == 0 then
    sum = a[i];
writeln("Sum of even values of a = ", sum);
