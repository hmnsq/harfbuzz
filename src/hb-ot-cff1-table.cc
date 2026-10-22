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

#اگر_غیر_تعریف_شدہ HB_OT_CFF1_جدول_CC
#تعریف_کرو HB_OT_CFF1_جدول_CC
#اگر_تعریف_شدہ HB_OT_CFF1_جدول_CC /* Pacify -Wunused-macros. */

#شامل "hb.hh"

#اگر_غیر_تعریف_شدہ HB_NO_CFF

#شامل "hb-draw.hh"
#شامل "hb-algs.hh"
#شامل "hb-ot-cff1-table.hh"
#شامل "hb-cff1-interp-cs.hh"

استعمال نامخلا CFF;

ساخت sid_مقصد_gid_t
{
  uint16_t  sid;
  uint8_t   gid;

  صحیح cmp (uint16_t a) ثابت
  {
    اگر (a == sid) واپس 0;
    واپس (a < sid) ? -1 : 1;
  }
};

/* SID to code */
ساکن ثابت uint8_t standard_encoding_مقصد_رمز [] =
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
ساکن ثابت uint8_t expert_encoding_مقصد_رمز [] =
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
ساکن ثابت uint16_t expert_charset_مقصد_sid [] =
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
ساکن ثابت uint16_t expert_subset_charset_مقصد_sid [] =
{
    0,    1,  231,  232,  235,  236,  237,  238,   13,   14,   15,   99,  239,  240,  241,  242,
  243,  244,  245,  246,  247,  248,   27,   28,  249,  250,  251,  253,  254,  255,  256,  257,
  258,  259,  260,  261,  262,  263,  264,  265,  266,  109,  110,  267,  268,  269,  270,  272,
  300,  301,  302,  305,  314,  315,  158,  155,  163,  320,  321,  322,  323,  324,  325,  326,
  150,  164,  169,  327,  328,  329,  330,  331,  332,  333,  334,  335,  336,  337,  338,  339,
  340,  341,  342,  343,  344,  345,  346
};

/* SID to glyph ID */
ساکن ثابت sid_مقصد_gid_t expert_charset_sid_مقصد_gid [] =
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
ساکن ثابت sid_مقصد_gid_t expert_subset_charset_sid_مقصد_gid [] =
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
ساکن ثابت uint8_t standard_encoding_مقصد_sid [] =
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

hb_codepoint_t OT::cff1::lookup_standard_encoding_for_رمز (hb_codepoint_t sid)
{
  اگر (sid < ARRAY_لمبائی (standard_encoding_مقصد_رمز))
    واپس (hb_codepoint_t)standard_encoding_مقصد_رمز[sid];
  ورنہ
    واپس 0;
}

hb_codepoint_t OT::cff1::lookup_expert_encoding_for_رمز (hb_codepoint_t sid)
{
  اگر (sid < ARRAY_لمبائی (expert_encoding_مقصد_رمز))
    واپس (hb_codepoint_t)expert_encoding_مقصد_رمز[sid];
  ورنہ
    واپس 0;
}

hb_codepoint_t OT::cff1::lookup_expert_charset_for_sid (hb_codepoint_t glyph)
{
  اگر (glyph < ARRAY_لمبائی (expert_charset_مقصد_sid))
    واپس (hb_codepoint_t)expert_charset_مقصد_sid[glyph];
  ورنہ
    واپس 0;
}

hb_codepoint_t OT::cff1::lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph)
{
  اگر (glyph < ARRAY_لمبائی (expert_subset_charset_مقصد_sid))
    واپس (hb_codepoint_t)expert_subset_charset_مقصد_sid[glyph];
  ورنہ
    واپس 0;
}

hb_codepoint_t OT::cff1::lookup_expert_charset_for_glyph (hb_codepoint_t sid)
{
  ثابت خودکار *pair = hb_sorted_array (expert_charset_sid_مقصد_gid).bsearch (sid);
  واپس pair ? pair->gid : 0;
}

hb_codepoint_t OT::cff1::lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid)
{
  ثابت خودکار *pair = hb_sorted_array (expert_subset_charset_sid_مقصد_gid).bsearch (sid);
  واپس pair ? pair->gid : 0;
}

