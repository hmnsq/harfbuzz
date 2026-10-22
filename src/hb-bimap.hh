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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-map.hh"

/* Bi-directional map */
संरचना hb_bimap_t
{
  शून्य reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  शून्य alloc (unsigned निकालो)
  {
    forw_map.alloc (निकालो);
    back_map.alloc (निकालो);
  }

  तार्किक in_error () ध्रुव { प्रत्यागम forw_map.in_error () || back_map.in_error (); }

  शून्य set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    यदि (in_error ()) प्रत्यागम;
    यदि (unlikely (lhs == HB_MAP_मूल्य_INVALID)) प्रत्यागम;
    यदि (unlikely (rhs == HB_MAP_मूल्य_INVALID)) { del (lhs); प्रत्यागम; }

    forw_map.set (lhs, rhs);
    यदि (unlikely (in_error ())) प्रत्यागम;

    back_map.set (rhs, lhs);
    यदि (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ध्रुव { प्रत्यागम forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ध्रुव { प्रत्यागम back_map.get (rhs); }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ध्रुव { प्रत्यागम get (lhs); }
  तार्किक has (hb_codepoint_t lhs) ध्रुव { प्रत्यागम forw_map.has (lhs); }


  शून्य del (hb_codepoint_t lhs)
  {
    back_map.del (get (lhs));
    forw_map.del (lhs);
  }

  शून्य clear ()
  {
    forw_map.clear ();
    back_map.clear ();
  }

  तार्किक is_empty () ध्रुव { प्रत्यागम forw_map.is_empty (); }

  unsigned पूर्णाङ्क get_population () ध्रुव { प्रत्यागम forw_map.get_population (); }

  रक्षित:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  सार्वजनिक:
  auto कुंजियाँ () ध्रुव HB_AUTO_RETURN (+ forw_map.कुंजियाँ())
  auto values () ध्रुव HB_AUTO_RETURN (+ forw_map.values())
  auto iter () ध्रुव HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
संरचना hb_inc_bimap_t
{
  तार्किक in_error () ध्रुव { प्रत्यागम forw_map.in_error () || back_map.in_error (); }

  unsigned पूर्णाङ्क get_population () ध्रुव { प्रत्यागम forw_map.get_population (); }

  शून्य reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  शून्य alloc (unsigned निकालो)
  {
    forw_map.alloc (निकालो);
    back_map.alloc (निकालो);
  }

  शून्य clear ()
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
    यदि (rhs == HB_MAP_मूल्य_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    प्रत्यागम rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_मूल्य_INVALID);
    प्रत्यागम start;
  }

  hb_codepoint_t skip (unsigned गिनती)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + गिनती);
    प्रति (unsigned i = 0; i < गिनती; i++)
      back_map.push (HB_MAP_मूल्य_INVALID);
    प्रत्यागम start;
  }

  hb_codepoint_t get_next_मूल्य () ध्रुव
  { प्रत्यागम back_map.length; }

  शून्य add_set (ध्रुव hb_set_t *set)
  {
    प्रति (auto i : *set) add (i);
  }

  /* Create an identity map. */
  तार्किक identity (unsigned पूर्णाङ्क size)
  {
    clear ();
    प्रति (hb_codepoint_t i = 0; i < size; i++) add (i);
    प्रत्यागम !in_error ();
  }

  रक्षित:
  स्थिर पूर्णाङ्क cmp_id (ध्रुव शून्य* a, ध्रुव शून्य* b)
  { प्रत्यागम (पूर्णाङ्क)*(ध्रुव hb_codepoint_t *)a - (पूर्णाङ्क)*(ध्रुव hb_codepoint_t *)b; }

  सार्वजनिक:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  शून्य sort ()
  {
    hb_codepoint_t  गिनती = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    यदि (unlikely (!work.resize_dirty  (गिनती))) प्रत्यागम;

    प्रति (hb_codepoint_t rhs = 0; rhs < गिनती; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.qsort (cmp_id);

    clear ();
    प्रति (hb_codepoint_t rhs = 0; rhs < गिनती; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ध्रुव { प्रत्यागम forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ध्रुव { प्रत्यागम back_map[rhs]; }

  hb_codepoint_t operator [] (hb_codepoint_t lhs) ध्रुव { प्रत्यागम get (lhs); }
  तार्किक has (hb_codepoint_t lhs) ध्रुव { प्रत्यागम forw_map.has (lhs); }

  रक्षित:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  सार्वजनिक:
  auto कुंजियाँ () ध्रुव HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
