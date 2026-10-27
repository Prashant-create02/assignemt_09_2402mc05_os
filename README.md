# Assignment 9: Virtual Memory Management in xv6 (x86)

**Author:** Prashant  
**Course:** Operating Systems Laboratory  
**Institution:** IIT Patna  

---

## Overview
This repository contains the implementation and test programs for Assignment 9, focusing on advanced virtual memory mechanisms in the **xv6 (x86)** operating system kernel. The assignment is divided into three core components:
1. **Printing Process Page Tables** (`vmprint` & `pttest.c`)[span_0](start_span)[span_0](end_span)
2. **Lazy Page Allocation & Fault Handling** (`faulttest.c`)[span_1](start_span)[span_1](end_span)
3. **Page Replacement Algorithms (FIFO and LRU)** (`pagerepl.c`)[span_2](start_span)[span_2](end_span)

---

## Project Structure & Modified Files

### Kernel Modifications
* **`vm.c`**: Implemented `vmprint()` to walk process page tables; modified `copyuvm()` and `deallocuvm()` to handle deferred lazy allocations; exposed `mappages()`.
* **`trap.c`**: Added `T_PGFLT` trap handler to catch user-mode page faults via `CR2`, allocate physical frames on demand, and update process fault counters.
* **`proc.c` & `proc.h`**: Added a `page_faults` tracking counter inside `struct proc` and initialized it during process creation.
* **`sysproc.c`**: Updated `sys_sbrk()` for lazy allocation (`n > 0`) and implemented `sys_getfaults()`.
* **`syscall.c`, `syscall.h`, `user.h`, `usys.S`, `defs.h`**: Registered system calls `vmprint` (`SYS_vmprint`) and `getfaults` (`SYS_getfaults`)[span_3](start_span)[span_3](end_span).

### User-Space Test Programs
* **`pttest.c`**: Tests process page table mapping at start-up and after expanding the heap via `sbrk(4 * 4096)`[span_4](start_span)[span_4](end_span).
* **`faulttest.c`**: Validates lazy allocation by writing to selected pages and verifying fault counts, as well as testing out-of-bounds protection[span_5](start_span)[span_5](end_span).
* **`pagerepl.c`**: Simulates and compares FIFO and LRU page replacement algorithms across varying frame counts[span_6](start_span)[span_6](end_span).

---

## How to Build and Run

1. Open your terminal in the xv6 root directory.
2. Clean and compile the kernel with the user test programs:
   ```bash
   make clean
   make qemu-nox
