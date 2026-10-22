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

#ifndef HB_ARRAY_HH
#define HB_ARRAY_HH

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-algs.hh"
#അന്തര്ഭാവ "hb-iter.hh"
#അന്തര്ഭാവ "hb-null.hh"


template <typename Type>
ഘടന hb_sorted_array_t;

എണ്ണൽ hb_not_found_t
{
  HB_NOT_FOUND_DONT_STORE,
  HB_NOT_FOUND_STORE,
  HB_NOT_FOUND_STORE_CLOSEST,
};


template <typename Type>
ഘടന hb_array_t : hb_iter_with_fallback_t<hb_array_t<Type>, Type&>
{
  സ്ഥിതം constexpr തർക്കം realloc_move = സത്യം;

  /*
   * Constructors.
   */
  hb_array_t () = സ്ഥിരസ്ഥിതി;
  hb_array_t (സ്ഥിരം hb_array_t&) = സ്ഥിരസ്ഥിതി;
  ~hb_array_t () = സ്ഥിരസ്ഥിതി;
  hb_array_t& operator= (സ്ഥിരം hb_array_t&) = സ്ഥിരസ്ഥിതി;
  hb_array_t& operator= (hb_array_t&&) = സ്ഥിരസ്ഥിതി;

  constexpr hb_array_t (Type *array_, unsigned പൂർണ്ണസംഖ്യ length_) : arrayZ (array_), length (length_) {}
  template <unsigned പൂർണ്ണസംഖ്യ length_>
  constexpr hb_array_t (Type (&array_)[length_]) : hb_array_t (array_, length_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_array_t (സ്ഥിരം hb_array_t<U> &o) :
    hb_iter_with_fallback_t<hb_array_t, Type&> (),
    arrayZ (o.arrayZ), length (o.length), backwards_length (o.backwards_length) {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_array_t& operator = (സ്ഥിരം hb_array_t<U> &o)
  { arrayZ = o.arrayZ; length = o.length; backwards_length = o.backwards_length; തിരികെ *ഇത്; }

  /*
   * Iterator implementation.
   */
  തരനിർവചനം Type& __item_t__;
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator = സത്യം;
  സ്ഥിതം constexpr തർക്കം has_fast_len = സത്യം;
  Type& __item__ () സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!length)) തിരികെ CrapOrNull (Type);
    തിരികെ *arrayZ;
  }
  Type& __item_at__ (unsigned i) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (i >= length)) തിരികെ CrapOrNull (Type);
    തിരികെ arrayZ[i];
  }
  ശൂന്യം __next__ ()
  {
    എങ്കിൽ (unlikely (!length))
      തിരികെ;
    length--;
    backwards_length++;
    arrayZ++;
  }
  ശൂന്യം __forward__ (unsigned n)
  {
    എങ്കിൽ (unlikely (n > length))
      n = length;
    length -= n;
    backwards_length += n;
    arrayZ += n;
  }
  ശൂന്യം __prev__ ()
  {
    എങ്കിൽ (unlikely (!backwards_length))
      തിരികെ;
    length++;
    backwards_length--;
    arrayZ--;
  }
  ശൂന്യം __rewind__ (unsigned n)
  {
    എങ്കിൽ (unlikely (n > backwards_length))
      n = backwards_length;
    length += n;
    backwards_length -= n;
    arrayZ -= n;
  }
  unsigned __len__ () സ്ഥിരം { തിരികെ length; }
  /* Ouch. The operator== compares the contents of the array.  For range-based for loops,
   * it's best if we can just compare arrayZ, though comparing contents is still fast,
   * but also would require that Type has operator==.  As such, we optimize this operator
   * for range-based for loop and just compare arrayZ and length.
   *
   * The above comment is outdated now because we implemented separate begin/end to
   * objects that were using hb_array_t for range-based loop before. */
  തർക്കം operator != (സ്ഥിരം hb_array_t& o) സ്ഥിരം
  { തിരികെ ഇത്->arrayZ != o.arrayZ || ഇത്->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () സ്ഥിരം { തിരികെ arrayZ; }
  Type *end () സ്ഥിരം { തിരികെ arrayZ + length; }


  /* Extra operators.
   */
  Type * operator & () സ്ഥിരം { തിരികെ arrayZ; }
  operator hb_array_t<സ്ഥിരം Type> () { തിരികെ hb_array_t<സ്ഥിരം Type> (arrayZ, length); }
  template <typename T> operator T * () സ്ഥിരം { തിരികെ arrayZ; }

  HB_INTERNAL തർക്കം operator == (സ്ഥിരം hb_array_t &o) സ്ഥിരം;

  uint32_t hash () സ്ഥിരം
  {
    // FNV-1a hash function
    // https://github.com/harfbuzz/harfbuzz/pull/4228
    uint32_t current = /*cbf29ce4*/0x84222325;
    വേണ്ടി (auto &v : *ഇത്)
    {
      current = current ^ hb_hash (v);
      current = current * 16777619;
    }
    തിരികെ current;
  }

  /*
   * Compare, Sort, and Search.
   */

  /* Note: our compare is NOT lexicographic; it also does NOT call Type::cmp. */
  പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം hb_array_t &a) സ്ഥിരം
  {
    എങ്കിൽ (length != a.length)
      തിരികെ (പൂർണ്ണസംഖ്യ) a.length - (പൂർണ്ണസംഖ്യ) length;
    തിരികെ hb_memcmp (a.arrayZ, arrayZ, get_size ());
  }
  HB_INTERNAL സ്ഥിതം പൂർണ്ണസംഖ്യ cmp (സ്ഥിരം ശൂന്യം *pa, സ്ഥിരം ശൂന്യം *pb)
  {
    hb_array_t *a = (hb_array_t *) pa;
    hb_array_t *b = (hb_array_t *) pb;
    തിരികെ b->cmp (*a);
  }

  template <typename T>
  Type *lsearch (സ്ഥിരം T &x, Type *not_found = nullptr)
  {
    unsigned i;
    തിരികെ lfind (x, &i) ? &ഇത്->arrayZ[i] : not_found;
  }
  template <typename T>
  സ്ഥിരം Type *lsearch (സ്ഥിരം T &x, സ്ഥിരം Type *not_found = nullptr) സ്ഥിരം
  {
    unsigned i;
    തിരികെ lfind (x, &i) ? &ഇത്->arrayZ[i] : not_found;
  }
  template <typename T>
  തർക്കം lfind (സ്ഥിരം T &x, unsigned *pos = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned പൂർണ്ണസംഖ്യ to_store = (unsigned പൂർണ്ണസംഖ്യ) -1) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < length; ++i)
      എങ്കിൽ (hb_equal (x, ഇത്->arrayZ[i]))
      {
	എങ്കിൽ (pos)
	  *pos = i;
	തിരികെ സത്യം;
      }

    എങ്കിൽ (pos)
    {
      തിരഞ്ഞെടുപ്പ് (not_found)
      {
	അവസ്ഥ HB_NOT_FOUND_DONT_STORE:
	  നിർത്തുക;

	അവസ്ഥ HB_NOT_FOUND_STORE:
	  *pos = to_store;
	  നിർത്തുക;

	അവസ്ഥ HB_NOT_FOUND_STORE_CLOSEST:
	  *pos = length;
	  നിർത്തുക;
      }
    }
    തിരികെ അസത്യം;
  }

  hb_sorted_array_t<Type> qsort (പൂർണ്ണസംഖ്യ (*cmp)(സ്ഥിരം ശൂന്യം*, സ്ഥിരം ശൂന്യം*))
  {
    എങ്കിൽ (likely (length))
      hb_qsort (arrayZ, length, ഇത്->get_item_size (), cmp);
    തിരികെ hb_sorted_array_t<Type> (*ഇത്);
  }

  /* Comparator follows the C qsort convention: returns
   * negative / zero / positive int. */
  template <typename Compar>
  hb_sorted_array_t<Type> qsort (Compar compar)
  {
    എങ്കിൽ (likely (length))
      hb_qsort_inline (arrayZ, length, compar);
    തിരികെ hb_sorted_array_t<Type> (*ഇത്);
  }

  സ്വകാര്യ:
  template <typename T = Type,
	    hb_enable_if (std::is_move_assignable<T>::value)>
  hb_sorted_array_t<Type> _qsort (hb_priority<1>)
  {
    തിരികെ qsort ([] (സ്ഥിരം Type &a, സ്ഥിരം Type &b) { തിരികെ Type::cmp (&a, &b); });
  }
  hb_sorted_array_t<Type> _qsort (hb_priority<0>)
  {
    തിരികെ qsort ((പൂർണ്ണസംഖ്യ(*)(സ്ഥിരം ശൂന്യം*, സ്ഥിരം ശൂന്യം*)) Type::cmp);
  }
  പൊതു:

  hb_sorted_array_t<Type> qsort ()
  {
    തിരികെ _qsort (hb_prioritize);
  }

  /*
   * Other methods.
   */

  size_t get_size () സ്ഥിരം { തിരികെ length * ഇത്->get_item_size (); }

  /*
   * Reverse the order of items in this array in the range [start, end).
   */
  ശൂന്യം reverse (unsigned start = 0, unsigned end = -1)
  {
    start = hb_min (start, length);
    end = hb_min (end, length);

    എങ്കിൽ (end < start + 2)
      തിരികെ;

    unsigned stop = start + (end - start) / 2;
    വേണ്ടി (unsigned lhs = start, rhs = end - 1; lhs < stop; lhs++, rhs--)
      hb_swap (arrayZ[rhs], arrayZ[lhs]);
  }

  hb_array_t sub_array (unsigned പൂർണ്ണസംഖ്യ start_offset = 0, unsigned പൂർണ്ണസംഖ്യ *seg_എണ്ണം = nullptr /* IN/OUT */) സ്ഥിരം
  {
    എങ്കിൽ (!start_offset && !seg_എണ്ണം)
      തിരികെ *ഇത്;

    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = length;
    എങ്കിൽ (unlikely (start_offset > എണ്ണം))
      എണ്ണം = 0;
    അല്ലെങ്കിൽ
      എണ്ണം -= start_offset;
    എങ്കിൽ (seg_എണ്ണം)
      എണ്ണം = *seg_എണ്ണം = hb_min (എണ്ണം, *seg_എണ്ണം);
    തിരികെ hb_array_t (arrayZ + start_offset, എണ്ണം);
  }
  hb_array_t sub_array (unsigned പൂർണ്ണസംഖ്യ start_offset, unsigned പൂർണ്ണസംഖ്യ seg_എണ്ണം) സ്ഥിരം
  { തിരികെ sub_array (start_offset, &seg_എണ്ണം); }

  hb_array_t truncate (unsigned length) സ്ഥിരം { തിരികെ sub_array (0, length); }

  template <typename T,
	    unsigned P = വലുപ്പം (Type),
	    hb_enable_if (P == 1)>
  സ്ഥിരം T *as () സ്ഥിരം
  { തിരികെ length < hb_min_size (T) ? &Null (T) : reinterpret_cast<സ്ഥിരം T *> (arrayZ); }

  template <typename T,
	    unsigned P = വലുപ്പം (Type),
	    hb_enable_if (P == 1)>
  തർക്കം check_range (സ്ഥിരം T *p, unsigned പൂർണ്ണസംഖ്യ size = T::static_size) സ്ഥിരം
  {
    തിരികെ arrayZ <= ((സ്ഥിരം അക്ഷരം *) p)
	&& ((സ്ഥിരം അക്ഷരം *) p) <= arrayZ + length
	&& (unsigned പൂർണ്ണസംഖ്യ) (arrayZ + length - (സ്ഥിരം അക്ഷരം *) p) >= size;
  }

  template <unsigned P = വലുപ്പം (Type),
	    hb_enable_if (P == 1)>
  തർക്കം check_end (സ്ഥിരം ശൂന്യം *p) സ്ഥിരം
  {
    തിരികെ (uintptr_t) (((സ്ഥിരം അക്ഷരം *) p) - arrayZ) <= length;
  }

  /* Only call if you allocated the underlying array using hb_malloc() or similar. */
  ശൂന്യം fini ()
  { hb_free ((ശൂന്യം *) arrayZ); arrayZ = nullptr; length = 0; }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (!(വലുപ്പം (U) < വലുപ്പം (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>)))>
  hb_array_t copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto* out = c->start_embed (arrayZ);
    എങ്കിൽ (unlikely (!c->extend_size (out, get_size (), അസത്യം))) return_trace (hb_array_t ());
    വേണ്ടി (unsigned i = 0; i < length; i++)
      out[i] = arrayZ[i]; /* TODO: add version that calls c->copy() */
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_serialize_context_t,
	    typename U = Type,
	    hb_enable_if (വലുപ്പം (U) < വലുപ്പം (long long) && hb_is_trivially_copy_assignable(hb_decay<Type>))>
  hb_array_t copy (hb_serialize_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    auto* out = c->start_embed (arrayZ);
    എങ്കിൽ (unlikely (!c->extend_size (out, get_size (), അസത്യം))) return_trace (hb_array_t ());
    hb_memcpy (out, arrayZ, get_size ());
    return_trace (hb_array_t (out, length));
  }

  template <typename hb_sanitize_context_t>
  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  { തിരികെ c->check_array (arrayZ, length); }

  /*
   * Members
   */

  പൊതു:
  Type *arrayZ = nullptr;
  unsigned പൂർണ്ണസംഖ്യ length = 0;
  unsigned പൂർണ്ണസംഖ്യ backwards_length = 0;
};
template <typename T> വരിയിൽ hb_array_t<T>
hb_array ()
{ തിരികെ hb_array_t<T> (); }
template <typename T> വരിയിൽ hb_array_t<T>
hb_array (T *array, unsigned പൂർണ്ണസംഖ്യ length)
{ തിരികെ hb_array_t<T> (array, length); }
template <typename T, unsigned പൂർണ്ണസംഖ്യ length_> വരിയിൽ hb_array_t<T>
hb_array (T (&array_)[length_])
{ തിരികെ hb_array_t<T> (array_); }

