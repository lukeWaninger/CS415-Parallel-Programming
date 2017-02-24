//------------------------------------------------------------------------- 
// This is supporting software for CS415/515 Parallel Programming.
// Copyright (c) Portland State University.
//------------------------------------------------------------------------- 

// A circular task queue (version 1)
//  - limited synchronization
//  - suitable for a single pair of producer and comsumer
//

module circQueue1 {

  class Task {
    var low, high: int;			// a task is a pair of integers
  }

  proc createTask(low = 0, high = 0) {
    return new Task(low, high);
  } 

  config const buffSize = 10;

  var buff$: [0..buffSize-1] sync Task;	// circular task buffer
  var head = 0;	  	                // head position (for removal)
  var tail = 0;			        // tail position (for addition)

  proc queueAdd(task) {
    buff$[tail] = task;
    tail = (tail + 1) % buffSize;
    return tail;
  }

  proc queueRemove() {
    var idx = head;
    head = (idx + 1) % buffSize;
    return buff$[idx];
  }

}

