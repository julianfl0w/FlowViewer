/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2009, 2010 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     HOST = 258,
     ANY = 259,
     IPADDR = 260,
     NUM = 261,
     NAME = 262,
     ACCESSLIST = 263,
     IP = 264,
     PERMIT = 265,
     DENY = 266,
     COMMENT = 267,
     ICMP = 268,
     IGMP = 269,
     TCP = 270,
     UDP = 271,
     PRECEDENCE = 272,
     TOS = 273,
     LOG = 274,
     ICMPTYPE = 275,
     LT = 276,
     GT = 277,
     EQ = 278,
     NEQ = 279,
     RANGE = 280,
     ESTABLISHED = 281,
     EXTENDED = 282,
     STANDARD = 283,
     NL = 284
   };
#endif
/* Tokens.  */
#define HOST 258
#define ANY 259
#define IPADDR 260
#define NUM 261
#define NAME 262
#define ACCESSLIST 263
#define IP 264
#define PERMIT 265
#define DENY 266
#define COMMENT 267
#define ICMP 268
#define IGMP 269
#define TCP 270
#define UDP 271
#define PRECEDENCE 272
#define TOS 273
#define LOG 274
#define ICMPTYPE 275
#define LT 276
#define GT 277
#define EQ 278
#define NEQ 279
#define RANGE 280
#define ESTABLISHED 281
#define EXTENDED 282
#define STANDARD 283
#define NL 284




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 16 "aclyacc.y"

	char *c;
	uint32_t ip;
	int i;
	u_int u;
	struct acl_ip_std_entry std_entry;
	struct acl_ip_ext_entry ext_entry;



/* Line 1685 of yacc.c  */
#line 120 "aclyacc.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


