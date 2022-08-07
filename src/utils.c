/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:18:34 by lbricio-          #+#    #+#             */
/*   Updated: 2022/08/03 15:06:49 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/monitoring.h"

void print_watch_begin()
{
    printf("\ec");
    printf(PURPLE); 
    printf("- - - - - - - - - - - - - - - - - - - -\n");
    printf("         the watch begins!");
    printf("\n- - - - - - - - - - - - - - - - - - - -\n");
    printf(DEFAULT);
}

void print_watch_end()
{
    printf(PURPLE); 
    printf("- - - - - - - - - - - - - - - - - - - -\n");
    printf("         the watch ended!");
    printf("\n- - - - - - - - - - - - - - - - - - - -\n");
    printf(DEFAULT);
}

void print_horizontal_bar()
{
    sleep(1);
    printf("- - - - - - - - - - - - - - - - - - - -\n");
    sleep(1);
}

int turn_config_into_structs(struct services *ptr, FILE *file ,int i)
{
	char c;
    
	for (int j = 0; (c = fgetc(file)) != '\t'; j++)
		ptr[i].name[j] = c;
	for (int j = 0; (c = fgetc(file)) != '\t'; j++)
		ptr[i].protocol[j] = c;
	for (int j = 0; (c = fgetc(file)) != '\t'; j++)
		ptr[i].address[j] = c;
	if (ptr[i].protocol[0] == 'H') //HTTP find
	{	
		for (int j = 0; (c = fgetc(file)) != '\t'; j++)
			ptr[i].method[j] = c;
		for (int j = 0; (c = fgetc(file)) != '\t'; j++)
			ptr[i].expected_code[j] = c;
    	for (int j = 0; (c = fgetc(file)) != '\n'; j++)
			ptr[i].max_timeout[j] = c;
	}
    else if (ptr[i].protocol[0] == 'P') // PING find
    {
       	for (int j = 0; (c = fgetc(file)) != '\n'; j++)
			ptr[i].max_timeout[j] = c;
    }
    else if (ptr[i].protocol[0] == 'D') // DNS find
    {
        for (int j = 0; (c = fgetc(file)) != '\t'; j++)
			ptr[i].max_timeout[j] = c;
       	for (int j = 0; (c = fgetc(file)) != '\n'; j++)
			ptr[i].DNS[j] = c;
    }
    if (*ptr[i].max_timeout == '\0')
    {
        free(ptr);
        return(-1);
    }
    return(0);
}

int check_format(FILE *file)
{
	int fields;
    char method;

	fields = 0;
    rewind(file);
    for(int c; c != '\n';)
    {
        c = fgetc(file);
        while(isprint(c))
            c = fgetc(file);
        if(c != '\t')
            return (-1);
        c = fgetc(file);
        method = c;
        while(c != '\n')
        {
            while(isprint(c))
                c = fgetc(file);
            fields++;
            if (c == '\t')
                c = fgetc(file);
            else if (c == '\n')
                break;
            else
            {
                perror("monitoring.db bad format\n");
                return(-1);
            }
        }
        if(method == 'H' && fields != 5)
        {
            perror("monitoring.db bad format\n");
            return(-1);
        }
        else if(method == 'P' && fields != 3)
        {
            perror("monitoring.db bad format\n");
            return(-1);
        }
        else if(method == 'D' && fields != 4)
        {
            perror("monitoring.db bad format\n");
            return(-1);
        }
        return (0);
    }
	return (0);
}

int lines_counter(FILE *file)
{
    int lines;
    
    lines = 0;
    for (char c; (c = fgetc(file)) != EOF;)
    {
		if (c == '\n')
            lines++;
        if (c == ' ')
        {
            perror("Unexpected ' ' in monitoring.db\n");
            return(-1);
        }
    }
    if (lines == 0)
        perror("Empty monitoring.db\n");
    return (lines);
}
