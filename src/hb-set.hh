/*
 * Copyright © 2012,2017  Google, Inc.
 * Copyright © 2021 Behdad Esfahbod
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

#جيڪڏهن_وصف_نه_ٿيل HB_سيٽ_HH
#وصف_ڪريو HB_سيٽ_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set-invertible.hh"
#ضمن "hb-bit-vector.hh" // Just to include


سانچو <قسم_نالو impl_t>
بنية hb_sparseset_t
{
  ساكن constexpr منطقي realloc_move = سچ;

  hb_object_سرو_t سرو;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ثابت hb_sparseset_t& other) : hb_sparseset_t () { سيٽ (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& عامل = (ثابت hb_sparseset_t& other) { سيٽ (other); أرجع *هذا; }
  hb_sparseset_t& عامل = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); أرجع *هذا; }
  دوست فراغ swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_مٽا_سٽا (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    لكل (خودڪار&& شي : lst)
      جوڙيو (شي);
  }
  سانچو <قسم_نالو Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ثابت Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *هذا);
  }

  فراغ init ()
  {
    hb_object_init (هذا);
    s.init ();
  }
  فراغ fini ()
  {
    hb_object_fini (هذا);
    s.fini ();
  }

  واضح عامل منطقي () ثابت { أرجع !is_empty (); }

  فراغ err () { s.err (); }
  منطقي اندريون_error () ثابت { أرجع s.اندريون_error (); }

  فراغ alloc (بنانشان sz) { s.alloc (sz); }
  فراغ reset () { s.reset (); }
  فراغ clear () { s.clear (); }
  فراغ invert () { s.invert (); }
  منطقي is_inverted () ثابت { أرجع s.is_inverted (); }
  منطقي is_empty () ثابت { أرجع s.is_empty (); }
  uint32_t hash () ثابت { أرجع s.hash (); }

  فراغ جوڙيو (hb_codepoint_t g) { s.جوڙيو (g); }
  فراغ جوڙيو_بٽ (hb_codepoint_t g, uint64_t بٽ) { s.جوڙيو_بٽ (g, بٽ); }
  منطقي جوڙيو_range (hb_codepoint_t پهريون, hb_codepoint_t آخري) { أرجع s.جوڙيو_range (پهريون, آخري); }

  سانچو <قسم_نالو T>
  فراغ جوڙيو_array (ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  { s.جوڙيو_array (array, ڳڻپ, stride); }
  سانچو <قسم_نالو T>
  فراغ جوڙيو_array (ثابت hb_array_t<ثابت T>& arr) { جوڙيو_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچو <قسم_نالو T>
  منطقي جوڙيو_sorted_array (ثابت T *array, بنانشان صحيح ڳڻپ, بنانشان صحيح stride=حجم(T))
  { أرجع s.جوڙيو_sorted_array (array, ڳڻپ, stride); }
  سانچو <قسم_نالو T>
  منطقي جوڙيو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جوڙيو_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { أرجع s.get (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي عامل [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }

  /* Predicate. */
  منطقي عامل () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_sparseset_t& عامل << (hb_codepoint_t v)
  { جوڙيو (v); أرجع *هذا; }
  hb_sparseset_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { جوڙيو_range (range.پهريون, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.may_intersect (other.s); }

  منطقي intersects (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.intersects (other.s); }

  منطقي intersects (hb_codepoint_t پهريون, hb_codepoint_t آخري) ثابت
  { أرجع s.intersects (پهريون, آخري); }

  فراغ سيٽ (ثابت hb_sparseset_t &other) { s.سيٽ (other.s); }

  منطقي is_equal (ثابت hb_sparseset_t &other) ثابت { أرجع s.is_equal (other.s); }
  منطقي عامل == (ثابت hb_سيٽ_t &other) ثابت { أرجع is_equal (other); }
  منطقي عامل != (ثابت hb_سيٽ_t &other) ثابت { أرجع !is_equal (other); }

  منطقي is_subset (ثابت hb_sparseset_t &larger_سيٽ) ثابت { أرجع s.is_subset (larger_سيٽ.s); }

  فراغ union_ (ثابت hb_sparseset_t &other) { s.union_ (other.s); }
  فراغ intersect (ثابت hb_sparseset_t &other,
		  hb_ويڪٽر_t<بنانشان> *workspace = خالي_اشارو)
  { s.intersect (other.s, workspace); }
  فراغ ڪٽيو (ثابت hb_sparseset_t &other) { s.ڪٽيو (other.s); }
  فراغ متناظر_difference (ثابت hb_sparseset_t &other) { s.متناظر_difference (other.s); }

  منطقي next (hb_codepoint_t *codepoint) ثابت { أرجع s.next (codepoint); }
  منطقي ايندڙ_بٽ (hb_codepoint_t *codepoint, uint64_t *بٽ) ثابت
  { أرجع s.ايندڙ_بٽ (codepoint, بٽ); }
  منطقي اڳيون (hb_codepoint_t *codepoint) ثابت { أرجع s.اڳيون (codepoint); }
  منطقي ايندڙ_range (hb_codepoint_t *پهريون, hb_codepoint_t *آخري) ثابت
  { أرجع s.ايندڙ_range (پهريون, آخري); }
  منطقي اڳيون_range (hb_codepoint_t *پهريون, hb_codepoint_t *آخري) ثابت
  { أرجع s.اڳيون_range (پهريون, آخري); }
  بنانشان صحيح ايندڙ_many (hb_codepoint_t codepoint, hb_codepoint_t *ٻاهريون, بنانشان صحيح سائيز) ثابت
  { أرجع s.ايندڙ_many (codepoint, ٻاهريون, سائيز); }

  بنانشان صحيح get_population () ثابت { أرجع s.get_population (); }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت { أرجع s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ثابت { أرجع s.get_min (); }
  hb_codepoint_t get_max () ثابت { أرجع s.get_max (); }

  ساكن constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  استعمال iter_t = قسم_نالو impl_t::iter_t;
  iter_t iter () ثابت { أرجع iter_t (هذا->s); }
  عامل iter_t () ثابت { أرجع iter (); }
};

بنية hb_سيٽ_t : hb_sparseset_t<hb_ٻٽو_انگ_سيٽ_invertible_t>
{
  استعمال sparseset = hb_sparseset_t<hb_ٻٽو_انگ_سيٽ_invertible_t>;

  ~hb_سيٽ_t () = افتراضي;
  hb_سيٽ_t () : sparseset () {};
  hb_سيٽ_t (ثابت hb_سيٽ_t &o) : sparseset ((sparseset &) o) {};
  hb_سيٽ_t (hb_سيٽ_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_سيٽ_t& عامل = (ثابت hb_سيٽ_t&) = افتراضي;
  hb_سيٽ_t& عامل = (hb_سيٽ_t&&) = افتراضي;
  hb_سيٽ_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  سانچو <قسم_نالو Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_سيٽ_t (ثابت Iterable &o) : sparseset (o) {}

  hb_سيٽ_t& عامل << (hb_codepoint_t v)
  { sparseset::عامل<< (v); أرجع *هذا; }
  hb_سيٽ_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { sparseset::عامل<< (range); أرجع *هذا; }
};

static_assert (hb_سيٽ_t::INVALID == HB_سيٽ_قدر_INVALID, "");


#جيڪڏهن_پورو /* HB_SET_HH */
