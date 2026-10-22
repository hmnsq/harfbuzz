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


hb_user_معلومات_کلی_t hb_subset_accelerator_user_معلومات_کلی = {};


/*
 * The list of tables in the open type spec. Used to check for tables that may need handling
 * if we are unable to list the tables in a face.
 */
ساكن hb_نښان_t معلوم_tables[] {
  HB_نښان('a','v','a','r'),
  HB_نښان('B','A','S','E'),
  HB_نښان('C','B','D','T'),
  HB_نښان('C','B','L','C'),
  HB_نښان('C','F','F',' '),
  HB_نښان('C','F','F','2'),
  HB_نښان('c','m','a','p'),
  HB_نښان('C','O','L','R'),
  HB_نښان('C','P','A','L'),
  HB_نښان('c','v','a','r'),
  HB_نښان('c','v','t',' '),
  HB_نښان('D','S','I','G'),
  HB_نښان('E','B','D','T'),
  HB_نښان('E','B','L','C'),
  HB_نښان('E','B','S','C'),
  HB_نښان('f','p','g','m'),
  HB_نښان('f','v','a','r'),
  HB_نښان('g','a','s','p'),
  HB_نښان('G','D','E','F'),
  HB_نښان('g','l','y','f'),
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  HB_نښان('G','L','Y','F'),
#د_که_پای
  HB_نښان('G','P','O','S'),
  HB_نښان('G','S','U','B'),
  HB_نښان('g','v','a','r'),
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  HB_نښان('G','V','A','R'),
#د_که_پای
  HB_نښان('h','d','m','x'),
  HB_نښان('h','e','a','d'),
  HB_نښان('h','h','e','a'),
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  HB_نښان('H','H','E','A'),
#د_که_پای
  HB_نښان('h','m','t','x'),
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  HB_نښان('H','M','T','X'),
#د_که_پای
  HB_نښان('H','V','A','R'),
  HB_نښان('J','S','T','F'),
  HB_نښان('k','e','r','n'),
  HB_نښان('l','o','c','a'),
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  HB_نښان('L','O','C','A'),
#د_که_پای
  HB_نښان('L','T','S','H'),
  HB_نښان('M','A','T','H'),
  HB_نښان('m','a','x','p'),
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  HB_نښان('M','A','X','P'),
#د_که_پای
  HB_نښان('M','E','R','G'),
  HB_نښان('m','e','t','a'),
  HB_نښان('M','V','A','R'),
  HB_نښان('P','C','L','T'),
  HB_نښان('p','o','s','t'),
  HB_نښان('p','r','e','p'),
  HB_نښان('s','b','i','x'),
  HB_نښان('S','T','A','T'),
  HB_نښان('S','V','G',' '),
  HB_نښان('V','D','M','X'),
  HB_نښان('v','h','e','a'),
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  HB_نښان('V','H','E','A'),
#د_که_پای
  HB_نښان('v','m','t','x'),
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  HB_نښان('V','M','T','X'),
#د_که_پای
  HB_نښان('V','O','R','G'),
  HB_نښان('V','A','R','C'),
  HB_نښان('V','V','A','R'),
  HB_نښان('n','a','m','e'),
  HB_نښان('O','S','/','2')
};

ساكن منطقي جدول_is_تشه (ثابت hb_face_t *face, hb_نښان_t نښان_2)
{
  hb_blob_t* blob = hb_face_reference_جدول (face, نښان_2);
  منطقي پایله = (blob == hb_blob_get_تشه ());
  hb_blob_destroy (blob);
  أرجع پایله;
}

ساكن بینښه صحيح
get_جدول_نښانونه_2 (ثابت hb_subset_plan_t* plan,
                 بینښه صحيح  پیل_دننه_واټن,
                 بینښه صحيح *جدول_شمېر_2, /* IN/OUT */
                 hb_نښان_t     *جدول_نښانونه /*بهر*/)
{
  بینښه عدد_tables = hb_face_get_جدول_نښانونه (plan->سرچینه, 0, تش_اشاره, تش_اشاره);
  إذا (عدد_tables)
    أرجع hb_face_get_جدول_نښانونه (plan->سرچینه, پیل_دننه_واټن, جدول_شمېر_2, جدول_نښانونه);

  // If face has 0 tables associated with it, assume that it was built from
  // hb_face_create_tables and thus is unable to list its tables. Fallback to
  // checking each table type we can handle for existence instead.
  خپلکاری it =
      hb_concat (
          + hb_array (معلوم_tables)
          | hb_filter ([&] (hb_نښان_t نښان_2) {
            أرجع !جدول_is_تشه (plan->سرچینه, نښان_2) && !plan->no_subset_tables.has (نښان_2);
          })
          | hb_map ([] (hb_نښان_t نښان_2) -> hb_نښان_t { أرجع نښان_2; }),

          plan->no_subset_tables.iter ()
          | hb_filter([&] (hb_نښان_t نښان_2) {
            أرجع !جدول_is_تشه (plan->سرچینه, نښان_2);
          }));

  it += پیل_دننه_واټن;

  بینښه عدد_written = 0;
  طالما (منطقي (it) && عدد_written < *جدول_شمېر_2)
    جدول_نښانونه[عدد_written++] = *it++;

  *جدول_شمېر_2 = عدد_written;
  أرجع عدد_written;
}


