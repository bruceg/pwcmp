#ifndef PW_CMP__H__
#define PW_CMP__H__

extern int pwcmp_start(const char* module);
extern int pwcmp_check(const char* plaintext, const char* encoded);
extern void pwcmp_stop(void);

#endif
