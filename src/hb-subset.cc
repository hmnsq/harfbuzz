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

#شامل "hb.hh"

#شامل "hb-open-type.hh"
#شامل "hb-open-file.hh"

#شامل "hb-subset.hh"
#شامل "hb-subset-table.hh"
#شامل "hb-subset-accelerator.hh"

#شامل "hb-ot-cmap-table.hh"
#شامل "hb-ot-var-cvar-table.hh"
#شامل "hb-ot-head-table.hh"
#شامل "hb-ot-stat-table.hh"
#شامل "hb-ot-post-table-v2subset.hh"


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


hb_user_ڈیٹا_کلید_t hb_subset_accelerator_user_ڈیٹا_کلید = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ساکن hb_لیبل_t معلوم_tables[] {
  HB_لیبل('a','v','a','r'),
  HB_لیبل('B','A','S','E'),
  HB_لیبل('C','B','D','T'),
  HB_لیبل('C','B','L','C'),
  HB_لیبل('C','F','F',' '),
  HB_لیبل('C','F','F','2'),
  HB_لیبل('c','m','a','p'),
  HB_لیبل('C','O','L','R'),
  HB_لیبل('C','P','A','L'),
  HB_لیبل('c','v','a','r'),
  HB_لیبل('c','v','t',' '),
  HB_لیبل('D','S','I','G'),
  HB_لیبل('E','B','D','T'),
  HB_لیبل('E','B','L','C'),
  HB_لیبل('E','B','S','C'),
  HB_لیبل('f','p','g','m'),
  HB_لیبل('f','v','a','r'),
  HB_لیبل('g','a','s','p'),
  HB_لیبل('G','D','E','F'),
  HB_لیبل('g','l','y','f'),
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  HB_لیبل('G','L','Y','F'),
#اگر_ختم
  HB_لیبل('G','P','O','S'),
  HB_لیبل('G','S','U','B'),
  HB_لیبل('g','v','a','r'),
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  HB_لیبل('G','V','A','R'),
#اگر_ختم
  HB_لیبل('h','d','m','x'),
  HB_لیبل('h','e','a','d'),
  HB_لیبل('h','h','e','a'),
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  HB_لیبل('H','H','E','A'),
#اگر_ختم
  HB_لیبل('h','m','t','x'),
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  HB_لیبل('H','M','T','X'),
#اگر_ختم
  HB_لیبل('H','V','A','R'),
  HB_لیبل('J','S','T','F'),
  HB_لیبل('k','e','r','n'),
  HB_لیبل('l','o','c','a'),
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  HB_لیبل('L','O','C','A'),
#اگر_ختم
  HB_لیبل('L','T','S','H'),
  HB_لیبل('M','A','T','H'),
  HB_لیبل('m','a','x','p'),
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  HB_لیبل('M','A','X','P'),
#اگر_ختم
  HB_لیبل('M','E','R','G'),
  HB_لیبل('m','e','t','a'),
  HB_لیبل('M','V','A','R'),
  HB_لیبل('P','C','L','T'),
  HB_لیبل('p','o','s','t'),
  HB_لیبل('p','r','e','p'),
  HB_لیبل('s','b','i','x'),
  HB_لیبل('S','T','A','T'),
  HB_لیبل('S','V','G',' '),
  HB_لیبل('V','D','M','X'),
  HB_لیبل('v','h','e','a'),
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  HB_لیبل('V','H','E','A'),
#اگر_ختم
  HB_لیبل('v','m','t','x'),
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  HB_لیبل('V','M','T','X'),
#اگر_ختم
  HB_لیبل('V','O','R','G'),
  HB_لیبل('V','A','R','C'),
  HB_لیبل('V','V','A','R'),
  HB_لیبل('n','a','m','e'),
  HB_لیبل('O','S','/','2')
};

ساکن منطقی جدول_is_خالی_خانے (ثابت hb_face_t *face, hb_لیبل_t لیبل_2)
{
  hb_blob_t* blob = hb_face_reference_جدول (face, لیبل_2);
  منطقی نتیجہ = (blob == hb_blob_get_خالی_خانے ());
  hb_blob_destroy (blob);
  واپس نتیجہ;
}