ساكن منطقي
is_جدول_present (hb_face_t *سرچینه, hb_نښان_t نښان_2)
{

  إذا (!hb_face_get_جدول_نښانونه (سرچینه, 0, تش_اشاره, تش_اشاره)) {
    // If face has 0 tables associated with it, assume that it was built from
    // hb_face_create_tables and thus is unable to list its tables. Fallback to
    // checking if the blob associated with tag is empty.
    أرجع !جدول_is_تشه (سرچینه, نښان_2);
  }

  hb_نښان_t جدول_نښانونه[32];
  بینښه دننه_واټن = 0, عدد_tables = ARRAY_اوږدتیا (جدول_نښانونه);
  طالما (((فراغ) hb_face_get_جدول_نښانونه (سرچینه, دننه_واټن, &عدد_tables, جدول_نښانونه), عدد_tables))
  {
    لكل (بینښه i = 0; i < عدد_tables; ++i)
      إذا (جدول_نښانونه[i] == نښان_2)
	أرجع ریښتیا;
    دننه_واټن += عدد_tables;
  }
  أرجع خطأ;
}

ساكن منطقي
should_drop_جدول (hb_subset_plan_t *plan, hb_نښان_t نښان_2)
{
  إذا (plan->drop_tables.has (نښان_2))
    أرجع ریښتیا;

  بدل (نښان_2)
  {
  حالة HB_نښان('c','v','a','r'): /* hint table, fallthrough */
    أرجع plan->all_axes_pinned || (plan->flags & HB_SUBSET_FLAGS_NO_HINTING);

  حالة HB_نښان('c','v','t',' '): /* hint table, fallthrough */
  حالة HB_نښان('f','p','g','m'): /* hint table, fallthrough */
  حالة HB_نښان('p','r','e','p'): /* hint table, fallthrough */
  حالة HB_نښان('h','d','m','x'): /* hint table, fallthrough */
  حالة HB_نښان('V','D','M','X'): /* hint table, fallthrough */
    أرجع plan->flags & HB_SUBSET_FLAGS_NO_HINTING;

#که_تعریف_شوی HB_NO_SUBSET_LAYOUT
    // Drop Layout Tables if requested.
  حالة HB_نښان('G','D','E','F'):
  حالة HB_نښان('G','P','O','S'):
  حالة HB_نښان('G','S','U','B'):
  حالة HB_نښان('m','o','r','x'):
  حالة HB_نښان('m','o','r','t'):
  حالة HB_نښان('k','e','r','x'):
  حالة HB_نښان('k','e','r','n'):
    أرجع ریښتیا;
#د_که_پای

  حالة HB_نښان('a','v','a','r'):
  حالة HB_نښان('f','v','a','r'):
  حالة HB_نښان('g','v','a','r'):
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  حالة HB_نښان('G','V','A','R'):
#د_که_پای
  حالة HB_نښان('H','V','A','R'):
  حالة HB_نښان('V','V','A','R'):
  حالة HB_نښان('M','V','A','R'):
    أرجع plan->all_axes_pinned;

  افتراضي:
    أرجع خطأ;
  }
}

