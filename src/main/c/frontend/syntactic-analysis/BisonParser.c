/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0




/* First part of user prologue.  */
#line 1 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"


#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

/**
 * The error reporting function for Bison parser.
 *
 * @todo Add location to the grammar and "pushToken" API function.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
 */
void yyerror(const YYLTYPE * location, const char * message) {
}


#line 90 "src/main/c/frontend/syntactic-analysis/BisonParser.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "BisonParser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INTEGER = 3,                    /* INTEGER  */
  YYSYMBOL_ADD = 4,                        /* ADD  */
  YYSYMBOL_CLOSE_BRACE = 5,                /* CLOSE_BRACE  */
  YYSYMBOL_CLOSE_COMMENT = 6,              /* CLOSE_COMMENT  */
  YYSYMBOL_CLOSE_PARENTHESIS = 7,          /* CLOSE_PARENTHESIS  */
  YYSYMBOL_DIV = 8,                        /* DIV  */
  YYSYMBOL_MUL = 9,                        /* MUL  */
  YYSYMBOL_OPEN_BRACE = 10,                /* OPEN_BRACE  */
  YYSYMBOL_OPEN_COMMENT = 11,              /* OPEN_COMMENT  */
  YYSYMBOL_OPEN_PARENTHESIS = 12,          /* OPEN_PARENTHESIS  */
  YYSYMBOL_SUB = 13,                       /* SUB  */
  YYSYMBOL_IGNORED = 14,                   /* IGNORED  */
  YYSYMBOL_UNKNOWN = 15,                   /* UNKNOWN  */
  YYSYMBOL_SCENE = 16,                     /* SCENE  */
  YYSYMBOL_DRAW = 17,                      /* DRAW  */
  YYSYMBOL_WITH = 18,                      /* WITH  */
  YYSYMBOL_LAYER = 19,                     /* LAYER  */
  YYSYMBOL_GROUP = 20,                     /* GROUP  */
  YYSYMBOL_SYMBOL = 21,                    /* SYMBOL  */
  YYSYMBOL_USE = 22,                       /* USE  */
  YYSYMBOL_PALETTE = 23,                   /* PALETTE  */
  YYSYMBOL_BACKGROUND = 24,                /* BACKGROUND  */
  YYSYMBOL_RECTANGLE = 25,                 /* RECTANGLE  */
  YYSYMBOL_CIRCLE = 26,                    /* CIRCLE  */
  YYSYMBOL_LINE = 27,                      /* LINE  */
  YYSYMBOL_ELLIPSE = 28,                   /* ELLIPSE  */
  YYSYMBOL_POLYLINE = 29,                  /* POLYLINE  */
  YYSYMBOL_POLYGON = 30,                   /* POLYGON  */
  YYSYMBOL_AT = 31,                        /* AT  */
  YYSYMBOL_FROM = 32,                      /* FROM  */
  YYSYMBOL_TO = 33,                        /* TO  */
  YYSYMBOL_SIZE = 34,                      /* SIZE  */
  YYSYMBOL_RADIUS = 35,                    /* RADIUS  */
  YYSYMBOL_WIDTH = 36,                     /* WIDTH  */
  YYSYMBOL_HEIGHT = 37,                    /* HEIGHT  */
  YYSYMBOL_FILL = 38,                      /* FILL  */
  YYSYMBOL_STROKE = 39,                    /* STROKE  */
  YYSYMBOL_STROKE_WIDTH = 40,              /* STROKE_WIDTH  */
  YYSYMBOL_OPACITY = 41,                   /* OPACITY  */
  YYSYMBOL_Z = 42,                         /* Z  */
  YYSYMBOL_ID = 43,                        /* ID  */
  YYSYMBOL_TRANSLATE = 44,                 /* TRANSLATE  */
  YYSYMBOL_ROTATE = 45,                    /* ROTATE  */
  YYSYMBOL_SCALE = 46,                     /* SCALE  */
  YYSYMBOL_COLOR = 47,                     /* COLOR  */
  YYSYMBOL_HEX_COLOR = 48,                 /* HEX_COLOR  */
  YYSYMBOL_RGB_COLOR = 49,                 /* RGB_COLOR  */
  YYSYMBOL_RGBA_COLOR = 50,                /* RGBA_COLOR  */
  YYSYMBOL_IDENTIFIER = 51,                /* IDENTIFIER  */
  YYSYMBOL_STRING = 52,                    /* STRING  */
  YYSYMBOL_DECIMAL = 53,                   /* DECIMAL  */
  YYSYMBOL_DIMENSIONS = 54,                /* DIMENSIONS  */
  YYSYMBOL_COORDINATES = 55,               /* COORDINATES  */
  YYSYMBOL_SEMICOLON = 56,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 57,                     /* COMMA  */
  YYSYMBOL_COLON = 58,                     /* COLON  */
  YYSYMBOL_YYACCEPT = 59,                  /* $accept  */
  YYSYMBOL_program = 60,                   /* program  */
  YYSYMBOL_scene_declaration = 61,         /* scene_declaration  */
  YYSYMBOL_scene_content = 62,             /* scene_content  */
  YYSYMBOL_figure_declaration = 63,        /* figure_declaration  */
  YYSYMBOL_draw_statement = 64,            /* draw_statement  */
  YYSYMBOL_draw_tail = 65,                 /* draw_tail  */
  YYSYMBOL_external_items = 66,            /* external_items  */
  YYSYMBOL_external_items_more = 67,       /* external_items_more  */
  YYSYMBOL_external_item = 68,             /* external_item  */
  YYSYMBOL_position_item = 69,             /* position_item  */
  YYSYMBOL_size_item = 70,                 /* size_item  */
  YYSYMBOL_figure_with_id = 71,            /* figure_with_id  */
  YYSYMBOL_figure_anonymous = 72,          /* figure_anonymous  */
  YYSYMBOL_figure_type = 73,               /* figure_type  */
  YYSYMBOL_figure_properties = 74,         /* figure_properties  */
  YYSYMBOL_property_list = 75,             /* property_list  */
  YYSYMBOL_property_item = 76,             /* property_item  */
  YYSYMBOL_size_property = 77,             /* size_property  */
  YYSYMBOL_position_property = 78,         /* position_property  */
  YYSYMBOL_fill_property = 79,             /* fill_property  */
  YYSYMBOL_stroke_property = 80,           /* stroke_property  */
  YYSYMBOL_radius_property = 81,           /* radius_property  */
  YYSYMBOL_from_to_property = 82,          /* from_to_property  */
  YYSYMBOL_stroke_width_property = 83,     /* stroke_width_property  */
  YYSYMBOL_opacity_property = 84,          /* opacity_property  */
  YYSYMBOL_scale_property = 85,            /* scale_property  */
  YYSYMBOL_rotate_property = 86,           /* rotate_property  */
  YYSYMBOL_dimensions_value = 87,          /* dimensions_value  */
  YYSYMBOL_coordinates_value = 88,         /* coordinates_value  */
  YYSYMBOL_color_value = 89                /* color_value  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   108

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  60
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  123

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   313


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   194,   194,   197,   200,   221,   224,   236,   252,   256,
     260,   268,   269,   272,   280,   286,   290,   296,   304,   310,
     316,   320,   326,   333,   340,   346,   352,   353,   354,   355,
     356,   357,   360,   364,   392,   396,   403,   407,   411,   415,
     419,   423,   427,   431,   435,   439,   445,   452,   459,   466,
     471,   478,   485,   490,   497,   504,   511,   518,   525,   532,
     539
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INTEGER", "ADD",
  "CLOSE_BRACE", "CLOSE_COMMENT", "CLOSE_PARENTHESIS", "DIV", "MUL",
  "OPEN_BRACE", "OPEN_COMMENT", "OPEN_PARENTHESIS", "SUB", "IGNORED",
  "UNKNOWN", "SCENE", "DRAW", "WITH", "LAYER", "GROUP", "SYMBOL", "USE",
  "PALETTE", "BACKGROUND", "RECTANGLE", "CIRCLE", "LINE", "ELLIPSE",
  "POLYLINE", "POLYGON", "AT", "FROM", "TO", "SIZE", "RADIUS", "WIDTH",
  "HEIGHT", "FILL", "STROKE", "STROKE_WIDTH", "OPACITY", "Z", "ID",
  "TRANSLATE", "ROTATE", "SCALE", "COLOR", "HEX_COLOR", "RGB_COLOR",
  "RGBA_COLOR", "IDENTIFIER", "STRING", "DECIMAL", "DIMENSIONS",
  "COORDINATES", "SEMICOLON", "COMMA", "COLON", "$accept", "program",
  "scene_declaration", "scene_content", "figure_declaration",
  "draw_statement", "draw_tail", "external_items", "external_items_more",
  "external_item", "position_item", "size_item", "figure_with_id",
  "figure_anonymous", "figure_type", "figure_properties", "property_list",
  "property_item", "size_property", "position_property", "fill_property",
  "stroke_property", "radius_property", "from_to_property",
  "stroke_width_property", "opacity_property", "scale_property",
  "rotate_property", "dimensions_value", "coordinates_value",
  "color_value", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-49)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      37,   -49,   -49,   -48,    19,   -49,    -4,   -49,   -49,    11,
       7,    42,   -49,    29,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,    -9,   -11,    -8,   -24,    51,    59,
     -24,    10,    52,    52,    52,     9,    62,    15,   -38,    64,
      16,    56,    58,    66,   -49,   -24,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,   -49,   -49,   -24,    65,    68,   -24,
      52,     9,   -49,    67,    18,   -49,   -49,    72,    20,    23,
      24,   -49,    25,    26,   -49,    27,   -49,    28,    30,    31,
      75,    82,   -49,   -49,    83,    79,   -49,    85,   -49,   -49,
     -24,    -6,   -49,    34,   -49,   -49,   -49,   -49,   -49,   -49,
     -49,   -49,   -49,    86,    35,   -49,   -49,   -49,    89,    18,
      92,    40,    94,    22,   -49,   -49,    91,   -49,    93,   -49,
     -49,    43,   -49
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     4,     3,     0,     0,     2,     5,     1,     8,     0,
       0,     0,     6,     0,    26,    27,    28,    29,    30,    31,
       9,    10,    11,    12,     0,     0,     0,    32,     0,     0,
      32,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    33,    34,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    32,     0,     0,    32,
       0,     0,    13,     0,    19,    20,    21,     0,     0,     0,
       0,    58,     0,     0,    60,     0,    50,     0,     0,     0,
       0,     0,    25,    35,     0,     0,    14,     0,    22,    23,
      32,     0,    17,     0,    47,    52,    53,    46,    51,    48,
      49,    54,    55,     0,     0,    24,     8,    15,     0,    19,
       0,     0,     0,     0,    16,    18,     0,    57,     0,     7,
      59,     0,    56
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -49,   -49,   -49,    -5,   -49,   -49,   -49,   -49,    -7,    12,
     -49,   -49,   -49,   -49,    95,   -30,    60,   -49,   -49,   -49,
     -49,   -49,   -49,   -49,   -49,   -49,   -49,   -49,    45,   -29,
      69
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,    10,    20,    21,    62,    63,    92,    64,
      65,    66,    22,    23,    24,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    72,    68,
      75
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      58,    27,    30,     6,    69,    70,     8,    32,    33,    34,
      35,    36,    12,    74,    37,    38,    39,    40,    76,     7,
      59,    41,    42,    11,    13,    60,    84,   119,    61,    87,
       9,    88,    14,    15,    16,    17,    18,    19,     1,    13,
       2,    60,    28,    31,    61,    25,    29,    14,    15,    16,
      17,    18,    19,     3,    14,    15,    16,    17,    18,    19,
     108,    56,    57,    71,    67,    73,    74,    78,    80,    79,
      81,    82,    85,    86,    91,    93,    94,    90,   103,    95,
      96,    97,    98,    99,   100,   104,   101,   102,   105,   106,
     107,   110,   112,   111,   114,   116,   117,   118,   120,   122,
     121,   113,   115,   109,     0,    83,    89,    77,    26
};

static const yytype_int8 yycheck[] =
{
      30,    10,    10,    51,    33,    34,    10,    31,    32,    33,
      34,    35,     5,    51,    38,    39,    40,    41,    56,     0,
      10,    45,    46,    12,    17,    31,    56,     5,    34,    59,
      34,    60,    25,    26,    27,    28,    29,    30,     1,    17,
       3,    31,    51,    51,    34,     3,    57,    25,    26,    27,
      28,    29,    30,    16,    25,    26,    27,    28,    29,    30,
      90,    10,     3,    54,    12,     3,    51,     3,    12,    53,
      12,     5,     7,     5,    56,     3,    56,    10,     3,    56,
      56,    56,    56,    56,    56,     3,    56,    56,     5,    10,
       5,    57,    57,     7,     5,     3,    56,     3,     7,    56,
       7,   106,   109,    91,    -1,    45,    61,    38,    13
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,     3,    16,    60,    61,    51,     0,    10,    34,
      62,    12,     5,    17,    25,    26,    27,    28,    29,    30,
      63,    64,    71,    72,    73,     3,    73,    10,    51,    57,
      10,    51,    31,    32,    33,    34,    35,    38,    39,    40,
      41,    45,    46,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    10,     3,    74,    10,
      31,    34,    65,    66,    68,    69,    70,    12,    88,    88,
      88,    54,    87,     3,    51,    89,    56,    89,     3,    53,
      12,    12,     5,    75,    74,     7,     5,    74,    88,    87,
      10,    56,    67,     3,    56,    56,    56,    56,    56,    56,
      56,    56,    56,     3,     3,     5,    10,     5,    74,    68,
      57,     7,    57,    62,     5,    67,     3,    56,     3,     5,
       7,     7,    56
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    60,    60,    61,    61,    61,    62,    62,
      62,    63,    63,    64,    64,    65,    65,    66,    67,    67,
      68,    68,    69,    70,    71,    72,    73,    73,    73,    73,
      73,    73,    74,    74,    75,    75,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    77,    78,    79,    80,
      80,    81,    82,    82,    83,    84,    85,    86,    87,    88,
      89
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     5,    11,     0,     2,
       2,     1,     1,     4,     5,     3,     4,     2,     3,     0,
       1,     1,     2,     2,     5,     4,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       2,     3,     3,     3,     3,     3,     7,     5,     1,     5,
       1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif
/* Parser data structure.  */
struct yypstate
  {
    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;
    /* Whether this instance has not started parsing yet.
     * If 2, it corresponds to a finished parsing.  */
    int yynew;
  };


