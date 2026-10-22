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

#جے_بنا_تعریف HB_SANITIZE_HH
#تعریف_کرو HB_SANITIZE_HH

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
#جے_بنا_تعریف HB_SANITIZE_MAX_EDITS
#تعریف_کرو HB_SANITIZE_MAX_EDITS 32
#جے_انت
#جے_بنا_تعریف HB_SANITIZE_MAX_OPS_ضارب
#تعریف_کرو HB_SANITIZE_MAX_OPS_ضارب 64
#جے_انت
#جے_بنا_تعریف HB_SANITIZE_MAX_OPS_MIN
#تعریف_کرو HB_SANITIZE_MAX_OPS_MIN 16384
#جے_انت
#جے_بنا_تعریف HB_SANITIZE_MAX_OPS_MAX
#تعریف_کرو HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#جے_انت
#جے_بنا_تعریف HB_SANITIZE_MAX_SUBTABLES
#تعریف_کرو HB_SANITIZE_MAX_SUBTABLES 0x4000
#جے_انت

بنية hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, منطقي, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ثابت حرف *شروع_2 = خالی_سوچک, ثابت حرف *سرا_4 = خالی_سوچک) :
	شروع (شروع_2), سرا (سرا_4),
	لمائی (0),
	max_ops (0), max_subtables (0),
        recursion_ڈونگھائی (0),
	writable (خطأ),
	blob (خالی_سوچک),
	عدد_glyphs (65536),
	عدد_glyphs_سیٹ (خطأ),
	lazy_some_gpos (خطأ) {}

  ثابت حرف *get_name () { أرجع "SANITIZE"; }
  سانچا <قسم_ناں T, قسم_ناں F>
  منطقي may_dispatch (ثابت T *obj HB_بن_ورتیا, ثابت F *format)
  {
    أرجع format->sanitize (هذا) &&
	   hb_barrier ();
  }
  ساكن return_t default_return_قدر () { أرجع سچ; }
  ساكن return_t no_dispatch_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (ثابت return_t r) ثابت { أرجع !r; }

  منطقي ویکھو_subtables (بنانشانی گنتی)
  {
    max_subtables += گنتی;
    أرجع max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  خاص:
  سانچا <قسم_ناں T, قسم_ناں ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (هذا, std::forward<Ts> (ds)...) )
  سانچا <قسم_ناں T, قسم_ناں ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (هذا, std::forward<Ts> (ds)...) )
  عام:
  سانچا <قسم_ناں T, قسم_ناں ...Ts> خودکار
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
      سرا_processing ();
  }

  فراغ init (hb_blob_t *b)
  {
    هذا->blob = hb_blob_reference (b);
    هذا->writable = خطأ;
  }

  فراغ سیٹ_عدد_glyphs (بنانشانی صحيح عدد_glyphs_2)
  {
    عدد_glyphs = عدد_glyphs_2;
    عدد_glyphs_سیٹ = سچ;
  }
  بنانشانی صحيح get_عدد_glyphs () { أرجع عدد_glyphs; }

  فراغ سیٹ_max_ops (صحيح max_ops_) { max_ops = max_ops_; }

  سانچا <قسم_ناں T>
  فراغ سیٹ_object (ثابت T *obj)
  {
    reset_object ();

    إذا (!obj) أرجع;

    ثابت حرف *obj_شروع = (ثابت حرف *) obj;
    إذا (unlikely (obj_شروع < هذا->شروع || هذا->سرا <= obj_شروع))
    {
      هذا->شروع = هذا->سرا = خالی_سوچک;
      هذا->لمائی = 0;
    }
    وإلا
    {
      هذا->شروع = obj_شروع;
      هذا->سرا   = obj_شروع + hb_min (size_t (هذا->سرا - obj_شروع), obj->get_ناپ ());
      هذا->لمائی = هذا->سرا - هذا->شروع;
    }
  }

  فراغ reset_object ()
  {
    إذا (هذا->blob)
    {
      هذا->شروع = هذا->blob->انکڑے;
      هذا->سرا = هذا->شروع + هذا->blob->لمائی;
    }
    هذا->لمائی = هذا->سرا - هذا->شروع;
    أكد (هذا->شروع <= هذا->سرا); /* Must not overflow. */
  }

  فراغ شروع_processing (ثابت حرف *شروع_2 = خالی_سوچک, ثابت حرف *سرا_4 = خالی_سوچک)
  {
    إذا (شروع_2)
    {
      هذا->شروع = شروع_2;
      هذا->سرا = سرا_4;
    }
    reset_object ();
    بنانشانی m;
    إذا (unlikely (hb_unsigned_mul_overflows (هذا->سرا - هذا->شروع, HB_SANITIZE_MAX_OPS_ضارب, &m)))
      هذا->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    وإلا
      هذا->max_ops = hb_clamp (m,
				(بنانشانی) HB_SANITIZE_MAX_OPS_MIN,
				(بنانشانی) HB_SANITIZE_MAX_OPS_MAX);
    هذا->debug_ڈونگھائی = 0;
    هذا->recursion_ڈونگھائی = 0;

    DEBUG_پیغام_LEVEL (SANITIZE, شروع, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->شروع, هذا->سرا,
		     (بنانشانی لما) (هذا->سرا - هذا->شروع));
  }

  فراغ سرا_processing ()
  {
    DEBUG_پیغام_LEVEL (SANITIZE, هذا->شروع, 0, -1,
		     "end [%p..%p]",
		     هذا->شروع, هذا->سرا);

    hb_blob_destroy (هذا->blob);
    هذا->blob = خالی_سوچک;
    هذا->شروع = هذا->سرا = خالی_سوچک;
    هذا->لمائی = 0;
  }

  منطقي check_ops(بنانشانی گنتی)
  {
    /* Avoid underflow */
    إذا (unlikely (هذا->max_ops < 0 || گنتی >= (بنانشانی) هذا->max_ops))
    {
      هذا->max_ops = -1;
      أرجع خطأ;
    }
    هذا->max_ops -= (صحيح) گنتی;
    أرجع سچ;
  }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  منطقي check_range (ثابت فراغ *ادھار,
		    بنانشانی صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) ادھار;
    منطقي ok = (uintptr_t) (p - هذا->شروع) <= هذا->لمائی &&
	      (بنانشانی صحيح) (هذا->سرا - p) >= len &&
	      ((هذا->max_ops -= len) > 0);

    DEBUG_پیغام_LEVEL (SANITIZE, p, هذا->debug_ڈونگھائی+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->شروع, هذا->سرا,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  منطقي check_range_fast (ثابت فراغ *ادھار,
			 بنانشانی صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) ادھار;
    منطقي ok = ((uintptr_t) (p - هذا->شروع) <= هذا->لمائی &&
	       (بنانشانی صحيح) (هذا->سرا - p) >= len);

    DEBUG_پیغام_LEVEL (SANITIZE, p, هذا->debug_ڈونگھائی+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->شروع, هذا->سرا,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  منطقي check_نقطہ (ثابت فراغ *ادھار) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) ادھار;
    منطقي ok = (uintptr_t) (p - هذا->شروع) <= هذا->لمائی;

    DEBUG_پیغام_LEVEL (SANITIZE, p, هذا->debug_ڈونگھائی+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     هذا->شروع, هذا->سرا,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

  سانچا <قسم_ناں T>
  منطقي check_range (ثابت T *ادھار,
		    بنانشانی صحيح a,
		    بنانشانی صحيح b) ثابت
  {
    بنانشانی m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (ادھار, m);
  }

  سانچا <قسم_ناں T>
  منطقي check_range (ثابت T *ادھار,
		    بنانشانی صحيح a,
		    بنانشانی صحيح b,
		    بنانشانی صحيح c) ثابت
  {
    بنانشانی m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (ادھار, m, c);
  }

  سانچا <قسم_ناں T>
  HB_ALWAYS_INLINE
  منطقي check_array_sized (ثابت T *ادھار, بنانشانی صحيح len, بنانشانی len_ناپ) ثابت
  {
    إذا (len_ناپ >= 4)
    {
      إذا (unlikely (hb_unsigned_mul_overflows (len, hb_static_ناپ (T), &len)))
	أرجع خطأ;
    }
    وإلا
      len = len * hb_static_ناپ (T);
    أرجع هذا->check_range (ادھار, len);
  }

  سانچا <قسم_ناں T>
  منطقي check_array (ثابت T *ادھار, بنانشانی صحيح len) ثابت
  {
    أرجع هذا->check_range (ادھار, len, hb_static_ناپ (T));
  }

  سانچا <قسم_ناں T>
  منطقي check_array (ثابت T *ادھار,
		    بنانشانی صحيح a,
		    بنانشانی صحيح b) ثابت
  {
    أرجع هذا->check_range (ادھار, hb_static_ناپ (T), a, b);
  }

  منطقي check_شروع_recursion (صحيح max_ڈونگھائی)
  {
    إذا (unlikely (recursion_ڈونگھائی >= max_ڈونگھائی)) أرجع خطأ;
    أرجع ++recursion_ڈونگھائی;
  }

  منطقي سرا_recursion (منطقي نتیجہ)
  {
    recursion_ڈونگھائی--;
    أرجع نتیجہ;
  }

  سانچا <قسم_ناں Type>
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
  HB_ALWAYS_INLINE
#جے_انت
  منطقي check_struct (ثابت Type *obj) ثابت
  {
    إذا (حجم (uintptr_t) == حجم (uint32_t))
      أرجع likely (هذا->check_range_fast (obj, obj->min_ناپ));
    وإلا
      أرجع likely (هذا->check_نقطہ ((ثابت حرف *) obj + obj->min_ناپ));
  }

  سانچا <قسم_ناں Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    منطقي sane;

    init (blob);

    DEBUG_پیغام_FUNC (SANITIZE, شروع, "start");

    شروع_processing ();

    إذا (unlikely (!شروع))
    {
      سرا_processing ();
      أرجع blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<حرف *> (شروع));

    sane = t->sanitize (هذا);

    سرا_processing ();

    DEBUG_پیغام_FUNC (SANITIZE, شروع, sane ? "PASSED" : "FAILED");
    إذا (sane)
    {
      hb_blob_make_immutable (blob);
      أرجع blob;
    }
    وإلا
    {
      hb_blob_destroy (blob);
      أرجع hb_blob_get_خالی_تھاواں ();
    }
  }

  سانچا <قسم_ناں Type>
  hb_blob_t *reference_جدول (ثابت hb_face_t *face, hb_ٹیگ_t جدول_ٹیگ = Type::جدول_ٹیگ)
  {
    إذا (!عدد_glyphs_سیٹ)
      سیٹ_عدد_glyphs (hb_face_get_glyph_گنتی (face));
    أرجع sanitize_blob<Type> (hb_face_reference_جدول (face, جدول_ٹیگ));
  }

  ثابت حرف *شروع, *سرا;
  بنانشانی لمائی;
  بدلنجوگ صحيح max_ops, max_subtables;
  خاص:
  صحيح recursion_ڈونگھائی;
  منطقي writable;
  hb_blob_t *blob;
  بنانشانی صحيح عدد_glyphs;
  منطقي  عدد_glyphs_سیٹ;
  عام:
  منطقي lazy_some_gpos;
};

بنية hb_sanitize_with_object_t
{
  سانچا <قسم_ناں T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ثابت T& obj) : c (c)
  { c->سیٹ_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  خاص:
  hb_sanitize_context_t *c;
};


#جے_انت /* HB_SANITIZE_HH */
