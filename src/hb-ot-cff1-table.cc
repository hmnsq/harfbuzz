/*
 * Copyright © 2018 Adobe Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Adobe Author(s): Michiharu Ariza
 */

#ifndef HB_OT_CFF1_TABLE_CC
#define HB_OT_CFF1_TABLE_CC
#ifdef HB_OT_CFF1_TABLE_CC /* Pacify -Wunused-macros. */

#અન્તર્ભાવ "hb.hh"

#ifndef HB_NO_CFF

#અન્તર્ભાવ "hb-draw.hh"
#અન્તર્ભાવ "hb-algs.hh"
#અન્તર્ભાવ "hb-ot-cff1-table.hh"
#અન્તર્ભાવ "hb-cff1-interp-cs.hh"

using નામાકાશ CFF;

સંરચના sid_to_gid_t
{
  uint16_t  sid;
  uint8_t   gid;

  પૂર્ણાંક cmp (uint16_t a) અચલ
  {
    જો (a == sid) પાછા 0;
    પાછા (a < sid) ? -1 : 1;
  }
};

/* SID to code */
સ્થિર અચલ uint8_t standard_encoding_to_code [] =
{
    0,   32,   33,   34,   35,   36,   37,   38,  39,   40,   41,   42,   43,   44,   45,   46,
   47,   48,   49,   50,   51,   52,   53,   54,  55,   56,   57,   58,   59,   60,   61,   62,
   63,   64,   65,   66,   67,   68,   69,   70,  71,   72,   73,   74,   75,   76,   77,   78,
   79,   80,   81,   82,   83,   84,   85,   86,  87,   88,   89,   90,   91,   92,   93,   94,
   95,   96,   97,   98,   99,  100,  101,  102, 103,  104,  105,  106,  107,  108,  109,  110,
  111,  112,  113,  114,  115,  116,  117,  118, 119,  120,  121,  122,  123,  124,  125,  126,
  161,  162,  163,  164,  165,  166,  167,  168, 169,  170,  171,  172,  173,  174,  175,  177,
  178,  179,  180,  182,  183,  184,  185,  186, 187,  188,  189,  191,  193,  194,  195,  196,
  197,  198,  199,  200,  202,  203,  205,  206, 207,  208,  225,  227,  232,  233,  234,  235,
  241,  245,  248,  249,  250,  251
};

/* SID to code */
સ્થિર અચલ uint8_t expert_encoding_to_code [] =
{
    0,   32,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   44,   45,   46,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   58,   59,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   47,    0,    0,    0,    0,    0,    0,    0,    0,    0,   87,   88,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  201,    0,    0,    0,    0,  189,    0,    0,  188,    0,
    0,    0,    0,  190,  202,    0,    0,    0,    0,  203,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   33,   34,   36,   37,   38,   39,   40,   41,   42,   43,   48,
   49,   50,   51,   52,   53,   54,   55,   56,   57,   60,   61,   62,   63,   65,   66,   67,
   68,   69,   73,   76,   77,   78,   79,   82,   83,   84,   86,   89,   90,   91,   93,   94,
   95,   96,   97,   98,   99,  100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,
  111,  112,  113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
  161,  162,  163,  166,  167,  168,  169,  170,  172,  175,  178,  179,  182,  183,  184,  191,
  192,  193,  194,  195,  196,  197,  200,  204,  205,  206,  207,  208,  209,  210,  211,  212,
  213,  214,  215,  216,  217,  218,  219,  220,  221,  222,  223,  224,  225,  226,  227,  228,
  229,  230,  231,  232,  233,  234,  235,  236,  237,  238,  239,  240,  241,  242,  243,  244,
  245,  246,  247,  248,  249,  250,  251,  252,  253,  254,  255
};

