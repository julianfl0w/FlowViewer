/*
 * Copyright (c) 2001 Mark Fullmer and The Ohio State University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *      $Id: ftlib.h,v 1.94 2005/05/10 15:51:33 maf Exp $
 */

#ifndef FTLIB_H
#define FTLIB_H

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/nameser.h>
#include <sys/resource.h>
#include <stdio.h>
#include <zlib.h>

#ifndef BIG_ENDIAN
#define BIG_ENDIAN 4321
#endif

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif

#ifndef BYTE_ORDER  
#define BYTE_ORDER BIG_ENDIAN
#endif

#define SWAPINT32(y) y = \
((((y)&0xff)<<24) | (((y)&0xff00)<<8) | (((y)&0xff0000)>>8) | (((y)>>24)&0xff));

#define SWAPINT16(y) y = \
    ( (((y)&0xff)<<8) | (((y)&0xff00)>>8) );

#include "ftqueue.h"

/*
 * compatability
 */
struct mymsghdr {
        void            *msg_name;             /* optional address */
        int             msg_namelen;           /* size of address */
        struct iovec    *msg_iov;              /* scatter/gather array */
        int             msg_iovlen;            /* # elements in msg_iov */
        void            *msg_control;          /* ancillary data, see below */
        int             msg_controllen;        /* ancillary data buffer len */
        int             msg_flags;             /* flags on received message */
};

/*
 * misc
 */
#define FT_OPT_PERCENT 0x1
#define FT_OPT_NAMES   0x2
#define FT_OPT_NOBUF   0x4
#define FT_OPT_TALLY   0x8
#define FT_OPT_WIDE    0x10

/*
 * stream header flags and options
 */
#define FT_HEADER_LITTLE_ENDIAN     1      /* stream data is little endian */
#define FT_HEADER_BIG_ENDIAN        2      /* stream data is big endian */
#define FT_HEADER_FLAG_DONE         0x1    /* complete, safe to read */
#define FT_HEADER_FLAG_COMPRESS     0x2    /* compression enabled */
#define FT_HEADER_FLAG_MULT_PDU     0x4    /* multiple PDU's XXX not used */
#define FT_HEADER_FLAG_STREAMING    0x8    /* stream ie flow-cat */
#define FT_HEADER_FLAG_XLATE        0x10   /* stream translated from old fmt */
#define FT_HEADER_FLAG_PRELOADED    0x20   /* streaming & preloaded header */
#define FT_HEADER_D_VERSION_UNKNOWN 0xFFFF /* unknown export format */
#define FT_HEADER_MAGIC1            0xCF   /* magic number of stream */
#define FT_HEADER_MAGIC2            0x10

#define FT_HEADER1_CMNT_LEN         256    /* length of comment buffer */
#define FT_HEADER1_HN_LEN           68     /* length of hostname buffer */

#define FT_HOSTNAME_LEN             256

/* stream flags */
#define FT_IO_FLAG_ZINIT       0x1    /* compression initialized */
#define FT_IO_FLAG_NO_SWAP     0x2    /* do not swap on write */
#define FT_IO_FLAG_READ        0x4    /* stream is open for reading */
#define FT_IO_FLAG_WRITE       0x8    /* stream is open for writing */
#define FT_IO_FLAG_HEADER_DONE 0x10   /* header written */
#define FT_IO_FLAG_MMAP        0x20   /* use mmap() for reading */

#define FT_PDU_V1_MAXFLOWS    24  /* max records in V1 packet */
#define FT_PDU_V5_MAXFLOWS    30  /* max records in V5 packet */
#define FT_PDU_V6_MAXFLOWS    27  /* max records in V6 packet */
#define FT_PDU_V7_MAXFLOWS    27  /* max records in V7 packet */
#define FT_PDU_V8_1_MAXFLOWS  51  /* max records in V8 AS packet */
#define FT_PDU_V8_2_MAXFLOWS  51  /* max records in V8 PROTO PORT packet */
#define FT_PDU_V8_3_MAXFLOWS  44  /* max records in V8 SRC PREFIX packet */
#define FT_PDU_V8_4_MAXFLOWS  44  /* max records in V8 DST PREFIX packet */
#define FT_PDU_V8_5_MAXFLOWS  35  /* max records in V8 PREFIX packet */
#define FT_PDU_V8_6_MAXFLOWS  44  /* max records in V8 DESTONLY packet */
#define FT_PDU_V8_7_MAXFLOWS  35  /* max records in V8 SRC_DEST packet */
#define FT_PDU_V8_8_MAXFLOWS  32  /* max records in V8 FULL_FLOW packet */
#define FT_PDU_V8_9_MAXFLOWS  44  /* max records in V8 AS_TOS packet */
#define FT_PDU_V8_10_MAXFLOWS 44  /* max records in V8 PROT_PORT_TOS packet */
#define FT_PDU_V8_11_MAXFLOWS 44  /* max records in V8 SRC_PREFIX_TOS packet */
#define FT_PDU_V8_12_MAXFLOWS 44  /* max records in V8 DST_PREFIX_TOS packet */
#define FT_PDU_V8_13_MAXFLOWS 35  /* max records in V8 PREFIX_TOS packet */
#define FT_PDU_V8_14_MAXFLOWS 35  /* max records in V8 PREFIX_PORT_TOS packet */

#define FT_PDU_V8_1_VERSION    2  /* version of AS packet */
#define FT_PDU_V8_2_VERSION    2  /* version of PROTO PORT packet */
#define FT_PDU_V8_3_VERSION    2  /* version of SRC PREFIX packet */
#define FT_PDU_V8_4_VERSION    2  /* version of DST PREFIX packet */
#define FT_PDU_V8_5_VERSION    2  /* version of PREFIX packet */
#define FT_PDU_V8_6_VERSION    2  /* version of DESTONLY packet */
#define FT_PDU_V8_7_VERSION    2  /* version of SRC_DEST packet */
#define FT_PDU_V8_8_VERSION    2  /* version of FULL_FLOW packet */
#define FT_PDU_V8_9_VERSION    2  /* version of AS_TOS packet */
#define FT_PDU_V8_10_VERSION   2  /* version of PROT_PORT_TOS packet */
#define FT_PDU_V8_11_VERSION   2  /* version of SRC_PREFIX_TOS packet */
#define FT_PDU_V8_12_VERSION   2  /* version of DST_PREFIX_TOS packet */
#define FT_PDU_V8_13_VERSION   2  /* version of PREFIX_TOS packet */
#define FT_PDU_V8_14_VERSION   2  /* version of PREFIX_PORT_TOS packet */

#define FT_PORT                9991  /* default listen port */
#define FT_IO_NBUFS            256   /* buffers to read/write */
#define FT_Z_BUFSIZE           16384 /* inflate/deflate buffer size */
#define FT_D_BUFSIZE           32768 /* stream data buffer size */
#define FT_RCV_BUFSIZE         2048  /* enough to handle largest export */
#define FT_SO_SND_BUFSIZE      1500  /* UDP send socket buffer size */
#define FT_SO_RCV_BUFSIZE      (4*1024*1024) /* UDP recv socket buffer size */

#define FT_IO_SVERSION         3     /* stream version */

#define FT_IO_MAXREC           512   /* >= max size of a flow record fts3_* */

#define FT_IO_MAXDECODE        4096  /* must be >= max possible size a pdu
                                      * could expand into stream records.  For
                                      * example 27 v7 streams at 60 bytes
                                      * is 1620 bytes
                                     */

#define FT_IO_MAXENCODE        4096  /* must be >= max possible size a pdu
                                      * could be. really
                                      * MAX(sizeof(ftpdu_*)) + size of
                                      * ip+udp header (20+8)
                                     */

#define FT_IO_MAXHEADER        16384  /* max size of header */



#define FT_SEQ_RESET           1000   /* maximum number of sequence numbers
                                       * that will be counted as lost before
                                       * assuming reset.
                                       */

#define FT_ENC_FLAGS_IPHDR     0x1    /* leave room for IP header */

#define FT_ENC_IPHDR_LEN       28     /* IP + UDP header length */

#define FT_FILE_SORT           0x1    /* sort entries */
#define FT_FILE_INIT           0x2    /* initialize */
#define FT_FILE_SKIPTMP        0x4    /* skip tmp files */
#define FT_FILE_CHECKNAMES     0x8    /* check filenames for sanity */

#define FT_LP_MAXLINE          1024   /* maximum length of line */

/* TLV_ - stream value
 * DEC_ - bit in internal header to indicate successful decode
 */

#define FT_TLV_NULL               0x0     /* 0       : null */
#define FT_TLV_VENDOR             0x1     /* uint8_t  : vendor (1=cisco) */
#define FT_FIELD_VENDOR           0x00000001L
#define FT_TLV_EX_VER             0x2     /* uint16_t : export version */
#define FT_FIELD_EX_VER           0x00000002L
#define FT_TLV_AGG_VER            0x3     /* uint8_t  : aggregation version */
#define FT_FIELD_AGG_VER          0x00000004L
#define FT_TLV_AGG_METHOD         0x4     /* uint8_t  : aggregation method */
#define FT_FIELD_AGG_METHOD       0x00000008L
#define FT_TLV_EXPORTER_IP        0x5     /* uint32_t : IP of exporter */
#define FT_FIELD_EXPORTER_IP      0x00000010L
#define FT_TLV_CAP_START          0x6     /* uint32_t : capture start time */
#define FT_FIELD_CAP_START        0x00000020L
#define FT_TLV_CAP_END            0x7     /* uint32_t : capture end time */
#define FT_FIELD_CAP_END          0x00000040L
#define FT_TLV_HEADER_FLAGS       0x8     /* uint32_t : FT_HEADER_FLAG_* */
#define FT_FIELD_HEADER_FLAGS     0x00000080L
#define FT_TLV_ROT_SCHEDULE       0x9     /* uint32_t : rotation schedule */
#define FT_FIELD_ROT_SCHEDULE     0x00000100L
#define FT_TLV_FLOW_COUNT         0xA     /* uint32_t : num flows */
#define FT_FIELD_FLOW_COUNT       0x00000200L
#define FT_TLV_FLOW_LOST          0xB     /* uint32_t : lost flows */
#define FT_FIELD_FLOW_LOST        0x00000400L
#define FT_TLV_FLOW_MISORDERED    0xC     /* uint32_t : misordered flows */
#define FT_FIELD_FLOW_MISORDERED  0x00000800L
#define FT_TLV_PKT_CORRUPT        0xD     /* uint32_t : corrupt packets */
#define FT_FIELD_PKT_CORRUPT      0x00001000L
#define FT_TLV_SEQ_RESET          0xE     /* uint32_t : times sequence # was so
                                           *           far off lost/misordered
                                           *           state could not be
                                           *           determined */
#define FT_FIELD_SEQ_RESET        0x00002000L
#define FT_TLV_CAP_HOSTNAME       0xF     /* string  : hostname of capture dev */
#define FT_FIELD_CAP_HOSTNAME     0x00004000L
#define FT_TLV_COMMENTS           0x10    /* string  : comments */
#define FT_FIELD_COMMENTS         0x00008000L
#define FT_TLV_IF_NAME            0x11    /* uint32_t uint16_t string 
                                           * IP address of device
                                           * ifIndex of interface
                                           * interface name */
#define FT_FIELD_IF_NAME          0x00010000L
#define FT_TLV_IF_ALIAS           0x12    /* uint32_t uint16_t uint16_t string 
                                           * IP address of device
                                           * ifIndex count
                                           * ifIndex of interface (count times)
                                           * alias name */
#define FT_FIELD_IF_ALIAS         0x00020000L

#define FT_TLV_INTERRUPT          0x13     /* uint8_t : interrupt processing 
                                            * code for interrupt (0) */
#define FT_FIELD_INTERRUPT        0x00040000L

#define FT_VENDOR_CISCO           0x1      /* Cisco exporter */

#define FT_CHASH_SORTED           0x1
#define FT_CHASH_SORT_ASCENDING   0x2
#define FT_CHASH_SORT_16          0x4
#define FT_CHASH_SORT_32          0x8
#define FT_CHASH_SORT_40          0x10
#define FT_CHASH_SORT_64          0x20
#define FT_CHASH_SORT_DOUBLE      0x40
#define FT_CHASH_SORT_8           0x80


/* possible fields in export */
#define FT_XFIELD_UNIX_SECS       0x0000000000000001LL
#define FT_XFIELD_UNIX_NSECS      0x0000000000000002LL
#define FT_XFIELD_SYSUPTIME       0x0000000000000004LL
#define FT_XFIELD_EXADDR          0x0000000000000008LL

#define FT_XFIELD_DFLOWS          0x0000000000000010LL
#define FT_XFIELD_DPKTS           0x0000000000000020LL
#define FT_XFIELD_DOCTETS         0x0000000000000040LL
#define FT_XFIELD_FIRST           0x0000000000000080LL

#define FT_XFIELD_LAST            0x0000000000000100LL
#define FT_XFIELD_ENGINE_TYPE     0x0000000000000200LL
#define FT_XFIELD_ENGINE_ID       0x0000000000000400LL

#define FT_XFIELD_SRCADDR         0x0000000000001000LL
#define FT_XFIELD_DSTADDR         0x0000000000002000LL

#define FT_XFIELD_NEXTHOP         0x0000000000010000LL
#define FT_XFIELD_INPUT           0x0000000000020000LL
#define FT_XFIELD_OUTPUT          0x0000000000040000LL
#define FT_XFIELD_SRCPORT         0x0000000000080000LL

#define FT_XFIELD_DSTPORT         0x0000000000100000LL
#define FT_XFIELD_PROT            0x0000000000200000LL
#define FT_XFIELD_TOS             0x0000000000400000LL
#define FT_XFIELD_TCP_FLAGS       0x0000000000800000LL

