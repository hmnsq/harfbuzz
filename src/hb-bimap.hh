/*
 * Copyright © 2019 Adobe Inc.
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

#ifndef HB_BIMAP_HH
#define HB_BIMAP_HH

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-map.hh"

/* Bi-directional map */
ഘടന hb_bimap_t
{
  ശൂന്യം reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  ശൂന്യം alloc (unsigned എടുക്കുക)
  {
    forw_map.alloc (എടുക്കുക);
    back_map.alloc (എടുക്കുക);
  }

  തർക്കം in_error () സ്ഥിരം { തിരികെ forw_map.in_error () || back_map.in_error (); }

  ശൂന്യം set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    എങ്കിൽ (in_error ()) തിരികെ;
    എങ്കിൽ (unlikely (lhs == HB_MAP_മൂല്യം_INVALID)) തിരികെ;
    എങ്കിൽ (unlikely (rhs == HB_MAP_മൂല്യം_INVALID)) { del (lhs); തിരികെ; }

    forw_map.set (lhs, rhs);
    എങ്കിൽ (unlikely (in_error ())) തിരികെ;

    back_map.set (rhs, lhs);
    എങ്കിൽ (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) സ്ഥിരം { തിരികെ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) സ്ഥിരം { തിരികെ back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) സ്ഥിരം { തിരികെ get (lhs); }
  തർക്കം has (hb_codepoint_t lhs) സ്ഥിരം { തിരികെ forw_map.has (lhs); }


  ശൂന്യം del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  ശൂന്യം clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  തർക്കം is_empty () സ്ഥിരം { തിരികെ forw_map.is_empty (); }

  unsigned പൂർണ്ണസംഖ്യ get_population () സ്ഥിരം { തിരികെ forw_map.get_population (); }

  സംരക്ഷിത:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  പൊതു:
  auto താക്കോലുകൾ () സ്ഥിരം HB_AUTO_RETURN (+ forw_map.താക്കോലുകൾ())
  auto values () സ്ഥിരം HB_AUTO_RETURN (+ forw_map.values())
  auto iter () സ്ഥിരം HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
ഘടന hb_inc_bimap_t
{
  തർക്കം in_error () സ്ഥിരം { തിരികെ forw_map.in_error () || back_map.in_error (); }

  unsigned പൂർണ്ണസംഖ്യ get_population () സ്ഥിരം { തിരികെ forw_map.get_population (); }

  ശൂന്യം reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  ശൂന്യം alloc (unsigned എടുക്കുക)
  {
    forw_map.alloc (എടുക്കുക);
    back_map.alloc (എടുക്കുക);
  }

  ശൂന്യം clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t add (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    എങ്കിൽ (rhs == HB_MAP_മൂല്യം_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    തിരികെ rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_മൂല്യം_INVALID);
    തിരികെ start;
  }

  hb_codepoint_t skip (unsigned എണ്ണം)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + എണ്ണം);
    വേണ്ടി (unsigned i = 0; i < എണ്ണം; i++)
      back_map.push (HB_MAP_മൂല്യം_INVALID);
    തിരികെ start;
  }

  hb_codepoint_t get_next_മൂല്യം () സ്ഥിരം
  { തിരികെ back_map.length; }

  ശൂന്യം add_set (സ്ഥിരം hb_set_t *set)
  {
    വേണ്ടി (auto i : *set) add (i);
  }

  /* Create an identity map. */
  തർക്കം identity (unsigned പൂർണ്ണസംഖ്യ size)
  {
    clear ();
    വേണ്ടി (hb_codepoint_t i = 0; i < size; i++) add (i);
    തിരികെ !in_error ();
  }

  സംരക്ഷിത:
  സ്ഥിതം പൂർണ്ണസംഖ്യ cmp_id (സ്ഥിരം ശൂന്യം* a, സ്ഥിരം ശൂന്യം* b)
  { തിരികെ (പൂർണ്ണസംഖ്യ)*(സ്ഥിരം hb_codepoint_t *)a - (പൂർണ്ണസംഖ്യ)*(സ്ഥിരം hb_codepoint_t *)b; }

  പൊതു:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  ശൂന്യം sort ()
  {
    hb_codepoint_t  എണ്ണം = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    എങ്കിൽ (unlikely (!work.resize_dirty  (എണ്ണം))) തിരികെ;

    വേണ്ടി (hb_codepoint_t rhs = 0; rhs < എണ്ണം; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    വേണ്ടി (hb_codepoint_t rhs = 0; rhs < എണ്ണം; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) സ്ഥിരം { തിരികെ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) സ്ഥിരം { തിരികെ back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) സ്ഥിരം { തിരികെ get (lhs); }
  തർക്കം has (hb_codepoint_t lhs) സ്ഥിരം { തിരികെ forw_map.has (lhs); }

  സംരക്ഷിത:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  പൊതു:
  auto താക്കോലുകൾ () സ്ഥിരം HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
