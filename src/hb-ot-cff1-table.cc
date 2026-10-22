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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_CFF1_ਸਾਰਣੀ_CC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_CFF1_ਸਾਰਣੀ_CC
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_OT_CFF1_ਸਾਰਣੀ_CC /* Pacify -Wunused-macros. */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_CFF

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-algs.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff1-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff1-interp-cs.hh"

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ CFF;

ਬਣਤਰ sid_ਮੰਜ਼ਿਲ_gid_t
{
  uint16_t  sid;
  uint8_t   gid;

  ਪੂਰਨ cmp (uint16_t a) ਅਚਲ
  {
    ਜੇ (a == sid) ਵਾਪਸ 0;
    ਵਾਪਸ (a < sid) ? -1 : 1;
  }
};

/* SID to code */
ਸਥਿਰ ਅਚਲ uint8_t standard_encoding_ਮੰਜ਼ਿਲ_ਕੋਡ [] =
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
ਸਥਿਰ ਅਚਲ uint8_t expert_encoding_ਮੰਜ਼ਿਲ_ਕੋਡ [] =
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
ਸਥਿਰ ਅਚਲ uint16_t expert_charset_ਮੰਜ਼ਿਲ_sid [] =
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
ਸਥਿਰ ਅਚਲ uint16_t expert_subset_charset_ਮੰਜ਼ਿਲ_sid [] =
{
    0,    1,  231,  232,  235,  236,  237,  238,   13,   14,   15,   99,  239,  240,  241,  242,
  243,  244,  245,  246,  247,  248,   27,   28,  249,  250,  251,  253,  254,  255,  256,  257,
  258,  259,  260,  261,  262,  263,  264,  265,  266,  109,  110,  267,  268,  269,  270,  272,
  300,  301,  302,  305,  314,  315,  158,  155,  163,  320,  321,  322,  323,  324,  325,  326,
  150,  164,  169,  327,  328,  329,  330,  331,  332,  333,  334,  335,  336,  337,  338,  339,
  340,  341,  342,  343,  344,  345,  346
};

/* SID to glyph ID */
ਸਥਿਰ ਅਚਲ sid_ਮੰਜ਼ਿਲ_gid_t expert_charset_sid_ਮੰਜ਼ਿਲ_gid [] =
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
ਸਥਿਰ ਅਚਲ sid_ਮੰਜ਼ਿਲ_gid_t expert_subset_charset_sid_ਮੰਜ਼ਿਲ_gid [] =
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
ਸਥਿਰ ਅਚਲ uint8_t standard_encoding_ਮੰਜ਼ਿਲ_sid [] =
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

hb_codepoint_t OT::cff1::lookup_standard_encoding_for_ਕੋਡ (hb_codepoint_t sid)
{
  ਜੇ (sid < ARRAY_ਲੰਮਾਈ (standard_encoding_ਮੰਜ਼ਿਲ_ਕੋਡ))
    ਵਾਪਸ (hb_codepoint_t)standard_encoding_ਮੰਜ਼ਿਲ_ਕੋਡ[sid];
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
}

hb_codepoint_t OT::cff1::lookup_expert_encoding_for_ਕੋਡ (hb_codepoint_t sid)
{
  ਜੇ (sid < ARRAY_ਲੰਮਾਈ (expert_encoding_ਮੰਜ਼ਿਲ_ਕੋਡ))
    ਵਾਪਸ (hb_codepoint_t)expert_encoding_ਮੰਜ਼ਿਲ_ਕੋਡ[sid];
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
}

hb_codepoint_t OT::cff1::lookup_expert_charset_for_sid (hb_codepoint_t glyph)
{
  ਜੇ (glyph < ARRAY_ਲੰਮਾਈ (expert_charset_ਮੰਜ਼ਿਲ_sid))
    ਵਾਪਸ (hb_codepoint_t)expert_charset_ਮੰਜ਼ਿਲ_sid[glyph];
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
}

