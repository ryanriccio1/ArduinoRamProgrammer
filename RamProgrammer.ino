/*

DATA PINOUT
.........................................
: 7  : 6  : 5  : 4  : 3  : 2  : 1  : 0  :
:....:....:....:....:....:....:....:....:
: D7 : D6 : D5 : D4 : D3 : D2 : A5 : A4 :
:....:....:....:....:....:....:....:....:

ADDR PINOUT
.....................
: 3  : 2  : 1  : 0  :
:....:....:....:....:
: A3 : A2 : A1 : A0 :
:....:....:....:....:

CONTROL PINOUT
.....................
: HLT : RESET : /WE :
:.....:.......:.....:
: D8  : D9    : D13 :
:.....:.......:.....:

INVERTED RESET LINE CONNECTED TO 74LS157's
5V -> 5V, GND -> GND

*/

#include "Program.h"

#define POST_HLT_TIME 2000

const char* program1_c_str[] = { "0: LDA 14",
                                 "1: ADD 15",
                                 "2: OUT",
                                 "3: HLT",
                                 "14: VAR 28",
                                 "15: VAR 14" };

const char* fibonacci_c_str[] = { "0: LDI 1",
                                  "1: STA 14",
                                  "2: LDI 0",
                                  "3: STA 15",
                                  "4: OUT",
                                  "5: LDA 14",
                                  "6: ADD 15",
                                  "7: STA 14",
                                  "8: OUT",
                                  "9: LDA 15",
                                  "10: ADD 14",
                                  "11: JC 13",
                                  "12: JMP 3",
                                  "13: HLT",
                                  "14: VAR 0",
                                  "15: VAR 0" };

const char* multiply_c_str[] = { "0: LDA 14",
                                 "1: SUB 12",
                                 "2: JC 6",
                                 "3: LDA 13",
                                 "4: OUT",
                                 "5: HLT",
                                 "6: STA 14",
                                 "7: LDA 13",
                                 "8: ADD 15",
                                 "9: STA 13",
                                 "10: JMP 0",
                                 "11: NOP",
                                 "12: VAR 1",
                                 "13: VAR 0",
                                 "14: VAR 3",
                                 "15: VAR 8" };

Program programs[] = {Program(String("Addition Program"), program1_c_str, ENTRIES(program1_c_str), true, 5000),
                      Program(String("Mult Pro"), multiply_c_str, ENTRIES(multiply_c_str), true, 22000L),
                      Program(String("Fib Prog"), fibonacci_c_str, ENTRIES(fibonacci_c_str), true, 50000L) };

void loadProgram(Program program) {
  setDataDirection();
  stopExecution();

  for (size_t idx = 0; idx < 16; idx++) {
    writeAddressPins(idx);
    if (program.programLines[idx].instruction != VAR) {
      writeDataPins((program.programLines[idx].instruction << 4) | program.programLines[idx].argument);
    } else if (program.programLines[idx].instruction == VAR) {
      writeDataPins(program.programLines[idx].argument);
    }

    delayMicroseconds(15);
    PORTB &= 0b011111;
    delayMicroseconds(10);
    PORTB |= 0b100000;
    delayMicroseconds(20);
  }

  delayMicroseconds(40);
  resetPins();
}

void wait_for_halt()
{
  DDRB &= ~0x01;   // D8 (HLT) as input
  while ((PINB & 0x01) != 1)
  {
    __asm__("nop\n\t");
  }
  resetPins();
}

void setup() {
  Serial.begin(57600);
  while (!Serial) {};
}

void loop() {
  for (auto program : programs) {
    Serial.print("Loading program: ");
    Serial.println(program.name);
    
    loadProgram(program);
    
    if (program.wait_for_halt)
    {
      Serial.println("Wating");
      wait_for_halt();
      delay(POST_HLT_TIME);
    }
    else
    {
      delay(program.program_time);
    }
  }
}
