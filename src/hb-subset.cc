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


hb_user_داده_کلید_t hb_subset_accelerator_user_داده_کلید = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ساكن hb_برچسب_t معلوم_tables[] {
  HB_برچسب('a','v','a','r'),
  HB_برچسب('B','A','S','E'),
  HB_برچسب('C','B','D','T'),
  HB_برچسب('C','B','L','C'),
  HB_برچسب('C','F','F',' '),
  HB_برچسب('C','F','F','2'),
  HB_برچسب('c','m','a','p'),
  HB_برچسب('C','O','L','R'),
  HB_برچسب('C','P','A','L'),
  HB_برچسب('c','v','a','r'),
  HB_برچسب('c','v','t',' '),
  HB_برچسب('D','S','I','G'),
  HB_برچسب('E','B','D','T'),
  HB_برچسب('E','B','L','C'),
  HB_برچسب('E','B','S','C'),
  HB_برچسب('f','p','g','m'),
  HB_برچسب('f','v','a','r'),
  HB_برچسب('g','a','s','p'),
  HB_برچسب('G','D','E','F'),
  HB_برچسب('g','l','y','f'),
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  HB_برچسب('G','L','Y','F'),
#پایان_اگر
  HB_برچسب('G','P','O','S'),
  HB_برچسب('G','S','U','B'),
  HB_برچسب('g','v','a','r'),
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  HB_برچسب('G','V','A','R'),
#پایان_اگر
  HB_برچسب('h','d','m','x'),
  HB_برچسب('h','e','a','d'),
  HB_برچسب('h','h','e','a'),
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  HB_برچسب('H','H','E','A'),
#پایان_اگر
  HB_برچسب('h','m','t','x'),
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  HB_برچسب('H','M','T','X'),
#پایان_اگر
  HB_برچسب('H','V','A','R'),
  HB_برچسب('J','S','T','F'),
  HB_برچسب('k','e','r','n'),
  HB_برچسب('l','o','c','a'),
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  HB_برچسب('L','O','C','A'),
#پایان_اگر
  HB_برچسب('L','T','S','H'),
  HB_برچسب('M','A','T','H'),
  HB_برچسب('m','a','x','p'),
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  HB_برچسب('M','A','X','P'),
#پایان_اگر
  HB_برچسب('M','E','R','G'),
  HB_برچسب('m','e','t','a'),
  HB_برچسب('M','V','A','R'),
  HB_برچسب('P','C','L','T'),
  HB_برچسب('p','o','s','t'),
  HB_برچسب('p','r','e','p'),
  HB_برچسب('s','b','i','x'),
  HB_برچسب('S','T','A','T'),
  HB_برچسب('S','V','G',' '),
  HB_برچسب('V','D','M','X'),
  HB_برچسب('v','h','e','a'),
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  HB_برچسب('V','H','E','A'),
#پایان_اگر
  HB_برچسب('v','m','t','x'),
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  HB_برچسب('V','M','T','X'),
#پایان_اگر
  HB_برچسب('V','O','R','G'),
  HB_برچسب('V','A','R','C'),
  HB_برچسب('V','V','A','R'),
  HB_برچسب('n','a','m','e'),
  HB_برچسب('O','S','/','2')
};

ساكن منطقي جدول_is_خالی (ثابت hb_face_t *face, hb_برچسب_t برچسب_2)
{
  hb_blob_t* blob = hb_face_reference_جدول (face, برچسب_2);
  منطقي نتیجه = (blob == hb_blob_get_خالی ());
  hb_blob_destroy (blob);
  أرجع نتیجه;
}