/* Context of a parse error.  */
typedef struct
{
  yypstate* yyps;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypstate_expected_tokens (yypstate *yyps,
                          yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyps->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}


/* Similar to the previous function.  */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  return yypstate_expected_tokens (yyctx->yyps, yyarg, yyargn);
}


#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}





#define yynerrs yyps->yynerrs
#define yystate yyps->yystate
#define yyerrstatus yyps->yyerrstatus
#define yyssa yyps->yyssa
#define yyss yyps->yyss
#define yyssp yyps->yyssp
#define yyvsa yyps->yyvsa
#define yyvs yyps->yyvs
#define yyvsp yyps->yyvsp
#define yylsa yyps->yylsa
#define yyls yyps->yyls
#define yylsp yyps->yylsp
#define yystacksize yyps->yystacksize

/* Initialize the parser data structure.  */
static void
yypstate_clear (yypstate *yyps)
{
  yynerrs = 0;
  yystate = 0;
  yyerrstatus = 0;

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

  /* Initialize the state stack, in case yypcontext_expected_tokens is
     called before the first call to yyparse. */
  *yyssp = 0;
  yyps->yynew = 1;
}

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = YY_CAST (yypstate *, YYMALLOC (sizeof *yyps));
  if (!yyps)
    return YY_NULLPTR;
  yystacksize = YYINITDEPTH;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yypstate_clear (yyps);
  return yyps;
}

