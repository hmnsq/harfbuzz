/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012,2018  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#که_نه_تعریف_شوی HB_SANITIZE_HH
#تعریف_کړه HB_SANITIZE_HH

#ضمن "hb.hh"
#ضمن "hb-blob.hh"
#ضمن "hb-dispatch.hh"


/*
 * Sanitize
 *
 *
 * === Introduction ===
 *
 * The sanitize machinery is at the core of our zero-cost font loading.  We
 * mmap() font file into memory and create a blob out of it.  Font subtables
 * are returned as a readonly sub-blob of the main font blob.  These table
 * blobs are then sanitized before use, to ensure invalid memory access does
 * not happen.  The toplevel sanitize API use is like, eg. to load the 'head'
 * table:
 *
 *   hb_blob_t *head_blob = hb_sanitize_context_t ().reference_table<OT::head> (face);
 *
 * The blob then can be converted to a head table struct with:
 *
 *   const head *head_table = head_blob->as<head> ();
 *
 * What the reference_table does is, to call hb_face_reference_table() to load
 * the table blob, sanitize it and return either the sanitized blob, or empty
 * blob if sanitization failed.  The blob->as() function returns the null
 * object of its template type argument if the blob is empty.  Otherwise, it
 * just casts the blob contents to the desired type.
 *
 * Sanitizing a blob of data with a type T works as follows (with minor
 * simplification):
 *
 *   - Cast blob content to T*, call sanitize() method of it,
 *   - If sanitize succeeded, return blob.
 *   - Return empty blob otherwise.
 *
 *
 * === The sanitize() contract ===
 *
 * The sanitize() method of each object type shall return `true` if it's safe to
 * call other methods of the object, and `false` otherwise.
 *
 * Note that what sanitize() checks for might align with what the specification
 * describes as valid table data, but does not have to be.  In particular, we
 * do NOT want to be pedantic and concern ourselves with validity checks that
 * are irrelevant to our use of the table.  On the contrary, we want to be
 * lenient with error handling and accept invalid data to the extent that it
 * does not impose extra burden on us.
 *
 * Based on the sanitize contract, one can see that what we check for depends
 * on how we use the data in other table methods.  Ie. if other table methods
 * assume that offsets do NOT point out of the table data block, then that's
 * something sanitize() must check for (GSUB/GPOS/GDEF/etc work this way).  On
 * the other hand, if other methods do such checks themselves, then sanitize()
 * does not have to bother with them (glyf/local work this way).  The choice
 * depends on the table structure and sanitize() performance.  For example, to
 * check glyf/loca offsets in sanitize() would cost O(num-glyphs).  We try hard
 * to avoid such costs during font loading.  By postponing such checks to the
 * actual glyph loading, we reduce the sanitize cost to O(1) and total runtime
 * cost to O(used-glyphs).  As such, this is preferred.
 *
 * The same argument can be made re GSUB/GPOS/GDEF, but there, the table
 * structure is so complicated that by checking all offsets at sanitize() time,
 * we make the code much simpler in other methods, as offsets and referenced
 * objects do not need to be validated at each use site.
 *
 * Note:
 * Sanitize was named so because it used to try to recover from errors by
 * modifying the data to make it valid.  This is no longer the case, as it
 * could make HarfBuzz hallucinate new rules if there was aliasing in the
 * data.  However, the name stuck.  See: https://behdad.github.io/harfbust/
 */

/* This limits sanitizing time on really broken fonts. */
#که_نه_تعریف_شوی HB_SANITIZE_MAX_EDITS
#تعریف_کړه HB_SANITIZE_MAX_EDITS 32
#د_که_پای
#که_نه_تعریف_شوی HB_SANITIZE_MAX_OPS_ضریب
#تعریف_کړه HB_SANITIZE_MAX_OPS_ضریب 64
#د_که_پای
#که_نه_تعریف_شوی HB_SANITIZE_MAX_OPS_MIN
#تعریف_کړه HB_SANITIZE_MAX_OPS_MIN 16384
#د_که_پای
#که_نه_تعریف_شوی HB_SANITIZE_MAX_OPS_MAX
#تعریف_کړه HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#د_که_پای
#که_نه_تعریف_شوی HB_SANITIZE_MAX_SUBTABLES
#تعریف_کړه HB_SANITIZE_MAX_SUBTABLES 0x4000
#د_که_پای

