/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Garret Rieger, Rod Sheeter, Behdad Esfahbod
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-file.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-accelerator.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cmap-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-var-cvar-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-head-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-stat-table.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-post-table-v2subset.hh"


/**
 * SECTION:hb-subset
 * @title: hb-subset
 * @short_description: Subsets font files.
 * @include: hb-subset.h
 *
 * Subsetting reduces the codepoint coverage of font files and removes all data
 * that is no longer needed. A subset input describes the desired subset. The input is
 * provided along with a font to the subsetting operation. Output is a new font file
 * containing only the data specified in the input.
 *
 * Currently most outline and bitmap tables are supported: glyf, CFF, CFF2, sbix,
 * COLR, and CBDT/CBLC. This also includes fonts with variable outlines via OpenType
 * variations. Notably EBDT/EBLC and SVG are not supported. Layout subsetting is supported
 * only for OpenType Layout tables (GSUB, GPOS, GDEF). Notably subsetting of graphite or AAT tables
 * is not yet supported.
 *
 * Fonts with graphite or AAT tables may still be subsetted but will likely need to use the
 * retain glyph ids option and configure the subset to pass through the layout tables untouched.
 */


hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_subset_accelerator_user_ਅੰਕੜੇ_ਕੁੰਜੀ = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ਸਥਿਰ hb_ਟੈਗ_t ਗਿਆਤ_tables[] {
  HB_ਟੈਗ('a','v','a','r'),
  HB_ਟੈਗ('B','A','S','E'),
  HB_ਟੈਗ('C','B','D','T'),
  HB_ਟੈਗ('C','B','L','C'),
  HB_ਟੈਗ('C','F','F',' '),
  HB_ਟੈਗ('C','F','F','2'),
  HB_ਟੈਗ('c','m','a','p'),
  HB_ਟੈਗ('C','O','L','R'),
  HB_ਟੈਗ('C','P','A','L'),
  HB_ਟੈਗ('c','v','a','r'),
  HB_ਟੈਗ('c','v','t',' '),
  HB_ਟੈਗ('D','S','I','G'),
  HB_ਟੈਗ('E','B','D','T'),
  HB_ਟੈਗ('E','B','L','C'),
  HB_ਟੈਗ('E','B','S','C'),
  HB_ਟੈਗ('f','p','g','m'),
  HB_ਟੈਗ('f','v','a','r'),
  HB_ਟੈਗ('g','a','s','p'),
  HB_ਟੈਗ('G','D','E','F'),
  HB_ਟੈਗ('g','l','y','f'),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  HB_ਟੈਗ('G','L','Y','F'),
#ਜੇ_ਅੰਤ
  HB_ਟੈਗ('G','P','O','S'),
  HB_ਟੈਗ('G','S','U','B'),
  HB_ਟੈਗ('g','v','a','r'),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  HB_ਟੈਗ('G','V','A','R'),
#ਜੇ_ਅੰਤ
  HB_ਟੈਗ('h','d','m','x'),
  HB_ਟੈਗ('h','e','a','d'),
  HB_ਟੈਗ('h','h','e','a'),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  HB_ਟੈਗ('H','H','E','A'),
#ਜੇ_ਅੰਤ
  HB_ਟੈਗ('h','m','t','x'),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  HB_ਟੈਗ('H','M','T','X'),
#ਜੇ_ਅੰਤ
  HB_ਟੈਗ('H','V','A','R'),
  HB_ਟੈਗ('J','S','T','F'),
  HB_ਟੈਗ('k','e','r','n'),
  HB_ਟੈਗ('l','o','c','a'),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  HB_ਟੈਗ('L','O','C','A'),
#ਜੇ_ਅੰਤ
  HB_ਟੈਗ('L','T','S','H'),
  HB_ਟੈਗ('M','A','T','H'),
  HB_ਟੈਗ('m','a','x','p'),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  HB_ਟੈਗ('M','A','X','P'),
#ਜੇ_ਅੰਤ
  HB_ਟੈਗ('M','E','R','G'),
  HB_ਟੈਗ('m','e','t','a'),
  HB_ਟੈਗ('M','V','A','R'),
  HB_ਟੈਗ('P','C','L','T'),
  HB_ਟੈਗ('p','o','s','t'),
  HB_ਟੈਗ('p','r','e','p'),
  HB_ਟੈਗ('s','b','i','x'),
  HB_ਟੈਗ('S','T','A','T'),
  HB_ਟੈਗ('S','V','G',' '),
  HB_ਟੈਗ('V','D','M','X'),
  HB_ਟੈਗ('v','h','e','a'),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  HB_ਟੈਗ('V','H','E','A'),
#ਜੇ_ਅੰਤ
  HB_ਟੈਗ('v','m','t','x'),
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  HB_ਟੈਗ('V','M','T','X'),
#ਜੇ_ਅੰਤ
  HB_ਟੈਗ('V','O','R','G'),
  HB_ਟੈਗ('V','A','R','C'),
  HB_ਟੈਗ('V','V','A','R'),
  HB_ਟੈਗ('n','a','m','e'),
  HB_ਟੈਗ('O','S','/','2')
};

