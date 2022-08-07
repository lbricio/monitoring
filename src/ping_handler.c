/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 20:33:16 by lbricio-          #+#    #+#             */
/*   Updated: 2022/08/03 13:48:29 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/monitoring.h"

void print_ping_output(struct services *ptr)
{
		char status[5];
		printf("%-26s",ptr->name);
		if (ptr->ping_latency[0] == 0)
		{
			printf(RED);
			strcpy(ptr->status, "down");
			printf("%-6s\n", ptr->status);
		}
		else if (atoi(ptr->ping_latency) < atoi(ptr->max_timeout))
		{
			printf(GREEN);
			strcpy(ptr->status, "good");
			printf("%-6s", ptr->status);
			printf("%s ms\n", ptr->ping_latency);
		}
		else
		{
			printf(YELLOW);
			strcpy(ptr->status, "bad");
			printf("%-6s", ptr->status);
			printf("%s ms\n", ptr->ping_latency);
		}
		printf(DEFAULT);
}

void pefform_ping(struct services *ptr)
{
	int fd[2];
	pid_t pid;
	
	pipe(fd);
	pid = fork();
    if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
    	execl(PING_PATH, "ping", ptr->address, "-c1", "-n", "-t2", NULL);
		exit(1);
	}
    else
    {
		int symbol = 0;
		int j = 0;
		time_t t;
		FILE *fptr;
		char ch;

		close(fd[1]);
		waitpid(pid, NULL, 0);
		ptr->ping_latency[0] = 0;
		
		fptr = fopen("log/ping.log", "a");
		time(&t);
		fprintf(fptr, "%s", ctime(&t));
		while (read(fd[0], &ch, 1) == 1)
        {
			if (ch != '\n')
				fprintf(fptr,"%c",ch);
			else
				fprintf(fptr,", ");
			if (ch == '=')
				symbol++;
			if (symbol == 3)
			{
				read(fd[0], &ch, 1);
				while (ch != '.' && ch != ' ' && j <= 5)
				{
					fprintf(fptr,"%c",ch);
					ptr->ping_latency[j++] = ch;
					read(fd[0], &ch, 1);
				}
				break;
			}
			if (ch == EOF)
				break;
        }
		fprintf(fptr," ms\n");
		fclose(fptr);
		close(fd[0]);
		print_ping_output(ptr);
    }
}
