/********************************************************************
 * Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *
 * $Id: dhcpstrap.c,v 1.8 2009/07/08 13:36:15 prabuc-iss Exp $
 *
 * Description:This file contains the routines for SNMP Trap     
 *             for DHCP module.
 *
 *******************************************************************/
#include "fssnmp.h"
#include "snmputil.h"
#include "l2dsinc.h"
#include "l2dsextn.h"

/*****************************************************************************/
/*                                                                           */
/* Function     : CmL2DhcpSNPSnmpSendTrap                                           */
/*                                                                           */
/* Description  : Routine to send trap to SNMP .                             */
/*                                                                           */
/* Input        : i4TrapId   : Trap Identifier                               */
/*                pi1TrapOid : Pointer to the trap OID                       */
/*                u1OidLen   : OID Length                                    */
/*                pTrapInfo  : Pointer to the trap information.              */
/*                                                                           */
/* Output       : None                                                       */
/*                                                                           */
/* Returns      : None                                                       */
/*                                                                           */
/*****************************************************************************/
VOID
CmL2DhcpSNPSnmpSendTrap (UINT1 u1TrapId, INT1 *pi1TrapOid, UINT1 u1OidLen,
                   VOID *pTrapInfo)
{
#ifdef SNMP_2_WANTED
    tDhcpSNPUtlTrap   *pDhcpSNPUtlTrap;
    tSNMP_VAR_BIND     *pVbList = NULL;
    tSNMP_VAR_BIND     *pStartVb = NULL;
    tSNMP_OID_TYPE     *pEnterpriseOid;
    tSNMP_OID_TYPE     *pOid;
    UINT1               au1Buf[DHCP_SNP_MAX_OBJ_LEN];
    UINT4               u4GenTrapType;
    UINT4               u4SpecTrapType;
    tSNMP_COUNTER64_TYPE SnmpCounter64Type;

    UNUSED_PARAM (u1OidLen);

    pEnterpriseOid = (tSNMP_OID_TYPE *) SNMP_AGT_GetOidFromString (pi1TrapOid);
    if (pEnterpriseOid == NULL)
    {
        return;
    }
    u4GenTrapType = ENTERPRISE_SPECIFIC;
    u4SpecTrapType = (UINT4) u1TrapId;
    SnmpCounter64Type.msn = 0;
    SnmpCounter64Type.lsn = 0;

    switch (u1TrapId)
    {

        case DHCP_SNP_UTIL_TRAP:

            pDhcpSNPUtlTrap = (tDhcpSNPUtlTrap *) pTrapInfo;
           /* For a IF recv Pkt from untrust port  Trap, ifIndex have to be notified through the TRAP. */
            SPRINTF ((char *) au1Buf, "ifIndex");

	      /* Pass the Object Name ifIndex to get the SNMP Oid */
            pOid = CmDhcpSnpMakeObjIdFromDotNew ((UINT1 *) au1Buf);	   
            if (pOid == NULL)
            {
                /* Release the memory Assigned for Entrerprise Oid */
		   free_oid (pEnterpriseOid);
                return;
            }
            pOid->pu4_OidList[SNMP_TRAP_OID_LEN - 1] = pDhcpSNPUtlTrap->i4PortNum;
            pOid->u4_Length = SNMP_TRAP_OID_LEN;
            pVbList =
                SNMP_AGT_FormVarBind (pOid, SNMP_DATA_TYPE_INTEGER32, 0,
                                      (INT4) pDhcpSNPUtlTrap->i4PortNum,
                                      NULL, NULL, SnmpCounter64Type);
            pStartVb = pVbList;

            break;

        default:
            free_oid (pEnterpriseOid);
            return;
    }

    /* The following API sends the Trap info to the FutureSNMP Agent. */
    SNMP_AGT_RIF_Notify_Trap (pEnterpriseOid, u4GenTrapType, u4SpecTrapType,
                              pStartVb);

#else /* SNMP_2_WANTED */
    UNUSED_PARAM (u1TrapId);
    UNUSED_PARAM (*pi1TrapOid);
    UNUSED_PARAM (u1OidLen);
    UNUSED_PARAM (pTrapInfo);
#endif /* SNMP_2_WANTED */

}



