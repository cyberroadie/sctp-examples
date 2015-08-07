#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include "util.h"

char *program_name;

void error( int status, int err, char *fmt, ... ) {
    fprintf( stderr, "%s: ", program_name );
    if ( err )
        fprintf( stderr, ": %s (%d)\n", strerror( err ), err );
    if ( status )
        exit( status );
}

void setAddress( char *hname, char *sname, struct sockaddr_in *sap, char *protocol ) {
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

