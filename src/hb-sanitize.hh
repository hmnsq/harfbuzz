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

#جيڪڏهن_وصف_نه_ٿيل HB_SANITIZE_HH
#وصف_ڪريو HB_SANITIZE_HH

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
#جيڪڏهن_وصف_نه_ٿيل HB_SANITIZE_MAX_EDITS
#وصف_ڪريو HB_SANITIZE_MAX_EDITS 32
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_SANITIZE_MAX_OPS_ضرب_ڪندڙ
#وصف_ڪريو HB_SANITIZE_MAX_OPS_ضرب_ڪندڙ 64
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_SANITIZE_MAX_OPS_MIN
#وصف_ڪريو HB_SANITIZE_MAX_OPS_MIN 16384
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_SANITIZE_MAX_OPS_MAX
#وصف_ڪريو HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_SANITIZE_MAX_SUBTABLES
#وصف_ڪريو HB_SANITIZE_MAX_SUBTABLES 0x4000
#جيڪڏهن_پورو

بنية hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, منطقي, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ثابت حرف *شروعات_2 = خالي_اشارو, ثابت حرف *پڇاڙي_4 = خالي_اشارو) :
	شروعات (شروعات_2), پڇاڙي (پڇاڙي_4),
	ڊيگهائي (0),
	max_ops (0), max_subtables (0),
        recursion_اونهائي (0),
	writable (خطأ),
	blob (خالي_اشارو),
	انگ_glyphs (65536),
	انگ_glyphs_سيٽ (خطأ),
	lazy_some_gpos (خطأ) {}

  ثابت حرف *get_name () { أرجع "SANITIZE"; }
  سانچو <قسم_نالو T, قسم_نالو F>
  منطقي may_dispatch (ثابت T *obj HB_اڻ_استعمال_ٿيل, ثابت F *format)
  {
    أرجع format->sanitize (هذا) &&
	   hb_barrier ();
  }
  ساكن return_t default_return_قدر () { أرجع سچ; }
  ساكن return_t no_dispatch_return_قدر () { أرجع خطأ; }
  منطقي stop_sublookup_iteration (ثابت return_t r) ثابت { أرجع !r; }

  منطقي گهمو_subtables (بنانشان ڳڻپ)
  {
    max_subtables += ڳڻپ;
    أرجع max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  خاص:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (هذا, std::forward<Ts> (ds)...) )
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (هذا, std::forward<Ts> (ds)...) )
  عام:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    إذا (blob)
      شروعات_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    إذا (blob)
      پڇاڙي_processing ();
  }

  فراغ init (hb_blob_t *b)
  {
    هذا->blob = hb_blob_reference (b);
    هذا->writable = خطأ;
  }

  فراغ سيٽ_انگ_glyphs (بنانشان صحيح انگ_glyphs_2)
  {
    انگ_glyphs = انگ_glyphs_2;
    انگ_glyphs_سيٽ = سچ;
  }
  بنانشان صحيح get_انگ_glyphs () { أرجع انگ_glyphs; }

  فراغ سيٽ_max_ops (صحيح max_ops_) { max_ops = max_ops_; }

  سانچو <قسم_نالو T>
  فراغ سيٽ_object (ثابت T *obj)
  {
    reset_object ();

    إذا (!obj) أرجع;

    ثابت حرف *obj_شروعات = (ثابت حرف *) obj;
    إذا (unlikely (obj_شروعات < هذا->شروعات || هذا->پڇاڙي <= obj_شروعات))
    {
      هذا->شروعات = هذا->پڇاڙي = خالي_اشارو;
      هذا->ڊيگهائي = 0;
    }
    وإلا
    {
      هذا->شروعات = obj_شروعات;
      هذا->پڇاڙي   = obj_شروعات + hb_min (size_t (هذا->پڇاڙي - obj_شروعات), obj->get_سائيز ());
      هذا->ڊيگهائي = هذا->پڇاڙي - هذا->شروعات;
    }
  }

  فراغ reset_object ()
  {
    إذا (هذا->blob)
    {
      هذا->شروعات = هذا->blob->معلومات;
      هذا->پڇاڙي = هذا->شروعات + هذا->blob->ڊيگهائي;
    }
    هذا->ڊيگهائي = هذا->پڇاڙي - هذا->شروعات;
    أكد (هذا->شروعات <= هذا->پڇاڙي); /* Must not overflow. */
  }

  فراغ شروعات_processing (ثابت حرف *شروعات_2 = خالي_اشارو, ثابت حرف *پڇاڙي_4 = خالي_اشارو)
  {
    إذا (شروعات_2)
    {
      هذا->شروعات = شروعات_2;
      هذا->پڇاڙي = پڇاڙي_4;
    }
    reset_object ();
    بنانشان m;
    إذا (unlikely (hb_unsigned_mul_overflows (هذا->پڇاڙي - هذا->شروعات, HB_SANITIZE_MAX_OPS_ضرب_ڪندڙ, &m)))
      هذا->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    وإلا
      هذا->max_ops = hb_clamp (m,
				(بنانشان) HB_SANITIZE_MAX_OPS_MIN,
				(بنانشان) HB_SANITIZE_MAX_OPS_MAX);
    هذا->debug_اونهائي = 0;
    هذا->recursion_اونهائي = 0;

    DEBUG_نياپو_LEVEL (SANITIZE, شروعات, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->شروعات, هذا->پڇاڙي,
		     (بنانشان ڊگهو) (هذا->پڇاڙي - هذا->شروعات));
  }

  فراغ پڇاڙي_processing ()
  {
    DEBUG_نياپو_LEVEL (SANITIZE, هذا->شروعات, 0, -1,
		     "end [%p..%p]",
		     هذا->شروعات, هذا->پڇاڙي);

    hb_blob_destroy (هذا->blob);
    هذا->blob = خالي_اشارو;
    هذا->شروعات = هذا->پڇاڙي = خالي_اشارو;
    هذا->ڊيگهائي = 0;
  }

  منطقي check_ops(بنانشان ڳڻپ)
  {
    /* Avoid underflow */
    إذا (unlikely (هذا->max_ops < 0 || ڳڻپ >= (بنانشان) هذا->max_ops))
    {
      هذا->max_ops = -1;
      أرجع خطأ;
    }
    هذا->max_ops -= (صحيح) ڳڻپ;
    أرجع سچ;
  }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  منطقي check_range (ثابت فراغ *بنياد,
		    بنانشان صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) بنياد;
    منطقي ok = (uintptr_t) (p - هذا->شروعات) <= هذا->ڊيگهائي &&
	      (بنانشان صحيح) (هذا->پڇاڙي - p) >= len &&
	      ((هذا->max_ops -= len) > 0);

    DEBUG_نياپو_LEVEL (SANITIZE, p, هذا->debug_اونهائي+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->شروعات, هذا->پڇاڙي,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  منطقي check_range_fast (ثابت فراغ *بنياد,
			 بنانشان صحيح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) بنياد;
    منطقي ok = ((uintptr_t) (p - هذا->شروعات) <= هذا->ڊيگهائي &&
	       (بنانشان صحيح) (هذا->پڇاڙي - p) >= len);

    DEBUG_نياپو_LEVEL (SANITIZE, p, هذا->debug_اونهائي+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     هذا->شروعات, هذا->پڇاڙي,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  منطقي check_نقطو (ثابت فراغ *بنياد) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) بنياد;
    منطقي ok = (uintptr_t) (p - هذا->شروعات) <= هذا->ڊيگهائي;

    DEBUG_نياپو_LEVEL (SANITIZE, p, هذا->debug_اونهائي+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     هذا->شروعات, هذا->پڇاڙي,
		     ok ? "OK" : "OUT-OF-RANGE");

    أرجع likely (ok);
  }

  سانچو <قسم_نالو T>
  منطقي check_range (ثابت T *بنياد,
		    بنانشان صحيح a,
		    بنانشان صحيح b) ثابت
  {
    بنانشان m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (بنياد, m);
  }

  سانچو <قسم_نالو T>
  منطقي check_range (ثابت T *بنياد,
		    بنانشان صحيح a,
		    بنانشان صحيح b,
		    بنانشان صحيح c) ثابت
  {
    بنانشان m;
    أرجع !hb_unsigned_mul_overflows (a, b, &m) &&
	   هذا->check_range (بنياد, m, c);
  }

  سانچو <قسم_نالو T>
  HB_ALWAYS_INLINE
  منطقي check_array_sized (ثابت T *بنياد, بنانشان صحيح len, بنانشان len_سائيز) ثابت
  {
    إذا (len_سائيز >= 4)
    {
      إذا (unlikely (hb_unsigned_mul_overflows (len, hb_static_سائيز (T), &len)))
	أرجع خطأ;
    }
    وإلا
      len = len * hb_static_سائيز (T);
    أرجع هذا->check_range (بنياد, len);
  }

  سانچو <قسم_نالو T>
  منطقي check_array (ثابت T *بنياد, بنانشان صحيح len) ثابت
  {
    أرجع هذا->check_range (بنياد, len, hb_static_سائيز (T));
  }

  سانچو <قسم_نالو T>
  منطقي check_array (ثابت T *بنياد,
		    بنانشان صحيح a,
		    بنانشان صحيح b) ثابت
  {
    أرجع هذا->check_range (بنياد, hb_static_سائيز (T), a, b);
  }

  منطقي check_شروعات_recursion (صحيح max_اونهائي)
  {
    إذا (unlikely (recursion_اونهائي >= max_اونهائي)) أرجع خطأ;
    أرجع ++recursion_اونهائي;
  }

  منطقي پڇاڙي_recursion (منطقي نتيجو)
  {
    recursion_اونهائي--;
    أرجع نتيجو;
  }

  سانچو <قسم_نالو Type>
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
  HB_ALWAYS_INLINE