#define FT_XFIELD_SRC_MASK        0x0000000001000000LL
#define FT_XFIELD_DST_MASK        0x0000000002000000LL
#define FT_XFIELD_SRC_AS          0x0000000004000000LL
#define FT_XFIELD_DST_AS          0x0000000008000000LL

#define FT_XFIELD_IN_ENCAPS       0x0000000010000000LL
#define FT_XFIELD_OUT_ENCAPS      0x0000000020000000LL
#define FT_XFIELD_PEER_NEXTHOP    0x0000000040000000LL
#define FT_XFIELD_ROUTER_SC       0x0000000080000000LL

#define FT_XFIELD_EXTRA_PKTS      0x0000000100000000LL
#define FT_XFIELD_MARKED_TOS      0x0000000200000000LL
#define FT_XFIELD_SRC_TAG         0x0000000400000000LL
#define FT_XFIELD_DST_TAG         0x0000000800000000LL

#define FT_XFIELD_V1_MASK         0x0000000000FF31EFLL
#define FT_XFIELD_V5_MASK         0x000000000FFF37EFLL
#define FT_XFIELD_V6_MASK         0x000000007FFF37EFLL
#define FT_XFIELD_V7_MASK         0x000000008FFF37EFLL
#define FT_XFIELD_V8_1_MASK       0x000000000C0607FFLL
#define FT_XFIELD_V8_2_MASK       0x00000000003807FFLL
#define FT_XFIELD_V8_3_MASK       0x00000000050217FFLL
#define FT_XFIELD_V8_4_MASK       0x000000000A0427FFLL
#define FT_XFIELD_V8_5_MASK       0x000000000F0637FFLL
#define FT_XFIELD_V8_6_MASK       0x00000003804427EFLL
#define FT_XFIELD_V8_7_MASK       0x00000003804637EFLL
#define FT_XFIELD_V8_8_MASK       0x00000003807E37EFLL
#define FT_XFIELD_V8_9_MASK       0x000000000C4607FFLL
#define FT_XFIELD_V8_10_MASK      0x00000000007807FFLL
#define FT_XFIELD_V8_11_MASK      0x00000000054217FFLL
#define FT_XFIELD_V8_12_MASK      0x000000000A4427FFLL
#define FT_XFIELD_V8_13_MASK      0x000000000F4637FFLL
#define FT_XFIELD_V8_14_MASK      0x00000000037E37FFLL
#define FT_XFIELD_V1005_MASK      0x0000000C0FFF37EFLL
#define FT_XFIELD_TAGGING_MASK    0x000000000FFF37EFLL

#define FT_XFIELD_ASC_UNIX_SECS "unix_secs"
#define FT_XFIELD_ASC_UNIX_NSECS "unix_nsecs"
#define FT_XFIELD_ASC_SYSUPTIME "sysuptime"
#define FT_XFIELD_ASC_EXADDR "exaddr"

#define FT_XFIELD_ASC_DFLOWS "dflows"
#define FT_XFIELD_ASC_DPKTS "dpkts"
#define FT_XFIELD_ASC_DOCTETS "doctets"
#define FT_XFIELD_ASC_FIRST "first"

#define FT_XFIELD_ASC_LAST "last"
#define FT_XFIELD_ASC_ENGINE_TYPE "engine_type"
#define FT_XFIELD_ASC_ENGINE_ID "engine_id"

#define FT_XFIELD_ASC_SRCADDR "srcaddr"
#define FT_XFIELD_ASC_DSTADDR "dstaddr"

#define FT_XFIELD_ASC_NEXTHOP "nexthop"
#define FT_XFIELD_ASC_INPUT "input"
#define FT_XFIELD_ASC_OUTPUT "output"
#define FT_XFIELD_ASC_SRCPORT "srcport"

#define FT_XFIELD_ASC_DSTPORT "dstport"
#define FT_XFIELD_ASC_PROT "prot"
#define FT_XFIELD_ASC_TOS "tos"
#define FT_XFIELD_ASC_TCP_FLAGS "tcp_flags"

#define FT_XFIELD_ASC_SRC_MASK "src_mask"
#define FT_XFIELD_ASC_DST_MASK "dst_mask"
#define FT_XFIELD_ASC_SRC_AS "src_as"
#define FT_XFIELD_ASC_DST_AS "dst_as"

#define FT_XFIELD_ASC_IN_ENCAPS "in_encaps"
#define FT_XFIELD_ASC_OUT_ENCAPS "out_encaps"
#define FT_XFIELD_ASC_PEER_NEXTHOP "peer_nexthop"
#define FT_XFIELD_ASC_ROUTER_SC "router_sc"

#define FT_XFIELD_ASC_MARKED_TOS "marked_tos"
#define FT_XFIELD_ASC_EXTRA_PKTS "extra_pkts"
#define FT_XFIELD_ASC_SRC_TAG "src_tag"
#define FT_XFIELD_ASC_DST_TAG "dst_tag"

struct ftipmask {
  uint32_t src_mask;
  uint32_t dst_mask;
  uint32_t mcast_mask;
  uint32_t mcast_val;
};


struct fttlv {
  uint16_t t, l;         /* type, length */
  char *v;              /* value */
};

struct ftprof {
  struct timeval  t0;   /* start time */
  struct timeval  t1;   /* end time */
  struct rusage r0;     /* system resources used */
  uint64_t nflows;       /* total # of flows processed */
};

struct ftset {
  int byte_order;
  int z_level;
  char *hostname;
  char *comments;
  char hnbuf[FT_HOSTNAME_LEN];
  uint16_t as_sub;
};

struct fttime {
  uint32_t secs;
  uint32_t msecs;
};

struct ftver {
  uint8_t s_version;
  uint8_t agg_version;
  uint8_t agg_method;
  uint8_t set;
  uint16_t d_version;
};

struct ftdecode {
  char buf[FT_IO_MAXDECODE]; /* buffer PDU decoded into - num records */
  int count;                 /* number of stream records */
  int rec_size;              /* size of stream record */
  int byte_order;            /* byte order to decode to */
  uint32_t exporter_ip;       /* ip address of exporter */
  uint16_t as_sub;            /* replace AS0 with this */
};

struct ftencode {
  char buf[FT_IO_MAXENCODE]; /* buffer stream encodes into */
  char *buf_enc;             /* actual encode buffer, will not be &buf if
                                FT_ENC_FLAGS_IPHDR is set */
  int buf_size;              /* bytes used in buf_enc */
  struct ftver ver;          /* version of stream encoding */
  uint32_t seq_next[65536];   /* sequence number for each engine_type & id */
  int flags;                 /* FT_ENC_FLAGS */
  int d_sum;                 /* data checksum for buf_enc */
};

struct ftpdu {
  char buf[FT_RCV_BUFSIZE];  /* raw packet */
  int bused;                 /* length of buf used */
  struct ftver ftv;          /* version of PDU */
  struct ftdecode ftd;       /* decoded records */
  int (*decodef)             /* decode function */
    (struct ftpdu *ftpdu);
};

struct ftseq {
  uint32_t seq[65536];        /* sequence number for each engine_type & id */
  uint8_t seq_set[65536];     /* initial sequence number received? */
  uint32_t seq_rcv, seq_exp,
          seq_lost;          /* sequence # received / expecting / lost */
};

struct ftheader_gen {
  uint8_t  magic1;                 /* 0xCF */
  uint8_t  magic2;                 /* 0x10 (cisco flow) */
  uint8_t  byte_order;             /* 1 for little endian (VAX) */
                                  /* 2 for big endian (Motorolla) */
  uint8_t  s_version;              /* flow stream format version 1 or 3 */
};

struct ftnet {
  struct sockaddr_in loc_addr;    /* local side */
  struct sockaddr_in rem_addr;    /* remote side (exporter) */
  uint32_t loc_ip;                 /* local IP */
  uint32_t rem_ip;                 /* remote IP */
  uint16_t dst_port;               /* exporter UDP destination port */
  int fd;                         /* fd receiving flows on */
  struct mymsghdr msg;            /* recvmsg data */
  struct {
#ifdef IP_RECVDSTADDR
#ifdef CMSG_DATA
    char cbuf[CMSG_SPACE(sizeof(struct sockaddr_storage))];
#else
    struct cmsghdr hdr;
    struct in_addr ip;
#endif /* CMSG_DATA */
#else
#ifdef IP_PKTINFO
    struct cmsghdr hdr;
    struct in_pktinfo pktinfo;
#endif /* else */
#endif /* IP RECVDSTADDR */
  } msgip;
  struct iovec iov[1];            /* msg buffer */
};

struct ftmap_ifalias {
  uint32_t ip;
  uint16_t entries;
  uint16_t *ifIndex_list;
  char *name;
  FT_LIST_ENTRY (ftmap_ifalias) chain;
};

struct ftmap_ifname {
  uint32_t ip;
  uint16_t ifIndex;
  char *name;
  FT_LIST_ENTRY (ftmap_ifname) chain;
};

struct ftmap {
  FT_LIST_HEAD(ftmap_ifaliash, ftmap_ifalias) ifalias;
  FT_LIST_HEAD(ftmap_ifnameh, ftmap_ifname) ifname;
};

#define FT_TAG_TYPE_MATCH_SRC_AS         0x1
#define FT_TAG_TYPE_MATCH_DST_AS         0x2
#define FT_TAG_TYPE_MATCH_AS             0x3
#define FT_TAG_TYPE_MATCH_TOS            0x4
#define FT_TAG_TYPE_MATCH_NEXTHOP        0x8
    
#define FT_TAG_TYPE_MATCH_SRC_PREFIX     0x10
#define FT_TAG_TYPE_MATCH_DST_PREFIX     0x20
#define FT_TAG_TYPE_MATCH_PREFIX         0x30
#define FT_TAG_TYPE_MATCH_ANY            0x40
#define FT_TAG_TYPE_MATCH_EXPORTER       0x80

#define FT_TAG_TYPE_MATCH_SRC_TCP_PORT   0x100
#define FT_TAG_TYPE_MATCH_DST_TCP_PORT   0x200
#define FT_TAG_TYPE_MATCH_TCP_PORT       0x300
#define FT_TAG_TYPE_MATCH_SRC_IP         0x400
#define FT_TAG_TYPE_MATCH_DST_IP         0x800
#define FT_TAG_TYPE_MATCH_IP             0xC00

#define FT_TAG_TYPE_MATCH_SRC_UDP_PORT   0x1000
#define FT_TAG_TYPE_MATCH_DST_UDP_PORT   0x2000
#define FT_TAG_TYPE_MATCH_UDP_PORT       0x3000
#define FT_TAG_TYPE_MATCH_IN_INTERFACE   0x4000
#define FT_TAG_TYPE_MATCH_OUT_INTERFACE  0x8000
#define FT_TAG_TYPE_MATCH_INTERFACE      0xC000

#define FT_TAG_DEF_FILTER_EXPORTER     0x1
#define FT_TAG_DEF_FILTER_INPUT        0x2
#define FT_TAG_DEF_FILTER_OUTPUT       0x4
#define FT_TAG_DEF_ACTIVE_DEFAULT      0x8  /* default_defs list is not empty */
#define FT_TAG_DEF_ACTIVE_HASH         0x10 /* exp_hash is not empty */

 
#define FT_TAG_SET_DST_TAG             0x001
#define FT_TAG_SET_SRC_TAG             0x002
#define FT_TAG_SET_TAG                 0x003

#define FT_TAG_OR_DST_TAG              0x004
#define FT_TAG_OR_SRC_TAG              0x008
#define FT_TAG_OR_TAG                  0x00C

struct fttag {
  FT_SLIST_HEAD(defshead, fttag_def) defs; /* list of definitions */
  FT_SLIST_HEAD(acthead, fttag_action) actions; /* list of actions */
  int flags;
};

struct ftxfield_table {
  char *name;
  uint64_t val;
};

/* internal representation of header */
struct ftiheader {
  uint32_t size;                   /* size of header written */
  uint32_t fields;                 /* decoded fields - FT_FIELD_* */
  uint8_t  magic1;                 /* 0xCF */
  uint8_t  magic2;                 /* 0xL0 (cisco flow) */
  uint8_t  byte_order;             /* 1 for little endian (VAX) */
                                  /* 2 for big endian (Motorolla) */
  uint8_t  s_version;              /* flow stream format version 1 or 2 */
  uint16_t d_version;              /* 1 or 5  - stream version 1 */
                                  /* 1,5,7,8 - stream version 2 */
  uint8_t  agg_version;            /* v8 aggregation version */
  uint8_t  agg_method;             /* v8 aggregation method */
  uint32_t exporter_ip;            /* IP address of exporter */
  uint32_t cap_start;              /* start time of flow capture */
  uint32_t cap_end;                /* end time of flow capture */
  uint32_t flags;                  /* FT_HEADER_FLAG_* */
  uint32_t rotation;               /* rotation schedule */
  uint32_t flows_count;            /* # flows */
  uint32_t flows_lost;             /* # lost flows */
  uint32_t flows_misordered;       /* # misordered flows */
  uint32_t pkts_corrupt;           /* # corrupt packets */
  uint32_t seq_reset;              /* # times sequence # was so far off
                                   *   lost/misordered state could not be 
                                   *   guessed */
  uint8_t vendor;                   /* vendor ID FT_VENDOR */
  uint8_t filler1;
  uint16_t filler2;
  char *cap_hostname;             /* hostname of capture device */
  char *comments;                 /* comments */
  struct ftmap *ftmap;            /* mappings */
  uint32_t enc_len;                /* length of encoded header */
};


