/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsmain.c,v 1.11 2012/05/03 13:00:52 siva Exp $               */
/*****************************************************************************/
/*    FILE  NAME            : l2dsmain.c                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snooping                                */
/*    MODULE NAME           : L2DHCP Snooping Main Module                    */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains init, deinit funtions       */
/*                            for L2DHCP Snooping module                     */
/*---------------------------------------------------------------------------*/

#include "l2dsinc.h"
#include "l2dsglob.h"

/*****************************************************************************/
/* Function Name      : L2DSMain                                             */
/*                                                                           */
/* Description        : This function is the main entry point function for   */
/*                      the L2DHCP Snooping task                             */
/*                                                                           */
/* Input(s)           : pi1Param - unused                                    */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None                                                 */
/*****************************************************************************/
VOID
L2DSMain (INT1 *pi1Param)
{
    UINT4	u4EventMask = L2DS_ZERO;

    UNUSED_PARAM(pi1Param);

	if(L2DS_SUCCESS != L2dsMainInit())
	{
		PRINTF("L2dsMainInit fail!");
		L2dsMainDeInit();
		lrInitComplete(OSIX_FAILURE);
		return;
	}

#ifdef SNMP_2_WANTED
    /* Register the L2DHCP Snooping MIB */
    L2dsUtilRegisterFsDhcSnpMib ();
#endif

    if (L2dsGetVcmSystemModeExt (L2DS_PROTOCOL_ID) == VCM_SI_MODE)
    {
        L2DS_IS_CONTEXT_EXIST (L2DS_DEFAULT_CXT_ID) = L2DS_TRUE;
    }

    lrInitComplete (OSIX_SUCCESS);

	/* Wait infinitely till an event comes. 
	* When the event comes, process it and wait again for further events. 
	*/
    while (L2DS_TRUE)
    {
        if(L2DS_RECEIVE_EVENT(L2DS_EVENTS, L2DS_EVENT_WAIT_FLAG,
                                L2DS_ZERO, &u4EventMask) == OSIX_SUCCESS)
        {
            if(u4EventMask & L2DS_EVENT_ARRIVED)
            {
                L2dsMainProcessEvent ();
            }

			if(u4EventMask & L2DS_TIMER_EVENT)
			{
				L2dsMainTmr();
			}

			/*CAMEOTAG:Ricann,20121229*/
			if(u4EventMask & L2DS_MIB_RESTORE_OVER)
			{
				CmL2dsMibRestoreOver();
			}
        }
    }
}

/*****************************************************************************
 *
 *    Function Name        : L2dsMainInit
 *
 *    Description        : This function  is used to init dhcp snooping module
 *
 *    Input(s)            :None
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
L2dsMainInit(VOID)
{
	/* Debug flag */
    L2DS_TRC_FLAG = L2DS_ALL_TRC;

    /* Create semaphore for mutual exclusion */
    if(L2DS_CREATE_SEM(L2DS_SEM_NAME, L2DS_SEM_COUNT, L2DS_SEM_FLAGS,
                         &(L2DS_SEM_ID)) != OSIX_SUCCESS)
    {
        return L2DS_FAILURE;
    }

    /* Create Queue */
    if (L2DS_CREATE_QUEUE (L2DS_QUEUE_NAME, OSIX_MAX_Q_MSG_LEN,
                           L2DS_QUEUE_DEPTH, &(L2DS_QUEUE_ID)) != OSIX_SUCCESS)
    {
        return L2DS_FAILURE;
    }

    /* Creation of all mempools */
    if (L2dsSizingMemCreateMemPools () == L2DS_FAILURE)
    {
        return L2DS_FAILURE;
    }
    /*Assigning mempool Id's */
    L2dsMainAssignMempoolIds ();

    if (gpau1L2dsDataBuffer == NULL)
    {
    	gpau1L2dsDataBuffer = (UINT1 *)L2DS_ALLOC_MEM_BLOCK(L2DS_DYN_POOL_ID);
        if (gpau1L2dsDataBuffer == NULL)
        {
            return L2DS_FAILURE;
        }
    }

    /* Create the RBTree For Interface nodes */
    L2DS_INTF_RBTREE = L2DS_CREATE_RBTREE (L2DS_ZERO, 
    	L2dsUtilRBTreeIntfEntryCmp);
    if (L2DS_INTF_RBTREE == NULL)
    {
        return L2DS_FAILURE;
    }

    /* Create timer list  */
    if(TMR_SUCCESS != TmrCreateTimerList((CONST UINT1 *)L2DS_TASK_NAME, 
        L2DS_TIMER_EVENT, NULL, &(L2DS_TMR_LIST)))
    {
        return L2DS_FAILURE;
    }
    /*CAMEOTAG: add by youchao 2013-1-18, add syslog for dhcp snooping module */
