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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-blob.hh"
#અન્તર્ભાવ "hb-dispatch.hh"


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

સંરચના hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, તાર્કિક, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (અચલ અક્ષર *start_ = nullptr, અચલ અક્ષર *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (ખોટું),
	blob (nullptr),
	અંક_glyphs (65536),
	અંક_glyphs_set (ખોટું),
	lazy_some_gpos (ખોટું) {}

  અચલ અક્ષર *get_name () { પાછા "SANITIZE"; }
  template <typename T, typename F>
  તાર્કિક may_dispatch (અચલ T *obj HB_UNUSED, અચલ F *format)
  {
    પાછા format->sanitize (આ) &&
	   hb_barrier ();
  }
  સ્થિર return_t default_return_કિંમત () { પાછા સાચું; }
  સ્થિર return_t no_dispatch_return_કિંમત () { પાછા ખોટું; }
  તાર્કિક stop_sublookup_iteration (અચલ return_t r) અચલ { પાછા !r; }

  તાર્કિક visit_subtables (unsigned ગણતરી)
  {
    max_subtables += ગણતરી;
    પાછા max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  ખાનગી:
  template <typename T, typename ...Ts> auto
  _dispatch (અચલ T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (આ, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (અચલ T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (આ, std::forward<Ts> (ds)...) )
  જાહેર:
  template <typename T, typename ...Ts> auto
  dispatch (અચલ T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    જો (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    જો (blob)
      end_processing ();
  }

  શૂન્ય init (hb_blob_t *b)
  {
    આ->blob = hb_blob_reference (b);
    આ->writable = ખોટું;
  }

  શૂન્ય set_અંક_glyphs (unsigned પૂર્ણાંક અંક_glyphs_2)
  {
    અંક_glyphs = અંક_glyphs_2;
    અંક_glyphs_set = સાચું;
  }
  unsigned પૂર્ણાંક get_અંક_glyphs () { પાછા અંક_glyphs; }

  શૂન્ય set_max_ops (પૂર્ણાંક max_ops_) { max_ops = max_ops_; }

  template <typename T>
  શૂન્ય set_object (અચલ T *obj)
  {
    reset_object ();

    જો (!obj) પાછા;

    અચલ અક્ષર *obj_start = (અચલ અક્ષર *) obj;
    જો (unlikely (obj_start < આ->start || આ->end <= obj_start))
    {
      આ->start = આ->end = nullptr;
      આ->length = 0;
    }
    નહીંતર
    {
      આ->start = obj_start;
      આ->end   = obj_start + hb_min (size_t (આ->end - obj_start), obj->get_size ());
      આ->length = આ->end - આ->start;
    }
  }

  શૂન્ય reset_object ()
  {
    જો (આ->blob)
    {
      આ->start = આ->blob->data;
      આ->end = આ->start + આ->blob->length;
    }
    આ->length = આ->end - આ->start;
    પ્રતિજ્ઞા (આ->start <= આ->end); /* Must not overflow. */
  }

  શૂન્ય start_processing (અચલ અક્ષર *start_ = nullptr, અચલ અક્ષર *end_ = nullptr)
  {
    જો (start_)
    {
      આ->start = start_;
      આ->end = end_;
    }
    reset_object ();
    unsigned m;
    જો (unlikely (hb_unsigned_mul_overflows (આ->end - આ->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      આ->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    નહીંતર
      આ->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    આ->debug_depth = 0;
    આ->recursion_depth = 0;

    DEBUG_સંદેશ_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     આ->start, આ->end,
		     (unsigned long) (આ->end - આ->start));
  }

  શૂન્ય end_processing ()
  {
    DEBUG_સંદેશ_LEVEL (SANITIZE, આ->start, 0, -1,
		     "end [%p..%p]",
		     આ->start, આ->end);

    hb_blob_destroy (આ->blob);
    આ->blob = nullptr;
    આ->start = આ->end = nullptr;
    આ->length = 0;
  }

  તાર્કિક check_ops(unsigned ગણતરી)
  {
    /* Avoid underflow */
    જો (unlikely (આ->max_ops < 0 || ગણતરી >= (unsigned) આ->max_ops))
    {
      આ->max_ops = -1;
      પાછા ખોટું;
    }
    આ->max_ops -= (પૂર્ણાંક) ગણતરી;
    પાછા સાચું;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  તાર્કિક check_range (અચલ શૂન્ય *base,
		    unsigned પૂર્ણાંક len) અચલ
  {
    અચલ અક્ષર *p = (અચલ અક્ષર *) base;
    તાર્કિક ok = (uintptr_t) (p - આ->start) <= આ->length &&
	      (unsigned પૂર્ણાંક) (આ->end - p) >= len &&
	      ((આ->max_ops -= len) > 0);

    DEBUG_સંદેશ_LEVEL (SANITIZE, p, આ->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     આ->start, આ->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    પાછા likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  તાર્કિક check_range_fast (અચલ શૂન્ય *base,
			 unsigned પૂર્ણાંક len) અચલ
  {
    અચલ અક્ષર *p = (અચલ અક્ષર *) base;
    તાર્કિક ok = ((uintptr_t) (p - આ->start) <= આ->length &&
	       (unsigned પૂર્ણાંક) (આ->end - p) >= len);

    DEBUG_સંદેશ_LEVEL (SANITIZE, p, આ->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     આ->start, આ->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    પાછા likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  તાર્કિક check_point (અચલ શૂન્ય *base) અચલ
  {
    અચલ અક્ષર *p = (અચલ અક્ષર *) base;
    તાર્કિક ok = (uintptr_t) (p - આ->start) <= આ->length;

    DEBUG_સંદેશ_LEVEL (SANITIZE, p, આ->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     આ->start, આ->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    પાછા likely (ok);
  }

  template <typename T>
  તાર્કિક check_range (અચલ T *base,
		    unsigned પૂર્ણાંક a,
		    unsigned પૂર્ણાંક b) અચલ
  {
    unsigned m;
    પાછા !hb_unsigned_mul_overflows (a, b, &m) &&
	   આ->check_range (base, m);
  }

  template <typename T>
  તાર્કિક check_range (અચલ T *base,
		    unsigned પૂર્ણાંક a,
		    unsigned પૂર્ણાંક b,
		    unsigned પૂર્ણાંક c) અચલ
  {
    unsigned m;
    પાછા !hb_unsigned_mul_overflows (a, b, &m) &&
	   આ->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  તાર્કિક check_array_sized (અચલ T *base, unsigned પૂર્ણાંક len, unsigned len_size) અચલ
  {
    જો (len_size >= 4)
    {
      જો (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	પાછા ખોટું;
    }
    નહીંતર
      len = len * hb_static_size (T);
    પાછા આ->check_range (base, len);
  }

  template <typename T>
  તાર્કિક check_array (અચલ T *base, unsigned પૂર્ણાંક len) અચલ
  {
    પાછા આ->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  તાર્કિક check_array (અચલ T *base,
		    unsigned પૂર્ણાંક a,
		    unsigned પૂર્ણાંક b) અચલ
  {
    પાછા આ->check_range (base, hb_static_size (T), a, b);
  }

  તાર્કિક check_start_recursion (પૂર્ણાંક max_depth)
  {
    જો (unlikely (recursion_depth >= max_depth)) પાછા ખોટું;
    પાછા ++recursion_depth;
  }

  તાર્કિક end_recursion (તાર્કિક result)
  {
    recursion_depth--;
    પાછા result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  તાર્કિક check_struct (અચલ Type *obj) અચલ
  {
    જો (કદ (uintptr_t) == કદ (uint32_t))
      પાછા likely (આ->check_range_fast (obj, obj->min_size));
    નહીંતર
      પાછા likely (આ->check_point ((અચલ અક્ષર *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    તાર્કિક sane;

    init (blob);

    DEBUG_સંદેશ_FUNC (SANITIZE, start, "start");

    start_processing ();

    જો (unlikely (!start))
    {
      end_processing ();
      પાછા blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<અક્ષર *> (start));

    sane = t->sanitize (આ);

    end_processing ();

    DEBUG_સંદેશ_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    જો (sane)
    {
      hb_blob_make_immutable (blob);
      પાછા blob;
    }
    નહીંતર
    {
      hb_blob_destroy (blob);
      પાછા hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (અચલ hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    જો (!અંક_glyphs_set)
      set_અંક_glyphs (hb_face_get_glyph_ગણતરી (face));
    પાછા sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  અચલ અક્ષર *start, *end;
  unsigned length;
  mutable પૂર્ણાંક max_ops, max_subtables;
  ખાનગી:
  પૂર્ણાંક recursion_depth;
  તાર્કિક writable;
  hb_blob_t *blob;
  unsigned પૂર્ણાંક અંક_glyphs;
  તાર્કિક  અંક_glyphs_set;
  જાહેર:
  તાર્કિક lazy_some_gpos;
};

સંરચના hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, અચલ T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  ખાનગી:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
