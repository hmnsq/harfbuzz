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

#اگر_غیر_تعریف_شدہ HB_SANITIZE_HH
#تعریف_کرو HB_SANITIZE_HH

#شامل "hb.hh"
#شامل "hb-blob.hh"
#شامل "hb-dispatch.hh"


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
#اگر_غیر_تعریف_شدہ HB_SANITIZE_MAX_EDITS
#تعریف_کرو HB_SANITIZE_MAX_EDITS 32
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_SANITIZE_MAX_OPS_ضربی_عدد
#تعریف_کرو HB_SANITIZE_MAX_OPS_ضربی_عدد 64
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_SANITIZE_MAX_OPS_MIN
#تعریف_کرو HB_SANITIZE_MAX_OPS_MIN 16384
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_SANITIZE_MAX_OPS_MAX
#تعریف_کرو HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_SANITIZE_MAX_SUBTABLES
#تعریف_کرو HB_SANITIZE_MAX_SUBTABLES 0x4000
#اگر_ختم

ساخت hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, منطقی, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ثابت حرف *آغاز_2 = خالی_اشاریہ, ثابت حرف *انتہا_4 = خالی_اشاریہ) :
	آغاز (آغاز_2), انتہا (انتہا_4),
	لمبائی (0),
	max_ops (0), max_subtables (0),
        recursion_گہرائی (0),
	writable (جھوٹ),
	blob (خالی_اشاریہ),
	عدد_glyphs (65536),
	عدد_glyphs_سیٹ (جھوٹ),
	lazy_some_gpos (جھوٹ) {}

  ثابت حرف *get_name () { واپس "SANITIZE"; }
  سانچہ <قسم_نام T, قسم_نام F>
  منطقی may_dispatch (ثابت T *obj HB_غیر_مستعمل, ثابت F *format)
  {
    واپس format->sanitize (یہ) &&
	   hb_barrier ();
  }
  ساکن return_t default_return_قدر () { واپس سچ; }
  ساکن return_t no_dispatch_return_قدر () { واپس جھوٹ; }
  منطقی stop_sublookup_iteration (ثابت return_t r) ثابت { واپس !r; }

  منطقی دورہ_subtables (غیرعلامتی گنتی)
  {
    max_subtables += گنتی;
    واپس max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  نجی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (یہ, std::forward<Ts> (ds)...) )
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (یہ, std::forward<Ts> (ds)...) )
  عوامی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    اگر (blob)
      آغاز_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    اگر (blob)
      انتہا_processing ();
  }

  خالی init (hb_blob_t *b)
  {
    یہ->blob = hb_blob_reference (b);
    یہ->writable = جھوٹ;
  }

  خالی سیٹ_عدد_glyphs (غیرعلامتی صحیح عدد_glyphs_2)
  {
    عدد_glyphs = عدد_glyphs_2;
    عدد_glyphs_سیٹ = سچ;
  }
  غیرعلامتی صحیح get_عدد_glyphs () { واپس عدد_glyphs; }

  خالی سیٹ_max_ops (صحیح max_ops_) { max_ops = max_ops_; }

  سانچہ <قسم_نام T>
  خالی سیٹ_object (ثابت T *obj)
  {
    reset_object ();

    اگر (!obj) واپس;

    ثابت حرف *obj_آغاز = (ثابت حرف *) obj;
    اگر (unlikely (obj_آغاز < یہ->آغاز || یہ->انتہا <= obj_آغاز))
    {
      یہ->آغاز = یہ->انتہا = خالی_اشاریہ;
      یہ->لمبائی = 0;
    }
    ورنہ
    {
      یہ->آغاز = obj_آغاز;
      یہ->انتہا   = obj_آغاز + hb_min (size_t (یہ->انتہا - obj_آغاز), obj->get_سائز ());
      یہ->لمبائی = یہ->انتہا - یہ->آغاز;
    }
  }

  خالی reset_object ()
  {
    اگر (یہ->blob)
    {
      یہ->آغاز = یہ->blob->ڈیٹا;
      یہ->انتہا = یہ->آغاز + یہ->blob->لمبائی;
    }
    یہ->لمبائی = یہ->انتہا - یہ->آغاز;
    أكد (یہ->آغاز <= یہ->انتہا); /* Must not overflow. */
  }

  خالی آغاز_processing (ثابت حرف *آغاز_2 = خالی_اشاریہ, ثابت حرف *انتہا_4 = خالی_اشاریہ)
  {
    اگر (آغاز_2)
    {
      یہ->آغاز = آغاز_2;
      یہ->انتہا = انتہا_4;
    }
    reset_object ();
    غیرعلامتی m;
    اگر (unlikely (hb_unsigned_mul_overflows (یہ->انتہا - یہ->آغاز, HB_SANITIZE_MAX_OPS_ضربی_عدد, &m)))
      یہ->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    ورنہ
      یہ->max_ops = hb_clamp (m,
				(غیرعلامتی) HB_SANITIZE_MAX_OPS_MIN,
				(غیرعلامتی) HB_SANITIZE_MAX_OPS_MAX);
    یہ->debug_گہرائی = 0;
    یہ->recursion_گہرائی = 0;

    DEBUG_پیغام_LEVEL (SANITIZE, آغاز, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     یہ->آغاز, یہ->انتہا,
		     (غیرعلامتی طویل) (یہ->انتہا - یہ->آغاز));
  }

  خالی انتہا_processing ()
  {
    DEBUG_پیغام_LEVEL (SANITIZE, یہ->آغاز, 0, -1,
		     "end [%p..%p]",
		     یہ->آغاز, یہ->انتہا);

    hb_blob_destroy (یہ->blob);
    یہ->blob = خالی_اشاریہ;
    یہ->آغاز = یہ->انتہا = خالی_اشاریہ;
    یہ->لمبائی = 0;
  }

  منطقی check_ops(غیرعلامتی گنتی)
  {
    /* Avoid underflow */
    اگر (unlikely (یہ->max_ops < 0 || گنتی >= (غیرعلامتی) یہ->max_ops))
    {
      یہ->max_ops = -1;
      واپس جھوٹ;
    }
    یہ->max_ops -= (صحیح) گنتی;
    واپس سچ;
  }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  منطقی check_range (ثابت خالی *بنیاد,
		    غیرعلامتی صحیح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) بنیاد;
    منطقی ok = (uintptr_t) (p - یہ->آغاز) <= یہ->لمبائی &&
	      (غیرعلامتی صحیح) (یہ->انتہا - p) >= len &&
	      ((یہ->max_ops -= len) > 0);

    DEBUG_پیغام_LEVEL (SANITIZE, p, یہ->debug_گہرائی+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     یہ->آغاز, یہ->انتہا,
		     ok ? "OK" : "OUT-OF-RANGE");

    واپس likely (ok);
  }
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  منطقی check_range_fast (ثابت خالی *بنیاد,
			 غیرعلامتی صحیح len) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) بنیاد;
    منطقی ok = ((uintptr_t) (p - یہ->آغاز) <= یہ->لمبائی &&
	       (غیرعلامتی صحیح) (یہ->انتہا - p) >= len);

    DEBUG_پیغام_LEVEL (SANITIZE, p, یہ->debug_گہرائی+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     یہ->آغاز, یہ->انتہا,
		     ok ? "OK" : "OUT-OF-RANGE");

    واپس likely (ok);
  }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  منطقی check_نقطہ (ثابت خالی *بنیاد) ثابت
  {
    ثابت حرف *p = (ثابت حرف *) بنیاد;
    منطقی ok = (uintptr_t) (p - یہ->آغاز) <= یہ->لمبائی;

    DEBUG_پیغام_LEVEL (SANITIZE, p, یہ->debug_گہرائی+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     یہ->آغاز, یہ->انتہا,
		     ok ? "OK" : "OUT-OF-RANGE");

    واپس likely (ok);
  }

  سانچہ <قسم_نام T>
  منطقی check_range (ثابت T *بنیاد,
		    غیرعلامتی صحیح a,
		    غیرعلامتی صحیح b) ثابت
  {
    غیرعلامتی m;
    واپس !hb_unsigned_mul_overflows (a, b, &m) &&
	   یہ->check_range (بنیاد, m);
  }

  سانچہ <قسم_نام T>
  منطقی check_range (ثابت T *بنیاد,
		    غیرعلامتی صحیح a,
		    غیرعلامتی صحیح b,
		    غیرعلامتی صحیح c) ثابت
  {
    غیرعلامتی m;
    واپس !hb_unsigned_mul_overflows (a, b, &m) &&
	   یہ->check_range (بنیاد, m, c);
  }

  سانچہ <قسم_نام T>
  HB_ALWAYS_INLINE
  منطقی check_array_sized (ثابت T *بنیاد, غیرعلامتی صحیح len, غیرعلامتی len_سائز) ثابت
  {
    اگر (len_سائز >= 4)
    {
      اگر (unlikely (hb_unsigned_mul_overflows (len, hb_static_سائز (T), &len)))
	واپس جھوٹ;
    }
    ورنہ
      len = len * hb_static_سائز (T);
    واپس یہ->check_range (بنیاد, len);
  }

  سانچہ <قسم_نام T>
  منطقی check_array (ثابت T *بنیاد, غیرعلامتی صحیح len) ثابت
  {
    واپس یہ->check_range (بنیاد, len, hb_static_سائز (T));
  }

  سانچہ <قسم_نام T>
  منطقی check_array (ثابت T *بنیاد,
		    غیرعلامتی صحیح a,
		    غیرعلامتی صحیح b) ثابت
  {
    واپس یہ->check_range (بنیاد, hb_static_سائز (T), a, b);
  }

  منطقی check_آغاز_recursion (صحیح max_گہرائی)
  {
    اگر (unlikely (recursion_گہرائی >= max_گہرائی)) واپس جھوٹ;
    واپس ++recursion_گہرائی;
  }

  منطقی انتہا_recursion (منطقی نتیجہ)
  {
    recursion_گہرائی--;
    واپس نتیجہ;
  }

  سانچہ <قسم_نام Type>
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  منطقی check_struct (ثابت Type *obj) ثابت
  {
    اگر (حجم (uintptr_t) == حجم (uint32_t))
      واپس likely (یہ->check_range_fast (obj, obj->min_سائز));
    ورنہ
      واپس likely (یہ->check_نقطہ ((ثابت حرف *) obj + obj->min_سائز));
  }

  سانچہ <قسم_نام Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    منطقی sane;

    init (blob);

    DEBUG_پیغام_FUNC (SANITIZE, آغاز, "start");

    آغاز_processing ();

    اگر (unlikely (!آغاز))
    {
      انتہا_processing ();
      واپس blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<حرف *> (آغاز));

    sane = t->sanitize (یہ);

    انتہا_processing ();

    DEBUG_پیغام_FUNC (SANITIZE, آغاز, sane ? "PASSED" : "FAILED");
    اگر (sane)
    {
      hb_blob_make_immutable (blob);
      واپس blob;
    }
    ورنہ
    {
      hb_blob_destroy (blob);
      واپس hb_blob_get_خالی_خانے ();
    }
  }

  سانچہ <قسم_نام Type>
  hb_blob_t *reference_جدول (ثابت hb_face_t *face, hb_لیبل_t جدول_لیبل = Type::جدول_لیبل)
  {
    اگر (!عدد_glyphs_سیٹ)
      سیٹ_عدد_glyphs (hb_face_get_glyph_گنتی (face));
    واپس sanitize_blob<Type> (hb_face_reference_جدول (face, جدول_لیبل));
  }

  ثابت حرف *آغاز, *انتہا;
  غیرعلامتی لمبائی;
  تغیرپذیر صحیح max_ops, max_subtables;
  نجی:
  صحیح recursion_گہرائی;
  منطقی writable;
  hb_blob_t *blob;
  غیرعلامتی صحیح عدد_glyphs;
  منطقی  عدد_glyphs_سیٹ;
  عوامی:
  منطقی lazy_some_gpos;
};

ساخت hb_sanitize_with_object_t
{
  سانچہ <قسم_نام T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ثابت T& obj) : c (c)
  { c->سیٹ_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  نجی:
  hb_sanitize_context_t *c;
};


#اگر_ختم /* HB_SANITIZE_HH */
