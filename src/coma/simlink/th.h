/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009  The Microgrid Project.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/
#ifndef _TH_H
#define _TH_H

#include <pthread.h>

#define USE_IPC_SEMS
#ifdef USE_IPC_SEMS
#include <sys/sem.h>
#include <cstdio>
#include <cstdlib>
#include <sys/errno.h>
     union semun {
             int     val;            /* value for SETVAL */
             struct  semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
             u_short *array;         /* array for GETALL & SETALL */
     };

#define sem_init(Sem, Shared, Val) do {					\
    if (-1 == (*(Sem) = semget(IPC_PRIVATE, 1, 0600|IPC_CREAT))) { perror("semget"); abort(); } \
    union semun arg; arg.val = (Val);					\
    if (-1 == semctl(*(Sem), 0, SETVAL, arg)) { perror("semctl"); abort(); } \
  } while(0)
#define sem_post(Sem) do {					\
    struct sembuf sop = { 0, 1, 0 };					\
    if (-1 == semop(*(Sem), &sop, 1)) { if (errno == EIDRM) pthread_exit(0); perror("semop"); abort(); } \
  } while(0)
#define sem_wait(Sem) do {	  \
    struct sembuf sop = { 0, -1, 0 };					\
    if (-1 == semop(*(Sem), &sop, 1)) { if (errno == EIDRM) pthread_exit(0); perror("semop"); abort(); } \
  } while(0)
#define sem_destroy(Sem) do {	 \
    semctl(*(Sem), IPC_RMID, 0); \
  } while(0)
typedef int sem_t;
#else
#include <semaphore.h>
#endif


typedef struct _th_t{
    int argc;
    char** argv;
    int* pmtcmd;

	sem_t sem_sync;
	sem_t sem_mgs;

    bool bterm;
} th_t;

extern th_t thpara;
#endif

