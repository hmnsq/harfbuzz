/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ARRAY_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ARRAY_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-algs.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-iter.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-null.hh"


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ hb_sorted_array_t;

ਗਿਣਤੀ hb_not_ਮਿਲਿਆ_t
{
  HB_NOT_ਮਿਲਿਆ_DONT_STORE,
  HB_NOT_ਮਿਲਿਆ_STORE,
  HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST,
};


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ realloc_move = ਸੱਚ;

  /*
   * Constructors.
   */
  hb_array_t () = ਮੂਲ;
  hb_array_t (ਅਚਲ hb_array_t&) = ਮੂਲ;
  ~hb_array_t () = ਮੂਲ;
  hb_array_t& ਸੰਚਾਲਕ= (ਅਚਲ hb_array_t&) = ਮੂਲ;
  hb_array_t& ਸੰਚਾਲਕ= (hb_array_t&&) = ਮੂਲ;

  constexpr hb_array_t (Type *array_, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ_2) : arrayZ (array_), ਲੰਮਾਈ (ਲੰਮਾਈ_2) {}
  ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ_2>
  constexpr hb_array_t (Type (&array_)[ਲੰਮਾਈ_2]) : hb_array_t (array_, ਲੰਮਾਈ_2) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (ਅਚਲ hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), ਲੰਮਾਈ (o.ਲੰਮਾਈ), backwards_ਲੰਮਾਈ (o.backwards_ਲੰਮਾਈ) {}
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_array_t<U> &o)
  { arrayZ = o.arrayZ; ਲੰਮਾਈ = o.ਲੰਮਾਈ; backwards_ਲੰਮਾਈ = o.backwards_ਲੰਮਾਈ; ਵਾਪਸ *ਇਹ; }

  /*
   * Iterator implementation.
   */
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Type& __item_t__;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator = ਸੱਚ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ has_fast_len = ਸੱਚ;
  Type& __item__ () ਅਚਲ
  {
    ਜੇ (unlikely (!ਲੰਮਾਈ)) ਵਾਪਸ CrapOrNull (Type);
    ਵਾਪਸ *arrayZ;
  }
  Type& __item_at__ (ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    ਜੇ (unlikely (i >= ਲੰਮਾਈ)) ਵਾਪਸ CrapOrNull (Type);
    ਵਾਪਸ arrayZ[i];
  }
  ਖਾਲੀ __next__ ()
  {
    ਜੇ (unlikely (!ਲੰਮਾਈ))
      ਵਾਪਸ;
    ਲੰਮਾਈ--;
    backwards_ਲੰਮਾਈ++;
    arrayZ++;
  }
  ਖਾਲੀ __forward__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n)
  {
    ਜੇ (unlikely (n > ਲੰਮਾਈ))
      n = ਲੰਮਾਈ;
    ਲੰਮਾਈ -= n;
    backwards_ਲੰਮਾਈ += n;
    arrayZ += n;
  }
  ਖਾਲੀ __prev__ ()
  {
    ਜੇ (unlikely (!backwards_ਲੰਮਾਈ))
      ਵਾਪਸ;
    ਲੰਮਾਈ++;
    backwards_ਲੰਮਾਈ--;
    arrayZ--;
  }
  ਖਾਲੀ __rewind__ (ਬਿਨਾਨਿਸ਼ਾਨੀ n)
  {
    ਜੇ (unlikely (n > backwards_ਲੰਮਾਈ))
      n = backwards_ਲੰਮਾਈ;
    ਲੰਮਾਈ += n;
    backwards_ਲੰਮਾਈ -= n;
    arrayZ -= n;
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ __len__ () ਅਚਲ { ਵਾਪਸ ਲੰਮਾਈ; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_array_t& o) ਅਚਲ
  { ਵਾਪਸ ਇਹ->arrayZ != o.arrayZ || ਇਹ->ਲੰਮਾਈ != o.ਲੰਮਾਈ; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ਅਚਲ { ਵਾਪਸ arrayZ; }
  Type *ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ arrayZ + ਲੰਮਾਈ; }


  /* Extra operators.
   */
  Type * ਸੰਚਾਲਕ & () ਅਚਲ { ਵਾਪਸ arrayZ; }
  ਸੰਚਾਲਕ hb_array_t<ਅਚਲ Type> () { ਵਾਪਸ hb_array_t<ਅਚਲ Type> (arrayZ, ਲੰਮਾਈ); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸੰਚਾਲਕ T * () ਅਚਲ { ਵਾਪਸ arrayZ; }

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_array_t &o) ਅਚਲ;

  uint32_t hash () ਅਚਲ
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    ਲਈ (ਸਵੈਚਾਲਿਤ &v : *ਇਹ)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    ਵਾਪਸ current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  ਪੂਰਨ cmp (ਅਚਲ hb_array_t &a) ਅਚਲ
  {
    ਜੇ (ਲੰਮਾਈ != a.ਲੰਮਾਈ)
      ਵਾਪਸ (ਪੂਰਨ) a.ਲੰਮਾਈ - (ਪੂਰਨ) ਲੰਮਾਈ;
    ਵਾਪਸ hb_memcmp (a.arrayZ, arrayZ, get_ਮਾਪ ());
  }
  HB_ਅੰਦਰੂਨੀ ਸਥਿਰ ਪੂਰਨ cmp (ਅਚਲ ਖਾਲੀ *pa, ਅਚਲ ਖਾਲੀ *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    ਵਾਪਸ b->cmp (*a);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  Type *lsearch (ਅਚਲ T &x, Type *not_ਮਿਲਿਆ = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਵਾਪਸ lfind (x, &i) ? &ਇਹ->arrayZ[i] : not_ਮਿਲਿਆ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਅਚਲ Type *lsearch (ਅਚਲ T &x, ਅਚਲ Type *not_ਮਿਲਿਆ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i;
    ਵਾਪਸ lfind (x, &i) ? &ਇਹ->arrayZ[i] : not_ਮਿਲਿਆ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ lfind (ਅਚਲ T &x, ਬਿਨਾਨਿਸ਼ਾਨੀ *pos = ਖਾਲੀ_ਸੂਚਕ,
	      hb_not_ਮਿਲਿਆ_t not_ਮਿਲਿਆ = HB_NOT_ਮਿਲਿਆ_DONT_STORE,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੰਜ਼ਿਲ_store = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਲੰਮਾਈ; ++i)
      ਜੇ (hb_equal (x, ਇਹ->arrayZ[i]))
      {
	ਜੇ (pos)
	  *pos = i;
	ਵਾਪਸ ਸੱਚ;
      }

    ਜੇ (pos)
    {
      ਚੋਣ (not_ਮਿਲਿਆ)
      {
	ਹਾਲਤ HB_NOT_ਮਿਲਿਆ_DONT_STORE:
	  ਰੋਕੋ;

	ਹਾਲਤ HB_NOT_ਮਿਲਿਆ_STORE:
	  *pos = ਮੰਜ਼ਿਲ_store;
	  ਰੋਕੋ;

	ਹਾਲਤ HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST:
	  *pos = ਲੰਮਾਈ;
	  ਰੋਕੋ;
      }
    }
    ਵਾਪਸ ਝੂਠ;
  }

  hb_sorted_array_t<Type> qsort (ਪੂਰਨ (*cmp)(ਅਚਲ ਖਾਲੀ*, ਅਚਲ ਖਾਲੀ*))
  {
    ਜੇ (likely (ਲੰਮਾਈ))
      hb_qsort (arrayZ, ਲੰਮਾਈ, ਇਹ->get_ਵਸਤੂ_ਮਾਪ (), cmp);
    ਵਾਪਸ hb_sorted_array_t<Type> (*ਇਹ);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    ਜੇ (likely (ਲੰਮਾਈ))
      hb_qsort_inline (arrayZ, ਲੰਮਾਈ, compar);
    ਵਾਪਸ hb_sorted_array_t<Type> (*ਇਹ);
  }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_ਤਰਜੀਹ<1>)
  {
    ਵਾਪਸ qsort ([] (ਅਚਲ Type &a, ਅਚਲ Type &b) { ਵਾਪਸ Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_ਤਰਜੀਹ<0>)
  {
    ਵਾਪਸ qsort ((ਪੂਰਨ(*)(ਅਚਲ ਖਾਲੀ*, ਅਚਲ ਖਾਲੀ*)) Type::cmp);
  }
  ਜਨਤਕ:

  hb_sorted_array_t<Type> qsort ()
  {
    ਵਾਪਸ _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_ਮਾਪ () ਅਚਲ { ਵਾਪਸ ਲੰਮਾਈ * ਇਹ->get_ਵਸਤੂ_ਮਾਪ (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  ਖਾਲੀ reverse (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = -1)
  {
    ਸ਼ੁਰੂ = hb_min (ਸ਼ੁਰੂ, ਲੰਮਾਈ);
    ਸਿਰਾ = hb_min (ਸਿਰਾ, ਲੰਮਾਈ);

    ਜੇ (ਸਿਰਾ < ਸ਼ੁਰੂ + 2)
      ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ stop = ਸ਼ੁਰੂ + (ਸਿਰਾ - ਸ਼ੁਰੂ) / 2;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ lhs = ਸ਼ੁਰੂ, rhs = ਸਿਰਾ - 1; lhs < stop; lhs++, rhs--)
      hb_ਅਦਲਾ_ਬਦਲੀ (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ = 0, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *seg_ਗਿਣਤੀ = ਖਾਲੀ_ਸੂਚਕ /* IN/OUT */) ਅਚਲ
  {
    ਜੇ (!ਸ਼ੁਰੂ_ਵਿੱਥ && !seg_ਗਿਣਤੀ)
      ਵਾਪਸ *ਇਹ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਲੰਮਾਈ;
    ਜੇ (unlikely (ਸ਼ੁਰੂ_ਵਿੱਥ > ਗਿਣਤੀ_2))
      ਗਿਣਤੀ_2 = 0;
    ਨਹੀਂਤਾਂ
      ਗਿਣਤੀ_2 -= ਸ਼ੁਰੂ_ਵਿੱਥ;
    ਜੇ (seg_ਗਿਣਤੀ)
      ਗਿਣਤੀ_2 = *seg_ਗਿਣਤੀ = hb_min (ਗਿਣਤੀ_2, *seg_ਗਿਣਤੀ);
    ਵਾਪਸ hb_array_t (arrayZ + ਸ਼ੁਰੂ_ਵਿੱਥ, ਗਿਣਤੀ_2);
  }
  hb_array_t sub_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seg_ਗਿਣਤੀ) ਅਚਲ
  { ਵਾਪਸ sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, &seg_ਗਿਣਤੀ); }

  hb_array_t truncate (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ) ਅਚਲ { ਵਾਪਸ sub_array (0, ਲੰਮਾਈ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
	    ਬਿਨਾਨਿਸ਼ਾਨੀ P = ਆਕਾਰ (Type),
	    hb_enable_if (P == 1)>
  ਅਚਲ T *as () ਅਚਲ
  { ਵਾਪਸ ਲੰਮਾਈ < hb_min_ਮਾਪ (T) ? &Null (T) : reinterpret_cast<ਅਚਲ T *> (arrayZ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T,
	    ਬਿਨਾਨਿਸ਼ਾਨੀ P = ਆਕਾਰ (Type),
	    hb_enable_if (P == 1)>
  ਤਾਰਕਿਕ check_range (ਅਚਲ T *p, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ = T::static_ਮਾਪ) ਅਚਲ
  {
    ਵਾਪਸ arrayZ <= ((ਅਚਲ ਅੱਖਰ *) p)
	&& ((ਅਚਲ ਅੱਖਰ *) p) <= arrayZ + ਲੰਮਾਈ
	&& (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (arrayZ + ਲੰਮਾਈ - (ਅਚਲ ਅੱਖਰ *) p) >= ਮਾਪ;
  }

  ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ P = ਆਕਾਰ (Type),
	    hb_enable_if (P == 1)>
  ਤਾਰਕਿਕ check_ਸਿਰਾ (ਅਚਲ ਖਾਲੀ *p) ਅਚਲ
  {
    ਵਾਪਸ (uintptr_t) (((ਅਚਲ ਅੱਖਰ *) p) - arrayZ) <= ਲੰਮਾਈ;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  ਖਾਲੀ fini ()
  { hb_free ((ਖਾਲੀ *) arrayZ); arrayZ = ਖਾਲੀ_ਸੂਚਕ; ਲੰਮਾਈ = 0; }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ hb_serialize_context_t,
	    ਕਿਸਮ_ਨਾਂ U = Type,
	    hb_enable_if (!(ਆਕਾਰ (U) < ਆਕਾਰ (ਲੰਮਾ ਲੰਮਾ) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ* ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (arrayZ);
    ਜੇ (unlikely (!c->extend_ਮਾਪ (ਬਾਹਰਲਾ, get_ਮਾਪ (), ਝੂਠ))) return_trace (hb_array_t ());
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਲੰਮਾਈ; i++)
      ਬਾਹਰਲਾ[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (ਬਾਹਰਲਾ, ਲੰਮਾਈ));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ hb_serialize_context_t,
	    ਕਿਸਮ_ਨਾਂ U = Type,
	    hb_enable_if (ਆਕਾਰ (U) < ਆਕਾਰ (ਲੰਮਾ ਲੰਮਾ) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    ਸਵੈਚਾਲਿਤ* ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (arrayZ);
    ਜੇ (unlikely (!c->extend_ਮਾਪ (ਬਾਹਰਲਾ, get_ਮਾਪ (), ਝੂਠ))) return_trace (hb_array_t ());
    hb_memcpy (ਬਾਹਰਲਾ, arrayZ, get_ਮਾਪ ());
    return_trace (hb_array_t (ਬਾਹਰਲਾ, ਲੰਮਾਈ));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ hb_sanitize_context_t>
  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  { ਵਾਪਸ c->check_array (arrayZ, ਲੰਮਾਈ); }

  /*
   * Members
   */

  ਜਨਤਕ:
  Type *arrayZ = ਖਾਲੀ_ਸੂਚਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ backwards_ਲੰਮਾਈ = 0;
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਤਰਅੰਦਰ hb_array_t<T>
hb_array ()
{ ਵਾਪਸ hb_array_t<T> (); }
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਤਰਅੰਦਰ hb_array_t<T>
hb_array (T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ)
{ ਵਾਪਸ hb_array_t<T> (array, ਲੰਮਾਈ); }
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ_2> ਸਤਰਅੰਦਰ hb_array_t<T>
hb_array (T (&array_)[ਲੰਮਾਈ_2])
{ ਵਾਪਸ hb_array_t<T> (array_); }

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Type>
ਬਣਤਰ hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_iter_t<hb_sorted_array_t, Type&> iter_ਅਧਾਰ_t;
  HB_ITER_USING (iter_ਅਧਾਰ_t);
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_random_access_iterator = ਸੱਚ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ is_sorted_iterator = ਸੱਚ;
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ has_fast_len = ਸੱਚ;

  hb_sorted_array_t () = ਮੂਲ;
  hb_sorted_array_t (ਅਚਲ hb_sorted_array_t&) = ਮੂਲ;
  ~hb_sorted_array_t () = ਮੂਲ;
  hb_sorted_array_t& ਸੰਚਾਲਕ= (ਅਚਲ hb_sorted_array_t&) = ਮੂਲ;
  hb_sorted_array_t& ਸੰਚਾਲਕ= (hb_sorted_array_t&&) = ਮੂਲ;

  constexpr hb_sorted_array_t (Type *array_, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ_2) : hb_array_t<Type> (array_, ਲੰਮਾਈ_2) {}
  ਸਾਂਚਾ <ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ_2>
  constexpr hb_sorted_array_t (Type (&array_)[ਲੰਮਾਈ_2]) : hb_array_t<Type> (array_) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (ਅਚਲ hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_array_t<U> &o)
  { hb_array_t<Type> (*ਇਹ) = o; ਵਾਪਸ *ਇਹ; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_sorted_array_t& o) ਅਚਲ
  { ਵਾਪਸ ਇਹ->arrayZ != o.arrayZ || ਇਹ->ਲੰਮਾਈ != o.ਲੰਮਾਈ; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () ਅਚਲ { ਵਾਪਸ ਇਹ->arrayZ; }
  Type *ਸਿਰਾ () ਅਚਲ { ਵਾਪਸ ਇਹ->arrayZ + ਇਹ->ਲੰਮਾਈ; }


  hb_sorted_array_t sub_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *seg_ਗਿਣਤੀ /* IN/OUT */) ਅਚਲ
  { ਵਾਪਸ hb_sorted_array_t (((ਅਚਲ hb_array_t<Type> *) (ਇਹ))->sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, seg_ਗਿਣਤੀ)); }
  hb_sorted_array_t sub_array (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਵਿੱਥ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seg_ਗਿਣਤੀ) ਅਚਲ
  { ਵਾਪਸ sub_array (ਸ਼ੁਰੂ_ਵਿੱਥ, &seg_ਗਿਣਤੀ); }

  hb_sorted_array_t truncate (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ) ਅਚਲ { ਵਾਪਸ sub_array (0, ਲੰਮਾਈ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  Type *bsearch (ਅਚਲ T &x, Type *not_ਮਿਲਿਆ = ਖਾਲੀ_ਸੂਚਕ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
    ਵਾਪਸ bfind (x, &i) ? &ਇਹ->arrayZ[i] : not_ਮਿਲਿਆ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਅਚਲ Type *bsearch (ਅਚਲ T &x, ਅਚਲ Type *not_ਮਿਲਿਆ = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;
    ਵਾਪਸ bfind (x, &i) ? &ਇਹ->arrayZ[i] : not_ਮਿਲਿਆ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ bfind (ਅਚਲ T &x, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *i = ਖਾਲੀ_ਸੂਚਕ,
	      hb_not_ਮਿਲਿਆ_t not_ਮਿਲਿਆ = HB_NOT_ਮਿਲਿਆ_DONT_STORE,
	      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮੰਜ਼ਿਲ_store = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) -1) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ pos;

    ਜੇ (bsearch_impl (x, &pos))
    {
      ਜੇ (i)
	*i = pos;
      ਵਾਪਸ ਸੱਚ;
    }

    ਜੇ (i)
    {
      ਚੋਣ (not_ਮਿਲਿਆ)
      {
	ਹਾਲਤ HB_NOT_ਮਿਲਿਆ_DONT_STORE:
	  ਰੋਕੋ;

	ਹਾਲਤ HB_NOT_ਮਿਲਿਆ_STORE:
	  *i = ਮੰਜ਼ਿਲ_store;
	  ਰੋਕੋ;

	ਹਾਲਤ HB_NOT_ਮਿਲਿਆ_STORE_CLOSEST:
	  *i = pos;
	  ਰੋਕੋ;
      }
    }
    ਵਾਪਸ ਝੂਠ;
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts>
  ਤਾਰਕਿਕ bsearch_impl (ਅਚਲ T &x, ਬਿਨਾਨਿਸ਼ਾਨੀ *pos, Ts... ds) ਅਚਲ
  {
    ਵਾਪਸ hb_bsearch_impl (pos,
			    x,
			    ਇਹ->arrayZ,
			    ਇਹ->ਲੰਮਾਈ,
			    ਆਕਾਰ (Type),
			    hb_cmp_ਵਿਧੀ<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T> ਸਤਰਅੰਦਰ hb_sorted_array_t<T>
hb_sorted_array (T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ)
{ ਵਾਪਸ hb_sorted_array_t<T> (array, ਲੰਮਾਈ); }
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ_2> ਸਤਰਅੰਦਰ hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[ਲੰਮਾਈ_2])
{ ਵਾਪਸ hb_sorted_array_t<T> (array_); }

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ hb_array_t<T>::ਸੰਚਾਲਕ == (ਅਚਲ hb_array_t<T> &o) ਅਚਲ
{
  ਜੇ (o.ਲੰਮਾਈ != ਇਹ->ਲੰਮਾਈ) ਵਾਪਸ ਝੂਠ;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਇਹ->ਲੰਮਾਈ; i++) {
    ਜੇ (ਇਹ->arrayZ[i] != o.arrayZ[i]) ਵਾਪਸ ਝੂਠ;
  }
  ਵਾਪਸ ਸੱਚ;
}
ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ hb_array_t<ਅਚਲ ਅੱਖਰ>::ਸੰਚਾਲਕ == (ਅਚਲ hb_array_t<ਅਚਲ ਅੱਖਰ> &o) ਅਚਲ
{
  ਜੇ (o.ਲੰਮਾਈ != ਇਹ->ਲੰਮਾਈ) ਵਾਪਸ ਝੂਠ;
  ਵਾਪਸ 0 == hb_memcmp (arrayZ, o.arrayZ, ਲੰਮਾਈ);
}
ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>::ਸੰਚਾਲਕ == (ਅਚਲ hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> &o) ਅਚਲ
{
  ਜੇ (o.ਲੰਮਾਈ != ਇਹ->ਲੰਮਾਈ) ਵਾਪਸ ਝੂਠ;
  ਵਾਪਸ 0 == hb_memcmp (arrayZ, o.arrayZ, ਲੰਮਾਈ);
}


/* Specialize hash() for byte arrays. */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ_ਹੋਰ
ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ uint32_t hb_array_t<ਅਚਲ ਅੱਖਰ>::hash () ਅਚਲ
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ਵਾਪਸ fasthash32(arrayZ, ਲੰਮਾਈ, 0xf437ffe6 /* magic? */);
}

ਸਾਂਚਾ <>
ਸਤਰਅੰਦਰ uint32_t hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>::hash () ਅਚਲ
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  ਵਾਪਸ fasthash32(arrayZ, ਲੰਮਾਈ, 0xf437ffe6 /* magic? */);
}
#ਜੇ_ਅੰਤ


ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_array_t<ਅਚਲ ਅੱਖਰ> hb_ਬਾਈਟਾਂ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_array_t<ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ> hb_ubytes_t;



#ਜੇ_ਅੰਤ /* HB_ARRAY_HH */