void
yypstate_delete (yypstate *yyps)
{
  if (yyps)
    {
#ifndef yyoverflow
      /* If the stack was reallocated but the parse did not complete, then the
         stack still needs to be freed.  */
      if (yyss != yyssa)
        YYSTACK_FREE (yyss);
#endif
      YYFREE (yyps);
    }
}



/*---------------.
| yypush_parse.  |
`---------------*/

int
yypush_parse (yypstate *yyps,
              int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  switch (yyps->yynew)
    {
    case 0:
      yyn = yypact[yystate];
      goto yyread_pushed_token;

    case 2:
      yypstate_clear (yyps);
      break;

    default:
      break;
    }

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = *yypushed_loc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      if (!yyps->yynew)
        {
          YYDPRINTF ((stderr, "Return for a new token:\n"));
          yyresult = YYPUSH_MORE;
          goto yypushreturn;
        }
      yyps->yynew = 0;
yyread_pushed_token:
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yypushed_char;
      if (yypushed_val)
        yylval = *yypushed_val;
      if (yypushed_loc)
        yylloc = *yypushed_loc;
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: scene_declaration  */
#line 194 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                { 
		(yyval.program) = SceneProgramSemanticAction((yyvsp[0].scene)); 
	}
#line 1732 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 3: /* program: INTEGER  */
#line 197 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                        { 
		(yyval.program) = NULL; 
	}