ساكن منطقي
_dependencies_satisfied (hb_subset_plan_t *plan, hb_نښان_t نښان_2,
                         ثابت hb_سېټ_t &subsetted_نښانونه,
                         ثابت hb_سېټ_t &pending_subset_نښانونه)
{
  منطقي outline_pending = pending_subset_نښانونه.has (HB_نښان('g','l','y','f'));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  outline_pending |= pending_subset_نښانونه.has (HB_نښان('G','L','Y','F'));
#د_که_پای

  بدل (نښان_2)
  {
  حالة HB_نښان('h','m','t','x'):
  حالة HB_نښان('v','m','t','x'):
  حالة HB_نښان('m','a','x','p'):
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  حالة HB_نښان('H','M','T','X'):
  حالة HB_نښان('V','M','T','X'):
  حالة HB_نښان('M','A','X','P'):
#د_که_پای
  حالة HB_نښان('O','S','/','2'):
    أرجع !plan->normalized_coords || !outline_pending;
  حالة HB_نښان('G','P','O','S'):
  حالة HB_نښان('G','S','U','B'):
    أرجع plan->all_axes_pinned || !pending_subset_نښانونه.has (HB_نښان('G','D','E','F'));
  /* For avar2, variation tables must wait for avar to be subsetted first. */
  حالة HB_نښان('g','v','a','r'):
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  حالة HB_نښان('G','V','A','R'):
#د_که_پای
  حالة HB_نښان('c','v','a','r'):
  حالة HB_نښان('H','V','A','R'):
  حالة HB_نښان('V','V','A','R'):
  حالة HB_نښان('M','V','A','R'):
  حالة HB_نښان('G','D','E','F'):
  حالة HB_نښان('C','F','F','2'):
    أرجع !plan->has_avar2 || !pending_subset_نښانونه.has (HB_نښان('a','v','a','r'));
  افتراضي:
    أرجع ریښتیا;
  }
}

ساكن منطقي
subset_جدول (hb_subset_plan_t *plan,
	       hb_ویکټور_t<حرف> &buf,
	       hb_نښان_t نښان_2)
{
  إذا (plan->no_subset_tables.has (نښان_2)) {
    أرجع hb_subset_جدول_passthrough (plan, نښان_2);
  }

  DEBUG_پیغام (SUBSET, تش_اشاره, "subset %c%c%c%c", HB_UNTAG (نښان_2));

  منطقي success;
  إذا (hb_subset_جدول_layout (plan, buf, نښان_2, &success) ||
      hb_subset_جدول_var (plan, buf, نښان_2, &success) ||
      hb_subset_جدول_cff (plan, buf, نښان_2, &success) ||
      hb_subset_جدول_color (plan, buf, نښان_2, &success) ||
      hb_subset_جدول_other (plan, buf, نښان_2, &success))
    أرجع success;


  بدل (نښان_2)
  {
  حالة HB_نښان('h','e','a','d'):
  {
    منطقي outline_جدول_handles_سر =
	is_جدول_present (plan->سرچینه, HB_نښان('g','l','y','f')) &&
	!should_drop_جدول (plan, HB_نښان('g','l','y','f'));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    outline_جدول_handles_سر |=
	is_جدول_present (plan->سرچینه, HB_نښان('G','L','Y','F')) &&
	!should_drop_جدول (plan, HB_نښان('G','L','Y','F'));
#د_که_پای
    إذا (outline_جدول_handles_سر)
      أرجع ریښتیا; /* skip head, handled by the outline table */
    أرجع hb_subset_جدول<ثابت OT::سر_2> (plan, buf);
  }

  حالة HB_نښان('S','T','A','T'):
    إذا (!plan->user_axes_location.is_empty ()) أرجع hb_subset_جدول<ثابت OT::STAT> (plan, buf);
    وإلا أرجع hb_subset_جدول_passthrough (plan, نښان_2);

  حالة HB_نښان('c','v','t',' '):
#که_نه_تعریف_شوی HB_NO_VAR
    إذا (is_جدول_present (plan->سرچینه, HB_نښان('c','v','a','r')) &&
        plan->normalized_coords && !plan->pinned_at_default)
    {
      خپلکاری &cvar = *plan->سرچینه->جدول.cvar;
      أرجع OT::cvar::جمع_کړه_cvt_and_apply_deltas (plan, cvar.get_tuple_var_معلومات (), &cvar);
    }
#د_که_پای
    أرجع hb_subset_جدول_passthrough (plan, نښان_2);
  }

  إذا (plan->flags & HB_SUBSET_FLAGS_PASSTHROUGH_UNRECOGNIZED)
    أرجع hb_subset_جدول_passthrough (plan, نښان_2);

  // Drop table
  أرجع ریښتیا;
}

