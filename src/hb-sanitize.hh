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

#ifndef HB_SANITIZE_HH
#define HB_SANITIZE_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-blob.hh"
#ಅನ್ತರ್ಭಾವ "hb-dispatch.hh"


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
#ifndef HB_SANITIZE_MAX_EDITS
#define HB_SANITIZE_MAX_EDITS 32
#endif
#ifndef HB_SANITIZE_MAX_OPS_FACTOR
#define HB_SANITIZE_MAX_OPS_FACTOR 64
#endif
#ifndef HB_SANITIZE_MAX_OPS_MIN
#define HB_SANITIZE_MAX_OPS_MIN 16384
#endif
#ifndef HB_SANITIZE_MAX_OPS_MAX
#define HB_SANITIZE_MAX_OPS_MAX 0x3FFFFFFF
#endif
#ifndef HB_SANITIZE_MAX_SUBTABLES
#define HB_SANITIZE_MAX_SUBTABLES 0x4000
#endif

ಸಂರಚನಾ hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, ತಾರ್ಕಿಕ, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (ಧ್ರುವ ವರ್ಣ *start_ = nullptr, ಧ್ರುವ ವರ್ಣ *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (ಅಸತ್ಯ),
	blob (nullptr),
	ಅಂಕೆ_glyphs (65536),
	ಅಂಕೆ_glyphs_set (ಅಸತ್ಯ),
	lazy_some_gpos (ಅಸತ್ಯ) {}

  ಧ್ರುವ ವರ್ಣ *get_name () { ಪ್ರತ್ಯಾಗಮ "SANITIZE"; }
  template <typename T, typename F>
  ತಾರ್ಕಿಕ may_dispatch (ಧ್ರುವ T *obj HB_UNUSED, ಧ್ರುವ F *format)
  {
    ಪ್ರತ್ಯಾಗಮ format->sanitize (ಏತತ್) &&
	   hb_barrier ();
  }
  ಸ್ಥಿರ return_t default_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
  ಸ್ಥಿರ return_t no_dispatch_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  ತಾರ್ಕಿಕ stop_sublookup_iteration (ಧ್ರುವ return_t r) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !r; }

  ತಾರ್ಕಿಕ visit_subtables (unsigned ಎಣಿಕೆ)
  {
    max_subtables += ಎಣಿಕೆ;
    ಪ್ರತ್ಯಾಗಮ max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  ಗುಪ್ತ:
  template <typename T, typename ...Ts> auto
  _dispatch (ಧ್ರುವ T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (ಏತತ್, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (ಧ್ರುವ T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (ಏತತ್, std::forward<Ts> (ds)...) )
  ಸಾರ್ವಜನಿಕ:
  template <typename T, typename ...Ts> auto
  dispatch (ಧ್ರುವ T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    ಯದಿ (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    ಯದಿ (blob)
      end_processing ();
  }

  ಶೂನ್ಯ init (hb_blob_t *b)
  {
    ಏತತ್->blob = hb_blob_reference (b);
    ಏತತ್->writable = ಅಸತ್ಯ;
  }

  ಶೂನ್ಯ set_ಅಂಕೆ_glyphs (unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs_2)
  {
    ಅಂಕೆ_glyphs = ಅಂಕೆ_glyphs_2;
    ಅಂಕೆ_glyphs_set = ಸತ್ಯ;
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_ಅಂಕೆ_glyphs () { ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_glyphs; }

  ಶೂನ್ಯ set_max_ops (ಪೂರ್ಣಾಙ್ಕ max_ops_) { max_ops = max_ops_; }

  template <typename T>
  ಶೂನ್ಯ set_object (ಧ್ರುವ T *obj)
  {
    reset_object ();

    ಯದಿ (!obj) ಪ್ರತ್ಯಾಗಮ;

    ಧ್ರುವ ವರ್ಣ *obj_start = (ಧ್ರುವ ವರ್ಣ *) obj;
    ಯದಿ (unlikely (obj_start < ಏತತ್->start || ಏತತ್->end <= obj_start))
    {
      ಏತತ್->start = ಏತತ್->end = nullptr;
      ಏತತ್->length = 0;
    }
    ಅನ್ಯಥಾ
    {
      ಏತತ್->start = obj_start;
      ಏತತ್->end   = obj_start + hb_min (size_t (ಏತತ್->end - obj_start), obj->get_size ());
      ಏತತ್->length = ಏತತ್->end - ಏತತ್->start;
    }
  }

  ಶೂನ್ಯ reset_object ()
  {
    ಯದಿ (ಏತತ್->blob)
    {
      ಏತತ್->start = ಏತತ್->blob->data;
      ಏತತ್->end = ಏತತ್->start + ಏತತ್->blob->length;
    }
    ಏತತ್->length = ಏತತ್->end - ಏತತ್->start;
    ಪ್ರತಿಜ್ಞಾ (ಏತತ್->start <= ಏತತ್->end); /* Must not overflow. */
  }

  ಶೂನ್ಯ start_processing (ಧ್ರುವ ವರ್ಣ *start_ = nullptr, ಧ್ರುವ ವರ್ಣ *end_ = nullptr)
  {
    ಯದಿ (start_)
    {
      ಏತತ್->start = start_;
      ಏತತ್->end = end_;
    }
    reset_object ();
    unsigned m;
    ಯದಿ (unlikely (hb_unsigned_mul_overflows (ಏತತ್->end - ಏತತ್->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      ಏತತ್->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    ಅನ್ಯಥಾ
      ಏತತ್->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    ಏತತ್->debug_depth = 0;
    ಏತತ್->recursion_depth = 0;

    DEBUG_ಸಂದೇಶ_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ಏತತ್->start, ಏತತ್->end,
		     (unsigned long) (ಏತತ್->end - ಏತತ್->start));
  }

  ಶೂನ್ಯ end_processing ()
  {
    DEBUG_ಸಂದೇಶ_LEVEL (SANITIZE, ಏತತ್->start, 0, -1,
		     "end [%p..%p]",
		     ಏತತ್->start, ಏತತ್->end);

    hb_blob_destroy (ಏತತ್->blob);
    ಏತತ್->blob = nullptr;
    ಏತತ್->start = ಏತತ್->end = nullptr;
    ಏತತ್->length = 0;
  }

  ತಾರ್ಕಿಕ check_ops(unsigned ಎಣಿಕೆ)
  {
    /* Avoid underflow */
    ಯದಿ (unlikely (ಏತತ್->max_ops < 0 || ಎಣಿಕೆ >= (unsigned) ಏತತ್->max_ops))
    {
      ಏತತ್->max_ops = -1;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಏತತ್->max_ops -= (ಪೂರ್ಣಾಙ್ಕ) ಎಣಿಕೆ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ತಾರ್ಕಿಕ check_range (ಧ್ರುವ ಶೂನ್ಯ *base,
		    unsigned ಪೂರ್ಣಾಙ್ಕ len) ಧ್ರುವ
  {
    ಧ್ರುವ ವರ್ಣ *p = (ಧ್ರುವ ವರ್ಣ *) base;
    ತಾರ್ಕಿಕ ok = (uintptr_t) (p - ಏತತ್->start) <= ಏತತ್->length &&
	      (unsigned ಪೂರ್ಣಾಙ್ಕ) (ಏತತ್->end - p) >= len &&
	      ((ಏತತ್->max_ops -= len) > 0);

    DEBUG_ಸಂದೇಶ_LEVEL (SANITIZE, p, ಏತತ್->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ಏತತ್->start, ಏತತ್->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ಪ್ರತ್ಯಾಗಮ likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ತಾರ್ಕಿಕ check_range_fast (ಧ್ರುವ ಶೂನ್ಯ *base,
			 unsigned ಪೂರ್ಣಾಙ್ಕ len) ಧ್ರುವ
  {
    ಧ್ರುವ ವರ್ಣ *p = (ಧ್ರುವ ವರ್ಣ *) base;
    ತಾರ್ಕಿಕ ok = ((uintptr_t) (p - ಏತತ್->start) <= ಏತತ್->length &&
	       (unsigned ಪೂರ್ಣಾಙ್ಕ) (ಏತತ್->end - p) >= len);

    DEBUG_ಸಂದೇಶ_LEVEL (SANITIZE, p, ಏತತ್->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ಏತತ್->start, ಏತತ್->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ಪ್ರತ್ಯಾಗಮ likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ತಾರ್ಕಿಕ check_point (ಧ್ರುವ ಶೂನ್ಯ *base) ಧ್ರುವ
  {
    ಧ್ರುವ ವರ್ಣ *p = (ಧ್ರುವ ವರ್ಣ *) base;
    ತಾರ್ಕಿಕ ok = (uintptr_t) (p - ಏತತ್->start) <= ಏತತ್->length;

    DEBUG_ಸಂದೇಶ_LEVEL (SANITIZE, p, ಏತತ್->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     ಏತತ್->start, ಏತತ್->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    ಪ್ರತ್ಯಾಗಮ likely (ok);
  }

  template <typename T>
  ತಾರ್ಕಿಕ check_range (ಧ್ರುವ T *base,
		    unsigned ಪೂರ್ಣಾಙ್ಕ a,
		    unsigned ಪೂರ್ಣಾಙ್ಕ b) ಧ್ರುವ
  {
    unsigned m;
    ಪ್ರತ್ಯಾಗಮ !hb_unsigned_mul_overflows (a, b, &m) &&
	   ಏತತ್->check_range (base, m);
  }

  template <typename T>
  ತಾರ್ಕಿಕ check_range (ಧ್ರುವ T *base,
		    unsigned ಪೂರ್ಣಾಙ್ಕ a,
		    unsigned ಪೂರ್ಣಾಙ್ಕ b,
		    unsigned ಪೂರ್ಣಾಙ್ಕ c) ಧ್ರುವ
  {
    unsigned m;
    ಪ್ರತ್ಯಾಗಮ !hb_unsigned_mul_overflows (a, b, &m) &&
	   ಏತತ್->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  ತಾರ್ಕಿಕ check_array_sized (ಧ್ರುವ T *base, unsigned ಪೂರ್ಣಾಙ್ಕ len, unsigned len_size) ಧ್ರುವ
  {
    ಯದಿ (len_size >= 4)
    {
      ಯದಿ (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಅನ್ಯಥಾ
      len = len * hb_static_size (T);
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->check_range (base, len);
  }

  template <typename T>
  ತಾರ್ಕಿಕ check_array (ಧ್ರುವ T *base, unsigned ಪೂರ್ಣಾಙ್ಕ len) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  ತಾರ್ಕಿಕ check_array (ಧ್ರುವ T *base,
		    unsigned ಪೂರ್ಣಾಙ್ಕ a,
		    unsigned ಪೂರ್ಣಾಙ್ಕ b) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ ಏತತ್->check_range (base, hb_static_size (T), a, b);
  }

  ತಾರ್ಕಿಕ check_start_recursion (ಪೂರ್ಣಾಙ್ಕ max_depth)
  {
    ಯದಿ (unlikely (recursion_depth >= max_depth)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ++recursion_depth;
  }

  ತಾರ್ಕಿಕ end_recursion (ತಾರ್ಕಿಕ result)
  {
    recursion_depth--;
    ಪ್ರತ್ಯಾಗಮ result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ತಾರ್ಕಿಕ check_struct (ಧ್ರುವ Type *obj) ಧ್ರುವ
  {
    ಯದಿ (ಪರಿಮಾಣ (uintptr_t) == ಪರಿಮಾಣ (uint32_t))
      ಪ್ರತ್ಯಾಗಮ likely (ಏತತ್->check_range_fast (obj, obj->min_size));
    ಅನ್ಯಥಾ
      ಪ್ರತ್ಯಾಗಮ likely (ಏತತ್->check_point ((ಧ್ರುವ ವರ್ಣ *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    ತಾರ್ಕಿಕ sane;

    init (blob);

    DEBUG_ಸಂದೇಶ_FUNC (SANITIZE, start, "start");

    start_processing ();

    ಯದಿ (unlikely (!start))
    {
      end_processing ();
      ಪ್ರತ್ಯಾಗಮ blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<ವರ್ಣ *> (start));

    sane = t->sanitize (ಏತತ್);

    end_processing ();

    DEBUG_ಸಂದೇಶ_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    ಯದಿ (sane)
    {
      hb_blob_make_immutable (blob);
      ಪ್ರತ್ಯಾಗಮ blob;
    }
    ಅನ್ಯಥಾ
    {
      hb_blob_destroy (blob);
      ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (ಧ್ರುವ hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    ಯದಿ (!ಅಂಕೆ_glyphs_set)
      set_ಅಂಕೆ_glyphs (hb_face_get_glyph_ಎಣಿಕೆ (face));
    ಪ್ರತ್ಯಾಗಮ sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  ಧ್ರುವ ವರ್ಣ *start, *end;
  unsigned length;
  mutable ಪೂರ್ಣಾಙ್ಕ max_ops, max_subtables;
  ಗುಪ್ತ:
  ಪೂರ್ಣಾಙ್ಕ recursion_depth;
  ತಾರ್ಕಿಕ writable;
  hb_blob_t *blob;
  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs;
  ತಾರ್ಕಿಕ  ಅಂಕೆ_glyphs_set;
  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ lazy_some_gpos;
};

ಸಂರಚನಾ hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, ಧ್ರುವ T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  ಗುಪ್ತ:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
