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

#ضمن "hb.hh"

#ضمن "hb-open-type.hh"
#ضمن "hb-open-file.hh"

#ضمن "hb-subset.hh"
#ضمن "hb-subset-table.hh"
#ضمن "hb-subset-accelerator.hh"

#ضمن "hb-ot-cmap-table.hh"
#ضمن "hb-ot-var-cvar-table.hh"
#ضمن "hb-ot-head-table.hh"
#ضمن "hb-ot-stat-table.hh"
#ضمن "hb-ot-post-table-v2subset.hh"


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


hb_user_data_کلیل_t hb_subset_accelerator_user_data_کلیل = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ساكن hb_tag_t known_tables[] {
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

ساكن منطقي _table_is_empty (ثابت hb_face_t *face, hb_tag_t tag)
{
  hb_blob_t* blob = hb_face_reference_table (face, tag);
  منطقي result = (blob == hb_blob_get_empty ());
  hb_blob_destroy (blob);
  أرجع result;
}

ساكن unsigned صحيح
_get_table_tags (ثابت hb_subset_plan_t* plan,
                 unsigned صحيح  start_offset,
                 unsigned صحيح *table_ژماردن_2, /* IN/OUT */
                 hb_tag_t     *table_tags /* OUT */)
{
  unsigned ژمارە_tables = hb_face_get_table_tags (plan->source, 0, nullptr, nullptr);
  إذا (ژمارە_tables)
    أرجع hb_face_get_table_tags (plan->source, start_offset, table_ژماردن_2, table_tags);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  auto it =
      hb_concat (
          + hb_array (known_tables)
          | hb_filter ([&] (hb_tag_t tag) {
            أرجع !_table_is_empty (plan->source, tag) && !plan->no_subset_tables.has (tag);
          })
          | hb_map ([] (hb_tag_t tag) -> hb_tag_t { أرجع tag; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_tag_t tag) {
            أرجع !_table_is_empty (plan->source, tag);
          }));

  it += start_offset;

  unsigned ژمارە_written = 0;
  طالما (منطقي (it) && ژمارە_written < *table_ژماردن_2)
    table_tags[ژمارە_written++] = *it++;

  *table_ژماردن_2 = ژمارە_written;
  أرجع ژمارە_written;
}


ساكن منطقي
_is_table_present (hb_face_t *source, hb_tag_t tag)
{

  إذا (!hb_face_get_table_tags (source, 0, nullptr, nullptr)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    أرجع !_table_is_empty (source, tag);
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, ژمارە_tables = ARRAY_LENGTH (table_tags);
  طالما (((فراغ) hb_face_get_table_tags (source, offset, &ژمارە_tables, table_tags), ژمارە_tables))
  {
    لكل (unsigned i = 0; i < ژمارە_tables; ++i)
      إذا (table_tags[i] == tag)
	أرجع true;
    offset += ژمارە_tables;
  }
  أرجع خطأ;
}

ساكن منطقي
_should_drop_table (hb_subset_plan_t *plan, hb_tag_t tag)
{
  إذا (plan->drop_tables.has (tag))
    أرجع true;

  بدل (tag)
  {
  حالة HB_TAG('c','v','a','r'): /* hint table, fallthrough */
    أرجع plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  حالة HB_TAG('c','v','t',' '): /* hint table, fallthrough */
  حالة HB_TAG('f','p','g','m'): /* hint table, fallthrough */
  حالة HB_TAG('p','r','e','p'): /* hint table, fallthrough */
  حالة HB_TAG('h','d','m','x'): /* hint table, fallthrough */
  حالة HB_TAG('V','D','M','X'): /* hint table, fallthrough */
    أرجع plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#ifdef HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  حالة HB_TAG('G','D','E','F'):
  حالة HB_TAG('G','P','O','S'):
  حالة HB_TAG('G','S','U','B'):
  حالة HB_TAG('m','o','r','x'):
  حالة HB_TAG('m','o','r','t'):
  حالة HB_TAG('k','e','r','x'):
  حالة HB_TAG('k','e','r','n'):
    أرجع true;
#endif

  حالة HB_TAG('a','v','a','r'):
  حالة HB_TAG('f','v','a','r'):
  حالة HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  حالة HB_TAG('G','V','A','R'):
#endif
  حالة HB_TAG('H','V','A','R'):
  حالة HB_TAG('V','V','A','R'):
  حالة HB_TAG('M','V','A','R'):
    أرجع plan->all_axes_pinned;

  افتراضي:
    أرجع خطأ;
  }
}

ساكن منطقي
_dependencies_satisfied (hb_subset_plan_t *plan, hb_tag_t tag,
                         ثابت hb_set_t &subsetted_tags,
                         ثابت hb_set_t &pending_subset_tags)
{
  منطقي outline_pending = pending_subset_tags.has (HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
  outline_pending |= pending_subset_tags.has (HB_TAG('G','L','Y','F'));
#endif

  بدل (tag)
  {
  حالة HB_TAG('h','m','t','x'):
  حالة HB_TAG('v','m','t','x'):
  حالة HB_TAG('m','a','x','p'):
#ifndef HB_NO_BEYOND_64K
  حالة HB_TAG('H','M','T','X'):
  حالة HB_TAG('V','M','T','X'):
  حالة HB_TAG('M','A','X','P'):
#endif
  حالة HB_TAG('O','S','/','2'):
    أرجع !plan->normalized_coords || !outline_pending;
  حالة HB_TAG('G','P','O','S'):
  حالة HB_TAG('G','S','U','B'):
    أرجع plan->all_axes_pinned || !pending_subset_tags.has (HB_TAG('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  حالة HB_TAG('g','v','a','r'):
#ifndef HB_NO_BEYOND_64K
  حالة HB_TAG('G','V','A','R'):
#endif
  حالة HB_TAG('c','v','a','r'):
  حالة HB_TAG('H','V','A','R'):
  حالة HB_TAG('V','V','A','R'):
  حالة HB_TAG('M','V','A','R'):
  حالة HB_TAG('G','D','E','F'):
  حالة HB_TAG('C','F','F','2'):
    أرجع !plan->has_avar2 || !pending_subset_tags.has (HB_TAG('a','v','a','r'));
  افتراضي:
    أرجع true;
  }
}

ساكن منطقي
_subset_table (hb_subset_plan_t *plan,
	       hb_vector_t<حرف> &buf,
	       hb_tag_t tag)
{
  إذا (plan->no_subset_tables.has (tag)) {
    أرجع _hb_subset_table_passthrough (plan, tag);
  }

  DEBUG_پەیام (SUBSET, nullptr, "subset %c%c%c%c", HB_UNTAG (tag));

  منطقي success;
  إذا (_hb_subset_table_layout (plan, buf, tag, &success) ||
      _hb_subset_table_var (plan, buf, tag, &success) ||
      _hb_subset_table_cff (plan, buf, tag, &success) ||
      _hb_subset_table_color (plan, buf, tag, &success) ||
      _hb_subset_table_other (plan, buf, tag, &success))
    أرجع success;


  بدل (tag)
  {
  حالة HB_TAG('h','e','a','d'):
  {
    منطقي outline_table_handles_head =
	_is_table_present (plan->source, HB_TAG('g','l','y','f')) &&
	!_should_drop_table (plan, HB_TAG('g','l','y','f'));
#ifndef HB_NO_BEYOND_64K
    outline_table_handles_head |=
	_is_table_present (plan->source, HB_TAG('G','L','Y','F')) &&
	!_should_drop_table (plan, HB_TAG('G','L','Y','F'));
#endif
    إذا (outline_table_handles_head)
      أرجع true; /* skip head, handled by the outline table */
    أرجع _hb_subset_table<ثابت OT::head> (plan, buf);
  }

  حالة HB_TAG('S','T','A','T'):
    إذا (!plan->user_axes_location.is_empty ()) أرجع _hb_subset_table<ثابت OT::STAT> (plan, buf);
    وإلا أرجع _hb_subset_table_passthrough (plan, tag);

  حالة HB_TAG('c','v','t',' '):
#ifndef HB_NO_VAR
    إذا (_is_table_present (plan->source, HB_TAG('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      auto &cvar = *plan->source->table.cvar;
      أرجع OT::cvar::add_cvt_and_apply_deltas (plan, cvar.get_tuple_var_data (), &cvar);
    }
#endif
    أرجع _hb_subset_table_passthrough (plan, tag);
  }

  إذا (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    أرجع _hb_subset_table_passthrough (plan, tag);

  // Drop table
  أرجع true;
}

ساكن فراغ _attach_accelerator_data (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  إذا (!plan->inprogress_accelerator) أرجع;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = nullptr;

  إذا (accel->in_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    أرجع;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_table<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  إذا (!hb_face_set_user_data(face,
                             hb_subset_accelerator_t::user_data_کلیل(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             true))
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
hb_subset_or_fail (hb_face_t *source, ثابت hb_subset_تێخستن_t *تێخستن)
{
  إذا (unlikely (!تێخستن || !source)) أرجع nullptr;

  إذا (unlikely (!source->get_ژمارە_glyphs ()))
  {
    DEBUG_پەیام (SUBSET, nullptr, "No glyphs in source font.");
    أرجع nullptr;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (source, تێخستن);
  إذا (unlikely (!plan)) {
    أرجع nullptr;
  }

  hb_face_t * result = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  أرجع result;
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
  إذا (unlikely (!plan || plan->in_error ())) {
    أرجع nullptr;
  }

  hb_tag_t table_tags[32];
  unsigned offset = 0, ژمارە_tables = ARRAY_LENGTH (table_tags);

  hb_set_t subsetted_tags, pending_subset_tags;
  طالما (((فراغ) _get_table_tags (plan, offset, &ژمارە_tables, table_tags), ژمارە_tables))
  {
    لكل (unsigned i = 0; i < ژمارە_tables; ++i)
    {
      hb_tag_t tag = table_tags[i];
      إذا (_should_drop_table (plan, tag)) استمر;
      pending_subset_tags.add (tag);
    }

    offset += ژمارە_tables;
  }

  منطقي success = true;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_vector_t<حرف> buf;
    buf.alloc (8192 - 16);

    طالما (!pending_subset_tags.is_empty ())
    {
      إذا (subsetted_tags.in_error ()
	  || pending_subset_tags.in_error ()) {
	success = خطأ;
	اذهب end;
      }

      منطقي made_changes = خطأ;
      لكل (hb_tag_t tag : pending_subset_tags)
      {
	إذا (!_dependencies_satisfied (plan, tag,
				      subsetted_tags,
				      pending_subset_tags))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  استمر;
	}

	pending_subset_tags.del (tag);
	subsetted_tags.add (tag);
	made_changes = true;

	success = _subset_table (plan, buf, tag);
	إذا (unlikely (!success)) اذهب end;
      }

      إذا (!made_changes)
      {
	DEBUG_پەیام (SUBSET, nullptr, "Table dependencies unable to be satisfied. Subset failed.");
	success = خطأ;
	اذهب end;
      }
    }
  }

  إذا (success && plan->attach_accelerator_data) {
    _attach_accelerator_data (plan, plan->dest);
  }

end:
  أرجع success ? hb_face_reference (plan->dest) : nullptr;
}