hb_codepoint_t OT::cff1::lookup_expert_subset_charset_for_sid (hb_codepoint_t glyph)
{
  ਜੇ (glyph < ARRAY_ਲੰਮਾਈ (expert_subset_charset_ਮੰਜ਼ਿਲ_sid))
    ਵਾਪਸ (hb_codepoint_t)expert_subset_charset_ਮੰਜ਼ਿਲ_sid[glyph];
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
}

hb_codepoint_t OT::cff1::lookup_expert_charset_for_glyph (hb_codepoint_t sid)
{
  ਅਚਲ ਸਵੈਚਾਲਿਤ *pair = hb_sorted_array (expert_charset_sid_ਮੰਜ਼ਿਲ_gid).bsearch (sid);
  ਵਾਪਸ pair ? pair->gid : 0;
}

hb_codepoint_t OT::cff1::lookup_expert_subset_charset_for_glyph (hb_codepoint_t sid)
{
  ਅਚਲ ਸਵੈਚਾਲਿਤ *pair = hb_sorted_array (expert_subset_charset_sid_ਮੰਜ਼ਿਲ_gid).bsearch (sid);
  ਵਾਪਸ pair ? pair->gid : 0;
}

hb_codepoint_t OT::cff1::lookup_standard_encoding_for_sid (hb_codepoint_t ਕੋਡ)
{
  ਜੇ (ਕੋਡ < ARRAY_ਲੰਮਾਈ (standard_encoding_ਮੰਜ਼ਿਲ_sid))
    ਵਾਪਸ (hb_codepoint_t)standard_encoding_ਮੰਜ਼ਿਲ_sid[ਕੋਡ];
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ CFF_UNDEF_SID;
}

ਬਣਤਰ bounds_t
{
  ਖਾਲੀ init ()
  {
    min.ਸੈੱਟ_int (ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ, ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ);
    max.ਸੈੱਟ_int (INT_MIN, INT_MIN);
  }

  ਖਾਲੀ update (ਅਚਲ ਬਿੰਦੂ_t &pt)
  {
    ਜੇ (pt.x < min.x) min.x = pt.x;
    ਜੇ (pt.x > max.x) max.x = pt.x;
    ਜੇ (pt.y < min.y) min.y = pt.y;
    ਜੇ (pt.y > max.y) max.y = pt.y;
  }

  ਖਾਲੀ ਮਿਲਾਓ (ਅਚਲ bounds_t &b)
  {
    ਜੇ (ਖਾਲੀ_ਥਾਵਾਂ_2 ())
      *ਇਹ = b;
    ਨਹੀਂਤਾਂ ਜੇ (!b.ਖਾਲੀ_ਥਾਵਾਂ_2 ())
    {
      ਜੇ (b.min.x < min.x) min.x = b.min.x;
      ਜੇ (b.max.x > max.x) max.x = b.max.x;
      ਜੇ (b.min.y < min.y) min.y = b.min.y;
      ਜੇ (b.max.y > max.y) max.y = b.max.y;
    }
  }

  ਖਾਲੀ ਵਿੱਥ (ਅਚਲ ਬਿੰਦੂ_t &delta)
  {
    ਜੇ (!ਖਾਲੀ_ਥਾਵਾਂ_2 ())
    {
      min.move (delta);
      max.move (delta);
    }
  }

  ਤਾਰਕਿਕ ਖਾਲੀ_ਥਾਵਾਂ_2 () ਅਚਲ { ਵਾਪਸ (min.x >= max.x) || (min.y >= max.y); }

  ਬਿੰਦੂ_t min;
  ਬਿੰਦੂ_t max;
};

ਬਣਤਰ cff1_extents_param_t
{
  cff1_extents_param_t (ਅਚਲ OT::cff1::accelerator_t *_cff) : cff (_cff)
  {
    bounds.init ();
  }

  ਖਾਲੀ ਸ਼ੁਰੂ_ਰਸਤਾ   ()       { ਰਸਤਾ_open = ਸੱਚ; }
  ਖਾਲੀ ਸਿਰਾ_ਰਸਤਾ     ()       { ਰਸਤਾ_open = ਝੂਠ; }
  ਤਾਰਕਿਕ is_ਰਸਤਾ_open () ਅਚਲ { ਵਾਪਸ ਰਸਤਾ_open; }

  ਤਾਰਕਿਕ ਰਸਤਾ_open = ਝੂਠ;
  bounds_t bounds;

  ਅਚਲ OT::cff1::accelerator_t *cff;
};

