#ifndef _L2DSSZ_C
#define _L2DSSZ_C


#include "l2dsinc.h"


INT4  L2dsSizingMemCreateMemPools()
{
    INT4 i4RetVal;
    INT4 i4SizingId;

    for( i4SizingId=0; i4SizingId < L2DS_MAX_SIZING_ID; i4SizingId++) {
        i4RetVal = MemCreateMemPool( 
                          FsL2DSSizingParams[i4SizingId].u4StructSize,
                          FsL2DSSizingParams[i4SizingId].u4PreAllocatedUnits,
                          MEM_DEFAULT_MEMORY_TYPE,
                          &(L2DSMemPoolIds[ i4SizingId]));
        if( i4RetVal == (INT4) MEM_FAILURE) {
            L2dsSizingMemDeleteMemPools();
            return OSIX_FAILURE;
        }
    }
    return OSIX_SUCCESS;
}


INT4   L2dsSzRegisterModuleSizingParams( CHR1 *pu1ModName)
{
      /* Copy the Module Name */ 
       IssSzRegisterModuleSizingParams( pu1ModName, FsL2DSSizingParams); 
      return OSIX_SUCCESS; 
}


VOID  L2dsSizingMemDeleteMemPools()
{
    INT4 i4SizingId;

    for( i4SizingId=0; i4SizingId < L2DS_MAX_SIZING_ID; i4SizingId++) {
        if(L2DSMemPoolIds[i4SizingId] != 0)
        {
            MemDeleteMemPool( L2DSMemPoolIds[ i4SizingId] );
            L2DSMemPoolIds[i4SizingId] = 0;
        }
    }
    return;
}

#endif /*L2DSSZ_C*/