#line 1740 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 4: /* program: error  */
#line 200 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                { 
		yyerrok; 
		(yyval.program) = NULL; 
	}
#line 1749 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 5: /* scene_declaration: SCENE IDENTIFIER  */
#line 221 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                        { 
		(yyval.scene) = BasicSceneSemanticAction((yyvsp[0].string)); 
	}
#line 1757 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 6: /* scene_declaration: SCENE IDENTIFIER OPEN_BRACE scene_content CLOSE_BRACE  */
#line 224 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		(yyval.scene) = BasicSceneSemanticAction((yyvsp[-3].string));
		if ((yyvsp[-1].scene) != NULL) {
			(yyval.scene) = (yyvsp[-1].scene);  // Use the scene with figures
			if ((yyval.scene) != NULL && (yyval.scene)->name == NULL) {
				(yyval.scene)->name = malloc(strlen((yyvsp[-3].string)) + 1);
				if ((yyval.scene)->name != NULL) {
					strcpy((yyval.scene)->name, (yyvsp[-3].string));
				}
			}
		}
	}
#line 1774 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 7: /* scene_declaration: SCENE IDENTIFIER SIZE OPEN_PARENTHESIS INTEGER COMMA INTEGER CLOSE_PARENTHESIS OPEN_BRACE scene_content CLOSE_BRACE  */
#line 236 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                                                              {
		printf("DEBUG: scene with size parsed - %s size(%d, %d)\n", (yyvsp[-9].string), (yyvsp[-6].integer), (yyvsp[-4].integer));
		(yyval.scene) = BasicSceneSemanticAction((yyvsp[-9].string));
		if ((yyvsp[-1].scene) != NULL) {
			(yyval.scene) = (yyvsp[-1].scene);  // Use the scene with figures  
			if ((yyval.scene) != NULL && (yyval.scene)->name == NULL) {
				(yyval.scene)->name = malloc(strlen((yyvsp[-9].string)) + 1);
				if ((yyval.scene)->name != NULL) {
					strcpy((yyval.scene)->name, (yyvsp[-9].string));
				}
			}
		}
		// TODO: Agregar acción semántica para tamaño de escena
	}
