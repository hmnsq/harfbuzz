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

#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "hb-open-file.hh"

#ಅನ್ತರ್ಭಾವ "hb-subset.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-accelerator.hh"

#ಅನ್ತರ್ಭಾವ "hb-ot-cmap-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-var-cvar-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-head-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-stat-table.hh"
#ಅನ್ತರ್ಭಾವ "hb-ot-post-table-v2subset.hh"


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


hb_user_data_ಕೀಲಿ_t hb_subset_accelerator_user_data_ಕೀಲಿ = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ಸ್ಥಿರ hb_tag_t known_tables[] {
  HB_TAG('a','v','a','r'),
  HB_TAG('B','A','S','E'),
  HB_TAG('C','B','D','T'),
  HB_TAG('C','B','L','C'),
  HB_TAG('C','F','F',' '),
  HB_TAG('C','F','F','2'),
  HB_TAG('c','m','a','p'),
  HB_TAG('C','O','L','R'),
  HB_TAG('C','P','A','L'),
  HB_TAG('c','v','a','r'),
  HB_TAG('c','v','t',' '),
  HB_TAG('D','S','I','G'),
  HB_TAG('E','B','D','T'),
  HB_TAG('E','B','L','C'),
  HB_TAG('E','B','S','C'),
  HB_TAG('f','p','g','m'),
  HB_TAG('f','v','a','r'),
  HB_TAG('g','a','s','p'),
  HB_TAG('G','D','E','F'),
  HB_TAG('g','l','y','f'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('G','L','Y','F'),
#endif
  HB_TAG('G','P','O','S'),
  HB_TAG('G','S','U','B'),
  HB_TAG('g','v','a','r'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('G','V','A','R'),
#endif
  HB_TAG('h','d','m','x'),
  HB_TAG('h','e','a','d'),
  HB_TAG('h','h','e','a'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('H','H','E','A'),
#endif
  HB_TAG('h','m','t','x'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('H','M','T','X'),
#endif
  HB_TAG('H','V','A','R'),
  HB_TAG('J','S','T','F'),
  HB_TAG('k','e','r','n'),
  HB_TAG('l','o','c','a'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('L','O','C','A'),
#endif
  HB_TAG('L','T','S','H'),
  HB_TAG('M','A','T','H'),
  HB_TAG('m','a','x','p'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('M','A','X','P'),
#endif
  HB_TAG('M','E','R','G'),
  HB_TAG('m','e','t','a'),
  HB_TAG('M','V','A','R'),
  HB_TAG('P','C','L','T'),
  HB_TAG('p','o','s','t'),
  HB_TAG('p','r','e','p'),
  HB_TAG('s','b','i','x'),
  HB_TAG('S','T','A','T'),
  HB_TAG('S','V','G',' '),
  HB_TAG('V','D','M','X'),
  HB_TAG('v','h','e','a'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('V','H','E','A'),
#endif
  HB_TAG('v','m','t','x'),
#ifndef HB_NO_BEYOND_64K
  HB_TAG('V','M','T','X'),
#endif
  HB_TAG('V','O','R','G'),
  HB_TAG('V','A','R','C'),
  HB_TAG('V','V','A','R'),
  HB_TAG('n','a','m','e'),
  HB_TAG('O','S','/','2')
};

ಸ್ಥಿರ ತಾರ್ಕಿಕ _table_is_empty (ಧ್ರುವ hb_face_t *face, hb_tag_t tag)
{
  hb_blob_t* blob = hb_face_reference_table (face, tag);
  ತಾರ್ಕಿಕ result = (blob == hb_blob_get_empty ());
  hb_blob_destroy (blob);
  ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ
_get_table_tags (ಧ್ರುವ hb_subset_plan_t* plan,
                 unsigned ಪೂರ್ಣಾಙ್ಕ  start_offset,
                 unsigned ಪೂರ್ಣಾಙ್ಕ *table_ಎಣಿಕೆ_2, /* IN/OUT */
                 hb_tag_t     *table_tags /* OUT */)
{
  unsigned ಅಂಕೆ_tables = hb_face_get_table_tags (plan->source, 0, nullptr, nullptr);
  ಯದಿ (ಅಂಕೆ_tables)
    ಪ್ರತ್ಯಾಗಮ hb_face_get_table_tags (plan->source, start_offset, table_ಎಣಿಕೆ_2, table_tags);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  auto it =
      hb_concat (
          + hb_array (known_tables)
          | hb_filter ([&] (hb_tag_t tag) {
            ಪ್ರತ್ಯಾಗಮ !_table_is_empty (plan->source, tag) && !plan->no_subset_tables.has (tag);
          })
          | hb_map ([] (hb_tag_t tag) -> hb_tag_t { ಪ್ರತ್ಯಾಗಮ tag; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_tag_t tag) {
            ಪ್ರತ್ಯಾಗಮ !_table_is_empty (plan->source, tag);
          }));

  it += start_offset;

  unsigned ಅಂಕೆ_written = 0;
  ಯಾವತ್ (ತಾರ್ಕಿಕ (it) && ಅಂಕೆ_written < *table_ಎಣಿಕೆ_2)
    table_tags[ಅಂಕೆ_written++] = *it++;

  *table_ಎಣಿಕೆ_2 = ಅಂಕೆ_written;
  ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_written;
}


ಸ್ಥಿರ ತಾರ್ಕಿಕ
_is_table_present (hb_face_t *source, hb_tag_t tag)
{

  ಯದಿ (!hb_face_get_table_tags (source, 0, nullptr, nullptr)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    ಪ್ರತ್ಯಾಗಮ !_table_is_empty (source, tag);
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, ಅಂಕೆ_tables = ARRAY_LENGTH (table_tags);
  ಯಾವತ್ (((ಶೂನ್ಯ) hb_face_get_table_tags (source, offset, &ಅಂಕೆ_tables, table_tags), ಅಂಕೆ_tables))
  {
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_tables; ++i)
      ಯದಿ (table_tags[i] == tag)
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    offset += ಅಂಕೆ_tables;
  }
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
_should_drop_table (hb_subset_plan_t *plan, hb_tag_t tag)
{
  ಯದಿ (plan->drop_tables.has (tag))
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

  ವಿಕಲ್ಪ (tag)
  {
  ಪಕ್ಷ HB_TAG('c','v','a','r'): /* hint table, fallthrough */
    ಪ್ರತ್ಯಾಗಮ plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  ಪಕ್ಷ HB_TAG('c','v','t',' '): /* hint table, fallthrough */
  ಪಕ್ಷ HB_TAG('f','p','g','m'): /* hint table, fallthrough */
  ಪಕ್ಷ HB_TAG('p','r','e','p'): /* hint table, fallthrough */
  ಪಕ್ಷ HB_TAG('h','d','m','x'): /* hint table, fallthrough */
  ಪಕ್ಷ HB_TAG('V','D','M','X'): /* hint table, fallthrough */
    ಪ್ರತ್ಯಾಗಮ plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#ifdef HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  ಪಕ್ಷ HB_TAG('G','D','E','F'):
  ಪಕ್ಷ HB_TAG('G','P','O','S'):
  ಪಕ್ಷ HB_TAG('G','S','U','B'):
  ಪಕ್ಷ HB_TAG('m','o','r','x'):
  ಪಕ್ಷ HB_TAG('m','o','r','t'):
  ಪಕ್ಷ HB_TAG('k','e','r','x'):
  ಪಕ್ಷ HB_TAG('k','e','r','n'):
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#endif

  ಪಕ್ಷ HB_TAG('a','v','a','r'):
  ಪಕ್ಷ HB_TAG('f','v','a','r'):
  ಪಕ್ಷ HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  ಪಕ್ಷ HB_TAG('G','V','A','R'):
#endif
  ಪಕ್ಷ HB_TAG('H','V','A','R'):
  ಪಕ್ಷ HB_TAG('V','V','A','R'):
  ಪಕ್ಷ HB_TAG('M','V','A','R'):
    ಪ್ರತ್ಯಾಗಮ plan->all_axes_pinned;

  ಸ್ವಭಾವ:
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
_dependencies_satisfied (hb_subset_plan_t *plan, hb_tag_t tag,
                         ಧ್ರುವ hb_set_t &subsetted_tags,
                         ಧ್ರುವ hb_set_t &pending_subset_tags)
{
  ತಾರ್ಕಿಕ outline_pending = pending_subset_tags.has (HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
  outline_pending |= pending_subset_tags.has (HB_TAG('G','L','Y','F'));
#endif

  ವಿಕಲ್ಪ (tag)
  {
  ಪಕ್ಷ HB_TAG('h','m','t','x'):
  ಪಕ್ಷ HB_TAG('v','m','t','x'):
  ಪಕ್ಷ HB_TAG('m','a','x','p'):
#ifndef HB_NO_BEYOND_64K
  ಪಕ್ಷ HB_TAG('H','M','T','X'):
  ಪಕ್ಷ HB_TAG('V','M','T','X'):
  ಪಕ್ಷ HB_TAG('M','A','X','P'):
#endif
  ಪಕ್ಷ HB_TAG('O','S','/','2'):
    ಪ್ರತ್ಯಾಗಮ !plan->normalized_coords || !outline_pending;
  ಪಕ್ಷ HB_TAG('G','P','O','S'):
  ಪಕ್ಷ HB_TAG('G','S','U','B'):
    ಪ್ರತ್ಯಾಗಮ plan->all_axes_pinned || !pending_subset_tags.has (HB_TAG('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  ಪಕ್ಷ HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  ಪಕ್ಷ HB_TAG('G','V','A','R'):
#endif
  ಪಕ್ಷ HB_TAG('c','v','a','r'):
  ಪಕ್ಷ HB_TAG('H','V','A','R'):
  ಪಕ್ಷ HB_TAG('V','V','A','R'):
  ಪಕ್ಷ HB_TAG('M','V','A','R'):
  ಪಕ್ಷ HB_TAG('G','D','E','F'):
  ಪಕ್ಷ HB_TAG('C','F','F','2'):
    ಪ್ರತ್ಯಾಗಮ !plan->has_avar2 || !pending_subset_tags.has (HB_TAG('a','v','a','r'));
  ಸ್ವಭಾವ:
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ
_subset_table (hb_subset_plan_t *plan,
	       hb_vector_t<ವರ್ಣ> &buf,
	       hb_tag_t tag)
{
  ಯದಿ (plan->no_subset_tables.has (tag)) {
    ಪ್ರತ್ಯಾಗಮ _hb_subset_table_passthrough (plan, tag);
  }

  DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "subset %c%c%c%c", HB_UNTAG (tag));

  ತಾರ್ಕಿಕ success;
  ಯದಿ (_hb_subset_table_layout (plan, buf, tag, &success) ||
      _hb_subset_table_var (plan, buf, tag, &success) ||
      _hb_subset_table_cff (plan, buf, tag, &success) ||
      _hb_subset_table_color (plan, buf, tag, &success) ||
      _hb_subset_table_other (plan, buf, tag, &success))
    ಪ್ರತ್ಯಾಗಮ success;


  ವಿಕಲ್ಪ (tag)
  {
  ಪಕ್ಷ HB_TAG('h','e','a','d'):
  {
    ತಾರ್ಕಿಕ outline_table_handles_head =
	_is_table_present (plan->source, HB_TAG('g','l','y','f')) &&
	!_should_drop_table (plan, HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
    outline_table_handles_head |=
	_is_table_present (plan->source, HB_TAG('G','L','Y','F')) &&
	!_should_drop_table (plan, HB_TAG('G','L','Y','F'));
#endif
    ಯದಿ (outline_table_handles_head)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* skip head, handled by the outline table */
    ಪ್ರತ್ಯಾಗಮ _hb_subset_table<ಧ್ರುವ OT::head> (plan, buf);
  }

  ಪಕ್ಷ HB_TAG('S','T','A','T'):
    ಯದಿ (!plan->user_axes_location.is_empty ()) ಪ್ರತ್ಯಾಗಮ _hb_subset_table<ಧ್ರುವ OT::STAT> (plan, buf);
    ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ _hb_subset_table_passthrough (plan, tag);

  ಪಕ್ಷ HB_TAG('c','v','t',' '):
#ifndef HB_NO_VAR
    ಯದಿ (_is_table_present (plan->source, HB_TAG('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      auto &cvar = *plan->source->table.cvar;
      ಪ್ರತ್ಯಾಗಮ OT::cvar::add_cvt_and_apply_deltas (plan, cvar.get_tuple_var_data (), &cvar);
    }
#endif
    ಪ್ರತ್ಯಾಗಮ _hb_subset_table_passthrough (plan, tag);
  }

  ಯದಿ (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    ಪ್ರತ್ಯಾಗಮ _hb_subset_table_passthrough (plan, tag);

  // Drop table
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ ಶೂನ್ಯ _attach_accelerator_data (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  ಯದಿ (!plan->inprogress_accelerator) ಪ್ರತ್ಯಾಗಮ;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = nullptr;

  ಯದಿ (accel->in_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    ಪ್ರತ್ಯಾಗಮ;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_table<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  ಯದಿ (!hb_face_set_user_data(face,
                             hb_subset_accelerator_t::user_data_ಕೀಲಿ(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             ಸತ್ಯ))
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
hb_subset_or_fail (hb_face_t *source, ಧ್ರುವ hb_subset_ಒಳಹರಿವು_t *ಒಳಹರಿವು)
{
  ಯದಿ (unlikely (!ಒಳಹರಿವು || !source)) ಪ್ರತ್ಯಾಗಮ nullptr;

  ಯದಿ (unlikely (!source->get_ಅಂಕೆ_glyphs ()))
  {
    DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "No glyphs in source font.");
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (source, ಒಳಹರಿವು);
  ಯದಿ (unlikely (!plan)) {
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  hb_face_t * result = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  ಪ್ರತ್ಯಾಗಮ result;
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
  ಯದಿ (unlikely (!plan || plan->in_error ())) {
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, ಅಂಕೆ_tables = ARRAY_LENGTH (table_tags);

  hb_set_t subsetted_tags, pending_subset_tags;
  ಯಾವತ್ (((ಶೂನ್ಯ) _get_table_tags (plan, offset, &ಅಂಕೆ_tables, table_tags), ಅಂಕೆ_tables))
  {
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_tables; ++i)
    {
      hb_tag_t tag = table_tags[i];
      ಯದಿ (_should_drop_table (plan, tag)) ಅನುವರ್ತ;
      pending_subset_tags.add (tag);
    }

    offset += ಅಂಕೆ_tables;
  }

  ತಾರ್ಕಿಕ success = ಸತ್ಯ;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_vector_t<ವರ್ಣ> buf;
    buf.alloc (8192 - 16);

    ಯಾವತ್ (!pending_subset_tags.is_empty ())
    {
      ಯದಿ (subsetted_tags.in_error ()
	  || pending_subset_tags.in_error ()) {
	success = ಅಸತ್ಯ;
	ಗಚ್ಛ end;
      }

      ತಾರ್ಕಿಕ made_changes = ಅಸತ್ಯ;
      ಪ್ರತಿ (hb_tag_t tag : pending_subset_tags)
      {
	ಯದಿ (!_dependencies_satisfied (plan, tag,
				      subsetted_tags,
				      pending_subset_tags))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  ಅನುವರ್ತ;
	}

	pending_subset_tags.del (tag);
	subsetted_tags.add (tag);
	made_changes = ಸತ್ಯ;

	success = _subset_table (plan, buf, tag);
	ಯದಿ (unlikely (!success)) ಗಚ್ಛ end;
      }

      ಯದಿ (!made_changes)
      {
	DEBUG_ಸಂದೇಶ (SUBSET, nullptr, "Table dependencies unable to be satisfied. Subset failed.");
	success = ಅಸತ್ಯ;
	ಗಚ್ಛ end;
      }
    }
  }

  ಯದಿ (success && plan->attach_accelerator_data) {
    _attach_accelerator_data (plan, plan->dest);
  }

end:
  ಪ್ರತ್ಯಾಗಮ success ? hb_face_reference (plan->dest) : nullptr;
}
