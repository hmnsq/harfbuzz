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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_CFF1_جدول_CC
#وصف_ڪريو HB_OT_CFF1_جدول_CC
#جيڪڏهن_وصف_ٿيل HB_OT_CFF1_جدول_CC /* Pacify -Wunused-macros. */

#ضمن "hb.hh"

#جيڪڏهن_وصف_نه_ٿيل HB_NO_CFF

#ضمن "hb-draw.hh"
#ضمن "hb-algs.hh"
#ضمن "hb-ot-cff1-table.hh"
#ضمن "hb-cff1-interp-cs.hh"

استعمال نطاق CFF;

بنية sid_منزل_gid_t
{
  uint16_t  sid;
  uint8_t   gid;

  صحيح cmp (uint16_t a) ثابت
  {
    إذا (a == sid) أرجع 0;
    أرجع (a < sid) ? -1 : 1;
  }
};

/* SID to code */
ساكن ثابت uint8_t standard_encoding_منزل_ڪوڊ [] =
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
ساكن ثابت uint8_t expert_encoding_منزل_ڪوڊ [] =
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
ساكن ثابت uint16_t expert_charset_منزل_sid [] =
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
ساكن ثابت uint16_t expert_subset_charset_منزل_sid [] =
{
    0,    1,  231,  232,  235,  236,  237,  238,   13,   14,   15,   99,  239,  240,  241,  242,
  243,  244,  245,  246,  247,  248,   27,   28,  249,  250,  251,  253,  254,  255,  256,  257,
  258,  259,  260,  261,  262,  263,  264,  265,  266,  109,  110,  267,  268,  269,  270,  272,
  300,  301,  302,  305,  314,  315,  158,  155,  163,  320,  321,  322,  323,  324,  325,  326,
  150,  164,  169,  327,  328,  329,  330,  331,  332,  333,  334,  335,  336,  337,  338,  339,
  340,  341,  342,  343,  344,  345,  346
};

/* SID to glyph ID */
ساكن ثابت sid_منزل_gid_t expert_charset_sid_منزل_gid [] =
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
ساكن ثابت sid_منزل_gid_t expert_subset_charset_sid_منزل_gid [] =
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
ساكن ثابت uint8_t standard_encoding_منزل_sid [] =
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

hb_codepoint_t OT::cff1::lookup_standard_encoding_for_ڪوڊ (hb_codepoint_t sid)
{
  إذا (sid < ARRAY_ڊيگهائي (standard_encoding_منزل_ڪوڊ))
    أرجع (hb_codepoint_t)standard_encoding_منزل_ڪوڊ[sid];
  وإلا
    أرجع 0;
}

hb_codepoint_t OT::cff1::lookup_expert_encoding_for_ڪوڊ (hb_codepoint_t sid)
{
  إذا (sid < ARRAY_ڊيگهائي (expert_encoding_منزل_ڪوڊ))
    أرجع (hb_codepoint_t)expert_encoding_منزل_ڪوڊ[sid];
  وإلا
    أرجع 0;
}

hb_codepoint_t OT::cff1::lookup_expert_charset_for_sid (hb_codepoint_t glyph)
{
  إذا (glyph < ARRAY_ڊيگهائي (expert_charset_منزل_sid))
    أرجع (hb_codepoint_t)expert_charset_منزل_sid[glyph];
  وإلا
    أرجع 0;
}

hb_codepoint_t OT::cff1::lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph)
{
  إذا (glyph < ARRAY_ڊيگهائي (expert_subset_charset_منزل_sid))
    أرجع (hb_codepoint_t)expert_subset_charset_منزل_sid[glyph];
  وإلا
    أرجع 0;
}

hb_codepoint_t OT::cff1::lookup_expert_charset_for_glyph (hb_codepoint_t sid)
{
  ثابت خودڪار *pair = hb_sorted_array (expert_charset_sid_منزل_gid).bsearch (sid);
  أرجع pair ? pair->gid : 0;
}

hb_codepoint_t OT::cff1::lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid)
{
  ثابت خودڪار *pair = hb_sorted_array (expert_subset_charset_sid_منزل_gid).bsearch (sid);
  أرجع pair ? pair->gid : 0;
}