#line 1793 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 8: /* scene_content: %empty  */
#line 252 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        { 
		printf("[DEBUG] scene_content: empty\n");
		(yyval.scene) = NULL; 
	}
#line 1802 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 9: /* scene_content: scene_content figure_declaration  */
#line 256 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                { 
		printf("[DEBUG] scene_content: adding figure_declaration\n");
		(yyval.scene) = AddFigureToSceneSemanticAction((yyvsp[-1].scene), (yyvsp[0].figure)); 
	}
#line 1811 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 10: /* scene_content: scene_content draw_statement  */
#line 260 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                { 
		printf("[DEBUG] scene_content: adding draw_statement\n");
		printf("[DEBUG] scene_content: scene=%p, figure=%p\n", (yyvsp[-1].scene), (yyvsp[0].figure));
		(yyval.scene) = AddFigureToSceneSemanticAction((yyvsp[-1].scene), (yyvsp[0].figure)); 
		printf("[DEBUG] scene_content: AddFigureToSceneSemanticAction completed\n");
	}
#line 1822 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 11: /* figure_declaration: figure_with_id  */
#line 268 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                { (yyval.figure) = (yyvsp[0].figure); }
#line 1828 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 12: /* figure_declaration: figure_anonymous  */
#line 269 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        { (yyval.figure) = (yyvsp[0].figure); }
#line 1834 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 13: /* draw_statement: DRAW figure_type IDENTIFIER draw_tail  */
#line 272 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        {
		printf("DEBUG: Draw statement with ID '%s'\n", (yyvsp[-1].string));
		printf("[DEBUG] draw_statement: About to call CreateFigureSemanticAction\n");
		printf("[DEBUG] draw_statement: type=%d, id='%s', tail=%p\n", (yyvsp[-2].integer), (yyvsp[-1].string), (yyvsp[0].property));
		(yyval.figure) = CreateFigureSemanticAction((yyvsp[-2].integer), (yyvsp[-1].string), (yyvsp[0].property));
		printf("[DEBUG] draw_statement: CreateFigureSemanticAction completed\n");
		printf("[DEBUG] draw_statement: Figure created successfully\n");
	}
#line 1847 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 14: /* draw_statement: DRAW figure_type OPEN_BRACE figure_properties CLOSE_BRACE  */
#line 280 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                        {
		printf("DEBUG: Draw statement anonymous figure\n");
		(yyval.figure) = CreateFigureSemanticAction((yyvsp[-3].integer), NULL, (yyvsp[-1].property));
	}
