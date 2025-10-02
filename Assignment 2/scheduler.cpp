//
// Created by Ryan Polasky on 10/1/2025.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int first_run_time;
    bool started;

    Process(int p, int a, int b) {
        pid = p;
        arrival_time = a;
        burst_time = b;
        remaining_time = b;
        completion_time = 0;
        first_run_time = -1;
        started = false;
    }
};

class Scheduler {
private:
    vector<Process> processes;
    int total_time;

public:
    void readInput(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Cannot open file " << filename << endl;
            exit(1);
        }

        int pid, arrival, burst;
        while (file >> pid >> arrival >> burst) {
            processes.push_back(Process(pid, arrival, burst));
        }
        file.close();
    }

    void scheduleFCFS() {
        vector<Process> procs = processes;
        int current_time = 0;
        int completed = 0;
        int n = procs.size();

        // sort by arrival
        sort(procs.begin(), procs.end(), [](const Process& a, const Process& b) {
            if (a.arrival_time == b.arrival_time) return a.pid < b.pid;
            return a.arrival_time < b.arrival_time;
        });

        int current_process = -1;

        while (completed < n) {
            // find next process to run
            if (current_process == -1) {
                for (int i = 0; i < n; i++) {
                    if (procs[i].remaining_time > 0 && procs[i].arrival_time <= current_time) {
                        current_process = i;
                        if (!procs[i].started) {
                            procs[i].first_run_time = current_time;
                            procs[i].started = true;
                        }
                        break;
                    }
                }
            }

            if (current_process == -1) {
                // CPU idle
                cout << "<system time " << current_time << "> [idle]" << endl;
                current_time++;
            } else {
                // run process
                cout << "<system time " << current_time << "> process "
                     << procs[current_process].pid << " is running" << endl;

                procs[current_process].remaining_time--;
                current_time++;

                if (procs[current_process].remaining_time == 0) {
                    procs[current_process].completion_time = current_time;
                    cout << "<system time " << current_time << "> process "
                         << procs[current_process].pid << " is finished........." << endl;
                    completed++;
                    current_process = -1;
                }
            }
        }

        total_time = current_time;
        cout << "<system time " << total_time << "> All processes finish...................." << endl;
        printStats(procs);
    }

    void scheduleRR(int quantum) {
        vector<Process> procs = processes;
        int current_time = 0;
        int completed = 0;
        int n = procs.size();

        queue<int> ready_queue;
        vector<bool> in_queue(n, false);

        while (completed < n) {
            // add new arrived processes to ready
            for (int i = 0; i < n; i++) {
                if (procs[i].arrival_time <= current_time &&
                    !in_queue[i] && procs[i].remaining_time > 0) {
                    ready_queue.push(i);
                    in_queue[i] = true;
                }
            }

            if (ready_queue.empty()) {
                cout << "<system time " << current_time << "> [idle]" << endl;
                current_time++;
                continue;
            }

            int current_process = ready_queue.front();
            ready_queue.pop();
            in_queue[current_process] = false;

            if (!procs[current_process].started) {
                procs[current_process].first_run_time = current_time;
                procs[current_process].started = true;
            }

            // execute for quantum or until completion
            int exec_time = min(quantum, procs[current_process].remaining_time);

            for (int t = 0; t < exec_time; t++) {
                cout << "<system time " << current_time << "> process "
                     << procs[current_process].pid << " is running" << endl;
                current_time++;
                procs[current_process].remaining_time--;

                // add new arrived processes
                for (int i = 0; i < n; i++) {
                    if (procs[i].arrival_time <= current_time &&
                        !in_queue[i] && procs[i].remaining_time > 0 && i != current_process) {
                        ready_queue.push(i);
                        in_queue[i] = true;
                    }
                }
            }

            if (procs[current_process].remaining_time == 0) {
                procs[current_process].completion_time = current_time;
                cout << "<system time " << current_time << "> process "
                     << procs[current_process].pid << " is finished........." << endl;
                completed++;
            } else {
                // readd to queue if not finished
                ready_queue.push(current_process);
                in_queue[current_process] = true;
            }
        }

        total_time = current_time;
        cout << "<system time " << total_time << "> All processes finish...................." << endl;
        printStats(procs);
    }

    void scheduleSJF() {
        vector<Process> procs = processes;
        int current_time = 0;
        int completed = 0;
        int n = procs.size();

        while (completed < n) {
            // find process with shortest remaining time
            int shortest = -1;
            int min_remaining = INT_MAX;

            for (int i = 0; i < n; i++) {
                if (procs[i].arrival_time <= current_time &&
                    procs[i].remaining_time > 0 &&
                    procs[i].remaining_time < min_remaining) {
                    min_remaining = procs[i].remaining_time;
                    shortest = i;
                }
            }

            if (shortest == -1) {
                cout << "<system time " << current_time << "> [idle]" << endl;
                current_time++;
                continue;
            }

            if (!procs[shortest].started) {
                procs[shortest].first_run_time = current_time;
                procs[shortest].started = true;
            }

            cout << "<system time " << current_time << "> process "
                 << procs[shortest].pid << " is running" << endl;

            procs[shortest].remaining_time--;
            current_time++;

            if (procs[shortest].remaining_time == 0) {
                procs[shortest].completion_time = current_time;
                cout << "<system time " << current_time << "> process "
                     << procs[shortest].pid << " is finished........." << endl;
                completed++;
            }
        }

        total_time = current_time;
        cout << "<system time " << total_time << "> All processes finish...................." << endl;
        printStats(procs);
    }

    void printStats(vector<Process>& procs) {
        int n = procs.size();
        double total_waiting = 0;
        double total_turnaround = 0;
        double total_response = 0;
        int total_burst = 0;

        for (int i = 0; i < n; i++) {
            int turnaround = procs[i].completion_time - procs[i].arrival_time;
            int waiting = turnaround - procs[i].burst_time;
            int response = procs[i].first_run_time - procs[i].arrival_time;

            total_turnaround += turnaround;
            total_waiting += waiting;
            total_response += response;
            total_burst += procs[i].burst_time;
        }

        double cpu_usage = (total_burst * 100.0) / total_time;
        double avg_waiting = total_waiting / n;
        double avg_response = total_response / n;
        double avg_turnaround = total_turnaround / n;

        cout << "============================================================" << endl;
        cout << fixed << setprecision(2);
        cout << "Average CPU usage: " << cpu_usage << " %" << endl;
        cout << "Average waiting time: " << avg_waiting << endl;
        cout << "Average response time: " << avg_response << endl;
        cout << "Average turnaround time: " << avg_turnaround << endl;
        cout << "============================================================" << endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: proj input_file [FCFS|RR|SJF] [time_quantum]" << endl;
        return 1;
    }

    string filename = argv[1];
    string algorithm = argv[2];

    Scheduler scheduler;
    scheduler.readInput(filename);

    if (algorithm == "FCFS") {
        scheduler.scheduleFCFS();
    } else if (algorithm == "RR") {
        if (argc < 4) {
            cout << "Error: Round Robin requires time quantum" << endl;
            return 1;
        }
        int quantum = stoi(argv[3]);
        scheduler.scheduleRR(quantum);
    } else if (algorithm == "SJF") {
        scheduler.scheduleSJF();
    } else {
        cout << "Error: Unknown algorithm " << algorithm << endl;
        return 1;
    }

    return 0;
}