hb_codepoint_t OT::cff1::lookup_standard_encoding_for_sid (hb_codepoint_t رمز)
{
  اگر (رمز < ARRAY_لمبائی (standard_encoding_مقصد_sid))
    واپس (hb_codepoint_t)standard_encoding_مقصد_sid[رمز];
  ورنہ
    واپس CFF_UNDEF_SID;
}

ساخت bounds_t
{
  خالی init ()
  {
    min.سیٹ_int (زیادہ_سے_زیادہ_عدد, زیادہ_سے_زیادہ_عدد);
    max.سیٹ_int (INT_MIN, INT_MIN);
  }

  خالی update (ثابت نقطہ_t &pt)
  {
    اگر (pt.x < min.x) min.x = pt.x;
    اگر (pt.x > max.x) max.x = pt.x;
    اگر (pt.y < min.y) min.y = pt.y;
    اگر (pt.y > max.y) max.y = pt.y;
  }

  خالی ملاؤ (ثابت bounds_t &b)
  {
    اگر (خالی_خانے_2 ())
      *یہ = b;
    ورنہ اگر (!b.خالی_خانے_2 ())
    {
      اگر (b.min.x < min.x) min.x = b.min.x;
      اگر (b.max.x > max.x) max.x = b.max.x;
      اگر (b.min.y < min.y) min.y = b.min.y;
      اگر (b.max.y > max.y) max.y = b.max.y;
    }
  }

  خالی انحراف (ثابت نقطہ_t &delta)
  {
    اگر (!خالی_خانے_2 ())
    {
      min.move (delta);
      max.move (delta);
    }
  }

  منطقی خالی_خانے_2 () ثابت { واپس (min.x >= max.x) || (min.y >= max.y); }

  نقطہ_t min;
  نقطہ_t max;
};

ساخت cff1_extents_param_t
{
  cff1_extents_param_t (ثابت OT::cff1::accelerator_t *_cff) : cff (_cff)
  {
    bounds.init ();
  }

  خالی آغاز_راستہ   ()       { راستہ_open = سچ; }
  خالی انتہا_راستہ     ()       { راستہ_open = جھوٹ; }
  منطقی is_راستہ_open () ثابت { واپس راستہ_open; }

  منطقی راستہ_open = جھوٹ;
  bounds_t bounds;

  ثابت OT::cff1::accelerator_t *cff;
};

ساخت cff1_راستہ_procs_extents_t : راستہ_procs_t<cff1_راستہ_procs_extents_t, cff1_cs_interp_env_t, cff1_extents_param_t>
{
  ساکن خالی moveto (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, ثابت نقطہ_t &pt)
  {
    param.انتہا_راستہ ();
    env.moveto (pt);
  }

  ساکن خالی سطر (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, ثابت نقطہ_t &pt1)
  {
    اگر (!param.is_راستہ_open ())
    {
      param.آغاز_راستہ ();
      param.bounds.update (env.get_pt ());
    }
    env.moveto (pt1);
    param.bounds.update (env.get_pt ());
  }

  ساکن خالی خم (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3)
  {
    اگر (!param.is_راستہ_open ())
    {
      param.آغاز_راستہ ();
      param.bounds.update (env.get_pt ());
    }
    /* include control points */
    param.bounds.update (pt1);
    param.bounds.update (pt2);
    env.moveto (pt3);
    param.bounds.update (env.get_pt ());
  }
};

ساکن منطقی _get_bounds (ثابت OT::cff1::accelerator_t *cff, hb_codepoint_t glyph, bounds_t &bounds, منطقی اندر_seac=جھوٹ, int64_t *budget = خالی_اشاریہ);

ساخت cff1_cs_opset_extents_t : cff1_cs_opset_t<cff1_cs_opset_extents_t, cff1_extents_param_t, cff1_راستہ_procs_extents_t>
{
  ساکن خالی process_seac (cff1_cs_interp_env_t &env, cff1_extents_param_t& param)
  {
    غیرعلامتی صحیح  n = env.arg_ڈھیر.get_گنتی ();
    نقطہ_t delta;
    delta.x = env.arg_ڈھیر[n-4];
    delta.y = env.arg_ڈھیر[n-3];
    hb_codepoint_t بنیاد = param.cff->std_رمز_مقصد_glyph (env.arg_ڈھیر[n-2].مقصد_int ());
    hb_codepoint_t accent = param.cff->std_رمز_مقصد_glyph (env.arg_ڈھیر[n-1].مقصد_int ());

    bounds_t  بنیاد_bounds, accent_bounds;
    اگر (likely (!env.اندر_seac && بنیاد && accent
	       && _get_bounds (param.cff, بنیاد, بنیاد_bounds, سچ, env.get_budget ())
	       && _get_bounds (param.cff, accent, accent_bounds, سچ, env.get_budget ())))
    {
      param.bounds.ملاؤ (بنیاد_bounds);
      accent_bounds.انحراف (delta);
      param.bounds.ملاؤ (accent_bounds);
    }
    ورنہ
      env.سیٹ_error ();
  }
};