#line 1856 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 15: /* draw_tail: OPEN_BRACE figure_properties CLOSE_BRACE  */
#line 286 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        {
		printf("[DEBUG] draw_tail: internal properties only\n");
		(yyval.property) = (yyvsp[-1].property);
	}
#line 1865 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 16: /* draw_tail: external_items OPEN_BRACE figure_properties CLOSE_BRACE  */
#line 290 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                        {
		printf("[DEBUG] draw_tail: external + internal properties\n");
		(yyval.property) = (yyvsp[-1].property);
	}
#line 1874 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 17: /* external_items: external_item external_items_more  */
#line 296 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        {
		printf("[DEBUG] external_items: head + more\n");
		printf("[DBG] link head %p -> %p\n", (void*)(yyvsp[-1].property), (void*)(yyvsp[0].property));
		(yyvsp[-1].property)->next = (yyvsp[0].property);
		(yyval.property) = (yyvsp[-1].property);
	}
#line 1885 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 18: /* external_items_more: SEMICOLON external_item external_items_more  */
#line 304 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                        {
		printf("[DEBUG] external_items_more: semicolon + item + more\n");
		printf("[DBG] link mid %p -> %p\n", (void*)(yyvsp[-1].property), (void*)(yyvsp[0].property));
		(yyvsp[-1].property)->next = (yyvsp[0].property);
		(yyval.property) = (yyvsp[-1].property);
	}
#line 1896 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 19: /* external_items_more: %empty  */
#line 310 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                        {
		printf("[DEBUG] external_items_more: empty\n");
		(yyval.property) = NULL;
	}
#line 1905 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 20: /* external_item: position_item  */
#line 316 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        {
		printf("[DEBUG] external_item: position_item\n");
		(yyval.property) = (yyvsp[0].property);
	}
#line 1914 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 21: /* external_item: size_item  */
#line 320 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                {
		printf("[DEBUG] external_item: size_item\n");
		(yyval.property) = (yyvsp[0].property);
	}
#line 1923 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 22: /* position_item: AT coordinates_value  */
#line 326 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        {
		printf("[DEBUG] position_item: AT coordinates\n");
		Property* prop = CreatePropertySemanticAction(POSITION_PROPERTY);
		(yyval.property) = SetPropertyCoordinatesSemanticAction(prop, (yyvsp[0].coordinates).x, (yyvsp[0].coordinates).y);
	}
#line 1933 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 23: /* size_item: SIZE dimensions_value  */
#line 333 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                {
		printf("[DEBUG] size_item: SIZE dimensions\n");
		Property* prop = CreatePropertySemanticAction(SIZE_PROPERTY);
		(yyval.property) = SetPropertyDimensionsSemanticAction(prop, (yyvsp[0].dimensions).width, (yyvsp[0].dimensions).height);
	}
#line 1943 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 24: /* figure_with_id: figure_type IDENTIFIER OPEN_BRACE figure_properties CLOSE_BRACE  */
#line 340 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                {
		printf("DEBUG: Creating figure with ID '%s' of type %d\n", (yyvsp[-3].string), (yyvsp[-4].integer));
		(yyval.figure) = CreateFigureSemanticAction((yyvsp[-4].integer), (yyvsp[-3].string), (yyvsp[-1].property));
	}
#line 1952 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 25: /* figure_anonymous: figure_type OPEN_BRACE figure_properties CLOSE_BRACE  */
#line 346 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                        {
		printf("DEBUG: Creating anonymous figure of type %d\n", (yyvsp[-3].integer));
		(yyval.figure) = CreateFigureSemanticAction((yyvsp[-3].integer), NULL, (yyvsp[-1].property));
	}
#line 1961 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 26: /* figure_type: RECTANGLE  */
#line 352 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                        { (yyval.integer) = RECTANGLE_FIGURE; }
#line 1967 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 27: /* figure_type: CIRCLE  */
#line 353 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                { (yyval.integer) = CIRCLE_FIGURE; }
#line 1973 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 28: /* figure_type: LINE  */
#line 354 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                { (yyval.integer) = LINE_FIGURE; }
#line 1979 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 29: /* figure_type: ELLIPSE  */
#line 355 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                { (yyval.integer) = ELLIPSE_FIGURE; }
#line 1985 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 30: /* figure_type: POLYLINE  */
#line 356 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                { (yyval.integer) = POLYLINE_FIGURE; }
#line 1991 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 31: /* figure_type: POLYGON  */
#line 357 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                { (yyval.integer) = POLYGON_FIGURE; }
#line 1997 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 32: /* figure_properties: %empty  */
#line 360 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        { 
		printf("[DEBUG] figure_properties: empty\n");
		(yyval.property) = NULL; 
	}
