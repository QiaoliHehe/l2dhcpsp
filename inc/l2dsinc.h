#ifndef _L2DSINC_H
#define _L2DSINC_H
/*****************************************************************************/
/* Copyright (C) 2007 Aricent Inc . All Rights Reserved                      */
/* Licensee Aricent Inc., 2007                                               */
/* $Id: l2dsinc.h,v 1.6 2012/03/21 13:04:53 siva Exp $                                                                    */
/*****************************************************************************/
/*    FILE  NAME            : l2dsinc.h                                     */
/*    PRINCIPAL AUTHOR      : Aricent Inc.                                   */
/*    SUBSYSTEM NAME        : L2DHCP Snoop                                   */
/*    MODULE NAME           : L2DHCP snoop Header files                      */
/*    LANGUAGE              : C                                              */
/*    TARGET ENVIRONMENT    : Any                                            */
/*    DATE OF FIRST RELEASE :                                                */
/*    AUTHOR                : Aricent Inc.                                   */
/*    DESCRIPTION           : This file contains include files               */
/*                            for L2 DHCP Snooping module                    */
/*---------------------------------------------------------------------------*/

#include "lr.h"
#include "fsvlan.h"
#include "l2iwf.h"
#include "ip.h"
#include "l2ds.h"
#include "cfa.h"
#include "vcm.h"
#include "ipdb.h"
#include "l2dscli.h"
#include "iss.h"
#include "msr.h"
#include "cmsinglefwapi.h"

#include "fssyslog.h"
#include "l2dsdefn.h"
#include "l2dstdfs.h"
#include "l2dsmacs.h"
#include "l2dsprot.h"
#include "fsdhcslw.h"
#include "fsdhcswr.h"
#include "fsmidhlw.h"
#include "fsmidhwr.h"
#include "l2dssz.h"
/*CAMEOTAG:add by maoqin 2012-6-12*/
#include "cmpf2api.h"

/*CAMETAG:Add by Ricann 20120924*/
#include "cmipdbapi.h"
#include "cmvlanapi.h"
#include "cml2dsapi.h"
#include "cmlaapi.h"
#include "cmcfaapi.h"
#include "cmdsrule.h"

/*CAMETAG:Add by youchao  2013-01-21*/
#include "cmdstrap.h"
#include "cmdsapi.h"

/* CAMEOTAG: add by Bob Yu 2015/6/5 */
#include "sizereg.h"

#endif /*_L2DSINC_H */