ਸਥਿਰ ਤਾਰਕਿਕ ਸਾਰਣੀ_is_ਖਾਲੀ_ਥਾਵਾਂ (ਅਚਲ hb_face_t *face, hb_ਟੈਗ_t ਟੈਗ_2)
{
  hb_blob_t* blob = hb_face_reference_ਸਾਰਣੀ (face, ਟੈਗ_2);
  ਤਾਰਕਿਕ ਨਤੀਜਾ = (blob == hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ());
  hb_blob_destroy (blob);
  ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_2 (ਅਚਲ hb_subset_plan_t* plan,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਸ਼ੁਰੂ_ਵਿੱਥ,
                 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *ਸਾਰਣੀ_ਗਿਣਤੀ_2, /* IN/OUT */
                 hb_ਟੈਗ_t     *ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ /*ਬਾਹਰਲਾ*/)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_tables = hb_face_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (plan->ਸਰੋਤ, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  ਜੇ (ਅੰਕ_tables)
    ਵਾਪਸ hb_face_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (plan->ਸਰੋਤ, ਸ਼ੁਰੂ_ਵਿੱਥ, ਸਾਰਣੀ_ਗਿਣਤੀ_2, ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  ਸਵੈਚਾਲਿਤ it =
      hb_concat (
          + hb_array (ਗਿਆਤ_tables)
          | hb_filter ([&] (hb_ਟੈਗ_t ਟੈਗ_2) {
            ਵਾਪਸ !ਸਾਰਣੀ_is_ਖਾਲੀ_ਥਾਵਾਂ (plan->ਸਰੋਤ, ਟੈਗ_2) && !plan->no_subset_tables.has (ਟੈਗ_2);
          })
          | hb_map ([] (hb_ਟੈਗ_t ਟੈਗ_2) -> hb_ਟੈਗ_t { ਵਾਪਸ ਟੈਗ_2; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_ਟੈਗ_t ਟੈਗ_2) {
            ਵਾਪਸ !ਸਾਰਣੀ_is_ਖਾਲੀ_ਥਾਵਾਂ (plan->ਸਰੋਤ, ਟੈਗ_2);
          }));

  it += ਸ਼ੁਰੂ_ਵਿੱਥ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_written = 0;
  ਜਦੋਂ (ਤਾਰਕਿਕ (it) && ਅੰਕ_written < *ਸਾਰਣੀ_ਗਿਣਤੀ_2)
    ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[ਅੰਕ_written++] = *it++;

  *ਸਾਰਣੀ_ਗਿਣਤੀ_2 = ਅੰਕ_written;
  ਵਾਪਸ ਅੰਕ_written;
}


ਸਥਿਰ ਤਾਰਕਿਕ
is_ਸਾਰਣੀ_present (hb_face_t *ਸਰੋਤ, hb_ਟੈਗ_t ਟੈਗ_2)
{

  ਜੇ (!hb_face_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਸਰੋਤ, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    ਵਾਪਸ !ਸਾਰਣੀ_is_ਖਾਲੀ_ਥਾਵਾਂ (ਸਰੋਤ, ਟੈਗ_2);
  }

  hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[32];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = 0, ਅੰਕ_tables = ARRAY_ਲੰਮਾਈ (ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ);
  ਜਦੋਂ (((ਖਾਲੀ) hb_face_get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ (ਸਰੋਤ, ਵਿੱਥ, &ਅੰਕ_tables, ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ), ਅੰਕ_tables))
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_tables; ++i)
      ਜੇ (ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[i] == ਟੈਗ_2)
	ਵਾਪਸ ਸੱਚ;
    ਵਿੱਥ += ਅੰਕ_tables;
  }
  ਵਾਪਸ ਝੂਠ;
}