/* glyph ID to SID */
સ્થિર અચલ uint16_t expert_charset_to_sid [] =
{
    0,    1,  229,  230,  231,  232,  233,  234,  235,  236,  237,  238,   13,   14,   15,   99,
  239,  240,  241,  242,  243,  244,  245,  246,  247,  248,   27,   28,  249,  250,  251,  252,
  253,  254,  255,  256,  257,  258,  259,  260,  261,  262,  263,  264,  265,  266,  109,  110,
  267,  268,  269,  270,  271,  272,  273,  274,  275,  276,  277,  278,  279,  280,  281,  282,
  283,  284,  285,  286,  287,  288,  289,  290,  291,  292,  293,  294,  295,  296,  297,  298,
  299,  300,  301,  302,  303,  304,  305,  306,  307,  308,  309,  310,  311,  312,  313,  314,
  315,  316,  317,  318,  158,  155,  163,  319,  320,  321,  322,  323,  324,  325,  326,  150,
  164,  169,  327,  328,  329,  330,  331,  332,  333,  334,  335,  336,  337,  338,  339,  340,
  341,  342,  343,  344,  345,  346,  347,  348,  349,  350,  351,  352,  353,  354,  355,  356,
  357,  358,  359,  360,  361,  362,  363,  364,  365,  366,  367,  368,  369,  370,  371,  372,
  373,  374,  375,  376,  377,  378
};

/* glyph ID to SID */
સ્થિર અચલ uint16_t expert_subset_charset_to_sid [] =
{
    0,    1,  231,  232,  235,  236,  237,  238,   13,   14,   15,   99,  239,  240,  241,  242,
  243,  244,  245,  246,  247,  248,   27,   28,  249,  250,  251,  253,  254,  255,  256,  257,
  258,  259,  260,  261,  262,  263,  264,  265,  266,  109,  110,  267,  268,  269,  270,  272,
  300,  301,  302,  305,  314,  315,  158,  155,  163,  320,  321,  322,  323,  324,  325,  326,
  150,  164,  169,  327,  328,  329,  330,  331,  332,  333,  334,  335,  336,  337,  338,  339,
  340,  341,  342,  343,  344,  345,  346
};

/* SID to glyph ID */
સ્થિર અચલ sid_to_gid_t expert_charset_sid_to_gid [] =
{
    { 1, 1 },     { 13, 12 },   { 14, 13 },   { 15, 14 },
    { 27, 26 },   { 28, 27 },   { 99, 15 },   { 109, 46 },
    { 110, 47 },  { 150, 111 }, { 155, 101 }, { 158, 100 },
    { 163, 102 }, { 164, 112 }, { 169, 113 }, { 229, 2 },
    { 230, 3 },   { 231, 4 },   { 232, 5 },   { 233, 6 },
    { 234, 7 },   { 235, 8 },   { 236, 9 },   { 237, 10 },
    { 238, 11 },  { 239, 16 },  { 240, 17 },  { 241, 18 },
    { 242, 19 },  { 243, 20 },  { 244, 21 },  { 245, 22 },
    { 246, 23 },  { 247, 24 },  { 248, 25 },  { 249, 28 },
    { 250, 29 },  { 251, 30 },  { 252, 31 },  { 253, 32 },
    { 254, 33 },  { 255, 34 },  { 256, 35 },  { 257, 36 },
    { 258, 37 },  { 259, 38 },  { 260, 39 },  { 261, 40 },
    { 262, 41 },  { 263, 42 },  { 264, 43 },  { 265, 44 },
    { 266, 45 },  { 267, 48 },  { 268, 49 },  { 269, 50 },
    { 270, 51 },  { 271, 52 },  { 272, 53 },  { 273, 54 },
    { 274, 55 },  { 275, 56 },  { 276, 57 },  { 277, 58 },
    { 278, 59 },  { 279, 60 },  { 280, 61 },  { 281, 62 },
    { 282, 63 },  { 283, 64 },  { 284, 65 },  { 285, 66 },
    { 286, 67 },  { 287, 68 },  { 288, 69 },  { 289, 70 },
    { 290, 71 },  { 291, 72 },  { 292, 73 },  { 293, 74 },
    { 294, 75 },  { 295, 76 },  { 296, 77 },  { 297, 78 },
    { 298, 79 },  { 299, 80 },  { 300, 81 },  { 301, 82 },
    { 302, 83 },  { 303, 84 },  { 304, 85 },  { 305, 86 },
    { 306, 87 },  { 307, 88 },  { 308, 89 },  { 309, 90 },
    { 310, 91 },  { 311, 92 },  { 312, 93 },  { 313, 94 },
    { 314, 95 },  { 315, 96 },  { 316, 97 },  { 317, 98 },
    { 318, 99 },  { 319, 103 }, { 320, 104 }, { 321, 105 },
    { 322, 106 }, { 323, 107 }, { 324, 108 }, { 325, 109 },
    { 326, 110 }, { 327, 114 }, { 328, 115 }, { 329, 116 },
    { 330, 117 }, { 331, 118 }, { 332, 119 }, { 333, 120 },
    { 334, 121 }, { 335, 122 }, { 336, 123 }, { 337, 124 },
    { 338, 125 }, { 339, 126 }, { 340, 127 }, { 341, 128 },
    { 342, 129 }, { 343, 130 }, { 344, 131 }, { 345, 132 },
    { 346, 133 }, { 347, 134 }, { 348, 135 }, { 349, 136 },
    { 350, 137 }, { 351, 138 }, { 352, 139 }, { 353, 140 },
    { 354, 141 }, { 355, 142 }, { 356, 143 }, { 357, 144 },
    { 358, 145 }, { 359, 146 }, { 360, 147 }, { 361, 148 },
    { 362, 149 }, { 363, 150 }, { 364, 151 }, { 365, 152 },
    { 366, 153 }, { 367, 154 }, { 368, 155 }, { 369, 156 },
    { 370, 157 }, { 371, 158 }, { 372, 159 }, { 373, 160 },
    { 374, 161 }, { 375, 162 }, { 376, 163 }, { 377, 164 },
    { 378, 165 }
};