منطقی _get_bounds (ثابت OT::cff1::accelerator_t *cff, hb_codepoint_t glyph, bounds_t &bounds, منطقی اندر_seac, int64_t *budget)
{
  bounds.init ();
  اگر (unlikely (!cff->is_valid () || (glyph >= cff->عدد_glyphs))) واپس جھوٹ;

  غیرعلامتی صحیح fd = cff->fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*cff->char_لڑیاں)[glyph];
  cff1_cs_interp_env_t env (str, *cff, fd);
  env.سیٹ_اندر_seac (اندر_seac);
  cff1_cs_interpreter_t<cff1_cs_opset_extents_t, cff1_extents_param_t> interp (env);
  cff1_extents_param_t param (cff);
  اگر (unlikely (!interp.interpret (param, budget))) واپس جھوٹ;
  bounds = param.bounds;
  واپس سچ;
}

منطقی OT::cff1::accelerator_t::get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget) ثابت
{
#اگر_تعریف_شدہ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  واپس سچ;
#اگر_ختم

  bounds_t bounds;

  int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
  اگر (!budget) budget = &ڈھیر_budget;

  اگر (!_get_bounds (یہ, glyph, bounds, جھوٹ, budget))
    واپس جھوٹ;

  اگر (bounds.min.x >= bounds.max.x)
  {
    extents->چوڑائی = 0;
    extents->x_bearing = 0;
  }
  ورنہ
  {
    دوہرا x_bearing = roundf (bounds.min.x.مقصد_real ());
    extents->x_bearing = hb_clamp_مقصد<hb_position_t> (x_bearing);
    extents->چوڑائی = hb_clamp_مقصد<hb_position_t> ((دوہرا) roundf (bounds.max.x.مقصد_real ()) - x_bearing);
  }
  اگر (bounds.min.y >= bounds.max.y)
  {
    extents->اونچائی = 0;
    extents->y_bearing = 0;
  }
  ورنہ
  {
    دوہرا y_bearing = roundf (bounds.max.y.مقصد_real ());
    extents->y_bearing = hb_clamp_مقصد<hb_position_t> (y_bearing);
    extents->اونچائی = hb_clamp_مقصد<hb_position_t> ((دوہرا) roundf (bounds.min.y.مقصد_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  واپس سچ;
}

ساخت cff1_راستہ_param_t
{
  cff1_راستہ_param_t (ثابت OT::cff1::accelerator_t *cff_, hb_font_t *font_,
		     hb_draw_session_t &draw_session_, نقطہ_t *delta_)
  {
    draw_session = &draw_session_;
    cff = cff_;
    font = font_;
    delta = delta_;
  }

  خالی move_مقصد (ثابت نقطہ_t &p)
  {
    نقطہ_t نقطہ = p;
    اگر (delta) نقطہ.move (*delta);
    draw_session->move_مقصد (font->em_fscalef_x (نقطہ.x.مقصد_real ()), font->em_fscalef_y (نقطہ.y.مقصد_real ()));
  }

  خالی سطر_مقصد (ثابت نقطہ_t &p)
  {
    نقطہ_t نقطہ = p;
    اگر (delta) نقطہ.move (*delta);
    draw_session->سطر_مقصد (font->em_fscalef_x (نقطہ.x.مقصد_real ()), font->em_fscalef_y (نقطہ.y.مقصد_real ()));
  }

  خالی cubic_مقصد (ثابت نقطہ_t &p1, ثابت نقطہ_t &p2, ثابت نقطہ_t &p3)
  {
    نقطہ_t point1 = p1, point2 = p2, point3 = p3;
    اگر (delta)
    {
      point1.move (*delta);
      point2.move (*delta);
      point3.move (*delta);
    }
    draw_session->cubic_مقصد (font->em_fscalef_x (point1.x.مقصد_real ()), font->em_fscalef_y (point1.y.مقصد_real ()),
			   font->em_fscalef_x (point2.x.مقصد_real ()), font->em_fscalef_y (point2.y.مقصد_real ()),
			   font->em_fscalef_x (point3.x.مقصد_real ()), font->em_fscalef_y (point3.y.مقصد_real ()));
  }

  خالی انتہا_راستہ () { draw_session->close_راستہ (); }

  hb_font_t *font;
  hb_draw_session_t *draw_session;
  نقطہ_t *delta;

  ثابت OT::cff1::accelerator_t *cff;
};

ساخت cff1_راستہ_procs_راستہ_t : راستہ_procs_t<cff1_راستہ_procs_راستہ_t, cff1_cs_interp_env_t, cff1_راستہ_param_t>
{
  ساکن خالی moveto (cff1_cs_interp_env_t &env, cff1_راستہ_param_t& param, ثابت نقطہ_t &pt)
  {
    param.move_مقصد (pt);
    env.moveto (pt);
  }

  ساکن خالی سطر (cff1_cs_interp_env_t &env, cff1_راستہ_param_t &param, ثابت نقطہ_t &pt1)
  {
    param.سطر_مقصد (pt1);
    env.moveto (pt1);
  }

  ساکن خالی خم (cff1_cs_interp_env_t &env, cff1_راستہ_param_t &param, ثابت نقطہ_t &pt1, ثابت نقطہ_t &pt2, ثابت نقطہ_t &pt3)
  {
    param.cubic_مقصد (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

ساکن منطقی get_راستہ_2 (ثابت OT::cff1::accelerator_t *cff, hb_font_t *font, hb_codepoint_t glyph,
		       hb_draw_session_t &draw_session, منطقی اندر_seac = جھوٹ, نقطہ_t *delta = خالی_اشاریہ,
		       int64_t *budget = خالی_اشاریہ);

ساخت cff1_cs_opset_راستہ_t : cff1_cs_opset_t<cff1_cs_opset_راستہ_t, cff1_راستہ_param_t, cff1_راستہ_procs_راستہ_t>
{
  ساکن خالی process_seac (cff1_cs_interp_env_t &env, cff1_راستہ_param_t& param)
  {
    /* End previous path */
    param.انتہا_راستہ ();

    غیرعلامتی صحیح n = env.arg_ڈھیر.get_گنتی ();
    نقطہ_t delta;
    delta.x = env.arg_ڈھیر[n-4];
    delta.y = env.arg_ڈھیر[n-3];
    hb_codepoint_t بنیاد = param.cff->std_رمز_مقصد_glyph (env.arg_ڈھیر[n-2].مقصد_int ());
    hb_codepoint_t accent = param.cff->std_رمز_مقصد_glyph (env.arg_ڈھیر[n-1].مقصد_int ());

    اگر (unlikely (!(!env.اندر_seac && بنیاد && accent
		    && get_راستہ_2 (param.cff, param.font, بنیاد, *param.draw_session, سچ, خالی_اشاریہ, env.get_budget ())
		    && get_راستہ_2 (param.cff, param.font, accent, *param.draw_session, سچ, &delta, env.get_budget ()))))
      env.سیٹ_error ();
  }
};

منطقی get_راستہ_2 (ثابت OT::cff1::accelerator_t *cff, hb_font_t *font, hb_codepoint_t glyph,
		hb_draw_session_t &draw_session, منطقی اندر_seac, نقطہ_t *delta,
		int64_t *budget)
{
  اگر (unlikely (!cff->is_valid () || (glyph >= cff->عدد_glyphs))) واپس جھوٹ;

  غیرعلامتی صحیح fd = cff->fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*cff->char_لڑیاں)[glyph];
  cff1_cs_interp_env_t env (str, *cff, fd);
  env.سیٹ_اندر_seac (اندر_seac);
  cff1_cs_interpreter_t<cff1_cs_opset_راستہ_t, cff1_راستہ_param_t> interp (env);
  cff1_راستہ_param_t param (cff, font, draw_session, delta);
  اگر (unlikely (!interp.interpret (param, budget))) واپس جھوٹ;

  /* Let's end the path specially since it is called inside seac also */
  param.انتہا_راستہ ();

  واپس سچ;
}

منطقی OT::cff1::accelerator_t::get_راستہ (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget) ثابت
{
#اگر_تعریف_شدہ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  واپس سچ;
#اگر_ختم

  int64_t ڈھیر_budget = HB_BUDGET_GLYPH;
  اگر (!budget) budget = &ڈھیر_budget;

  واپس get_راستہ_2 (یہ, font, glyph, draw_session, جھوٹ, خالی_اشاریہ, budget);
}

ساخت get_seac_param_t
{
  get_seac_param_t (ثابت OT::cff1::accelerator_subset_t *_cff) : cff (_cff) {}

  منطقی has_seac () ثابت { واپس بنیاد && accent; }

  ثابت OT::cff1::accelerator_subset_t *cff;
  hb_codepoint_t  بنیاد = 0;
  hb_codepoint_t  accent = 0;
};

ساخت cff1_cs_opset_seac_t : cff1_cs_opset_t<cff1_cs_opset_seac_t, get_seac_param_t>
{
  ساکن خالی process_seac (cff1_cs_interp_env_t &env, get_seac_param_t& param)
  {
    غیرعلامتی صحیح  n = env.arg_ڈھیر.get_گنتی ();
    hb_codepoint_t  بنیاد_char = (hb_codepoint_t)env.arg_ڈھیر[n-2].مقصد_int ();
    hb_codepoint_t  accent_char = (hb_codepoint_t)env.arg_ڈھیر[n-1].مقصد_int ();

    param.بنیاد = param.cff->std_رمز_مقصد_glyph (بنیاد_char);
    param.accent = param.cff->std_رمز_مقصد_glyph (accent_char);
  }
};

ساکن منطقی
cff1_charstring_may_انتہا_اندر_seac (ثابت hb_ubytes_t &str)
{
  اگر (unlikely (!str.لمبائی || str[str.لمبائی - 1] != Op_رمز_endchar))
    واپس جھوٹ;

  غیرعلامتی صحیح انتہا = str.لمبائی - 1;
  اگر (!انتہا) واپس جھوٹ;

  غیرعلامتی صحیح آخری = str[انتہا - 1];

  /* Type 2 numbers are not self-synchronizing when read backwards.
   * Keep interpreting if any number encoding could end at endchar. */
  اگر (Op_رمز_One_Byte_Int_پہلا <= آخری && آخری <= Op_رمز_One_Byte_Int_آخری)
    واپس سچ;
  اگر (انتہا >= 2 &&
      Op_رمز_Two_Byte_Pos_Int0 <= str[انتہا - 2] &&
      str[انتہا - 2] <= Op_رمز_Two_Byte_Neg_Int3)
    واپس سچ;
  اگر (انتہا >= 3 && str[انتہا - 3] == Op_رمز_shortint)
    واپس سچ;
  اگر (انتہا >= 5 && str[انتہا - 5] == Op_رمز_fixedcs)
    واپس سچ;

  /* A subroutine can leave the seac operands on the shared stack. */
  واپس آخری == Op_رمز_callsubr || آخری == Op_رمز_callgsubr;
}

منطقی OT::cff1::accelerator_subset_t::get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *بنیاد, hb_codepoint_t *accent) ثابت
{
  اگر (unlikely (!is_valid () || (glyph >= عدد_glyphs))) واپس جھوٹ;

  غیرعلامتی صحیح fd = fdSelect->get_fd (glyph);
  ثابت hb_ubytes_t str = (*char_لڑیاں)[glyph];
  اگر (unlikely (!cff1_charstring_may_انتہا_اندر_seac (str))) واپس جھوٹ;
  cff1_cs_interp_env_t env (str, *یہ, fd);
  cff1_cs_interpreter_t<cff1_cs_opset_seac_t, get_seac_param_t> interp (env);
  get_seac_param_t  param (یہ);
  اگر (unlikely (!interp.interpret (param))) واپس جھوٹ;

  اگر (param.has_seac ())
  {
    *بنیاد = param.بنیاد;
    *accent = param.accent;
    واپس سچ;
  }
  واپس جھوٹ;
}


#اگر_ختم

#اگر_ختم /* HB_OT_CFF1_TABLE_CC pacify */
#اگر_ختم /* HB_OT_CFF1_TABLE_CC guard */
