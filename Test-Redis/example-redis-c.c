//
//  example-redis-c.c
//  Test-Redis
//
//  Created by Vicente Cubells Nonell on 15/08/16.
//  Copyright © 2016 Vicente Cubells Nonell. All rights reserved.
//
/*
 

 gcc example-redis-c.c -o test-redis-c  -lhiredis -I /usr/local/include/hiredis/ -pthread
 
 */

/* ./test-redis-c [HOSTNAME | IP] [PORT] [AUTH] */




#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hiredis.h"

#include <pthread.h>

#define N_THREADS 8
#define N_INSERT 1000000

const char *hostname;
int port;
const char *auth;

static void * worker (void *data) {
    
    int tid = (int) data;
    int j;
    long int number = 0;
    
    redisContext *c;
    redisReply *reply;
    
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    
     srand(time(NULL));
    
    c = redisConnectWithTimeout(hostname, port, timeout);
    
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        
        pthread_exit(NULL);
    }
    
    /* Auth */
    
    if (auth != "") {
        reply= redisCommand(c, "AUTH %s", auth);
        if (reply->type == REDIS_REPLY_ERROR) {
            /* Authentication failed */
            printf("ERROR: Auth...\n");
        }
        freeReplyObject(reply);
    }
    
    /* Set a key */
    
    int inicio = tid * N_INSERT;
    int fin = (tid + 1) * N_INSERT;
    
    for (j = inicio; j < fin; ++j) {
        number = rand() % 1000000 + 1;        
        reply = redisCommand(c,"SET %d %ld", j, number);
        if (reply->type == REDIS_REPLY_ERROR) {
            /* SET failed */
            printf("ERROR: SET %d %ld...\n", j, number);
        }
        freeReplyObject(reply);
    }
    
    
    /* Disconnects and frees the context */
    redisFree(c);
    
    pthread_exit(NULL);
}

int main(int argc, const char * argv[]) {
    
    pthread_t  threads[N_THREADS];
    
    int i = 0;
    
    srand(time(NULL));
    
    hostname = (argc > 1) ? argv[1] : "127.0.0.1";
    
    port = (argc > 2) ? atoi(argv[2]) : 6379;
    
    auth = (argc > 3) ? argv[3] : "";

    for (i = 0; i < N_THREADS; i++) {
        pthread_create (&threads[i], NULL, worker,  i);
    }
    
    for (i = 0; i < N_THREADS; i++) {
        pthread_join (threads[i], NULL);
    }
    
    return EXIT_SUCCESS;
}