struct fts1header {
  /*
   * following four 4 fields are not byte order dependent
   */
  uint8_t  magic1;                 /* 0xCF */
  uint8_t  magic2;                 /* 0xL0 (cisco flow) */
  uint8_t  byte_order;             /* 1 for little endian (VAX) */
                                  /* 2 for big endian (Motorolla) */
  uint8_t  s_version;              /* flow stream format version 1 or 2 */
  /*
   * byte order specified by byte_order
   */
  uint16_t d_version;                  /* 1 or 5  - stream version 1 */
                                      /* 1,5,7,8 - stream version 2 */
  uint32_t start;                      /* start time of flow capture */
  uint32_t end;                        /* end time of flow capture */
  uint32_t flags;                      /* FT_HEADER_FLAG_* */
  uint32_t rotation;                   /* rotation schedule */
  uint32_t nflows;                     /* # of flows */
  uint32_t pdu_drops;                  /* # of dropped pdu's detected */
  uint32_t pdu_misordered;             /* # of detected misordered packets */
  char hostname[FT_HEADER1_HN_LEN];   /* 0 terminated name of capture device */
  char comments[FT_HEADER1_CMNT_LEN]; /* 0 terminated ascii comments */
};

/* offsets to all possible entries in export structure */

struct fts3rec_offsets {
  struct ftver ftv;
  uint64_t xfields;
  uint16_t unix_secs;
  uint16_t unix_nsecs;
  uint16_t sysUpTime;
  uint16_t exaddr;
  uint16_t srcaddr;
  uint16_t dstaddr;
  uint16_t nexthop;
  uint16_t input;
  uint16_t output;
  uint16_t dFlows;
  uint16_t dPkts;
  uint16_t dOctets;
  uint16_t First;
  uint16_t Last;
  uint16_t srcport;
  uint16_t dstport;
  uint16_t prot;
  uint16_t tos;
  uint16_t tcp_flags;
  uint16_t pad;
  uint16_t engine_type;
  uint16_t engine_id;
  uint16_t src_mask;
  uint16_t dst_mask;
  uint16_t src_as;
  uint16_t dst_as;
  uint16_t in_encaps;
  uint16_t out_encaps;
  uint16_t peer_nexthop;
  uint16_t router_sc;
  uint16_t src_tag;
  uint16_t dst_tag;
  uint16_t extra_pkts;
  uint16_t marked_tos;
};

/* "all" fields */
struct fts3rec_all {
  uint32_t *unix_secs;
  uint32_t *unix_nsecs;
  uint32_t *sysUpTime;
  uint32_t *exaddr;
  uint32_t *srcaddr;
  uint32_t *dstaddr;
  uint32_t *nexthop;
  uint16_t *input;
  uint16_t *output;
  uint32_t *dFlows;
  uint32_t *dPkts;
  uint32_t *dOctets;
  uint32_t *First;
  uint32_t *Last;
  uint16_t *srcport;
  uint16_t *dstport;
  uint8_t  *prot;
  uint8_t  *tos;
  uint8_t  *tcp_flags;
  uint8_t  *engine_type;
  uint8_t  *engine_id;
  uint8_t  *src_mask;
  uint8_t  *dst_mask;
  uint16_t *src_as;
  uint16_t *dst_as;
  uint8_t  *in_encaps;
  uint8_t  *out_encaps;
  uint32_t *peer_nexthop;
  uint32_t *router_sc;
  uint32_t *src_tag;
  uint32_t *dst_tag;
  uint32_t *extra_pkts;
  uint8_t  *marked_tos;
};

/* "all" fields */
struct fts3rec_all2 {
  uint64_t dFlows64;
  uint64_t dPkts64;
  uint64_t dOctets64;
  uint32_t unix_secs;
  uint32_t unix_nsecs;
  uint32_t sysUpTime;
  uint32_t exaddr;
  uint32_t srcaddr;
  uint32_t dstaddr;
  uint32_t nexthop;
  uint16_t input;
  uint16_t output;
  uint32_t dFlows;
  uint32_t dPkts;
  uint32_t dOctets;
  uint32_t First;
  uint32_t Last;
  uint16_t srcport;
  uint16_t dstport;
  uint8_t  prot;
  uint8_t  tos;
  uint8_t  tcp_flags;
  uint8_t  engine_type;
  uint8_t  engine_id;
  uint8_t  src_mask;
  uint8_t  dst_mask;
  uint16_t src_as;
  uint16_t dst_as;
  uint8_t  in_encaps;
  uint8_t  out_encaps;
  uint32_t peer_nexthop;
  uint32_t router_sc;
  uint32_t src_tag;
  uint32_t dst_tag;
  uint32_t extra_pkts;
  uint8_t  marked_tos;
};

#define FT_RECGET_UNIX_SECS(A,B,C) A.unix_secs =\
  *((uint32_t*)(B+(C).unix_secs));
#define FT_RECGET_UNIX_NSECS(A,B,C) A.unix_nsecs =\
  *((uint32_t*)(B+(C).unix_nsecs));
#define FT_RECGET_SYSUPTIME(A,B,C) A.sysUpTime =\
  *((uint32_t*)(B+(C).sysUpTime));
#define FT_RECGET_EXADDR(A,B,C) A.exaddr =\
  *((uint32_t*)(B+(C).exaddr));
#define FT_RECGET_DFLOWS(A,B,C) A.dFlows =\
  *((uint32_t*)(B+(C).dFlows));
#define FT_RECGET64_DFLOWS(A,B,C) A.dFlows64 =\
  *((uint32_t*)(B+(C).dFlows));
#define FT_RECGET_DPKTS(A,B,C) A.dPkts =\
  *((uint32_t*)(B+(C).dPkts));
#define FT_RECGET64_DPKTS(A,B,C) A.dPkts64 =\
  *((uint32_t*)(B+(C).dPkts));
#define FT_RECGET_DOCTETS(A,B,C) A.dOctets =\
  *((uint32_t*)(B+(C).dOctets));
#define FT_RECGET64_DOCTETS(A,B,C) A.dOctets64 =\
  *((uint32_t*)(B+(C).dOctets));
#define FT_RECGET_FIRST(A,B,C) A.First =\
  *((uint32_t*)(B+(C).First));
#define FT_RECGET_LAST(A,B,C) A.Last =\
  *((uint32_t*)(B+(C).Last));
#define FT_RECGET_ENGINE_TYPE(A,B,C) A.engine_type =\
  *((uint8_t*)(B+(C).engine_type));
#define FT_RECGET_ENGINE_ID(A,B,C) A.engine_id =\
  *((uint8_t*)(B+(C).engine_id));
#define FT_RECGET_SRCADDR(A,B,C) A.srcaddr =\
  *((uint32_t*)(B+(C).srcaddr));
#define FT_RECGET_DSTADDR(A,B,C) A.dstaddr =\
  *((uint32_t*)(B+(C).dstaddr));
#define FT_RECGET_NEXTHOP(A,B,C) A.nexthop =\
  *((uint32_t*)(B+(C).nexthop));
#define FT_RECGET_INPUT(A,B,C) A.input =\
  *((uint16_t*)(B+(C).input));
#define FT_RECGET_OUTPUT(A,B,C) A.output =\
  *((uint16_t*)(B+(C).output));
#define FT_RECGET_SRCPORT(A,B,C) A.srcport =\
  *((uint16_t*)(B+(C).srcport));
#define FT_RECGET_DSTPORT(A,B,C) A.dstport =\
  *((uint16_t*)(B+(C).dstport));
#define FT_RECGET_PROT(A,B,C) A.prot =\
  *((uint8_t*)(B+(C).prot));
#define FT_RECGET_TOS(A,B,C) A.tos =\
  *((uint8_t*)(B+(C).tos));
#define FT_RECGET_TCP_FLAGS(A,B,C) A.tcp_flags =\
  *((uint8_t*)(B+(C).tcp_flags));
#define FT_RECGET_SRC_MASK(A,B,C) A.src_mask =\
  *((uint8_t*)(B+(C).src_mask));
#define FT_RECGET_DST_MASK(A,B,C) A.dst_mask =\
  *((uint8_t*)(B+(C).dst_mask));
#define FT_RECGET_SRC_AS(A,B,C) A.src_as =\
  *((uint16_t*)(B+(C).src_as));
#define FT_RECGET_DST_AS(A,B,C) A.dst_as =\
  *((uint16_t*)(B+(C).dst_as));
#define FT_RECGET_IN_ENCAPS(A,B,C) A.in_encaps =\
  *((uint8_t*)(B+(C).in_encaps));
#define FT_RECGET_OUT_ENCAPS(A,B,C) A.out_encaps =\
  *((uint8_t*)(B+(C).out_encaps));
#define FT_RECGET_PEER_NEXTHOP(A,B,C) A.peer_nexthop =\
  *((uint32_t*)(B+(C).peer_nexthop));
#define FT_RECGET_EXTRA_PKTS(A,B,C) A.extra_pkts =\
  *((uint32_t*)(B+(C).extra_pkts));
#define FT_RECGET_MARKED_TOS(A,B,C) A.marked_tos =\
  *((uint8_t*)(B+(C).marked_tos));
#define FT_RECGET_SRC_TAG(A,B,C) A.src_tag =\
  *((uint32_t*)(B+(C).src_tag));
#define FT_RECGET_DST_TAG(A,B,C) A.dst_tag =\
  *((uint32_t*)(B+(C).dst_tag));

struct fts3rec_gen {
  uint32_t unix_secs;      /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;     /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;      /* Current time in millisecs since router booted */
  uint32_t exaddr;         /* Exporter IP address */
  uint32_t srcaddr;        /* Source IP Address */
  uint32_t dstaddr;        /* Destination IP Address */
  uint32_t nexthop;        /* Next hop router's IP Address */
  uint16_t input;          /* Input interface index */
  uint16_t output;         /* Output interface index */
  uint32_t dPkts;          /* Packets sent in Duration */
  uint32_t dOctets;        /* Octets sent in Duration. */
  uint32_t First;          /* SysUptime at start of flow */
  uint32_t Last;           /* and of last packet of flow */
  uint16_t srcport;        /* TCP/UDP source port number or equivalent */
  uint16_t dstport;        /* TCP/UDP destination port number or equiv */
  uint8_t  prot;           /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
  uint8_t  tos;            /* IP Type-of-Service */
  uint8_t  tcp_flags;      /* OR of TCP header bits */
  uint8_t  pad;
};

struct fts3rec_v5_gen {
  uint32_t unix_secs;      /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;     /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;      /* Current time in millisecs since router booted */
  uint32_t exaddr;         /* Exporter IP address */
  uint32_t srcaddr;        /* Source IP Address */
  uint32_t dstaddr;        /* Destination IP Address */
  uint32_t nexthop;        /* Next hop router's IP Address */
  uint16_t input;          /* Input interface index */
  uint16_t output;         /* Output interface index */
  uint32_t dPkts;          /* Packets sent in Duration */
  uint32_t dOctets;        /* Octets sent in Duration. */
  uint32_t First;          /* SysUptime at start of flow */
  uint32_t Last;           /* and of last packet of flow */
  uint16_t srcport;        /* TCP/UDP source port number or equivalent */
  uint16_t dstport;        /* TCP/UDP destination port number or equiv */
  uint8_t  prot;           /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
  uint8_t  tos;            /* IP Type-of-Service */
  uint8_t  tcp_flags;      /* OR of TCP header bits */
  uint8_t  pad;
  uint8_t  engine_type;    /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;      /* Slot number of the flow switching engine */
  uint8_t  src_mask;       /* mask length of source address */
  uint8_t  dst_mask;       /* mask length of destination address */
  uint16_t src_as;         /* AS of source address */
  uint16_t dst_as;         /* AS of destination address */
};

struct fts3rec_v1 {
  uint32_t unix_secs;      /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;     /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;      /* Current time in millisecs since router booted */
  uint32_t exaddr;         /* Exporter IP address */
  uint32_t srcaddr;        /* Source IP Address */
  uint32_t dstaddr;        /* Destination IP Address */
  uint32_t nexthop;        /* Next hop router's IP Address */
  uint16_t input;          /* Input interface index */
  uint16_t output;         /* Output interface index */
  uint32_t dPkts;          /* Packets sent in Duration */
  uint32_t dOctets;        /* Octets sent in Duration. */
  uint32_t First;          /* SysUptime at start of flow */
  uint32_t Last;           /* and of last packet of flow */
  uint16_t srcport;        /* TCP/UDP source port number or equivalent */
  uint16_t dstport;        /* TCP/UDP destination port number or equiv */
  uint8_t  prot;           /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
  uint8_t  tos;            /* IP Type-of-Service */
  uint8_t  tcp_flags;      /* OR of TCP header bits */
  uint8_t  pad;
  uint32_t reserved;
};

/* note the v5 struct is a subset of v6 and v7.  v6 and v7 are assumed
 *  to be in the same order so the engine_* src_mask, dst_mask, src_as and
 *  dst_as are in the same place.  v5 is like a generic v5, v6, v7
 */
struct fts3rec_v5 {
  uint32_t unix_secs;      /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;     /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;      /* Current time in millisecs since router booted */
  uint32_t exaddr;         /* Exporter IP address */
  uint32_t srcaddr;        /* Source IP Address */
  uint32_t dstaddr;        /* Destination IP Address */
  uint32_t nexthop;        /* Next hop router's IP Address */
  uint16_t input;          /* Input interface index */
  uint16_t output;         /* Output interface index */
  uint32_t dPkts;          /* Packets sent in Duration */
  uint32_t dOctets;        /* Octets sent in Duration. */
  uint32_t First;          /* SysUptime at start of flow */
  uint32_t Last;           /* and of last packet of flow */
  uint16_t srcport;        /* TCP/UDP source port number or equivalent */
  uint16_t dstport;        /* TCP/UDP destination port number or equiv */
  uint8_t  prot;           /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
  uint8_t  tos;            /* IP Type-of-Service */
  uint8_t  tcp_flags;      /* OR of TCP header bits */
  uint8_t  pad;
  uint8_t  engine_type;    /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;      /* Slot number of the flow switching engine */
  uint8_t  src_mask;       /* mask length of source address */
  uint8_t  dst_mask;       /* mask length of destination address */
  uint16_t src_as;         /* AS of source address */
  uint16_t dst_as;         /* AS of destination address */
};

