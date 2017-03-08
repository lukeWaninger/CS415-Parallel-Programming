//------------------------------------------------------------------------- 
// This is supporting software for CS415P/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A demo program for using C routines in Chapel.
//
//
extern proc time(t=nil): uint;
extern proc srand(uint);
extern proc rand(): int;

srand(time());
var x = rand();
var y = rand();
writeln (x % 50);
writeln (y % 50);
