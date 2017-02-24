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
  const endFlag = createTask();   // special "termination" task
  var count$: sync int = 0;       // sync variable for counting completed tasks

  proc producer() { 
    var task: Task;
    var idx: int;

    for i in 1..numTasks {
      task = createTask(i,i);
      idx = queueAdd(task);
      writeln("producer added ", task, " to queue (idx=", idx, ")");
    }
    idx = queueAdd(endFlag);
    writeln("producer added endFlag to queue (idx=", idx, ")");
  }

  proc consumer() { 
    var task: Task;
    
    while (true) {
      var c = count$; // grabs the lock
      task = queueRemove();
      count$ = c + 1; // sets the new count and unlocks
      writeln("consumer removed ", task, " from queue");
      if (task == endFlag) then break;
    }
    writeln("consumer got endFlag, total tasks = ", count$.readXX());
  } 

  proc main() {
    begin producer();
    consumer();
  } 
}
