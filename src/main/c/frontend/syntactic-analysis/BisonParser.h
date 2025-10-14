/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_MAIN_C_FRONTEND_SYNTACTIC_ANALYSIS_BISONPARSER_H_INCLUDED
# define YY_YY_SRC_MAIN_C_FRONTEND_SYNTACTIC_ANALYSIS_BISONPARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INTEGER = 258,                 /* INTEGER  */
    ADD = 259,                     /* ADD  */
    CLOSE_BRACE = 260,             /* CLOSE_BRACE  */
    CLOSE_COMMENT = 261,           /* CLOSE_COMMENT  */
    CLOSE_PARENTHESIS = 262,       /* CLOSE_PARENTHESIS  */
    DIV = 263,                     /* DIV  */
    MUL = 264,                     /* MUL  */
    OPEN_BRACE = 265,              /* OPEN_BRACE  */
    OPEN_COMMENT = 266,            /* OPEN_COMMENT  */
    OPEN_PARENTHESIS = 267,        /* OPEN_PARENTHESIS  */
    SUB = 268,                     /* SUB  */
    IGNORED = 269,                 /* IGNORED  */
    UNKNOWN = 270,                 /* UNKNOWN  */
    SCENE = 271,                   /* SCENE  */
    DRAW = 272,                    /* DRAW  */
    WITH = 273,                    /* WITH  */
    LAYER = 274,                   /* LAYER  */
    GROUP = 275,                   /* GROUP  */
    SYMBOL = 276,                  /* SYMBOL  */
    USE = 277,                     /* USE  */
    PALETTE = 278,                 /* PALETTE  */
    BACKGROUND = 279,              /* BACKGROUND  */
    RECTANGLE = 280,               /* RECTANGLE  */
    CIRCLE = 281,                  /* CIRCLE  */
    LINE = 282,                    /* LINE  */
    ELLIPSE = 283,                 /* ELLIPSE  */
    POLYLINE = 284,                /* POLYLINE  */
    POLYGON = 285,                 /* POLYGON  */
    AT = 286,                      /* AT  */
    FROM = 287,                    /* FROM  */
    TO = 288,                      /* TO  */
    SIZE = 289,                    /* SIZE  */
    RADIUS = 290,                  /* RADIUS  */
    WIDTH = 291,                   /* WIDTH  */
    HEIGHT = 292,                  /* HEIGHT  */
    FILL = 293,                    /* FILL  */
    STROKE = 294,                  /* STROKE  */
    STROKE_WIDTH = 295,            /* STROKE_WIDTH  */
    OPACITY = 296,                 /* OPACITY  */
    Z = 297,                       /* Z  */
    ID = 298,                      /* ID  */
    TRANSLATE = 299,               /* TRANSLATE  */
    ROTATE = 300,                  /* ROTATE  */
    SCALE = 301,                   /* SCALE  */
    SCALING = 302,                 /* SCALING  */
    COLOR = 303,                   /* COLOR  */
    HEX_COLOR = 304,               /* HEX_COLOR  */
    RGB_COLOR = 305,               /* RGB_COLOR  */
    RGBA_COLOR = 306,              /* RGBA_COLOR  */
    IDENTIFIER = 307,              /* IDENTIFIER  */
    STRING = 308,                  /* STRING  */
    DECIMAL = 309,                 /* DECIMAL  */
    DIMENSIONS = 310,              /* DIMENSIONS  */
    COORDINATES = 311,             /* COORDINATES  */
    SEMICOLON = 312,               /* SEMICOLON  */
    COMMA = 313,                   /* COMMA  */
    COLON = 314                    /* COLON  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
#line 34 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
union SemanticValue
{
#line 40 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"

	/** Terminals. */

	signed int integer;
	TokenLabel token;
	char * string;
	float decimal;
	struct {
		int width;
		int height;
		UnitType widthUnit;
        UnitType heightUnit;
	} dimensions;
	struct {
		int x;
		int y;
	} coordinates;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
	Scene * scene;
	Figure * figure;
	Property * property;
	Color * color;
	PaletteEntry * palette_entry;

#line 155 "src/main/c/frontend/syntactic-analysis/BisonParser.h"

};
#line 34 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
typedef union SemanticValue YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




#ifndef YYPUSH_MORE_DEFINED
# define YYPUSH_MORE_DEFINED
enum { YYPUSH_MORE = 4 };
#endif

typedef struct yypstate yypstate;


int yypush_parse (yypstate *ps,
                  int pushed_char, YYSTYPE const *pushed_val, YYLTYPE *pushed_loc);

yypstate *yypstate_new (void);
void yypstate_delete (yypstate *ps);


#endif /* !YY_YY_SRC_MAIN_C_FRONTEND_SYNTACTIC_ANALYSIS_BISONPARSER_H_INCLUDED  */
