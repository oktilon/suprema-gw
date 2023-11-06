#ifndef SERVER_H
#define SERVER_H

int server_init (bool clean, uint32_t logIndex, uint32_t logMax);
int server_run ();

#endif // SERVER_H