// rg_sprintf_hybrid - Selects the fastest available rg_sprintf implementation
//
// Part of the Reverse Gravity (rg_) core libraries.
// Uses rg_sprintf_asm.h when the build provides asm helpers, otherwise falls
// back to the baseline rg_sprintf.h implementation.
// Note: the asm path uses external helpers (for example
// src/asm/sprintf/win_x64/rg_sprintf_asm_x64.asm or
// src/asm/sprintf/linux_x64/rg_sprintf_asm_x64.S). Define RG_SPRINTF_NO_ASM to force C.
//
// OPTIONS:
//   #define RG_SPRINTF_HYBRID_FORCE_C   - Always use rg_sprintf.h
//   #define RG_SPRINTF_HYBRID_FORCE_ASM - Always use rg_sprintf_asm.h
//
// Author: Steven Wendel (superwendel)

#ifndef RG_SPRINTF_HYBRID_H
#define RG_SPRINTF_HYBRID_H

#if defined(RG_SPRINTF_HYBRID_FORCE_C) || defined(RG_SPRINTF_NO_ASM)
#include "rg_sprintf.h"
#elif defined(RG_SPRINTF_HYBRID_FORCE_ASM) || defined(RG_SPRINTF_HAS_ASM)
#include "rg_sprintf_asm.h"
#else
#if defined(_MSC_VER) && defined(_M_X64) && defined(__AVX2__)
#include "rg_sprintf_asm.h"
#else
#include "rg_sprintf.h"
#endif
#endif
#endif // RG_SPRINTF_HYBRID_H
