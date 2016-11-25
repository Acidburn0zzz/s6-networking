/* ISC license. */

#ifndef SBEARSSL_H
#define SBEARSSL_H

#include <sys/types.h>
#include <bearssl.h>
#include <skalibs/buffer.h>
#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>
#include <skalibs/tai.h>

 /*
  * Support library for bearssl.
  * Provides types for relocatable objects (instead of pointers,
  * indices, with storage in a stralloc), conversion functions
  * from/to native bearssl types, and a higher-level API to
  * read secret keys / certificate chains / trust anchors from
  * the filesystem.
  */


 /* Utility functions */

extern int sbearssl_isder (unsigned char const *, size_t) ;


 /* Certificates (x509-encoded) */

typedef struct sbearssl_cert_s sbearssl_cert, *sbearssl_cert_ref ;
struct sbearssl_cert_s
{
  size_t data ;
  size_t datalen ;
} ;

extern int sbearssl_cert_from (sbearssl_cert *, br_x509_certificate const *, stralloc *) ;
extern void sbearssl_cert_to (sbearssl_cert const *, br_x509_certificate *, char *) ;

extern int sbearssl_cert_readfile (char const *, genalloc *, stralloc *) ;


 /* Private keys */

typedef struct sbearssl_rsa_skey_s sbearssl_rsa_skey, *sbearssl_rsa_skey_ref ;
struct sbearssl_rsa_skey_s
{
  uint32_t n_bitlen ;
  size_t p ;
  size_t plen ;
  size_t q ;
  size_t qlen ;
  size_t dp ;
  size_t dplen ;
  size_t dq ;
  size_t dqlen ;
  size_t iq ;
  size_t iqlen ;
} ;

extern int sbearssl_rsa_skey_from (sbearssl_rsa_skey *, br_rsa_private_key const *, stralloc *) ;
extern void sbearssl_rsa_skey_to (sbearssl_rsa_skey const *, br_rsa_private_key *, char *) ;


typedef struct sbearssl_ec_skey_s sbearssl_ec_skey, *sbearssl_ec_skey_ref ;
struct sbearssl_ec_skey_s
{
  int curve ;
  size_t x ;
  size_t xlen ;
} ;

extern int sbearssl_ec_skey_from (sbearssl_ec_skey *, br_ec_private_key const *, stralloc *) ;
extern void sbearssl_ec_skey_to (sbearssl_ec_skey const *, br_ec_private_key *, char *) ;
extern int sbearssl_ec_issuer_keytype (int *, br_x509_certificate const *) ;

union sbearssl_skey_u
{
  sbearssl_rsa_skey rsa ;
  sbearssl_ec_skey ec ;
} ;

typedef struct sbearssl_skey_s sbearssl_skey, *sbearssl_skey_ref ;
struct sbearssl_skey_s
{
  unsigned int type ;
  union sbearssl_skey_u data ;
} ;

union br_skey_u
{
  br_rsa_private_key rsa ;
  br_ec_private_key ec ;
} ;

typedef struct br_skey_s br_skey, *br_skey_ref ;
struct br_skey_s
{
  unsigned char type ;
  union br_skey_u data ;
} ;

extern int sbearssl_skey_from (sbearssl_skey *, br_skey const *, stralloc *) ;
extern int sbearssl_skey_to (sbearssl_skey const *, br_skey *, char *) ;

extern int sbearssl_skey_readfile (char const *, sbearssl_skey *, stralloc *) ;


 /* Public keys */

typedef struct sbearssl_rsa_pkey_s sbearssl_rsa_pkey, *sbearssl_rsa_pkey_ref ;
struct sbearssl_rsa_pkey_s
{
  size_t n ;
  size_t nlen ;
  size_t e ;
  size_t elen ;
} ;

extern int sbearssl_rsa_pkey_from (sbearssl_rsa_pkey *, br_rsa_public_key const *, stralloc *) ;
extern void sbearssl_rsa_pkey_to (sbearssl_rsa_pkey const *, br_rsa_public_key *, char *) ;


typedef struct sbearssl_ec_pkey_s sbearssl_ec_pkey, *sbearssl_ec_pkey_ref ;
struct sbearssl_ec_pkey_s
{
  int curve ;
  size_t q ;
  size_t qlen ;
} ;

extern int sbearssl_ec_pkey_from (sbearssl_ec_pkey *, br_ec_public_key const *, stralloc *) ;
extern void sbearssl_ec_pkey_to (sbearssl_ec_pkey const *, br_ec_public_key *, char *) ;


union sbearssl_pkey_u
{
  sbearssl_rsa_pkey rsa ;
  sbearssl_ec_pkey ec ;
} ;

typedef struct sbearssl_pkey_s sbearssl_pkey, *sbearssl_pkey_ref ;
struct sbearssl_pkey_s
{
  unsigned char type ;
  union sbearssl_pkey_u data ;
} ;

extern int sbearssl_pkey_from (sbearssl_pkey *, br_x509_pkey const *, stralloc *) ;
extern int sbearssl_pkey_to (sbearssl_pkey const *, br_x509_pkey *, char *) ;


 /* Generic PEM */

typedef struct sbearssl_pemobject_s sbearssl_pemobject, *sbearssl_pemobject_ref ;
struct sbearssl_pemobject_s
{
  size_t name ;
  size_t data ;
  size_t datalen ;
} ;

extern int sbearssl_pem_decode_from_string (char const *, size_t, genalloc *, stralloc *) ;
extern int sbearssl_pem_decode_from_buffer (buffer *, genalloc *, stralloc *) ;


 /* Trust anchors */

typedef struct sbearssl_ta_s sbearssl_ta, *sbearssl_ta_ref ;
struct sbearssl_ta_s
{
  size_t dn ;
  size_t dnlen ;
  unsigned int flags ;
  sbearssl_pkey pkey ;
} ;

extern int sbearssl_ta_from (sbearssl_ta *, br_x509_trust_anchor const *, stralloc *) ;
extern void sbearssl_ta_to (sbearssl_ta const *, br_x509_trust_anchor *, char *) ;

extern int sbearssl_ta_cert (sbearssl_ta *, sbearssl_cert const *, char const *, stralloc *) ;

extern int sbearssl_ta_readfile (char const *, genalloc *, stralloc *) ;
extern int sbearssl_ta_readdir (char const *, genalloc *, stralloc *) ;


 /* Errors */

extern char const *sbearssl_error_str (int) ;


 /* Engine */

extern int sbearssl_run (br_ssl_engine_context *, int *, unsigned int, uint32, tain_t const *) ;


 /* s6-tlsc and s6-tlsd implementations */

extern int sbearssl_s6tlsc (char const *const *, char const *const *, tain_t const *, uint32_t, uint32_t, uid_t, gid_t, unsigned int, char const *, int *) ;
extern int sbearssl_s6tlsd (char const *const *, char const *const *, tain_t const *, uint32_t, uint32_t, uid_t, gid_t, unsigned int) ;

#endif
