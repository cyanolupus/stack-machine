#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>

#define SIZE 100

enum Op {
    PUSH,
    POP,
    JUMP,
    JUMPIF,
    ADD,
    SUB,
    MUL,
    GET,
    SET,
    HALT,
    STSIZE,
    CALL,
    NOP
};

struct OpArg {
    enum Op op;
    int arg;
};

std::vector<std::string> split(std::string s, char cut) {
    std::vector<std::string> rtn;
    std::string ss_sp;
    std::stringstream ss{s};
    while(std::getline(ss, ss_sp, ' ')) {
        rtn.push_back(ss_sp);
    }
    return rtn;
}

std::vector<OpArg> input2Instructions() {
    std::string input;
    std::vector<std::string> inst;
    std::vector<OpArg> instructions;
    while(std::getline(std::cin, input)) {
        inst = split(input, ' ');
        if (inst[0] == "push") {
            instructions.push_back({PUSH, atoi(inst[1].c_str())});
        } else if (inst[0] == "pop") {
            instructions.push_back({POP});
        } else if (inst[0] == "jump") {
            instructions.push_back({JUMP, atoi(inst[1].c_str())});
        } else if (inst[0] == "jumpif") {
            instructions.push_back({JUMPIF, atoi(inst[1].c_str())});
        } else if (inst[0] == "add") {
            instructions.push_back({ADD});
        } else if (inst[0] == "sub") {
            instructions.push_back({SUB});
        } else if (inst[0] == "mul") {
            instructions.push_back({MUL});
        } else if (inst[0] == "get") {
            instructions.push_back({GET, atoi(inst[1].c_str())});
        } else if (inst[0] == "set") {
            instructions.push_back({SET, atoi(inst[1].c_str())});
        } else if (inst[0] == "stsize") {
            instructions.push_back({STSIZE});
        } else if (inst[0] == "halt") {
            instructions.push_back({HALT});
        } else if (inst[0] == "call") {
            instructions.push_back({CALL, atoi(inst[1].c_str())});
        } else if (inst[0] == "nop") {
            instructions.push_back({NOP});
        }
    }
    return instructions;
}

enum hostfunc {
    println,
    print
};

void call(hostfunc argv, int pop) {
    switch(argv) {
        case println:
            printf("%d\n",pop);
            break;
        case print:
            printf("%d",pop);
            break;
    }
}

void interpreter(std::vector<OpArg> instructions) {
    int stack[SIZE];
    int lc_var[SIZE];
    int s_pointer = 0;
    for (int pc = 0; pc < instructions.size(); pc++) {
        switch(instructions[pc].op) {
            case PUSH:
                s_pointer++;
                stack[s_pointer] = instructions[pc].arg;
                break;
            case POP:
                s_pointer--;
                break;
            case JUMPIF:
                s_pointer--;
                if (stack[s_pointer+1] != 0) {
                    break;
                }
            case JUMP:
                pc = instructions[pc].arg - 2;
                break;
            case ADD:
                s_pointer--;
                stack[s_pointer] = stack[s_pointer]+stack[s_pointer+1];
                break;
            case SUB:
                s_pointer--;
                stack[s_pointer] = stack[s_pointer]-stack[s_pointer+1];
                break;
            case MUL:
                s_pointer--;
                stack[s_pointer] = stack[s_pointer]*stack[s_pointer+1];
                break;
            case GET:
                s_pointer++;
                stack[s_pointer] = lc_var[instructions[pc].arg];
                break;
            case SET:
                lc_var[instructions[pc].arg] = stack[s_pointer];
                s_pointer--;
                break;
            case STSIZE:
                printf("stsize:%d\n",s_pointer);
                break;
            case HALT:
                exit(0);
                break;
            case CALL:
                call((hostfunc)instructions[pc].arg, stack[s_pointer]);
                s_pointer--;
                break;
            case NOP:
                break;
        }
    }
}

int main() {
    std::vector<OpArg> instructions = input2Instructions();
    interpreter(instructions);
    return 0;
}