
#include <iostream>
#include <queue>
#include <string>

using namespace std;

struct PCB {
    int processID;
    // 0 = new, 1 = ready, 2 = running, 3 = terminated
    int state;
    int programCounter;
    int instructionBase;
    int dataBase;
    int memoryLimit;
    int cpuCyclesUsed;
    int registerValue;
    int maxMemoryNeeded;
    int mainMemoryBase;
    vector<int> logicalMemory;
};

void loadJobsToMemory(queue<PCB> &newJobQueue, queue<int> &readyQueue, int* mainMemory, int maxMemory) {
    int currentAddress = 0;
    while(!newJobQueue.empty()) {
        PCB newJob = newJobQueue.front();
        newJobQueue.pop();
        if(newJob.memoryLimit <= maxMemory - currentAddress) {
            // Adjust the PCB fields to reflect the new memory location
            newJob.mainMemoryBase = currentAddress;
            newJob.state = 1;
            newJob.instructionBase = newJob.instructionBase + newJob.mainMemoryBase;
            newJob.dataBase = newJob.dataBase + newJob.mainMemoryBase;

            // Load the job into memory
            mainMemory[currentAddress] = newJob.processID;
            mainMemory[currentAddress + 1] = newJob.state;
            mainMemory[currentAddress + 2] = newJob.programCounter;
            mainMemory[currentAddress + 3] = newJob.instructionBase;
            mainMemory[currentAddress + 4] = newJob.dataBase;
            mainMemory[currentAddress + 5] = newJob.memoryLimit;
            mainMemory[currentAddress + 6] = newJob.cpuCyclesUsed;
            mainMemory[currentAddress + 7] = newJob.registerValue;
            mainMemory[currentAddress + 8] = newJob.maxMemoryNeeded;
            mainMemory[currentAddress + 9] = newJob.mainMemoryBase;

            currentAddress += 10;
            for(int i = 0; i < newJob.logicalMemory.size(); i++) {
                mainMemory[currentAddress + i] = newJob.logicalMemory[i];
            }
            readyQueue.push(newJob.mainMemoryBase);
            currentAddress += newJob.memoryLimit;
        } else {
            cout << "Error: Not enough memory to load job " << newJob.processID << endl;
        }
    }
}

void executeCPU(int startAddress, int* mainMemory) {
    int processID = mainMemory[startAddress];
    int state = mainMemory[startAddress + 1];
    int programCounter = mainMemory[startAddress + 2];
    int instructionBase = mainMemory[startAddress + 3];
    int dataBase = mainMemory[startAddress + 4];
    int memoryLimit = mainMemory[startAddress + 5];
    int cpuCyclesUsed = mainMemory[startAddress + 6];
    int registerValue = mainMemory[startAddress + 7];
    int maxMemoryNeeded = mainMemory[startAddress + 8];
    int mainMemoryBase = mainMemory[startAddress + 9];

    programCounter = instructionBase;
    int dataPointer = dataBase;
    state = 2;
    mainMemory[startAddress + 1] = state;
    while(programCounter < dataBase) {
        int instruction = mainMemory[programCounter];
        switch (instruction){
            case 1: {
                cout << "compute" << endl;
                int iterations = mainMemory[dataPointer];
                dataPointer++;
                int cycles = mainMemory[dataPointer];
                dataPointer++;
                cpuCyclesUsed += cycles;
                mainMemory[startAddress + 6] = cpuCyclesUsed;
                programCounter++;
                mainMemory[startAddress + 2] = programCounter;
                break;
            }
            case 2: {
                cout << "print" << endl;
                int cycles = mainMemory[dataPointer];
                dataPointer++;
                cpuCyclesUsed += cycles;
                mainMemory[startAddress + 6] = cpuCyclesUsed;
                programCounter++;
                mainMemory[startAddress + 2] = programCounter;
                break;
            }
            case 3: {
                int value = mainMemory[dataPointer];
                dataPointer++;
                registerValue = value;
                mainMemory[startAddress + 7] = registerValue;
                int address = mainMemory[dataPointer];
                dataPointer++;
                address += mainMemoryBase;
                if(address >= dataBase && address < startAddress + memoryLimit) {
                    cout << "stored" << endl;
                    mainMemory[address] = value;
                } else {
                    cout << "store error!" << endl;
                }
                cpuCyclesUsed++;
                mainMemory[startAddress + 6] = cpuCyclesUsed;
                programCounter++;
                mainMemory[startAddress + 2] = programCounter;
                break;
            }
            case 4: {
                int address = mainMemory[dataPointer];
                dataPointer++;
                address += mainMemoryBase;
                if(address >= dataBase && address < startAddress + memoryLimit) {
                    cout << "loaded" << endl;
                    registerValue = mainMemory[address];
                    mainMemory[startAddress + 7] = registerValue;
                } else {
                    cout << "load error!" << endl;
                }
                cpuCyclesUsed++;
                mainMemory[startAddress + 6] = cpuCyclesUsed;
                programCounter++;
                mainMemory[startAddress + 2] = programCounter;
                break;
            }
        }
    }

    programCounter = instructionBase-1;
    mainMemory[startAddress + 2] = programCounter;
    state = 3;
    mainMemory[startAddress + 1] = state;

    // Output PCB details after execution
    cout << "Process ID: " << mainMemory[startAddress] << endl;
    cout << "State: " << "TERMINATED" << endl;
    cout << "Program Counter: " << mainMemory[startAddress + 2] << endl;
    cout << "Instruction Base: " << mainMemory[startAddress + 3] << endl;
    cout << "Data Base: " << mainMemory[startAddress + 4] << endl;
    cout << "Memory Limit: " << mainMemory[startAddress + 5] << endl;
    cout << "CPU Cycles Used: " << mainMemory[startAddress + 6] << endl;
    cout << "Register Value: " << mainMemory[startAddress + 7] << endl;
    cout << "Max Memory Needed: " << mainMemory[startAddress + 8] << endl;
    cout << "Main Memory Base: " << mainMemory[startAddress + 9] << endl;
    cout << "Total CPU Cycles Consumed: " << mainMemory[startAddress + 6] << endl;

}

