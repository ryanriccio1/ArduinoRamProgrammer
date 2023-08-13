#pragma once
#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>
#include "Constants.h"

struct ProgramLine
{
  Instruction instruction;
  uint8_t argument;
};

class Program
{
public:
  String name;
  bool wait_for_halt;
  unsigned long program_time;
  ProgramLine programLines[16];
  Program(String name, const char* program[], size_t lines, bool wait_for_halt, unsigned long program_time);
};

Program::Program(String name, const char* program[], size_t lines, bool wait_for_halt, unsigned long program_time) : name{ name }, wait_for_halt{ wait_for_halt }, program_time{ program_time }
{
  for (auto programLine : programLines)
  {
    programLine.instruction = NOP;
    programLine.argument = 0;
  }

  for (size_t idx = 0; idx < lines; idx++)
  {
    int addr;
    char mnemonic[4];
    int arg = 0;
    sscanf(program[idx], "%d: %s %d", &addr, mnemonic, &arg);
    programLines[addr].instruction = parseStringAsInstruction(mnemonic);
    programLines[addr].argument = arg;
  }
 
}