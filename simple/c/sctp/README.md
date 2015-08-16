There are two method to send and receive data over SCPT, both methods have 
an example implementations.

===sendmsg and recvmsg

According to RFC Section 3.1 the follwoing socket call 
sequence can be used for sending and receiving messages:

 A typical server in this style uses the following socket calls in
   sequence to prepare an endpoint for servicing requests:

   o  socket()

   o  bind()

   o  listen()

   o  recvmsg()

   o  sendmsg()

   o  close()

   A typical client uses the following calls in sequence to set up an
   association with a server to request services:

   o  socket()

   o  sendmsg()

   o  recvmsg()

   o  close()

Sendmsg and recmsg ae standard POSIX method calls.


=== sctp_sendv and sctp_recv

The other method, described in RFC6458 section 9.12 and 9.13 are:

   ssize_t sctp_sendv(
      int sd,
      const struct iovec *iov,
      int iovcnt,
      struct sockaddr *addrs,
      int addrcnt,
      void *info,
      socklen_t infolen,
      unsigned int infotype,
      int flags);
                      
    ssize_t sctp_recvv(
      int sd,
      const struct iovec *iov,
       int iovlen,
      struct sockaddr *from,
      socklen_t *fromlen,
      void *info,
      socklen_t *infolen,
      unsigned int *infotype,
      int *flags);   
                        
                                        
                                        