ਬਣਤਰ cff1_ਰਸਤਾ_procs_extents_t : ਰਸਤਾ_procs_t<cff1_ਰਸਤਾ_procs_extents_t, cff1_cs_interp_env_t, cff1_extents_param_t>
{
  ਸਥਿਰ ਖਾਲੀ moveto (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt)
  {
    param.ਸਿਰਾ_ਰਸਤਾ ();
    env.moveto (pt);
  }

  ਸਥਿਰ ਖਾਲੀ ਸਤਰ (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt1)
  {
    ਜੇ (!param.is_ਰਸਤਾ_open ())
    {
      param.ਸ਼ੁਰੂ_ਰਸਤਾ ();
      param.bounds.update (env.get_pt ());
    }
    env.moveto (pt1);
    param.bounds.update (env.get_pt ());
  }

  ਸਥਿਰ ਖਾਲੀ ਵਕਰ (cff1_cs_interp_env_t &env, cff1_extents_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt1, ਅਚਲ ਬਿੰਦੂ_t &pt2, ਅਚਲ ਬਿੰਦੂ_t &pt3)
  {
    ਜੇ (!param.is_ਰਸਤਾ_open ())
    {
      param.ਸ਼ੁਰੂ_ਰਸਤਾ ();
      param.bounds.update (env.get_pt ());
    }
    /* include control points */
    param.bounds.update (pt1);
    param.bounds.update (pt2);
    env.moveto (pt3);
    param.bounds.update (env.get_pt ());
  }
};

ਸਥਿਰ ਤਾਰਕਿਕ _get_bounds (ਅਚਲ OT::cff1::accelerator_t *cff, hb_codepoint_t glyph, bounds_t &bounds, ਤਾਰਕਿਕ ਵਿੱਚ_seac=ਝੂਠ, int64_t *budget = ਖਾਲੀ_ਸੂਚਕ);

ਬਣਤਰ cff1_cs_opset_extents_t : cff1_cs_opset_t<cff1_cs_opset_extents_t, cff1_extents_param_t, cff1_ਰਸਤਾ_procs_extents_t>
{
  ਸਥਿਰ ਖਾਲੀ process_seac (cff1_cs_interp_env_t &env, cff1_extents_param_t& param)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  n = env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();
    ਬਿੰਦੂ_t delta;
    delta.x = env.arg_ਢੇਰ[n-4];
    delta.y = env.arg_ਢੇਰ[n-3];
    hb_codepoint_t ਅਧਾਰ = param.cff->std_ਕੋਡ_ਮੰਜ਼ਿਲ_glyph (env.arg_ਢੇਰ[n-2].ਮੰਜ਼ਿਲ_int ());
    hb_codepoint_t accent = param.cff->std_ਕੋਡ_ਮੰਜ਼ਿਲ_glyph (env.arg_ਢੇਰ[n-1].ਮੰਜ਼ਿਲ_int ());

    bounds_t  ਅਧਾਰ_bounds, accent_bounds;
    ਜੇ (likely (!env.ਵਿੱਚ_seac && ਅਧਾਰ && accent
	       && _get_bounds (param.cff, ਅਧਾਰ, ਅਧਾਰ_bounds, ਸੱਚ, env.get_budget ())
	       && _get_bounds (param.cff, accent, accent_bounds, ਸੱਚ, env.get_budget ())))
    {
      param.bounds.ਮਿਲਾਓ (ਅਧਾਰ_bounds);
      accent_bounds.ਵਿੱਥ (delta);
      param.bounds.ਮਿਲਾਓ (accent_bounds);
    }
    ਨਹੀਂਤਾਂ
      env.ਸੈੱਟ_error ();
  }
};