hb_codepoint_t OT::cff1::lookup_standard_encoding_for_sid (hb_codepoint_t ڪوڊ)
{
  إذا (ڪوڊ < ARRAY_ڊيگهائي (standard_encoding_منزل_sid))
    أرجع (hb_codepoint_t)standard_encoding_منزل_sid[ڪوڊ];
  وإلا
    أرجع CFF_UNDEF_SID;
}

بنية bounds_t
{
  فراغ init ()
  {
    min.سيٽ_int (وڌ_کان_وڌ_عدد, وڌ_کان_وڌ_عدد);
    max.سيٽ_int (INT_MIN, INT_MIN);
  }

  فراغ update (ثابت نقطو_t &pt)
  {
    إذا (pt.x < min.x) min.x = pt.x;
    إذا (pt.x > max.x) max.x = pt.x;
    إذا (pt.y < min.y) min.y = pt.y;
    إذا (pt.y > max.y) max.y = pt.y;
  }

  فراغ ملايو (ثابت bounds_t &b)
  {
    إذا (خالي_جايون_2 ())
      *هذا = b;
    وإلا إذا (!b.خالي_جايون_2 ())
    {
      إذا (b.min.x < min.x) min.x = b.min.x;
      إذا (b.max.x > max.x) max.x = b.max.x;
      إذا (b.min.y < min.y) min.y = b.min.y;
      إذا (b.max.y > max.y) max.y = b.max.y;
    }
  }

  فراغ وٿي (ثابت نقطو_t &delta)
  {
    إذا (!خالي_جايون_2 ())
    {
      min.move (delta);
      max.move (delta);
    }
  }

  منطقي خالي_جايون_2 () ثابت { أرجع (min.x >= max.x) || (min.y >= max.y); }

  نقطو_t min;
  نقطو_t max;
};

بنية cff1_extents_param_t
{
  cff1_extents_param_t (ثابت OT::cff1::accelerator_t *_cff) : cff (_cff)
  {
    bounds.init ();
  }

  فراغ شروعات_رستو   ()       { رستو_open = سچ; }
  فراغ پڇاڙي_رستو     ()       { رستو_open = خطأ; }
  منطقي is_رستو_open () ثابت { أرجع رستو_open; }

  منطقي رستو_open = خطأ;
  bounds_t bounds;

  ثابت OT::cff1::accelerator_t *cff;
};

بنية cff1_رستو_procs_extents_t : رستو_procs_t<cff1_رستو_procs_extents_t, cff1_cs_interp_env_t, cff1_extents_param_t>
{
  ساكن فراغ moveto (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, ثابت نقطو_t &pt)
  {
    param.پڇاڙي_رستو ();
    env.moveto (pt);
  }

  ساكن فراغ سٽ (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, ثابت نقطو_t &pt1)
  {
    إذا (!param.is_رستو_open ())
    {
      param.شروعات_رستو ();
      param.bounds.update (env.get_pt ());
    }
    env.moveto (pt1);
    param.bounds.update (env.get_pt ());
  }

  ساكن فراغ وڪر (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, ثابت نقطو_t &pt1, ثابت نقطو_t &pt2, ثابت نقطو_t &pt3)
  {
    إذا (!param.is_رستو_open ())
    {
      param.شروعات_رستو ();
      param.bounds.update (env.get_pt ());
    }
    /* include control points */
    param.bounds.update (pt1);
    param.bounds.update (pt2);
    env.moveto (pt3);
    param.bounds.update (env.get_pt ());
  }
};

ساكن منطقي _get_bounds (ثابت OT::cff1::accelerator_t *cff, hb_codepoint_t glyph, bounds_t &bounds, منطقي اندريون_seac=خطأ, int64_t *budget = خالي_اشارو);

