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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-map.hh"

/* Bi-directional map */
கட்டமைப்பு hb_bimap_t
{
  வெறுமை reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  வெறுமை alloc (unsigned எடு)
  {
    forw_map.alloc (எடு);
    back_map.alloc (எடு);
  }

  தருக்க in_error () மாறிலி { திரும்பு forw_map.in_error () || back_map.in_error (); }

  வெறுமை set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    எனில் (in_error ()) திரும்பு;
    எனில் (unlikely (lhs == HB_MAP_மதிப்பு_INVALID)) திரும்பு;
    எனில் (unlikely (rhs == HB_MAP_மதிப்பு_INVALID)) { del (lhs); திரும்பு; }

    forw_map.set (lhs, rhs);
    எனில் (unlikely (in_error ())) திரும்பு;

    back_map.set (rhs, lhs);
    எனில் (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) மாறிலி { திரும்பு forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) மாறிலி { திரும்பு back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) மாறிலி { திரும்பு get (lhs); }
  தருக்க has (hb_codepoint_t lhs) மாறிலி { திரும்பு forw_map.has (lhs); }


  வெறுமை del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  வெறுமை clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  தருக்க is_empty () மாறிலி { திரும்பு forw_map.is_empty (); }

  unsigned முழு get_population () மாறிலி { திரும்பு forw_map.get_population (); }

  பாதுகாக்கப்பட்ட:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  பொது:
  auto சாவிகள் () மாறிலி HB_AUTO_RETURN (+ forw_map.சாவிகள்())
  auto values () மாறிலி HB_AUTO_RETURN (+ forw_map.values())
  auto iter () மாறிலி HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
கட்டமைப்பு hb_inc_bimap_t
{
  தருக்க in_error () மாறிலி { திரும்பு forw_map.in_error () || back_map.in_error (); }

  unsigned முழு get_population () மாறிலி { திரும்பு forw_map.get_population (); }

  வெறுமை reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  வெறுமை alloc (unsigned எடு)
  {
    forw_map.alloc (எடு);
    back_map.alloc (எடு);
  }

  வெறுமை clear ()
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
    எனில் (rhs == HB_MAP_மதிப்பு_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    திரும்பு rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_மதிப்பு_INVALID);
    திரும்பு start;
  }

  hb_codepoint_t skip (unsigned மொத்தம்)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + மொத்தம்);
    க்கு (unsigned i = 0; i < மொத்தம்; i++)
      back_map.push (HB_MAP_மதிப்பு_INVALID);
    திரும்பு start;
  }

  hb_codepoint_t get_next_மதிப்பு () மாறிலி
  { திரும்பு back_map.length; }

  வெறுமை add_set (மாறிலி hb_set_t *set)
  {
    க்கு (auto i : *set) add (i);
  }

  /* Create an identity map. */
  தருக்க identity (unsigned முழு size)
  {
    clear ();
    க்கு (hb_codepoint_t i = 0; i < size; i++) add (i);
    திரும்பு !in_error ();
  }

  பாதுகாக்கப்பட்ட:
  நிலையான முழு cmp_id (மாறிலி வெறுமை* a, மாறிலி வெறுமை* b)
  { திரும்பு (முழு)*(மாறிலி hb_codepoint_t *)a - (முழு)*(மாறிலி hb_codepoint_t *)b; }

  பொது:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  வெறுமை sort ()
  {
    hb_codepoint_t  மொத்தம் = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    எனில் (unlikely (!work.resize_dirty  (மொத்தம்))) திரும்பு;

    க்கு (hb_codepoint_t rhs = 0; rhs < மொத்தம்; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    க்கு (hb_codepoint_t rhs = 0; rhs < மொத்தம்; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) மாறிலி { திரும்பு forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) மாறிலி { திரும்பு back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) மாறிலி { திரும்பு get (lhs); }
  தருக்க has (hb_codepoint_t lhs) மாறிலி { திரும்பு forw_map.has (lhs); }

  பாதுகாக்கப்பட்ட:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  பொது:
  auto சாவிகள் () மாறிலி HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