#line 2006 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 33: /* figure_properties: property_list  */
#line 364 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		printf("[DEBUG] figure_properties: property_list=%p\n", (yyvsp[0].property));
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2015 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 34: /* property_list: property_item  */
#line 392 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        { 
		printf("[DEBUG] property_list: single property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2024 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 35: /* property_list: property_item property_list  */
#line 396 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        { 
		printf("[DEBUG] property_list: linking property to list\n");
		(yyvsp[-1].property)->next = (yyvsp[0].property); 
		(yyval.property) = (yyvsp[-1].property); 
	}
#line 2034 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 36: /* property_item: size_property  */
#line 403 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        { 
		printf("[DEBUG] property_item: size_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2043 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 37: /* property_item: position_property  */
#line 407 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		printf("[DEBUG] property_item: position_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2052 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 38: /* property_item: fill_property  */
#line 411 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		printf("[DEBUG] property_item: fill_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2061 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 39: /* property_item: stroke_property  */
#line 415 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		printf("[DEBUG] property_item: stroke_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2070 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 40: /* property_item: radius_property  */
#line 419 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		printf("[DEBUG] property_item: radius_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2079 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 41: /* property_item: from_to_property  */
#line 423 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		printf("[DEBUG] property_item: from_to_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2088 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 42: /* property_item: stroke_width_property  */
#line 427 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        { 
		printf("[DEBUG] property_item: stroke_width_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2097 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 43: /* property_item: opacity_property  */
#line 431 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		printf("[DEBUG] property_item: opacity_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2106 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 44: /* property_item: scale_property  */
#line 435 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		printf("[DEBUG] property_item: scale_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2115 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 45: /* property_item: rotate_property  */
#line 439 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                { 
		printf("[DEBUG] property_item: rotate_property\n");
		(yyval.property) = (yyvsp[0].property); 
	}
#line 2124 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 46: /* size_property: SIZE dimensions_value SEMICOLON  */
#line 445 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                               {
		printf("DEBUG: size_property with dimensions parsed\n");
		(yyval.property) = CreatePropertySemanticAction(SIZE_PROPERTY);
		(yyval.property) = SetPropertyDimensionsSemanticAction((yyval.property), (yyvsp[-1].dimensions).width, (yyvsp[-1].dimensions).height);
	}
#line 2134 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 47: /* position_property: AT coordinates_value SEMICOLON  */
#line 452 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                  {
		printf("DEBUG: position_property with coordinates parsed\n");
		(yyval.property) = CreatePropertySemanticAction(POSITION_PROPERTY);
		(yyval.property) = SetPropertyCoordinatesSemanticAction((yyval.property), (yyvsp[-1].coordinates).x, (yyvsp[-1].coordinates).y);
	}
#line 2144 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 48: /* fill_property: FILL color_value SEMICOLON  */
#line 459 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                          {
		printf("DEBUG: fill_property with color parsed\n");
		(yyval.property) = CreatePropertySemanticAction(FILL_PROPERTY);
		(yyval.property) = SetPropertyStringValueSemanticAction((yyval.property), (yyvsp[-1].string));
	}
#line 2154 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 49: /* stroke_property: STROKE color_value SEMICOLON  */
#line 466 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                {
		printf("DEBUG: stroke_property with color parsed\n");
		(yyval.property) = CreatePropertySemanticAction(STROKE_PROPERTY);
		(yyval.property) = SetPropertyStringValueSemanticAction((yyval.property), (yyvsp[-1].string));
	}
#line 2164 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 50: /* stroke_property: STROKE SEMICOLON  */
#line 471 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                {
		printf("DEBUG: stroke_property without color parsed\n");
		(yyval.property) = CreatePropertySemanticAction(STROKE_PROPERTY);
		(yyval.property) = SetPropertyStringValueSemanticAction((yyval.property), "black");
	}
#line 2174 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 51: /* radius_property: RADIUS INTEGER SEMICOLON  */
#line 478 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                {
		printf("DEBUG: radius_property parsed: radius(%d)\n", (yyvsp[-1].integer));
		(yyval.property) = CreatePropertySemanticAction(RADIUS_PROPERTY);
		(yyval.property) = SetPropertyIntValueSemanticAction((yyval.property), (yyvsp[-1].integer));
	}
