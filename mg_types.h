#if !defined(MG_TYPES_H_)
#define MG_TYPES_H_ 1

#include <limits.h>

#if ULONG_MAX > 4294967295U		/* looks like >32-bit world */
typedef int			MG_long_t;
typedef unsigned int		MG_u_long_t;
#elif ULONG_MAX == 4294967295U		/* looks like 32-bit world */
typedef long int		MG_long_t;
typedef unsigned long int	MG_u_long_t;
#else
#error Unknown word size
#endif

#endif /* defined(MG_TYPES_H_) */
