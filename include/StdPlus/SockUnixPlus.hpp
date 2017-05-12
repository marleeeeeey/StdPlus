#pragma once

#include "SockSharedPlus.hpp"

#include <errno.h>      // errno
#include <sys/time.h>   // select()
#include <sys/types.h>  // socket(), setsockopt(), bind()
#include <sys/socket.h> // socket(), setsockopt(), bind(), listen(), inet_aton(), accept()
#include <unistd.h>     // gethostname(), read(), write(), {select()}
#include <netdb.h>      // gethostbyname()
#include <arpa/inet.h>  // inet_aton()
#include <netinet/in.h> // inet_aton()


#ifdef ECOS
#  include <network.h> // select() under eCos
#endif // ECOS

#ifdef SL_USE_POLL
#  include <sys/poll.h>   // poll()
#else
#  include <sys/select.h> // select() [POSIX way]
#endif // SL_USE_POLL

namespace stdplus
{    
    static int sl_initialized = 0;    
    
    // initialize network subsystem
    inline int sl_init(void)
    {
        sl_initialized = 1;
        return SL_SUCCESS;
    }

    // finalize network subsystem
    inline void sl_term(void)
    {
        sl_initialized = 0;
    }

    // get extra error code 
    inline int sl_get_last_error()
    {
        if (!sl_initialized)
            return SL_ERROR_NOTINIT;
        
        return errno;
    }