ساكن بیعلامت صحيح
get_جدول_برچسبها_2 (ثابت hb_subset_plan_t* plan,
                 بیعلامت صحيح  شروع_جابه_جایی,
                 بیعلامت صحيح *جدول_شمار_2, /* IN/OUT */
                 hb_برچسب_t     *جدول_برچسبها /*بیرون*/)
{
  بیعلامت رقم_tables = hb_face_get_جدول_برچسبها (plan->منبع, 0, اشاره_تهی, اشاره_تهی);
  إذا (رقم_tables)
    أرجع hb_face_get_جدول_برچسبها (plan->منبع, شروع_جابه_جایی, جدول_شمار_2, جدول_برچسبها);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  خودکار it =
      hb_concat (
          + hb_array (معلوم_tables)
          | hb_filter ([&] (hb_برچسب_t برچسب_2) {
            أرجع !جدول_is_خالی (plan->منبع, برچسب_2) && !plan->no_subset_tables.has (برچسب_2);
          })
          | hb_map ([] (hb_برچسب_t برچسب_2) -> hb_برچسب_t { أرجع برچسب_2; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_برچسب_t برچسب_2) {
            أرجع !جدول_is_خالی (plan->منبع, برچسب_2);
          }));

  it += شروع_جابه_جایی;

  بیعلامت رقم_written = 0;
  طالما (منطقي (it) && رقم_written < *جدول_شمار_2)
    جدول_برچسبها[رقم_written++] = *it++;

  *جدول_شمار_2 = رقم_written;
  أرجع رقم_written;
}


ساكن منطقي
is_جدول_present (hb_face_t *منبع, hb_برچسب_t برچسب_2)
{

  إذا (!hb_face_get_جدول_برچسبها (منبع, 0, اشاره_تهی, اشاره_تهی)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    أرجع !جدول_is_خالی (منبع, برچسب_2);
  }

  hb_برچسب_t جدول_برچسبها[32];
  بیعلامت جابه_جایی = 0, رقم_tables = ARRAY_درازی (جدول_برچسبها);
  طالما (((فراغ) hb_face_get_جدول_برچسبها (منبع, جابه_جایی, &رقم_tables, جدول_برچسبها), رقم_tables))
  {
    لكل (بیعلامت i = 0; i < رقم_tables; ++i)
      إذا (جدول_برچسبها[i] == برچسب_2)
	أرجع درست;
    جابه_جایی += رقم_tables;
  }
  أرجع خطأ;
}

ساكن منطقي
should_drop_جدول (hb_subset_plan_t *plan, hb_برچسب_t برچسب_2)
{
  إذا (plan->drop_tables.has (برچسب_2))
    أرجع درست;

  بدل (برچسب_2)
  {
  حالة HB_برچسب('c','v','a','r'): /* hint table, fallthrough */
    أرجع plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  حالة HB_برچسب('c','v','t',' '): /* hint table, fallthrough */
  حالة HB_برچسب('f','p','g','m'): /* hint table, fallthrough */
  حالة HB_برچسب('p','r','e','p'): /* hint table, fallthrough */
  حالة HB_برچسب('h','d','m','x'): /* hint table, fallthrough */
  حالة HB_برچسب('V','D','M','X'): /* hint table, fallthrough */
    أرجع plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#اگر_تعریف_شده HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  حالة HB_برچسب('G','D','E','F'):
  حالة HB_برچسب('G','P','O','S'):
  حالة HB_برچسب('G','S','U','B'):
  حالة HB_برچسب('m','o','r','x'):
  حالة HB_برچسب('m','o','r','t'):
  حالة HB_برچسب('k','e','r','x'):
  حالة HB_برچسب('k','e','r','n'):
    أرجع درست;
#پایان_اگر

  حالة HB_برچسب('a','v','a','r'):
  حالة HB_برچسب('f','v','a','r'):
  حالة HB_برچسب('g','v','a','r'):
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  حالة HB_برچسب('G','V','A','R'):
#پایان_اگر
  حالة HB_برچسب('H','V','A','R'):
  حالة HB_برچسب('V','V','A','R'):
  حالة HB_برچسب('M','V','A','R'):
    أرجع plan->all_axes_pinned;

  افتراضي:
    أرجع خطأ;
  }
}

