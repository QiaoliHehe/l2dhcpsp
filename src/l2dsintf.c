/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsintf.c,v 1.3 2010/08/04 08:51:03 prabuc Exp $               */
/*****************************************************************************/
/*    FILE  NAME            : l2dsintf.c                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snooping                                */
/*    MODULE NAME           : L2DHCP Snooping Interface Module               */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains interface related functions */
/*                            for L2DHCP Snooping module                     */
/*---------------------------------------------------------------------------*/

#include "l2dsinc.h"
#include "l2dsextn.h"

/*****************************************************************************/
/* Function Name      : L2dsIntfDeleteIntfEntry                              */
/*                                                                           */
/* Description        : This function is to delete an interface entry        */
/*                                                                           */
/* Input(s)           : u4L2dsCxtId - Context Identifier                     */
/*                      VlanId     - Incoming VLAN Id                        */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : L2DS_SUCCESS/L2DS_FAILURE                            */
/*****************************************************************************/
INT4
L2dsIntfDeleteIntfEntry (UINT4 u4L2dsCxtId, tVlanId VlanId)
{
    tL2DhcpSnpIfaceEntry L2DSVlanEntry;
    tL2DhcpSnpIfaceEntry *pL2DSVlanEntry = NULL;
    INT4                i4RetStat = L2DS_SUCCESS;

    MEMSET (&L2DSVlanEntry, L2DS_ZERO, sizeof (tL2DhcpSnpIfaceEntry));

    L2DS_TRC(L2DS_FN_ENTRY, "[L2dsIntfDeleteIntfEntry:Begin]\r\n");
    L2DS_TRC_ARG1 (L2DS_FN_ARGS, "[L2dsIntfDeleteIntfEntry]"
		"Arguments: VlanId=%d \r\n", VlanId);

    L2DSVlanEntry.u4L2dsCxtId = u4L2dsCxtId;
    L2DSVlanEntry.VlanId = VlanId;

    /* Get the interface entry */
    pL2DSVlanEntry = (tL2DhcpSnpIfaceEntry *)
        RBTreeGet (L2DS_INTF_RBTREE, &L2DSVlanEntry);

    if (pL2DSVlanEntry == NULL)
    {
        L2DS_TRC_ARG1 (L2DS_FAIL_TRC, "[L2dsIntfDeleteIntfEntry]"
			"Unable to find the entry for VLAN %d.\r\n", VlanId);
        return L2DS_SUCCESS;
    }
    /* Clear all entries in the binding database, 
     * which are based on this VLAN */
    if (L2DS_INT_ROWSTATUS (pL2DSVlanEntry) == L2DS_ACTIVE)
    {
        if (L2dsPortDeleteEntries (u4L2dsCxtId, VlanId) != L2DS_SUCCESS)
        {
            L2DS_TRC_ARG1 (L2DS_FAIL_TRC, "[L2dsIntfDeleteIntfEntry]"
				"Unable to clear the binding entry on VLAN %d.\r\n", VlanId);
            i4RetStat = L2DS_FAILURE;
        }
    }
    /* remove the entry from RBTree */
    if (RBTreeRemove (L2DS_INTF_RBTREE,
                      (tRBElem *) pL2DSVlanEntry) != RB_SUCCESS)
    {
        L2DS_TRC_ARG1 (L2DS_FAIL_TRC, "[L2dsIntfDeleteIntfEntry]"
			"Remove DHCP VLAN%d entry failed.\r\n", VlanId);
        i4RetStat = L2DS_FAILURE;
    }
    /* Release the memory back to pool */
    L2DS_RELEASE_MEM_BLOCK (L2DS_INTF_POOL_ID, pL2DSVlanEntry);

    L2DS_TRC(L2DS_FN_EXIT, "[L2dsIntfDeleteIntfEntry:End]\r\n");
    return i4RetStat;
}

