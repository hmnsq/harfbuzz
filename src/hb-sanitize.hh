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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-blob.hh"
#അന്തര്ഭാവ "hb-dispatch.hh"


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

ഘടന hb_sanitize_context_t :
       hb_dispatch_context_t<hb_sanitize_context_t, തർക്കം, HB_DEBUG_SANITIZE>
{
  hb_sanitize_context_t (സ്ഥിരം അക്ഷരം *start_ = nullptr, സ്ഥിരം അക്ഷരം *end_ = nullptr) :
	start (start_), end (end_),
	length (0),
	max_ops (0), max_subtables (0),
        recursion_depth (0),
	writable (അസത്യം),
	blob (nullptr),
	അക്കം_glyphs (65536),
	അക്കം_glyphs_set (അസത്യം),
	lazy_some_gpos (അസത്യം) {}

  സ്ഥിരം അക്ഷരം *get_name () { തിരികെ "SANITIZE"; }
  template <typename T, typename F>
  തർക്കം may_dispatch (സ്ഥിരം T *obj HB_UNUSED, സ്ഥിരം F *format)
  {
    തിരികെ format->sanitize (ഇത്) &&
	   hb_barrier ();
  }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ സത്യം; }
  സ്ഥിതം return_t no_dispatch_return_മൂല്യം () { തിരികെ അസത്യം; }
  തർക്കം stop_sublookup_iteration (സ്ഥിരം return_t r) സ്ഥിരം { തിരികെ !r; }

  തർക്കം visit_subtables (unsigned എണ്ണം)
  {
    max_subtables += എണ്ണം;
    തിരികെ max_subtables < HB_SANITIZE_MAX_SUBTABLES;
  }

  സ്വകാര്യ:
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.sanitize (ഇത്, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (ഇത്, std::forward<Ts> (ds)...) )
  പൊതു:
  template <typename T, typename ...Ts> auto
  dispatch (സ്ഥിരം T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_sanitize_context_t (hb_blob_t *b) : hb_sanitize_context_t ()
  {
    init (b);

    എങ്കിൽ (blob)
      start_processing ();
  }

  ~hb_sanitize_context_t ()
  {
    എങ്കിൽ (blob)
      end_processing ();
  }

  ശൂന്യം init (hb_blob_t *b)
  {
    ഇത്->blob = hb_blob_reference (b);
    ഇത്->writable = അസത്യം;
  }

  ശൂന്യം set_അക്കം_glyphs (unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs_2)
  {
    അക്കം_glyphs = അക്കം_glyphs_2;
    അക്കം_glyphs_set = സത്യം;
  }
  unsigned പൂർണ്ണസംഖ്യ get_അക്കം_glyphs () { തിരികെ അക്കം_glyphs; }

  ശൂന്യം set_max_ops (പൂർണ്ണസംഖ്യ max_ops_) { max_ops = max_ops_; }

  template <typename T>
  ശൂന്യം set_object (സ്ഥിരം T *obj)
  {
    reset_object ();

    എങ്കിൽ (!obj) തിരികെ;

    സ്ഥിരം അക്ഷരം *obj_start = (സ്ഥിരം അക്ഷരം *) obj;
    എങ്കിൽ (unlikely (obj_start < ഇത്->start || ഇത്->end <= obj_start))
    {
      ഇത്->start = ഇത്->end = nullptr;
      ഇത്->length = 0;
    }
    അല്ലെങ്കിൽ
    {
      ഇത്->start = obj_start;
      ഇത്->end   = obj_start + hb_min (size_t (ഇത്->end - obj_start), obj->get_size ());
      ഇത്->length = ഇത്->end - ഇത്->start;
    }
  }

  ശൂന്യം reset_object ()
  {
    എങ്കിൽ (ഇത്->blob)
    {
      ഇത്->start = ഇത്->blob->data;
      ഇത്->end = ഇത്->start + ഇത്->blob->length;
    }
    ഇത്->length = ഇത്->end - ഇത്->start;
    പ്രതിജ്ഞാ (ഇത്->start <= ഇത്->end); /* Must not overflow. */
  }

  ശൂന്യം start_processing (സ്ഥിരം അക്ഷരം *start_ = nullptr, സ്ഥിരം അക്ഷരം *end_ = nullptr)
  {
    എങ്കിൽ (start_)
    {
      ഇത്->start = start_;
      ഇത്->end = end_;
    }
    reset_object ();
    unsigned m;
    എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (ഇത്->end - ഇത്->start, HB_SANITIZE_MAX_OPS_FACTOR, &m)))
      ഇത്->max_ops = HB_SANITIZE_MAX_OPS_MAX;
    അല്ലെങ്കിൽ
      ഇത്->max_ops = hb_clamp (m,
				(unsigned) HB_SANITIZE_MAX_OPS_MIN,
				(unsigned) HB_SANITIZE_MAX_OPS_MAX);
    ഇത്->debug_depth = 0;
    ഇത്->recursion_depth = 0;

    DEBUG_സന്ദേശം_LEVEL (SANITIZE, start, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     ഇത്->start, ഇത്->end,
		     (unsigned long) (ഇത്->end - ഇത്->start));
  }

  ശൂന്യം end_processing ()
  {
    DEBUG_സന്ദേശം_LEVEL (SANITIZE, ഇത്->start, 0, -1,
		     "end [%p..%p]",
		     ഇത്->start, ഇത്->end);

    hb_blob_destroy (ഇത്->blob);
    ഇത്->blob = nullptr;
    ഇത്->start = ഇത്->end = nullptr;
    ഇത്->length = 0;
  }

  തർക്കം check_ops(unsigned എണ്ണം)
  {
    /* Avoid underflow */
    എങ്കിൽ (unlikely (ഇത്->max_ops < 0 || എണ്ണം >= (unsigned) ഇത്->max_ops))
    {
      ഇത്->max_ops = -1;
      തിരികെ അസത്യം;
    }
    ഇത്->max_ops -= (പൂർണ്ണസംഖ്യ) എണ്ണം;
    തിരികെ സത്യം;
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  തർക്കം check_range (സ്ഥിരം ശൂന്യം *base,
		    unsigned പൂർണ്ണസംഖ്യ len) സ്ഥിരം
  {
    സ്ഥിരം അക്ഷരം *p = (സ്ഥിരം അക്ഷരം *) base;
    തർക്കം ok = (uintptr_t) (p - ഇത്->start) <= ഇത്->length &&
	      (unsigned പൂർണ്ണസംഖ്യ) (ഇത്->end - p) >= len &&
	      ((ഇത്->max_ops -= len) > 0);

    DEBUG_സന്ദേശം_LEVEL (SANITIZE, p, ഇത്->debug_depth+1, 0,
		     "check_range [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ഇത്->start, ഇത്->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    തിരികെ likely (ok);
  }
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  തർക്കം check_range_fast (സ്ഥിരം ശൂന്യം *base,
			 unsigned പൂർണ്ണസംഖ്യ len) സ്ഥിരം
  {
    സ്ഥിരം അക്ഷരം *p = (സ്ഥിരം അക്ഷരം *) base;
    തർക്കം ok = ((uintptr_t) (p - ഇത്->start) <= ഇത്->length &&
	       (unsigned പൂർണ്ണസംഖ്യ) (ഇത്->end - p) >= len);

    DEBUG_സന്ദേശം_LEVEL (SANITIZE, p, ഇത്->debug_depth+1, 0,
		     "check_range_fast [%p..%p]"
		     " (%u bytes) in [%p..%p] -> %s",
		     p, p + len, len,
		     ഇത്->start, ഇത്->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    തിരികെ likely (ok);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  തർക്കം check_point (സ്ഥിരം ശൂന്യം *base) സ്ഥിരം
  {
    സ്ഥിരം അക്ഷരം *p = (സ്ഥിരം അക്ഷരം *) base;
    തർക്കം ok = (uintptr_t) (p - ഇത്->start) <= ഇത്->length;

    DEBUG_സന്ദേശം_LEVEL (SANITIZE, p, ഇത്->debug_depth+1, 0,
		     "check_point [%p]"
		     " in [%p..%p] -> %s",
		     p,
		     ഇത്->start, ഇത്->end,
		     ok ? "OK" : "OUT-OF-RANGE");

    തിരികെ likely (ok);
  }

  template <typename T>
  തർക്കം check_range (സ്ഥിരം T *base,
		    unsigned പൂർണ്ണസംഖ്യ a,
		    unsigned പൂർണ്ണസംഖ്യ b) സ്ഥിരം
  {
    unsigned m;
    തിരികെ !hb_unsigned_mul_overflows (a, b, &m) &&
	   ഇത്->check_range (base, m);
  }

  template <typename T>
  തർക്കം check_range (സ്ഥിരം T *base,
		    unsigned പൂർണ്ണസംഖ്യ a,
		    unsigned പൂർണ്ണസംഖ്യ b,
		    unsigned പൂർണ്ണസംഖ്യ c) സ്ഥിരം
  {
    unsigned m;
    തിരികെ !hb_unsigned_mul_overflows (a, b, &m) &&
	   ഇത്->check_range (base, m, c);
  }

  template <typename T>
  HB_ALWAYS_INLINE
  തർക്കം check_array_sized (സ്ഥിരം T *base, unsigned പൂർണ്ണസംഖ്യ len, unsigned len_size) സ്ഥിരം
  {
    എങ്കിൽ (len_size >= 4)
    {
      എങ്കിൽ (unlikely (hb_unsigned_mul_overflows (len, hb_static_size (T), &len)))
	തിരികെ അസത്യം;
    }
    അല്ലെങ്കിൽ
      len = len * hb_static_size (T);
    തിരികെ ഇത്->check_range (base, len);
  }

  template <typename T>
  തർക്കം check_array (സ്ഥിരം T *base, unsigned പൂർണ്ണസംഖ്യ len) സ്ഥിരം
  {
    തിരികെ ഇത്->check_range (base, len, hb_static_size (T));
  }

  template <typename T>
  തർക്കം check_array (സ്ഥിരം T *base,
		    unsigned പൂർണ്ണസംഖ്യ a,
		    unsigned പൂർണ്ണസംഖ്യ b) സ്ഥിരം
  {
    തിരികെ ഇത്->check_range (base, hb_static_size (T), a, b);
  }

  തർക്കം check_start_recursion (പൂർണ്ണസംഖ്യ max_depth)
  {
    എങ്കിൽ (unlikely (recursion_depth >= max_depth)) തിരികെ അസത്യം;
    തിരികെ ++recursion_depth;
  }

  തർക്കം end_recursion (തർക്കം result)
  {
    recursion_depth--;
    തിരികെ result;
  }

  template <typename Type>
#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  തർക്കം check_struct (സ്ഥിരം Type *obj) സ്ഥിരം
  {
    എങ്കിൽ (വലുപ്പം (uintptr_t) == വലുപ്പം (uint32_t))
      തിരികെ likely (ഇത്->check_range_fast (obj, obj->min_size));
    അല്ലെങ്കിൽ
      തിരികെ likely (ഇത്->check_point ((സ്ഥിരം അക്ഷരം *) obj + obj->min_size));
  }

  template <typename Type>
  hb_blob_t *sanitize_blob (hb_blob_t *blob)
  {
    തർക്കം sane;

    init (blob);

    DEBUG_സന്ദേശം_FUNC (SANITIZE, start, "start");

    start_processing ();

    എങ്കിൽ (unlikely (!start))
    {
      end_processing ();
      തിരികെ blob;
    }

    Type *t = reinterpret_cast<Type *> (const_cast<അക്ഷരം *> (start));

    sane = t->sanitize (ഇത്);

    end_processing ();

    DEBUG_സന്ദേശം_FUNC (SANITIZE, start, sane ? "PASSED" : "FAILED");
    എങ്കിൽ (sane)
    {
      hb_blob_make_immutable (blob);
      തിരികെ blob;
    }
    അല്ലെങ്കിൽ
    {
      hb_blob_destroy (blob);
      തിരികെ hb_blob_get_empty ();
    }
  }

  template <typename Type>
  hb_blob_t *reference_table (സ്ഥിരം hb_face_t *face, hb_tag_t tableTag = Type::tableTag)
  {
    എങ്കിൽ (!അക്കം_glyphs_set)
      set_അക്കം_glyphs (hb_face_get_glyph_എണ്ണം (face));
    തിരികെ sanitize_blob<Type> (hb_face_reference_table (face, tableTag));
  }

  സ്ഥിരം അക്ഷരം *start, *end;
  unsigned length;
  mutable പൂർണ്ണസംഖ്യ max_ops, max_subtables;
  സ്വകാര്യ:
  പൂർണ്ണസംഖ്യ recursion_depth;
  തർക്കം writable;
  hb_blob_t *blob;
  unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs;
  തർക്കം  അക്കം_glyphs_set;
  പൊതു:
  തർക്കം lazy_some_gpos;
};

ഘടന hb_sanitize_with_object_t
{
  template <typename T>
  hb_sanitize_with_object_t (hb_sanitize_context_t *c, സ്ഥിരം T& obj) : c (c)
  { c->set_object (obj); }
  ~hb_sanitize_with_object_t ()
  { c->reset_object (); }

  സ്വകാര്യ:
  hb_sanitize_context_t *c;
};


#endif /* HB_SANITIZE_HH */
