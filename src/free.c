/*
** free.c for free in /home/sinet_l/Documents/project/PSU_2013_malloc
**
** Made by luc sinet
** Login   <sinet_l@epitech.net>
**
** Started on  Tue Feb  4 17:42:01 2014 luc sinet
** Last update Sun Feb 16 17:16:10 2014 luc sinet
*/

#include <stdio.h>
#include <unistd.h>
#include "malloc.h"

extern t_block	g_block_head;
extern t_block	g_bfree;

int		move_break(t_block start, t_block end, size_t size)
{
  size_t	page_size;
  size_t	plage;

  if (end != NULL)
    return 0;
  page_size = sysconf(_SC_PAGESIZE);
  if (size < (plage = NB_PAGE * page_size))
    return (0);
  plage = size - (size & (page_size - 1));
  size = size & (page_size - 1);
  start->size = size;
  start->next = NULL;
  if (!g_bfree || g_bfree > start || !g_bfree->free)
    g_bfree = start;
  sbrk(-plage);
  return (1);
}

void		merge_free(t_block block)
{
  t_block	start;
  t_block	end;
  size_t	nsize;

  start = block;
  end = block;
  nsize = block->size;
  if (block->prev && block->prev->free)
    {
      start = block->prev;
      nsize += (start->size + BLOCK_SIZE);
    }
  if (block->next && block->next->free)
    {
      end = block->next;
      nsize += (end->size + BLOCK_SIZE);
    }
  if (nsize == block->size || move_break(start, (end = end->next), nsize))
    return ;
  start->size = nsize;
  start->next = end;
  if (end != NULL)
    end->prev = start;
  if (!g_bfree || !g_bfree->free || g_bfree->size < nsize)
    g_bfree = start;
}

void		free(void *ptr)
{
  t_block	block;

  malloc_thread_lock();
  if (ptr == NULL || (block = (t_block)((size_t)ptr - BLOCK_SIZE))->free)
    {
      malloc_thread_unlock();
      return ;
    }
  block->free = 1;
  merge_free(block);
  malloc_thread_unlock();
}