template <typename Type>
ഘടന hb_sorted_array_t :
	hb_array_t<Type>,
	hb_iter_t<hb_sorted_array_t<Type>, Type&>
{
  തരനിർവചനം hb_iter_t<hb_sorted_array_t, Type&> iter_base_t;
  HB_ITER_USING (iter_base_t);
  സ്ഥിതം constexpr തർക്കം is_random_access_iterator = സത്യം;
  സ്ഥിതം constexpr തർക്കം is_sorted_iterator = സത്യം;
  സ്ഥിതം constexpr തർക്കം has_fast_len = സത്യം;

  hb_sorted_array_t () = സ്ഥിരസ്ഥിതി;
  hb_sorted_array_t (സ്ഥിരം hb_sorted_array_t&) = സ്ഥിരസ്ഥിതി;
  ~hb_sorted_array_t () = സ്ഥിരസ്ഥിതി;
  hb_sorted_array_t& operator= (സ്ഥിരം hb_sorted_array_t&) = സ്ഥിരസ്ഥിതി;
  hb_sorted_array_t& operator= (hb_sorted_array_t&&) = സ്ഥിരസ്ഥിതി;

  constexpr hb_sorted_array_t (Type *array_, unsigned പൂർണ്ണസംഖ്യ length_) : hb_array_t<Type> (array_, length_) {}
  template <unsigned പൂർണ്ണസംഖ്യ length_>
  constexpr hb_sorted_array_t (Type (&array_)[length_]) : hb_array_t<Type> (array_) {}

  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  constexpr hb_sorted_array_t (സ്ഥിരം hb_array_t<U> &o) :
    hb_array_t<Type> (o),
    hb_iter_t<hb_sorted_array_t, Type&> () {}
  template <typename U,
	    hb_enable_if (hb_is_cr_convertible(U, Type))>
  hb_sorted_array_t& operator = (സ്ഥിരം hb_array_t<U> &o)
  { hb_array_t<Type> (*ഇത്) = o; തിരികെ *ഇത്; }

  /* Iterator implementation. */

  /* See comment in hb_array_of::operator != */
  തർക്കം operator != (സ്ഥിരം hb_sorted_array_t& o) സ്ഥിരം
  { തിരികെ ഇത്->arrayZ != o.arrayZ || ഇത്->length != o.length; }

  /* Faster range-based for loop without bounds-check. */
  Type *begin () സ്ഥിരം { തിരികെ ഇത്->arrayZ; }
  Type *end () സ്ഥിരം { തിരികെ ഇത്->arrayZ + ഇത്->length; }


  hb_sorted_array_t sub_array (unsigned പൂർണ്ണസംഖ്യ start_offset, unsigned പൂർണ്ണസംഖ്യ *seg_എണ്ണം /* IN/OUT */) സ്ഥിരം
  { തിരികെ hb_sorted_array_t (((സ്ഥിരം hb_array_t<Type> *) (ഇത്))->sub_array (start_offset, seg_എണ്ണം)); }
  hb_sorted_array_t sub_array (unsigned പൂർണ്ണസംഖ്യ start_offset, unsigned പൂർണ്ണസംഖ്യ seg_എണ്ണം) സ്ഥിരം
  { തിരികെ sub_array (start_offset, &seg_എണ്ണം); }

  hb_sorted_array_t truncate (unsigned length) സ്ഥിരം { തിരികെ sub_array (0, length); }

  template <typename T>
  Type *bsearch (സ്ഥിരം T &x, Type *not_found = nullptr)
  {
    unsigned പൂർണ്ണസംഖ്യ i;
    തിരികെ bfind (x, &i) ? &ഇത്->arrayZ[i] : not_found;
  }
  template <typename T>
  സ്ഥിരം Type *bsearch (സ്ഥിരം T &x, സ്ഥിരം Type *not_found = nullptr) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ i;
    തിരികെ bfind (x, &i) ? &ഇത്->arrayZ[i] : not_found;
  }
  template <typename T>
  തർക്കം bfind (സ്ഥിരം T &x, unsigned പൂർണ്ണസംഖ്യ *i = nullptr,
	      hb_not_found_t not_found = HB_NOT_FOUND_DONT_STORE,
	      unsigned പൂർണ്ണസംഖ്യ to_store = (unsigned പൂർണ്ണസംഖ്യ) -1) സ്ഥിരം
  {
    unsigned pos;

    എങ്കിൽ (bsearch_impl (x, &pos))
    {
      എങ്കിൽ (i)
	*i = pos;
      തിരികെ സത്യം;
    }

    എങ്കിൽ (i)
    {
      തിരഞ്ഞെടുപ്പ് (not_found)
      {
	അവസ്ഥ HB_NOT_FOUND_DONT_STORE:
	  നിർത്തുക;

	അവസ്ഥ HB_NOT_FOUND_STORE:
	  *i = to_store;
	  നിർത്തുക;

	അവസ്ഥ HB_NOT_FOUND_STORE_CLOSEST:
	  *i = pos;
	  നിർത്തുക;
      }
    }
    തിരികെ അസത്യം;
  }
  template <typename T, typename ...Ts>
  തർക്കം bsearch_impl (സ്ഥിരം T &x, unsigned *pos, Ts... ds) സ്ഥിരം
  {
    തിരികെ hb_bsearch_impl (pos,
			    x,
			    ഇത്->arrayZ,
			    ഇത്->length,
			    വലുപ്പം (Type),
			    _hb_cmp_method<T, Type, Ts...>,
			    std::forward<Ts> (ds)...);
  }
};
template <typename T> വരിയിൽ hb_sorted_array_t<T>
hb_sorted_array (T *array, unsigned പൂർണ്ണസംഖ്യ length)
{ തിരികെ hb_sorted_array_t<T> (array, length); }
template <typename T, unsigned പൂർണ്ണസംഖ്യ length_> വരിയിൽ hb_sorted_array_t<T>
hb_sorted_array (T (&array_)[length_])
{ തിരികെ hb_sorted_array_t<T> (array_); }

