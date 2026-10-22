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

#اگر_تعریف_نشده HB_SANITIZE_HH
#تعریف_کن HB_SANITIZE_HH

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
#اگر_تعریف_نشده HB_SANITIZE_MAX_EDITS
#تعریف_کن HB_SANITIZE_MAX_EDITS 32
#پایان_اگر
#اگر_تعریف_نشده HB_SANITIZE_MAX_OPS_ضریب
#تعریف_کن HB_SANITIZE_MAX_OPS_ضریب 64
#پایان_اگر
#اگر_تعریف_نشده HB_SANITIZE_MAX_OPS_MIN
#تعریف_کن HB_SANITIZE_MAX_OPS_MIN 16384
#پایان_اگر
#اگر_تعریف_نشده HB_SANITIZE_MAX_OPS_MAX
#تعریف_کن HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#پایان_اگر
#اگر_تعریف_نشده HB_SANITIZE_MAX_SUBTABLES
#تعریف_کن HB_SANITIZE_MAX_SUBTABLES 0x4000
#پایان_اگر

بنية hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, منطقي, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ثابت حرف *شروع_2 = اشاره_تهی, ثابت حرف *انتها_4 = اشاره_تهی) :
	شروع (شروع_2), انتها (انتها_4),
	درازی (0),
	max_ops (0), max_subtables (0),
        recursion_عمق (0),
	writable (خطأ),
	blob (اشاره_تهی),
	رقم_glyphs (65536),
	رقم_glyphs_مجموعه (خطأ),
	lazy_some_gpos (خطأ) {}

  ثابت حرف *get_name () { أرجع "SANITIZE"; }
  قالب <نام_نوع T, نام_نوع F>
  منطقي may_dispatch (ثابت T *obj HB_استفادهنشده, ثابت F *format)
  {
    أرجع format->sanitize (هذا) &&
	   hb_barrier ();
  }
  ساكن return_t default_return_مقدار () { أرجع درست; }
  ساكن return_t no_dispatch_return_مقدار () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (ثابت return_t r) ثابت { أرجع !r; }

  منطقي بازدید_subtables (بیعلامت شمار)
  {
    max_subtables += شمار;
    أرجع max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  خاص:
  قالب <نام_نوع T, نام_نوع ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_اولویت<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (هذا, std::forward<Ts> (ds)...) )
  قالب <نام_نوع T, نام_نوع ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_اولویت<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (هذا, std::forward<Ts> (ds)...) )
  عام:
  قالب <نام_نوع T, نام_نوع ...Ts> خودکار
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    إذا (blob)
      شروع_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    إذا (blob)
      انتها_processing ();
  }

  فراغ init (hb_blob_t *b)
  {
    هذا->blob = hb_blob_reference (b);
    هذا->writable = خطأ;
  }

  فراغ مجموعه_رقم_glyphs (بیعلامت صحيح رقم_glyphs_2)
  {
    رقم_glyphs = رقم_glyphs_2;
    رقم_glyphs_مجموعه = درست;
  }
  بیعلامت صحيح get_رقم_glyphs () { أرجع رقم_glyphs; }

  فراغ مجموعه_max_ops (صحيح max_ops_) { max_ops = max_ops_; }

  قالب <نام_نوع T>
  فراغ مجموعه_object (ثابت T *obj)
  {
    reset_object ();

    إذا (!obj) أرجع;

    ثابت حرف *obj_شروع = (ثابت حرف *) obj;
    إذا (unlikely (obj_شروع < هذا->شروع || هذا->انتها <= obj_شروع))
    {
      هذا->شروع = هذا->انتها = اشاره_تهی;
      هذا->درازی = 0;
    }
    وإلا
    {
      هذا->شروع = obj_شروع;
      هذا->انتها   = obj_شروع + hb_min (size_t (هذا->انتها - obj_شروع), obj->get_اندازه ());
      هذا->درازی = هذا->انتها - هذا->شروع;
    }
  }

  فراغ reset_object ()
  {
    إذا (هذا->blob)
    {
      هذا->شروع = هذا->blob->داده;
      هذا->انتها = هذا->شروع + هذا->blob->درازی;
    }
    هذا->درازی = هذا->انتها - هذا->شروع;
    أكد (هذا->شروع <= هذا->انتها); /* Must not overflow. */
  }

  فراغ شروع_processing (ثابت حرف *شروع_2 = اشاره_تهی, ثابت حرف *انتها_4 = اشاره_تهی)
  {
    إذا (شروع_2)
    {
      هذا->شروع = شروع_2;
      هذا->انتها = انتها_4;
    }
    reset_object ();
    بیعلامت m;
    إذا (unlikely (hb_unsigned_mul_overflows (هذا->انتها - هذا->شروع, HB_SANITIZE_MAX_OPS_ضریب, &m)))
      هذا->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    وإلا
      هذا->max_ops = hb_clamp (m,
				(بیعلامت) HB_SANITIZE_MAX_OPS_MIN,
				(بیعلامت) HB_SANITIZE_MAX_OPS_MAX);
    هذا->debug_عمق = 0;
    هذا->recursion_عمق = 0;

    DEBUG_پیام_LEVEL (SANITIZE, شروع, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->شروع, هذا->انتها,
		     (بیعلامت دراز) (هذا->انتها - هذا->شروع));
  }

  فراغ انتها_processing ()
  {
    DEBUG_پیام_LEVEL (SANITIZE, هذا->شروع, 0, -1,
		     "end [%p..%p]",
		     هذا->شروع, هذا->انتها);

    hb_blob_destroy (هذا->blob);
    هذا->blob = اشاره_تهی;
    هذا->شروع = هذا->انتها = اشاره_تهی;
    هذا->درازی = 0;
  }

  منطقي check_ops(بیعلامت شمار)
  {
    /* Avoid underflow */
    إذا (unlikely (هذا->max_ops < 0 || شمار >= (بیعلامت) هذا->max_ops))
    {
      هذا->max_ops = -1;
      أرجع خطأ;
    }
    هذا->max_ops -= (صحيح) شمار;
    أرجع درست;
  }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  منطقي check_range (ثابت فراغ *پایه,
		    بیعلامت صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) پایه;
    منطقي ok = (uintptr_t) (p - هذا->شروع) <= هذا->درازی &&
	      (بیعلامت صحيح) (هذا->انتها - p) >= len &&
	      ((هذا->max_ops -= len) > 0);

    DEBUG_پیام_LEVEL (SANITIZE, p, هذا->debug_عمق+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->شروع, هذا->انتها,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  منطقي check_range_fast (ثابت فراغ *پایه,
			 بیعلامت صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) پایه;
    منطقي ok = ((uintptr_t) (p - هذا->شروع) <= هذا->درازی &&
	       (بیعلامت صحيح) (هذا->انتها - p) >= len);

    DEBUG_پیام_LEVEL (SANITIZE, p, هذا->debug_عمق+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->شروع, هذا->انتها,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  منطقي check_نقطه (ثابت فراغ *پایه) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) پایه;
    منطقي ok = (uintptr_t) (p - هذا->شروع) <= هذا->درازی;

    DEBUG_پیام_LEVEL (SANITIZE, p, هذا->debug_عمق+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     هذا->شروع, هذا->انتها,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

  قالب <نام_نوع T>
  منطقي check_range (ثابت T *پایه,
		    بیعلامت صحيح a,
		    بیعلامت صحيح b) ثابت
  {
    بیعلامت m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (پایه, m);
  }

  قالب <نام_نوع T>
  منطقي check_range (ثابت T *پایه,
		    بیعلامت صحيح a,
		    بیعلامت صحيح b,
		    بیعلامت صحيح c) ثابت
  {
    بیعلامت m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (پایه, m, c);
  }

  قالب <نام_نوع T>
  HB_ALWAYS_INLINE
  منطقي check_array_sized (ثابت T *پایه, بیعلامت صحيح len, بیعلامت len_اندازه) ثابت
  {
    إذا (len_اندازه >= 4)
    {
      إذا (unlikely (hb_unsigned_mul_overflows (len, hb_static_اندازه (T), &len)))
	أرجع خطأ;
    }
    وإلا
      len = len * hb_static_اندازه (T);
    أرجع هذا->check_range (پایه, len);
  }

  قالب <نام_نوع T>
  منطقي check_array (ثابت T *پایه, بیعلامت صحيح len) ثابت
  {
    أرجع هذا->check_range (پایه, len, hb_static_اندازه (T));
  }

  قالب <نام_نوع T>
  منطقي check_array (ثابت T *پایه,
		    بیعلامت صحيح a,
		    بیعلامت صحيح b) ثابت
  {
    أرجع هذا->check_range (پایه, hb_static_اندازه (T), a, b);
  }

  منطقي check_شروع_recursion (صحيح max_عمق)
  {
    إذا (unlikely (recursion_عمق >= max_عمق)) أرجع خطأ;
    أرجع ++recursion_عمق;
  }

  منطقي انتها_recursion (منطقي نتیجه)
  {
    recursion_عمق--;
    أرجع نتیجه;
  }

  قالب <نام_نوع Type>
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
  HB_ALWAYS_INLINE
#پایان_اگر
  منطقي check_struct (ثابت Type *obj) ثابت
  {
    إذا (حجم (uintptr_t) == حجم (uint32_t))
      أرجع likely (هذا->check_range_fast (obj, obj->min_اندازه));
    وإلا
      أرجع likely (هذا->check_نقطه ((ثابت حرف *) obj + obj->min_اندازه));
  }

  قالب <نام_نوع Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    منطقي sane;

    init (blob);

    DEBUG_پیام_FUNC (SANITIZE, شروع, "start");

    شروع_processing ();

    إذا (unlikely (!شروع))
    {
      انتها_processing ();
      أرجع blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<حرف *> (شروع));

    sane = t->sanitize (هذا);

    انتها_processing ();

    DEBUG_پیام_FUNC (SANITIZE, شروع, sane ? "PASSED" : "FAILED");
    إذا (sane)
    {
      hb_blob_make_immutable (blob);
      أرجع blob;
    }
    وإلا
    {
      hb_blob_destroy (blob);
      أرجع hb_blob_get_خالی ();
    }
  }

  قالب <نام_نوع Type>
  hb_blob_t *reference_جدول (ثابت hb_face_t *face, hb_برچسب_t جدول_برچسب = Type::جدول_برچسب)
  {
    إذا (!رقم_glyphs_مجموعه)
      مجموعه_رقم_glyphs (hb_face_get_glyph_شمار (face));
    أرجع sanitize_blob<Type> (hb_face_reference_جدول (face, جدول_برچسب));
  }

  ثابت حرف *شروع, *انتها;
  بیعلامت درازی;
  تغییرپذیر صحيح max_ops, max_subtables;
  خاص:
  صحيح recursion_عمق;
  منطقي writable;
  hb_blob_t *blob;
  بیعلامت صحيح رقم_glyphs;
  منطقي  رقم_glyphs_مجموعه;
  عام:
  منطقي lazy_some_gpos;
};

بنية hb_sanitize_with_object_t
{
  قالب <نام_نوع T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ثابت T& obj) : c (c)
  { c->مجموعه_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  خاص:
  hb_sanitize_context_t *c;
};


#پایان_اگر /* HB_SANITIZE_HH */
