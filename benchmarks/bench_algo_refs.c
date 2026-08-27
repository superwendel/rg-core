// Reference sort implementations for rg_algo benchmarks (C-only).

#include <quadsort.h>

#ifdef QUAD_CACHE
    #undef QUAD_CACHE
#endif

#define CMPFUNC CRUMSORT_CMPFUNC
#include <crumsort.h>
#undef CMPFUNC

#ifdef QUAD_CACHE
    #undef QUAD_CACHE
#endif
