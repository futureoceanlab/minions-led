#ifndef SYNC_H
#define SYNC_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <signal.h>
#include <time.h>

#define BILLION 1000000000LL
#define NUM_AVG 100
#define PORT 8080 
#define SERVER_IP "192.168.4.1"

struct timeinfo
{
    long long T_skew_n;
    long long T_start_n;
};

long long as_nsec(struct timespec *T);
long long bytes_to_nsec(char *buffer);
void as_timespec(long long t, struct timespec *T);
int synchronize(struct timeinfo* TI);
int get_skew(struct timeinfo* TI);

#endif