    // make server TCP/IP socket
    inline int sl_make_server_socket_ex(const char *host_ip, int port, int backlog, bool isNonBlock = false)
    {
        int sock; // socket ID
        struct sockaddr_in saddr; // address of socket
        struct in_addr iaddr;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        // socket(...)
        sock = (int)socket(AF_INET, SOCK_STREAM, 0); // get socket

        if (sock < 0)
            return SL_ERROR_SOCKET;

        if (isNonBlock)
        {
            unsigned long NOT_BLOCKING = 1;
            int result = ioctlsocket(sock, FIONBIO, &NOT_BLOCKING);

            if (result < 0)
                return SL_ERROR_NOT_BLOKING_INIT;
        }

        // bind(...)
        memset((void*)&saddr, (int)0, (size_t) sizeof(saddr)); // clear address of socket
        if (inet_aton(host_ip, &iaddr) == 0)
        {
            close(sock);
            return SL_ERROR_ADDR;
        }

        memcpy((void*)&saddr.sin_addr, (const void*)&iaddr, (size_t) sizeof(iaddr));

        saddr.sin_port = htons((unsigned short)port);
        saddr.sin_family = AF_INET;
        if (bind(sock, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
        {
            close(sock);
            return SL_ERROR_BIND;
        }

        // listen(...)
        if (listen(sock, backlog) != 0)
        {
            close(sock);
            return SL_ERROR_LISTEN;
        }

        return sock; // return vailid server socket for "accept(...)"
    }

    // make server TCP/IP socket (simple)
    inline int sl_make_server_socket(int port)
    {
        return sl_make_server_socket_ex("0.0.0.0", port, 1);
    }

    // make client TCP/IP socket
    inline int sl_connect_to_server(const char *host, int port)
    {
        int sock; // socket ID
        unsigned ip_addr;
        struct sockaddr_in saddr;
        struct hostent  *hp;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        // socket(...)
        sock = (int)socket(AF_INET, SOCK_STREAM, 0); // get socket
        if (sock < 0)
            return SL_ERROR_SOCKET;

        // connect(...)
        memset((void*)&saddr, (int)0, (size_t) sizeof(saddr)); // clear address of socket

        hp = gethostbyname(host);
        if (hp == NULL)
        {
            ip_addr = inet_addr(host);

            if (ip_addr == INADDR_NONE)
            {
                close(sock);
                return SL_ERROR_RESOLVE;
            }
            else
                saddr.sin_addr.s_addr = ip_addr;
        }
        else
        {
            memcpy((void*)&saddr.sin_addr, (const void*)hp->h_addr, (size_t)hp->h_length);
        }

        saddr.sin_port = htons((unsigned short)port);
        saddr.sin_family = AF_INET;

        if (connect(sock, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
        {
            close(sock);
            return SL_ERROR_CONNECT;
        }

        return sock;
    }

    // close socket
    inline int sl_disconnect(int fd)
    {
        int ret;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        ret = shutdown(fd, SHUT_RDWR);
        if (ret != 0)
            return SL_ERROR_DISCONNECT;

        ret = close(fd); // close file descriptor
        if (ret != 0)
            return SL_ERROR_DISCONNECT;

        return SL_SUCCESS;
    }

    // accept wrapper (return file descriptor or -1 on error)
    inline int sl_accept(int server_socket, unsigned *ipaddr)
    {
        int fd;
        struct sockaddr addr;
        socklen_t addrlen = sizeof(struct sockaddr);

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        while (1)
        {
            fd = (int)accept(server_socket, &addr, &addrlen);

            if (fd < 0)
            {
                if (errno == EINTR)
                    continue; // interrupt by signal

                return SL_ERROR_ACCEPT; // error
            }

            *ipaddr = (unsigned)(((struct sockaddr_in *)&addr)->sin_addr.s_addr);

            return fd; // success, client connected
        } // while(1)
    }

    // select wraper for non block read (return 0:false, 1:true, -1:error)
    inline int sl_select(int fd, int msec)
    {
#ifdef SL_USE_POLL

        // use poll()
        struct pollfd fds[1];
        int retv;

        while (1)
        {
            fds->fd = fd;
            fds->events = POLLIN;
            fds->revents = 0;

            retv = poll(fds, 1, msec);
            if (retv < 0)
            {
                if (errno == EINTR)
                    continue; // interrupt by signal

                return SL_ERROR_POOL; // error
            }

            break;
        } // while (1)

        if (retv > 0)
        {
            if (fds->revents & (POLLERR | POLLHUP | POLLNVAL))
                return SL_ERROR_POOL; // error

            if (fds->revents & (POLLIN | POLLPRI))
                return 1; // may non block read
        } // if (retv > 0)

        return 0; // empty

#else // !SL_USE_POLL

        // use select()
        fd_set fds;
        struct timeval to, *pto;

        while (true)
        {
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            if (msec >= 0)
            {
                to.tv_sec = msec / 1000;
                to.tv_usec = (msec % 1000) * 1000;
                pto = &to;
            }
            else // if msec < 0 then wait forewer
                pto = (struct timeval *) NULL;

            if (select(fd + 1, &fds, NULL, NULL, pto) < 0)
            {
                if (errno == EINTR)
                    continue; // interrupt by signal

                return SL_ERROR_SELECT; // error
            }
            break;
        } 

        if (FD_ISSET(fd, &fds))
            return 1; // may non block read

        return 0; // empty
#endif // !SL_USE_SELECT
    }

    // fuse select wraper (always return 1)
    inline int sl_select_fuse(int fd, int msec)
    {
        fd = fd;
        msec = msec;

        return 1;
    }

    // read wraper
    inline int sl_read(int fd, void *buf, int size)
    {
        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        if (size > 0)
        {
            while (true)
            {
                size = read(fd, buf, (size_t)size);

                if (size < 0)
                {
                    if (errno == EINTR)
                        continue; // interrupt by signal

                    return SL_ERROR_READ;
                } // if (size < 0)

                return size;
            }
        } 

        return 0;
    }

    // read `size` bytes from stream `fd` to `buf` at once
    inline int sl_read_all(int fd, void *buf, int size)
    {
        int retv, cnt = 0;
        char *ptr = (char*)buf;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        while (size > 0)
        {
            retv = read(fd, (void*)ptr, (size_t)size);

            if (retv < 0)
            {
                if (errno == EINTR)
                    continue; // interrupt by signal

                return SL_ERROR_READ;
            }
            else if (retv == 0)
                return cnt;

            ptr += retv;
            cnt += retv;
            size -= retv;
        }
        return cnt;
    }

    // read `size` bytes from stream `fd` to `buf` at once with use timeout
    inline int sl_read_all_to(int fd, void *buf, int size, int ms)
    {
        int retv, cnt = 0;
        char *ptr = (char*)buf;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        while (size > 0)
        {
            retv = sl_select(fd, ms);

            if (retv < 0)
            {
                if (errno == EINTR)
                    continue; // interrupt by signal

                return SL_ERROR_SELECT;
            }

            if (retv == 0)
                return SL_TIMEOUT;

            retv = read(fd, (void*)ptr, (size_t)size);

            if (retv < 0)
            {
                if (errno == EINTR)
                    continue; // interrupt by signal

                return SL_ERROR_READ;
            }
            else if (retv == 0)
                return cnt;

            ptr += retv;
            cnt += retv;
            size -= retv;
        }
        return cnt;
    }

    // write `size` bytes to stream `fd` from `buf` at once
    inline int sl_write(int fd, const void *buf, int size)
    {
        int retv, cnt = 0;
        char *ptr = (char*)buf;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        while (size > 0)
        {
            retv = write(fd, (void*)ptr, size);

            if (retv < 0)
            {
                if (errno == EINTR)
                    continue;
                else
                    return SL_ERROR_WRITE;
            }
            ptr += retv;
            cnt += retv;
            size -= retv;
        }
        return cnt;
    }

    // make server UDP socket
    inline int sl_udp_make_server_socket(int port)
    {
        int sock;
        struct sockaddr_in saddr;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        sock = (int)socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0)
            return SL_ERROR_SOCKET;

        memset(&saddr, 0, sizeof(saddr));
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons((unsigned short)port);
        saddr.sin_family = AF_INET;

        if (bind(sock, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)
        {
            close(sock);
            return SL_ERROR_BIND;
        }

        return sock;
    }

    // make client UDP socket
    inline int sl_udp_make_client_socket()
    {
        int ret;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        ret = (int)socket(AF_INET, SOCK_DGRAM, 0);
        if (ret < 0)
            return SL_ERROR_SOCKET;
        else
            return ret;
    }

    // read datagram from UDP socket (blocked)
    inline int sl_udp_read(int fd, void *buf, int size, unsigned *ipaddr)
    {
        int ret, len;
        struct sockaddr_in client;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        len = sizeof(client);

        ret = recvfrom(fd, buf, size, 0, (struct sockaddr *)&client, (socklen_t *)&len);

        if (ret < 0)
        {
            *ipaddr = (unsigned)-1;
            return SL_ERROR_READ;
        }

        *ipaddr = (unsigned)(((struct sockaddr_in *)&client)->sin_addr.s_addr);

        return ret;
    }

    // read datagram from UDP socket (timeout)
    inline int sl_udp_read_to(int fd, void *buf, int size, unsigned *ipaddr, int ms)
    {
        int ret, len;
        struct sockaddr_in client;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        ret = sl_select(fd, ms);
        if (ret < 0)
            return SL_ERROR_SELECT;
        else if (ret == 0)
            return SL_TIMEOUT;

        len = sizeof(client);

        ret = recvfrom(fd, buf, size, 0, (struct sockaddr *)&client, (socklen_t *)&len);

        if (ret < 0)
        {
            *ipaddr = (unsigned)-1;
            return SL_ERROR_READ;
        }

        *ipaddr = (unsigned)(((struct sockaddr_in *)&client)->sin_addr.s_addr);

        return ret;
    }

    // send datagram to peer via UDP to ip
    inline int sl_udp_sendto(int fd, unsigned ipaddr, int port, const void *buf, int size)
    {
        struct sockaddr_in to_addr;
        int ret;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        memset(&to_addr, 0, sizeof(to_addr));

        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons((unsigned short)port);
        to_addr.sin_addr.s_addr = ipaddr;

        ret = sendto(fd, buf, size, 0, (struct sockaddr *)&to_addr, sizeof(to_addr));

        if (ret < 0)
            return SL_ERROR_WRITE;

        return ret;
    }

    // send datagram to peer via UDP to host
    inline int sl_udp_sendto_addr(int fd, const char *host, int port, const void *buf, int size)
    {
        int ret;
        struct sockaddr_in to_addr;
        unsigned ip_addr;

        if (!sl_initialized)
            return SL_ERROR_NOTINIT;

        ip_addr = inet_addr(host);

        if (ip_addr == INADDR_NONE)
            return SL_ERROR_RESOLVE;

        memset(&to_addr, 0, sizeof(to_addr));

        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons((unsigned short)port);
        to_addr.sin_addr.s_addr = inet_addr(host);

        ret = sendto(fd, buf, size, 0, (struct sockaddr *)&to_addr, sizeof(to_addr));

        if (ret < 0)
            return SL_ERROR_WRITE;

        return ret;
    }

    // convert dotted ipaddr to numeric
    inline unsigned sl_inet_aton(const char *s)
    {
        struct in_addr iaddr;

        memset((void *)&iaddr, 0, sizeof(iaddr));
        inet_aton(s, &iaddr);

        return (unsigned)(iaddr.s_addr);
    }

    // convert numeric ipaddr to dotted
    inline const char *sl_inet_ntoa(unsigned ipaddr)
    {
        struct in_addr iaddr;

        iaddr.s_addr = ipaddr;

        return (const char *)inet_ntoa(iaddr);
    }

    inline unsigned sl_htonl(unsigned hostlong)
    {
        return htonl(hostlong);
    }

    inline unsigned short sl_htons(unsigned short hostshort)
    {
        return htons(hostshort);
    }

    inline unsigned sl_ntohl(unsigned netlong)
    {
        return htonl(netlong);
    }

    inline unsigned short sl_ntohs(unsigned short netshort)
    {
        return htons(netshort);
    }

}
