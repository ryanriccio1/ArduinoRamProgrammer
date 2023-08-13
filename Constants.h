#pragma once
#define ENTRIES(x) (sizeof(x) / sizeof(x[0]))

#define PORT_B_DD 0b00100011
#define PORT_C_DD 0b00111111
#define PORT_D_DD 0b11111100

enum Instruction
{
  NOP = 0b0000,
  LDA = 0b0001,
  ADD = 0b0010,
  SUB = 0b0011,
  STA = 0b0100,
  LDI = 0b0101,
  JMP = 0b0110,
  JC  = 0b0111,
  JZ  = 0b1000,
  OUT = 0b1110,
  HLT = 0b1111,
  VAR = 0xFFFF
};

Instruction parseStringAsInstruction(const char* input)
{
  if(!strcmp("NOP", input))
    return NOP;
  if(!strcmp("LDA", input))
    return LDA;
  if(!strcmp("ADD", input))
    return ADD;
  if(!strcmp("SUB", input))
    return SUB;
  if(!strcmp("STA", input))
    return STA;
  if(!strcmp("LDI", input))
    return LDI;
  if(!strcmp("JMP", input))
    return JMP;
  if(!strcmp("JC", input))
    return JC;
  if(!strcmp("JZ", input))
    return JZ;
  if(!strcmp("OUT", input))
    return OUT;
  if(!strcmp("HLT", input))
    return HLT;
  if(!strcmp("VAR", input))
    return VAR;
}

void setDataDirection()
{
  DDRB = PORT_B_DD;
  DDRC = PORT_C_DD;
  DDRD = PORT_D_DD;
}

void resetPins()
{
  DDRB = 0;
  DDRC = 0;
  DDRD = 0;

  PORTB = 0b100000;
  PORTC = 0;
  PORTD = 0;
}

void stopExecution()
{
  PORTB = 0b100011;
}

void writeAddressPins(uint8_t byte)
{
  int write_to_c = (byte & 0b1111) | (PORTC & 0b110000);
  PORTC = write_to_c;
}

void writeDataPins(uint8_t byte)
{
  int write_to_D = (byte & 0b11111100) | (PORTD & 0b11);
  int write_to_C = ((byte & 0b11) << 4) | (PORTC & 0b1111);
  PORTD = write_to_D;
  PORTC = write_to_C;
}
