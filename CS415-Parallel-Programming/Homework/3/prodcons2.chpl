//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A producer-consumer program using the circQueue1 module.
//  - a single pair of producer and comsumer
//

module main {

  use circQueue1;

  config const numTasks = 20;
  config const numCons  = 2;
  const endFlag: Task = createTask();
  var globalCount$: sync int = 0;       // sync variable for counting completed tasks

  proc producer() { 
    var task: Task;
    var idx: int;

    for i in 1..numTasks {
      task = createTask(i,i);
      idx = queueAdd(task);
      writeln("producer added ", task, " to queue (idx=", idx, ")");
    }
    
    for i in 1..numCons {
      idx = queueAdd(endFlag); // add ending flags to gueue
    }

    writeln("producer added endFlag to queue (idx=", idx, ")");
  }

  proc consumer() { 
    var task: Task;
    var localCount: int = 0;

    while (true) {
      task = queueRemove();
      localCount += 1; // sets the new count and unlocks
      writeln("consumer removed ", task, " from queue");
      if (task == endFlag) then break;
    }
    writeln("consumer got endFlag, total tasks = ", localCount);
    
    var gc = globalCount$; // grab the lock
    globalCount$ = gc + localCount; // update and unlock
  } 

  proc main() {
    sync {
      begin producer();
      coforall i in 1..numCons do begin consumer();
    }
    writeln("total tasks completed = ", globalCount$.readXX());
  } 
}