/*****************************************************************************/
/* Function Name      : L2dsIntfCreateIntfEntry                              */
/*                                                                           */
/* Description        : This function is to create an interface entry        */
/*                                                                           */
/* Input(s)           : u4L2dsCxtId - Context Identifier                     */
/*                      VlanId     - Incoming VLAN Id                        */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : NULL/Pointer to interface entry                      */
/*****************************************************************************/
tL2DhcpSnpIfaceEntry *
L2dsIntfCreateIntfEntry (UINT4 u4L2dsCxtId, tVlanId VlanId)
{
    tL2DhcpSnpIfaceEntry L2DSVlanEntry;
    tL2DhcpSnpIfaceEntry *pL2DSVlanEntry = NULL;

    MEMSET (&L2DSVlanEntry, L2DS_ZERO, sizeof (tL2DhcpSnpIfaceEntry));

    L2DS_TRC(L2DS_FN_ENTRY, "[L2dsIntfCreateIntfEntry:Begin]\r\n");
    L2DS_TRC_ARG1(L2DS_FN_ARGS, "[L2dsIntfCreateIntfEntry]"
		"Arguments: VlanId=%d.\r\n ", VlanId);

    L2DSVlanEntry.u4L2dsCxtId = u4L2dsCxtId;
    L2DSVlanEntry.VlanId = VlanId;

    /* Get the interface entry */
    pL2DSVlanEntry = (tL2DhcpSnpIfaceEntry *)
        RBTreeGet (L2DS_INTF_RBTREE, &L2DSVlanEntry);

    if (pL2DSVlanEntry != NULL)
    {
        return pL2DSVlanEntry;
    }

    /* Allocate memory for the interface entry */
    if ((pL2DSVlanEntry = (tL2DhcpSnpIfaceEntry *)
         (L2DS_ALLOC_MEM_BLOCK (L2DS_INTF_POOL_ID))) == NULL)
    {
        L2DS_TRC_ARG1 (L2DS_FAIL_TRC, "[L2dsIntfCreateIntfEntry]"
			"Allocate memory for VLAN%d interface entry failed.\r\n", VlanId);
        return NULL;
    }

    MEMSET (pL2DSVlanEntry, L2DS_ZERO, sizeof (tL2DhcpSnpIfaceEntry));

    pL2DSVlanEntry->u4L2dsCxtId = u4L2dsCxtId;
    pL2DSVlanEntry->VlanId = VlanId;
    pL2DSVlanEntry->u1VlanSnpStatus = L2DS_DISABLED;

    /* Add the entry to RBTree */
    if (RBTreeAdd (L2DS_INTF_RBTREE, (tRBElem *) pL2DSVlanEntry) ==
        RB_FAILURE)
    {
        L2DS_RELEASE_MEM_BLOCK (L2DS_INTF_POOL_ID, pL2DSVlanEntry);

        L2DS_TRC_ARG1 (L2DS_FAIL_TRC, "[L2dsIntfCreateIntfEntry]"
			"Add VLAN%d interface entry in RBTree failed.\r\n", VlanId);
        return NULL;
    }

    L2DS_TRC(L2DS_FN_EXIT, "[L2dsIntfCreateIntfEntry:End]\r\n");
    return pL2DSVlanEntry;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsIntfDelAll
 *
 *    Description        : Delete all dhcp snooping vlan interfaces
 *
 *    Input(s)            :None
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
CmL2dsIntfDelAll(VOID)
{
    tL2DhcpSnpIfaceEntry	*pL2dsVlanEntry = NULL;
    tL2DhcpSnpIfaceEntry	*pL2dsVlanEntryNext = NULL;

    pL2dsVlanEntry = (tL2DhcpSnpIfaceEntry *)RBTreeGetFirst(
		L2DS_INTF_RBTREE);

    while(pL2dsVlanEntry)
    {
        pL2dsVlanEntryNext = (tL2DhcpSnpIfaceEntry *)RBTreeGetNext(
			L2DS_INTF_RBTREE, (tRBElem *)pL2dsVlanEntry, NULL);
		
		RBTreeRemove(L2DS_INTF_RBTREE, (tRBElem *)pL2dsVlanEntry);
		L2DS_RELEASE_MEM_BLOCK(L2DS_INTF_POOL_ID, pL2dsVlanEntry);

        pL2dsVlanEntry = pL2dsVlanEntryNext;
    }

	return L2DS_SUCCESS;
}



/*                                                                           */
/***************************** END OF FILE ***********************************/
/*                                                                           */
