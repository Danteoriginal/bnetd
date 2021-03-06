/*
 * Copyright (C) 2001		sousou	(liupeng.cs@263.net)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#ifndef INLCUDED_D2DBS_SETUP_H
#define INLCUDED_D2DBS_SETUP_H

/* FIXME: each and every use of this value as an array length is a _bug_ */
#if !defined(MAX_PATH) && !defined(WIN32)
# define MAX_PATH		1024
#endif


#ifndef WIN32
typedef unsigned int BOOL;
#endif

#define TRUE			1
#define FALSE			0
#define tf(a)			((a)?1:0)

#define SELECT_TIME_OUT		20000
#define kBufferSize		(1024*20)
#define kMaxPacketLength	(1024*5)

#define MAX_GAMEPASS_LEN	16
#define MAX_GAMEDESC_LEN	32
#define MAX_NAME_LEN		16
#define MAX_CHARNAME_LEN	16
#define MAX_ACCTNAME_LEN	16
#define MAX_GAMENAME_LEN	16
#define MAX_REALMNAME_LEN	32

#define DEFAULT_LOG_FILE		"/usr/local/var/d2dbs.log"
#define DEFAULT_LOG_FILE_GS		"/usr/local/var/d2dbs-gs.log"
#define DEFAULT_LOG_LEVELS		"info,warn,error"
#define D2DBS_CHARSAVE_DIR		"/usr/local/var/charsave"
#define D2DBS_CHARINFO_DIR		"/usr/local/var/charinfo"
#define D2DBS_CHARSAVEBAK_DIR		"/usr/local/var/bak/charsave"
#define D2DBS_CHARINFOBAK_DIR		"/usr/local/var/bak/charinfo"
#define D2DBS_LADDER_DIR		"/usr/local/var/ladders"
#define DEFAULT_MEMLOG_FILE		"/tmp/d2dbs-mem.log"
#define DEFAULT_LISTEN_PORT		6114
#define D2DBS_SERVER_ADDRS		"0.0.0.0"
#define D2GS_SERVER_LIST		"192.168.0.1"
#define LOG_LEVEL			LOG_MSG
#define DEFAULT_GS_MAX			256
#define DEFAULT_SHUTDOWN_DELAY          300
#define DEFAULT_SHUTDOWN_DECR           60
#define DEFAULT_IDLETIME		300
#define DEFAULT_KEEPALIVE_INTERVAL	60
#define DEFAULT_TIMEOUT_CHECKINTERVAL	60
#define DEFAULT_LADDERUPDATE_THRESHOLD	0

#endif
