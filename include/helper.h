#ifndef HELPER_HEADER_FILE
#define HELPER_HEADER_FILE

// _WIN32 defined by compiler to indicate it's a windows compilation
#if defined(_WIN32)   /* ---------------------------------- do windows stuff */
// _WIN32_WINNT - specify minimum windows SDK(or OS) version needed
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   _WIN32_WINNT_WIN7   // Windows 7
#endif

// Winsock specific header files
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// MACRO definitions
/* CHECK IF THE SOCKET IS VALID */
// in winsock, socket() return INVALID_SOCKET if fails
// in berkeley, socket() return negative number if fails
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)

/* CLOSE THE SOCKET */
#define CLOSESOCKET(s) closesocket(s)

/* GET SOCKET ERROR */
#define GETSOCKERRORNO()  (WSAGetLastError())

#else                 /* ------------------------------------ do linux stuff */

// Berkeley socket specific header files
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

// MACRO definitions
#define ISVALIDSOCKET(s) ((s) >= 0)

#define CLOSESOCKET(s) close(s)

#define GETSOCKERRORNO()  (errno)

// in winsock, socket() return SOCKET upon success (SOCKET -typedef for unsigned int)
// in berkeley socket, socket() returns non-negative int upon success
#define SOCKET  int

#endif   // _WIN32 CHECK


// General Header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Function declerations
void winsock_init(WSADATA *);


#endif   // HELPER_HEADER_FILE