/* SID to glyph ID */
સ્થિર અચલ sid_to_gid_t expert_subset_charset_sid_to_gid [] =
{
  { 1, 1 },       { 13, 8 },      { 14, 9 },      { 15, 10 },
  { 27, 22 },     { 28, 23 },     { 99, 11 },     { 109, 41 },
  { 110, 42 },    { 150, 64 },    { 155, 55 },    { 158, 54 },
  { 163, 56 },    { 164, 65 },    { 169, 66 },    { 231, 2 },
  { 232, 3 },     { 235, 4 },     { 236, 5 },     { 237, 6 },
  { 238, 7 },     { 239, 12 },    { 240, 13 },    { 241, 14 },
  { 242, 15 },    { 243, 16 },    { 244, 17 },    { 245, 18 },
  { 246, 19 },    { 247, 20 },    { 248, 21 },    { 249, 24 },
  { 250, 25 },    { 251, 26 },    { 253, 27 },    { 254, 28 },
  { 255, 29 },    { 256, 30 },    { 257, 31 },    { 258, 32 },
  { 259, 33 },    { 260, 34 },    { 261, 35 },    { 262, 36 },
  { 263, 37 },    { 264, 38 },    { 265, 39 },    { 266, 40 },
  { 267, 43 },    { 268, 44 },    { 269, 45 },    { 270, 46 },
  { 272, 47 },    { 300, 48 },    { 301, 49 },    { 302, 50 },
  { 305, 51 },    { 314, 52 },    { 315, 53 },    { 320, 57 },
  { 321, 58 },    { 322, 59 },    { 323, 60 },    { 324, 61 },
  { 325, 62 },    { 326, 63 },    { 327, 67 },    { 328, 68 },
  { 329, 69 },    { 330, 70 },    { 331, 71 },    { 332, 72 },
  { 333, 73 },    { 334, 74 },    { 335, 75 },    { 336, 76 },
  { 337, 77 },    { 338, 78 },    { 339, 79 },    { 340, 80 },
  { 341, 81 },    { 342, 82 },    { 343, 83 },    { 344, 84 },
  { 345, 85 },    { 346, 86 }
};

/* code to SID */
સ્થિર અચલ uint8_t standard_encoding_to_sid [] =
{
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
    17,  18,   19,   20,   21,   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
    33,  34,   35,   36,   37,   38,   39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
    49,  50,   51,   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,   62,   63,   64,
    65,  66,   67,   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,   78,   79,   80,
    81,  82,   83,   84,   85,   86,   87,   88,   89,   90,   91,   92,   93,   94,   95,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   96,   97,   98,   99,  100,  101,  102,  103,  104,  105,  106,  107,  108,  109,  110,
    0,  111,  112,  113,  114,    0,  115,  116,  117,  118,  119,  120,  121,  122,    0,  123,
    0,  124,  125,  126,  127,  128,  129,  130,  131,    0,  132,  133,    0,  134,  135,  136,
  137,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   138,   0,  139,    0,    0,    0,    0,  140,  141,  142,  143,    0,    0,    0,    0,
    0,   144,   0,    0,    0,  145,    0,    0,  146,  147,  148,  149,    0,    0,    0,    0
};