ساکن غیرعلامتی صحیح
get_جدول_لیبل_فہرست_2 (ثابت hb_subset_plan_t* plan,
                 غیرعلامتی صحیح  آغاز_انحراف,
                 غیرعلامتی صحیح *جدول_گنتی_2, /* IN/OUT */
                 hb_لیبل_t     *جدول_لیبل_فہرست /*برآمد*/)
{
  غیرعلامتی عدد_tables = hb_face_get_جدول_لیبل_فہرست (plan->ماخذ, 0, خالی_اشاریہ, خالی_اشاریہ);
  اگر (عدد_tables)
    واپس hb_face_get_جدول_لیبل_فہرست (plan->ماخذ, آغاز_انحراف, جدول_گنتی_2, جدول_لیبل_فہرست);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  خودکار it =
      hb_concat (
          + hb_array (معلوم_tables)
          | hb_filter ([&] (hb_لیبل_t لیبل_2) {
            واپس !جدول_is_خالی_خانے (plan->ماخذ, لیبل_2) && !plan->no_subset_tables.has (لیبل_2);
          })
          | hb_map ([] (hb_لیبل_t لیبل_2) -> hb_لیبل_t { واپس لیبل_2; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_لیبل_t لیبل_2) {
            واپس !جدول_is_خالی_خانے (plan->ماخذ, لیبل_2);
          }));

  it += آغاز_انحراف;

  غیرعلامتی عدد_written = 0;
  جب (منطقی (it) && عدد_written < *جدول_گنتی_2)
    جدول_لیبل_فہرست[عدد_written++] = *it++;

  *جدول_گنتی_2 = عدد_written;
  واپس عدد_written;
}


ساکن منطقی
is_جدول_present (hb_face_t *ماخذ, hb_لیبل_t لیبل_2)
{

  اگر (!hb_face_get_جدول_لیبل_فہرست (ماخذ, 0, خالی_اشاریہ, خالی_اشاریہ)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    واپس !جدول_is_خالی_خانے (ماخذ, لیبل_2);
  }

  hb_لیبل_t جدول_لیبل_فہرست[32];
  غیرعلامتی انحراف = 0, عدد_tables = ARRAY_لمبائی (جدول_لیبل_فہرست);
  جب (((خالی) hb_face_get_جدول_لیبل_فہرست (ماخذ, انحراف, &عدد_tables, جدول_لیبل_فہرست), عدد_tables))
  {
    برائے (غیرعلامتی i = 0; i < عدد_tables; ++i)
      اگر (جدول_لیبل_فہرست[i] == لیبل_2)
	واپس سچ;
    انحراف += عدد_tables;
  }
  واپس جھوٹ;
}

ساکن منطقی
should_drop_جدول (hb_subset_plan_t *plan, hb_لیبل_t لیبل_2)
{
  اگر (plan->drop_tables.has (لیبل_2))
    واپس سچ;

  انتخاب (لیبل_2)
  {
  حالت HB_لیبل('c','v','a','r'): /* hint table, fallthrough */
    واپس plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  حالت HB_لیبل('c','v','t',' '): /* hint table, fallthrough */
  حالت HB_لیبل('f','p','g','m'): /* hint table, fallthrough */
  حالت HB_لیبل('p','r','e','p'): /* hint table, fallthrough */
  حالت HB_لیبل('h','d','m','x'): /* hint table, fallthrough */
  حالت HB_لیبل('V','D','M','X'): /* hint table, fallthrough */
    واپس plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#اگر_تعریف_شدہ HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  حالت HB_لیبل('G','D','E','F'):
  حالت HB_لیبل('G','P','O','S'):
  حالت HB_لیبل('G','S','U','B'):
  حالت HB_لیبل('m','o','r','x'):
  حالت HB_لیبل('m','o','r','t'):
  حالت HB_لیبل('k','e','r','x'):
  حالت HB_لیبل('k','e','r','n'):
    واپس سچ;
#اگر_ختم

  حالت HB_لیبل('a','v','a','r'):
  حالت HB_لیبل('f','v','a','r'):
  حالت HB_لیبل('g','v','a','r'):
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  حالت HB_لیبل('G','V','A','R'):
#اگر_ختم
  حالت HB_لیبل('H','V','A','R'):
  حالت HB_لیبل('V','V','A','R'):
  حالت HB_لیبل('M','V','A','R'):
    واپس plan->all_axes_pinned;

  طےشدہ:
    واپس جھوٹ;
  }
}

