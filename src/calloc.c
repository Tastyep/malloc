/*
** calloc.c for Malloc in /home/fillon_g/projects/tek2/unix/malloc/src
**
** Made by guillaume fillon
** Login   <fillon_g@epitech.net>
**
** Started on  Thu Feb  6 14:59:18 2014 guillaume fillon
** Last update Sun Feb 16 20:58:13 2014 luc sinet
*/

#include <string.h>
#include <errno.h>
#include "malloc.h"

void		*calloc(size_t nmemb, size_t size)
{
  size_t	bytes;
  void		*mem;

  bytes = nmemb * size;
  if (size != 0 && bytes / size != nmemb)
    {
      errno = ENOMEM;
      return (NULL);
    }
  if ((mem = malloc(bytes)) == NULL)
    {
      errno = ENOMEM;
      return (NULL);
    }
  return (memset(mem, 0, bytes));
}