/*****************************************************************************/
/*                                                                           */
/* Function     : DhpSNPSnmpSendUntrustPortUtlTrap                                    */
/*                                                                           */
/* Description  : API to send trap to SNMP .                                 */
/*                                                                           */
/* Input        : i4PoolId       : Pool ID of the pool                       */
/*                i4Threshold    : Utilisation threshold of the pool         */
/*                pi1TrapOid     : Pointer to the trap OID                   */
/*                u1TrapOidLen   : OID Length                                */
/*                                                                           */
/* Output       : None                                                       */
/*                                                                           */
/* Returns      : None                                                       */
/*                                                                           */
/*****************************************************************************/

 VOID
CmDhpSNPSnmpSendUntrustPortUtlTrap ( INT4 i4PortNUM,
                         INT1 *pi1TrapsOid, UINT1 u1TrapOidLen)
{
#ifdef SNMP_2_WANTED
    tDhcpSNPUtlTrap    DhcpSNPUtlTrap;

    DhcpSNPUtlTrap.i4TrapId= DHCP_SNP_UTIL_TRAP;
    DhcpSNPUtlTrap.i4PortNum= i4PortNUM;

    CmL2DhcpSNPSnmpSendTrap (DHCP_SNP_UTIL_TRAP, pi1TrapsOid, u1TrapOidLen,
                       &DhcpSNPUtlTrap);

#else
    UNUSED_PARAM (i4PortNUM);
    UNUSED_PARAM (pi1TrapsOid);
    UNUSED_PARAM (u1TrapOidLen);
#endif /* SNMP_2_WANTED */

}

/**********************************************************************
* Function             : CmDhcpSnpMakeObjIdFromDotNew                      *
*                                                                     *
* Role of the function : This function returns the pointer to the     *
*                         object id of the passed object to it.       * 
* Formal Parameters    : textStr  :  pointer to the object whose      *
                                      object id is to be found        *
* Global Variables     : None                                         *
* Side Effects         : None                                         *
* Exception Handling   : None                                         *
* Use of Recursion     : None                                         *
* Return Value         : pOid : pointer to object id.                 *
**********************************************************************/

tSNMP_OID_TYPE     *
CmDhcpSnpMakeObjIdFromDotNew (UINT1 *textStr)
{
    tSNMP_OID_TYPE     *pOid = NULL;
    INT1               *pTemp = NULL, *pDot = NULL;
    UINT1               au1tempBuffer[DHCP_SNP_MAX_OBJ_LEN + 1];

    UINT4               au4IfIndex[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 1 };

    /* NOTE: Currently this function is hardcoded to return the Oid of 
     * the Object names. Once ifmib.mib is compiled with latest MIB 
     * compiler, this function will be modified to generate the OID 
     * from the given string .*/

    /* Is there an alpha descriptor at begining ?? */
    if (ISALPHA (*textStr))
    {
        pDot = ((INT1 *) STRCHR ((char *) textStr, '.'));

        /* if no dot, point to end of string */
        if (pDot == NULL)
        {
            pDot = ((INT1 *) (textStr + STRLEN ((char *) textStr)));
        }
        pTemp = ((INT1 *) textStr);
        MEMSET (au1tempBuffer, 0, DHCP_SNP_MAX_OBJ_LEN + 1);
        STRNCPY (au1tempBuffer, pTemp, (pDot - pTemp));
        au1tempBuffer[(pDot - pTemp)] = '\0';

        if ((pOid = alloc_oid (SNMP_TRAP_OID_LEN)) == NULL)
        {
            return (NULL);
        }

        if (STRCMP ("ifIndex", (INT1 *) au1tempBuffer) == 0)
        {
            MEMCPY (pOid->pu4_OidList, au4IfIndex,
                    (SNMP_TRAP_OID_LEN - 1) * sizeof (UINT4));
            pOid->u4_Length = SNMP_TRAP_OID_LEN;
            return (pOid);

        }
    }

    if (pOid != NULL)
    {
        free_oid (pOid);
    }
    return NULL;

}

