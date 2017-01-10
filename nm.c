/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/12 13:53:53 by aduban            #+#    #+#             */
/*   Updated: 2017/01/10 19:11:30 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nm.h"

void	print_list(t_elem *elems)
{
	t_elem *tmp;
	tmp = elems;
	while (tmp)
	{
		ft_printf("VALUE %c %s\n", tmp->type, tmp->str);
		tmp = tmp->next;
	}
}

char sect(void)
{
	return '?';
}

void	fill_list(int nsyms, int symoff, int stroff, char *ptr)
{
	int				i;
	char			*stringtable;
	struct nlist_64	*array;
	t_elem *elems = NULL;
	t_elem *elem;
	t_elem *tmp;

	array = (void*)ptr + symoff;
	stringtable = (void*)ptr + stroff;
	i = -1;
	while (++i < nsyms)
	{
		elem = malloc(sizeof(t_elem));
		//	ft_printf("%x", (uint8_t)(stringtable + array[i].n_value));
		int t = (uint8_t)(stringtable + array[i].n_type) & N_TYPE;
		if (t == N_PBUD)
			elem->type = 'U';
		else if (t == N_UNDF)
			elem->type = 'U';
		//sometimes C if value if set ( ??? )
		else if (t == N_ABS)
			elem->type = 'A';
		else if (t == N_SECT)
			elem->type = sect();
		else if (t == N_INDR)
			elem->type = 'I';
		else
		{
			elem->type = '?';
		}
		/*else
		  ft_printf(" %d", (uint8_t)(stringtable + array[i].n_sect));*/
		elem->str = stringtable + array[i].n_un.n_strx;
		elem->next = NULL;
		if (elems == NULL)
		{
			elems = elem;
			elems->next = NULL;
			elems->prev = NULL;
		}
		else
		{
			tmp = elems;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = elem;
			tmp->next->prev = tmp;
		}
	}
	print_list(elems);
}


void	get_sections(char *ptr, int ncmds, struct segment_command_64 *lc)
{

	int i = -1;
	while (++i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			ft_printf("%s ", lc->segname);
			ft_printf("%d\n", i);
		}
		lc = (void*)lc + lc->cmdsize;
	}
}

void	handle_64(char *ptr)
{
	int						ncmds;
	int						i;
	struct mach_header_64	*header;
	struct load_command		*lc;
	struct symtab_command	*sym;

	header = (struct mach_header_64 *)ptr;
	ncmds = header->ncmds;
	lc = (void *)ptr + sizeof(*header);
	i = 0;

	//	get_sections(ptr, ncmds, (struct segment_command_64*)lc);

	while (++i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command*) lc;
			fill_list(sym->nsyms, sym->symoff, sym->stroff, ptr);
			return ;
		}
		lc = (void*)lc + lc->cmdsize;
	}
}



void	nm(void *ptr)
{
	unsigned int		number;

	number = *(int *)ptr;
	if (number == MH_MAGIC_64)
	{
		handle_64(ptr);
	}
	else if (number == FAT_MAGIC || number == FAT_CIGAM)
	{
		//go_fat(ptr);
	}
	else if (!ft_strncmp(ptr, ARMAG, SARMAG))
	{
		//go_archive(ptr, name);
	}
	else
		ft_putendl("Wrong binary format");

}

int main(int ac, char **av)
{
	int			fd;
	char		*ptr;
	struct stat	buf;

	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		ft_printf("Error opening binary\n");
		exit(-1);
	}
	if (fstat(fd, &buf) < 0)
	{
		ft_printf("fstat failed\n");
		exit(-1);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE,fd, 0)) == MAP_FAILED)
	{
		ft_printf("mmap failed\n");
		return(-1);
	}
	nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		ft_printf("unmap failed\n");
		exit(-1);
	}

}
