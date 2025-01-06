# Shared Memory with Inter-Process Communication

This C program uses shared memory and signals to enable multiple processes to communicate with each other. Each process is assigned an identifier, sends its message to shared memory, and other processes read and display the message. The inter-process communication is achieved through the `SIGALRM` signal.

## Requirements

- A C compiler (e.g., gcc)
- A Linux/Unix-based operating system
- Shared memory support

## Features

- Uses shared memory to allow up to 5 processes to run concurrently.
- Each process communicates by sending a unique identifier and message.
- Processes receive messages from other processes and display them on the terminal.
- Processes are synchronized through the `SIGALRM` signal to ensure proper message delivery.

## Installation and Compilation

1. Clone or download the repository.
2. Open a terminal in the project directory.
3. Compile the program using the following command:
    ```bash
    gcc -o ipc_message ipc_message.c
    ```

## Usage

To run the program, execute it with a unique process identifier. For example:

```bash
./ipc_message Process_1
