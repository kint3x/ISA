#ifndef _POPCL_H
#define _POPCL_H
#include "connect.h"

int popcl_client();
int popcl_client_unsecure();
int popcl_login();
int parse_stats(char *buf,int *msg_count,int *total_size);
int download_emails(BIO *bio);
int get_request_reply(char *buf,BIO * bio,int buff_size);
int download_single_email(int num,char *buf,int buff_size,BIO * bio);
#endif