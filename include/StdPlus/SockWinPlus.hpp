#pragma once
#pragma comment(lib, "ws2_32.lib")

#include "SockSharedPlus.hpp"
#include <windows.h>
#include <winsock.h>

namespace stdplus
{    
    //static int sl_initialized = 1;    

    // emulate BSD inet_aton for win32
    inline static int inet_aton(const char *name, struct in_addr *addr)
    {
        unsigned long a;

        a = inet_addr(name);
        addr->s_addr = a;

        return (a != (unsigned long)-1);
    }


    // initialize network subsystem
    inline int sl_init(void)
    {
        AFUN;

        WSADATA data;
        WORD ver;

//         if (sl_initialized)
//             return SL_ERROR_ALREADY_INIT;

        ver = 0x0202;

        if (WSAStartup(ver, &data))
            return SL_ERROR_INIT;

//        sl_initialized = 1;

        return SL_SUCCESS;
    }

    // finalize network subsystem
    inline void sl_term(void)
    {
        AFUN;

        WSACleanup();
//        sl_initialized = 0;
    }

    // get extra error code (useful for win32 WSA functions)
    inline int sl_get_last_error()
    {
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        return WSAGetLastError();
    }

    // make server TCP/IP socket
    inline int sl_make_server_socket_ex(const char *host_ip, int port, int backlog)
    {
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        // socket(...)
        int sock = (int)socket(AF_INET, SOCK_STREAM, 0); // get socket

        if (sock < 0)
            return SL_ERROR_SOCKET;
        
        // bind(...)
        struct sockaddr_in saddr = { 0 }; // address of socket
        struct in_addr iaddr;
        if (inet_aton(host_ip, &iaddr) == 0)
        {
            closesocket(sock);
            return SL_ERROR_ADDR;
        }

        memcpy((void*)&saddr.sin_addr, (const void*)&iaddr, (size_t) sizeof(iaddr));

        saddr.sin_port = htons((unsigned short)port);
        saddr.sin_family = AF_INET;
        if (bind(sock, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
        {
            closesocket(sock);
            return SL_ERROR_BIND;
        }

        // listen(...)
        if (listen(sock, backlog) != 0)
        {
            closesocket(sock);
            return SL_ERROR_LISTEN;
        }

        return sock; // return valid server socket for "accept(...)"
    }

    // make server TCP/IP socket (simple)
    inline int sl_make_server_socket(int port)
    {
        return sl_make_server_socket_ex("0.0.0.0", port, 1);
    }

    // make client TCP/IP socket
    inline int sl_connect_to_server(const char *host, int port)
    {
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        // socket(...)
        int sock = (int)socket(AF_INET, SOCK_STREAM, 0); // get socket
        if (sock < 0)
            return SL_ERROR_SOCKET;

        // connect(...)
        struct sockaddr_in saddr = { 0 };
        struct hostent * hp = gethostbyname(host);
        if (hp == NULL)
        {
            unsigned ip_addr = inet_addr(host);

            if (ip_addr == INADDR_NONE)
            {
                closesocket(sock);
                return SL_ERROR_RESOLVE;
            }
            else
            {
                saddr.sin_addr.s_addr = ip_addr;
            }
        }
        else
        {
            memcpy((void*)&saddr.sin_addr, (const void*)hp->h_addr, (size_t)hp->h_length);
        }

        saddr.sin_port = htons((unsigned short)port);
        saddr.sin_family = AF_INET;

        if (connect(sock, (struct sockaddr *) &saddr, sizeof(saddr)) != 0)
        {
            closesocket(sock);
            return SL_ERROR_CONNECT;
        }

        return sock;
    }

    // close socket
    inline int sl_disconnect(int fd)
    {
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        int ret = shutdown(fd, SHUT_RDWR);

        if (ret != 0)
            return SL_ERROR_DISCONNECT;

        ret = closesocket(fd);

        if (ret != 0)
            return SL_ERROR_DISCONNECT;

        return SL_SUCCESS;
    }

    // select wraper for non block read and accept (return 0:false, 1:true, -1:error)
    inline int sl_select(int fd, int msec)
    {
        fd_set fds;
        struct timeval to, *pto;

        while (true)
        {
            FD_ZERO(&fds);

            FD_SET((SOCKET)fd, &fds);

            if (msec >= 0)
            {
                to.tv_sec = msec / 1000;
                to.tv_usec = (msec % 1000) * 1000;
                pto = &to;
            }
            else // if msec < 0 then wait forewer
            {
                pto = (struct timeval *) NULL;
            }

            if (select(fd + 1, &fds, NULL, NULL, pto) < 0)
                return SL_ERROR_SELECT;

            break;
        } 

        if (FD_ISSET(fd, &fds))
            return 1; // may non block read

        return 0; // empty
    }
    
    // accept wrapper (return file descriptor or -1 on error)
    inline int sl_accept(int server_socket, unsigned *ipaddr, int timeout_ms = -1)
    {
        if (timeout_ms >= 0)
        {
            unsigned long NOT_BLOCKING = 1;
            int result = ioctlsocket(server_socket, FIONBIO, &NOT_BLOCKING);

            if (result < 0)
                return SL_ERROR_NOT_BLOKING_INIT;

            result = sl_select(server_socket, timeout_ms);
            if (result < 0)
                return result;
        }

//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        struct sockaddr addr;
        socklen_t addrlen = sizeof(struct sockaddr);

        while (true)
        {
            int fd = (int)accept(server_socket, &addr, &addrlen);

            if (fd < 0)
                return SL_ERROR_ACCEPT; 

            *ipaddr = (unsigned)(((struct sockaddr_in *)&addr)->sin_addr.s_addr);

            return fd; // success, client connected
        }
    }

    // read wraper
    inline int sl_read(int fd, void *buf, int size)
    {
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        if (size > 0)
        {
            while (true)
            {
                size = recv(fd, (char *)buf, size, 0);

                if (size < 0)
                    return SL_ERROR_READ;

                return size;
            } 
        } 

        return 0;
    }

    // read `size` bytes from stream `fd` to `buf` at once
    inline int sl_read_all(int fd, void *buf, int size)
    {
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        int cnt = 0;
        char *ptr = (char*)buf;

        while (size > 0)
        {
            int retv = recv(fd, ptr, size, 0);

            if (retv < 0)
                return SL_ERROR_READ;

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
        int cnt = 0;
        char *ptr = (char*)buf;

//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        while (size > 0)
        {
            int retv = sl_select(fd, ms);

            if (retv < 0)
                return SL_ERROR_SELECT;

            if (retv == 0)
                return SL_TIMEOUT;

            retv = recv(fd, ptr, size, 0);

            if (retv < 0)
                return SL_ERROR_READ;

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
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        int cnt = 0;
        char *ptr = (char*)buf;

        while (size > 0)
        {
            int retv = send(fd, (const char *)ptr, size, 0);

            if (retv < 0)
                    return SL_ERROR_WRITE;

            ptr += retv;
            cnt += retv;
            size -= retv;
        }
        return cnt;
    }

    // make server UDP socket
    inline int sl_udp_make_server_socket(int port)
    {

//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        int sock = (int)socket(AF_INET, SOCK_DGRAM, 0);

        if (sock < 0)
            return SL_ERROR_SOCKET;

        struct sockaddr_in saddr = { 0 };
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons((unsigned short)port);
        saddr.sin_family = AF_INET;

        if (bind(sock, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)
        {
            closesocket(sock);
            return SL_ERROR_BIND;
        }

        return sock;
    }

    // make client UDP socket
    inline int sl_udp_make_client_socket()
    {
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        int ret = (int)socket(AF_INET, SOCK_DGRAM, 0);
        if (ret < 0)
            return SL_ERROR_SOCKET;
        else
            return ret;
    }

    // read datagram from UDP socket (blocked)
    inline int sl_udp_read(int fd, void *buf, int size, unsigned *ipaddr)
    {
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        struct sockaddr_in client;
        int len = sizeof(client);

        int ret = recvfrom(fd, (char *)buf, size, 0, (struct sockaddr *)&client, (socklen_t *)&len);

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
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        int ret = sl_select(fd, ms);

        if (ret < 0)
            return SL_ERROR_SELECT;

        else if (ret == 0)
            return SL_TIMEOUT;

        struct sockaddr_in client;
        int len = sizeof(client);

        ret = recvfrom(fd, (char *)buf, size, 0, (struct sockaddr *)&client, (socklen_t *)&len);

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
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;
        
        struct sockaddr_in to_addr = { 0 };
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons((unsigned short)port);
        to_addr.sin_addr.s_addr = ipaddr;

        int ret = sendto(fd, (const char *)buf, size, 0, (struct sockaddr *)&to_addr, sizeof(to_addr));

        if (ret < 0)
            return SL_ERROR_WRITE;

        return ret;
    }

    // send datagram to peer via UDP to host
    inline int sl_udp_sendto_addr(int fd, const char *host, int port, const void *buf, int size)
    {
//         if (!sl_initialized)
//             return SL_ERROR_NOTINIT;

        unsigned ip_addr = inet_addr(host);

        if (ip_addr == INADDR_NONE)
            return SL_ERROR_RESOLVE;
        
        struct sockaddr_in to_addr = { 0 };
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons((unsigned short)port);
        to_addr.sin_addr.s_addr = inet_addr(host);

        int ret = sendto(fd, (const char *)buf, size, 0, (struct sockaddr *)&to_addr, sizeof(to_addr));

        if (ret < 0)
            return SL_ERROR_WRITE;

        return ret;
    }

    // convert dotted ipaddr to numeric
    inline unsigned sl_inet_aton(const char *s)
    {
        struct in_addr iaddr = { 0 };
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
