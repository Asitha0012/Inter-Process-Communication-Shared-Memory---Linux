# Inter-Process Communication using Shared Memory (Linux)

## 📘 Project Description

This project demonstrates **Inter-Process Communication (IPC)** using **shared memory** and **process hierarchy** in a Linux environment. The implementation models a parent-child process chain, where each process communicates recursively with its children using structured messaging.

### 👨‍👩‍👧 Process Hierarchy

P1
└── C1
  └── C2
    └── C3
      └── C4
        └── C5
          └── C6


## 💬 Communication Protocol

* The root process **P1** sends the message `"Hello my child"` to its child.
* Each child process, upon receiving the message:

  * Prints its process name (e.g., `C2`), its PID, the sender's PID, and the message.
  * If the child **has further children**, it:

    * Sends `"Hello my child"` to them.
    * Waits for all of them to respond with `"Hello mum"`.
    * Then responds to its own parent with `"Hello mum"` and terminates.
  * If the child **has no children**, it immediately responds with `"Hello mum"` to its parent.
* Once a process receives all `"Hello mum"` responses from its children, it appends a statement to `File2.txt`.


## 📦 Repository Contents

* Project source file with logic for:

  * Process creation using `fork()`
  * Shared memory communication
  * Recursive message handling
  * File writing and cleanup
* `Filex.text`: Output file that stores logs of completed child responses (e.g., `File14.text` for Group 14)
* `README.md`: This documentation


## 🛠 Technologies Used

* Language: C
* Operating System: Linux
* IPC Mechanism: Shared Memory
* System Concepts:

  * `fork()` for process creation
  * `shmget()`, `shmat()`, `shmdt()` for shared memory
  * `waitpid()` and `getpid()/getppid()` for control and communication
  * File I/O using standard C functions



## 🔄 Execution Logic

1. **P1** initiates communication by sending `"Hello my child"` to **C1**.
2. Each process does the following:

   * Receives a message and prints details.
   * If it has children:

     * Forwards the message to the next child.
     * Waits for a response from that child.
   * If it has no children:

     * Responds immediately with `"Hello mum"` to its parent.
3. Once a process receives all responses from its children, it:

   * Responds with `"Hello mum"` to its parent.
   * Logs its completion in `Filex.text`.
   * Terminates.
4. The root process **P1** terminates last, after receiving `"Hello mum"` from **C1**.


## ⚠️ Error Handling

* All system calls (e.g., `fork`, shared memory operations, file I/O) are checked for failures.
* Informative error messages are printed to standard error.
* Resources (shared memory, file descriptors) are properly cleaned up to avoid memory leaks or orphaned IPC objects.


## 📚 Academic Use

This project is developed as part of the **EE6206 Operating Systems Programming** coursework and is intended strictly for academic and demonstration purposes.

