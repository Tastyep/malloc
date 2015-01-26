/*
** show_alloc_mem.c for malloc in /home/fillon_g/projects/tek2/unix/malloc/src
**
** Made by guillaume fillon
** Login   <fillon_g@epitech.net>
**
** Started on  Wed Feb  5 20:49:19 2014 guillaume fillon
** Last update Mon Feb 10 21:53:10 2014 guillaume fillon
*/

#include <stdio.h>
#include "malloc.h"

extern t_block	g_block_head;

void		show_alloc_mem()
{
  size_t	data;
  t_block	block;

  printf("break : 0x%lX\n", (size_t) sbrk(0));
  block = g_block_head;
  while (block != NULL)
    {
      data = (size_t)block + BLOCK_SIZE;
      if (!block->free)
	printf("0x%lX - 0x%lX : %lu bytes\n",
	       data,
	       data + block->rsize,
	       block->rsize);
      block = block->next;
    }
}
