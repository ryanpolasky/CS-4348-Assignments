# CPU Scheduling Simulator

## Compilation

### Manual compilation:
```
g++ -o proj scheduler.cpp -std=c++11
```

## Usage

```
{path}/proj.exe input_file [FCFS|RR|SJF] [time_quantum]
```

### Examples:
```
{path}/proj.exe input.1 FCFS
{path}/proj.exe input.1 RR 2
{path}/proj.exe input.1 SJF
```

## Environment
- OS: Windows (with MinGW)
- IDE: JetBrains CLion
- Compilation: Handled via CMakeLists

## Input Format
Each line contains: `pid arrival_time burst_time`

Example:
```
1 0 10
2 0 9
3 3 5
```

## Algorithms Implemented
1. **FCFS** - First Come First Serve (non-preemptive)
2. **RR** - Round Robin (preemptive with time quantum)
3. **SJF** - Shortest Job First (preemptive SRTF)