ساكن منطقي
_dependencies_satisfied (hb_subset_plan_t *plan, hb_برچسب_t برچسب_2,
                         ثابت hb_مجموعه_t &subsetted_برچسبها,
                         ثابت hb_مجموعه_t &pending_subset_برچسبها)
{
  منطقي outline_pending = pending_subset_برچسبها.has (HB_برچسب('g','l','y','f'));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  outline_pending |= pending_subset_برچسبها.has (HB_برچسب('G','L','Y','F'));
#پایان_اگر

  بدل (برچسب_2)
  {
  حالة HB_برچسب('h','m','t','x'):
  حالة HB_برچسب('v','m','t','x'):
  حالة HB_برچسب('m','a','x','p'):
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  حالة HB_برچسب('H','M','T','X'):
  حالة HB_برچسب('V','M','T','X'):
  حالة HB_برچسب('M','A','X','P'):
#پایان_اگر
  حالة HB_برچسب('O','S','/','2'):
    أرجع !plan->normalized_coords || !outline_pending;
  حالة HB_برچسب('G','P','O','S'):
  حالة HB_برچسب('G','S','U','B'):
    أرجع plan->all_axes_pinned || !pending_subset_برچسبها.has (HB_برچسب('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  حالة HB_برچسب('g','v','a','r'):
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  حالة HB_برچسب('G','V','A','R'):
#پایان_اگر
  حالة HB_برچسب('c','v','a','r'):
  حالة HB_برچسب('H','V','A','R'):
  حالة HB_برچسب('V','V','A','R'):
  حالة HB_برچسب('M','V','A','R'):
  حالة HB_برچسب('G','D','E','F'):
  حالة HB_برچسب('C','F','F','2'):
    أرجع !plan->has_avar2 || !pending_subset_برچسبها.has (HB_برچسب('a','v','a','r'));
  افتراضي:
    أرجع درست;
  }
}

ساكن منطقي
subset_جدول (hb_subset_plan_t *plan,
	       hb_وکتور_t<حرف> &buf,
	       hb_برچسب_t برچسب_2)
{
  إذا (plan->no_subset_tables.has (برچسب_2)) {
    أرجع hb_subset_جدول_passthrough (plan, برچسب_2);
  }

  DEBUG_پیام (SUBSET, اشاره_تهی, "subset %c%c%c%c", HB_UNTAG (برچسب_2));

  منطقي success;
  إذا (hb_subset_جدول_layout (plan, buf, برچسب_2, &success) ||
      hb_subset_جدول_var (plan, buf, برچسب_2, &success) ||
      hb_subset_جدول_cff (plan, buf, برچسب_2, &success) ||
      hb_subset_جدول_color (plan, buf, برچسب_2, &success) ||
      hb_subset_جدول_other (plan, buf, برچسب_2, &success))
    أرجع success;


  بدل (برچسب_2)
  {
  حالة HB_برچسب('h','e','a','d'):
  {
    منطقي outline_جدول_handles_سر =
	is_جدول_present (plan->منبع, HB_برچسب('g','l','y','f')) &&
	!should_drop_جدول (plan, HB_برچسب('g','l','y','f'));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    outline_جدول_handles_سر |=
	is_جدول_present (plan->منبع, HB_برچسب('G','L','Y','F')) &&
	!should_drop_جدول (plan, HB_برچسب('G','L','Y','F'));
#پایان_اگر
    إذا (outline_جدول_handles_سر)
      أرجع درست; /* skip head, handled by the outline table */
    أرجع hb_subset_جدول<ثابت OT::سر_2> (plan, buf);
  }

  حالة HB_برچسب('S','T','A','T'):
    إذا (!plan->user_axes_location.is_empty ()) أرجع hb_subset_جدول<ثابت OT::STAT> (plan, buf);
    وإلا أرجع hb_subset_جدول_passthrough (plan, برچسب_2);

  حالة HB_برچسب('c','v','t',' '):
#اگر_تعریف_نشده HB_NO_VAR
    إذا (is_جدول_present (plan->منبع, HB_برچسب('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      خودکار &cvar = *plan->منبع->جدول.cvar;
      أرجع OT::cvar::جمع_کن_cvt_and_apply_deltas (plan, cvar.get_tuple_var_داده (), &cvar);
    }
#پایان_اگر
    أرجع hb_subset_جدول_passthrough (plan, برچسب_2);
  }

  إذا (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    أرجع hb_subset_جدول_passthrough (plan, برچسب_2);

  // Drop table
  أرجع درست;
}

ساكن فراغ attach_accelerator_داده_2 (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  إذا (!plan->inprogress_accelerator) أرجع;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = اشاره_تهی;

  إذا (accel->درون_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    أرجع;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_جدول<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  إذا (!hb_face_مجموعه_user_داده(face,
                             hb_subset_accelerator_t::user_داده_کلید(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             درست))
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
hb_subset_or_fail (hb_face_t *منبع, ثابت hb_subset_درون_داد_t *درون_داد)
{
  إذا (unlikely (!درون_داد || !منبع)) أرجع اشاره_تهی;

  إذا (unlikely (!منبع->get_رقم_glyphs ()))
  {
    DEBUG_پیام (SUBSET, اشاره_تهی, "No glyphs in source font.");
    أرجع اشاره_تهی;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (منبع, درون_داد);
  إذا (unlikely (!plan)) {
    أرجع اشاره_تهی;
  }

  hb_face_t * نتیجه = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  أرجع نتیجه;
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
  إذا (unlikely (!plan || plan->درون_error ())) {
    أرجع اشاره_تهی;
  }

  hb_برچسب_t جدول_برچسبها[32];
  بیعلامت جابه_جایی = 0, رقم_tables = ARRAY_درازی (جدول_برچسبها);

  hb_مجموعه_t subsetted_برچسبها, pending_subset_برچسبها;
  طالما (((فراغ) get_جدول_برچسبها_2 (plan, جابه_جایی, &رقم_tables, جدول_برچسبها), رقم_tables))
  {
    لكل (بیعلامت i = 0; i < رقم_tables; ++i)
    {
      hb_برچسب_t برچسب_2 = جدول_برچسبها[i];
      إذا (should_drop_جدول (plan, برچسب_2)) استمر;
      pending_subset_برچسبها.جمع_کن (برچسب_2);
    }

    جابه_جایی += رقم_tables;
  }

  منطقي success = درست;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_وکتور_t<حرف> buf;
    buf.alloc (8192 - 16);

    طالما (!pending_subset_برچسبها.is_empty ())
    {
      إذا (subsetted_برچسبها.درون_error ()
	  || pending_subset_برچسبها.درون_error ()) {
	success = خطأ;
	اذهب انتها;
      }

      منطقي made_changes = خطأ;
      لكل (hb_برچسب_t برچسب_2 : pending_subset_برچسبها)
      {
	إذا (!_dependencies_satisfied (plan, برچسب_2,
				      subsetted_برچسبها,
				      pending_subset_برچسبها))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  استمر;
	}

	pending_subset_برچسبها.del (برچسب_2);
	subsetted_برچسبها.جمع_کن (برچسب_2);
	made_changes = درست;

	success = subset_جدول (plan, buf, برچسب_2);
	إذا (unlikely (!success)) اذهب انتها;
      }

      إذا (!made_changes)
      {
	DEBUG_پیام (SUBSET, اشاره_تهی, "Table dependencies unable to be satisfied. Subset failed.");
	success = خطأ;
	اذهب انتها;
      }
    }
  }

  إذا (success && plan->attach_accelerator_داده) {
    attach_accelerator_داده_2 (plan, plan->dest);
  }

انتها:
  أرجع success ? hb_face_reference (plan->dest) : اشاره_تهی;
}
