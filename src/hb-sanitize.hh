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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SANITIZE_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SANITIZE_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-blob.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-dispatch.hh"


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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SANITIZE_MAX_EDITS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SANITIZE_MAX_EDITS 32
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SANITIZE_MAX_OPS_ਗੁਣਕ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SANITIZE_MAX_OPS_ਗੁਣਕ 64
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SANITIZE_MAX_OPS_MIN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SANITIZE_MAX_OPS_MIN 16384
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SANITIZE_MAX_OPS_MAX
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SANITIZE_MAX_SUBTABLES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SANITIZE_MAX_SUBTABLES 0x4000
#ਜੇ_ਅੰਤ

ਬਣਤਰ hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, ਤਾਰਕਿਕ, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ_2 = ਖਾਲੀ_ਸੂਚਕ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ_4 = ਖਾਲੀ_ਸੂਚਕ) :
	ਸ਼ੁਰੂ (ਸ਼ੁਰੂ_2), ਸਿਰਾ (ਸਿਰਾ_4),
	ਲੰਮਾਈ (0),
	max_ops (0), max_subtables (0),
        recursion_ਡੂੰਘਾਈ (0),
	writable (ਝੂਠ),
	blob (ਖਾਲੀ_ਸੂਚਕ),
	ਅੰਕ_glyphs (65536),
	ਅੰਕ_glyphs_ਸੈੱਟ (ਝੂਠ),
	lazy_some_gpos (ਝੂਠ) {}

  ਅਚਲ ਅੱਖਰ *get_name () { ਵਾਪਸ "SANITIZE"; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ F>
  ਤਾਰਕਿਕ may_dispatch (ਅਚਲ T *obj HB_ਅਣਵਰਤਿਆ, ਅਚਲ F *format)
  {
    ਵਾਪਸ format->sanitize (ਇਹ) &&
	   hb_barrier ();
  }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ ਸੱਚ; }
  ਸਥਿਰ return_t no_dispatch_return_ਮੁੱਲ () { ਵਾਪਸ ਝੂਠ; }
  ਤਾਰਕਿਕ stop_sublookup_iteration (ਅਚਲ return_t r) ਅਚਲ { ਵਾਪਸ !r; }

  ਤਾਰਕਿਕ ਦੇਖੋ_subtables (ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  {
    max_subtables += ਗਿਣਤੀ_2;
    ਵਾਪਸ max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (ਇਹ, std::forward<Ts> (ds)...) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (ਇਹ, std::forward<Ts> (ds)...) )
  ਜਨਤਕ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  dispatch (ਅਚਲ T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    ਜੇ (blob)
      ਸ਼ੁਰੂ_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    ਜੇ (blob)
      ਸਿਰਾ_processing ();
  }

  ਖਾਲੀ init (hb_blob_t *b)
  {
    ਇਹ->blob = hb_blob_reference (b);
    ਇਹ->writable = ਝੂਠ;
  }

  ਖਾਲੀ ਸੈੱਟ_ਅੰਕ_glyphs (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs_2)
  {
    ਅੰਕ_glyphs = ਅੰਕ_glyphs_2;
    ਅੰਕ_glyphs_ਸੈੱਟ = ਸੱਚ;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_ਅੰਕ_glyphs () { ਵਾਪਸ ਅੰਕ_glyphs; }

  ਖਾਲੀ ਸੈੱਟ_max_ops (ਪੂਰਨ max_ops_) { max_ops = max_ops_; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਸੈੱਟ_object (ਅਚਲ T *obj)
  {
    reset_object ();

    ਜੇ (!obj) ਵਾਪਸ;

    ਅਚਲ ਅੱਖਰ *obj_ਸ਼ੁਰੂ = (ਅਚਲ ਅੱਖਰ *) obj;
    ਜੇ (unlikely (obj_ਸ਼ੁਰੂ < ਇਹ->ਸ਼ੁਰੂ || ਇਹ->ਸਿਰਾ <= obj_ਸ਼ੁਰੂ))
    {
      ਇਹ->ਸ਼ੁਰੂ = ਇਹ->ਸਿਰਾ = ਖਾਲੀ_ਸੂਚਕ;
      ਇਹ->ਲੰਮਾਈ = 0;
    }
    ਨਹੀਂਤਾਂ
    {
      ਇਹ->ਸ਼ੁਰੂ = obj_ਸ਼ੁਰੂ;
      ਇਹ->ਸਿਰਾ   = obj_ਸ਼ੁਰੂ + hb_min (size_t (ਇਹ->ਸਿਰਾ - obj_ਸ਼ੁਰੂ), obj->get_ਮਾਪ ());
      ਇਹ->ਲੰਮਾਈ = ਇਹ->ਸਿਰਾ - ਇਹ->ਸ਼ੁਰੂ;
    }
  }

  ਖਾਲੀ reset_object ()
  {
    ਜੇ (ਇਹ->blob)
    {
      ਇਹ->ਸ਼ੁਰੂ = ਇਹ->blob->ਅੰਕੜੇ;
      ਇਹ->ਸਿਰਾ = ਇਹ->ਸ਼ੁਰੂ + ਇਹ->blob->ਲੰਮਾਈ;
    }
    ਇਹ->ਲੰਮਾਈ = ਇਹ->ਸਿਰਾ - ਇਹ->ਸ਼ੁਰੂ;
    ਪ੍ਰਤਿਜ੍ਞਾ (ਇਹ->ਸ਼ੁਰੂ <= ਇਹ->ਸਿਰਾ); /* Must not overflow. */
  }

  ਖਾਲੀ ਸ਼ੁਰੂ_processing (ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ_2 = ਖਾਲੀ_ਸੂਚਕ, ਅਚਲ ਅੱਖਰ *ਸਿਰਾ_4 = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਜੇ (ਸ਼ੁਰੂ_2)
    {
      ਇਹ->ਸ਼ੁਰੂ = ਸ਼ੁਰੂ_2;
      ਇਹ->ਸਿਰਾ = ਸਿਰਾ_4;
    }
    reset_object ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ m;
    ਜੇ (unlikely (hb_unsigned_mul_overflows (ਇਹ->ਸਿਰਾ - ਇਹ->ਸ਼ੁਰੂ, HB_SANITIZE_MAX_OPS_ਗੁਣਕ, &m)))
      ਇਹ->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    ਨਹੀਂਤਾਂ
      ਇਹ->max_ops = hb_clamp (m,
				(ਬਿਨਾਨਿਸ਼ਾਨੀ) HB_SANITIZE_MAX_OPS_MIN,
				(ਬਿਨਾਨਿਸ਼ਾਨੀ) HB_SANITIZE_MAX_OPS_MAX);
    ਇਹ->debug_ਡੂੰਘਾਈ = 0;
    ਇਹ->recursion_ਡੂੰਘਾਈ = 0;

    DEBUG_ਸੁਨੇਹਾ_LEVEL (SANITIZE, ਸ਼ੁਰੂ, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ਇਹ->ਸ਼ੁਰੂ, ਇਹ->ਸਿਰਾ,
		     (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) (ਇਹ->ਸਿਰਾ - ਇਹ->ਸ਼ੁਰੂ));
  }

  ਖਾਲੀ ਸਿਰਾ_processing ()
  {
    DEBUG_ਸੁਨੇਹਾ_LEVEL (SANITIZE, ਇਹ->ਸ਼ੁਰੂ, 0, -1,
		     "end [%p..%p]",
		     ਇਹ->ਸ਼ੁਰੂ, ਇਹ->ਸਿਰਾ);

    hb_blob_destroy (ਇਹ->blob);
    ਇਹ->blob = ਖਾਲੀ_ਸੂਚਕ;
    ਇਹ->ਸ਼ੁਰੂ = ਇਹ->ਸਿਰਾ = ਖਾਲੀ_ਸੂਚਕ;
    ਇਹ->ਲੰਮਾਈ = 0;
  }

  ਤਾਰਕਿਕ check_ops(ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2)
  {
    /* Avoid underflow */
    ਜੇ (unlikely (ਇਹ->max_ops < 0 || ਗਿਣਤੀ_2 >= (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਇਹ->max_ops))
    {
      ਇਹ->max_ops = -1;
      ਵਾਪਸ ਝੂਠ;
    }
    ਇਹ->max_ops -= (ਪੂਰਨ) ਗਿਣਤੀ_2;
    ਵਾਪਸ ਸੱਚ;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ check_range (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len) ਅਚਲ
  {
    ਅਚਲ ਅੱਖਰ *p = (ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ;
    ਤਾਰਕਿਕ ok = (uintptr_t) (p - ਇਹ->ਸ਼ੁਰੂ) <= ਇਹ->ਲੰਮਾਈ &&
	      (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (ਇਹ->ਸਿਰਾ - p) >= len &&
	      ((ਇਹ->max_ops -= len) > 0);

    DEBUG_ਸੁਨੇਹਾ_LEVEL (SANITIZE, p, ਇਹ->debug_ਡੂੰਘਾਈ+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ਇਹ->ਸ਼ੁਰੂ, ਇਹ->ਸਿਰਾ,
		     ok ? "OK" : "OUT-OF-RANGE");

    ਵਾਪਸ likely (ok);
  }
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ check_range_fast (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
			 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len) ਅਚਲ
  {
    ਅਚਲ ਅੱਖਰ *p = (ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ;
    ਤਾਰਕਿਕ ok = ((uintptr_t) (p - ਇਹ->ਸ਼ੁਰੂ) <= ਇਹ->ਲੰਮਾਈ &&
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (ਇਹ->ਸਿਰਾ - p) >= len);

    DEBUG_ਸੁਨੇਹਾ_LEVEL (SANITIZE, p, ਇਹ->debug_ਡੂੰਘਾਈ+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ਇਹ->ਸ਼ੁਰੂ, ਇਹ->ਸਿਰਾ,
		     ok ? "OK" : "OUT-OF-RANGE");

    ਵਾਪਸ likely (ok);
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ check_ਬਿੰਦੂ (ਅਚਲ ਖਾਲੀ *ਅਧਾਰ) ਅਚਲ
  {
    ਅਚਲ ਅੱਖਰ *p = (ਅਚਲ ਅੱਖਰ *) ਅਧਾਰ;
    ਤਾਰਕਿਕ ok = (uintptr_t) (p - ਇਹ->ਸ਼ੁਰੂ) <= ਇਹ->ਲੰਮਾਈ;

    DEBUG_ਸੁਨੇਹਾ_LEVEL (SANITIZE, p, ਇਹ->debug_ਡੂੰਘਾਈ+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     ਇਹ->ਸ਼ੁਰੂ, ਇਹ->ਸਿਰਾ,
		     ok ? "OK" : "OUT-OF-RANGE");

    ਵਾਪਸ likely (ok);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ check_range (ਅਚਲ T *ਅਧਾਰ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ m;
    ਵਾਪਸ !hb_unsigned_mul_overflows (a, b, &m) &&
	   ਇਹ->check_range (ਅਧਾਰ, m);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ check_range (ਅਚਲ T *ਅਧਾਰ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ c) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ m;
    ਵਾਪਸ !hb_unsigned_mul_overflows (a, b, &m) &&
	   ਇਹ->check_range (ਅਧਾਰ, m, c);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  HB_ALWAYS_INLINE
  ਤਾਰਕਿਕ check_array_sized (ਅਚਲ T *ਅਧਾਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len, ਬਿਨਾਨਿਸ਼ਾਨੀ len_ਮਾਪ) ਅਚਲ
  {
    ਜੇ (len_ਮਾਪ >= 4)
    {
      ਜੇ (unlikely (hb_unsigned_mul_overflows (len, hb_static_ਮਾਪ (T), &len)))
	ਵਾਪਸ ਝੂਠ;
    }
    ਨਹੀਂਤਾਂ
      len = len * hb_static_ਮਾਪ (T);
    ਵਾਪਸ ਇਹ->check_range (ਅਧਾਰ, len);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ check_array (ਅਚਲ T *ਅਧਾਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len) ਅਚਲ
  {
    ਵਾਪਸ ਇਹ->check_range (ਅਧਾਰ, len, hb_static_ਮਾਪ (T));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ check_array (ਅਚਲ T *ਅਧਾਰ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ a,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ b) ਅਚਲ
  {
    ਵਾਪਸ ਇਹ->check_range (ਅਧਾਰ, hb_static_ਮਾਪ (T), a, b);
  }

  ਤਾਰਕਿਕ check_ਸ਼ੁਰੂ_recursion (ਪੂਰਨ max_ਡੂੰਘਾਈ)
  {
    ਜੇ (unlikely (recursion_ਡੂੰਘਾਈ >= max_ਡੂੰਘਾਈ)) ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ ++recursion_ਡੂੰਘਾਈ;
  }

  ਤਾਰਕਿਕ ਸਿਰਾ_recursion (ਤਾਰਕਿਕ ਨਤੀਜਾ)
  {
    recursion_ਡੂੰਘਾਈ--;
    ਵਾਪਸ ਨਤੀਜਾ;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਤਾਰਕਿਕ check_struct (ਅਚਲ Type *obj) ਅਚਲ
  {
    ਜੇ (ਆਕਾਰ (uintptr_t) == ਆਕਾਰ (uint32_t))
      ਵਾਪਸ likely (ਇਹ->check_range_fast (obj, obj->min_ਮਾਪ));
    ਨਹੀਂਤਾਂ
      ਵਾਪਸ likely (ਇਹ->check_ਬਿੰਦੂ ((ਅਚਲ ਅੱਖਰ *) obj + obj->min_ਮਾਪ));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    ਤਾਰਕਿਕ sane;

    init (blob);

    DEBUG_ਸੁਨੇਹਾ_FUNC (SANITIZE, ਸ਼ੁਰੂ, "start");

    ਸ਼ੁਰੂ_processing ();

    ਜੇ (unlikely (!ਸ਼ੁਰੂ))
    {
      ਸਿਰਾ_processing ();
      ਵਾਪਸ blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<ਅੱਖਰ *> (ਸ਼ੁਰੂ));

    sane = t->sanitize (ਇਹ);

    ਸਿਰਾ_processing ();

    DEBUG_ਸੁਨੇਹਾ_FUNC (SANITIZE, ਸ਼ੁਰੂ, sane ? "PASSED" : "FAILED");
    ਜੇ (sane)
    {
      hb_blob_make_immutable (blob);
      ਵਾਪਸ blob;
    }
    ਨਹੀਂਤਾਂ
    {
      hb_blob_destroy (blob);
      ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
    }
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
  hb_blob_t *reference_ਸਾਰਣੀ (ਅਚਲ hb_face_t *face, hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = Type::ਸਾਰਣੀ_ਟੈਗ)
  {
    ਜੇ (!ਅੰਕ_glyphs_ਸੈੱਟ)
      ਸੈੱਟ_ਅੰਕ_glyphs (hb_face_get_glyph_ਗਿਣਤੀ (face));
    ਵਾਪਸ sanitize_blob<Type> (hb_face_reference_ਸਾਰਣੀ (face, ਸਾਰਣੀ_ਟੈਗ));
  }

  ਅਚਲ ਅੱਖਰ *ਸ਼ੁਰੂ, *ਸਿਰਾ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ;
  ਬਦਲਣਯੋਗ ਪੂਰਨ max_ops, max_subtables;
  ਨਿਜੀ:
  ਪੂਰਨ recursion_ਡੂੰਘਾਈ;
  ਤਾਰਕਿਕ writable;
  hb_blob_t *blob;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs;
  ਤਾਰਕਿਕ  ਅੰਕ_glyphs_ਸੈੱਟ;
  ਜਨਤਕ:
  ਤਾਰਕਿਕ lazy_some_gpos;
};

ਬਣਤਰ hb_sanitize_with_object_t
{
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ਅਚਲ T& obj) : c (c)
  { c->ਸੈੱਟ_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  ਨਿਜੀ:
  hb_sanitize_context_t *c;
};


#ਜੇ_ਅੰਤ /* HB_SANITIZE_HH */
