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

  शून्य alloc (अचिन्हित काढा)
  {
    forw_map.alloc (काढा);
    back_map.alloc (काढा);
  }

  तार्किक in_error () स्थिरांक { परत forw_map.in_error () || back_map.in_error (); }

  शून्य set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    जर (in_error ()) परत;
    जर (unlikely (lhs == HB_MAP_मूल्य_INVALID)) परत;
    जर (unlikely (rhs == HB_MAP_मूल्य_INVALID)) { del (lhs); परत; }

    forw_map.set (lhs, rhs);
    जर (unlikely (in_error ())) परत;

    back_map.set (rhs, lhs);
    जर (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) स्थिरांक { परत forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) स्थिरांक { परत back_map.get (rhs); }

  hb_codepoint_t चालक [] (hb_codepoint_t lhs) स्थिरांक { परत get (lhs); }
  तार्किक has (hb_codepoint_t lhs) स्थिरांक { परत forw_map.has (lhs); }


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

  तार्किक is_empty () स्थिरांक { परत forw_map.is_empty (); }

  अचिन्हित पूर्णांक get_population () स्थिरांक { परत forw_map.get_population (); }

  संरक्षित:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  सार्वजनिक:
  स्वतः किल्ल्या () स्थिरांक HB_AUTO_RETURN (+ forw_map.किल्ल्या())
  स्वतः values () स्थिरांक HB_AUTO_RETURN (+ forw_map.values())
  स्वतः iter () स्थिरांक HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
संरचना hb_inc_bimap_t
{
  तार्किक in_error () स्थिरांक { परत forw_map.in_error () || back_map.in_error (); }

  अचिन्हित पूर्णांक get_population () स्थिरांक { परत forw_map.get_population (); }

  शून्य reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  शून्य alloc (अचिन्हित काढा)
  {
    forw_map.alloc (काढा);
    back_map.alloc (काढा);
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
    जर (rhs == HB_MAP_मूल्य_INVALID)
    {
      rhs = back_map.length;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    परत rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.length;
    back_map.push (HB_MAP_मूल्य_INVALID);
    परत start;
  }

  hb_codepoint_t skip (अचिन्हित मोजणी)
  {
    hb_codepoint_t start = back_map.length;
    back_map.alloc (back_map.length + मोजणी);
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      back_map.push (HB_MAP_मूल्य_INVALID);
    परत start;
  }

  hb_codepoint_t get_next_मूल्य () स्थिरांक
  { परत back_map.length; }

  शून्य add_set (स्थिरांक hb_set_t *set)
  {
    साठी (स्वतः i : *set) add (i);
  }

  /* Create an identity map. */
  तार्किक identity (अचिन्हित पूर्णांक size)
  {
    clear ();
    साठी (hb_codepoint_t i = 0; i < size; i++) add (i);
    परत !in_error ();
  }

  संरक्षित:
  स्थिर पूर्णांक cmp_id (स्थिरांक शून्य* a, स्थिरांक शून्य* b)
  { परत (पूर्णांक)*(स्थिरांक hb_codepoint_t *)a - (पूर्णांक)*(स्थिरांक hb_codepoint_t *)b; }

  सार्वजनिक:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  शून्य sort ()
  {
    hb_codepoint_t  मोजणी = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    जर (unlikely (!work.resize_dirty  (मोजणी))) परत;

    साठी (hb_codepoint_t rhs = 0; rhs < मोजणी; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.क्विक (cmp_id);

    clear ();
    साठी (hb_codepoint_t rhs = 0; rhs < मोजणी; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) स्थिरांक { परत forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) स्थिरांक { परत back_map[rhs]; }

  hb_codepoint_t चालक [] (hb_codepoint_t lhs) स्थिरांक { परत get (lhs); }
  तार्किक has (hb_codepoint_t lhs) स्थिरांक { परत forw_map.has (lhs); }

  संरक्षित:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  सार्वजनिक:
  स्वतः किल्ल्या () स्थिरांक HB_AUTO_RETURN (+ back_map.iter())
};

#endif /* HB_BIMAP_HH */