struct fts3rec_v6 {
  uint32_t unix_secs;      /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;     /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;      /* Current time in millisecs since router booted */
  uint32_t exaddr;         /* Exporter IP address */
  uint32_t srcaddr;        /* Source IP Address */
  uint32_t dstaddr;        /* Destination IP Address */
  uint32_t nexthop;        /* Next hop router's IP Address */
  uint16_t input;          /* Input interface index */
  uint16_t output;         /* Output interface index */
  uint32_t dPkts;          /* Packets sent in Duration */
  uint32_t dOctets;        /* Octets sent in Duration. */
  uint32_t First;          /* SysUptime at start of flow */
  uint32_t Last;           /* and of last packet of flow */
  uint16_t srcport;        /* TCP/UDP source port number or equivalent */
  uint16_t dstport;        /* TCP/UDP destination port number or equiv */
  uint8_t  prot;           /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
  uint8_t  tos;            /* IP Type-of-Service */
  uint8_t  tcp_flags;      /* OR of TCP header bits */
  uint8_t  pad;
  uint8_t  engine_type;    /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;      /* Slot number of the flow switching engine */
  uint8_t  src_mask;       /* mask length of source address */
  uint8_t  dst_mask;       /* mask length of destination address */
  uint16_t src_as;         /* AS of source address */
  uint16_t dst_as;         /* AS of destination address */
  uint8_t  in_encaps;      /* size in bytes of the input encapsulation */
  uint8_t  out_encaps;     /* size in bytes of the output encapsulation */
  uint16_t pad2;
  uint32_t peer_nexthop;   /* IP address of the next hop within the peer */
};

struct fts3rec_v7 {
  uint32_t unix_secs;      /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;     /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;      /* Current time in millisecs since router booted */
  uint32_t exaddr;         /* Exporter IP address */
  uint32_t srcaddr;        /* Source IP Address */
  uint32_t dstaddr;        /* Destination IP Address */
  uint32_t nexthop;        /* Next hop router's IP Address */
  uint16_t input;          /* Input interface index */
  uint16_t output;         /* Output interface index */
  uint32_t dPkts;          /* Packets sent in Duration */
  uint32_t dOctets;        /* Octets sent in Duration. */
  uint32_t First;          /* SysUptime at start of flow */
  uint32_t Last;           /* and of last packet of flow */
  uint16_t srcport;        /* TCP/UDP source port number or equivalent */
  uint16_t dstport;        /* TCP/UDP destination port number or equiv */
  uint8_t  prot;           /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
  uint8_t  tos;            /* IP Type-of-Service */
  uint8_t  tcp_flags;      /* OR of TCP header bits */
  uint8_t  flags;          /* Reason flow discarded, etc */
  uint8_t  engine_type;    /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;      /* Slot number of the flow switching engine */
  uint8_t  src_mask;       /* mask length of source address */
  uint8_t  dst_mask;       /* mask length of destination address */
  uint16_t src_as;         /* AS of source address */
  uint16_t dst_as;         /* AS of destination address */
  uint32_t router_sc;      /* ID of router shortcut by switch */
};

struct fts3rec_v8_1 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint16_t src_as;     /* originating AS of source address */
  uint16_t dst_as;     /* originating AS of destination address */
  uint16_t input;      /* input interface index */
  uint16_t output;     /* output interface index */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
  uint16_t pad;
};


struct fts3rec_v8_2 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint8_t  prot;       /* IP protocol */
  uint8_t  pad;
  uint16_t reserved;
  uint16_t srcport;    /* TCP/UDP source port number of equivalent */
  uint16_t dstport;    /* TCP/UDP dst port number of equivalent */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
  uint16_t pad2;
};

struct fts3rec_v8_3 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t srcaddr;
  uint8_t  src_mask;
  uint8_t  pad;
  uint16_t src_as;
  uint16_t input;
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
};

struct fts3rec_v8_4 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t dstaddr;
  uint8_t  dst_mask;
  uint8_t  pad;
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
  uint16_t dst_as;
  uint16_t output;
};

struct fts3rec_v8_5 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t srcaddr;
  uint32_t dstaddr;
  uint8_t  dst_mask;
  uint8_t  src_mask;
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
  uint16_t src_as;
  uint16_t dst_as;
  uint16_t input;
  uint16_t output;
};

struct fts3rec_v8_6 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t dstaddr;    /* destination IP address */
  uint32_t extra_pkts; /* packets that exceed the contract */
  uint32_t router_sc;  /* IP address of the router being shortcut */
  uint16_t output;     /* output interface index */
  uint16_t pad;
  uint8_t  tos;        /* tos */
  uint8_t  marked_tos; /* tos of pkts that exceeded the contract */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
};

struct fts3rec_v8_7 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t dstaddr;    /* destination IP address */
  uint32_t srcaddr;    /* source IP address */
  uint32_t extra_pkts; /* packets that exceed the contract */
  uint32_t router_sc;  /* IP address of the router being shortcut */
  uint16_t output;     /* output interface index */
  uint16_t input;      /* input interface index */
  uint8_t  tos;        /* tos */
  uint8_t  marked_tos; /* tos of pkts that exceeded the contract */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
};

struct fts3rec_v8_8 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t dstaddr;    /* destination IP address */
  uint32_t srcaddr;    /* source IP address */
  uint32_t extra_pkts; /* packets that exceed the contract */
  uint32_t router_sc;  /* IP address of the router being shortcut */
  uint16_t dstport;    /* TCP/UDP destination port */
  uint16_t srcport;    /* TCP/UDP source port */
  uint16_t output;     /* output interface index */
  uint16_t input;      /* input interface index */
  uint8_t  tos;        /* tos */
  uint8_t  marked_tos; /* tos of pkts that exceeded the contract */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
  uint8_t  prot;       /* IP protocol */
  uint8_t  pad1;
  uint16_t pad2;
};

struct fts3rec_v8_9 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint16_t src_as;     /* originating AS of source address */
  uint16_t dst_as;     /* originating AS of destination address */
  uint16_t input;      /* input interface index */
  uint16_t output;     /* output interface index */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
  uint8_t  tos;        /* ToS */
  uint8_t  pad;
};

struct fts3rec_v8_10 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint16_t srcport;    /* TCP/UDP source port number of equivalent */
  uint16_t dstport;    /* TCP/UDP dst port number of equivalent */
  uint16_t input;      /* input interface index */
  uint16_t output;     /* output interface index */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
  uint8_t  prot;       /* IP protocol */
  uint8_t  tos;        /* ToS */
};

struct fts3rec_v8_11 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t srcaddr;    /* Source Prefix */
  uint8_t  src_mask;   /* Source Prefix mask length */
  uint8_t  tos;        /* ToS */
  uint16_t src_as;     /* Source AS */
  uint16_t input;      /* input interface index */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
};

struct fts3rec_v8_12 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t dstaddr;    /* Destination Prefix */
  uint16_t output;     /* output interface index */
  uint16_t dst_as;     /* Destination AS */
  uint8_t  dst_mask;   /* Destination Prefix mask length */
  uint8_t  tos;        /* ToS */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
};

struct fts3rec_v8_13 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t srcaddr;    /* Source Prefix */
  uint32_t dstaddr;    /* Destination Prefix */
  uint16_t src_as;     /* Source AS */
  uint16_t dst_as;     /* Destination AS */
  uint16_t input;      /* input interface */
  uint16_t output;     /* output interface */
  uint8_t  dst_mask;   /* Destination Prefix mask length */
  uint8_t  src_mask;   /* Source Prefix mask length */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
  uint8_t  tos;        /* ToS */
  uint8_t  pad1;
  uint16_t pad2;
};

struct fts3rec_v8_14 {
  uint32_t unix_secs;  /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs; /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;  /* Current time in millisecs since router booted */
  uint32_t exaddr;     /* Exporter IP address */
  uint32_t dFlows;     /* Number of flows */
  uint32_t dPkts;      /* Packets sent in duration */
  uint32_t dOctets;    /* Octets sent in duration */
  uint32_t First;      /* SysUpTime at start of flow */
  uint32_t Last;       /* and of last packet of flow */
  uint32_t srcaddr;    /* Source Prefix */
  uint32_t dstaddr;    /* Destination Prefix */
  uint16_t srcport;    /* Source Port */
  uint16_t dstport;    /* Destination Port */
  uint16_t input;      /* input interface */
  uint16_t output;     /* output interface */
  uint8_t  dst_mask;   /* Destination Prefix mask length */
  uint8_t  src_mask;   /* Source Prefix mask length */
  uint8_t  engine_type;/* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;  /* Slot number of the flow switching engine */
  uint8_t  tos;        /* ToS */
  uint8_t  prot;       /* IP protocol */
  uint16_t pad2;
};


/*
 * internal formats
*/

/* tagged v5 */
struct fts3rec_v1005 {
  uint32_t unix_secs;      /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;     /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t sysUpTime;      /* Current time in millisecs since router booted */
  uint32_t exaddr;         /* Exporter IP address */
  uint32_t srcaddr;        /* Source IP Address */
  uint32_t dstaddr;        /* Destination IP Address */
  uint32_t nexthop;        /* Next hop router's IP Address */
  uint16_t input;          /* Input interface index */
  uint16_t output;         /* Output interface index */
  uint32_t dPkts;          /* Packets sent in Duration */
  uint32_t dOctets;        /* Octets sent in Duration. */
  uint32_t First;          /* SysUptime at start of flow */
  uint32_t Last;           /* and of last packet of flow */
  uint16_t srcport;        /* TCP/UDP source port number or equivalent */
  uint16_t dstport;        /* TCP/UDP destination port number or equiv */
  uint8_t  prot;           /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
  uint8_t  tos;            /* IP Type-of-Service */
  uint8_t  tcp_flags;      /* OR of TCP header bits */
  uint8_t  pad;
  uint8_t  engine_type;    /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;      /* Slot number of the flow switching engine */
  uint8_t  src_mask;       /* mask length of source address */
  uint8_t  dst_mask;       /* mask length of destination address */
  uint16_t src_as;         /* AS of source address */
  uint16_t dst_as;         /* AS of destination address */
  uint32_t src_tag;        /* Local TAG for source address */
  uint32_t dst_tag;        /* Local TAG for destination address */
};

struct fts1rec_compat {
  uint32_t unix_secs;      /* offset from real time the flow started */
  uint32_t unix_msecs;     /* "" */
  uint32_t srcaddr;        /* Source IP Address */
  uint32_t dstaddr;        /* Destination IP Address */
  uint32_t nexthop;        /* Next hop router's IP Address */
  uint16_t input;          /* Input interface index */
  uint16_t output;         /* Output interface index */
  uint32_t dPkts;          /* Packets sent in Duration */
  uint32_t dOctets;        /* Octets sent in Duration. */
  uint32_t First;          /* SysUptime at start of flow */
  uint32_t Last;           /* and of last packet of flow */
  uint16_t srcport;        /* TCP/UDP source port number or equivalent */
  uint16_t dstport;        /* TCP/UDP destination port number or equiv */
  uint16_t pad;
  uint8_t  prot;           /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
  uint8_t  tos;            /* IP Type-of-Service */
  uint8_t  flags;          /* Reason flow was discarded, etc...  */
  uint8_t  tcp_retx_cnt;   /* Number of mis-seq with delay > 1sec */
  uint8_t  tcp_retx_secs;  /* Cumulative secs between mis-sequenced pkts */
  uint8_t  tcp_misseq_cnt; /* Number of mis-sequenced tcp pkts seen */
  uint16_t src_as;         /* originating AS of source address */
  uint16_t dst_as;         /* originating AS of destination address */
  uint8_t  src_mask;       /* source address prefix mask bits */
  uint8_t  dst_mask;       /* destination address prefix mask bits */
  uint16_t drops;          /* ?? */
};

struct ftio {
  caddr_t mr;                        /* mmap region */
  size_t mr_size;                    /* size of mmap'd region */
  int rec_size;                      /* size of stream record */
  struct ftiheader fth;              /* header */
  char *d_buf;                       /* records */
  uint32_t d_start;                    /* offset to next byte in d_buf */
  uint32_t d_end;                      /* available bytes in d_buf */
  char *z_buf;                       /* zlib inflate/deflate buffer */
  int z_level;                       /* compression level */
  z_stream zs;                       /* zlib io */
  int flags;                         /* FT_IO_FLAG_* */
  int fd;                            /* file description of stream */
  uint64_t xfield;                    /* FT_XFIELD* available when reading */
  void (*swapf)(struct ftio *ftio);  /* swap function */
  uint64_t rec_total;                 /* records read/written */
  struct fts3rec_v1 compat_v1;       /* backwards compatability */
  struct fts3rec_v5 compat_v5;       /* backwards compatability */
  struct fts3rec_offsets fo;         /* offsets to fields */
  int debug;
};

struct ftpdu_header_small {
  uint16_t version;      /* 1 for now. */
  uint16_t count;        /* The number of records in the PDU */
};

struct ftpdu_header {
  /* common header for everything except v1 */
  uint16_t version;      /* 1 for now. */
  uint16_t count;        /* The number of records in the PDU */
  uint32_t sysUpTime;    /* Current time in millisecs since router booted */
  uint32_t unix_secs;    /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;   /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
};