hb_codepoint_t OT::cff1::lookup_standard_encoding_for_code (hb_codepoint_t sid)
{
  જો (sid < ARRAY_LENGTH (standard_encoding_to_code))
    પાછા (hb_codepoint_t)standard_encoding_to_code[sid];
  નહીંતર
    પાછા 0;
}

hb_codepoint_t OT::cff1::lookup_expert_encoding_for_code (hb_codepoint_t sid)
{
  જો (sid < ARRAY_LENGTH (expert_encoding_to_code))
    પાછા (hb_codepoint_t)expert_encoding_to_code[sid];
  નહીંતર
    પાછા 0;
}

hb_codepoint_t OT::cff1::lookup_expert_charset_for_sid (hb_codepoint_t glyph)
{
  જો (glyph < ARRAY_LENGTH (expert_charset_to_sid))
    પાછા (hb_codepoint_t)expert_charset_to_sid[glyph];
  નહીંતર
    પાછા 0;
}

hb_codepoint_t OT::cff1::lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph)
{
  જો (glyph < ARRAY_LENGTH (expert_subset_charset_to_sid))
    પાછા (hb_codepoint_t)expert_subset_charset_to_sid[glyph];
  નહીંતર
    પાછા 0;
}

hb_codepoint_t OT::cff1::lookup_expert_charset_for_glyph (hb_codepoint_t sid)
{
  અચલ auto *pair = hb_sorted_array (expert_charset_sid_to_gid).bsearch (sid);
  પાછા pair ? pair->gid : 0;
}

hb_codepoint_t OT::cff1::lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid)
{
  અચલ auto *pair = hb_sorted_array (expert_subset_charset_sid_to_gid).bsearch (sid);
  પાછા pair ? pair->gid : 0;
}

hb_codepoint_t OT::cff1::lookup_standard_encoding_for_sid (hb_codepoint_t code)
{
  જો (code < ARRAY_LENGTH (standard_encoding_to_sid))
    પાછા (hb_codepoint_t)standard_encoding_to_sid[code];
  નહીંતર
    પાછા CFF_UNDEF_SID;
}

સંરચના bounds_t
{
  શૂન્ય init ()
  {
    min.set_int (INT_MAX, INT_MAX);
    max.set_int (INT_MIN, INT_MIN);
  }

  શૂન્ય update (અચલ point_t &pt)
  {
    જો (pt.x < min.x) min.x = pt.x;
    જો (pt.x > max.x) max.x = pt.x;
    જો (pt.y < min.y) min.y = pt.y;
    જો (pt.y > max.y) max.y = pt.y;
  }

  શૂન્ય merge (અચલ bounds_t &b)
  {
    જો (empty ())
      *આ = b;
    નહીંતર જો (!b.empty ())
    {
      જો (b.min.x < min.x) min.x = b.min.x;
      જો (b.max.x > max.x) max.x = b.max.x;
      જો (b.min.y < min.y) min.y = b.min.y;
      જો (b.max.y > max.y) max.y = b.max.y;
    }
  }

  શૂન્ય offset (અચલ point_t &delta)
  {
    જો (!empty ())
    {
      min.move (delta);
      max.move (delta);
    }
  }

  તાર્કિક empty () અચલ { પાછા (min.x >= max.x) || (min.y >= max.y); }

  point_t min;
  point_t max;
};

સંરચના cff1_extents_param_t
{
  cff1_extents_param_t (અચલ OT::cff1::accelerator_t *_cff) : cff (_cff)
  {
    bounds.init ();
  }

  શૂન્ય start_path   ()       { path_open = સાચું; }
  શૂન્ય end_path     ()       { path_open = ખોટું; }
  તાર્કિક is_path_open () અચલ { પાછા path_open; }

  તાર્કિક path_open = ખોટું;
  bounds_t bounds;

  અચલ OT::cff1::accelerator_t *cff;
};

સંરચના cff1_path_procs_extents_t : path_procs_t<cff1_path_procs_extents_t, cff1_cs_interp_env_t, cff1_extents_param_t>
{
  સ્થિર શૂન્ય moveto (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, અચલ point_t &pt)
  {
    param.end_path ();
    env.moveto (pt);
  }

  સ્થિર શૂન્ય લીટી (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, અચલ point_t &pt1)
  {
    જો (!param.is_path_open ())
    {
      param.start_path ();
      param.bounds.update (env.get_pt ());
    }
    env.moveto (pt1);
    param.bounds.update (env.get_pt ());
  }

  સ્થિર શૂન્ય curve (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, અચલ point_t &pt1, અચલ point_t &pt2, અચલ point_t &pt3)
  {
    જો (!param.is_path_open ())
    {
      param.start_path ();
      param.bounds.update (env.get_pt ());
    }
    /* include control points */
    param.bounds.update (pt1);
    param.bounds.update (pt2);
    env.moveto (pt3);
    param.bounds.update (env.get_pt ());
  }
};

