/* pwcmp-md5.c - MD5 password comparison module
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
#include "crypto/md5.h"
#include "hex.h"
#include "module.h"

struct md5_ctx ctx;
char md5buf[16];
char hexbuf[33];

int pwcmp_check(const char* plain, const char* encod)
{
  md5_init_ctx(&ctx);
  md5_process_bytes(plain, strlen(plain), &ctx);
  md5_finish_ctx(&ctx, md5buf);
  hex_encode(md5buf, 16, hexbuf);
  return strcasecmp(hexbuf, encod);
}