ਤਾਰਕਿਕ _get_bounds (ਅਚਲ OT::cff1::accelerator_t *cff, hb_codepoint_t glyph, bounds_t &bounds, ਤਾਰਕਿਕ ਵਿੱਚ_seac, int64_t *budget)
{
  bounds.init ();
  ਜੇ (unlikely (!cff->is_valid () || (glyph >= cff->ਅੰਕ_glyphs))) ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = cff->fdSelect->get_fd (glyph);
  ਅਚਲ hb_ubytes_t str = (*cff->char_ਲੜੀਆਂ)[glyph];
  cff1_cs_interp_env_t env (str, *cff, fd);
  env.ਸੈੱਟ_ਵਿੱਚ_seac (ਵਿੱਚ_seac);
  cff1_cs_interpreter_t<cff1_cs_opset_extents_t, cff1_extents_param_t> interp (env);
  cff1_extents_param_t param (cff);
  ਜੇ (unlikely (!interp.interpret (param, budget))) ਵਾਪਸ ਝੂਠ;
  bounds = param.bounds;
  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ OT::cff1::accelerator_t::get_extents (hb_font_t *font, hb_codepoint_t glyph, hb_glyph_extents_t *extents, int64_t *budget) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ

  bounds_t bounds;

  int64_t ਢੇਰ_budget = HB_BUDGET_GLYPH;
  ਜੇ (!budget) budget = &ਢੇਰ_budget;

  ਜੇ (!_get_bounds (ਇਹ, glyph, bounds, ਝੂਠ, budget))
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (bounds.min.x >= bounds.max.x)
  {
    extents->ਚੌੜਾਈ = 0;
    extents->x_bearing = 0;
  }
  ਨਹੀਂਤਾਂ
  {
    ਦੁੱਗਣਾ x_bearing = roundf (bounds.min.x.ਮੰਜ਼ਿਲ_real ());
    extents->x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (x_bearing);
    extents->ਚੌੜਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) roundf (bounds.max.x.ਮੰਜ਼ਿਲ_real ()) - x_bearing);
  }
  ਜੇ (bounds.min.y >= bounds.max.y)
  {
    extents->ਉਚਾਈ = 0;
    extents->y_bearing = 0;
  }
  ਨਹੀਂਤਾਂ
  {
    ਦੁੱਗਣਾ y_bearing = roundf (bounds.max.y.ਮੰਜ਼ਿਲ_real ());
    extents->y_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (y_bearing);
    extents->ਉਚਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) roundf (bounds.min.y.ਮੰਜ਼ਿਲ_real ()) - y_bearing);
  }

  font->scale_glyph_extents (extents);

  ਵਾਪਸ ਸੱਚ;
}