struct ftpdu_v1 {
  /* 16 byte header */
  uint16_t version;      /* 1 for now. */
  uint16_t count;        /* The number of records in the PDU */
  uint32_t sysUpTime;    /* Current time in millisecs since router booted */
  uint32_t unix_secs;    /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;   /* Residual nanoseconds since 0000 UTC 1970 */
  /* 48 byte payload */
  struct ftrec_v1 {
    uint32_t srcaddr;        /* Source IP Address */
    uint32_t dstaddr;        /* Destination IP Address */
    uint32_t nexthop;        /* Next hop router's IP Address */
    uint16_t input;          /* Input interface index */
    uint16_t output;         /* Output interface index */
    uint32_t dPkts;          /* Packets sent in Duration */
    uint32_t dOctets;        /* Octets sent in Duration. */
    uint32_t First;          /* SysUptime at start of flow */
    uint32_t Last;           /* and of last packet of flow */
    uint16_t srcport;        /* TCP/UDP source port number or equivalent */
    uint16_t dstport;        /* TCP/UDP destination port number or equiv */
    uint16_t pad;
    uint8_t  prot;           /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
    uint8_t  tos;            /* IP Type-of-Service */
    uint8_t  flags;          /* Reason flow was discarded, etc...  */
    uint8_t  tcp_retx_cnt;   /* Number of mis-seq with delay > 1sec */
    uint8_t  tcp_retx_secs;  /* Cumulative secs between mis-sequenced pkts */
    uint8_t  tcp_misseq_cnt; /* Number of mis-sequenced tcp pkts seen */
    uint32_t  reserved;
  } records[FT_PDU_V1_MAXFLOWS];
};

struct ftpdu_v5 {
  /* 24 byte header */
  uint16_t version;       /* 5 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint16_t reserved;
  /* 48 byte payload */
  struct ftrec_v5 {
    uint32_t srcaddr;    /* Source IP Address */
    uint32_t dstaddr;    /* Destination IP Address */
    uint32_t nexthop;    /* Next hop router's IP Address */
    uint16_t input;      /* Input interface index */
    uint16_t output;     /* Output interface index */
    uint32_t dPkts;      /* Packets sent in Duration */
    uint32_t dOctets;    /* Octets sent in Duration. */
    uint32_t First;      /* SysUptime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint16_t srcport;    /* TCP/UDP source port number or equivalent */
    uint16_t dstport;    /* TCP/UDP destination port number or equiv */
    uint8_t  pad;
    uint8_t  tcp_flags;  /* Cumulative OR of tcp flags */
    uint8_t  prot;       /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
    uint8_t  tos;        /* IP Type-of-Service */
    uint16_t src_as;     /* originating AS of source address */
    uint16_t dst_as;     /* originating AS of destination address */
    uint8_t  src_mask;   /* source address prefix mask bits */
    uint8_t  dst_mask;   /* destination address prefix mask bits */
    uint16_t drops;
  } records[FT_PDU_V5_MAXFLOWS];
};

struct ftpdu_v6 {
  /* 24 byte header */
  uint16_t version;       /* 6 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint16_t reserved;
  /* 48 byte payload */
  struct ftrec_v6 {
    uint32_t srcaddr;      /* Source IP Address */
    uint32_t dstaddr;      /* Destination IP Address */
    uint32_t nexthop;      /* Next hop router's IP Address */
    uint16_t input;        /* Input interface index */
    uint16_t output;       /* Output interface index */
    uint32_t dPkts;        /* Packets sent in Duration */
    uint32_t dOctets;      /* Octets sent in Duration. */
    uint32_t First;        /* SysUptime at start of flow */
    uint32_t Last;         /* and of last packet of flow */
    uint16_t srcport;      /* TCP/UDP source port number or equivalent */
    uint16_t dstport;      /* TCP/UDP destination port number or equiv */
    uint8_t  pad;
    uint8_t  tcp_flags;    /* Cumulative OR of tcp flags */
    uint8_t  prot;         /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
    uint8_t  tos;          /* IP Type-of-Service */
    uint16_t src_as;       /* originating AS of source address */
    uint16_t dst_as;       /* originating AS of destination address */
    uint8_t  src_mask;     /* source address prefix mask bits */
    uint8_t  dst_mask;     /* destination address prefix mask bits */
    uint8_t  in_encaps;    /* size in bytes of the input encapsulation */
    uint8_t  out_encaps;   /* size in bytes of the output encapsulation */
    uint32_t peer_nexthop; /* IP address of the next hop within the peer */
  } records[FT_PDU_V6_MAXFLOWS];
};

struct ftpdu_v7 {
  /* 24 byte header */
  uint16_t version;       /* 7 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint16_t reserved;
  /* 48 byte payload */
  struct ftrec_v7 {
    uint32_t srcaddr;    /* Source IP Address */
    uint32_t dstaddr;    /* Destination IP Address */
    uint32_t nexthop;    /* Next hop router's IP Address */
    uint16_t input;      /* Input interface index */
    uint16_t output;     /* Output interface index */
    uint32_t dPkts;      /* Packets sent in Duration */
    uint32_t dOctets;    /* Octets sent in Duration. */
    uint32_t First;      /* SysUptime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint16_t srcport;    /* TCP/UDP source port number or equivalent */
    uint16_t dstport;    /* TCP/UDP destination port number or equiv */
    uint8_t  pad;
    uint8_t  tcp_flags;  /* Cumulative OR of tcp flags */
    uint8_t  prot;       /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
    uint8_t  tos;        /* IP Type-of-Service */
    uint16_t src_as;     /* originating AS of source address */
    uint16_t dst_as;     /* originating AS of destination address */
    uint8_t  src_mask;   /* source address prefix mask bits */
    uint8_t  dst_mask;   /* destination address prefix mask bits */
    uint16_t drops;
    uint32_t router_sc;  /* Router which is shortcut by switch */
  } records[FT_PDU_V7_MAXFLOWS];
};

/* Generic v8 pdu */
struct ftpdu_v8_gen {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
};

struct ftpdu_v8_1 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 28 byte payload */
  struct ftrec_v8_1 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint16_t src_as;     /* originating AS of source address */
    uint16_t dst_as;     /* originating AS of destination address */
    uint16_t input;      /* input interface index */
    uint16_t output;     /* output interface index */
  } records[FT_PDU_V8_1_MAXFLOWS];
};

struct ftpdu_v8_2 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 28 byte payload */
  struct ftrec_v8_2 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint8_t  prot;       /* IP protocol */
    uint8_t  pad;
    uint16_t reserved;
    uint16_t srcport;    /* TCP/UDP source port number of equivalent */
    uint16_t dstport;    /* TCP/UDP dst port number of equivalent */
  } records[FT_PDU_V8_2_MAXFLOWS];
};


struct ftpdu_v8_3 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 32 byte payload */
  struct ftrec_v8_3 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint32_t src_prefix;
    uint8_t  src_mask;
    uint8_t  pad;
    uint16_t src_as;
    uint16_t input;
    uint16_t reserved;
  } records[FT_PDU_V8_3_MAXFLOWS];
};


struct ftpdu_v8_4 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 32 byte payload */
  struct ftrec_v8_4 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint32_t dst_prefix;
    uint8_t  dst_mask;
    uint8_t  pad;
    uint16_t dst_as;
    uint16_t output;
    uint16_t reserved;
  } records[FT_PDU_V8_4_MAXFLOWS];
};


struct ftpdu_v8_5 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 40 byte payload */
  struct ftrec_v8_5 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint32_t src_prefix;
    uint32_t dst_prefix;
    uint8_t  dst_mask;
    uint8_t  src_mask;
    uint16_t reserved;
    uint16_t src_as;
    uint16_t dst_as;
    uint16_t input;
    uint16_t output;
  } records[FT_PDU_V8_5_MAXFLOWS];
};

struct ftpdu_v8_6 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 32 byte payload */
  struct ftrec_v8_6 {
    uint32_t dstaddr;   /* destination IP address */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint16_t output;     /* output interface index */
    uint8_t  tos;        /* tos */
    uint8_t  marked_tos; /* tos of pkts that exceeded the contract */
    uint32_t extra_pkts; /* packets that exceed the contract */
    uint32_t router_sc;  /* IP address of the router being shortcut */
  } records[FT_PDU_V8_6_MAXFLOWS];
};

struct ftpdu_v8_7 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 40 byte payload */
  struct ftrec_v8_7 {
    uint32_t dstaddr;    /* destination IP address */
    uint32_t srcaddr;    /* source address */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint16_t output;     /* output interface index */
    uint16_t input;      /* input interface index */
    uint8_t  tos;        /* tos */
    uint8_t  marked_tos; /* tos of pkts that exceeded the contract */
    uint16_t reserved;
    uint32_t extra_pkts; /* packets that exceed the contract */
    uint32_t router_sc;  /* IP address of the router being shortcut */
  } records[FT_PDU_V8_7_MAXFLOWS];
};

struct ftpdu_v8_8 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 44 byte payload */
  struct ftrec_v8_8 {
    uint32_t dstaddr;    /* destination IP address */
    uint32_t srcaddr;    /* source IP address */
    uint16_t dstport;    /* TCP/UDP destination port */
    uint16_t srcport;    /* TCP/UDP source port */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint16_t output;     /* output interface index */
    uint16_t input;      /* input interface index */
    uint8_t  tos;        /* tos */
    uint8_t  prot;       /* protocol */
    uint8_t  marked_tos; /* tos of pkts that exceeded the contract */
    uint8_t  reserved;
    uint32_t extra_pkts; /* packets that exceed the contract */
    uint32_t router_sc;  /* IP address of the router being shortcut */
  } records[FT_PDU_V8_8_MAXFLOWS];
};


struct ftpdu_v8_9 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 32 byte payload */
  struct ftrec_v8_9 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint16_t src_as;     /* originating AS of source address */
    uint16_t dst_as;     /* originating AS of destination address */
    uint16_t input;      /* input interface index */
    uint16_t output;     /* output interface index */
    uint8_t  tos;        /* tos */
    uint8_t  pad;
    uint16_t reserved;
  } records[FT_PDU_V8_9_MAXFLOWS];
};

struct ftpdu_v8_10 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 32 byte payload */
  struct ftrec_v8_10 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint8_t  prot;       /* IP protocol */
    uint8_t  tos;        /* tos */
    uint16_t reserved;
    uint16_t srcport;    /* TCP/UDP source port number of equivalent */
    uint16_t dstport;    /* TCP/UDP dst port number of equivalent */
    uint16_t input;      /* input interface */
    uint16_t output;     /* output interface index */
  } records[FT_PDU_V8_10_MAXFLOWS];
};

struct ftpdu_v8_11 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 32 byte payload */
  struct ftrec_v8_11 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint32_t src_prefix; /* Source Prefix */
    uint8_t  src_mask;   /* Source Prefix mask length */
    uint8_t  tos;        /* tos */
    uint16_t src_as;     /* Source AS */
    uint16_t input;      /* input interface */
    uint16_t reserved;
  } records[FT_PDU_V8_11_MAXFLOWS];
};

struct ftpdu_v8_12 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 32 byte payload */
  struct ftrec_v8_12 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint32_t dst_prefix; /* Destination Prefix */
    uint8_t  dst_mask;   /* Destination Prefix mask length */
    uint8_t  tos;        /* tos */
    uint16_t dst_as;     /* Destination AS */
    uint16_t output;     /* output interface */
    uint16_t reserved;
  } records[FT_PDU_V8_12_MAXFLOWS];
};

struct ftpdu_v8_13 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 40 byte payload */
  struct ftrec_v8_13 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint32_t src_prefix; /* Source Prefix */
    uint32_t dst_prefix; /* Destination Prefix */
    uint8_t  dst_mask;   /* Destination Prefix mask length */
    uint8_t  src_mask;   /* Source Prefix mask length */
    uint8_t  tos;        /* tos */
    uint8_t  pad;
    uint16_t src_as;     /* Source AS */
    uint16_t dst_as;     /* Destination AS */
    uint16_t input;      /* input interface */
    uint16_t output;     /* output interface */
  } records[FT_PDU_V8_13_MAXFLOWS];
};

struct ftpdu_v8_14 {
  /* 28 byte header */
  uint16_t version;       /* 8 */
  uint16_t count;         /* The number of records in the PDU */
  uint32_t sysUpTime;     /* Current time in millisecs since router booted */
  uint32_t unix_secs;     /* Current seconds since 0000 UTC 1970 */
  uint32_t unix_nsecs;    /* Residual nanoseconds since 0000 UTC 1970 */
  uint32_t flow_sequence; /* Seq counter of total flows seen */
  uint8_t  engine_type;   /* Type of flow switching engine (RP,VIP,etc.) */
  uint8_t  engine_id;     /* Slot number of the flow switching engine */
  uint8_t  aggregation;   /* Aggregation method being used */
  uint8_t  agg_version;   /* Version of the aggregation export */
  uint32_t reserved;
  /* 40 byte payload */
  struct ftrec_v8_14 {
    uint32_t dFlows;     /* Number of flows */
    uint32_t dPkts;      /* Packets sent in duration */
    uint32_t dOctets;    /* Octets sent in duration */
    uint32_t First;      /* SysUpTime at start of flow */
    uint32_t Last;       /* and of last packet of flow */
    uint32_t src_prefix; /* Source Prefix */
    uint32_t dst_prefix; /* Destination Prefix */
    uint8_t  dst_mask;   /* Destination Prefix mask length */
    uint8_t  src_mask;   /* Source Prefix mask length */
    uint8_t  tos;        /* tos */
    uint8_t  prot;       /* protocol */
    uint16_t srcport;    /* Source port */
    uint16_t dstport;    /* Destination port */
    uint16_t input;      /* input interface */
    uint16_t output;     /* output interface */
  } records[FT_PDU_V8_14_MAXFLOWS];
};


enum ftfil_mode { FT_FIL_MODE_UNSET, FT_FIL_MODE_PERMIT, FT_FIL_MODE_DENY };

