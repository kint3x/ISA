/*
*	Implementácia POP3 klienta
*	Predmet: ISA
*	Autor: Martin Matějka <xmatej55@stud.fit.vutbr.cz>
*	Ročník: 3BIT
*
*/

/*
* Hlavičkový súbor obsahuje všetky chybové kódy použité v programe
*/
#define _MISSING_HOSTNAME_OR_IP 9
#define _MISSING_PARAMETER 10
#define _CANNOT_OPEN_FOLDER 11
#define _BAD_PARAMETER_COMBINATION 12

#define _FILE_FAILURE 29
#define _FILE_NOT_EXIST 30
#define _FILE_TOO_BIG 31
#define _BAD_AUTHFILE_FORMAT 32


#define _BAD_USERNAME_OR_PASS 48
#define _REPLAY_BAD 49
#define _CONNECTION_CLOSED 50
#define _CONNECTION_FAILED 51
#define _CONNECTION_FAILED_TO_OPEN 52
#define _BAD_CERTFILE 53
#define _CANT_LOAD_CERT 54
#define _FAILED_VALIDATE_CERT 55