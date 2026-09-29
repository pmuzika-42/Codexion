*This project has been created as part of the 42 curriculum by pmuzika.*

## Description

Codexion is a variation of the classic [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem).

It is a concurrent programming project written in C. It simulates several coders competing for a shared set of USB dongles.

Each coder runs in its own thread. There are as many dongles as coders, and a coder must acquire **two dongles simultaneously** to compile.

After compiling, the coder releases both dongles, then spends time debugging and refactoring before requesting the dongles again.

The simulation stops when either:

* a coder burns out because they fail to start a new compilation before their deadline;
* every coder has completed the required number of compilations.

Two scheduling policies are available:

* **FIFO** — requests are prioritized by arrival order.
* **EDF** — requests are prioritized by their compilation deadline.

---

## Instructions

### Compilation

```bash
make
```

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```bash
./codexion 3 800 200 200 200 3 10 fifo
```

### Arguments

| Argument                      | Description                                                                     |
| ----------------------------- | ------------------------------------------------------------------------------- |
| `number_of_coders`            | Number of coder threads and available dongles.                                  |
| `time_to_burnout`             | Maximum time in milliseconds between the start of two consecutive compilations. |
| `time_to_compile`             | Time spent compiling while holding two dongles.                                 |
| `time_to_debug`               | Time spent debugging after compilation.                                         |
| `time_to_refactor`            | Time spent refactoring before requesting dongles again.                         |
| `number_of_compiles_required` | Number of successful compilations required from every coder.                    |
| `dongle_cooldown`             | Time in milliseconds during which a released dongle cannot be acquired again.   |
| `scheduler`                   | `fifo` or `edf`.                                                                |

For this implementation, all numerical arguments must be greater than 0, except for `dongle_cooldown`, which may be 0.

For EDF, a request's deadline is calculated as:

```text
deadline = last_compile_start + time_to_burnout
```

---

## Blocking cases handled

### Deadlock prevention

A coder never acquires one dongle and waits while holding it for the second one.

The arbiter grants a request only when **both required dongles can be acquired at the same time**. If this is not possible, the coder keeps waiting without holding either dongle.

This removes the **hold-and-wait** condition responsible for the classical deadlock scenario.

### Request arbitration

All coder requests are managed by a global priority heap.

The scheduler determines the priority of requests:

* FIFO prioritizes requests by arrival order.
* EDF prioritizes requests by earliest compilation deadline.

A request only acts as a competing request for a coder when it has sufficient access to the two dongles it requires. This allows the arbiter to make a decision based on complete resource requests rather than individual dongles.

### Starvation and priority

Waiting coders are repeatedly evaluated according to the selected scheduling policy. Once a request becomes eligible, its priority determines whether it can be served before competing requests.

### Dongle cooldown

After a dongle is released, it cannot immediately be acquired again. Its `last_released` timestamp is checked before granting a request.

If the cooldown has not expired, the coder waits and retries when appropriate.

### Burnout detection

The monitor continuously checks the coders' compilation state and their deadlines.

The shared completion flag is updated when the simulation must stop, allowing waiting and running coder threads to terminate safely.

### Log serialization

All log messages are protected by a dedicated mutex. This prevents concurrent coder and monitor threads from interleaving their output.

---

## Thread synchronization mechanisms

The implementation uses POSIX threads, mutexes and condition variables to coordinate the coder threads, the monitor and the shared resources.

Different mutexes protect different pieces of shared state.

#### `arbiter_lock`

Protects the global arbitration state, including:

* the request heap;
* request insertion/removal;
* decisions about dongle allocation.

Only one thread can modify this state at a time.

#### Dongle mutexes

Each dongle has its own mutex protecting its state, including whether it is currently taken and its last release time.

#### `compile_lock`

Protects shared compilation data such as:

```c
last_compile_start
compiles_achieved
```

The coder updates these values while holding the mutex, while the monitor reads them under the same synchronization mechanism. This prevents races between compilation and burnout detection.

#### `finished_lock`

Protects the shared `finished_flag`, which indicates that the simulation must terminate.

#### `write_lock`

Serializes log output so that messages from different threads cannot overlap.

### `pthread_cond_t`

The arbiter uses a condition variable:

```c
pthread_cond_t arbiter_cond;
```

When a coder cannot currently acquire its two dongles, it waits on this condition variable.

`pthread_cond_wait()` releases `arbiter_lock` while the thread is sleeping and reacquires it before returning. This allows other threads to modify the arbitration state.

When a resource becomes available or the simulation state changes, waiting coders are awakened and re-evaluate:

* request priority;
* availability of both dongles;
* dongle cooldown;
* simulation termination state.

The conditions are always checked again after waking, since waking from a condition variable does not itself guarantee that the requested resources are available.

### Monitor / coder communication

The monitor and coder threads communicate through shared state protected by mutexes.

This provides thread-safe communication without the monitor directly controlling the coder threads.

---

## Testing

Concurrency problems are highly dependent on timing and scheduling, so the program should be tested with different combinations of parameters.

The following cases are useful for testing the implementation.

### Basic FIFO execution

```bash
./codexion 3 800 150 150 150 3 100 fifo
```

Tests a normal multi-coder simulation using FIFO arbitration.

### Basic EDF execution

```bash
./codexion 3 800 150 150 150 3 100 edf
```

Tests the same general workload using Earliest Deadline First scheduling.

### Single coder

```bash
./codexion 1 800 150 150 150 1 100 fifo
```

Tests the special case where only one dongle exists.

### Very short cooldown

```bash
./codexion 5 800 200 200 200 3 1 fifo
```

Tests resource reuse with almost no cooldown.

### Long cooldown

```bash
./codexion 5 800 200 200 200 3 300 fifo
```

Creates stronger contention around recently released dongles.

### High contention

```bash
./codexion 10 800 200 200 200 3 10 fifo
```

Useful for testing the arbitration system with several simultaneous requests.

### EDF under contention

```bash
./codexion 10 800 200 200 200 3 10 edf
```

Tests the EDF priority calculation when many coders compete for the same resources.

### Tight burnout deadline

```bash
./codexion 5 500 200 200 200 2 10 edf
```

Useful for testing burnout detection and deadline handling.

### Minimal compilation requirement

```bash
./codexion 4 800 200 200 200 1 10 fifo
```

Checks that the simulation stops correctly once every coder has completed one compilation.

---

## Resources

* **Threads, race conditions, mutexes, and condition variables:**
  [YouTube playlist](https://youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&si=xHUmNjhVcr_fA4Zb)

* **Philosophers project:**
  [GitHub repository](https://github.com/DeRuina/philosophers) — used as a reference to better understand the core logic and concepts behind the project.

* **Pthread API documentation:**
  [GeeksforGeeks — Thread Functions in C/C++](https://www.geeksforgeeks.org/c/thread-functions-in-c-c/)

### AI Usage

AI tools were used as a support resource for:

* Debugging segmentation faults and unexpected behavior.
* Designing test cases for different timing and thread-contention scenarios.
* Understanding and investigating issues encountered during development.
