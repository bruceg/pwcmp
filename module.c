/* pwcmp/module.c - Password comparison module base code
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
#include <unistd.h>
#include "module.h"

static char buf[4096];
static unsigned buflen;
static unsigned bufpos;
static const char* plain;
static const char* encod;

static int do_read(void)
{
  if (bufpos) {
    memmove(buf, buf+bufpos, buflen-bufpos);
    buflen -= bufpos;
  }
  while (buflen < sizeof buf) {
    const char* tmp;
    unsigned long rd;
    if (buflen) {
      if ((tmp = memchr(buf, 0, buflen)) != 0) {
	unsigned len;
	plain = buf;
	encod = tmp + 1;
	len = encod - buf;
	if ((tmp = memchr(encod, 0, buflen - len)) != 0) {
	  ++tmp;
	  bufpos = tmp - buf;
	  return 1;
	}
      }
    }
    if ((rd = read(0, buf + buflen, sizeof buf - buflen)) == 0 ||
	rd == (unsigned long)-1)
      break;
    buflen += rd;
  }
  return 0;
}

int main(void) 
{
  while (do_read()) {
    char tmp[1];
    tmp[0] = pwcmp_check(plain, encod) ? 1 : 0;
    if (write(1, tmp, 1) != 1) break;
  }
  return 1;
}
