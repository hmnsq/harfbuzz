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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-blob.hh"
#அந்தர்பாவ "hb-dispatch.hh"


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

கட்டமைப்பு hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, தருக்க, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (மாறிலி எழுத்து *start_ = nullptr, மாறிலி எழுத்து *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (பொய்),
	blob (nullptr),
	இலக்கம்_glyphs (65536),
	இலக்கம்_glyphs_set (பொய்),
	lazy_some_gpos (பொய்) {}

  மாறிலி எழுத்து *get_name () { திரும்பு "SANITIZE"; }
  template <typename T, typename F>
  தருக்க may_dispatch (மாறிலி T *obj HB_UNUSED, மாறிலி F *format)
  {
    திரும்பு format->sanitize (இது) &&
	   hb_barrier ();
  }
  நிலையான return_t default_return_மதிப்பு () { திரும்பு உண்மை; }
  நிலையான return_t no_dispatch_return_மதிப்பு () { திரும்பு பொய்; }
  தருக்க stop_sublookup_iteration (மாறிலி return_t r) மாறிலி { திரும்பு !r; }

  தருக்க visit_subtables (unsigned மொத்தம்)
  {
    max_subtables += மொத்தம்;
    திரும்பு max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  தனிப்பட்ட:
  template <typename T, typename ...Ts> auto
  _dispatch (மாறிலி T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (இது, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (மாறிலி T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (இது, std::forward<Ts> (ds)...) )
  பொது:
  template <typename T, typename ...Ts> auto
  dispatch (மாறிலி T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    எனில் (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    எனில் (blob)
      end_processing ();
  }

  வெறுமை init (hb_blob_t *b)
  {
    இது->blob = hb_blob_reference (b);
    இது->writable = பொய்;
  }

  வெறுமை set_இலக்கம்_glyphs (unsigned முழு இலக்கம்_glyphs_2)
  {
    இலக்கம்_glyphs = இலக்கம்_glyphs_2;
    இலக்கம்_glyphs_set = உண்மை;
  }
  unsigned முழு get_இலக்கம்_glyphs () { திரும்பு இலக்கம்_glyphs; }

  வெறுமை set_max_ops (முழு max_ops_) { max_ops = max_ops_; }

  template <typename T>
  வெறுமை set_object (மாறிலி T *obj)
  {
    reset_object ();

    எனில் (!obj) திரும்பு;

    மாறிலி எழுத்து *obj_start = (மாறிலி எழுத்து *) obj;
    எனில் (unlikely (obj_start < இது->start || இது->end <= obj_start))
    {
      இது->start = இது->end = nullptr;
      இது->length = 0;
    }
    இல்லையெனில்
    {
      இது->start = obj_start;
      இது->end   = obj_start + hb_min (size_t (இது->end - obj_start), obj->get_size ());
      இது->length = இது->end - இது->start;
    }
  }

  வெறுமை reset_object ()
  {
    எனில் (இது->blob)
    {
      இது->start = இது->blob->data;
      இது->end = இது->start + இது->blob->length;
    }
    இது->length = இது->end - இது->start;
    ப்ரதிஜ்ஞா (இது->start <= இது->end); /* Must not overflow. */
  }

  வெறுமை start_processing (மாறிலி எழுத்து *start_ = nullptr, மாறிலி எழுத்து *end_ = nullptr)
  {
    எனில் (start_)
    {
      இது->start = start_;
      இது->end = end_;
    }
    reset_object ();
    unsigned m;
    எனில் (unlikely (hb_unsigned_mul_overflows (இது->end - இது->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      இது->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    இல்லையெனில்
      இது->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    இது->debug_depth = 0;
    இது->recursion_depth = 0;

    DEBUG_செய்தி_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     இது->start, இது->end,
		     (unsigned long) (இது->end - இது->start));
  }

  வெறுமை end_processing ()
  {
    DEBUG_செய்தி_LEVEL (SANITIZE, இது->start, 0, -1,
		     "end [%p..%p]",
		     இது->start, இது->end);

    hb_blob_destroy (இது->blob);
    இது->blob = nullptr;
    இது->start = இது->end = nullptr;
    இது->length = 0;
  }

  தருக்க check_ops(unsigned மொத்தம்)
  {
    /* Avoid underflow */
    எனில் (unlikely (இது->max_ops < 0 || மொத்தம் >= (unsigned) இது->max_ops))
    {
      இது->max_ops = -1;
      திரும்பு பொய்;
    }
    இது->max_ops -= (முழு) மொத்தம்;
    திரும்பு உண்மை;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  தருக்க check_range (மாறிலி வெறுமை *base,
		    unsigned முழு len) மாறிலி
  {
    மாறிலி எழுத்து *p = (மாறிலி எழுத்து *) base;
    தருக்க ok = (uintptr_t) (p - இது->start) <= இது->length &&
	      (unsigned முழு) (இது->end - p) >= len &&
	      ((இது->max_ops -= len) > 0);

    DEBUG_செய்தி_LEVEL (SANITIZE, p, இது->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     இது->start, இது->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    திரும்பு likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  தருக்க check_range_fast (மாறிலி வெறுமை *base,
			 unsigned முழு len) மாறிலி
  {
    மாறிலி எழுத்து *p = (மாறிலி எழுத்து *) base;
    தருக்க ok = ((uintptr_t) (p - இது->start) <= இது->length &&
	       (unsigned முழு) (இது->end - p) >= len);

    DEBUG_செய்தி_LEVEL (SANITIZE, p, இது->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     இது->start, இது->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    திரும்பு likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  தருக்க check_point (மாறிலி வெறுமை *base) மாறிலி
  {
    மாறிலி எழுத்து *p = (மாறிலி எழுத்து *) base;
    தருக்க ok = (uintptr_t) (p - இது->start) <= இது->length;

    DEBUG_செய்தி_LEVEL (SANITIZE, p, இது->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     இது->start, இது->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    திரும்பு likely (ok);
  }

  template <typename T>
  தருக்க check_range (மாறிலி T *base,
		    unsigned முழு a,
		    unsigned முழு b) மாறிலி
  {
    unsigned m;
    திரும்பு !hb_unsigned_mul_overflows (a, b, &m) &&
	   இது->check_range (base, m);
  }

  template <typename T>
  தருக்க check_range (மாறிலி T *base,
		    unsigned முழு a,
		    unsigned முழு b,
		    unsigned முழு c) மாறிலி
  {
    unsigned m;
    திரும்பு !hb_unsigned_mul_overflows (a, b, &m) &&
	   இது->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  தருக்க check_array_sized (மாறிலி T *base, unsigned முழு len, unsigned len_size) மாறிலி
  {
    எனில் (len_size >= 4)
    {
      எனில் (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	திரும்பு பொய்;
    }
    இல்லையெனில்
      len = len * hb_static_size (T);
    திரும்பு இது->check_range (base, len);
  }

  template <typename T>
  தருக்க check_array (மாறிலி T *base, unsigned முழு len) மாறிலி
  {
    திரும்பு இது->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  தருக்க check_array (மாறிலி T *base,
		    unsigned முழு a,
		    unsigned முழு b) மாறிலி
  {
    திரும்பு இது->check_range (base, hb_static_size (T), a, b);
  }

  தருக்க check_start_recursion (முழு max_depth)
  {
    எனில் (unlikely (recursion_depth >= max_depth)) திரும்பு பொய்;
    திரும்பு ++recursion_depth;
  }

  தருக்க end_recursion (தருக்க result)
  {
    recursion_depth--;
    திரும்பு result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  தருக்க check_struct (மாறிலி Type *obj) மாறிலி
  {
    எனில் (அளவு (uintptr_t) == அளவு (uint32_t))
      திரும்பு likely (இது->check_range_fast (obj, obj->min_size));
    இல்லையெனில்
      திரும்பு likely (இது->check_point ((மாறிலி எழுத்து *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    தருக்க sane;

    init (blob);

    DEBUG_செய்தி_FUNC (SANITIZE, start, "start");

    start_processing ();

    எனில் (unlikely (!start))
    {
      end_processing ();
      திரும்பு blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<எழுத்து *> (start));

    sane = t->sanitize (இது);

    end_processing ();

    DEBUG_செய்தி_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    எனில் (sane)
    {
      hb_blob_make_immutable (blob);
      திரும்பு blob;
    }
    இல்லையெனில்
    {
      hb_blob_destroy (blob);
      திரும்பு hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (மாறிலி hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    எனில் (!இலக்கம்_glyphs_set)
      set_இலக்கம்_glyphs (hb_face_get_glyph_மொத்தம் (face));
    திரும்பு sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  மாறிலி எழுத்து *start, *end;
  unsigned length;
  mutable முழு max_ops, max_subtables;
  தனிப்பட்ட:
  முழு recursion_depth;
  தருக்க writable;
  hb_blob_t *blob;
  unsigned முழு இலக்கம்_glyphs;
  தருக்க  இலக்கம்_glyphs_set;
  பொது:
  தருக்க lazy_some_gpos;
};

கட்டமைப்பு hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, மாறிலி T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  தனிப்பட்ட:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
