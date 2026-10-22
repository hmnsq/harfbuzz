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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_BIMAP_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_BIMAP_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"

/* Bi-directional map */
ਬਣਤਰ hb_bimap_t
{
  ਖਾਲੀ reset ()
  {
    forw_map.reset ();
    ਪਿੱਠ_map.reset ();
  }

  ਖਾਲੀ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੱਢੋ)
  {
    forw_map.alloc (ਕੱਢੋ);
    ਪਿੱਠ_map.alloc (ਕੱਢੋ);
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ forw_map.ਵਿੱਚ_error () || ਪਿੱਠ_map.ਵਿੱਚ_error (); }

  ਖਾਲੀ ਸੈੱਟ (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    ਜੇ (ਵਿੱਚ_error ()) ਵਾਪਸ;
    ਜੇ (unlikely (lhs == HB_MAP_ਮੁੱਲ_INVALID)) ਵਾਪਸ;
    ਜੇ (unlikely (rhs == HB_MAP_ਮੁੱਲ_INVALID)) { del (lhs); ਵਾਪਸ; }

    forw_map.ਸੈੱਟ (lhs, rhs);
    ਜੇ (unlikely (ਵਿੱਚ_error ())) ਵਾਪਸ;

    ਪਿੱਠ_map.ਸੈੱਟ (rhs, lhs);
    ਜੇ (unlikely (ਵਿੱਚ_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ਅਚਲ { ਵਾਪਸ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ਅਚਲ { ਵਾਪਸ ਪਿੱਠ_map.get (rhs); }

  hb_codepoint_t ਸੰਚਾਲਕ [] (hb_codepoint_t lhs) ਅਚਲ { ਵਾਪਸ get (lhs); }
  ਤਾਰਕਿਕ has (hb_codepoint_t lhs) ਅਚਲ { ਵਾਪਸ forw_map.has (lhs); }


  ਖਾਲੀ del (hb_codepoint_t lhs)
  {
    ਪਿੱਠ_map.del (get (lhs));
    forw_map.del (lhs);
  }

  ਖਾਲੀ clear ()
  {
    forw_map.clear ();
    ਪਿੱਠ_map.clear ();
  }

  ਤਾਰਕਿਕ is_empty () ਅਚਲ { ਵਾਪਸ forw_map.is_empty (); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_population () ਅਚਲ { ਵਾਪਸ forw_map.get_population (); }

  ਸੁਰੱਖਿਅਤ:
  hb_map_t  forw_map;
  hb_map_t  ਪਿੱਠ_map;

  ਜਨਤਕ:
  ਸਵੈਚਾਲਿਤ ਕੁੰਜੀਆਂ () ਅਚਲ HB_AUTO_RETURN (+ forw_map.ਕੁੰਜੀਆਂ())
  ਸਵੈਚਾਲਿਤ values () ਅਚਲ HB_AUTO_RETURN (+ forw_map.values())
  ਸਵੈਚਾਲਿਤ iter () ਅਚਲ HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
ਬਣਤਰ hb_inc_bimap_t
{
  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ forw_map.ਵਿੱਚ_error () || ਪਿੱਠ_map.ਵਿੱਚ_error (); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_population () ਅਚਲ { ਵਾਪਸ forw_map.get_population (); }

  ਖਾਲੀ reset ()
  {
    forw_map.reset ();
    ਪਿੱਠ_map.reset ();
  }

  ਖਾਲੀ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੱਢੋ)
  {
    forw_map.alloc (ਕੱਢੋ);
    ਪਿੱਠ_map.alloc (ਕੱਢੋ);
  }

  ਖਾਲੀ clear ()
  {
    forw_map.clear ();
    ਪਿੱਠ_map.clear ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t ਜੋੜੋ (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    ਜੇ (rhs == HB_MAP_ਮੁੱਲ_INVALID)
    {
      rhs = ਪਿੱਠ_map.ਲੰਮਾਈ;
      forw_map.ਸੈੱਟ (lhs, rhs);
      ਪਿੱਠ_map.push (lhs);
    }
    ਵਾਪਸ rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t ਸ਼ੁਰੂ = ਪਿੱਠ_map.ਲੰਮਾਈ;
    ਪਿੱਠ_map.push (HB_MAP_ਮੁੱਲ_INVALID);
    ਵਾਪਸ ਸ਼ੁਰੂ;
  }

  hb_codepoint_t skip (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  {
    hb_codepoint_t ਸ਼ੁਰੂ = ਪਿੱਠ_map.ਲੰਮਾਈ;
    ਪਿੱਠ_map.alloc (ਪਿੱਠ_map.ਲੰਮਾਈ + ਗਿਣਤੀ_2);
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; i++)
      ਪਿੱਠ_map.push (HB_MAP_ਮੁੱਲ_INVALID);
    ਵਾਪਸ ਸ਼ੁਰੂ;
  }

  hb_codepoint_t get_ਅਗਲਾ_ਮੁੱਲ () ਅਚਲ
  { ਵਾਪਸ ਪਿੱਠ_map.ਲੰਮਾਈ; }

  ਖਾਲੀ ਜੋੜੋ_ਸੈੱਟ (ਅਚਲ hb_ਸੈੱਟ_t *ਸੈੱਟ)
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ i : *ਸੈੱਟ) ਜੋੜੋ (i);
  }

  /* Create an identity map. */
  ਤਾਰਕਿਕ ਪਛਾਣ_ਤੱਤ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ)
  {
    clear ();
    ਲਈ (hb_codepoint_t i = 0; i < ਮਾਪ; i++) ਜੋੜੋ (i);
    ਵਾਪਸ !ਵਿੱਚ_error ();
  }

  ਸੁਰੱਖਿਅਤ:
  ਸਥਿਰ ਪੂਰਨ cmp_id (ਅਚਲ ਖਾਲੀ* a, ਅਚਲ ਖਾਲੀ* b)
  { ਵਾਪਸ (ਪੂਰਨ)*(ਅਚਲ hb_codepoint_t *)a - (ਪੂਰਨ)*(ਅਚਲ hb_codepoint_t *)b; }

  ਜਨਤਕ:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  ਖਾਲੀ sort ()
  {
    hb_codepoint_t  ਗਿਣਤੀ_2 = get_population ();
    hb_ਸਦਿਸ਼_t <hb_codepoint_t> work;
    ਜੇ (unlikely (!work.resize_dirty  (ਗਿਣਤੀ_2))) ਵਾਪਸ;

    ਲਈ (hb_codepoint_t rhs = 0; rhs < ਗਿਣਤੀ_2; rhs++)
      work.arrayZ[rhs] = ਪਿੱਠ_map[rhs];

    work.qsort (cmp_id);

    clear ();
    ਲਈ (hb_codepoint_t rhs = 0; rhs < ਗਿਣਤੀ_2; rhs++)
      ਜੋੜੋ (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) ਅਚਲ { ਵਾਪਸ forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) ਅਚਲ { ਵਾਪਸ ਪਿੱਠ_map[rhs]; }

  hb_codepoint_t ਸੰਚਾਲਕ [] (hb_codepoint_t lhs) ਅਚਲ { ਵਾਪਸ get (lhs); }
  ਤਾਰਕਿਕ has (hb_codepoint_t lhs) ਅਚਲ { ਵਾਪਸ forw_map.has (lhs); }

  ਸੁਰੱਖਿਅਤ:
  hb_map_t forw_map;
  hb_ਸਦਿਸ਼_t<hb_codepoint_t> ਪਿੱਠ_map;

  ਜਨਤਕ:
  ਸਵੈਚਾਲਿਤ ਕੁੰਜੀਆਂ () ਅਚਲ HB_AUTO_RETURN (+ ਪਿੱਠ_map.iter())
};

#ਜੇ_ਅੰਤ /* HB_BIMAP_HH */
