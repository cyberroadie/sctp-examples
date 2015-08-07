//
// Created by Olivier Van Acker on 07/08/15.
//

#ifndef SCTP_EXAMPLES_UTIL_H
#define SCTP_EXAMPLES_UTIL_H

void setAddress( char *hname, char *sname, struct sockaddr_in *sap, char *protocol );
void error( int status, int err, char *fmt, ... );

#endif //SCTP_EXAMPLES_UTIL_H
