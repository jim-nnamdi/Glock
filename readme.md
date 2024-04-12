## Glock
> Memory Mapping

Memory-mapped I/O Input and output are handled by load/store (LDI/STI, LDR/STR) instructions using memory addresses to designate each I/O device register. Addresses xFE00 through xFFFF have been allocated to represent the addresses of I/O devices. Also, the code lists each of the relevant device registers that have been identified for the LC-3 thus far, along with their corresponding assigned addresses from the memory address space.