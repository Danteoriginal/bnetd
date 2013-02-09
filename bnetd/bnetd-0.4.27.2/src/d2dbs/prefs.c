/*
 * Copyright (C) 2000,2001	Onlyer	(onlyer@263.net)
 * Copyright (C) 2001		sousou	(liupeng.cs@263.net)
 * Some DB-Storage (MySQL) modifications:
 *      Copyright (C) 2002  Joerg Ebeling (jebs@shbe.net)
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
#include "common/setup_before.h"
#include "setup.h"

#ifdef HAVE_STDDEF_H
# include <stddef.h>
#else
# ifndef NULL
#  define NULL ((void *)0)
# endif
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#else
# ifdef HAVE_STRINGS_H
#  include <strings.h>
# endif
# ifdef HAVE_MEMORY_H
#  include <memory.h>
# endif
#endif

#include "prefs.h"
#include "d2cs/conf.h"
#include "common/eventlog.h"
#ifdef WITH_STORAGE_DB
# ifdef WITH_MYSQL
#  include <mysql.h>
# endif
#endif
#include "common/setup_after.h"


typedef struct
{
        char const      * logfile;
        char const      * logfile_gs;
        char const      * loglevels;
        char const      * servaddrs;
        char const      * gameservlist;
        char const      * charsavedir;
        char const      * charsavebakdir;
        char const      * charinfodir;
        char const      * charinfobakdir;
        char const      * ladderdir;
        unsigned int    laddersave_interval;
        unsigned int    ladderinit_time;
        unsigned int    shutdown_delay;
        unsigned int    shutdown_decr;
        unsigned int    idletime;
        unsigned int    keepalive_interval;
        unsigned int    timeout_checkinterval;
	unsigned int    ladderupdate_threshold;
	#ifdef WITH_STORAGE_DB
	    char const 		* db_host;
	    unsigned int  	db_port;
	    char const 		* db_name;
	    char const 		* db_char_table;
	    char const 		* db_user;
	    char const 		* db_pw;
	#endif
} t_prefs;

static t_prefs prefs_conf;


/* FIXME: arg... pointers can't always fit into ints nor are they guaranteed to be numeric */
static t_conf_table prefs_conf_table[]={
    /*{ "logfile",              offsetof(t_prefs,logfile),              conf_type_str,  (const int)DEFAULT_LOG_FILE        },*/
    /*{ "logfile-gs",           offsetof(t_prefs,logfile_gs),           conf_type_str,  (const int)DEFAULT_LOG_FILE_GS     },*/
    /*{ "loglevels",            offsetof(t_prefs,loglevels),            conf_type_str,  (const int)DEFAULT_LOG_LEVELS      },*/
    /*{ "servaddrs",            offsetof(t_prefs,servaddrs),            conf_type_str,  (const int)D2DBS_SERVER_ADDRS      },*/
    /*{ "gameservlist",         offsetof(t_prefs,gameservlist),         conf_type_str,  (const int)D2GS_SERVER_LIST        },*/
    /*{ "charsavedir",          offsetof(t_prefs,charsavedir),          conf_type_str,  (const int)D2DBS_CHARSAVE_DIR      },*/
    /*{ "charinfodir",          offsetof(t_prefs,charinfodir),          conf_type_str,  (const int)D2DBS_CHARINFO_DIR      },*/
    /*{ "bak_charsavedir",      offsetof(t_prefs,charsavebakdir),       conf_type_str,  (const int)D2DBS_CHARSAVEBAK_DIR   },*/
    /*{ "bak_charinfodir",      offsetof(t_prefs,charinfobakdir),       conf_type_str,  (const int)D2DBS_CHARINFOBAK_DIR   },*/
    /*{ "ladderdir",            offsetof(t_prefs,ladderdir),            conf_type_str,  (const int)D2DBS_LADDER_DIR        },*/
    { "laddersave_interval",  offsetof(t_prefs,laddersave_interval),  conf_type_int,  3600                         },
    { "ladderinit_time",      offsetof(t_prefs,ladderinit_time),      conf_type_int,  0                            },
    { "shutdown_delay",       offsetof(t_prefs,shutdown_delay),       conf_type_int,  DEFAULT_SHUTDOWN_DELAY       },
    { "shutdown_decr",        offsetof(t_prefs,shutdown_decr),        conf_type_int,  DEFAULT_SHUTDOWN_DECR        },
    { "idletime",             offsetof(t_prefs,idletime),             conf_type_int,  DEFAULT_IDLETIME             },
    { "keepalive_interval",   offsetof(t_prefs,keepalive_interval),   conf_type_int,  DEFAULT_KEEPALIVE_INTERVAL   },
    { "timeout_checkinterval",offsetof(t_prefs,timeout_checkinterval),conf_type_int,  DEFAULT_TIMEOUT_CHECKINTERVAL},
    { "ladderupdate_threshold",offsetof(t_prefs,ladderupdate_threshold),conf_type_int,  DEFAULT_LADDERUPDATE_THRESHOLD},
#ifdef WITH_STORAGE_DB
    { "db_host",              offsetof(t_prefs,db_host),              conf_type_str,  (const int)LOCAL_HOST              },
    { "db_port",              offsetof(t_prefs,db_port),              conf_type_int,  MYSQL_PORT                   },
    { "db_name",              offsetof(t_prefs,db_name),              conf_type_str,  (const int)DB_NAME                 },
    { "db_char_table",        offsetof(t_prefs,db_char_table),        conf_type_str,  (const int)DB_CHAR_TABLE           },
    { "db_user",              offsetof(t_prefs,db_user),              conf_type_str,  (const int)""                      },
    { "db_pw",                offsetof(t_prefs,db_pw),                conf_type_str,  (const int)""                      },
#endif
    { NULL,                   0,                                      conf_type_none, 0                            }
};


