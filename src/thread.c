/*
** thread.c for Malloc in /home/fillon_g/projects/tek2/unix/malloc/src
**
** Made by guillaume fillon
** Login   <fillon_g@epitech.net>
**
** Started on  Wed Feb  5 21:39:03 2014 guillaume fillon
** Last update Mon Feb 10 23:03:04 2014 guillaume fillon
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "malloc.h"

static pthread_mutex_t g_mutex_malloc = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t g_mutex_realloc = PTHREAD_MUTEX_INITIALIZER;

void	malloc_thread_lock()
{
  if (pthread_mutex_lock(&g_mutex_malloc) != 0)
    {
      write(2, "Fatal Error: Mutex locking failed\n", 34);
      abort();
    }
}

void	malloc_thread_unlock()
{
  if (pthread_mutex_unlock(&g_mutex_malloc) != 0)
    {
      write(2, "Fatal Error: Mutex unlocking failed\n", 36);
      abort();
    }
}

void	realloc_thread_lock()
{
  if (pthread_mutex_lock(&g_mutex_realloc) != 0)
    {
      write(2, "Fatal Error: Mutex locking failed\n", 34);
      abort();
    }
}

void	realloc_thread_unlock()
{
  if (pthread_mutex_unlock(&g_mutex_realloc) != 0)
    {
      write(2, "Fatal Error: Mutex unlocking failed\n", 36);
      abort();
    }
}
