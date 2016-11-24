//
//  main.c
//  Test-Redis
//
//  Created by Vicente Cubells Nonell on 11/08/16.
//  Copyright © 2016 Vicente Cubells Nonell. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hiredis.h"

#include <omp.h>

#define N_HILOS 100
#define N_INSERT 1000000

int main(int argc, char **argv) {
    unsigned int j;
    int tid;
    long int number;
    
    srand(time(NULL));
    
    const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";
    
    int port = (argc > 2) ? atoi(argv[2]) : 6379;
    
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds

#pragma omp parallel private(j,tid, number) num_threads(N_HILOS)
{
    int tid = omp_get_thread_num();
    
    redisContext *c;
    redisReply *reply;
    
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }
    
    /* Set a key */
    
    for (j = 0; j < N_INSERT; ++j) {
        number = rand() % N_INSERT + 1;
        number *= tid;
        
        reply = redisCommand(c,"SET %d %ld", number + j, number);
        freeReplyObject(reply);
    }

    
    /* Disconnects and frees the context */
    redisFree(c);
}
    
    return 0;
}
