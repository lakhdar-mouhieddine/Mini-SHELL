# Mini-SHELL 🐚

A simple UNIX shell implementation in C, built for educational purposes. This shell supports executing commands in the foreground and background, handling pipelines, and managing process states via signals.

## Features

- **Basic Command Execution**: Runs standard UNIX commands using `execvp`.
- **Background & Foreground Jobs**:
  - Run commands in the background by appending `&` to the command line.
  - Commands run in the foreground by default.
- **Job Control**:
  - `jobs`: Lists all running and stopped jobs.
  - `bg <job>`: Resumes a suspended job in the background.
  - `fg <job>`: Brings a background or suspended job to the foreground.
- **Pipelines (`|`)**: Supports piping the standard output of one command to the standard input of another (e.g., `ls -l | grep .c`). Properly manages process groups to avoid deadlocks.
- **Signal Handling**:
  - `Ctrl+C` (`SIGINT`): Forwards the interrupt signal to the foreground job.
  - `Ctrl+Z` (`SIGTSTP`): Suspends the current foreground job.
  - Proper reaping of background processes via `SIGCHLD`.

## Built With
- **C** - The core language used.
- **UNIX System Calls**: `fork`, `execvp`, `pipe`, `waitpid`, `kill`, `sigprocmask`.

## Usage
Compile the shell using the provided `Makefile`:
```bash
make
```

Run the shell:
```bash
./shell
```

Execute commands:
```bash
shell> ls -la
shell> sleep 10 &
shell> jobs
shell> fg %1
```

Exit the shell:
```bash
shell> quit
```

## Testing
The repository includes several basic tests in the `tests/` directory to evaluate functionality. You can also run the provided driver script to evaluate functionality.