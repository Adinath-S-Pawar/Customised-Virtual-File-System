📁 Customised Virtual File System (CVFS)
A Virtual File System implemented in C/C++, simulating core Linux file system concepts like inodes, file tables, UAREA, and system call behavior.

🚀 Project Overview
This project is a custom implementation of a Virtual File System (VFS) written entirely in C/C++.
It features a custom shell, Linux-like commands, and fully simulated system calls like:
create, read, write, unlink, stat, lseek, exit, man ,ls
The project gives a strong understanding of OS internals, file structures, system calls, and memory management.

Key Features
* Custom Shell Interface :
Provides Linux-like commands for file operations (create, open, read, write, delete, Is, etc.).
* System Call Simulation :
Implements core Linux file system system calls (open, read, write, Iseek, close, rm, etc.) using C.
* File System Data Structures : 
  1) Incore Inode Table
  2) File Table
  3) UAREA (User Area)
  4) User File Descriptor Table
* Platform Independent
Allows system-level file handling functionalities of Linux to be used on any operating system platform.
* Database-like Functionality
Provides a customised database management layer with structured file handling.