بنية hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, منطقي, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ثابت حرف *پیل_2 = تش_اشاره, ثابت حرف *پای_ټکی_4 = تش_اشاره) :
	پیل (پیل_2), پای_ټکی (پای_ټکی_4),
	اوږدتیا (0),
	max_ops (0), max_subtables (0),
        recursion_ژوروالی (0),
	writable (خطأ),
	blob (تش_اشاره),
	عدد_glyphs (65536),
	عدد_glyphs_سېټ (خطأ),
	lazy_some_gpos (خطأ) {}

  ثابت حرف *get_name () { أرجع "SANITIZE"; }
  کينډۍ <ډول_نوم T, ډول_نوم F>
  منطقي may_dispatch (ثابت T *obj HB_ناکارول_شوی, ثابت F *format)
  {
    أرجع format->sanitize (هذا) &&
	   hb_barrier ();
  }
  ساكن return_t default_return_ارزښت () { أرجع ریښتیا; }
  ساكن return_t no_dispatch_return_ارزښت () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (ثابت return_t r) ثابت { أرجع !r; }

  منطقي لیدنه_subtables (بینښه شمېر)
  {
    max_subtables += شمېر;
    أرجع max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  خاص:
  کينډۍ <ډول_نوم T, ډول_نوم ...Ts> خپلکاری
  _dispatch (ثابت T &obj, hb_لومړیتوب<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (هذا, std::forward<Ts> (ds)...) )
  کينډۍ <ډول_نوم T, ډول_نوم ...Ts> خپلکاری
  _dispatch (ثابت T &obj, hb_لومړیتوب<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (هذا, std::forward<Ts> (ds)...) )
  عام:
  کينډۍ <ډول_نوم T, ډول_نوم ...Ts> خپلکاری
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    إذا (blob)
      پیل_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    إذا (blob)
      پای_ټکی_processing ();
  }

  فراغ init (hb_blob_t *b)
  {
    هذا->blob = hb_blob_reference (b);
    هذا->writable = خطأ;
  }

  فراغ سېټ_عدد_glyphs (بینښه صحيح عدد_glyphs_2)
  {
    عدد_glyphs = عدد_glyphs_2;
    عدد_glyphs_سېټ = ریښتیا;
  }
  بینښه صحيح get_عدد_glyphs () { أرجع عدد_glyphs; }

  فراغ سېټ_max_ops (صحيح max_ops_) { max_ops = max_ops_; }

  کينډۍ <ډول_نوم T>
  فراغ سېټ_object (ثابت T *obj)
  {
    reset_object ();

    إذا (!obj) أرجع;

    ثابت حرف *obj_پیل = (ثابت حرف *) obj;
    إذا (unlikely (obj_پیل < هذا->پیل || هذا->پای_ټکی <= obj_پیل))
    {
      هذا->پیل = هذا->پای_ټکی = تش_اشاره;
      هذا->اوږدتیا = 0;
    }
    وإلا
    {
      هذا->پیل = obj_پیل;
      هذا->پای_ټکی   = obj_پیل + hb_min (size_t (هذا->پای_ټکی - obj_پیل), obj->get_اندازه ());
      هذا->اوږدتیا = هذا->پای_ټکی - هذا->پیل;
    }
  }

  فراغ reset_object ()
  {
    إذا (هذا->blob)
    {
      هذا->پیل = هذا->blob->معلومات;
      هذا->پای_ټکی = هذا->پیل + هذا->blob->اوږدتیا;
    }
    هذا->اوږدتیا = هذا->پای_ټکی - هذا->پیل;
    أكد (هذا->پیل <= هذا->پای_ټکی); /* Must not overflow. */
  }

  فراغ پیل_processing (ثابت حرف *پیل_2 = تش_اشاره, ثابت حرف *پای_ټکی_4 = تش_اشاره)
  {
    إذا (پیل_2)
    {
      هذا->پیل = پیل_2;
      هذا->پای_ټکی = پای_ټکی_4;
    }
    reset_object ();
    بینښه m;
    إذا (unlikely (hb_unsigned_mul_overflows (هذا->پای_ټکی - هذا->پیل, HB_SANITIZE_MAX_OPS_ضریب, &m)))
      هذا->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    وإلا
      هذا->max_ops = hb_clamp (m,
				(بینښه) HB_SANITIZE_MAX_OPS_MIN,
				(بینښه) HB_SANITIZE_MAX_OPS_MAX);
    هذا->debug_ژوروالی = 0;
    هذا->recursion_ژوروالی = 0;

    DEBUG_پیغام_LEVEL (SANITIZE, پیل, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->پیل, هذا->پای_ټکی,
		     (بینښه اوږد) (هذا->پای_ټکی - هذا->پیل));
  }

  فراغ پای_ټکی_processing ()
  {
    DEBUG_پیغام_LEVEL (SANITIZE, هذا->پیل, 0, -1,
		     "end [%p..%p]",
		     هذا->پیل, هذا->پای_ټکی);

    hb_blob_destroy (هذا->blob);
    هذا->blob = تش_اشاره;
    هذا->پیل = هذا->پای_ټکی = تش_اشاره;
    هذا->اوږدتیا = 0;
  }

  منطقي check_ops(بینښه شمېر)
  {
    /* Avoid underflow */
    إذا (unlikely (هذا->max_ops < 0 || شمېر >= (بینښه) هذا->max_ops))
    {
      هذا->max_ops = -1;
      أرجع خطأ;
    }
    هذا->max_ops -= (صحيح) شمېر;
    أرجع ریښتیا;
  }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  منطقي check_range (ثابت فراغ *بنسټ,
		    بینښه صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) بنسټ;
    منطقي ok = (uintptr_t) (p - هذا->پیل) <= هذا->اوږدتیا &&
	      (بینښه صحيح) (هذا->پای_ټکی - p) >= len &&
	      ((هذا->max_ops -= len) > 0);

    DEBUG_پیغام_LEVEL (SANITIZE, p, هذا->debug_ژوروالی+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->پیل, هذا->پای_ټکی,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  منطقي check_range_fast (ثابت فراغ *بنسټ,
			 بینښه صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) بنسټ;
    منطقي ok = ((uintptr_t) (p - هذا->پیل) <= هذا->اوږدتیا &&
	       (بینښه صحيح) (هذا->پای_ټکی - p) >= len);

    DEBUG_پیغام_LEVEL (SANITIZE, p, هذا->debug_ژوروالی+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->پیل, هذا->پای_ټکی,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  منطقي check_ټکی (ثابت فراغ *بنسټ) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) بنسټ;
    منطقي ok = (uintptr_t) (p - هذا->پیل) <= هذا->اوږدتیا;

    DEBUG_پیغام_LEVEL (SANITIZE, p, هذا->debug_ژوروالی+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     هذا->پیل, هذا->پای_ټکی,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

  کينډۍ <ډول_نوم T>
  منطقي check_range (ثابت T *بنسټ,
		    بینښه صحيح a,
		    بینښه صحيح b) ثابت
  {
    بینښه m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (بنسټ, m);
  }

  کينډۍ <ډول_نوم T>
  منطقي check_range (ثابت T *بنسټ,
		    بینښه صحيح a,
		    بینښه صحيح b,
		    بینښه صحيح c) ثابت
  {
    بینښه m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (بنسټ, m, c);
  }

  کينډۍ <ډول_نوم T>
  HB_ALWAYS_INLINE
  منطقي check_array_sized (ثابت T *بنسټ, بینښه صحيح len, بینښه len_اندازه) ثابت
  {
    إذا (len_اندازه >= 4)
    {
      إذا (unlikely (hb_unsigned_mul_overflows (len, hb_static_اندازه (T), &len)))
	أرجع خطأ;
    }
    وإلا
      len = len * hb_static_اندازه (T);
    أرجع هذا->check_range (بنسټ, len);
  }

  کينډۍ <ډول_نوم T>
  منطقي check_array (ثابت T *بنسټ, بینښه صحيح len) ثابت
  {
    أرجع هذا->check_range (بنسټ, len, hb_static_اندازه (T));
  }

  کينډۍ <ډول_نوم T>
  منطقي check_array (ثابت T *بنسټ,
		    بینښه صحيح a,
		    بینښه صحيح b) ثابت
  {
    أرجع هذا->check_range (بنسټ, hb_static_اندازه (T), a, b);
  }

  منطقي check_پیل_recursion (صحيح max_ژوروالی)
  {
    إذا (unlikely (recursion_ژوروالی >= max_ژوروالی)) أرجع خطأ;
    أرجع ++recursion_ژوروالی;
  }

  منطقي پای_ټکی_recursion (منطقي پایله)
  {
    recursion_ژوروالی--;
    أرجع پایله;
  }

  کينډۍ <ډول_نوم Type>
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#د_که_پای
  منطقي check_struct (ثابت Type *obj) ثابت
  {
    إذا (حجم (uintptr_t) == حجم (uint32_t))
      أرجع likely (هذا->check_range_fast (obj, obj->min_اندازه));
    وإلا
      أرجع likely (هذا->check_ټکی ((ثابت حرف *) obj + obj->min_اندازه));
  }

  کينډۍ <ډول_نوم Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    منطقي sane;

    init (blob);

    DEBUG_پیغام_FUNC (SANITIZE, پیل, "start");

    پیل_processing ();

    إذا (unlikely (!پیل))
    {
      پای_ټکی_processing ();
      أرجع blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<حرف *> (پیل));

    sane = t->sanitize (هذا);

    پای_ټکی_processing ();

    DEBUG_پیغام_FUNC (SANITIZE, پیل, sane ? "PASSED" : "FAILED");
    إذا (sane)
    {
      hb_blob_make_immutable (blob);
      أرجع blob;
    }
    وإلا
    {
      hb_blob_destroy (blob);
      أرجع hb_blob_get_تشه ();
    }
  }

  کينډۍ <ډول_نوم Type>
  hb_blob_t *reference_جدول (ثابت hb_face_t *face, hb_نښان_t جدول_نښان = Type::جدول_نښان)
  {
    إذا (!عدد_glyphs_سېټ)
      سېټ_عدد_glyphs (hb_face_get_glyph_شمېر (face));
    أرجع sanitize_blob<Type> (hb_face_reference_جدول (face, جدول_نښان));
  }

  ثابت حرف *پیل, *پای_ټکی;
  بینښه اوږدتیا;
  بدلیدونکی صحيح max_ops, max_subtables;
  خاص:
  صحيح recursion_ژوروالی;
  منطقي writable;
  hb_blob_t *blob;
  بینښه صحيح عدد_glyphs;
  منطقي  عدد_glyphs_سېټ;
  عام:
  منطقي lazy_some_gpos;
};

بنية hb_sanitize_with_object_t
{
  کينډۍ <ډول_نوم T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ثابت T& obj) : c (c)
  { c->سېټ_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  خاص:
  hb_sanitize_context_t *c;
};


#د_که_پای /* HB_SANITIZE_HH */
