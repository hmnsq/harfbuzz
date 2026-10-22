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

  शून्य alloc (अचिन्हित झिक)
  {
    forw_map.alloc (झिक);
    back_map.alloc (झिक);
  }

  तार्किक in_error () अचर { फर्काऊ forw_map.in_error () || back_map.in_error (); }

  शून्य set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    यदि (in_error ()) फर्काऊ;
    यदि (unlikely (lhs == HB_MAP_मान_INVALID)) फर्काऊ;
    यदि (unlikely (rhs == HB_MAP_मान_INVALID)) { del (lhs); फर्काऊ; }

    forw_map.set (lhs, rhs);
    यदि (unlikely (in_error ())) फर्काऊ;

    back_map.set (rhs, lhs);
    यदि (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) अचर { फर्काऊ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) अचर { फर्काऊ back_map.get (rhs); }

  hb_codepoint_t चालक [] (hb_codepoint_t lhs) अचर { फर्काऊ get (lhs); }
  तार्किक has (hb_codepoint_t lhs) अचर { फर्काऊ forw_map.has (lhs); }


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

  तार्किक is_empty () अचर { फर्काऊ forw_map.is_empty (); }

  अचिन्हित पूर्णाङ्क get_population () अचर { फर्काऊ forw_map.get_population (); }

  रक्षित:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  सार्वजनिक:
  स्वतः साँचोहरू () अचर HB_AUTO_RETURN (+ forw_map.साँचोहरू())
  स्वतः values () अचर HB_AUTO_RETURN (+ forw_map.values())
  स्वतः iter () अचर HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
संरचना hb_inc_bimap_t
{
  तार्किक in_error () अचर { फर्काऊ forw_map.in_error () || back_map.in_error (); }

  अचिन्हित पूर्णाङ्क get_population () अचर { फर्काऊ forw_map.get_population (); }

  शून्य reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  शून्य alloc (अचिन्हित झिक)
  {
    forw_map.alloc (झिक);
    back_map.alloc (झिक);
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
    यदि (rhs == HB_MAP_मान_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    फर्काऊ rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_मान_INVALID);
    फर्काऊ start;
  }

  hb_codepoint_t skip (अचिन्हित गन्ती)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + गन्ती);
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      back_map.push (HB_MAP_मान_INVALID);
    फर्काऊ start;
  }

  hb_codepoint_t get_next_मान () अचर
  { फर्काऊ back_map.length; }

  शून्य add_set (अचर hb_set_t *set)
  {
    लागि (स्वतः i : *set) add (i);
  }

  /* Create an identity map. */
  तार्किक identity (अचिन्हित पूर्णाङ्क size)
  {
    clear ();
    लागि (hb_codepoint_t i = 0; i < size; i++) add (i);
    फर्काऊ !in_error ();
  }

  रक्षित:
  स्थिर पूर्णाङ्क cmp_id (अचर शून्य* a, अचर शून्य* b)
  { फर्काऊ (पूर्णाङ्क)*(अचर hb_codepoint_t *)a - (पूर्णाङ्क)*(अचर hb_codepoint_t *)b; }

  सार्वजनिक:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  शून्य sort ()
  {
    hb_codepoint_t  गन्ती = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    यदि (unlikely (!work.resize_dirty  (गन्ती))) फर्काऊ;

    लागि (hb_codepoint_t rhs = 0; rhs < गन्ती; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.क्विक (cmp_id);

    clear ();
    लागि (hb_codepoint_t rhs = 0; rhs < गन्ती; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) अचर { फर्काऊ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) अचर { फर्काऊ back_map[rhs]; }

  hb_codepoint_t चालक [] (hb_codepoint_t lhs) अचर { फर्काऊ get (lhs); }
  तार्किक has (hb_codepoint_t lhs) अचर { फर्काऊ forw_map.has (lhs); }

  रक्षित:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  सार्वजनिक:
  स्वतः साँचोहरू () अचर HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
