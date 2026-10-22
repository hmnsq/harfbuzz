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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-open-file.hh"

#অন্তর্ভাব "hb-subset.hh"
#অন্তর্ভাব "hb-subset-table.hh"
#অন্তর্ভাব "hb-subset-accelerator.hh"

#অন্তর্ভাব "hb-ot-cmap-table.hh"
#অন্তর্ভাব "hb-ot-var-cvar-table.hh"
#অন্তর্ভাব "hb-ot-head-table.hh"
#অন্তর্ভাব "hb-ot-stat-table.hh"
#অন্তর্ভাব "hb-ot-post-table-v2subset.hh"


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


hb_user_তথ্য_চাবি_t hb_subset_accelerator_user_তথ্য_চাবি = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
স্থির hb_tag_t known_tables[] {
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

স্থির যৌক্তিক _table_is_empty (ধ্রুবক hb_face_t *face, hb_tag_t tag)
{
  hb_blob_t* blob = hb_face_reference_table (face, tag);
  যৌক্তিক ফলাফল = (blob == hb_blob_get_empty ());
  hb_blob_destroy (blob);
  ফেরত ফলাফল;
}

স্থির unsigned পূর্ণসংখ্যা
_get_table_tags (ধ্রুবক hb_subset_plan_t* plan,
                 unsigned পূর্ণসংখ্যা  start_offset,
                 unsigned পূর্ণসংখ্যা *table_গুনতি_2, /* IN/OUT */
                 hb_tag_t     *table_tags /* OUT */)
{
  unsigned অঙ্ক_tables = hb_face_get_table_tags (plan->source, 0, nullptr, nullptr);
  যদি (অঙ্ক_tables)
    ফেরত hb_face_get_table_tags (plan->source, start_offset, table_গুনতি_2, table_tags);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  auto it =
      hb_concat (
          + hb_array (known_tables)
          | hb_filter ([&] (hb_tag_t tag) {
            ফেরত !_table_is_empty (plan->source, tag) && !plan->no_subset_tables.has (tag);
          })
          | hb_map ([] (hb_tag_t tag) -> hb_tag_t { ফেরত tag; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_tag_t tag) {
            ফেরত !_table_is_empty (plan->source, tag);
          }));

  it += start_offset;

  unsigned অঙ্ক_written = 0;
  যখন (যৌক্তিক (it) && অঙ্ক_written < *table_গুনতি_2)
    table_tags[অঙ্ক_written++] = *it++;

  *table_গুনতি_2 = অঙ্ক_written;
  ফেরত অঙ্ক_written;
}


স্থির যৌক্তিক
_is_table_present (hb_face_t *source, hb_tag_t tag)
{

  যদি (!hb_face_get_table_tags (source, 0, nullptr, nullptr)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    ফেরত !_table_is_empty (source, tag);
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, অঙ্ক_tables = ARRAY_দৈর্ঘ্য (table_tags);
  যখন (((শূন্য) hb_face_get_table_tags (source, offset, &অঙ্ক_tables, table_tags), অঙ্ক_tables))
  {
    জন্য (unsigned i = 0; i < অঙ্ক_tables; ++i)
      যদি (table_tags[i] == tag)
	ফেরত সত্য;
    offset += অঙ্ক_tables;
  }
  ফেরত মিথ্যা;
}

স্থির যৌক্তিক
_should_drop_table (hb_subset_plan_t *plan, hb_tag_t tag)
{
  যদি (plan->drop_tables.has (tag))
    ফেরত সত্য;

  নির্বাচন (tag)
  {
  ক্ষেত্র HB_TAG('c','v','a','r'): /* hint table, fallthrough */
    ফেরত plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  ক্ষেত্র HB_TAG('c','v','t',' '): /* hint table, fallthrough */
  ক্ষেত্র HB_TAG('f','p','g','m'): /* hint table, fallthrough */
  ক্ষেত্র HB_TAG('p','r','e','p'): /* hint table, fallthrough */
  ক্ষেত্র HB_TAG('h','d','m','x'): /* hint table, fallthrough */
  ক্ষেত্র HB_TAG('V','D','M','X'): /* hint table, fallthrough */
    ফেরত plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#ifdef HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  ক্ষেত্র HB_TAG('G','D','E','F'):
  ক্ষেত্র HB_TAG('G','P','O','S'):
  ক্ষেত্র HB_TAG('G','S','U','B'):
  ক্ষেত্র HB_TAG('m','o','r','x'):
  ক্ষেত্র HB_TAG('m','o','r','t'):
  ক্ষেত্র HB_TAG('k','e','r','x'):
  ক্ষেত্র HB_TAG('k','e','r','n'):
    ফেরত সত্য;
#endif

  ক্ষেত্র HB_TAG('a','v','a','r'):
  ক্ষেত্র HB_TAG('f','v','a','r'):
  ক্ষেত্র HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  ক্ষেত্র HB_TAG('G','V','A','R'):
#endif
  ক্ষেত্র HB_TAG('H','V','A','R'):
  ক্ষেত্র HB_TAG('V','V','A','R'):
  ক্ষেত্র HB_TAG('M','V','A','R'):
    ফেরত plan->all_axes_pinned;

  পূর্বনির্ধারিত:
    ফেরত মিথ্যা;
  }
}

স্থির যৌক্তিক
_dependencies_satisfied (hb_subset_plan_t *plan, hb_tag_t tag,
                         ধ্রুবক hb_set_t &subsetted_tags,
                         ধ্রুবক hb_set_t &pending_subset_tags)
{
  যৌক্তিক outline_pending = pending_subset_tags.has (HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
  outline_pending |= pending_subset_tags.has (HB_TAG('G','L','Y','F'));
#endif

  নির্বাচন (tag)
  {
  ক্ষেত্র HB_TAG('h','m','t','x'):
  ক্ষেত্র HB_TAG('v','m','t','x'):
  ক্ষেত্র HB_TAG('m','a','x','p'):
#ifndef HB_NO_BEYOND_64K
  ক্ষেত্র HB_TAG('H','M','T','X'):
  ক্ষেত্র HB_TAG('V','M','T','X'):
  ক্ষেত্র HB_TAG('M','A','X','P'):
#endif
  ক্ষেত্র HB_TAG('O','S','/','2'):
    ফেরত !plan->normalized_coords || !outline_pending;
  ক্ষেত্র HB_TAG('G','P','O','S'):
  ক্ষেত্র HB_TAG('G','S','U','B'):
    ফেরত plan->all_axes_pinned || !pending_subset_tags.has (HB_TAG('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  ক্ষেত্র HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  ক্ষেত্র HB_TAG('G','V','A','R'):
#endif
  ক্ষেত্র HB_TAG('c','v','a','r'):
  ক্ষেত্র HB_TAG('H','V','A','R'):
  ক্ষেত্র HB_TAG('V','V','A','R'):
  ক্ষেত্র HB_TAG('M','V','A','R'):
  ক্ষেত্র HB_TAG('G','D','E','F'):
  ক্ষেত্র HB_TAG('C','F','F','2'):
    ফেরত !plan->has_avar2 || !pending_subset_tags.has (HB_TAG('a','v','a','r'));
  পূর্বনির্ধারিত:
    ফেরত সত্য;
  }
}

স্থির যৌক্তিক
_subset_table (hb_subset_plan_t *plan,
	       hb_vector_t<অক্ষর> &buf,
	       hb_tag_t tag)
{
  যদি (plan->no_subset_tables.has (tag)) {
    ফেরত _hb_subset_table_passthrough (plan, tag);
  }

  DEBUG_বার্তা (SUBSET, nullptr, "subset %c%c%c%c", HB_UNTAG (tag));

  যৌক্তিক success;
  যদি (_hb_subset_table_layout (plan, buf, tag, &success) ||
      _hb_subset_table_var (plan, buf, tag, &success) ||
      _hb_subset_table_cff (plan, buf, tag, &success) ||
      _hb_subset_table_color (plan, buf, tag, &success) ||
      _hb_subset_table_other (plan, buf, tag, &success))
    ফেরত success;


  নির্বাচন (tag)
  {
  ক্ষেত্র HB_TAG('h','e','a','d'):
  {
    যৌক্তিক outline_table_handles_শীর্ষ =
	_is_table_present (plan->source, HB_TAG('g','l','y','f')) &&
	!_should_drop_table (plan, HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
    outline_table_handles_শীর্ষ |=
	_is_table_present (plan->source, HB_TAG('G','L','Y','F')) &&
	!_should_drop_table (plan, HB_TAG('G','L','Y','F'));
#endif
    যদি (outline_table_handles_শীর্ষ)
      ফেরত সত্য; /* skip head, handled by the outline table */
    ফেরত _hb_subset_table<ধ্রুবক OT::শীর্ষ_2> (plan, buf);
  }

  ক্ষেত্র HB_TAG('S','T','A','T'):
    যদি (!plan->user_axes_location.is_empty ()) ফেরত _hb_subset_table<ধ্রুবক OT::STAT> (plan, buf);
    অন্যথা ফেরত _hb_subset_table_passthrough (plan, tag);

  ক্ষেত্র HB_TAG('c','v','t',' '):
#ifndef HB_NO_VAR
    যদি (_is_table_present (plan->source, HB_TAG('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      auto &cvar = *plan->source->table.cvar;
      ফেরত OT::cvar::add_cvt_and_apply_deltas (plan, cvar.get_tuple_var_তথ্য (), &cvar);
    }
#endif
    ফেরত _hb_subset_table_passthrough (plan, tag);
  }

  যদি (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    ফেরত _hb_subset_table_passthrough (plan, tag);

  // Drop table
  ফেরত সত্য;
}

স্থির শূন্য attach_accelerator_তথ্য_2 (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  যদি (!plan->inprogress_accelerator) ফেরত;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = nullptr;

  যদি (accel->in_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    ফেরত;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_table<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  যদি (!hb_face_set_user_তথ্য(face,
                             hb_subset_accelerator_t::user_তথ্য_চাবি(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             সত্য))
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
hb_subset_or_fail (hb_face_t *source, ধ্রুবক hb_subset_প্রবেশ_t *প্রবেশ)
{
  যদি (unlikely (!প্রবেশ || !source)) ফেরত nullptr;

  যদি (unlikely (!source->get_অঙ্ক_glyphs ()))
  {
    DEBUG_বার্তা (SUBSET, nullptr, "No glyphs in source font.");
    ফেরত nullptr;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (source, প্রবেশ);
  যদি (unlikely (!plan)) {
    ফেরত nullptr;
  }

  hb_face_t * ফলাফল = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  ফেরত ফলাফল;
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
  যদি (unlikely (!plan || plan->in_error ())) {
    ফেরত nullptr;
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, অঙ্ক_tables = ARRAY_দৈর্ঘ্য (table_tags);

  hb_set_t subsetted_tags, pending_subset_tags;
  যখন (((শূন্য) _get_table_tags (plan, offset, &অঙ্ক_tables, table_tags), অঙ্ক_tables))
  {
    জন্য (unsigned i = 0; i < অঙ্ক_tables; ++i)
    {
      hb_tag_t tag = table_tags[i];
      যদি (_should_drop_table (plan, tag)) চালু;
      pending_subset_tags.add (tag);
    }

    offset += অঙ্ক_tables;
  }

  যৌক্তিক success = সত্য;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_vector_t<অক্ষর> buf;
    buf.alloc (8192 - 16);

    যখন (!pending_subset_tags.is_empty ())
    {
      যদি (subsetted_tags.in_error ()
	  || pending_subset_tags.in_error ()) {
	success = মিথ্যা;
	যাও end;
      }

      যৌক্তিক made_changes = মিথ্যা;
      জন্য (hb_tag_t tag : pending_subset_tags)
      {
	যদি (!_dependencies_satisfied (plan, tag,
				      subsetted_tags,
				      pending_subset_tags))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  চালু;
	}

	pending_subset_tags.del (tag);
	subsetted_tags.add (tag);
	made_changes = সত্য;

	success = _subset_table (plan, buf, tag);
	যদি (unlikely (!success)) যাও end;
      }

      যদি (!made_changes)
      {
	DEBUG_বার্তা (SUBSET, nullptr, "Table dependencies unable to be satisfied. Subset failed.");
	success = মিথ্যা;
	যাও end;
      }
    }
  }

  যদি (success && plan->attach_accelerator_তথ্য) {
    attach_accelerator_তথ্য_2 (plan, plan->dest);
  }

end:
  ফেরত success ? hb_face_reference (plan->dest) : nullptr;
}
