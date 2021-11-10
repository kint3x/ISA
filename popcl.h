/*
*	Implementácia POP3 klienta
*	Predmet: ISA
*	Autor: Martin Matějka <xmatej55@stud.fit.vutbr.cz>
*	Ročník: 3BIT
*
*/
#ifndef _POPCL_H
#define _POPCL_H
#include "connect.h"

int popcl_client();
int popcl_client_unsecure();
int popcl_login();
int parse_stats(char *buf,int *msg_count,int *total_size);
int download_emails(BIO *bio);
int get_request_reply(char *buf,BIO * bio,int buff_size);
int download_single_email(int num,char *message,int message_size,int spacing,BIO * bio);
int check_if_file_exists(char *file);
int popcl_client_secure();
int cut_buffer_lines(char *buf);
void remove_character(char *buf);
int popcl_client_starttls();
int starttls(BIO * bio);
#endif