સ્થિર તાર્કિક _get_bounds (અચલ OT::cff1::accelerator_t *cff, hb_codepoint_t glyph, bounds_t &bounds, તાર્કિક in_seac=ખોટું, int64_t *budget = nullptr);

સંરચના cff1_cs_opset_extents_t : cff1_cs_opset_t<cff1_cs_opset_extents_t, cff1_extents_param_t, cff1_path_procs_extents_t>
{
  સ્થિર શૂન્ય process_seac (cff1_cs_interp_env_t &env, cff1_extents_param_t& param)
  {
    unsigned પૂર્ણાંક  n = env.arg_ઢગલો.get_ગણતરી ();
    point_t delta;
    delta.x = env.arg_ઢગલો[n-4];
    delta.y = env.arg_ઢગલો[n-3];
    hb_codepoint_t base = param.cff->std_code_to_glyph (env.arg_ઢગલો[n-2].to_int ());
    hb_codepoint_t accent = param.cff->std_code_to_glyph (env.arg_ઢગલો[n-1].to_int ());

    bounds_t  base_bounds, accent_bounds;
    જો (likely (!env.in_seac && base && accent
	       && _get_bounds (param.cff, base, base_bounds, સાચું, env.get_budget ())
	       && _get_bounds (param.cff, accent, accent_bounds, સાચું, env.get_budget ())))
    {
      param.bounds.merge (base_bounds);
      accent_bounds.offset (delta);
      param.bounds.merge (accent_bounds);
    }
    નહીંતર
      env.set_error ();
  }
};

તાર્કિક _get_bounds (અચલ OT::cff1::accelerator_t *cff, hb_codepoint_t glyph, bounds_t &bounds, તાર્કિક in_seac, int64_t *budget)
{
  bounds.init ();
  જો (unlikely (!cff->is_valid () || (glyph >= cff->અંક_glyphs))) પાછા ખોટું;

  unsigned પૂર્ણાંક fd = cff->fdSelect->get_fd (glyph);
  અચલ hb_ubytes_t str = (*cff->charStrings)[glyph];
  cff1_cs_interp_env_t env (str, *cff, fd);
  env.set_in_seac (in_seac);
  cff1_cs_interpreter_t<cff1_cs_opset_extents_t, cff1_extents_param_t> interp (env);
  cff1_extents_param_t param (cff);
  જો (unlikely (!interp.interpret (param, budget))) પાછા ખોટું;
  bounds = param.bounds;
  પાછા સાચું;
}