extern int prefs_load(char const * filename)
{
	memset(&prefs_conf,0,sizeof(prefs_conf));
	if (conf_load_file(filename,prefs_conf_table,&prefs_conf,sizeof(prefs_conf))<0) {
		return -1;
	}
	return 0;
}

extern int prefs_reload(char const * filename)
{
        prefs_unload();
        if (prefs_load(filename)<0) return -1;
        return 0;
}

extern int prefs_unload(void)
{
	return conf_cleanup(prefs_conf_table, &prefs_conf, sizeof(prefs_conf));
}

extern char const * prefs_get_servaddrs(void)
{
	return prefs_conf.servaddrs;
}

extern char const * prefs_get_charsave_dir(void)
{
	return prefs_conf.charsavedir;
}

extern char const * prefs_get_charinfo_dir(void)
{
	return prefs_conf.charinfodir;
}

extern char const * prefs_get_d2gs_list(void)
{
	return prefs_conf.gameservlist;
}

extern char const * prefs_get_ladder_dir(void)
{
	return prefs_conf.ladderdir;
}

extern char const * prefs_get_logfile(void)
{
	return prefs_conf.logfile;
}

extern char const * prefs_get_logfile_gs(void)
{
	return prefs_conf.logfile_gs;
}

extern char const * prefs_get_charsave_bak_dir(void)
{
	return prefs_conf.charsavebakdir;
}

extern char const * prefs_get_charinfo_bak_dir(void)
{
	return prefs_conf.charinfobakdir;
}

extern unsigned int prefs_get_laddersave_interval(void)
{
	return prefs_conf.laddersave_interval;
}

extern unsigned int prefs_get_ladderinit_time(void)
{
	return prefs_conf.ladderinit_time;
}

extern char const * prefs_get_loglevels(void)
{
	return prefs_conf.loglevels;
}

extern unsigned int prefs_get_shutdown_delay(void)
{
        return prefs_conf.shutdown_delay;
}

extern unsigned int prefs_get_shutdown_decr(void)
{
        return prefs_conf.shutdown_decr;
}

extern unsigned int prefs_get_keepalive_interval(void)
{
        return prefs_conf.keepalive_interval;
}

extern unsigned int prefs_get_idletime(void)
{
        return prefs_conf.idletime;
}

extern unsigned int prefs_get_timeout_checkinterval(void)
{
	return prefs_conf.timeout_checkinterval;
}
extern unsigned int prefs_get_ladderupdate_threshold(void)
{
	return prefs_conf.ladderupdate_threshold;
}

#ifdef WITH_STORAGE_DB
extern char const * prefs_get_db_host(void)
{
	return prefs_conf.db_host;
}

extern unsigned int prefs_get_db_port(void)
{
	return prefs_conf.db_port;
}

extern char const * prefs_get_db_name(void)
{
	return prefs_conf.db_name;
}

extern char const * prefs_get_db_char_table(void)
{
	return prefs_conf.db_char_table;
}

extern char const * prefs_get_db_user(void)
{
	return prefs_conf.db_user;
}

extern char const * prefs_get_db_pw(void)
{
	return prefs_conf.db_pw;
}
#endif
