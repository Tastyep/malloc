/*
** malloc.c for malloc & free in /home/sinet_l/Documents/project/PSU_2013_malloc/src
**
** Made by luc sinet
** Login   <sinet_l@epitech.net>
**
** Started on  Mon Feb  3 16:22:31 2014 luc sinet
** Last update Sun Feb 16 20:59:08 2014 luc sinet
*/

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "malloc.h"

t_block		g_block_head = NULL;
t_block		g_bfree = NULL;

void		split_block(const t_block block, size_t size)
{
  t_block	nblock;
  t_block	next;

  if (block->size > ALIGN(size + BLOCK_SIZE, ALIGN_SIZE))
    {
      next = block->next;
      nblock = (t_block)((size_t)block + size + BLOCK_SIZE);
      nblock->size = block->size - size - BLOCK_SIZE;
      nblock->next = next;
      nblock->prev = block;
      if (next)
	next->prev = nblock;
      block->next = nblock;
      nblock->free = 1;
      block->size = size;
      if (!g_bfree || !g_bfree->free || g_bfree->size < nblock->size)
	g_bfree = nblock;
    }
}

t_block		create_block(t_block last, size_t size, char free)
{
  t_block	block;
  size_t	page_size;

  page_size = NB_PAGE * sysconf(_SC_PAGESIZE);
  page_size = ALIGN(size + BLOCK_SIZE, page_size);
  if ((block = (t_block)sbrk(page_size)) == (void *)-1)
    {
      block = NULL;
      return (NULL);
    }
  last->next = block;
  block->next = NULL;
  block->prev = last;
  block->size = page_size - BLOCK_SIZE;
  block->free = free;
  split_block(block, size);
  return ((void *)((size_t)block + BLOCK_SIZE));
}

void		*init_head(size_t size)
{
  long		page_size;

  page_size = NB_PAGE * sysconf(_SC_PAGESIZE);
  page_size = ALIGN(size + BLOCK_SIZE, page_size);
  if ((g_block_head = (t_block)sbrk(page_size)) == (void *)-1)
    {
      g_block_head = NULL;
      return (NULL);
    }
  g_block_head->next = NULL;
  g_block_head->prev = NULL;
  g_block_head->free = 1;
  g_block_head->size = page_size - BLOCK_SIZE;
  return ((void *)((size_t)g_block_head + BLOCK_SIZE));
}

void		*get_space(size_t size)
{
  t_block	block;
  t_block	last;

  block = g_block_head;
  if (g_bfree && g_bfree->free && g_bfree->size >= size)
    {
      block = g_bfree;
      g_bfree->free = 0;
      split_block(g_bfree, size);
      return ((void *)((size_t)block + BLOCK_SIZE));
    }
  while (block != NULL)
    {
      if (block->free && block->size >= size)
	{
	  block->free = 0;
	  split_block(block, size);
	  return ((void *)((size_t)block + BLOCK_SIZE));
	}
      last = block;
      block = block->next;
    }
  return (create_block(last, size, 0));
}

void		*malloc(size_t rsize)
{
  void		*mem;
  size_t	size;

  if (rsize >> 63 == 1)
    {
      errno = ENOMEM;
      return (NULL);
    }
  malloc_thread_lock();
  size = ALIGN(rsize, ALIGN_SIZE);
  if (g_block_head == NULL && init_head(size) == NULL)
    {
      errno = ENOMEM;
      malloc_thread_unlock();
      return (NULL);
    }
  if ((mem = get_space(size)) != NULL)
    ((t_block)(mem - BLOCK_SIZE))->rsize = rsize;
  malloc_thread_unlock();
  return (mem);
}
