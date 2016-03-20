#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* void *_t_serv(void *args) */
/* { */
/*     //Detach thread */
/*     pthread_detach(pthread_self()); */

/*     //Extract args */
/*     int fd = ((par_t *)args)->fd; */

/*     free(args);args = NULL; //Deallocate memory for argument */

/*     char in_buf[REQ_LEN] = {0}; */
/*     char out_buf[RES_LEN] = {0}; */
/*     char tmp_buf[RES_LEN] = {0}; */

/*     ssize_t count_r = recv(fd, in_buf, REQ_LEN, 0); */
/*     if (count_r < 0) { */
/*         perror ("server recv failed"); */
/*     } */
/*     else if (count_r == 0) { */
/*         perror ("cfd is closed"); */
/*     } */
/*     else if (count_r >= REQ_LEN) { */
/*         perror ("cmd too long"); */
/*     } */
/*     else { */
/*         atm_ci_t *ci = calloc(1, sizeof(atm_ci_t)); */
/*         atm_command_dissect(ci, in_buf); */
/*         if (ci->cmd_type == 0) { */
/*             atm_core_root(pids, ci, out_buf); */
/*         } */
/*         else if (ci->cmd_type == 1) {    atm_econv_ut_to_et(gbks, ci->cmd); */
/*             /\* 接收eterm原始字符串 *\/ */
/*             atm_core_ops_do(grps, pids, mpi, ci, tmp_buf); */
/*             if (strlen(tmp_buf) > 2) { */
/*                 /\* 转换成为UTF-8字符串 *\/ */
/*                 atm_econv_et_to_ut(tmp_buf, out_buf); */
/*             } */
/*             else { */
/*                 strcpy(out_buf, tmp_buf); */
/*             } */
/*         } */
/*         else if (ci->cmd_type == 2) { */
/*             atm_core_ops_mn(pids, mpi, ci, out_buf); */
/*         } */
/*         else { */
/*             strcpy(out_buf, "wrong cmd type!"); */
/*         } */

/*         ssize_t count_s = send(fd, out_buf, strlen(out_buf), 0); */

/*         if (count_s < 0) { */
/*             perror ("send_caca"); */
/*         } */

/*         free(ci);ci = NULL; */
/*     } */
/*     close(fd); */
/*     pthread_exit(0); */
/*     //return NULL; */
/* } */

int nx_server(char *port)
{
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    struct addrinfo *serv_addr;
    int s = getaddrinfo (NULL, port, &hints, &serv_addr);
    if (s != 0) {
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
        return -1;
    }

    struct addrinfo *addr;
    int sfd = -1;
    for (addr = serv_addr; addr != NULL; addr = addr->ai_next) {
        sfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sfd < 0) {
            continue;
        }

        int opt = 1;
        setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if (bind(sfd, addr->ai_addr, addr->ai_addrlen) == 0 && (listen(sfd, MAXPENDING) == 0)) {
            break;
        }

        close(sfd);
        sfd = -1;
    }

    if (addr == NULL) {
        fprintf (stderr, "Could not bind\n");
        return -1;
    }

    freeaddrinfo(serv_addr);
    return sfd;
}

int nx_accept(int sfd) {
    struct sockaddr_storage clnt_addr;

    socklen_t clnt_addr_len = sizeof(clnt_addr);

    int cfd = accept(sfd, (struct sockaddr *) &clnt_addr, &clnt_addr_len);

    return cfd;
}

int main(void)
{
  int sfd;

  sfd = nx_server("1234");

  while (1) {
    int cfd = nx_accept(sfd);
    if (cfd == -1) {
      perror("accept failed");
    }
    else {
      /* par_t *t_par = (par_t *)malloc(sizeof(par_t)); */
      /* if (t_par == NULL) { */
      /*   perror("create thread failed"); */
      /* } */
      /* else { */
      /*   t_par->fd = cfd; */
      /*   pthread_t tid;    // create thread */
      /*   int error = pthread_create(&tid, NULL, _t_serv, t_par); */
      /*   if (error != 0) { */
      /*     perror("Couldn't run thread, errno"); */
      /*   } */
      /* } */


    }
    usleep(100000);
  }
  //NOT REACHED
}
