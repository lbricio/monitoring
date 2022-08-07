/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:58:48 by lbricio-          #+#    #+#             */
/*   Updated: 2022/08/03 15:14:03 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/monitoring.h"

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data)
{
	size_t index = data->size;
	size_t n = (size * nmemb);
	char* tmp;

	data->size += (size * nmemb);
	tmp = realloc(data->data, data->size + 1);
	if (tmp) {data->data = tmp;}
	else 
	{ 
		if (data->data) {free(data->data); }
		fprintf(stderr, "Failed to allocate memory.\n");
		return 0;
	}
	memcpy((data->data + index), ptr, n);
	data->data[data->size] = '\0';
	return size * nmemb;
}

char *handle_url(struct services *ptr)
{
	CURL *curl;
	struct url_data data;
	
	data.size = 0;
	data.data = malloc(1024);
	if (NULL == data.data)
	{
		fprintf(stderr, "Failed to allocate memory.\n");
		return NULL;
	}
	data.data[0] = '\0';
	CURLcode res;
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, ptr->protocol);
		if (*ptr->DNS)
			curl_easy_setopt(curl, CURLOPT_DNS_SERVERS, ptr->DNS);
		else if (*ptr->method)
			curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, ptr->method);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_URL, ptr->address);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 4L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		res = curl_easy_perform(curl);
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &ptr->last_response);
		curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME_T, &ptr->last_latency);
		curl_easy_cleanup(curl);

	}
	return data.data;
}

void write_log(struct services *ptr)
{
	FILE *fptr;
	time_t t;
	
	time(&t);
	printf(DEFAULT);
	if (*ptr->DNS)
	{
		fptr = fopen("log/dns.log", "a");
		fprintf(fptr, "%s", ctime(&t));
		fprintf(fptr, "DNS Proxy ");
	}
	else
	{
		fptr = fopen("log/http.log", "a");
		fprintf(fptr, "%s", ctime(&t));
		fprintf(fptr, "method: %s ", ptr->method);
	}
	fprintf(fptr, "protocol: %s ", ptr->protocol);
	fprintf(fptr, "address: %s ", ptr->address);
	fprintf(fptr, "response: %d ", ptr->last_response);
	fprintf(fptr, "expected response: %s ", ptr->expected_code);
	fprintf(fptr, "latency: %ld\n", ptr->last_latency/1000);
	fclose(fptr);
}

void print_data(char *data, struct services *ptr)
{
    char status[5];
	
    if ((atoi(ptr->expected_code) == ptr->last_response))
    {
        printf("%-26s",ptr->name);
        if ((ptr->last_latency)/1000 < atoi(ptr->max_timeout))
        {
            printf(GREEN);
            strcpy(ptr->status, "good");
        }
        else
        {
            printf(YELLOW);
            strcpy(ptr->status, "bad");
        }
        printf("%-6s", ptr->status);
	    printf("%ld ms\n", ptr->last_latency/1000);
    }
	else if (*ptr->DNS)
	{
		printf("%-26s",ptr->name);
        if ((ptr->last_latency)/1000 < atoi(ptr->max_timeout))
        {
            printf(GREEN);
            strcpy(ptr->status, "good");
        }
        else
        {
            printf(YELLOW);
            strcpy(ptr->status, "bad");
        }
        printf("%-6s", ptr->status);
	    printf("%ld ms\n", ptr->last_latency/1000);
	}
    else
    {
        printf("%-25s",ptr->name);
        printf(RED);
        strcpy(ptr->status, "bad");
        printf("unexpected %i\n", ptr->last_response);
    }
	write_log(ptr);
}
