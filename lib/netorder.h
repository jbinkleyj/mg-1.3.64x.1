#ifndef NETORDER_H
#define NETORDER_H

#include "sysfuncs.h"

/* [RPAP - Feb 97: WIN32 Port] */
#ifdef __WIN32__
# define htonl(x)         (x)
# define ntohl(x)         (x)
# define htons(x)         (x)
# define ntohs(x)         (x)
#else
# include <netinet/in.h>
#endif

#if !defined(WORDS_BIGENDIAN)		/* then LITTLE_ENDIAN */

/* double */
#define HTOND(d)                                                                                  \
        do {                                                                                      \
             MG_u_long_t tmph, tmpl;                                                            \
	     bcopy ((char *) &d, (char *) &tmph, sizeof(double) >> 1);                            \
	     bcopy ((char *) &d + (sizeof(double) >> 1), (char *) &tmpl, sizeof (double) >> 1);   \
	     tmph = htonl (tmph);                                                                 \
	     tmpl = htonl (tmpl);                                                                 \
	     bcopy ((char *) &tmpl, (char *) &d, sizeof (double) >> 1);                           \
	     bcopy ((char *) &tmph, (char *) &d + (sizeof(double) >> 1), sizeof (double) >> 1);   \
	}while(0)
#define NTOHD(d)                                                                                  \
        do {                                                                                      \
             MG_u_long_t tmph, tmpl;                                                            \
	     bcopy ((char *) &d, (char *) &tmph, sizeof(double) >> 1);                            \
	     bcopy ((char *) &d + (sizeof(double) >> 1), (char *) &tmpl, sizeof (double) >> 1);   \
	     tmph = ntohl (tmph);                                                                 \
	     tmpl = ntohl (tmpl);                                                                 \
	     bcopy ((char *) &tmpl, (char *) &d, sizeof (double) >> 1);                           \
	     bcopy ((char *) &tmph, (char *) &d + (sizeof(double) >> 1), sizeof (double) >> 1);   \
        }while(0)
#define HTOND2(hd, nd)                                                                            \
        do {                                                                                      \
             MG_u_long_t tmph, tmpl;                                                            \
	     bcopy ((char *) &hd, (char *) &tmph, sizeof(double) >> 1);                           \
	     bcopy ((char *) &hd + (sizeof(double) >> 1), (char *) &tmpl, sizeof (double) >> 1);  \
	     tmph = htonl (tmph);                                                                 \
	     tmpl = htonl (tmpl);                                                                 \
	     bcopy ((char *) &tmpl, (char *) &nd, sizeof (double) >> 1);                          \
	     bcopy ((char *) &tmph, (char *) &nd + (sizeof(double) >> 1), sizeof (double) >> 1);  \
	}while(0)
#define NTOHD2(nd, hd)                                                                            \
        do {                                                                                      \
             MG_u_long_t tmph, tmpl;                                                            \
	     bcopy ((char *) &nd, (char *) &tmph, sizeof(double) >> 1);                           \
	     bcopy ((char *) &nd + (sizeof(double) >> 1), (char *) &tmpl, sizeof (double) >> 1);  \
	     tmph = ntohl (tmph);                                                                 \
	     tmpl = ntohl (tmpl);                                                                 \
	     bcopy ((char *) &tmpl, (char *) &hd, sizeof (double) >> 1);                          \
	     bcopy ((char *) &tmph, (char *) &hd + (sizeof(double) >> 1), sizeof (double) >> 1);  \
        }while(0)

/* float */
#define HTONF(f)                                                                   \
        do {                                                                       \
             MG_u_long_t tmp;                                                    \
             bcopy ((char *) &(f), (char *) &tmp, sizeof (float));                 \
             HTONUL (tmp);                                                         \
	     bcopy ((char *) &tmp, (char *) &(f), sizeof (float));                 \
	}while(0)
#define NTOHF(f)                                                                   \
        do {                                                                       \
             MG_u_long_t tmp;                                                    \
             bcopy ((char *) &(f), (char *) &tmp, sizeof (float));                 \
	     NTOHUL (tmp);                                                         \
	     bcopy ((char *) &tmp, (char *) &(f), sizeof (float));                 \
	}while(0)
#define HTONF2(hf, nf)                                                             \
        do {                                                                       \
             MG_u_long_t tmp;                                                    \
             bcopy ((char *) &(hf), (char *) &tmp, sizeof (float));                \
             HTONUL (tmp);                                                         \
	     bcopy ((char *) &tmp, (char *) &(nf), sizeof (float));                \
	}while(0)
