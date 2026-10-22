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

#अगर_अघोषित HB_BIMAP_HH
#घोषणा HB_BIMAP_HH

#समावेश "hb.hh"
#समावेश "hb-map.hh"

/* Bi-directional map */
काष्ठा hb_bimap_t
{
  व्योम reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  व्योम alloc (अचिन्हित निकालो)
  {
    forw_map.alloc (निकालो);
    back_map.alloc (निकालो);
  }

  bool in_error () स्थिर { वापस forw_map.in_error () || back_map.in_error (); }

  व्योम set (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    अगर (in_error ()) वापस;
    अगर (unlikely (lhs == HB_MAP_मूल्य_INVALID)) वापस;
    अगर (unlikely (rhs == HB_MAP_मूल्य_INVALID)) { del (lhs); वापस; }

    forw_map.set (lhs, rhs);
    अगर (unlikely (in_error ())) वापस;

    back_map.set (rhs, lhs);
    अगर (unlikely (in_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t लो (hb_codepoint_t lhs) स्थिर { वापस forw_map.लो (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) स्थिर { वापस back_map.लो (rhs); }

  hb_codepoint_t चालक [] (hb_codepoint_t lhs) स्थिर { वापस लो (lhs); }
  bool has (hb_codepoint_t lhs) स्थिर { वापस forw_map.has (lhs); }


  व्योम del (hb_codepoint_t lhs)
  {
    back_map.del (लो (lhs));
    forw_map.del (lhs);
  }

  व्योम सफा ()
  {
    forw_map.सफा ();
    back_map.सफा ();
  }

  bool is_empty () स्थिर { वापस forw_map.is_empty (); }

  अचिन्हित पूर्णांक get_population () स्थिर { वापस forw_map.get_population (); }

  रक्षित:
  hb_map_t  forw_map;
  hb_map_t  back_map;

  खुला:
  स्वतः कुंजियाँ () स्थिर HB_AUTO_RETURN (+ forw_map.कुंजियाँ())
  स्वतः values () स्थिर HB_AUTO_RETURN (+ forw_map.values())
  स्वतः iter () स्थिर HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
काष्ठा hb_inc_bimap_t
{
  bool in_error () स्थिर { वापस forw_map.in_error () || back_map.in_error (); }

  अचिन्हित पूर्णांक get_population () स्थिर { वापस forw_map.get_population (); }

  व्योम reset ()
  {
    forw_map.reset ();
    back_map.reset ();
  }

  व्योम alloc (अचिन्हित निकालो)
  {
    forw_map.alloc (निकालो);
    back_map.alloc (निकालो);
  }

  व्योम सफा ()
  {
    forw_map.सफा ();
    back_map.सफा ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t add (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    अगर (rhs == HB_MAP_मूल्य_INVALID)
    {
      rhs = back_map.लंबाई;
      forw_map.set (lhs, rhs);
      back_map.push (lhs);
    }
    वापस rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t start = back_map.लंबाई;
    back_map.push (HB_MAP_मूल्य_INVALID);
    वापस start;
  }

  hb_codepoint_t skip (अचिन्हित गिनती)
  {
    hb_codepoint_t start = back_map.लंबाई;
    back_map.alloc (back_map.लंबाई + गिनती);
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      back_map.push (HB_MAP_मूल्य_INVALID);
    वापस start;
  }

  hb_codepoint_t get_अग्र_मूल्य () स्थिर
  { वापस back_map.लंबाई; }

  व्योम add_set (स्थिर hb_set_t *set)
  {
    क्रम (स्वतः i : *set) add (i);
  }

  /* Create an identity map. */
  bool identity (अचिन्हित पूर्णांक आकार)
  {
    सफा ();
    क्रम (hb_codepoint_t i = 0; i < आकार; i++) add (i);
    वापस !in_error ();
  }

  रक्षित:
  अटल पूर्णांक cmp_id (स्थिर व्योम* a, स्थिर व्योम* b)
  { वापस (पूर्णांक)*(स्थिर hb_codepoint_t *)a - (पूर्णांक)*(स्थिर hb_codepoint_t *)b; }

  खुला:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  व्योम sort ()
  {
    hb_codepoint_t  गिनती = get_population ();
    hb_vector_t <hb_codepoint_t> work;
    अगर (unlikely (!work.resize_dirty  (गिनती))) वापस;

    क्रम (hb_codepoint_t rhs = 0; rhs < गिनती; rhs++)
      work.arrayZ[rhs] = back_map[rhs];

    work.क्विक (cmp_id);

    सफा ();
    क्रम (hb_codepoint_t rhs = 0; rhs < गिनती; rhs++)
      add (work.arrayZ[rhs]);
  }

  hb_codepoint_t लो (hb_codepoint_t lhs) स्थिर { वापस forw_map.लो (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) स्थिर { वापस back_map[rhs]; }

  hb_codepoint_t चालक [] (hb_codepoint_t lhs) स्थिर { वापस लो (lhs); }
  bool has (hb_codepoint_t lhs) स्थिर { वापस forw_map.has (lhs); }

  रक्षित:
  hb_map_t forw_map;
  hb_vector_t<hb_codepoint_t> back_map;

  खुला:
  स्वतः कुंजियाँ () स्थिर HB_AUTO_RETURN (+ back_map.iter())
};

#पूर्ण_अगर /* HB_BIMAP_HH */