enum ftfil_primitive_type { FT_FIL_PRIMITIVE_TYPE_UNSET,
                             FT_FIL_PRIMITIVE_TYPE_AS,
                             FT_FIL_PRIMITIVE_TYPE_IP_PREFIX,
                             FT_FIL_PRIMITIVE_TYPE_IP_ADDRESS,
                             FT_FIL_PRIMITIVE_TYPE_IP_MASK,
                             FT_FIL_PRIMITIVE_TYPE_IP_PROTOCOL,
                             FT_FIL_PRIMITIVE_TYPE_IP_PORT,
                             FT_FIL_PRIMITIVE_TYPE_IP_PREFIX_LEN,
                             FT_FIL_PRIMITIVE_TYPE_IP_TOS,
                             FT_FIL_PRIMITIVE_TYPE_IP_TCP_FLAGS,
                             FT_FIL_PRIMITIVE_TYPE_IF_INDEX,   
                             FT_FIL_PRIMITIVE_TYPE_COUNTER,
                             FT_FIL_PRIMITIVE_TYPE_TIME_DATE,
                             FT_FIL_PRIMITIVE_TYPE_ENGINE,
                             FT_FIL_PRIMITIVE_TYPE_TAG,
                             FT_FIL_PRIMITIVE_TYPE_TAG_MASK,
                             FT_FIL_PRIMITIVE_TYPE_TIME,
                             FT_FIL_PRIMITIVE_TYPE_DOUBLE,
                             FT_FIL_PRIMITIVE_TYPE_RATE, };

struct ftfil_match_item {
  FT_STAILQ_ENTRY(ftfil_match_item) chain; /* list (AND) */
  void *lookup; /* data for evaluator */
  int (*eval)(void *lookup, void *rec, struct fts3rec_offsets *fo);
  char *tmp_type; /* temporary pointer to FT_FIL_DEFINITION_MATCH_* word */
  char *tmp_primitive; /* temporary pointer to primitive name */
};

struct ftfil_match {
  FT_STAILQ_ENTRY(ftfil_match) chain; /* list (OR) */
  FT_STAILQ_HEAD(filmatihead, ftfil_match_item) items; /* list (AND) */
};
   
struct ftfil_def {
  FT_SLIST_ENTRY(ftfil_def) chain; /* list */
  FT_STAILQ_HEAD(filmathead, ftfil_match) matches;
  char *name; /* name of the def */
  uint64_t xfields; /* required flow fields - FT_XFIELD_* */
  int invert; /* invert the PERMIT/DENY? */
};

struct ftfil_primitive {
  FT_SLIST_ENTRY(ftfil_primitive) chain; /* list */
  enum ftfil_primitive_type type; /* FT_FIL_PRIMITIVE_TYPE_* */
  char *name; /* name of the filter */
  void *lookup; /* data associated with filter */
};

struct ftfil {
  FT_SLIST_HEAD(fildefshead, ftfil_def) defs; /* definitions */
  FT_SLIST_HEAD(filprimhead, ftfil_primitive) primitives; /* primitives */
  struct ftfil_def *active_def;
};

struct ftmask_def {
  FT_SLIST_ENTRY(ftmask_def) chain; /* list */
  struct radix_node_head *rhead;
  char *name; /* name of the def */
};

struct ftmask {
  FT_SLIST_HEAD(maskdefshead, ftmask_def) defs; /* definitions */
  struct ftmask_def *active_def;
};

int ftmask_load(struct ftmask *ftmask, const char *fname);
void ftmask_free(struct ftmask *ftmask);
struct ftmask_def *ftmask_def_find(struct ftmask *ftmask, const char *name);
int ftmask_def_eval(struct ftmask_def *active_def,
  char *rec, struct fts3rec_offsets *fo);

/* ftxlate */

struct ftxlate {
  FT_SLIST_HEAD(xdefshead, ftxlate_def) defs; /* list of definitions */
  FT_SLIST_HEAD(xacthead, ftxlate_action) actions; /* list of actions */
  struct ftfil ftfil;
  int ftfil_init; /* ftfil initialized? */
  char *filter_fname;
  int flags;
  struct ftvar *ftvar;
};

/* area of struct generic to v1, v5, v6, v7 formats */

struct ftfile_entry {
  char     *name;
  off_t    size;
  uint32_t  start;
  int      skip; /* skip this during processing */
  FT_TAILQ_ENTRY(ftfile_entry) chain;
};

struct ftfile_entries {
  FT_TAILQ_HEAD(talkqhead, ftfile_entry) head;
  uint64_t num_bytes;   /* space used by all flow files except current */
  uint64_t max_bytes;   /* min space before removing files 0=disable */
  uint32_t max_files;   /* max num of files to keep before removing 0=disable */
  uint32_t num_files;   /* number of files in the queue */
  int expiring;        /* expiring in use? */
};


struct ftchash_chunk {
  void *base;                                /* base pointer */
  uint32_t next;                              /* offset to next free record */
  FT_SLIST_ENTRY(ftchash_chunk) chain;       /* next */
};

struct ftchash {
  unsigned int h_size;                              /* hash_table_size */
  unsigned int d_size;                              /* data entry size */
  unsigned int key_size;                            /* size of key */
  int   chunk_size;                          /* chunk size (entries * d_size */
  uint64_t entries;                           /* entries in hash table */
  void *traverse_rec;                        /* record when traversing */
  struct ftchash_chunk *traverse_chunk;      /* chunk when traversing */
  uint64_t traverse_srec;                     /* sorted rec when traversing */
  struct ftchash_chunk *active_chunk;        /* currently active chunk */
                                             /* memory chunk list */
  FT_SLIST_HEAD(ftchash_chunkh, ftchash_chunk) chunk_list;
                                             /* hash table buckets */
  FT_SLIST_HEAD(ftchash_bhead, ftchash_rec_gen) *buckets;
  struct ftchash_rec_gen **sorted_recs;      /* array of pointers to sorted 
                                                records */
  int sort_flags;                            /* sorted version available? */
};

struct ftchash_rec_gen {
  FT_SLIST_ENTRY(ftchash_rec_gen) chain;
  uint32_t data;
};

struct ftchash_rec_prefixs {
  FT_SLIST_ENTRY(ftchash_rec_ips) chain;
  uint32_t prefix;
  uint8_t  mask;         /* mask */
  uint8_t  fil1;
  uint16_t fil2;
};

struct ftchash_rec_tags {
  FT_SLIST_ENTRY(ftchash_rec_ips) chain;
  uint32_t tag;
};

struct ftps {
  double avg_pps, avg_bps;   /* packets/bytes per second */
  double max_pps, max_bps;   /* packets/bytes per second */
  double min_pps, min_bps;   /* packets/bytes per second */
};

