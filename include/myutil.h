//
// Created by Olivier Van Acker on 07/08/15.
//

#ifndef SCTP_EXAMPLES_UTIL_H
#define SCTP_EXAMPLES_UTIL_H

#include <netinet/sctp_uio.h>

#define RECVBUFSIZE 100

void createAddress(char *, char *, struct sockaddr_in *, char *);
//void createAddress(char *hname, char *sname, struct sockaddr_in *sap, char *protocol);

void error( int status, int err, char *fmt, ... );

void createInitMsg(struct sctp_initmsg *initmsg,
                   u_int16_t num_ostreams,
                   u_int16_t max_instreams,
                   u_int16_t max_attempts,
                   u_int16_t max_init_timeo);

void createSndRcvInfo(struct sctp_sndrcvinfo *sinfo ,
                      uint32_t ppid,
                      uint16_t flags,
                      uint16_t stream_no,
                      uint32_t timetolive,
                      uint32_t context);

void createMessageHdrSndRcv(struct msghdr *outmsghdr,
                            struct sctp_initmsg *initmsg,
                            struct sctp_sndrcvinfo *sinfo,
                            struct sockaddr *to,
                            socklen_t tolen,
                            const void *msg,
                            size_t len);

void createMessageHdrRcv(struct msghdr *msghdr,
                         void *message,
                         size_t mlen);

#endif //SCTP_EXAMPLES_UTIL_H
