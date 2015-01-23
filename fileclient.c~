/*****************************************************************************/
/* Copyright (C) SSE-USTC, 2014-2015                                         */
/*                                                                           */
/*  FILE NAME             :  fileclient.c                                    */
/*  PRINCIPAL AUTHOR      :  SA14226208                                      */
/*  LANGUAGE              :  C                                               */
/*  TARGET ENVIRONMENT    :  ANY                                             */
/*  DATE OF FIRST RELEASE :  2015/01/21                                      */
/*  DESCRIPTION           :  client for file transfer                        */
/*****************************************************************************/

#include"file-stream.h"
#include"file-transfer.h"

void main()
{
    char           filename[20];        /* file name string */
    int            fd;                  /* client socket fd */

    /* input filename */
    printf("input file name:\n");
    scanf("%s", filename);

    fd = open_client();
		
    tcp_file_get(fd, filename);

    //udp_file_get(fd, filename);
}
