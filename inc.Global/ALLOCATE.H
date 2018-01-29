/*
 *  File        :   allocate.h
 *
 *  Project     :   SRA VPR26   -   Enhanced Conference Facility.
 *  Author      :   Ansa Pty. Ltd. (c) 1992
 *
 *  Description :   accountable dynamic memory allocation functions
 *
 */

#ifndef _ALLOCATE_H_
#define _ALLOCATE_H_

EXPORT int  Gen_alloc_blocks;

EXPORT int  Gen_Alloc ( uint size, void **ptr );
EXPORT void Gen_DeAlloc ( void *ptr );
EXPORT int  Gen_AllocArray ( uint elem_numb, uint size, void **ptr );

#endif

