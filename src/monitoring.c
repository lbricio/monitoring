/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:58:52 by lbricio-          #+#    #+#             */
/*   Updated: 2022/08/03 15:15:14 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/monitoring.h"

static volatile int g_whatching = 1;

int get_config(FILE *file)
{
    int lines_count;
    
    lines_count = lines_counter(file);
    if (lines_count == -1 || lines_count == 0)
        return(-1);
    if (check_format(file) == -1)
        return (-1);
    return (lines_count);
}

void intHandler()
{
    printf("\b\b[stop signal received]\n");
    g_whatching = 0;
}

int watch(FILE *file, struct services *ptr, int lines_count)
{
    char *data;
    printf("\e[8;30;40t");
    print_watch_begin();

    while (g_whatching == 1)
    {
        for(int arr = 0; arr < lines_count; arr++)
        {
            if (ptr[arr].protocol[0] == 'H' ) // pefform HTTP
            {
                if (g_whatching == 0) break;
                data = handle_url(&ptr[arr]);
                print_data(data, &ptr[arr]);
            }
            else if (ptr[arr].protocol[0] == 'P' ) // pefform PING
            {
                if (g_whatching == 0) break;
                pefform_ping(&ptr[arr]);
            }
            else if (ptr[arr].protocol[0] == 'D' ) // pefform DNS
            {
                if (g_whatching == 0) break;
                data = handle_url(&ptr[arr]);
                print_data(data, &ptr[arr]);
            }
        }
        if (g_whatching == 0)
            break;
        print_horizontal_bar();
	}
    free(ptr);
    print_watch_end();
    return (0);
}

FILE* open_file()
{
    FILE * file;

    file = fopen("config/monitoring.db", "r");
    if (file == NULL)
    {
        perror("Unexpected format in monitoring.db\n");
        return(NULL);
    }
    return (file);
}

int check_simplify(int ac, char **av)
{
    if (ac == 2) 
    {
        if (strcmp(av[1], "--simplify") == 0 || strcmp(av[1], "-s") == 0)
            return (1);
        perror("bad arguments\n");
        return (-1);
    }
    else if (ac > 2)
    {
        perror("bad arguments\n");
        return (-1);
    }
    return (0);
}

int main(int ac, char **av)
{
    FILE *file;
    char *line;
	int lines_count;
    int simplify;

    file = open_file();
    simplify = check_simplify(ac, av);
	lines_count = get_config(file);
	if (lines_count == -1)
        return (-1);
    rewind(file);
    
	struct services *ptr = calloc(lines_count, sizeof(struct services));
	for (int arr = 0; arr < lines_count; arr++)
		if (turn_config_into_structs(ptr, file, arr) == -1)
            return (-1);
	signal(SIGINT, intHandler);
    watch(file,  ptr, lines_count);
    return 0;
}
