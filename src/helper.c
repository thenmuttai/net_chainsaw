#include "helper.h"


void winsock_init(WSADATA *d) {
  // Initiate the use of Winsock DLL (Dynamic link library)
  // MAKEWORD specify the version of Winsock to use
  int result = WSAStartup(MAKEWORD(2, 2), d);
  if (result != 0) {
    fprintf(stderr, "WSAStartup failed [ERR: %d]\n", result);
    exit(EXIT_FAILURE);
  }
}