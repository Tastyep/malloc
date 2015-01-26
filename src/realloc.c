/*
** realloc.c for realloc in /home/sinet_l/Documents/project/PSU_2013_malloc
**
** Made by luc sinet
** Login   <sinet_l@epitech.net>
**
** Started on  Wed Feb  5 14:26:15 2014 luc sinet
** Last update Sun Feb 16 17:35:14 2014 guillaume fillon
*/

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "malloc.h"

extern t_block	g_block_head;
extern t_block	g_bfree;

void		*increase_block_size(t_block block, size_t size)
{
  size_t	page_size;

  if (size < block->size)
    {
      block->size = size;
      brk(block + BLOCK_SIZE + size);
      return ((void *)((size_t)block + BLOCK_SIZE));
    }
  page_size = NB_PAGE * sysconf(_SC_PAGESIZE);
  page_size = ALIGN(size + BLOCK_SIZE, page_size);
  if (sbrk(page_size) == (void *)-1)
    return (NULL);
  block->size = page_size - BLOCK_SIZE;
  split_block(block, size);
  return ((void *)((size_t)block + BLOCK_SIZE));
}

void		*new_block(t_block block, size_t size)
{
  t_block	nblock;
  size_t	bsize;

  bsize = ((block->size < size) ? block->size : size);
  if ((nblock = malloc(size)) == NULL)
    return (NULL);
  block = (t_block)((size_t)block + BLOCK_SIZE);
  memcpy(nblock, block, bsize);
  free(block);
  return (nblock);
}

void		*take_next_block(t_block block, t_block next, size_t size)
{
  if (g_bfree == next)
    g_bfree = NULL;
  block->size += (next->size + BLOCK_SIZE);
  block->next = next->next;
  if (next->next != NULL)
    next->next->prev = block;
  split_block(block, size);
  return ((void *)((size_t)block + BLOCK_SIZE));
}

void		*get_memblock(t_block block, size_t size, size_t rsize)
{
  t_block	next;
  void		*mem;

  next = block->next;
  if (next == NULL)
    mem = increase_block_size(block, size);
  else if (next->free && next->size + block->size + BLOCK_SIZE >= size)
    mem = take_next_block(block, next, size);
  else
    mem = new_block(block, size);
  if (mem == NULL)
    errno = ENOMEM;
  else
    ((t_block)(mem - BLOCK_SIZE))->rsize = rsize;
  return (mem);
}

void		*realloc(void *ptr, size_t rsize)
{
  void		*mem;
  size_t	size;
  t_block	block;

  if (ptr == NULL)
    return (malloc(rsize));
  if (rsize == 0 || rsize >> 63 == 1)
    {
      free(ptr);
      return (malloc(rsize));
    }
  realloc_thread_lock();
  block = (t_block)((size_t)ptr - BLOCK_SIZE);
  size = ALIGN(rsize, ALIGN_SIZE);
  if (size == block->size)
    mem = ptr;
  else
    mem = get_memblock(block, size, rsize);
  realloc_thread_unlock();
  return (mem);
}