#line 2184 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 52: /* from_to_property: FROM coordinates_value SEMICOLON  */
#line 485 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        {
		printf("DEBUG: from_property parsed\n");
		(yyval.property) = CreatePropertySemanticAction(FROM_PROPERTY);
		(yyval.property) = SetPropertyCoordinatesSemanticAction((yyval.property), (yyvsp[-1].coordinates).x, (yyvsp[-1].coordinates).y);
	}
#line 2194 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 53: /* from_to_property: TO coordinates_value SEMICOLON  */
#line 490 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                {
		printf("DEBUG: to_property parsed\n");
		(yyval.property) = CreatePropertySemanticAction(TO_PROPERTY);
		(yyval.property) = SetPropertyCoordinatesSemanticAction((yyval.property), (yyvsp[-1].coordinates).x, (yyvsp[-1].coordinates).y);
	}
#line 2204 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 54: /* stroke_width_property: STROKE_WIDTH INTEGER SEMICOLON  */
#line 497 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                        {
		printf("DEBUG: stroke_width_property parsed: stroke-width(%d)\n", (yyvsp[-1].integer));
		(yyval.property) = CreatePropertySemanticAction(STROKE_WIDTH_PROPERTY);
		(yyval.property) = SetPropertyIntValueSemanticAction((yyval.property), (yyvsp[-1].integer));
	}
#line 2214 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 55: /* opacity_property: OPACITY DECIMAL SEMICOLON  */
#line 504 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                {
		printf("DEBUG: opacity_property parsed: opacity(%f)\n", (yyvsp[-1].decimal));
		(yyval.property) = CreatePropertySemanticAction(OPACITY_PROPERTY);
		(yyval.property) = SetPropertyFloatValueSemanticAction((yyval.property), (yyvsp[-1].decimal));
	}
#line 2224 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 56: /* scale_property: SCALE OPEN_PARENTHESIS INTEGER COMMA INTEGER CLOSE_PARENTHESIS SEMICOLON  */
#line 511 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                                         {
		printf("DEBUG: scale_property parsed: scale(%d, %d)\n", (yyvsp[-4].integer), (yyvsp[-2].integer));
		(yyval.property) = CreatePropertySemanticAction(SCALE_PROPERTY);
		(yyval.property) = SetPropertyScaleSemanticAction((yyval.property), (float)(yyvsp[-4].integer), (float)(yyvsp[-2].integer));
	}
#line 2234 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 57: /* rotate_property: ROTATE OPEN_PARENTHESIS INTEGER CLOSE_PARENTHESIS SEMICOLON  */
#line 518 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                             {
		printf("DEBUG: rotate_property parsed: rotate(%d)\n", (yyvsp[-2].integer));
		(yyval.property) = CreatePropertySemanticAction(ROTATE_PROPERTY);
		(yyval.property) = SetPropertyFloatValueSemanticAction((yyval.property), (float)(yyvsp[-2].integer));
	}
#line 2244 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 58: /* dimensions_value: DIMENSIONS  */
#line 525 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                             {
		printf("DEBUG: dimensions_value parsed successfully\n");
		(yyval.dimensions).width = (yyvsp[0].dimensions).width;
		(yyval.dimensions).height = (yyvsp[0].dimensions).height;
	}
#line 2254 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 59: /* coordinates_value: OPEN_PARENTHESIS INTEGER COMMA INTEGER CLOSE_PARENTHESIS  */
#line 532 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                                                                            {
		printf("DEBUG: coordinates_value parsed successfully\n");
		(yyval.coordinates).x = (yyvsp[-3].integer);
		(yyval.coordinates).y = (yyvsp[-1].integer);
	}
#line 2264 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;

  case 60: /* color_value: IDENTIFIER  */
#line 539 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"
                        {
		printf("DEBUG: color_value parsed: %s\n", (yyvsp[0].string));
		(yyval.string) = (yyvsp[0].string);
	}
#line 2273 "src/main/c/frontend/syntactic-analysis/BisonParser.c"
    break;


#line 2277 "src/main/c/frontend/syntactic-analysis/BisonParser.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyps, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
  yyps->yynew = 2;
  goto yypushreturn;


/*-------------------------.
| yypushreturn -- return.  |
`-------------------------*/
yypushreturn:
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}
#undef yynerrs
#undef yystate
#undef yyerrstatus
#undef yyssa
#undef yyss
#undef yyssp
#undef yyvsa
#undef yyvs
#undef yyvsp
#undef yylsa
#undef yyls
#undef yylsp
#undef yystacksize
#line 545 "src/main/c/frontend/syntactic-analysis/BisonGrammar.y"

