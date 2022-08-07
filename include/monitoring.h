/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:58:54 by lbricio-          #+#    #+#             */
/*   Updated: 2022/08/03 15:15:05 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITORING_H
# define MONITORING_H

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define PING_PATH "/sbin/ping"

#define PURPLE "\033[0;35m"
#define DEFAULT "\033[0m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"

struct url_data
{
    size_t size;
    char* data;
};

struct services
{
	char name[32];
	char protocol[8];
	char address[64];
	char max_timeout[8];
	char expected_code[8]; //only HTTP;
	char method[8]; //only HTTP;
	char DNS[16]; //only DNS;
	
	char status[8];
	char ping_latency[8];
	int last_response;
	curl_off_t last_latency;
	
	struct services *next;
};

void print_data(char *data, struct services *ptr);
int get_config(FILE *file);
void intHandler();

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data);
char *handle_url(struct services *ptr);

int turn_config_into_structs(struct services *ptr, FILE *file ,int i);
int check_format(FILE *file);
int lines_counter(FILE *file);

void print_watch_begin();
void print_watch_end();
void print_horizontal_bar();

void pefform_ping(struct services *ptr);
void print_ping_output(struct services *ptr);

#endif
