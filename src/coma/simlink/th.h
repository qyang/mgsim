/*
mgsim: Microgrid Simulator
Copyright (C) 2006,2007,2008,2009,2010,2011  The Microgrid Project.

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

struct sem_t
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int signaled;
};

static inline void sem_init(sem_t* sem)
{
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
    sem->signaled = 0;
}

static inline void sem_post(sem_t* sem)
{
    pthread_mutex_lock(&sem->mutex);
    sem->signaled++;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}

static inline void sem_wait(sem_t* sem)
{
    pthread_mutex_lock(&sem->mutex);
    while (sem->signaled == 0)
    {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->signaled--;
    pthread_mutex_unlock(&sem->mutex);
}

static inline void sem_destroy(sem_t* sem)
{
    pthread_cond_destroy(&sem->cond);
    pthread_mutex_destroy(&sem->mutex);
}

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