بنية cff1_cs_opset_extents_t : cff1_cs_opset_t<cff1_cs_opset_extents_t, cff1_extents_param_t, cff1_رستو_procs_extents_t>
{
  ساكن فراغ process_seac (cff1_cs_interp_env_t &env, cff1_extents_param_t& param)
  {
    بنانشان صحيح  n = env.arg_ڍير.get_ڳڻپ ();
    نقطو_t delta;
    delta.x = env.arg_ڍير[n-4];
    delta.y = env.arg_ڍير[n-3];
    hb_codepoint_t بنياد = param.cff->std_ڪوڊ_منزل_glyph (env.arg_ڍير[n-2].منزل_int ());
    hb_codepoint_t accent = param.cff->std_ڪوڊ_منزل_glyph (env.arg_ڍير[n-1].منزل_int ());

    bounds_t  بنياد_bounds, accent_bounds;
    إذا (likely (!env.اندريون_seac && بنياد && accent
	       && _get_bounds (param.cff, بنياد, بنياد_bounds, سچ, env.get_budget ())
	       && _get_bounds (param.cff, accent, accent_bounds, سچ, env.get_budget ())))
    {
      param.bounds.ملايو (بنياد_bounds);
      accent_bounds.وٿي (delta);
      param.bounds.ملايو (accent_bounds);
    }
    وإلا
      env.سيٽ_error ();
  }
};

منطقي _get_bounds (ثابت OT::cff1::accelerator_t *cff, hb_codepoint_t glyph, bounds_t &bounds, منطقي اندريون_seac, int64_t *budget)
{
  bounds.init ();
  إذا (unlikely (!cff->is_valid () || (glyph >= cff->انگ_glyphs))) أرجع خطأ;

  بنانشان صحيح fd = cff->fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*cff->char_لڙيون)[glyph];
  cff1_cs_interp_env_t env (str, *cff, fd);
  env.سيٽ_اندريون_seac (اندريون_seac);
  cff1_cs_interpreter_t<cff1_cs_opset_extents_t, cff1_extents_param_t> interp (env);
  cff1_extents_param_t param (cff);
  إذا (unlikely (!interp.interpret (param, budget))) أرجع خطأ;
  bounds = param.bounds;
  أرجع سچ;
}