#ifdef SYSLOG_WANTED
    gi4SysLogId = SYS_LOG_REGISTER ((CONST UINT1 *) "DHCPSNP", SYSLOG_INFO_LEVEL);
    if (gi4SysLogId <= 0)
    {
        L2DS_TRC (L2DS_FAIL_TRC, "L2dsMainInit: Sys log failed. \r\n");
        return L2DS_FAILURE;
    }
#endif

    /* Set the Global status and MacVerification status */
    L2DS_ADMIN_STATUS(L2DS_DEFAULT_CXT_ID) = L2DS_DISABLED;
    L2DS_MAC_VERIFY_STATUS(L2DS_DEFAULT_CXT_ID) = L2DS_ENABLED;
	
	/* Set the Global dhcp snooping insert option82 status */
	L2DS_ADD_OP82_STATUS(L2DS_DEFAULT_CXT_ID) = L2DS_DISABLED;
	L2DS_FWD_OP82_STATUS(L2DS_DEFAULT_CXT_ID) = L2DS_DISABLED;
	L2DS_BACKUP_DB_STATUS(L2DS_DEFAULT_CXT_ID) = L2DS_DISABLED;
	L2DS_BACKUP_INTERVAL(L2DS_DEFAULT_CXT_ID) = DCS_BACKUP_DB_ITVL_DFT;

	/*CAMEOTAG:Ricann,20130228*/
	MEMSET(&gL2DSFdbGlob, 0, sizeof(gL2DSFdbGlob));

	/*Used to print l2ds fdb table every 5 minutes for debug*/
	TmrStart(L2DS_TMR_LIST, &gL2DSFdbGlob.PrintFdbTmrBlk,
			L2DS_TMR_PRINT_FDB, L2DS_PRINT_FDB_TIME, 0);

	return L2DS_SUCCESS;
}

/*****************************************************************************/
/* Function Name      : L2dsMainDeInit                                       */
/*                                                                           */
/* Description        : This function Deletes Semaphore, Queue and MemPools  */
/*                                                                           */
/* Input(s)           : None                                                 */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None.                                                */
/*****************************************************************************/
VOID
L2dsMainDeInit (VOID)
{
    if (L2DS_SEM_ID != L2DS_ZERO)
    {
        L2DS_DELETE_SEM (L2DS_ZERO, L2DS_SEM_NAME);
    }

    if (L2DS_QUEUE_ID != L2DS_ZERO)
    {
        L2DS_DELETE_QUEUE (L2DS_QUEUE_ID);
    }

    L2dsSizingMemDeleteMemPools ();

    return;
}

