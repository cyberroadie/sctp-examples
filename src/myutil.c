#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/sctp_uio.h>
#include "util.h"

char *program_name;

void error( int status, int err, char *fmt, ... ) {
    fprintf( stderr, "%s: ", program_name );
    if ( err )
        fprintf( stderr, ": %s (%d)\n", strerror( err ), err );
    if ( status )
        exit( status );
}

void createAddress(char *hname, char *sname, struct sockaddr_in *sap, char *protocol) {
    struct servent *sp;
    struct hostent *hp;
    char *endptr;
    short port;

    bzero( sap, sizeof( *sap ) );
    sap->sin_family = AF_INET;
    if ( hname != NULL )
    {
        if ( !inet_aton( hname, &sap->sin_addr ) )
        {
            hp = gethostbyname( hname );
            if ( hp == NULL )
                error( 1, 0, "unknown host: %s\n", hname );
            sap->sin_addr = *( struct in_addr * )hp->h_addr;
        }
    }
    else
        sap->sin_addr.s_addr = htonl( INADDR_ANY );
    port = strtol( sname, &endptr, 0 );
    if ( *endptr == '\0' )
        sap->sin_port = htons( port );
    else
    {
        sp = getservbyname( sname, protocol );
        if ( sp == NULL )
            error( 1, 0, "unknown service: %s\n", sname );
        sap->sin_port = sp->s_port;
    }
}

void createInitMsg(struct sctp_initmsg *initmsg,
                   u_int16_t num_ostreams,
                   u_int16_t max_instreams,
                   u_int16_t max_attempts,
                   u_int16_t max_init_timeo) {

    bzero(initmsg, sizeof(*initmsg));
    initmsg->sinit_num_ostreams = num_ostreams;
    initmsg->sinit_max_instreams = max_instreams;
    initmsg->sinit_max_attempts = max_attempts;
    initmsg->sinit_max_init_timeo = max_init_timeo;

}

void createSndRcvInfo(struct sctp_sndrcvinfo *sinfo,
                      uint32_t ppid,
                      uint16_t flags,
                      uint16_t stream_no,
                      uint32_t timetolive,
                      uint32_t context) {
    bzero(sinfo, sizeof(*sinfo));
    sinfo->sinfo_ppid = ppid;
    sinfo->sinfo_flags = flags;
    sinfo->sinfo_stream = stream_no;
    sinfo->sinfo_timetolive = timetolive;
    sinfo->sinfo_context = context;
// TODO Research what they do and add
//    sinfo->sinfo_cumtsn
//    sinfo->sinfo_keynumber
//    sinfo->sinfo_keynumber_valid
//    sinfo->sinfo_ssn
//    sinfo->sinfo_tsn

}

void createMessageHdr(struct msghdr *outmsghdr,
                      struct sctp_initmsg *initmsg,
                      struct sctp_sndrcvinfo *sinfo,
                      struct sockaddr *to,
                      socklen_t tolen,
                      const void *msg,
                      size_t len) {



    char cmsgbuf[CMSG_SPACE(sizeof(struct sctp_sndrcvinfo))];
    struct cmsghdr *cmsg;

    // server address
    outmsghdr->msg_name = to;
    outmsghdr->msg_namelen = tolen;

    // message
    struct iovec iov;
    outmsghdr->msg_iov = &iov;
    iov.iov_base = (void *) msg;
    iov.iov_len = 1;

    outmsghdr->msg_control = cmsgbuf;
    outmsghdr->msg_controllen = sizeof(cmsgbuf);
    outmsghdr->msg_flags = 0;

    cmsg = CMSG_FIRSTHDR(outmsghdr);
    cmsg->cmsg_level = IPPROTO_SCTP;
    cmsg->cmsg_type = SCTP_SNDRCV;
    cmsg->cmsg_len = CMSG_LEN(sizeof(struct sctp_sndrcvinfo));

    outmsghdr->msg_controllen = cmsg->cmsg_len;
    memcpy(CMSG_DATA(cmsg), sinfo, sizeof(struct sctp_sndrcvinfo));

}