ਸਥਿਰ ਤਾਰਕਿਕ
should_drop_ਸਾਰਣੀ (hb_subset_plan_t *plan, hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਜੇ (plan->drop_tables.has (ਟੈਗ_2))
    ਵਾਪਸ ਸੱਚ;

  ਚੋਣ (ਟੈਗ_2)
  {
  ਹਾਲਤ HB_ਟੈਗ('c','v','a','r'): /* hint table, fallthrough */
    ਵਾਪਸ plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  ਹਾਲਤ HB_ਟੈਗ('c','v','t',' '): /* hint table, fallthrough */
  ਹਾਲਤ HB_ਟੈਗ('f','p','g','m'): /* hint table, fallthrough */
  ਹਾਲਤ HB_ਟੈਗ('p','r','e','p'): /* hint table, fallthrough */
  ਹਾਲਤ HB_ਟੈਗ('h','d','m','x'): /* hint table, fallthrough */
  ਹਾਲਤ HB_ਟੈਗ('V','D','M','X'): /* hint table, fallthrough */
    ਵਾਪਸ plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  ਹਾਲਤ HB_ਟੈਗ('G','D','E','F'):
  ਹਾਲਤ HB_ਟੈਗ('G','P','O','S'):
  ਹਾਲਤ HB_ਟੈਗ('G','S','U','B'):
  ਹਾਲਤ HB_ਟੈਗ('m','o','r','x'):
  ਹਾਲਤ HB_ਟੈਗ('m','o','r','t'):
  ਹਾਲਤ HB_ਟੈਗ('k','e','r','x'):
  ਹਾਲਤ HB_ਟੈਗ('k','e','r','n'):
    ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ

  ਹਾਲਤ HB_ਟੈਗ('a','v','a','r'):
  ਹਾਲਤ HB_ਟੈਗ('f','v','a','r'):
  ਹਾਲਤ HB_ਟੈਗ('g','v','a','r'):
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਹਾਲਤ HB_ਟੈਗ('G','V','A','R'):
#ਜੇ_ਅੰਤ
  ਹਾਲਤ HB_ਟੈਗ('H','V','A','R'):
  ਹਾਲਤ HB_ਟੈਗ('V','V','A','R'):
  ਹਾਲਤ HB_ਟੈਗ('M','V','A','R'):
    ਵਾਪਸ plan->all_axes_pinned;

  ਮੂਲ:
    ਵਾਪਸ ਝੂਠ;
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
_dependencies_satisfied (hb_subset_plan_t *plan, hb_ਟੈਗ_t ਟੈਗ_2,
                         ਅਚਲ hb_ਸੈੱਟ_t &subsetted_ਟੈਗ_ਸੂਚੀ,
                         ਅਚਲ hb_ਸੈੱਟ_t &pending_subset_ਟੈਗ_ਸੂਚੀ)
{
  ਤਾਰਕਿਕ outline_pending = pending_subset_ਟੈਗ_ਸੂਚੀ.has (HB_ਟੈਗ('g','l','y','f'));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  outline_pending |= pending_subset_ਟੈਗ_ਸੂਚੀ.has (HB_ਟੈਗ('G','L','Y','F'));
#ਜੇ_ਅੰਤ

  ਚੋਣ (ਟੈਗ_2)
  {
  ਹਾਲਤ HB_ਟੈਗ('h','m','t','x'):
  ਹਾਲਤ HB_ਟੈਗ('v','m','t','x'):
  ਹਾਲਤ HB_ਟੈਗ('m','a','x','p'):
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਹਾਲਤ HB_ਟੈਗ('H','M','T','X'):
  ਹਾਲਤ HB_ਟੈਗ('V','M','T','X'):
  ਹਾਲਤ HB_ਟੈਗ('M','A','X','P'):
#ਜੇ_ਅੰਤ
  ਹਾਲਤ HB_ਟੈਗ('O','S','/','2'):
    ਵਾਪਸ !plan->normalized_coords || !outline_pending;
  ਹਾਲਤ HB_ਟੈਗ('G','P','O','S'):
  ਹਾਲਤ HB_ਟੈਗ('G','S','U','B'):
    ਵਾਪਸ plan->all_axes_pinned || !pending_subset_ਟੈਗ_ਸੂਚੀ.has (HB_ਟੈਗ('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  ਹਾਲਤ HB_ਟੈਗ('g','v','a','r'):
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਹਾਲਤ HB_ਟੈਗ('G','V','A','R'):
#ਜੇ_ਅੰਤ
  ਹਾਲਤ HB_ਟੈਗ('c','v','a','r'):
  ਹਾਲਤ HB_ਟੈਗ('H','V','A','R'):
  ਹਾਲਤ HB_ਟੈਗ('V','V','A','R'):
  ਹਾਲਤ HB_ਟੈਗ('M','V','A','R'):
  ਹਾਲਤ HB_ਟੈਗ('G','D','E','F'):
  ਹਾਲਤ HB_ਟੈਗ('C','F','F','2'):
    ਵਾਪਸ !plan->has_avar2 || !pending_subset_ਟੈਗ_ਸੂਚੀ.has (HB_ਟੈਗ('a','v','a','r'));
  ਮੂਲ:
    ਵਾਪਸ ਸੱਚ;
  }
}

ਸਥਿਰ ਤਾਰਕਿਕ
subset_ਸਾਰਣੀ (hb_subset_plan_t *plan,
	       hb_ਸਦਿਸ਼_t<ਅੱਖਰ> &buf,
	       hb_ਟੈਗ_t ਟੈਗ_2)
{
  ਜੇ (plan->no_subset_tables.has (ਟੈਗ_2)) {
    ਵਾਪਸ hb_subset_ਸਾਰਣੀ_passthrough (plan, ਟੈਗ_2);
  }

  DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "subset %c%c%c%c", HB_UNTAG (ਟੈਗ_2));

  ਤਾਰਕਿਕ success;
  ਜੇ (hb_subset_ਸਾਰਣੀ_layout (plan, buf, ਟੈਗ_2, &success) ||
      hb_subset_ਸਾਰਣੀ_var (plan, buf, ਟੈਗ_2, &success) ||
      hb_subset_ਸਾਰਣੀ_cff (plan, buf, ਟੈਗ_2, &success) ||
      hb_subset_ਸਾਰਣੀ_color (plan, buf, ਟੈਗ_2, &success) ||
      hb_subset_ਸਾਰਣੀ_other (plan, buf, ਟੈਗ_2, &success))
    ਵਾਪਸ success;


  ਚੋਣ (ਟੈਗ_2)
  {
  ਹਾਲਤ HB_ਟੈਗ('h','e','a','d'):
  {
    ਤਾਰਕਿਕ outline_ਸਾਰਣੀ_handles_ਸਿਰ =
	is_ਸਾਰਣੀ_present (plan->ਸਰੋਤ, HB_ਟੈਗ('g','l','y','f')) &&
	!should_drop_ਸਾਰਣੀ (plan, HB_ਟੈਗ('g','l','y','f'));
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    outline_ਸਾਰਣੀ_handles_ਸਿਰ |=
	is_ਸਾਰਣੀ_present (plan->ਸਰੋਤ, HB_ਟੈਗ('G','L','Y','F')) &&
	!should_drop_ਸਾਰਣੀ (plan, HB_ਟੈਗ('G','L','Y','F'));
#ਜੇ_ਅੰਤ
    ਜੇ (outline_ਸਾਰਣੀ_handles_ਸਿਰ)
      ਵਾਪਸ ਸੱਚ; /* skip head, handled by the outline table */
    ਵਾਪਸ hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::ਸਿਰ_2> (plan, buf);
  }

  ਹਾਲਤ HB_ਟੈਗ('S','T','A','T'):
    ਜੇ (!plan->user_axes_location.is_empty ()) ਵਾਪਸ hb_subset_ਸਾਰਣੀ<ਅਚਲ OT::STAT> (plan, buf);
    ਨਹੀਂਤਾਂ ਵਾਪਸ hb_subset_ਸਾਰਣੀ_passthrough (plan, ਟੈਗ_2);

  ਹਾਲਤ HB_ਟੈਗ('c','v','t',' '):
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (is_ਸਾਰਣੀ_present (plan->ਸਰੋਤ, HB_ਟੈਗ('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      ਸਵੈਚਾਲਿਤ &cvar = *plan->ਸਰੋਤ->ਸਾਰਣੀ.cvar;
      ਵਾਪਸ OT::cvar::ਜੋੜੋ_cvt_and_apply_deltas (plan, cvar.get_tuple_var_ਅੰਕੜੇ (), &cvar);
    }
#ਜੇ_ਅੰਤ
    ਵਾਪਸ hb_subset_ਸਾਰਣੀ_passthrough (plan, ਟੈਗ_2);
  }

  ਜੇ (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    ਵਾਪਸ hb_subset_ਸਾਰਣੀ_passthrough (plan, ਟੈਗ_2);

  // Drop table
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ ਖਾਲੀ attach_accelerator_ਅੰਕੜੇ_2 (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  ਜੇ (!plan->inprogress_accelerator) ਵਾਪਸ;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (accel->ਵਿੱਚ_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    ਵਾਪਸ;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_ਸਾਰਣੀ<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  ਜੇ (!hb_face_ਸੈੱਟ_user_ਅੰਕੜੇ(face,
                             hb_subset_accelerator_t::user_ਅੰਕੜੇ_ਕੁੰਜੀ(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             ਸੱਚ))
    hb_subset_accelerator_t::destroy (accel);
}

/**
 * hb_subset_or_fail:
 * @source: font face data to be subset.
 * @input: input to use for the subsetting.
 *
 * Subsets a font according to provided input. Returns nullptr
 * if the subset operation fails or the face has no glyphs.
 *
 * Since: 2.9.0
 **/
hb_face_t *
hb_subset_or_fail (hb_face_t *ਸਰੋਤ, ਅਚਲ hb_subset_ਨਿਵੇਸ਼_t *ਨਿਵੇਸ਼)
{
  ਜੇ (unlikely (!ਨਿਵੇਸ਼ || !ਸਰੋਤ)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਜੇ (unlikely (!ਸਰੋਤ->get_ਅੰਕ_glyphs ()))
  {
    DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "No glyphs in source font.");
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (ਸਰੋਤ, ਨਿਵੇਸ਼);
  ਜੇ (unlikely (!plan)) {
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_face_t * ਨਤੀਜਾ = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  ਵਾਪਸ ਨਤੀਜਾ;
}


/**
 * hb_subset_plan_execute_or_fail:
 * @plan: a subsetting plan.
 *
 * Executes the provided subsetting @plan.
 *
 * Return value:
 * on success returns a reference to generated font subset. If the subsetting operation fails
 * returns nullptr.
 *
 * Since: 4.0.0
 **/
hb_face_t *
hb_subset_plan_execute_or_fail (hb_subset_plan_t *plan)
{
  ਜੇ (unlikely (!plan || plan->ਵਿੱਚ_error ())) {
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[32];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = 0, ਅੰਕ_tables = ARRAY_ਲੰਮਾਈ (ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ);

  hb_ਸੈੱਟ_t subsetted_ਟੈਗ_ਸੂਚੀ, pending_subset_ਟੈਗ_ਸੂਚੀ;
  ਜਦੋਂ (((ਖਾਲੀ) get_ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ_2 (plan, ਵਿੱਥ, &ਅੰਕ_tables, ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ), ਅੰਕ_tables))
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_tables; ++i)
    {
      hb_ਟੈਗ_t ਟੈਗ_2 = ਸਾਰਣੀ_ਟੈਗ_ਸੂਚੀ[i];
      ਜੇ (should_drop_ਸਾਰਣੀ (plan, ਟੈਗ_2)) ਜਾਰੀ;
      pending_subset_ਟੈਗ_ਸੂਚੀ.ਜੋੜੋ (ਟੈਗ_2);
    }

    ਵਿੱਥ += ਅੰਕ_tables;
  }

  ਤਾਰਕਿਕ success = ਸੱਚ;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_ਸਦਿਸ਼_t<ਅੱਖਰ> buf;
    buf.alloc (8192 - 16);

    ਜਦੋਂ (!pending_subset_ਟੈਗ_ਸੂਚੀ.is_empty ())
    {
      ਜੇ (subsetted_ਟੈਗ_ਸੂਚੀ.ਵਿੱਚ_error ()
	  || pending_subset_ਟੈਗ_ਸੂਚੀ.ਵਿੱਚ_error ()) {
	success = ਝੂਠ;
	ਜਾਓ ਸਿਰਾ;
      }

      ਤਾਰਕਿਕ made_changes = ਝੂਠ;
      ਲਈ (hb_ਟੈਗ_t ਟੈਗ_2 : pending_subset_ਟੈਗ_ਸੂਚੀ)
      {
	ਜੇ (!_dependencies_satisfied (plan, ਟੈਗ_2,
				      subsetted_ਟੈਗ_ਸੂਚੀ,
				      pending_subset_ਟੈਗ_ਸੂਚੀ))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  ਜਾਰੀ;
	}

	pending_subset_ਟੈਗ_ਸੂਚੀ.del (ਟੈਗ_2);
	subsetted_ਟੈਗ_ਸੂਚੀ.ਜੋੜੋ (ਟੈਗ_2);
	made_changes = ਸੱਚ;

	success = subset_ਸਾਰਣੀ (plan, buf, ਟੈਗ_2);
	ਜੇ (unlikely (!success)) ਜਾਓ ਸਿਰਾ;
      }

      ਜੇ (!made_changes)
      {
	DEBUG_ਸੁਨੇਹਾ (SUBSET, ਖਾਲੀ_ਸੂਚਕ, "Table dependencies unable to be satisfied. Subset failed.");
	success = ਝੂਠ;
	ਜਾਓ ਸਿਰਾ;
      }
    }
  }

  ਜੇ (success && plan->attach_accelerator_ਅੰਕੜੇ) {
    attach_accelerator_ਅੰਕੜੇ_2 (plan, plan->dest);
  }

ਸਿਰਾ:
  ਵਾਪਸ success ? hb_face_reference (plan->dest) : ਖਾਲੀ_ਸੂਚਕ;
}
