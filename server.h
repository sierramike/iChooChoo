#ifndef SERVER_H
#define SERVER_H

void* ServerThread(void* ptr);
bool CheckMessage (char* message, int size);
int ProcessMessage(char* message, char* buffer_out);

#endif /* SERVER_H */