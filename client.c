/* pwcmp/client.c - Password comparison client library
 * Copyright (C) 2001  Bruce Guenter <bruceg@em.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "sysdeps.h"

#include "client.h"

typedef int (*pwcmpfn)(const char*, const char*);

static long pid;
static int fd0;
static int fd1;
static pwcmpfn cmpfn;


static int pwcmp_plain(const char* plaintext, const char* encoded)
{
  return strcmp(plaintext, encoded) == 0 ? 0 : 1;
}

static int pwcmp_crypt(const char* plaintext, const char* encoded)
{
  extern char* crypt(const char* key, const char* salt);
  plaintext = crypt(plaintext, encoded);
  return pwcmp_plain(plaintext, encoded);
}

static int rwrite(int fd, const char* buf, unsigned long len)
{
  while (len > 0) {
    unsigned long wr = write(fd, buf, len);
    if (wr == (unsigned long)-1) return 0;
    len -= wr;
    buf += wr;
  }
  return 1;
}

static int pwcmp_pipe(const char* plaintext, const char* encoded)
{
  char buf[1];
  if (!rwrite(fd0, plaintext, strlen(plaintext)+1)) return -1;
  if (!rwrite(fd0, encoded, strlen(encoded)+1)) return -1;
  if (read(fd1, buf, 1) != 1) return -1;
  return buf[0] == 0 ? 0 : 1;
}

static int pwcmp_start_pipe(const char* module)
{
  int pipe0[2];
  int pipe1[2];
  if (pipe(pipe0) == -1) return 0;
  if (pipe(pipe1) == -1) { close(pipe0[0]); close(pipe0[1]); return 0; }
  if ((pid = fork()) == -1) {
    close(pipe0[0]); close(pipe0[1]);
    close(pipe1[0]); close(pipe1[1]);
    return 0;
  }
  if (pid == 0) {
    close(0);
    dup2(pipe0[0], 0);
    close(1);
    dup2(pipe1[1], 1);
    close(pipe0[0]); close(pipe0[1]);
    close(pipe1[0]); close(pipe1[1]);
    execlp(module, module, 0);
    _exit(1);
  }
  else {
    close(pipe0[0]); fd0 = pipe0[1];
    close(pipe1[1]); fd1 = pipe1[0];
  }
  return 1;
}

int pwcmp_start(const char* module)
{
  if (!module || module[0] == 0 || strcmp(module, "plain") == 0)
    cmpfn = pwcmp_plain;
  else if (strcmp(module, "crypt") == 0)
    cmpfn = pwcmp_crypt;
  else {
    cmpfn = pwcmp_pipe;
    if (!pwcmp_start_pipe(module)) return 0;
  }
  return 1;
}

int pwcmp_check(const char* plaintext, const char* encoded)
{
  if (!cmpfn) return -1;
  return cmpfn(plaintext, encoded);
}

void pwcmp_stop(void)
{
  if (cmpfn == pwcmp_pipe) {
    close(fd0);
    close(fd1);
    waitpid(pid, 0, WUNTRACED);
  }
  cmpfn = 0;
}