ਬਣਤਰ cff1_ਰਸਤਾ_param_t
{
  cff1_ਰਸਤਾ_param_t (ਅਚਲ OT::cff1::accelerator_t *cff_, hb_font_t *font_,
		     hb_draw_session_t &draw_session_, ਬਿੰਦੂ_t *delta_)
  {
    draw_session = &draw_session_;
    cff = cff_;
    font = font_;
    delta = delta_;
  }

  ਖਾਲੀ move_ਮੰਜ਼ਿਲ (ਅਚਲ ਬਿੰਦੂ_t &p)
  {
    ਬਿੰਦੂ_t ਬਿੰਦੂ = p;
    ਜੇ (delta) ਬਿੰਦੂ.move (*delta);
    draw_session->move_ਮੰਜ਼ਿਲ (font->em_fscalef_x (ਬਿੰਦੂ.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (ਬਿੰਦੂ.y.ਮੰਜ਼ਿਲ_real ()));
  }

  ਖਾਲੀ ਸਤਰ_ਮੰਜ਼ਿਲ (ਅਚਲ ਬਿੰਦੂ_t &p)
  {
    ਬਿੰਦੂ_t ਬਿੰਦੂ = p;
    ਜੇ (delta) ਬਿੰਦੂ.move (*delta);
    draw_session->ਸਤਰ_ਮੰਜ਼ਿਲ (font->em_fscalef_x (ਬਿੰਦੂ.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (ਬਿੰਦੂ.y.ਮੰਜ਼ਿਲ_real ()));
  }

  ਖਾਲੀ cubic_ਮੰਜ਼ਿਲ (ਅਚਲ ਬਿੰਦੂ_t &p1, ਅਚਲ ਬਿੰਦੂ_t &p2, ਅਚਲ ਬਿੰਦੂ_t &p3)
  {
    ਬਿੰਦੂ_t point1 = p1, point2 = p2, point3 = p3;
    ਜੇ (delta)
    {
      point1.move (*delta);
      point2.move (*delta);
      point3.move (*delta);
    }
    draw_session->cubic_ਮੰਜ਼ਿਲ (font->em_fscalef_x (point1.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (point1.y.ਮੰਜ਼ਿਲ_real ()),
			   font->em_fscalef_x (point2.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (point2.y.ਮੰਜ਼ਿਲ_real ()),
			   font->em_fscalef_x (point3.x.ਮੰਜ਼ਿਲ_real ()), font->em_fscalef_y (point3.y.ਮੰਜ਼ਿਲ_real ()));
  }

  ਖਾਲੀ ਸਿਰਾ_ਰਸਤਾ () { draw_session->close_ਰਸਤਾ (); }

  hb_font_t *font;
  hb_draw_session_t *draw_session;
  ਬਿੰਦੂ_t *delta;

  ਅਚਲ OT::cff1::accelerator_t *cff;
};

ਬਣਤਰ cff1_ਰਸਤਾ_procs_ਰਸਤਾ_t : ਰਸਤਾ_procs_t<cff1_ਰਸਤਾ_procs_ਰਸਤਾ_t, cff1_cs_interp_env_t, cff1_ਰਸਤਾ_param_t>
{
  ਸਥਿਰ ਖਾਲੀ moveto (cff1_cs_interp_env_t &env, cff1_ਰਸਤਾ_param_t& param, ਅਚਲ ਬਿੰਦੂ_t &pt)
  {
    param.move_ਮੰਜ਼ਿਲ (pt);
    env.moveto (pt);
  }

  ਸਥਿਰ ਖਾਲੀ ਸਤਰ (cff1_cs_interp_env_t &env, cff1_ਰਸਤਾ_param_t &param, ਅਚਲ ਬਿੰਦੂ_t &pt1)
  {
    param.ਸਤਰ_ਮੰਜ਼ਿਲ (pt1);
    env.moveto (pt1);
  }

  ਸਥਿਰ ਖਾਲੀ ਵਕਰ (cff1_cs_interp_env_t &env, cff1_ਰਸਤਾ_param_t &param, ਅਚਲ ਬਿੰਦੂ_t &pt1, ਅਚਲ ਬਿੰਦੂ_t &pt2, ਅਚਲ ਬਿੰਦੂ_t &pt3)
  {
    param.cubic_ਮੰਜ਼ਿਲ (pt1, pt2, pt3);
    env.moveto (pt3);
  }
};

ਸਥਿਰ ਤਾਰਕਿਕ get_ਰਸਤਾ_2 (ਅਚਲ OT::cff1::accelerator_t *cff, hb_font_t *font, hb_codepoint_t glyph,
		       hb_draw_session_t &draw_session, ਤਾਰਕਿਕ ਵਿੱਚ_seac = ਝੂਠ, ਬਿੰਦੂ_t *delta = ਖਾਲੀ_ਸੂਚਕ,
		       int64_t *budget = ਖਾਲੀ_ਸੂਚਕ);

ਬਣਤਰ cff1_cs_opset_ਰਸਤਾ_t : cff1_cs_opset_t<cff1_cs_opset_ਰਸਤਾ_t, cff1_ਰਸਤਾ_param_t, cff1_ਰਸਤਾ_procs_ਰਸਤਾ_t>
{
  ਸਥਿਰ ਖਾਲੀ process_seac (cff1_cs_interp_env_t &env, cff1_ਰਸਤਾ_param_t& param)
  {
    /* End previous path */
    param.ਸਿਰਾ_ਰਸਤਾ ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ n = env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();
    ਬਿੰਦੂ_t delta;
    delta.x = env.arg_ਢੇਰ[n-4];
    delta.y = env.arg_ਢੇਰ[n-3];
    hb_codepoint_t ਅਧਾਰ = param.cff->std_ਕੋਡ_ਮੰਜ਼ਿਲ_glyph (env.arg_ਢੇਰ[n-2].ਮੰਜ਼ਿਲ_int ());
    hb_codepoint_t accent = param.cff->std_ਕੋਡ_ਮੰਜ਼ਿਲ_glyph (env.arg_ਢੇਰ[n-1].ਮੰਜ਼ਿਲ_int ());

    ਜੇ (unlikely (!(!env.ਵਿੱਚ_seac && ਅਧਾਰ && accent
		    && get_ਰਸਤਾ_2 (param.cff, param.font, ਅਧਾਰ, *param.draw_session, ਸੱਚ, ਖਾਲੀ_ਸੂਚਕ, env.get_budget ())
		    && get_ਰਸਤਾ_2 (param.cff, param.font, accent, *param.draw_session, ਸੱਚ, &delta, env.get_budget ()))))
      env.ਸੈੱਟ_error ();
  }
};

ਤਾਰਕਿਕ get_ਰਸਤਾ_2 (ਅਚਲ OT::cff1::accelerator_t *cff, hb_font_t *font, hb_codepoint_t glyph,
		hb_draw_session_t &draw_session, ਤਾਰਕਿਕ ਵਿੱਚ_seac, ਬਿੰਦੂ_t *delta,
		int64_t *budget)
{
  ਜੇ (unlikely (!cff->is_valid () || (glyph >= cff->ਅੰਕ_glyphs))) ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = cff->fdSelect->get_fd (glyph);
  ਅਚਲ hb_ubytes_t str = (*cff->char_ਲੜੀਆਂ)[glyph];
  cff1_cs_interp_env_t env (str, *cff, fd);
  env.ਸੈੱਟ_ਵਿੱਚ_seac (ਵਿੱਚ_seac);
  cff1_cs_interpreter_t<cff1_cs_opset_ਰਸਤਾ_t, cff1_ਰਸਤਾ_param_t> interp (env);
  cff1_ਰਸਤਾ_param_t param (cff, font, draw_session, delta);
  ਜੇ (unlikely (!interp.interpret (param, budget))) ਵਾਪਸ ਝੂਠ;

  /* Let's end the path specially since it is called inside seac also */
  param.ਸਿਰਾ_ਰਸਤਾ ();

  ਵਾਪਸ ਸੱਚ;
}

ਤਾਰਕਿਕ OT::cff1::accelerator_t::get_ਰਸਤਾ (hb_font_t *font, hb_codepoint_t glyph, hb_draw_session_t &draw_session, int64_t *budget) ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OT_FONT_CFF
  /* XXX Remove check when this code moves to .hh file. */
  ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ

  int64_t ਢੇਰ_budget = HB_BUDGET_GLYPH;
  ਜੇ (!budget) budget = &ਢੇਰ_budget;

  ਵਾਪਸ get_ਰਸਤਾ_2 (ਇਹ, font, glyph, draw_session, ਝੂਠ, ਖਾਲੀ_ਸੂਚਕ, budget);
}

ਬਣਤਰ get_seac_param_t
{
  get_seac_param_t (ਅਚਲ OT::cff1::accelerator_subset_t *_cff) : cff (_cff) {}

  ਤਾਰਕਿਕ has_seac () ਅਚਲ { ਵਾਪਸ ਅਧਾਰ && accent; }

  ਅਚਲ OT::cff1::accelerator_subset_t *cff;
  hb_codepoint_t  ਅਧਾਰ = 0;
  hb_codepoint_t  accent = 0;
};

ਬਣਤਰ cff1_cs_opset_seac_t : cff1_cs_opset_t<cff1_cs_opset_seac_t, get_seac_param_t>
{
  ਸਥਿਰ ਖਾਲੀ process_seac (cff1_cs_interp_env_t &env, get_seac_param_t& param)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  n = env.arg_ਢੇਰ.get_ਗਿਣਤੀ ();
    hb_codepoint_t  ਅਧਾਰ_char = (hb_codepoint_t)env.arg_ਢੇਰ[n-2].ਮੰਜ਼ਿਲ_int ();
    hb_codepoint_t  accent_char = (hb_codepoint_t)env.arg_ਢੇਰ[n-1].ਮੰਜ਼ਿਲ_int ();

    param.ਅਧਾਰ = param.cff->std_ਕੋਡ_ਮੰਜ਼ਿਲ_glyph (ਅਧਾਰ_char);
    param.accent = param.cff->std_ਕੋਡ_ਮੰਜ਼ਿਲ_glyph (accent_char);
  }
};

ਸਥਿਰ ਤਾਰਕਿਕ
cff1_charstring_may_ਸਿਰਾ_ਵਿੱਚ_seac (ਅਚਲ hb_ubytes_t &str)
{
  ਜੇ (unlikely (!str.ਲੰਮਾਈ || str[str.ਲੰਮਾਈ - 1] != Op_ਕੋਡ_endchar))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = str.ਲੰਮਾਈ - 1;
  ਜੇ (!ਸਿਰਾ) ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ = str[ਸਿਰਾ - 1];

  /* Type 2 numbers are not self-synchronizing when read backwards.
   * Keep interpreting if any number encoding could end at endchar. */
  ਜੇ (Op_ਕੋਡ_One_Byte_Int_ਪਹਿਲਾ <= ਆਖਰੀ && ਆਖਰੀ <= Op_ਕੋਡ_One_Byte_Int_ਆਖਰੀ)
    ਵਾਪਸ ਸੱਚ;
  ਜੇ (ਸਿਰਾ >= 2 &&
      Op_ਕੋਡ_Two_Byte_Pos_Int0 <= str[ਸਿਰਾ - 2] &&
      str[ਸਿਰਾ - 2] <= Op_ਕੋਡ_Two_Byte_Neg_Int3)
    ਵਾਪਸ ਸੱਚ;
  ਜੇ (ਸਿਰਾ >= 3 && str[ਸਿਰਾ - 3] == Op_ਕੋਡ_shortint)
    ਵਾਪਸ ਸੱਚ;
  ਜੇ (ਸਿਰਾ >= 5 && str[ਸਿਰਾ - 5] == Op_ਕੋਡ_fixedcs)
    ਵਾਪਸ ਸੱਚ;

  /* A subroutine can leave the seac operands on the shared stack. */
  ਵਾਪਸ ਆਖਰੀ == Op_ਕੋਡ_callsubr || ਆਖਰੀ == Op_ਕੋਡ_callgsubr;
}

ਤਾਰਕਿਕ OT::cff1::accelerator_subset_t::get_seac_components (hb_codepoint_t glyph, hb_codepoint_t *ਅਧਾਰ, hb_codepoint_t *accent) ਅਚਲ
{
  ਜੇ (unlikely (!is_valid () || (glyph >= ਅੰਕ_glyphs))) ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ fd = fdSelect->get_fd (glyph);
  ਅਚਲ hb_ubytes_t str = (*char_ਲੜੀਆਂ)[glyph];
  ਜੇ (unlikely (!cff1_charstring_may_ਸਿਰਾ_ਵਿੱਚ_seac (str))) ਵਾਪਸ ਝੂਠ;
  cff1_cs_interp_env_t env (str, *ਇਹ, fd);
  cff1_cs_interpreter_t<cff1_cs_opset_seac_t, get_seac_param_t> interp (env);
  get_seac_param_t  param (ਇਹ);
  ਜੇ (unlikely (!interp.interpret (param))) ਵਾਪਸ ਝੂਠ;

  ਜੇ (param.has_seac ())
  {
    *ਅਧਾਰ = param.ਅਧਾਰ;
    *accent = param.accent;
    ਵਾਪਸ ਸੱਚ;
  }
  ਵਾਪਸ ਝੂਠ;
}


#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* HB_OT_CFF1_TABLE_CC pacify */
#ਜੇ_ਅੰਤ /* HB_OT_CFF1_TABLE_CC guard */
