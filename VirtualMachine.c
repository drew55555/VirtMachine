#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#define HALT 0
#define SET 1
#define PUSH 2
#define POP 3
#define EQ 4
#define GT 5
#define JMP 6
#define JT 7
#define JF 8
#define ADD 9
#define MULT 10
#define MOD 11
#define AND 12
#define OR 13
#define NOT 14
#define RMEM 15
#define WMEM 16
#define CALL 17
#define RET 18
#define OUT 19
#define IN 20
#define NOOP 21
#define MINREG 32768
#define MAXREG 32775
#define MAX_VALUE 32768


unsigned short Registers[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

//int16_t *Memory = NULL;
unsigned short *Memory = NULL;

typedef struct Stack
{
  unsigned short *pointer;
  int size;
  int count;
} Stack;

Stack RuntimeStack;

void push(unsigned short value);
unsigned short pop();
int initialize();

int main(int argc, char *argv[])
{
  unsigned short temp, temp2;
  char c;
  int location = 0;
  int size = initialize(argv[1]), i;
  while(1)
  {
      //printf("\n%d\n%d\n%d\n", Memory[location], Memory[location+1], Memory[location+2]);
    switch(Memory[location])
    {
      case HALT:
	exit(0);
	break;
      case SET:
	temp = Memory[location+2];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	Registers[Memory[location+1] % MINREG] =
	  temp;
	location += 3;
	break;
      case PUSH:
	temp = Memory[location+1];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	push(temp);
	location += 2;
	break;
      case POP:
	Registers[Memory[location+1] % MINREG] = pop();
	location += 2;
	break;
      case EQ:
	temp = Memory[location+2];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp2 = Memory[location+3];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	  temp2 = Registers[temp2%MINREG];
	if(temp == temp2)
	  Registers[Memory[location+1]%MINREG] = 1;
	else
	  Registers[Memory[location+1]%MINREG] = 0;
	location += 4;
	break;
      case GT:
	temp = Memory[location+2];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp2 = Memory[location+3];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	  temp2 = Registers[temp2%MINREG];
	if(temp > temp2)
	  Registers[Memory[location+1]%MINREG] = 1;
	else
	  Registers[Memory[location+1]%MINREG] = 0;
	location += 4;
	break;
      case JMP:
	temp = Memory[location+1];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	location = temp;
	break;
      case JT:
	temp = Memory[location+1];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp2 = Memory[location+2];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	  temp2 = Registers[temp2%MINREG];
	if(temp != 0)
	  location = temp2;
	else
	  location += 3;
	break;
      case JF:
	temp = Memory[location+1];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp2 = Memory[location+2];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	  temp2 = Registers[temp2%MINREG];
	if(temp == 0)
	  location = temp2;
	else
	  location += 3;
	break;
      case ADD:
	temp = Memory[location+2];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp2 = Memory[location+3];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	  temp2 = Registers[temp2%MINREG];
	Registers[Memory[location+1]%MINREG] =
	  (temp + temp2) % MAX_VALUE;
	location += 4;
	break;
      case MULT:
	temp = Memory[location+2];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp2 = Memory[location+3];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	  temp2 = Registers[temp2%MINREG];
	Registers[Memory[location+1]%MINREG] =
	  (temp * temp2) % MAX_VALUE;
	location += 4;
	break;
      case MOD:
	temp = Memory[location+2];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp2 = Memory[location+3];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	  temp2 = Registers[temp2%MINREG];
	Registers[Memory[location+1]%MINREG] =
	  temp % temp2;
	location += 4;
	break;
      case AND:
	temp = Memory[location+2];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp2 = Memory[location+3];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	  temp2 = Registers[temp2%MINREG];
	Registers[Memory[location+1]%MINREG] =
	  temp & temp2;
	location += 4;
	break;
      case OR:
	temp = Memory[location+2];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp2 = Memory[location+3];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	  temp2 = Registers[temp2%MINREG];
	Registers[Memory[location+1]%MINREG] =
	  temp | temp2;
	location += 4;
	break;
      case NOT:
	temp = Memory[location+2];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	temp = ~temp;
	if((temp & 32768) != 0) temp -= 32768;
	Registers[Memory[location+1]%MINREG] =
	  temp;
	location += 3;
	break;
      case RMEM:
	temp = Memory[location+1];
	temp2 = Memory[location+2];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	    temp2 = Registers[temp2%MINREG];
	if(temp >= MINREG && temp <= MAXREG)
	  Registers[temp%MINREG] = Memory[temp2];
	else
	Memory[location+1] = Memory[temp2];
	location += 3;
	break;
      case WMEM:
	temp = Memory[location+1];
	temp2 = Memory[location+2];
	if(temp2 >= MINREG && temp2 <= MAXREG)
	    temp2 = Registers[temp2%MINREG];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	Memory[temp] = temp2;
	location += 3;
	break;
      case CALL:
	temp = Memory[location+1];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	push(location+2);
	location = temp;
	break;
      case RET:
	if((temp = pop()) < 0)
	  exit(0);
	location = temp;
	break;
      case OUT:
	temp = Memory[location+1];
	if(temp >= MINREG && temp <= MAXREG)
	  temp = Registers[temp%MINREG];
	printf("%c", (char)temp);
	location += 2;
	break;
      case IN:
	scanf("%c", &c);
	temp = Memory[location+1];
	if(temp >= MINREG && temp <= MAXREG)
	  Registers[temp%MINREG] = c;
	location += 2;
	break;
      case NOOP:
	location += 1;
	break;
      default:
	break;
    }
  }
}

int initialize(char *fileName)
{
  size_t result;
  struct stat st;
  int size;
  stat(fileName, &st);
  size = st.st_size;
  Memory = malloc(size * sizeof(char));
  if(Memory == NULL) { write(1, "Memory Error\n", 13); exit(0); }
  FILE *fp = fopen(fileName, "r");
  if(fp == NULL)
  {
    write(1, "File Error\n", 11);
    exit(0);
  }
  result = fread(Memory, 1, size, fp);
  fclose(fp);
  if(result < size)
  {
    write(1, "File Read Error\n", 16);
    exit(0);
  }
  RuntimeStack.pointer = malloc(10 * sizeof(unsigned short));
  RuntimeStack.size = 10;
  RuntimeStack.count = 0;
  return size;
}

void push(unsigned short value)
{
  if(RuntimeStack.count >= RuntimeStack.size)
  {
    RuntimeStack.size += 10;
    RuntimeStack.pointer = realloc(RuntimeStack.pointer,
	RuntimeStack.size * sizeof(unsigned short));
  }
  RuntimeStack.pointer[RuntimeStack.count] = value;
  RuntimeStack.count++;
  //write(1, "Push\n", 5);
}

unsigned short pop()
{
  RuntimeStack.count--;
  if(RuntimeStack.count < 0) { write(1, "Stack Empty\n", 12); exit(0); }
  return RuntimeStack.pointer[RuntimeStack.count];
  //write(1, "Pop\n", 4);
}
