/* mpscmvff.h: MEMORY POOL SYSTEM CLASS "MVFF"
 *
 * $Id: //info.ravenbrook.com/project/mps/version/1.113/code/mpscmvff.h#1 $
 * Copyright (c) 2001-2013 Ravenbrook Limited.  See end of file for license.
 */

#ifndef mpscmvff_h
#define mpscmvff_h

#include "mps.h"

extern const struct mps_key_s _mps_key_mvff_slot_high;
#define MPS_KEY_MVFF_SLOT_HIGH (&_mps_key_mvff_slot_high)
#define MPS_KEY_MVFF_SLOT_HIGH_FIELD b
extern const struct mps_key_s _mps_key_mvff_arena_high;
#define MPS_KEY_MVFF_ARENA_HIGH (&_mps_key_mvff_arena_high)
#define MPS_KEY_MVFF_ARENA_HIGH_FIELD b
extern const struct mps_key_s _mps_key_mvff_first_fit;
#define MPS_KEY_MVFF_FIRST_FIT (&_mps_key_mvff_first_fit)
#define MPS_KEY_MVFF_FIRST_FIT_FIELD b

extern size_t mps_mvff_free_size(mps_pool_t mps_pool);
extern size_t mps_mvff_size(mps_pool_t mps_pool);
extern mps_class_t mps_class_mvff(void);
extern mps_class_t mps_class_mvff_debug(void);

#endif /* mpscmvff_h */


/* C. COPYRIGHT AND LICENSE
 *
 * Copyright (C) 2001-2013 Ravenbrook Limited <http://www.ravenbrook.com/>.
 * All rights reserved.  This is an open source license.  Contact
 * Ravenbrook for commercial licensing options.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * 3. Redistributions in any form must be accompanied by information on how
 * to obtain complete source code for this software and any accompanying
 * software that uses this software.  The source code must either be
 * included in the distribution or be available for no more than the cost
 * of distribution plus a nominal fee, and must be freely redistributable
 * under reasonable conditions.  For an executable file, complete source
 * code means the source code for all modules it contains. It does not
 * include source code for modules or files that typically accompany the
 * major components of the operating system on which the executable file
 * runs.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