તાર્કિક OT::cff1::accelerator_t::get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget) અચલ
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  પાછા સાચું;
#endif

  bounds_t bounds;

  int64_t ઢગલો_budget = HB_BUDGET_GLYPH;
  જો (!budget) budget = &ઢગલો_budget;

  જો (!_get_bounds (આ, glyph, bounds, ખોટું, budget))
    પાછા ખોટું;

  જો (bounds.min.x >= bounds.max.x)
  {
    extents->width = 0;
    extents->x_bearing = 0;
  }
  નહીંતર
  {
    દ્વિગુણ x_bearing = roundf (bounds.min.x.to_real ());
    extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
    extents->width = hb_clamp_to<hb_position_t> ((દ્વિગુણ) roundf (bounds.max.x.to_real ()) - x_bearing);
  }
  જો (bounds.min.y >= bounds.max.y)
  {
    extents->height = 0;
    extents->y_bearing = 0;
  }
  નહીંતર
  {
    દ્વિગુણ y_bearing = roundf (bounds.max.y.to_real ());
    extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
    extents->height = hb_clamp_to<hb_position_t> ((દ્વિગુણ) roundf (bounds.min.y.to_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  પાછા સાચું;
}

સંરચના cff1_path_param_t
{
  cff1_path_param_t (અચલ OT::cff1::accelerator_t *cff_, hb_font_t *font_,
		     hb_draw_session_t &draw_session_, point_t *delta_)
  {
    draw_session = &draw_session_;
    cff = cff_;
    font = font_;
    delta = delta_;
  }

  શૂન્ય move_to (અચલ point_t &p)
  {
    point_t point = p;
    જો (delta) point.move (*delta);
    draw_session->move_to (font->em_fscalef_x (point.x.to_real ()), font->em_fscalef_y (point.y.to_real ()));
  }

  શૂન્ય લીટી_to (અચલ point_t &p)
  {
    point_t point = p;
    જો (delta) point.move (*delta);
    draw_session->લીટી_to (font->em_fscalef_x (point.x.to_real ()), font->em_fscalef_y (point.y.to_real ()));
  }

  શૂન્ય cubic_to (અચલ point_t &p1, અચલ point_t &p2, અચલ point_t &p3)
  {
    point_t point1 = p1, point2 = p2, point3 = p3;
    જો (delta)
    {
      point1.move (*delta);
      point2.move (*delta);
      point3.move (*delta);
    }
    draw_session->cubic_to (font->em_fscalef_x (point1.x.to_real ()), font->em_fscalef_y (point1.y.to_real ()),
			   font->em_fscalef_x (point2.x.to_real ()), font->em_fscalef_y (point2.y.to_real ()),
			   font->em_fscalef_x (point3.x.to_real ()), font->em_fscalef_y (point3.y.to_real ()));
  }

  શૂન્ય end_path () { draw_session->close_path (); }

  hb_font_t *font;
  hb_draw_session_t *draw_session;
  point_t *delta;

  અચલ OT::cff1::accelerator_t *cff;
};

સંરચના cff1_path_procs_path_t : path_procs_t<cff1_path_procs_path_t, cff1_cs_interp_env_t, cff1_path_param_t>
{
  સ્થિર શૂન્ય moveto (cff1_cs_interp_env_t &env, cff1_path_param_t& param, અચલ point_t &pt)
  {
    param.move_to (pt);
    env.moveto (pt);
  }

  સ્થિર શૂન્ય લીટી (cff1_cs_interp_env_t &env, cff1_path_param_t &param, અચલ point_t &pt1)
  {
    param.લીટી_to (pt1);
    env.moveto (pt1);
  }

  સ્થિર શૂન્ય curve (cff1_cs_interp_env_t &env, cff1_path_param_t &param, અચલ point_t &pt1, અચલ point_t &pt2, અચલ point_t &pt3)
  {
    param.cubic_to (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

સ્થિર તાર્કિક _get_path (અચલ OT::cff1::accelerator_t *cff, hb_font_t *font, hb_codepoint_t glyph,
		       hb_draw_session_t &draw_session, તાર્કિક in_seac = ખોટું, point_t *delta = nullptr,
		       int64_t *budget = nullptr);

સંરચના cff1_cs_opset_path_t : cff1_cs_opset_t<cff1_cs_opset_path_t, cff1_path_param_t, cff1_path_procs_path_t>
{
  સ્થિર શૂન્ય process_seac (cff1_cs_interp_env_t &env, cff1_path_param_t& param)
  {
    /* End previous path */
    param.end_path ();

    unsigned પૂર્ણાંક n = env.arg_ઢગલો.get_ગણતરી ();
    point_t delta;
    delta.x = env.arg_ઢગલો[n-4];
    delta.y = env.arg_ઢગલો[n-3];
    hb_codepoint_t base = param.cff->std_code_to_glyph (env.arg_ઢગલો[n-2].to_int ());
    hb_codepoint_t accent = param.cff->std_code_to_glyph (env.arg_ઢગલો[n-1].to_int ());

    જો (unlikely (!(!env.in_seac && base && accent
		    && _get_path (param.cff, param.font, base, *param.draw_session, સાચું, nullptr, env.get_budget ())
		    && _get_path (param.cff, param.font, accent, *param.draw_session, સાચું, &delta, env.get_budget ()))))
      env.set_error ();
  }
};

તાર્કિક _get_path (અચલ OT::cff1::accelerator_t *cff, hb_font_t *font, hb_codepoint_t glyph,
		hb_draw_session_t &draw_session, તાર્કિક in_seac, point_t *delta,
		int64_t *budget)
{
  જો (unlikely (!cff->is_valid () || (glyph >= cff->અંક_glyphs))) પાછા ખોટું;

  unsigned પૂર્ણાંક fd = cff->fdSelect->get_fd (glyph);
  અચલ hb_ubytes_t str = (*cff->charStrings)[glyph];
  cff1_cs_interp_env_t env (str, *cff, fd);
  env.set_in_seac (in_seac);
  cff1_cs_interpreter_t<cff1_cs_opset_path_t, cff1_path_param_t> interp (env);
  cff1_path_param_t param (cff, font, draw_session, delta);
  જો (unlikely (!interp.interpret (param, budget))) પાછા ખોટું;

  /* Let's end the path specially since it is called inside seac also */
  param.end_path ();

  પાછા સાચું;
}

તાર્કિક OT::cff1::accelerator_t::get_path (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget) અચલ
{
#ifdef HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  પાછા સાચું;
#endif

  int64_t ઢગલો_budget = HB_BUDGET_GLYPH;
  જો (!budget) budget = &ઢગલો_budget;

  પાછા _get_path (આ, font, glyph, draw_session, ખોટું, nullptr, budget);
}

સંરચના get_seac_param_t
{
  get_seac_param_t (અચલ OT::cff1::accelerator_subset_t *_cff) : cff (_cff) {}

  તાર્કિક has_seac () અચલ { પાછા base && accent; }

  અચલ OT::cff1::accelerator_subset_t *cff;
  hb_codepoint_t  base = 0;
  hb_codepoint_t  accent = 0;
};

સંરચના cff1_cs_opset_seac_t : cff1_cs_opset_t<cff1_cs_opset_seac_t, get_seac_param_t>
{
  સ્થિર શૂન્ય process_seac (cff1_cs_interp_env_t &env, get_seac_param_t& param)
  {
    unsigned પૂર્ણાંક  n = env.arg_ઢગલો.get_ગણતરી ();
    hb_codepoint_t  base_char = (hb_codepoint_t)env.arg_ઢગલો[n-2].to_int ();
    hb_codepoint_t  accent_char = (hb_codepoint_t)env.arg_ઢગલો[n-1].to_int ();

    param.base = param.cff->std_code_to_glyph (base_char);
    param.accent = param.cff->std_code_to_glyph (accent_char);
  }
};

સ્થિર તાર્કિક
_cff1_charstring_may_end_in_seac (અચલ hb_ubytes_t &str)
{
  જો (unlikely (!str.length || str[str.length - 1] != OpCode_endchar))
    પાછા ખોટું;

  unsigned પૂર્ણાંક end = str.length - 1;
  જો (!end) પાછા ખોટું;

  unsigned પૂર્ણાંક last = str[end - 1];

  /* Type 2 numbers are not self-synchronizing when read backwards.
   * Keep interpreting if any number encoding could end at endchar. */
  જો (OpCode_OneByteIntFirst <= last && last <= OpCode_OneByteIntLast)
    પાછા સાચું;
  જો (end >= 2 &&
      OpCode_TwoBytePosInt0 <= str[end - 2] &&
      str[end - 2] <= OpCode_TwoByteNegInt3)
    પાછા સાચું;
  જો (end >= 3 && str[end - 3] == OpCode_shortint)
    પાછા સાચું;
  જો (end >= 5 && str[end - 5] == OpCode_fixedcs)
    પાછા સાચું;

  /* A subroutine can leave the seac operands on the shared stack. */
  પાછા last == OpCode_callsubr || last == OpCode_callgsubr;
}

તાર્કિક OT::cff1::accelerator_subset_t::get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *base, hb_codepoint_t *accent) અચલ
{
  જો (unlikely (!is_valid () || (glyph >= અંક_glyphs))) પાછા ખોટું;

  unsigned પૂર્ણાંક fd = fdSelect->get_fd (glyph);
  અચલ hb_ubytes_t str = (*charStrings)[glyph];
  જો (unlikely (!_cff1_charstring_may_end_in_seac (str))) પાછા ખોટું;
  cff1_cs_interp_env_t env (str, *આ, fd);
  cff1_cs_interpreter_t<cff1_cs_opset_seac_t, get_seac_param_t> interp (env);
  get_seac_param_t  param (આ);
  જો (unlikely (!interp.interpret (param))) પાછા ખોટું;

  જો (param.has_seac ())
  {
    *base = param.base;
    *accent = param.accent;
    પાછા સાચું;
  }
  પાછા ખોટું;
}


#endif

#endif /* HB_OT_CFF1_TABLE_CC pacify */
#endif /* HB_OT_CFF1_TABLE_CC guard */