template <typename T>
വരിയിൽ തർക്കം hb_array_t<T>::operator == (സ്ഥിരം hb_array_t<T> &o) സ്ഥിരം
{
  എങ്കിൽ (o.length != ഇത്->length) തിരികെ അസത്യം;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < ഇത്->length; i++) {
    എങ്കിൽ (ഇത്->arrayZ[i] != o.arrayZ[i]) തിരികെ അസത്യം;
  }
  തിരികെ സത്യം;
}
template <>
വരിയിൽ തർക്കം hb_array_t<സ്ഥിരം അക്ഷരം>::operator == (സ്ഥിരം hb_array_t<സ്ഥിരം അക്ഷരം> &o) സ്ഥിരം
{
  എങ്കിൽ (o.length != ഇത്->length) തിരികെ അസത്യം;
  തിരികെ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}
template <>
വരിയിൽ തർക്കം hb_array_t<സ്ഥിരം unsigned അക്ഷരം>::operator == (സ്ഥിരം hb_array_t<സ്ഥിരം unsigned അക്ഷരം> &o) സ്ഥിരം
{
  എങ്കിൽ (o.length != ഇത്->length) തിരികെ അസത്യം;
  തിരികെ 0 == hb_memcmp (arrayZ, o.arrayZ, length);
}


/* Specialize hash() for byte arrays. */

#ifndef HB_OPTIMIZE_SIZE_MORE
template <>
വരിയിൽ uint32_t hb_array_t<സ്ഥിരം അക്ഷരം>::hash () സ്ഥിരം
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  തിരികെ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}

template <>
വരിയിൽ uint32_t hb_array_t<സ്ഥിരം unsigned അക്ഷരം>::hash () സ്ഥിരം
{
  // https://github.com/harfbuzz/harfbuzz/pull/4228
  തിരികെ fasthash32(arrayZ, length, 0xf437ffe6 /* magic? */);
}
#endif


തരനിർവചനം hb_array_t<സ്ഥിരം അക്ഷരം> hb_bytes_t;
തരനിർവചനം hb_array_t<സ്ഥിരം unsigned അക്ഷരം> hb_ubytes_t;



#endif /* HB_ARRAY_HH */
