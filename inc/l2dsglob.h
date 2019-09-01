#ifndef _L2DSGLOB_H
#define _L2DSGLOB_H
/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsglob.h,v 1.6 2010/09/17 06:24:52 prabuc Exp $                                                                    */
/*****************************************************************************/
/*    FILE  NAME            : l2dsglob.h                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snoop                                   */
/*    MODULE NAME           : L2DHCP snoop Gobals                            */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains global variables            */
/*                            for L2 DHCP Snooping module                    */
/*---------------------------------------------------------------------------*/

/* Instance of global configurations */
tL2DhcpSnpGlobalConfig  gaL2DhcpSnpGlobalConfig [L2DS_MAX_CONTEXTS];

/* Instance of global Informations */
tL2DhcpSnpGlobalInfo    gL2DhcpSnpGlobalInfo;

/* Broadcast Mac address */
tMacAddr                BCastMacAddr = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

/*  Data buffer for duplicating data information */
UINT1                  *gpau1L2dsDataBuffer = NULL;

/*GLOABL  SYSLOG ID */
INT4 gi4SysLogId;

/* This is a global array for storing option value to be returned by
 * the function L2dsUtilGetOption.
 * The size is allocated to L2DS_OPTION_LEN, so that any variable length 
 * option also will be accommadated
 * */
UINT1               gau1OptionVal[L2DS_OPTION_LEN];

/*CAMEOTAG:Ricann,20130228*/
tL2DhcpSnpFdbGlob	gL2DSFdbGlob;

#endif /* _L2DSGLOB_H */