/*****************************************************************************/
/* Function Name      : L2dsMainProcessEvent                                 */
/*                                                                           */
/* Description        : This function handles the events.                    */
/*                                                                           */
/* Input(s)           : None                                                 */
/*                                                                           */
/* Output(s)          : None                                                 */
/*                                                                           */
/* Return Value(s)    : None.                                                */
/*****************************************************************************/
VOID
L2dsMainProcessEvent (VOID)
{
    tCRU_BUF_CHAIN_HEADER *pBuf = NULL;
    tL2DhcpSnpQMsg     *pL2DhcpSnpQMsg = NULL;
    tOsixMsg           *pOsixQMsg = NULL;
    tVlanTag            VlanTag;
    tVlanId             VlanId = L2DS_ZERO;
    UINT4               u4InPort = L2DS_ZERO;
    UINT4               u4L2dsCxtId = L2DS_ZERO;

    MEMSET (&VlanTag, L2DS_ZERO, sizeof (tVlanTag));

    L2DS_TRC(L2DS_FN_ENTRY, "[L2dsMainProcessEvent:Begin]\r\n");

    /* Take the lock, before processing the event */
    if (L2DS_LOCK () == SNMP_FAILURE)
    {
        L2DS_TRC(L2DS_FAIL_TRC, "[L2dsMainProcessEvent]"
			"Taking the protocol lock failed.\r\n");
        return;
    }

    /* Receive the event from the queue */
    while (L2DS_RECV_FROM_QUEUE
           (L2DS_QUEUE_ID, (UINT1 *) &pOsixQMsg, OSIX_DEF_MSG_LEN,
            L2DS_ZERO) == OSIX_SUCCESS)
    {
        pL2DhcpSnpQMsg = (tL2DhcpSnpQMsg *) pOsixQMsg;

        if (pL2DhcpSnpQMsg->u4EventType == L2DS_PKT_RECEIVE_EVENT)
        {
            /* This is an incoming packet related event */
            pBuf = pL2DhcpSnpQMsg->pInQMsg;
            u4L2dsCxtId = pL2DhcpSnpQMsg->u4L2dsCxtId;
            u4InPort = pL2DhcpSnpQMsg->u4InPort;

            MEMCPY (&VlanTag, &(pL2DhcpSnpQMsg->VlanTag), sizeof (tVlanTag));

            /* Process the incoming packet */
            if(L2dsPktHdl (pBuf, u4L2dsCxtId, u4InPort, VlanTag) 
				== L2DS_FAILURE)
            {
                L2DS_TRC_ARG2(L2DS_FAIL_TRC, "[L2dsMainProcessEvent]"
					"Handle DHCP Packet failed, VLAN %d, Port %d .\r\n",
					VlanTag.OuterVlanTag.u2VlanId, u4InPort);
            }
        }
        else if (pL2DhcpSnpQMsg->u4EventType == L2DS_VLAN_INTERFACE_EVENT)
        {
            /* This is a VLAN deletion related event */
            u4L2dsCxtId = pL2DhcpSnpQMsg->u4L2dsCxtId;
            VlanId = pL2DhcpSnpQMsg->VlanId;

            if (L2dsIntfDeleteIntfEntry (u4L2dsCxtId, VlanId) == L2DS_FAILURE)
            {
                L2DS_TRC_ARG1(L2DS_FAIL_TRC, "[L2dsMainProcessEvent]"
					"Delete DHCP VLAN%d interface failed.\r\n", VlanId);
            }
        }
        else
        {
            L2DS_TRC(L2DS_FAIL_TRC, "[L2dsMainProcessEvent]Invalid Event.\r\n");
        }
        L2DS_RELEASE_MEM_BLOCK (L2DS_Q_POOL_ID, pL2DhcpSnpQMsg);
    }
    L2DS_UNLOCK ();

    L2DS_TRC(L2DS_FN_EXIT, "[L2dsMainProcessEvent:End]\r\n");
    return;
}

/*****************************************************************************/
/* Function Name      : L2dsMainAssignMempoolIds                             */
/*                                                                           */
/* Description        : This function is used to assign respective mempool   */
/*                       ID's                                                */
/*                                                                           */
/* Input (s)          : NONE.                                                */
/*                                                                           */
/* Output (s)         : NONE                                                 */
/*                                                                           */
/* Return Value(s)    : NONE                                                 */
/*                                                                           */
/*****************************************************************************/

VOID
L2dsMainAssignMempoolIds (VOID)
{
    /*tL2DhcpSnpQMsg */
    L2DS_Q_POOL_ID = L2DSMemPoolIds[MAX_L2DS_Q_MESG_SIZING_ID];
    /*tL2DhcpSnpIfaceEntry */
    L2DS_INTF_POOL_ID = L2DSMemPoolIds[MAX_L2DS_IFACE_ENTRIES_SIZING_ID];
    L2DS_DYN_POOL_ID = L2DSMemPoolIds[MAX_L2DS_PDU_COUNT_SIZING_ID];
    /*tL2DhcpSnpPktInfo */
    L2DS_PKT_INFO_POOL_ID = L2DSMemPoolIds[MAX_L2DS_PKT_INFO_SIZING_ID];
}

