#include "helper.h"

#define SERV_PORT         "8080"
#define CONN_QUEUE_LIMIT  10


int main() {
  puts("[+] Starting the time_server");

  /* A. Initalize Socket (applicable only to Winsock) */
  #if defined(_WIN32)
  WSADATA d;
  winsock_init(&d);
  #endif


  /* B. Configure Local IP and Port to bind to */
  struct addrinfo hint, *bind_addr;
  memset(&hint, 0, sizeof(hint));

  // updating additional information for the socket
  hint.ai_family   = AF_INET;        // IPv4 protocol family
  hint.ai_socktype = SOCK_STREAM;    // TCP socket type
  hint.ai_flags    = AI_PASSIVE;     // specify the returned socket address
                                     // is suitable with bind() and accept()

  // translate the hostname(IP) + port and returns a socket address with
  // additional information used for creating a socket
  getaddrinfo(0, SERV_PORT, &hint, &bind_addr);


  /* C. Create a Socket */
  SOCKET socket_listen = socket(
    bind_addr->ai_family,
    bind_addr->ai_socktype,
    bind_addr->ai_protocol
  );

  if (!ISVALIDSOCKET(socket_listen)) {
    fprintf(stderr, "socket() failed. [Err: %d]\n", GETSOCKERRORNO());
    exit(EXIT_FAILURE);
  }


  /* D. Bind the socket with address from getaddrinfo() */
  if (bind(socket_listen, bind_addr->ai_addr, bind_addr->ai_addrlen)) {
    fprintf(stderr, "bind() failed. [Err: %d]\n", GETSOCKERRORNO());
    exit(EXIT_FAILURE);
  }
  freeaddrinfo(bind_addr);


  /* E. Start listening for connection */
  if (listen(socket_listen, CONN_QUEUE_LIMIT) < 0) {
    fprintf(stderr, "listen() failed. [Err: %d]\n", GETSOCKERRORNO());
    exit(EXIT_FAILURE);
  }


  /* F. Accept any incoming connection */
  puts("[+] Watiting for connections...");

  // Upon hitting accept(), the program is blocked until a connection is made
  // to listening socket. When connection is made, accept() returns new socket.
  // New socket is used to send & recieve data with that client.
  // the original socket(socket_listen) continue to listen for new connection.
  // accept() also fills address info of client into passed in struct (sockaddr_storage)
  struct sockaddr_storage client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  SOCKET socket_client = accept(
    socket_listen,
    (struct sockaddr*) &client_addr,
    &client_addr_len
  );

  if (!ISVALIDSOCKET(socket_client)) {
    fprintf(stderr, "accept() failed. [Err: %d]\n", GETSOCKERRORNO());
    exit(EXIT_FAILURE);
  }

  // log the connected clients
  char address_buf[100];
  getnameinfo(
    (struct sockaddr*) &client_addr,
    client_addr_len,
    address_buf,
    sizeof(address_buf),
    0,
    0,
    NI_NUMERICHOST                      // specify hostname as IP address
  );
  fprintf(stdout, "[+] Connected to [%s]\n", address_buf);


  /* G. Receive date/request from the client */
  char request[1024];

  // Since it's a web server, the client (client's browser) will sent HTTP request
  // This server has to receive this request and to sent a response

  // recv() returns the no of bytes received
  // recv() blocks the program until it recive something
  // recv() return 0 or -1, if the connection is closed by client
  int bytes_received = recv(socket_client, request, 1024, 0);
  if (bytes_received <= 0) {
    fprintf(stderr, "recv() failed. [Err: %d]\n", GETSOCKERRORNO());
    exit(EXIT_FAILURE);
  }
  fprintf(stdout, "REQUEST: Received %d bytes.\n", bytes_received);
  fprintf(stdout, "REQUEST: %.*s \n", bytes_received, request);


  /* H. Sent data/response to the client */
  const char *response = 
    "HTTP/1.1 200 OK\r\n"                      // request is OK
    "Connection: close\r\n"                    // close the connection after getting all data
    "Content-Type: text/plain\r\n\r\n"         // date will be of plain text
    "Local time: ";
  int bytes_sent = send(socket_client, response, strlen(response), 0);
  fprintf(stdout, "RESPONSE: Send %d of %d bytes.\n", bytes_sent, (int)strlen(response));

  // get the current local time and sent to the client
  time_t timer;
  time(&timer);
  char *curr_time = ctime(&timer);
  bytes_sent = send(socket_client, curr_time, strlen(curr_time), 0);
  fprintf(stdout, "RESPONSE: Send %d of %d bytes.\n", bytes_sent, (int)strlen(curr_time));


  /* I. Close the client connection (to indicate server send all the data) */
  fprintf(stdout, "[+] Close client connection [%s]\n", address_buf);

  // If the connection is not closed, the client(browser) will wait for more
  // date until it times out. At this point the accept() again blocks the
  // program and waits for new connection.
  CLOSESOCKET(socket_client);

  fprintf(stdout, "[+] Close server.\n");
  CLOSESOCKET(socket_listen);


  /* J. Perform cleanup (applicable to Winsock only) */
  #if defined(_WIN32)
  WSACleanup();
  #endif

  return EXIT_SUCCESS;
}