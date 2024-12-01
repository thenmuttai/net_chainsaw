#include "helper.h"


int main() {
  time_t timer;
  time(&timer);

  printf("Local time: %s \n", ctime(&timer));

  return 0;
}