/*****************************************************************************
 *
 *    Function Name        : L2dsMainTmr
 *
 *    Description        : This function  is used to handle timer
 *
 *    Input(s)            :None
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
L2dsMainTmr(VOID)
{
    tTmrAppTimer 	*pTmr;
	tTmrBlk      	*pTmrBlk;
	INT4			i4Offset;
	tL2DhcpSnpFdb	*pL2DSFdbEntry = NULL;

    TmrGetExpiredTimers(L2DS_TMR_LIST, &pTmr);
	if(pTmr == NULL)
    {
        return L2DS_SUCCESS;
    }

	L2DS_TRC(L2DS_TRC_FDB, "[L2dsMainTmr]\r\n");
	
	while(pTmr)
    {
    	pTmrBlk = (tTmrBlk *)pTmr;

		/*CAMEOTAG:Ricann,20130228*/
		if(pTmrBlk->u1TimerId == L2DS_TMR_FDB)
		{
            i4Offset = L2DS_FDB_TMR_OFFSET;
            pL2DSFdbEntry = (tL2DhcpSnpFdb *)(VOID*)((UINT1 *)pTmrBlk - i4Offset);
			L2DS_TRC_ARG1(L2DS_TRC_FDB, "i4Offset = %d\r\n", i4Offset);
			L2DS_TRC_ARG1(L2DS_TRC_FDB, "Entry index = %d\r\n",
				L2DS_FDB_ENTRY_IDX(pL2DSFdbEntry));
			CmL2dsFdbDel(pL2DSFdbEntry->Vid, pL2DSFdbEntry->Mac, 
				pL2DSFdbEntry->u4Port);
		}
		else if(pTmrBlk->u1TimerId == L2DS_TMR_DB)
		{
			/* Start refresh DB timer  */
			TmrStart(L2DS_TMR_LIST, &L2DS_TMR_DB_BLK, L2DS_TMR_DB, 
				L2DS_BACKUP_INTERVAL(L2DS_DEFAULT_CXT_ID), 0);
			
			/* Write binding db to flash */
			if(L2DS_ADMIN_STATUS(L2DS_DEFAULT_CXT_ID) == L2DS_ENABLED &&
				L2DS_BACKUP_DB_STATUS(L2DS_DEFAULT_CXT_ID) == L2DS_ENABLED)
			{
				CmAPIIpdbWrite((UINT1 *)DCS_BACKUP_DB_FILE);
			}
		}
		else if(pTmrBlk->u1TimerId == L2DS_TMR_PRINT_FDB)
		{
			/*Just for debug, to be removed*/
			/*
			TmrStart(L2DS_TMR_LIST, &gL2DSFdbGlob.PrintFdbTmrBlk,
				L2DS_TMR_PRINT_FDB, L2DS_PRINT_FDB_TIME, 0);
			CmL2dsFdbPrint();
			
			{
				tIssPortList		PortListTrust;
				tIssPortList		PortListUnTrust;

				CmAPICfaPhyPortListGet(PortListTrust, PortListUnTrust);
			}
			*/
		}
		
        pTmr = TmrGetNextExpiredTimer(L2DS_TMR_LIST);
    }
	return L2DS_SUCCESS;
}

/*****************************************************************************
 *
 *    Function Name        : CmL2dsMibRestoreOver
 *
 *    Description        : When mib restore is over, we will exec this func
 *
 *    Input(s)            :None
 *
 *    Output(s)            : None.
 *
 *    Returns            : L2DS_SUCCESS/L2DS_FAILURE 
 *
 *****************************************************************************/
INT4
CmL2dsMibRestoreOver(VOID)
{
	if(L2DS_ADMIN_STATUS(L2DS_DEFAULT_CXT_ID) == L2DS_ENABLED)
	{
		/* Start refresh DB timer  */
		if(L2DS_BACKUP_DB_STATUS(L2DS_DEFAULT_CXT_ID) == L2DS_ENABLED)
		{
			TmrStart(L2DS_TMR_LIST, &L2DS_TMR_DB_BLK, L2DS_TMR_DB, 
				L2DS_BACKUP_INTERVAL(L2DS_DEFAULT_CXT_ID), 0);

			/* Read binding db from flash */
			CmL2dsReadDB();
		}
	}

	return L2DS_SUCCESS;
}



/*                                                                           */
/***************************** END OF FILE ***********************************/
/*                                                                           */
