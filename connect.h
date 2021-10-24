
#ifndef _CONNECT_H
#define _CONNECT_H

#include <string.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "headers.h"
#include "args.h"



BIO * popcl_unsecured_connect(int *error);

#endif