struct ftchash_rec_prefixh {
  FT_SLIST_ENTRY(ftchash_rec_prefix) chain;
  uint32_t prefix;       /* prefix */
  uint8_t  mask;         /* mask */
  uint8_t  fil1;
  uint16_t fil2;
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  uint64_t nprefixes;    /* number of prefixes seen */
  struct ftchash *ftch; /* second list */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_c32 {
  FT_SLIST_ENTRY(ftchash_rec_c32) chain;
  uint32_t c32;          /* 32 bit quantity */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_c64 {
  FT_SLIST_ENTRY(ftchash_rec_c64) chain;
  uint64_t c64;          /* 64 bit quantity */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_c322 {
  FT_SLIST_ENTRY(ftchash_rec_c322) chain;
  uint32_t c32a;         /* 32 bit quantity */
  uint32_t c32b;         /* 32 bit quantity */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_prefix16 {
  FT_SLIST_ENTRY(ftchash_rec_prefix16) chain;
  uint32_t prefix;       /* 32 bit quantity */
  uint8_t  mask;         /* 8 bit quantity */
  uint8_t  fill;         /* 8 bit quantity */
  uint16_t c16;          /* 16 bit quantity */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_prefix162 {
  FT_SLIST_ENTRY(ftchash_rec_prefix16) chain;
  uint32_t prefix;       /* 32 bit quantity */
  uint8_t  mask;         /* 8 bit quantity */
  uint8_t  fill;         /* 8 bit quantity */
  uint16_t c16a;         /* 16 bit quantity */
  uint16_t c16b;         /* 16 bit quantity */
  uint16_t fill2;        /* 16 bit quanity */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_prefix216 {
  FT_SLIST_ENTRY(ftchash_rec_prefix216) chain;
  uint32_t src_prefix;   /* 32 bit quantity */
  uint8_t  src_mask;     /* 8 bit quantity */
  uint8_t  fill;         /* 8 bit quantity */
  uint16_t c16;          /* 16 bit quantity */
  uint32_t dst_prefix;   /* 32 bit quantity */
  uint8_t  dst_mask;     /* 8 bit quantity */
  uint8_t  fill2;        /* 8 bit quantity */
  uint16_t fill3;        /* 16 bit quantity */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_prefix2162 {
  FT_SLIST_ENTRY(ftchash_rec_prefix2162) chain;
  uint32_t src_prefix;   /* 32 bit quantity */
  uint8_t  src_mask;     /* 8 bit quantity */
  uint8_t  fill;         /* 8 bit quantity */
  uint16_t c16a;         /* 16 bit quantity */
  uint32_t dst_prefix;   /* 32 bit quantity */
  uint8_t  dst_mask;     /* 8 bit quantity */
  uint8_t  fill2;        /* 8 bit quantity */
  uint16_t c16b;         /* 16 bit quantity */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_ip {
  FT_SLIST_ENTRY(ftchash_rec_ip) chain;
  uint32_t addr;         /* ip address */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_ip2 {
  FT_SLIST_ENTRY(ftchash_rec_ip2) chain;
  uint32_t src_addr;     /* ip address */
  uint32_t dst_addr;     /* ip address */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_c162 {
  FT_SLIST_ENTRY(ftchash_rec_162) chain;
  uint16_t c16a;         /* 16 bit quantity # */
  uint16_t c16b;         /* 16 bit quantity # */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_c163 {
  FT_SLIST_ENTRY(ftchash_rec_163) chain;
  uint16_t c16a;         /* 16 bit quantity # */
  uint16_t c16b;         /* 16 bit quantity # */
  uint16_t c16c;         /* 16 bit quantity # */
  uint16_t fill;         /* 16 bit quantity # */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_c164 {
  FT_SLIST_ENTRY(ftchash_rec_164) chain;
  uint16_t c16a;         /* 16 bit quantity # */
  uint16_t c16b;         /* 16 bit quantity # */
  uint16_t c16c;         /* 16 bit quantity # */
  uint16_t c16d;         /* 16 bit quantity # */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_as2 {
  FT_SLIST_ENTRY(ftchash_rec_as2) chain;
  uint16_t src_as;       /* AS # */
  uint16_t dst_as;       /* AS # */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_if2 {
  FT_SLIST_ENTRY(ftchash_rec_if2) chain;
  uint16_t input;        /* interface ifIndex */
  uint16_t output;       /* interface ifIndex */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_prefix {
  FT_SLIST_ENTRY(ftchash_rec_ip) chain;
  uint32_t prefix;       /* prefix */
  uint8_t  mask;         /* mask */
  uint8_t  fil1;
  uint16_t fil2;
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_prefix_tag {
  FT_SLIST_ENTRY(ftchash_rec_ip) chain;
  uint32_t prefix;       /* prefix */
  uint8_t  mask;         /* mask */
  uint8_t  fil1;
  uint16_t fil2;
  uint32_t tag;          /* tag */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_prefix2 {
  FT_SLIST_ENTRY(ftchash_rec_ip) chain;
  uint32_t src_prefix;   /* prefix */
  uint8_t  src_mask;     /* mask */
  uint8_t  fil1;
  uint16_t fil2;
  uint32_t dst_prefix;   /* prefix */
  uint8_t  dst_mask;     /* mask */
  uint8_t  fil3;
  uint16_t fil4;
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_prefix2tag2 {
  FT_SLIST_ENTRY(ftchash_rec_ip) chain;
  uint32_t src_prefix;   /* prefix */
  uint8_t  src_mask;     /* mask */
  uint8_t  fil1;
  uint16_t fil2;
  uint32_t dst_prefix;   /* prefix */
  uint8_t  dst_mask;     /* mask */
  uint8_t  fil3;
  uint16_t fil4;
  uint32_t src_tag;      /* source tag */
  uint32_t dst_tag;      /* destination tag */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_flow1 {
  FT_SLIST_ENTRY(ftchash_rec_ip) chain;
  uint32_t src_prefix;   /* prefix */
  uint8_t  src_mask;     /* mask */
  uint8_t  prot;         /* protocol */
  uint16_t src_port;     /* source port */
  uint32_t dst_prefix;   /* prefix */
  uint8_t  dst_mask;     /* mask */
  uint8_t  tos;          /* type of service */
  uint16_t dst_port;     /* destination port */
  uint64_t nrecs;        /* flow records */
  uint64_t nflows;       /* flows */
  uint64_t noctets;      /* octets */
  uint64_t npackets;     /* packets */
  uint64_t etime;        /* duration */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_int {
  FT_SLIST_ENTRY(ftchash_rec_int) chain;
  uint32_t time;         /* unix seconds */
  uint64_t nrecs;        /* flow records */
  double nflows;        /* flows */
  double noctets;       /* octets */
  double npackets;      /* packets */
  struct ftps ps;       /* per second counters */
};

struct ftchash_rec_exp {
  FT_SLIST_ENTRY(ftchash_rec_ip) chain;
  uint32_t src_ip;       /* exporter src IP */
  uint32_t dst_ip;       /* exporter dst IP (us) */
  uint16_t d_version;    /* data version */
  uint16_t dst_port;     /* destination port (FUTURE) */
  uint32_t packets;      /* packets received from this exporter */
  uint32_t flows;        /* flows received from this exporter */
  uint32_t filtered_flows; /* flows filtered and dropped from this exporter */
  uint32_t lost;         /* flows lost */
  uint32_t reset;        /* sequence number resets */
  struct ftseq ftseq;   /* sequence numbers for this exporter */
  void (*xlate)(void *in_rec, void *out_rec); /* translation function */
};

struct ftchash_rec_sym {
  FT_SLIST_ENTRY(ftchash_rec_ip) chain;
  uint32_t val;          /* string value */
  char *str;            /* string */
};

struct ftchash_rec_fil_c32 {
  FT_SLIST_ENTRY(ftchash_rec_fil_ip) chain;
  uint32_t c32;  /* 32 bit quantity */
  int mode;     /* mode */
};

struct ftchash_rec_split {
  FT_SLIST_ENTRY(ftchash_rec_fil_ip) chain;
  uint32_t tag; /* key */
  struct ftio ftio;
  int fd, id;
  uint32_t total_flows;
  uint32_t cap_start, cap_end;
  int newfile;
};

struct ftsym {
  char *fbuf;           /* file buffer */
  struct ftchash *ftch; /* hash table for value field */
};

int ftio_init(struct ftio *ftio, int fd, int flag);
int ftio_interrupt(struct ftio *ftio, uint32_t fields);
void ftio_set_preloaded(struct ftio *ftio, int flag);
void ftio_set_streaming(struct ftio *ftio, int flag);
int ftio_set_ver(struct ftio *ftio, struct ftver *ver);
void ftio_set_byte_order(struct ftio *ftio, int byte_order);
void ftio_set_z_level(struct ftio *ftio, int z_level);
void ftio_set_debug(struct ftio *ftio, int debug);
int ftio_set_comment(struct ftio *ftio, char *comment);
int ftio_set_cap_hostname(struct ftio *ftio, char *hostname);
void ftio_set_cap_time(struct ftio *ftio, uint32_t start, uint32_t end);
void ftio_set_cap_time_start(struct ftio *ftio, uint32_t start);
void ftio_set_flows_count(struct ftio *ftio, uint32_t n);
void ftio_set_corrupt(struct ftio *ftio, uint32_t n);
void ftio_set_lost(struct ftio *ftio, uint32_t n);
void ftio_set_reset(struct ftio *ftio, uint32_t n);
void ftio_set_xip(struct ftio *ftio, uint32_t ip);

void ftio_get_ver(struct ftio *ftio, struct ftver *ver);
uint32_t ftio_get_cap_start(const struct ftio *ftio);
uint32_t ftio_get_cap_end(const struct ftio *ftio);
time_t ftio_get_cap_start_time_t(const struct ftio *ftio);
time_t ftio_get_cap_end_time_t(const struct ftio *ftio);
uint64_t ftio_get_rec_total(struct ftio *ftio);
int ftio_get_debug(struct ftio *ftio);
uint32_t ftio_get_corrupt(struct ftio *ftio);
uint32_t ftio_get_lost(struct ftio *ftio);
uint32_t ftio_get_flows_count(struct ftio *ftio);

char *ftio_get_hostname(struct ftio *ftio);
char *ftio_get_comment(struct ftio *ftio);
int ftio_close(struct ftio *ftio);
void *ftio_read(struct ftio *ftio);
int ftio_write(struct ftio *ftio, void *data);
int ftio_write_header(struct ftio *ftio);
void *ftio_rec_swapfunc(struct ftio *ftio);
int ftio_rec_size(struct ftio *ftio);
void ftio_header_swap(struct ftio *ftio);
void ftio_header_print(struct ftio *ftio, FILE *std, char cc);
void ftio_zstat_print(struct ftio *ftio, FILE *std);
int ftio_check_generic(struct ftio *ftio);
int ftio_check_generic5(struct ftio *ftio);
int ftio_check_xfield(struct ftio *ftio, uint64_t xfield_need);

int ftio_map_load(struct ftio *ftio, char *fname, uint32_t ip);
uint64_t ftio_xfield(struct ftio *ftio);


int ftrec_size(struct ftver *ver);
uint64_t ftrec_xfield(struct ftver *ver);



void fts3rec_swap_v1(struct fts3rec_v1 *rec);
void fts3rec_swap_v5(struct fts3rec_v5 *rec);
void fts3rec_swap_v6(struct fts3rec_v6 *rec);
void fts3rec_swap_v7(struct fts3rec_v7 *rec);
void fts3rec_swap_v8_1(struct fts3rec_v8_1 *rec);
void fts3rec_swap_v8_2(struct fts3rec_v8_2 *rec);
void fts3rec_swap_v8_3(struct fts3rec_v8_3 *rec);
void fts3rec_swap_v8_4(struct fts3rec_v8_4 *rec);
void fts3rec_swap_v8_5(struct fts3rec_v8_5 *rec);
void fts3rec_swap_v8_6(struct fts3rec_v8_6 *rec);
void fts3rec_swap_v8_7(struct fts3rec_v8_7 *rec);
void fts3rec_swap_v8_8(struct fts3rec_v8_8 *rec);
void fts3rec_swap_v8_9(struct fts3rec_v8_9 *rec);
void fts3rec_swap_v8_10(struct fts3rec_v8_10 *rec);
void fts3rec_swap_v8_11(struct fts3rec_v8_11 *rec);
void fts3rec_swap_v8_12(struct fts3rec_v8_12 *rec);
void fts3rec_swap_v8_13(struct fts3rec_v8_13 *rec);
void fts3rec_swap_v8_14(struct fts3rec_v8_14 *rec);
void fts1rec_swap_compat(struct fts1rec_compat *rec);
void fts3rec_swap_v1005(struct fts3rec_v1005 *rec);

int fts3rec_pdu_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v1_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v5_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v6_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v7_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_1_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_2_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_3_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_4_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_5_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_6_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_7_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_8_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_9_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_10_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_11_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_12_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_13_decode(struct ftpdu *ftpdu);
int fts3rec_pdu_v8_14_decode(struct ftpdu *ftpdu);

int fts3rec_pdu_encode(struct ftencode *enc, void *rec);
int fts3rec_pdu_v1_encode(struct ftencode *enc, struct fts3rec_v1 *rec_v1);
int fts3rec_pdu_v5_encode(struct ftencode *enc, struct fts3rec_v5 *rec_v5);
int fts3rec_pdu_v6_encode(struct ftencode *enc, struct fts3rec_v6 *rec_v6);
int fts3rec_pdu_v7_encode(struct ftencode *enc, struct fts3rec_v7 *rec_v7);
int fts3rec_pdu_v8_1_encode(struct ftencode *enc,
  struct fts3rec_v8_1 *rec_v8_1);
int fts3rec_pdu_v8_2_encode(struct ftencode *enc,
  struct fts3rec_v8_2 *rec_v8_2);
int fts3rec_pdu_v8_3_encode(struct ftencode *enc,
  struct fts3rec_v8_3 *rec_v8_3);
int fts3rec_pdu_v8_4_encode(struct ftencode *enc,
  struct fts3rec_v8_4 *rec_v8_4);
int fts3rec_pdu_v8_5_encode(struct ftencode *enc,
  struct fts3rec_v8_5 *rec_v8_5);
int fts3rec_pdu_v8_6_encode(struct ftencode *enc,
  struct fts3rec_v8_6 *rec_v8_6);
int fts3rec_pdu_v8_7_encode(struct ftencode *enc,
  struct fts3rec_v8_7 *rec_v8_7);
int fts3rec_pdu_v8_8_encode(struct ftencode *enc,
  struct fts3rec_v8_8 *rec_v8_8);
int fts3rec_pdu_v8_9_encode(struct ftencode *enc,
  struct fts3rec_v8_9 *rec_v8_9);
int fts3rec_pdu_v8_10_encode(struct ftencode *enc,
  struct fts3rec_v8_10 *rec_v8_10);
int fts3rec_pdu_v8_11_encode(struct ftencode *enc,
  struct fts3rec_v8_11 *rec_v8_11);
int fts3rec_pdu_v8_12_encode(struct ftencode *enc,
  struct fts3rec_v8_12 *rec_v8_12);
int fts3rec_pdu_v8_13_encode(struct ftencode *enc,
  struct fts3rec_v8_13 *rec_v8_13);
int fts3rec_pdu_v8_14_encode(struct ftencode *enc,
  struct fts3rec_v8_14 *rec_v8_14);

int ftprof_start(struct ftprof *ftp);
int ftprof_end(struct ftprof *ftp, uint64_t nflows);
void ftprof_print(struct ftprof *ftp, char *prog, FILE *std);

int ftiheader_read(int fd, struct ftiheader *h);

struct fttime ftltime(uint32_t sys, uint32_t secs, uint32_t nsecs, uint32_t t);

void ftencode_init(struct ftencode *enc, int flags);
void ftencode_reset(struct ftencode *enc);
void ftencode_sum_data(struct ftencode *enc);

void ftpdu_swap(void *pdu, int cur);
void ftpdu_v1_swap(struct ftpdu_v1 *pdu, int cur);
void ftpdu_v5_swap(struct ftpdu_v5 *pdu, int cur);
void ftpdu_v6_swap(struct ftpdu_v6 *pdu, int cur);
void ftpdu_v7_swap(struct ftpdu_v7 *pdu, int cur);
void ftpdu_v8_1_swap(struct ftpdu_v8_1 *pdu, int cur);
void ftpdu_v8_2_swap(struct ftpdu_v8_2 *pdu, int cur);
void ftpdu_v8_3_swap(struct ftpdu_v8_3 *pdu, int cur);
void ftpdu_v8_4_swap(struct ftpdu_v8_4 *pdu, int cur);
void ftpdu_v8_5_swap(struct ftpdu_v8_5 *pdu, int cur);
void ftpdu_v8_6_swap(struct ftpdu_v8_6 *pdu, int cur);
void ftpdu_v8_7_swap(struct ftpdu_v8_7 *pdu, int cur);
void ftpdu_v8_8_swap(struct ftpdu_v8_8 *pdu, int cur);
void ftpdu_v8_9_swap(struct ftpdu_v8_9 *pdu, int cur);
void ftpdu_v8_10_swap(struct ftpdu_v8_10 *pdu, int cur);
void ftpdu_v8_11_swap(struct ftpdu_v8_11 *pdu, int cur);
void ftpdu_v8_12_swap(struct ftpdu_v8_12 *pdu, int cur);
void ftpdu_v8_13_swap(struct ftpdu_v8_13 *pdu, int cur);
void ftpdu_v8_14_swap(struct ftpdu_v8_14 *pdu, int cur);

int ftpdu_verify(struct ftpdu *pdu);
int ftpdu_check_seq(struct ftpdu *pdu, struct ftseq *ftseq);

int fttlv_enc_uint32(void *buf, int buf_size, int flip, uint16_t t, uint32_t v);
int fttlv_enc_uint16(void *buf, int buf_size, int flip, uint16_t t, uint16_t v);
int fttlv_enc_uint8(void *buf, int buf_size, int flip, uint16_t t, uint8_t v);
int fttlv_enc_str(void *buf, int buf_size, int flip, uint16_t t, char *v);
int fttlv_enc_ifname(void *buf, int buf_size, int flip, uint16_t t,
  uint32_t ip, uint16_t ifIndex, char *name);
int fttlv_enc_ifalias(void *buf, int buf_size, int flip, uint16_t t,
  uint32_t ip, uint16_t *ifIndex_list, uint16_t entries, char *name);


void ftfile_free(struct ftfile_entries *fte);
int ftfile_loadfile(struct ftfile_entries *fte, char *fname, int sort);
int ftfile_loaddir(struct ftfile_entries *fte, char *dir, int sort);
int ftfile_add_tail(struct ftfile_entries *fte, char *fname, off_t size,
  uint32_t start);
int ftfile_expire (struct ftfile_entries *fte, int doit, int curbytes);
int ftfile_dump(struct ftfile_entries *fte);
struct ftfile_entry *ftfile_entry_new(int len);
void ftfile_entry_free(struct ftfile_entry *entry);
int ftfile_mkpath(time_t ftime, int nest);
void ftfile_pathname(char *buf, int bsize, int nest, struct ftver ftv,
 int done, time_t ftime);



void ftset_init(struct ftset *ftset, int z_level);

struct ftmap *ftmap_load(char *fname, uint32_t ip);
void ftmap_free(struct ftmap *ftmap);
struct ftmap *ftmap_new(void);
struct ftmap_ifalias *ftmap_ifalias_new(uint32_t ip, uint16_t *ifIndex_list,
  uint16_t entries, char *name);
struct ftmap_ifname *ftmap_ifname_new(uint32_t ip, uint16_t ifIndex, char *name);

/* fterr */

void fterr_setid(char *id);
void fterr_setfile(int enable, void *fp);
void fterr_setsyslog(int enable, int logopt, int facility);
void fterr_setexit(void (*f)(int));
void fterr_warn(const char *fmt, ...);
void fterr_warnx(const char *fmt, ...);
void fterr_err(int code, const char *fmt, ...);
void fterr_errx(int code, const char *fmt, ...);
void fterr_info(const char *fmt, ...);

int ftrec_mask_ip(void *rec, struct ftver *ftv, struct ftipmask *m);
void ftrec_compute_mask(struct ftipmask *m, uint32_t src, uint32_t dst, int
  byte_order);

/* ftvar */
struct ftvar_entry {
  FT_SLIST_ENTRY(ftvar_entry) chain; /* list */
  char *name, *val;
};
 
struct ftvar { 
   FT_SLIST_HEAD(ftvarhead, ftvar_entry) entries; /* variables */
};
   

/* ftchash_ */
struct ftchash *ftchash_new(int h_size, int d_size, int key_size,
  int chunk_entries);
void *ftchash_lookup(struct ftchash *ftch, void *key, uint32_t hash);
void ftchash_free(struct ftchash *ftch);
void *ftchash_update(struct ftchash *ftch, void *newrec, uint32_t hash);
void *ftchash_alloc_rec(struct ftchash *ftch);
void *ftchash_foreach(struct ftchash *ftch);
void ftchash_first(struct ftchash *ftch);
int ftchash_sort(struct ftchash *ftch, int offset, int flags);

void ftrec_xlate(void *in_rec, struct ftver *in_ftv, void *out_rec,
  struct ftver *out_ftv);

void *ftrec_xlate_func(struct ftver *in_ftv, struct ftver *out_ftv);

int fts3rec_compute_offsets(struct fts3rec_offsets *o, struct ftver *ftv);

struct ftsym *ftsym_new(const char *fname);
void ftsym_free(struct ftsym *ftsym);
int ftsym_findbyname(struct ftsym *ftsym, const char *name, uint32_t *val);
int ftsym_findbyval(struct ftsym *ftsym, uint32_t val, char **name);

/* fttag */
int fttag_load(struct fttag *fttag, struct ftvar *ftvar, const char *fname);
void fttag_free(struct fttag *fttag);
int fttag_def_eval(struct fttag_def *ftd, struct fts3rec_v1005 *rec_out);
struct fttag_def *fttag_def_find(struct fttag *fttag, const char *name);

/* ftxlate */
int ftxlate_load(struct ftxlate *ftxlate, struct ftvar *ftvar, const char *fname);
void ftxlate_free(struct ftxlate *ftxlate);
int ftxlate_def_eval(struct ftxlate_def *ftd, char *rec,
  struct fts3rec_offsets *fo);
struct ftxlate_def *ftxlate_def_find(struct ftxlate *ftxlate, const char *name);
int ftxlate_def_test_xfields(struct ftxlate_def *active_def, uint64_t test);

/* ftfil */
struct ftfil_def *ftfil_def_find(struct ftfil *ftfil, const char *name);
int ftfil_def_eval(struct ftfil_def *active_def,
  char *rec, struct fts3rec_offsets *fo);
void ftfil_free(struct ftfil *ftfil);
int ftfil_load(struct ftfil *ftfil, struct ftvar *ftvar, const char *fname);
int ftfil_def_test_xfields(struct ftfil_def *active_def, uint64_t test);


enum ftstat_rpt_format {FT_STAT_FMT_UNSET,
                        FT_STAT_FMT_ASCII,
                        FT_STAT_FMT_BINARY};

enum ftstat_rpt_time { FT_STAT_TIME_UNSET, FT_STAT_TIME_NOW,
                       FT_STAT_TIME_START, FT_STAT_TIME_END,
                       FT_STAT_TIME_MID };

struct ftstat_rpt {
  FT_SLIST_ENTRY(ftstat_rpt) chain; /* list */
  FT_STAILQ_HEAD(ftstatrptouthead, ftstat_rpt_out) outs;
  struct ftstat_rpt_out *out; /* current output */
  enum ftstat_rpt_format format; /* FT_STAT_FMT* */
  char *name; /* name of the report */
  char *format_name; /* name of format */
  uint32_t scale; /* scale data by? 0=no scaling */
  uint32_t tag_mask_src, tag_mask_dst; /* enabled with FT_STAT_OPT_TAG_MASK  */
  int options; /* options FT_STAT_OPT_* default to 0 */
  int allowed_options; /* options supported by report */
  int allowed_fields; /* fields supported by report */
  uint32_t all_fields; /* all of out->fields */
  struct ftfil_def *ftfd; /* filter definition */
  uint64_t xfields; /* FT_XFIELD_* */
  void *data; /* ftstat_rpt_n */
  void* (*f_new)(struct ftstat_rpt *ftsrpt); /* ftstat_rpt_n_new */
  void* (*f_accum)(struct ftstat_rpt *ftsrpt, char *rec, struct fts3rec_offsets *fo); /* ftstat_rpt_n_accum */
  void* (*f_calc)(struct ftstat_rpt *ftsrpt); /* ftstat_rpt_n_calc */
  void* (*f_free)(void *data); /* ftstat_rpt_n_free */
  void* (*f_dump)(FILE *fp, struct ftio *ftio, struct ftstat_rpt *ftsrpt); /* ftstat_rpt_n_dump */
  /* totals */
  uint64_t t_ignores, t_recs, t_flows, t_octets, t_packets, t_duration;
  uint64_t t_count;
  double avg_pps, min_pps, max_pps;
  double avg_bps, min_bps, max_bps;
  uint32_t time_start; /* real time of earliest flow */
  uint32_t time_end; /* real time of latest flow */
  uint64_t recs; /* records in report */
  uint64_t idx;
};

struct ftstat_rpt_out {
  FT_STAILQ_ENTRY(ftstat_rpt_out) chain; /* list */
  uint64_t records; /* report records 0=all */
  uint32_t tally; /* tally lines 0=none */
  int options; /* options FT_STAT_OPT_* default to 0 */
  uint32_t fields; /* FT_STAT_FIELD_* - default to allowed_fields */
  int sort_field;  /* field to sort on FT_STAT_FIELD_* */
  int sort_order; /* FT_STAT_FIELD_SORT_* */
  enum ftstat_rpt_time time; /* where to derive clock for output path */
  char *path; /* pathname of output */
  void* (*f_dump)(FILE *fp, struct ftio *ftio, struct ftstat_rpt *ftsrpt); /* ftstat_rpt_n_dump */
};

struct ftstat_rpt_item {
  FT_STAILQ_ENTRY(ftstat_rpt_item) chain; /* list */
  char *tmp_report; /* temporary pointer to report name */
  struct ftstat_rpt *rpt;
};

struct ftstat_def {
  FT_SLIST_ENTRY(ftstat_def) chain; /* list */
  FT_STAILQ_HEAD(ftstatrptihead, ftstat_rpt_item) items;
  char *name; /* name of the def */
  struct ftfil_def *ftfd; /* filter definition */
  struct fttag_def *ftd; /* tag definition */
  struct ftmask_def *ftmd; /* tag definition */
  struct ftstat *ftstat; /* back pointer */
  uint32_t max_time; /* maximum seconds for report definition */
  uint32_t start_time; /* start seconds */
  uint32_t interval; /* time series interval */
  uint64_t xfields; /* required flow fields - FT_XFIELD_* */
};
  
struct ftstat {
  FT_SLIST_HEAD(statrpthead, ftstat_rpt) rpts; /* reports */
  FT_SLIST_HEAD(statdefhead, ftstat_def) defs; /* defs */
  struct fttag fttag;
  struct ftfil ftfil;
  struct ftmask ftmask;
  int fttag_init; /* fttag initialized? */
  int ftfil_init; /* ftfil initialized? */
  int ftmask_init; /* ftmask initialized? */
  char *tag_fname, *filter_fname, *mask_fname;
  struct ftvar *ftvar;
};

/* ftstat */
int ftstat_load(struct ftstat *ftstat, struct ftvar *ftvar, const char *fname);
void ftstat_free(struct ftstat *ftstat);
struct ftstat_def *ftstat_def_find(struct ftstat *ftstat, const char *name);
int ftstat_def_test_xfields(struct ftstat_def *active_def, uint64_t test);
int ftstat_def_new(struct ftstat_def *active_def);
int ftstat_def_accum(struct ftstat_def *active_def,
  char *rec, struct fts3rec_offsets *fo);
int ftstat_def_calc(struct ftstat_def *active_def);
int ftstat_def_dump(struct ftio *ftio, struct ftstat_def *active_def);
int ftstat_def_free(struct ftstat_def *active_def);
int ftstat_def_reset(struct ftstat_def *active_def);
void ftstat_list_reports(FILE *out);

/* fttag */
struct fttag_action {
  FT_SLIST_ENTRY(fttag_action) chain; /* list of all actions */
  int type; /* FT_TAG_TYPE_MATCH_* */
  char *name;
  void *look;
  void (*eval)(struct fttag_action *fta, struct fts3rec_v1005 *rec);
};

struct fttag_def_term {
  FT_STAILQ_ENTRY(fttag_def_term) chain; /* list of terms */
  FT_STAILQ_HEAD(actdhead, fttag_def_term_actions) actions; /* actions */
  int type; /* FT_TAG_TYPE_MATCH_* */
  int flags; /* FT_TAG_DEF_ */
  uint32_t exporter_ip; /* exporter filter */
  char in_tbl[65536]; /* input filter */
  char out_tbl[65536]; /* output filter */
};

struct fttag_def_term_actions {
  FT_STAILQ_ENTRY(fttag_def_term_actions) chain; /* all actions */
  struct fttag_action *action; /* filled in by resolve_actions */
  char *name; /* temporary, invalid after config file is closed */
};

struct fttag_def {
  FT_SLIST_ENTRY(fttag_def) chain;
  FT_STAILQ_HEAD(dthead, fttag_def_term) terms; /* terms */
  char *name;
};

int ftvar_new(struct ftvar *ftvar);
void ftvar_free(struct ftvar *ftvar);
int ftvar_set(struct ftvar *ftvar, char *name, char *val);
struct ftvar_entry *ftvar_find(struct ftvar *ftvar, char *name);
void ftvar_clear(struct ftvar *ftvar, char *name);
int ftvar_pset(struct ftvar *ftvar, char *binding);
int ftvar_evalstr(struct ftvar *ftvar, char *src, char *dst, int dstlen);


struct ftpeeri {
  uint32_t loc_ip;        /* local ip address */
  uint32_t rem_ip;        /* remote ip address */
  uint16_t dst_port;      /* destination port */
  uint8_t  ttl;           /* ttl */
};

struct ip_prefix {
  uint32_t addr;
  uint8_t len;
};

void print_3float(float f);
void print_3float2(float f);
int load_lookup(char *s, int size, char *list);
uint32_t scan_ip(char *s);
int64_t scan_size(char *val);
int udp_cksum(struct ip *ip, struct udphdr *up, int len);

int unlink_pidfile(int pid, char *file, uint16_t port);
int write_pidfile(int pid, char *file, uint16_t port);

struct ftpeeri scan_peeri(char *input);
struct ip_prefix scan_ip_prefix(char *input);

#ifdef POSIX_SIGNALS
#define signal mysignal
void *mysignal(int signo, void *func);
#endif /* POSIX_SIGNALS */

int get_gmtoff(time_t t);

uint32_t ipv4_len2mask(uint8_t len);

int bigsockbuf(int fd, int dir, int size);

int mkpath(const char *path, mode_t mode);

time_t get_date (const char *p, const time_t *now);

struct bit1024 {
 uint32_t n[32];
};

void bit1024_store(int d, struct bit1024 *old);
void bit1024_print(FILE *FP, struct bit1024 *b);
int bit1024_count(struct bit1024 *b);

#define FMT_UINT64  21      /* 18446744073709551615 + \0 */
#define FMT_UINT32  11      /* 4294967295 + \0 */
#define FMT_UINT16  6       /* 65535 + \0 */
#define FMT_UINT8   4       /* 256 + \0 */
#define FMT_IPV4  16        /* 111.111.111.111 + \0 */
#define FMT_IPV4_PREFIX 19  /* 111.111.111.111/32 + \0 */

#define FMT_PAD_LEFT    0
#define FMT_PAD_RIGHT   1
#define FMT_JUST_LEFT   2
#define FMT_JUST_RIGHT  FMT_PAD_LEFT
#define FMT_SYM         4

unsigned int fmt_uint8(char *s, uint8_t u, int format);
unsigned int fmt_uint16(char *s, uint16_t u, int format);
unsigned int fmt_uint32(char *s, uint32_t u, int format);
unsigned int fmt_uint64(char *s, uint64_t u, int format);
unsigned int fmt_ipv4(char *s, uint32_t u, int format);
unsigned int fmt_ipv4s(char *s, uint32_t u, int len,
  int format);
unsigned int fmt_ipv4prefix(char *s, uint32_t u,
  unsigned char mask, int format);
unsigned int fmt_ipv4prefixs(char *s, uint32_t u,
  unsigned char mask, int len, int format);

unsigned int fmt_uint8s(struct ftsym *ftsym, int max, char *s, uint8_t u,
  int format);

unsigned int fmt_uint16s(struct ftsym *ftsym, int max, char *s, uint16_t u,
  int format);

unsigned int fmt_uint32s(struct ftsym *ftsym, int max, char *s, uint32_t u,
  int format);

void *mysignal(int signo, void *func);

/* ftxfield */
int ftxfield_parse(char *line, uint64_t *xfields);

#ifndef IN_CLASSD_SSM
#define IN_CLASSD_SSM(i) (((uint32_t)(i) & 0xff000000) == 0xe8000000)
#endif /* IN_CLASSD_SSM */

enum ft_config_path {
  _FT_PATH_CFG_MAP,
  _FT_PATH_CFG_TAG,
  _FT_PATH_CFG_FILTER,
  _FT_PATH_CFG_STAT,
  _FT_PATH_CFG_MASK,
  _FT_PATH_CFG_XLATE,
  _FT_PATH_SYM_IP_PROT,
  _FT_PATH_SYM_IP_TYPE,
  _FT_PATH_SYM_TCP_PORT,
  _FT_PATH_SYM_ASN,
  _FT_PATH_SYM_TAG
};

const char *ft_get_path(enum ft_config_path pathid);

#define FTPATHS_H

#define FT_PATH_CFG_MAP ft_get_path(_FT_PATH_CFG_MAP)
#define FT_PATH_CFG_TAG ft_get_path(_FT_PATH_CFG_TAG)
#define FT_PATH_CFG_FILTER ft_get_path(_FT_PATH_CFG_FILTER)
#define FT_PATH_CFG_STAT ft_get_path(_FT_PATH_CFG_STAT)
#define FT_PATH_CFG_MASK ft_get_path(_FT_PATH_CFG_MASK)
#define FT_PATH_CFG_XLATE ft_get_path(_FT_PATH_CFG_XLATE)

#define FT_PATH_SYM_IP_PROT ft_get_path(_FT_PATH_SYM_IP_PROT)
#define FT_PATH_SYM_IP_TYPE ft_get_path(_FT_PATH_SYM_IP_TYPE)
#define FT_PATH_SYM_TCP_PORT ft_get_path(_FT_PATH_SYM_TCP_PORT)
#define FT_PATH_SYM_ASN ft_get_path(_FT_PATH_SYM_ASN)
#define FT_PATH_SYM_TAG ft_get_path(_FT_PATH_SYM_TAG)

#endif /* FTLIB_H */
