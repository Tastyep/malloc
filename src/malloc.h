/*
** malloc.h for malloch in /home/sinet_l/Documents/project/PSU_2013_malloc/src
**
** Made by luc sinet
** Login   <sinet_l@epitech.net>
**
** Started on  Mon Feb  3 16:22:49 2014 luc sinet
** Last update Sun Feb 16 17:21:56 2014 luc sinet
*/

#ifndef _MALLOC_H_
# define _MALLOC_H_

# include <unistd.h>

# define BLOCK_SIZE sizeof(struct s_block)
# define NB_PAGE 8

# ifdef __x86_64__
#  define ALIGN_SIZE 8
# else
#  define ALIGN_SIZE 4
# endif

# define ALIGN(x, size) ((x + size - 1) &~ (size - 1))

typedef struct	s_block	*t_block;

struct		s_block
{
  size_t	size;
  size_t	rsize;
  t_block	next;
  t_block	prev;
  char		free;
};

void	*malloc(size_t size);
void	free(void *ptr);
void	*calloc(size_t nmemb, size_t size);
void	*realloc(void *ptr, size_t size);
void	split_block(t_block block, size_t size);
void	show_alloc_mem();

/*
** thread safe functions
*/

void	malloc_thread_lock();
void	malloc_thread_unlock();
void	realloc_thread_lock();
void	realloc_thread_unlock();

#endif /* _MALLOC_H_ */
