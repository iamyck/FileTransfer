/*****************************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                         */
/*                                                                           */
/*  FILE NAME             :  fileserver.c                                    */
/*  PRINCIPAL AUTHOR      :  SA14226208                                      */
/*  LANGUAGE              :  C                                               */
/*  TARGET ENVIRONMENT    :  ANY                                             */
/*  DATE OF FIRST RELEASE :  2015/01/21                                      */
/*  DESCRIPTION           :  server for file transfer                        */
/*****************************************************************************/

#include"file-stream.h"
#include"file-transfer.h"

void main()
{
    int           serverfd;       /* tcp server fd */
    int           controlfd;      /* tcp connection fd */

    serverfd = open_tcp_server();

    controlfd = wait_for_request(serverfd);

    tcp_file_send(controlfd, serverfd);

    //udp_file_send(controlfd);
}
