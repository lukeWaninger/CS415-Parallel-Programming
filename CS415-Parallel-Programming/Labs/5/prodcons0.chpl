//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A simple producer-consumer program using the taskQueue module.
//  - unlimited queue size
//  - a single pair of producer and consumer
//  - simple synchronization -- busy-waiting lock
//

module main {

  use taskQueue;

  config const numTasks = 20;

  const endFlag = createTask(0);        // special "termination" task
  var queue = new Queue(limit=0);       // queue with unlimited size
  var lock$: sync bool;
  var dummy: bool;

  proc producer() { 
    var task: Task;

    for i in 1..numTasks {
      task = createTask(i);
      lock$ = true;        // lock
      queue.add(task);
      dummy = lock$;       // unlock
      writeln("producer added task ", i, " to queue");
    }
    lock$ = true;
    queue.add(endFlag);
    dummy = lock$;
    writeln("producer added endFlag to queue");
  }

  proc consumer() { 
    var task: Task;
    var cnt: int = 0;

    while true {
      lock$ = true;
      task = queue.remove();
      dummy = lock$;      
      if task == nil {
        continue;
      }
      if task != endFlag {
        writeln("consumer removed task ", task.val, " from queue");
        cnt += 1;
      } else {
        writeln("consumer got endFlag, total tasks = ", cnt);
        break;
      }
    }
  } 

  proc main() {
    begin producer();
    consumer();
  } 

}
