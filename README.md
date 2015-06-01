# Repository for SCTP example programs and their UDP and/or TCP equivalent

## OS Support

### FreeBSD
The reference implementation of SCTP is on FreeBSD, so all programs are guarenteed to work on this OS.

### OS X
The reference implementation gets ported frequently to the latest version of Mac OS X: [install instructions](https://nplab.fh-muenster.de/wiki/pages/27J6w9D2/SCTP_on_Yosemite.html). All programs should also work on this OS.

### Linux
No guarenteed support. [SCTP Linux driver developement](https://github.com/borkmann/lksctp-tools)

### Microsoft Windows
No guarenteed support. [SCTP Windows driver development](http://www.bluestop.org/SctpDrv/)

## Example programs

### Echo: Client - Server
1. Clients send a single message 
2. Server echos message back
3. Client prints message
4. Client closes

### Multi Echo: Client - Server
Multi threaded client, keeps sending messages to server

### File Upload: Client - Server
Uploading a file.

### Multi File Upload: Client - Server
Uplaoding multiple files over multiple channels.

### Binary protocol: Client - Server
Simple binary protocol

### Streaming audio
Stream audio from single client to multiple clients via server.