int main() {
    int maxMemory;
    int numProcesses;
    queue<PCB> newJobQueue;
    queue<int> readyQueue;
    int* mainMemory;

    // Step 1: Read and parse input file
    cin >> maxMemory >> numProcesses;
    mainMemory = new int[maxMemory];
    for(int i = 0; i < maxMemory; i++) {
        mainMemory[i] = -1;
    }
    for(int i = 0; i < numProcesses; i++) {
        // Each process gets a new PCB struct, initialize all fields
        PCB newJob;
        cin >> newJob.processID;
        newJob.state = 0;
        newJob.programCounter = 0;
        newJob.cpuCyclesUsed = 0;
        newJob.registerValue = 0;
        newJob.instructionBase = 10;
        cin >> newJob.maxMemoryNeeded;
        newJob.memoryLimit = newJob.maxMemoryNeeded;
        int numInstructions;
        cin >> numInstructions;
        newJob.dataBase = newJob.instructionBase + numInstructions;

        // Make enough space for the instructions in the local memory
        for(int j = 0; j < numInstructions; j++) {
            newJob.logicalMemory.push_back(0);
        }
        // Store each instruction in the previously padded space, then store corresponding data sequentially after the instruction space
        for(int j = 0; j < numInstructions; j++) {
            int instruction;
            cin >> instruction;
            newJob.logicalMemory[j] = instruction;
            int data;
            // Even encoded instructions take 1 data value, odd encoded instructions take 2
            if(instruction % 2 == 0){
                cin >> data;
                newJob.logicalMemory.push_back(data);
            } else {
                cin >> data;
                newJob.logicalMemory.push_back(data);
                cin >> data;
                newJob.logicalMemory.push_back(data);
            }
        }
        newJobQueue.push(newJob);
    }

    // Step 2: Load jobs into main memory
    loadJobsToMemory(newJobQueue, readyQueue, mainMemory, maxMemory);

    // Step 3: After you load the jobs in the queue go over the main memory and print the content of mainMemory
    for(int i = 0; i < maxMemory; i++) {
        cout << i << " : " << mainMemory[i] << endl;
    }

    // Step 4: Process execution
    while (!readyQueue.empty()) {
        int startAddress = readyQueue.front();
        // readyQueue contains start addressses w.r.t. mainMemory for jobs

        readyQueue.pop();

        // Execute the job
        executeCPU(startAddress, mainMemory);

        // Output Job that just completed execution
    }



    return 0;
}