ساکن منطقی
_dependencies_satisfied (hb_subset_plan_t *plan, hb_لیبل_t لیبل_2,
                         ثابت hb_سیٹ_t &subsetted_لیبل_فہرست,
                         ثابت hb_سیٹ_t &pending_subset_لیبل_فہرست)
{
  منطقی outline_pending = pending_subset_لیبل_فہرست.has (HB_لیبل('g','l','y','f'));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  outline_pending |= pending_subset_لیبل_فہرست.has (HB_لیبل('G','L','Y','F'));
#اگر_ختم

  انتخاب (لیبل_2)
  {
  حالت HB_لیبل('h','m','t','x'):
  حالت HB_لیبل('v','m','t','x'):
  حالت HB_لیبل('m','a','x','p'):
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  حالت HB_لیبل('H','M','T','X'):
  حالت HB_لیبل('V','M','T','X'):
  حالت HB_لیبل('M','A','X','P'):
#اگر_ختم
  حالت HB_لیبل('O','S','/','2'):
    واپس !plan->normalized_coords || !outline_pending;
  حالت HB_لیبل('G','P','O','S'):
  حالت HB_لیبل('G','S','U','B'):
    واپس plan->all_axes_pinned || !pending_subset_لیبل_فہرست.has (HB_لیبل('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  حالت HB_لیبل('g','v','a','r'):
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  حالت HB_لیبل('G','V','A','R'):
#اگر_ختم
  حالت HB_لیبل('c','v','a','r'):
  حالت HB_لیبل('H','V','A','R'):
  حالت HB_لیبل('V','V','A','R'):
  حالت HB_لیبل('M','V','A','R'):
  حالت HB_لیبل('G','D','E','F'):
  حالت HB_لیبل('C','F','F','2'):
    واپس !plan->has_avar2 || !pending_subset_لیبل_فہرست.has (HB_لیبل('a','v','a','r'));
  طےشدہ:
    واپس سچ;
  }
}

ساکن منطقی
subset_جدول (hb_subset_plan_t *plan,
	       hb_سمتیہ_t<حرف> &buf,
	       hb_لیبل_t لیبل_2)
{
  اگر (plan->no_subset_tables.has (لیبل_2)) {
    واپس hb_subset_جدول_passthrough (plan, لیبل_2);
  }

  DEBUG_پیغام (SUBSET, خالی_اشاریہ, "subset %c%c%c%c", HB_UNTAG (لیبل_2));

  منطقی success;
  اگر (hb_subset_جدول_layout (plan, buf, لیبل_2, &success) ||
      hb_subset_جدول_var (plan, buf, لیبل_2, &success) ||
      hb_subset_جدول_cff (plan, buf, لیبل_2, &success) ||
      hb_subset_جدول_color (plan, buf, لیبل_2, &success) ||
      hb_subset_جدول_other (plan, buf, لیبل_2, &success))
    واپس success;


  انتخاب (لیبل_2)
  {
  حالت HB_لیبل('h','e','a','d'):
  {
    منطقی outline_جدول_handles_سر =
	is_جدول_present (plan->ماخذ, HB_لیبل('g','l','y','f')) &&
	!should_drop_جدول (plan, HB_لیبل('g','l','y','f'));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    outline_جدول_handles_سر |=
	is_جدول_present (plan->ماخذ, HB_لیبل('G','L','Y','F')) &&
	!should_drop_جدول (plan, HB_لیبل('G','L','Y','F'));
#اگر_ختم
    اگر (outline_جدول_handles_سر)
      واپس سچ; /* skip head, handled by the outline table */
    واپس hb_subset_جدول<ثابت OT::سر_2> (plan, buf);
  }

  حالت HB_لیبل('S','T','A','T'):
    اگر (!plan->user_axes_location.is_empty ()) واپس hb_subset_جدول<ثابت OT::STAT> (plan, buf);
    ورنہ واپس hb_subset_جدول_passthrough (plan, لیبل_2);

  حالت HB_لیبل('c','v','t',' '):
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (is_جدول_present (plan->ماخذ, HB_لیبل('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      خودکار &cvar = *plan->ماخذ->جدول.cvar;
      واپس OT::cvar::جوڑو_cvt_and_apply_deltas (plan, cvar.get_tuple_var_ڈیٹا (), &cvar);
    }
#اگر_ختم
    واپس hb_subset_جدول_passthrough (plan, لیبل_2);
  }

  اگر (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    واپس hb_subset_جدول_passthrough (plan, لیبل_2);

  // Drop table
  واپس سچ;
}

ساکن خالی attach_accelerator_ڈیٹا_2 (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  اگر (!plan->inprogress_accelerator) واپس;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = خالی_اشاریہ;

  اگر (accel->اندر_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    واپس;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_جدول<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  اگر (!hb_face_سیٹ_user_ڈیٹا(face,
                             hb_subset_accelerator_t::user_ڈیٹا_کلید(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             سچ))
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
hb_subset_or_fail (hb_face_t *ماخذ, ثابت hb_subset_داخلہ_t *داخلہ)
{
  اگر (unlikely (!داخلہ || !ماخذ)) واپس خالی_اشاریہ;

  اگر (unlikely (!ماخذ->get_عدد_glyphs ()))
  {
    DEBUG_پیغام (SUBSET, خالی_اشاریہ, "No glyphs in source font.");
    واپس خالی_اشاریہ;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (ماخذ, داخلہ);
  اگر (unlikely (!plan)) {
    واپس خالی_اشاریہ;
  }

  hb_face_t * نتیجہ = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  واپس نتیجہ;
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
  اگر (unlikely (!plan || plan->اندر_error ())) {
    واپس خالی_اشاریہ;
  }

  hb_لیبل_t جدول_لیبل_فہرست[32];
  غیرعلامتی انحراف = 0, عدد_tables = ARRAY_لمبائی (جدول_لیبل_فہرست);

  hb_سیٹ_t subsetted_لیبل_فہرست, pending_subset_لیبل_فہرست;
  جب (((خالی) get_جدول_لیبل_فہرست_2 (plan, انحراف, &عدد_tables, جدول_لیبل_فہرست), عدد_tables))
  {
    برائے (غیرعلامتی i = 0; i < عدد_tables; ++i)
    {
      hb_لیبل_t لیبل_2 = جدول_لیبل_فہرست[i];
      اگر (should_drop_جدول (plan, لیبل_2)) جاری;
      pending_subset_لیبل_فہرست.جوڑو (لیبل_2);
    }

    انحراف += عدد_tables;
  }

  منطقی success = سچ;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_سمتیہ_t<حرف> buf;
    buf.alloc (8192 - 16);

    جب (!pending_subset_لیبل_فہرست.is_empty ())
    {
      اگر (subsetted_لیبل_فہرست.اندر_error ()
	  || pending_subset_لیبل_فہرست.اندر_error ()) {
	success = جھوٹ;
	جاؤ انتہا;
      }

      منطقی made_changes = جھوٹ;
      برائے (hb_لیبل_t لیبل_2 : pending_subset_لیبل_فہرست)
      {
	اگر (!_dependencies_satisfied (plan, لیبل_2,
				      subsetted_لیبل_فہرست,
				      pending_subset_لیبل_فہرست))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  جاری;
	}

	pending_subset_لیبل_فہرست.del (لیبل_2);
	subsetted_لیبل_فہرست.جوڑو (لیبل_2);
	made_changes = سچ;

	success = subset_جدول (plan, buf, لیبل_2);
	اگر (unlikely (!success)) جاؤ انتہا;
      }

      اگر (!made_changes)
      {
	DEBUG_پیغام (SUBSET, خالی_اشاریہ, "Table dependencies unable to be satisfied. Subset failed.");
	success = جھوٹ;
	جاؤ انتہا;
      }
    }
  }

  اگر (success && plan->attach_accelerator_ڈیٹا) {
    attach_accelerator_ڈیٹا_2 (plan, plan->dest);
  }

انتہا:
  واپس success ? hb_face_reference (plan->dest) : خالی_اشاریہ;
}
