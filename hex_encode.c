/* hex_encode.c - Binary to hexadecimal encoding routine
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
#include "hex.h"

const char hex_digits[16] = "0123456789abcdef";

void hex_encode(const unsigned char* bin, unsigned long len, char* hex)
{
  while (len--) {
    unsigned char ch = *bin++;
    *hex++ = hex_digits[(ch >> 4) & 0xf];
    *hex++ = hex_digits[ch & 0xf];
  }
  *hex = 0;
}