ساكن فراغ attach_accelerator_معلومات_2 (hb_subset_plan_t* plan,
                                      hb_face_t* face /* IN/OUT */)
{
  إذا (!plan->inprogress_accelerator) أرجع;

  // Transfer the accelerator from the plan to us.
  hb_subset_accelerator_t* accel = plan->inprogress_accelerator;
  plan->inprogress_accelerator = تش_اشاره;

  إذا (accel->دننه_error ())
  {
    hb_subset_accelerator_t::destroy (accel);
    أرجع;
  }

  // Populate caches that need access to the final tables.
  hb_blob_ptr_t<OT::cmap> cmap_ptr (hb_sanitize_context_t ().reference_جدول<OT::cmap> (face));
  accel->cmap_cache = OT::cmap::create_filled_cache (cmap_ptr);
  accel->destroy_cmap_cache = OT::SubtableUnicodesCache::destroy;

  إذا (!hb_face_سېټ_user_معلومات(face,
                             hb_subset_accelerator_t::user_معلومات_کلی(),
                             accel,
                             hb_subset_accelerator_t::destroy,
                             ریښتیا))
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
hb_subset_or_fail (hb_face_t *سرچینه, ثابت hb_subset_ننوت_t *ننوت)
{
  إذا (unlikely (!ننوت || !سرچینه)) أرجع تش_اشاره;

  إذا (unlikely (!سرچینه->get_عدد_glyphs ()))
  {
    DEBUG_پیغام (SUBSET, تش_اشاره, "No glyphs in source font.");
    أرجع تش_اشاره;
  }

  hb_subset_plan_t *plan = hb_subset_plan_create_or_fail (سرچینه, ننوت);
  إذا (unlikely (!plan)) {
    أرجع تش_اشاره;
  }

  hb_face_t * پایله = hb_subset_plan_execute_or_fail (plan);
  hb_subset_plan_destroy (plan);
  أرجع پایله;
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
  إذا (unlikely (!plan || plan->دننه_error ())) {
    أرجع تش_اشاره;
  }

  hb_نښان_t جدول_نښانونه[32];
  بینښه دننه_واټن = 0, عدد_tables = ARRAY_اوږدتیا (جدول_نښانونه);

  hb_سېټ_t subsetted_نښانونه, pending_subset_نښانونه;
  طالما (((فراغ) get_جدول_نښانونه_2 (plan, دننه_واټن, &عدد_tables, جدول_نښانونه), عدد_tables))
  {
    لكل (بینښه i = 0; i < عدد_tables; ++i)
    {
      hb_نښان_t نښان_2 = جدول_نښانونه[i];
      إذا (should_drop_جدول (plan, نښان_2)) استمر;
      pending_subset_نښانونه.جمع_کړه (نښان_2);
    }

    دننه_واټن += عدد_tables;
  }

  منطقي success = ریښتیا;

  {
    // Grouping to deallocate buf before calling hb_face_reference (plan->dest).

    hb_ویکټور_t<حرف> buf;
    buf.alloc (8192 - 16);

    طالما (!pending_subset_نښانونه.is_empty ())
    {
      إذا (subsetted_نښانونه.دننه_error ()
	  || pending_subset_نښانونه.دننه_error ()) {
	success = خطأ;
	اذهب پای_ټکی;
      }

      منطقي made_changes = خطأ;
      لكل (hb_نښان_t نښان_2 : pending_subset_نښانونه)
      {
	إذا (!_dependencies_satisfied (plan, نښان_2,
				      subsetted_نښانونه,
				      pending_subset_نښانونه))
	{
	  // delayed subsetting for some tables since they might have dependency on other tables
	  // in some cases: e.g: during instantiating glyf tables, hmetrics/vmetrics are updated
	  // and saved in subset plan, hmtx/vmtx subsetting need to use these updated metrics values
	  استمر;
	}

	pending_subset_نښانونه.del (نښان_2);
	subsetted_نښانونه.جمع_کړه (نښان_2);
	made_changes = ریښتیا;

	success = subset_جدول (plan, buf, نښان_2);
	إذا (unlikely (!success)) اذهب پای_ټکی;
      }

      إذا (!made_changes)
      {
	DEBUG_پیغام (SUBSET, تش_اشاره, "Table dependencies unable to be satisfied. Subset failed.");
	success = خطأ;
	اذهب پای_ټکی;
      }
    }
  }

  إذا (success && plan->attach_accelerator_معلومات) {
    attach_accelerator_معلومات_2 (plan, plan->dest);
  }

پای_ټکی:
  أرجع success ? hb_face_reference (plan->dest) : تش_اشاره;
}