منطقي OT::cff1::accelerator_t::get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget) ثابت
{
#جيڪڏهن_وصف_ٿيل HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  أرجع سچ;
#جيڪڏهن_پورو

  bounds_t bounds;

  int64_t ڍير_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &ڍير_budget;

  إذا (!_get_bounds (هذا, glyph, bounds, خطأ, budget))
    أرجع خطأ;

  إذا (bounds.min.x >= bounds.max.x)
  {
    extents->ويڪر = 0;
    extents->x_bearing = 0;
  }
  وإلا
  {
    مزدوج x_bearing = roundf (bounds.min.x.منزل_real ());
    extents->x_bearing = hb_clamp_منزل<hb_position_t> (x_bearing);
    extents->ويڪر = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (bounds.max.x.منزل_real ()) - x_bearing);
  }
  إذا (bounds.min.y >= bounds.max.y)
  {
    extents->اوچائي = 0;
    extents->y_bearing = 0;
  }
  وإلا
  {
    مزدوج y_bearing = roundf (bounds.max.y.منزل_real ());
    extents->y_bearing = hb_clamp_منزل<hb_position_t> (y_bearing);
    extents->اوچائي = hb_clamp_منزل<hb_position_t> ((مزدوج) roundf (bounds.min.y.منزل_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  أرجع سچ;
}

بنية cff1_رستو_param_t
{
  cff1_رستو_param_t (ثابت OT::cff1::accelerator_t *cff_, hb_font_t *font_,
		     hb_draw_session_t &draw_session_, نقطو_t *delta_)
  {
    draw_session = &draw_session_;
    cff = cff_;
    font = font_;
    delta = delta_;
  }

  فراغ move_منزل (ثابت نقطو_t &p)
  {
    نقطو_t نقطو = p;
    إذا (delta) نقطو.move (*delta);
    draw_session->move_منزل (font->em_fscalef_x (نقطو.x.منزل_real ()), font->em_fscalef_y (نقطو.y.منزل_real ()));
  }

  فراغ سٽ_منزل (ثابت نقطو_t &p)
  {
    نقطو_t نقطو = p;
    إذا (delta) نقطو.move (*delta);
    draw_session->سٽ_منزل (font->em_fscalef_x (نقطو.x.منزل_real ()), font->em_fscalef_y (نقطو.y.منزل_real ()));
  }

  فراغ cubic_منزل (ثابت نقطو_t &p1, ثابت نقطو_t &p2, ثابت نقطو_t &p3)
  {
    نقطو_t point1 = p1, point2 = p2, point3 = p3;
    إذا (delta)
    {
      point1.move (*delta);
      point2.move (*delta);
      point3.move (*delta);
    }
    draw_session->cubic_منزل (font->em_fscalef_x (point1.x.منزل_real ()), font->em_fscalef_y (point1.y.منزل_real ()),
			   font->em_fscalef_x (point2.x.منزل_real ()), font->em_fscalef_y (point2.y.منزل_real ()),
			   font->em_fscalef_x (point3.x.منزل_real ()), font->em_fscalef_y (point3.y.منزل_real ()));
  }

  فراغ پڇاڙي_رستو () { draw_session->close_رستو (); }

  hb_font_t *font;
  hb_draw_session_t *draw_session;
  نقطو_t *delta;

  ثابت OT::cff1::accelerator_t *cff;
};

بنية cff1_رستو_procs_رستو_t : رستو_procs_t<cff1_رستو_procs_رستو_t, cff1_cs_interp_env_t, cff1_رستو_param_t>
{
  ساكن فراغ moveto (cff1_cs_interp_env_t &env, cff1_رستو_param_t& param, ثابت نقطو_t &pt)
  {
    param.move_منزل (pt);
    env.moveto (pt);
  }

  ساكن فراغ سٽ (cff1_cs_interp_env_t &env, cff1_رستو_param_t &param, ثابت نقطو_t &pt1)
  {
    param.سٽ_منزل (pt1);
    env.moveto (pt1);
  }

  ساكن فراغ وڪر (cff1_cs_interp_env_t &env, cff1_رستو_param_t &param, ثابت نقطو_t &pt1, ثابت نقطو_t &pt2, ثابت نقطو_t &pt3)
  {
    param.cubic_منزل (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

ساكن منطقي get_رستو_2 (ثابت OT::cff1::accelerator_t *cff, hb_font_t *font, hb_codepoint_t glyph,
		       hb_draw_session_t &draw_session, منطقي اندريون_seac = خطأ, نقطو_t *delta = خالي_اشارو,
		       int64_t *budget = خالي_اشارو);

بنية cff1_cs_opset_رستو_t : cff1_cs_opset_t<cff1_cs_opset_رستو_t, cff1_رستو_param_t, cff1_رستو_procs_رستو_t>
{
  ساكن فراغ process_seac (cff1_cs_interp_env_t &env, cff1_رستو_param_t& param)
  {
    /* End previous path */
    param.پڇاڙي_رستو ();

    بنانشان صحيح n = env.arg_ڍير.get_ڳڻپ ();
    نقطو_t delta;
    delta.x = env.arg_ڍير[n-4];
    delta.y = env.arg_ڍير[n-3];
    hb_codepoint_t بنياد = param.cff->std_ڪوڊ_منزل_glyph (env.arg_ڍير[n-2].منزل_int ());
    hb_codepoint_t accent = param.cff->std_ڪوڊ_منزل_glyph (env.arg_ڍير[n-1].منزل_int ());

    إذا (unlikely (!(!env.اندريون_seac && بنياد && accent
		    && get_رستو_2 (param.cff, param.font, بنياد, *param.draw_session, سچ, خالي_اشارو, env.get_budget ())
		    && get_رستو_2 (param.cff, param.font, accent, *param.draw_session, سچ, &delta, env.get_budget ()))))
      env.سيٽ_error ();
  }
};

منطقي get_رستو_2 (ثابت OT::cff1::accelerator_t *cff, hb_font_t *font, hb_codepoint_t glyph,
		hb_draw_session_t &draw_session, منطقي اندريون_seac, نقطو_t *delta,
		int64_t *budget)
{
  إذا (unlikely (!cff->is_valid () || (glyph >= cff->انگ_glyphs))) أرجع خطأ;

  بنانشان صحيح fd = cff->fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*cff->char_لڙيون)[glyph];
  cff1_cs_interp_env_t env (str, *cff, fd);
  env.سيٽ_اندريون_seac (اندريون_seac);
  cff1_cs_interpreter_t<cff1_cs_opset_رستو_t, cff1_رستو_param_t> interp (env);
  cff1_رستو_param_t param (cff, font, draw_session, delta);
  إذا (unlikely (!interp.interpret (param, budget))) أرجع خطأ;

  /* Let's end the path specially since it is called inside seac also */
  param.پڇاڙي_رستو ();

  أرجع سچ;
}

منطقي OT::cff1::accelerator_t::get_رستو (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget) ثابت
{
#جيڪڏهن_وصف_ٿيل HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  أرجع سچ;
#جيڪڏهن_پورو

  int64_t ڍير_budget = HB_BUDGET_GLYPH;
  إذا (!budget) budget = &ڍير_budget;

  أرجع get_رستو_2 (هذا, font, glyph, draw_session, خطأ, خالي_اشارو, budget);
}

بنية get_seac_param_t
{
  get_seac_param_t (ثابت OT::cff1::accelerator_subset_t *_cff) : cff (_cff) {}

  منطقي has_seac () ثابت { أرجع بنياد && accent; }

  ثابت OT::cff1::accelerator_subset_t *cff;
  hb_codepoint_t  بنياد = 0;
  hb_codepoint_t  accent = 0;
};

بنية cff1_cs_opset_seac_t : cff1_cs_opset_t<cff1_cs_opset_seac_t, get_seac_param_t>
{
  ساكن فراغ process_seac (cff1_cs_interp_env_t &env, get_seac_param_t& param)
  {
    بنانشان صحيح  n = env.arg_ڍير.get_ڳڻپ ();
    hb_codepoint_t  بنياد_char = (hb_codepoint_t)env.arg_ڍير[n-2].منزل_int ();
    hb_codepoint_t  accent_char = (hb_codepoint_t)env.arg_ڍير[n-1].منزل_int ();

    param.بنياد = param.cff->std_ڪوڊ_منزل_glyph (بنياد_char);
    param.accent = param.cff->std_ڪوڊ_منزل_glyph (accent_char);
  }
};

ساكن منطقي
cff1_charstring_may_پڇاڙي_اندريون_seac (ثابت hb_ubytes_t &str)
{
  إذا (unlikely (!str.ڊيگهائي || str[str.ڊيگهائي - 1] != Op_ڪوڊ_endchar))
    أرجع خطأ;

  بنانشان صحيح پڇاڙي = str.ڊيگهائي - 1;
  إذا (!پڇاڙي) أرجع خطأ;

  بنانشان صحيح آخري = str[پڇاڙي - 1];

  /* Type 2 numbers are not self-synchronizing when read backwards.
   * Keep interpreting if any number encoding could end at endchar. */
  إذا (Op_ڪوڊ_One_Byte_Int_پهريون <= آخري && آخري <= Op_ڪوڊ_One_Byte_Int_آخري)
    أرجع سچ;
  إذا (پڇاڙي >= 2 &&
      Op_ڪوڊ_Two_Byte_Pos_Int0 <= str[پڇاڙي - 2] &&
      str[پڇاڙي - 2] <= Op_ڪوڊ_Two_Byte_Neg_Int3)
    أرجع سچ;
  إذا (پڇاڙي >= 3 && str[پڇاڙي - 3] == Op_ڪوڊ_shortint)
    أرجع سچ;
  إذا (پڇاڙي >= 5 && str[پڇاڙي - 5] == Op_ڪوڊ_fixedcs)
    أرجع سچ;

  /* A subroutine can leave the seac operands on the shared stack. */
  أرجع آخري == Op_ڪوڊ_callsubr || آخري == Op_ڪوڊ_callgsubr;
}

منطقي OT::cff1::accelerator_subset_t::get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *بنياد, hb_codepoint_t *accent) ثابت
{
  إذا (unlikely (!is_valid () || (glyph >= انگ_glyphs))) أرجع خطأ;

  بنانشان صحيح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_لڙيون)[glyph];
  إذا (unlikely (!cff1_charstring_may_پڇاڙي_اندريون_seac (str))) أرجع خطأ;
  cff1_cs_interp_env_t env (str, *هذا, fd);
  cff1_cs_interpreter_t<cff1_cs_opset_seac_t, get_seac_param_t> interp (env);
  get_seac_param_t  param (هذا);
  إذا (unlikely (!interp.interpret (param))) أرجع خطأ;

  إذا (param.has_seac ())
  {
    *بنياد = param.بنياد;
    *accent = param.accent;
    أرجع سچ;
  }
  أرجع خطأ;
}


#جيڪڏهن_پورو

#جيڪڏهن_پورو /* HB_OT_CFF1_TABLE_CC pacify */
#جيڪڏهن_پورو /* HB_OT_CFF1_TABLE_CC guard */
