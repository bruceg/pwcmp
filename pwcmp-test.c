/* pwcmp/pwcmp-test.c - pwcmp test code
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
#include "iobuf/iobuf.h"
#include "client.h"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    obuf_putsflush(&errbuf, "usage: pwcmp-test module\n");
    return 1;
  }
  if (!pwcmp_start(argv[1])) {
    obuf_putsflush(&errbuf, "pwcmp-test: Could not start module!\n");
    return 1;
  }
  for (;;) {
    char plain[256];
    char encod[256];
    const char* resp;
    if (!ibuf_gets(&inbuf, plain, sizeof plain, '\n')) break;
    plain[inbuf.count-1] = 0;
    if (!ibuf_gets(&inbuf, encod, sizeof encod, '\n')) break;
    encod[inbuf.count-1] = 0;
    switch (pwcmp_check(plain, encod)) {
    case 0:  resp = "0\n";  break;
    case -1: resp = "-1\n"; break;
    default: resp = "1\n";
    }
    obuf_putsflush(&outbuf, resp);
  }
  pwcmp_stop();
  return 0;
}