#جيڪڏهن_پورو
  منطقي check_struct (ثابت Type *obj) ثابت
  {
    إذا (حجم (uintptr_t) == حجم (uint32_t))
      أرجع likely (هذا->check_range_fast (obj, obj->min_سائيز));
    وإلا
      أرجع likely (هذا->check_نقطو ((ثابت حرف *) obj + obj->min_سائيز));
  }

  سانچو <قسم_نالو Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    منطقي sane;

    init (blob);

    DEBUG_نياپو_FUNC (SANITIZE, شروعات, "start");

    شروعات_processing ();

    إذا (unlikely (!شروعات))
    {
      پڇاڙي_processing ();
      أرجع blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<حرف *> (شروعات));

    sane = t->sanitize (هذا);

    پڇاڙي_processing ();

    DEBUG_نياپو_FUNC (SANITIZE, شروعات, sane ? "PASSED" : "FAILED");
    إذا (sane)
    {
      hb_blob_make_immutable (blob);
      أرجع blob;
    }
    وإلا
    {
      hb_blob_destroy (blob);
      أرجع hb_blob_get_خالي_جايون ();
    }
  }

  سانچو <قسم_نالو Type>
  hb_blob_t *reference_جدول (ثابت hb_face_t *face, hb_ٽيگ_t جدول_ٽيگ = Type::جدول_ٽيگ)
  {
    إذا (!انگ_glyphs_سيٽ)
      سيٽ_انگ_glyphs (hb_face_get_glyph_ڳڻپ (face));
    أرجع sanitize_blob<Type> (hb_face_reference_جدول (face, جدول_ٽيگ));
  }

  ثابت حرف *شروعات, *پڇاڙي;
  بنانشان ڊيگهائي;
  بدلجندڙ صحيح max_ops, max_subtables;
  خاص:
  صحيح recursion_اونهائي;
  منطقي writable;
  hb_blob_t *blob;
  بنانشان صحيح انگ_glyphs;
  منطقي  انگ_glyphs_سيٽ;
  عام:
  منطقي lazy_some_gpos;
};

بنية hb_sanitize_with_object_t
{
  سانچو <قسم_نالو T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ثابت T& obj) : c (c)
  { c->سيٽ_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  خاص:
  hb_sanitize_context_t *c;
};


#جيڪڏهن_پورو /* HB_SANITIZE_HH */