#define NTOHF2(nf, hf)                                                             \
        do {                                                                       \
             MG_u_long_t tmp;                                                    \
             bcopy ((char *) &(nf), (char *) &tmp, sizeof (float));                \
	     NTOHUL (tmp);                                                         \
	     bcopy ((char *) &tmp, (char *) &(hf), sizeof (float));                \
	}while(0)

/* pointers */
#define HTONP(p)          ((p) = (void *) htonl ( (MG_u_long_t)  p))
#define NTOHP(p)          ((p) = (void *) ntohl ( (MG_u_long_t)  p))
#define HTONP2(hp, np)    ((np) = (void *) htonl ( (MG_u_long_t)  hp))
#define NTOHP2(np, hp)    ((hp) = (void *) ntohl ( (MG_u_long_t)  np))

/* MG_u_long_t */
#define HTONUL(l)         ((l) = htonl((l)))
#define NTOHUL(l)         ((l) = ntohl((l)))
#define HTONUL2(hl, nl)   ((nl) = htonl ((hl)))
#define NTOHUL2(nl, hl)   ((hl) = ntohl ((nl)))

/* signed MG_long_t */
#define HTONSL(l)         ((l) =  (MG_long_t)  htonl ( (MG_u_long_t)  (l)))
#define NTOHSL(l)         ((l) =  (MG_long_t)  ntohl ( (MG_u_long_t)  (l)))
#define HTONSL2(hl, nl)   ((nl) =  (MG_long_t)  htonl ( (MG_u_long_t)  (hl)))
#define NTOHSL2(nl, hl)   ((hl) =  (MG_long_t)  ntohl ( (MG_u_long_t)  (nl)))

/* unsigned int */
#define HTONUI(i)         ((i) = (unsigned int) htonl ( (MG_u_long_t)  (i)))
#define NTOHUI(i)         ((i) = (unsigned int) ntohl ( (MG_u_long_t)  (i)))
#define HTONUI2(hi, ni)   ((ni) = (unsigned int) htonl ( (MG_u_long_t)  (hi)))
#define NTOHUI2(ni, hi)   ((hi) = (unsigned int) ntohl ( (MG_u_long_t)  (ni)))

/* signed int */
#define HTONSI(i)         ((i) = (int) htonl ( (MG_u_long_t)  (i)))
#define NTOHSI(i)         ((i) = (int) ntohl ( (MG_u_long_t)  (i)))
#define HTONSI2(hi, ni)   ((ni) = (int) htonl ( (MG_u_long_t)  (hi)))
#define NTOHSI2(ni, hi)   ((hi) = (int) ntohl ( (MG_u_long_t)  (ni)))

/* unsigned short */
#define HTONUS(s)         ((s) = htons((s)))
#define NTOHUS(s)         ((s) = ntohs((s)))
#define HTONUS2(hs, ns)   ((ns) = htons((hs)))
#define NTOHUS2(ns, hs)   ((hs) = ntohs((ns)))

#else   /* _BIG_ENDIAN */

/* double */
#define HTOND(d)          (d)
#define NTOHD(d)          (d)
#define HTOND2(hd, nd)    ((hd) = (nd))
#define NTOHD2(nd, hd)    ((nd) = (hd))

/* float */
#define HTONF(f)          (f)
#define NTOHF(f)          (f)
#define HTONF2(hf, nf)    ((nf) = (hf))
#define NTOHF2(nf, hf)    ((hf) = (nf))

/* pointers */
#define HTONP(p)          (p)
#define NTOHP(p)          (p)
#define HTONP2(hp, np)    ((np) = (hp))
#define NTOHP2(np, hp)    ((hp) = (np))

/* MG_u_long_t */
#define HTONUL(l)         (l)
#define NTOHUL(l)         (l)
#define HTONUL2(hl, nl)   ((nl) = (hl))
#define NTOHUL2(nl, hl)   ((hl) = (nl))

/* signed MG_long_t */
#define HTONSL(l)         (l)
#define NTOHSL(l)         (l)
#define HTONSL2(hl, nl)   ((nl) = (hl))
#define NTOHSL2(nl, hl)   ((hl) = (nl))

/* unsigned int */
#define HTONUI(i)         (i)
#define NTOHUI(i)         (i)
#define HTONUI2(hi, ni)   ((ni) = (hi))
#define NTOHUI2(ni, hi)   ((hi) = (ni))

/* signed int */
#define HTONSI(i)         (i)
#define NTOHSI(i)         (i)
#define HTONSI2(hi, ni)   ((ni) = (hi))
#define NTOHSI2(ni, hi)   ((hi) = (ni))

/* unsigned short */
#define HTONUS(s)         (s)
#define NTOHUS(s)         (s)
#define HTONUS2(hs, ns)   ((ns) = (hs))
#define NTOHUS2(ns, hs)   ((hs) = (ns))



#endif

#endif /* netorder.h */
