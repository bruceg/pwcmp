#include "installer.h"
#include "conf_bin.c"

void insthier(void) {
  int bin = opendir(conf_bin);
  c(bin, 0, "pwcmp-crypt",     -1, -1, 0755);
  c(bin, 0, "pwcmp-md5",       -1, -1, 0755);
  c(bin, 0, "pwcmp-md5-crypt", -1, -1, 0755);
  c(bin, 0, "pwcmp-plain",     -1, -1, 0755);
  c(bin, 0, "pwcmp-test",      -1, -1, 0755);
}
