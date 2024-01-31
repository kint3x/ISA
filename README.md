# Popcl

## Overview
`Popcl` is a program designed to download emails using the `POP3` protocol from a server.

## Usage Instructions

To execute the program, use the following command:

```bash
popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]] [-d] [-n] \\ -a <auth_file> -o <out_dir>
```
<server>: Mandatory. Specifies the server's IP address or hostname.

<port>: Optional. The port number where the POP3 server is listening.

-T: Optional. Switch for establishing an encrypted connection.

-S: Optional. Switch for starting an unencrypted connection and then switching to encrypted.

-d: Optional. Switch that deletes the message after downloading.

-n: Optional. Switch for downloading only new messages.

-a <auth_file>: Mandatory. Specifies the file with login credentials in the format:

```text
username=<username>
password=<password>
```

-o <out_dir>: Mandatory. Specifies the folder for storing emails.


## Author

Martin Matějka <xmatej55@stud.fit.vutbr.cz>
Brno University of Technology, Faculty of Information Technology 

