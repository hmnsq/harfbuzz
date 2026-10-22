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

#جے_بنا_تعریف HB_سیٹ_HH
#تعریف_کرو HB_سیٹ_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set-invertible.hh"
#ضمن "hb-bit-vector.hh" // Just to include


سانچا <قسم_ناں impl_t>
بنية hb_sparseset_t
{
  ساكن constexpr منطقي realloc_move = سچ;

  hb_object_سرخی_t سرخی;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ثابت hb_sparseset_t& other) : hb_sparseset_t () { سیٹ (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& عامل = (ثابت hb_sparseset_t& other) { سیٹ (other); أرجع *هذا; }
  hb_sparseset_t& عامل = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); أرجع *هذا; }
  یار فراغ swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_وٹاندرا (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    لكل (خودکار&& شے : lst)
      جوڑو (شے);
  }
  سانچا <قسم_ناں Iterable,
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
  منطقي وچ_error () ثابت { أرجع s.وچ_error (); }

  فراغ alloc (بنانشانی sz) { s.alloc (sz); }
  فراغ reset () { s.reset (); }
  فراغ clear () { s.clear (); }
  فراغ invert () { s.invert (); }
  منطقي is_inverted () ثابت { أرجع s.is_inverted (); }
  منطقي is_empty () ثابت { أرجع s.is_empty (); }
  uint32_t hash () ثابت { أرجع s.hash (); }

  فراغ جوڑو (hb_codepoint_t g) { s.جوڑو (g); }
  فراغ جوڑو_بٹ (hb_codepoint_t g, uint64_t بٹ) { s.جوڑو_بٹ (g, بٹ); }
  منطقي جوڑو_range (hb_codepoint_t پہلا, hb_codepoint_t آخری) { أرجع s.جوڑو_range (پہلا, آخری); }

  سانچا <قسم_ناں T>
  فراغ جوڑو_array (ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  { s.جوڑو_array (array, گنتی, stride); }
  سانچا <قسم_ناں T>
  فراغ جوڑو_array (ثابت hb_array_t<ثابت T>& arr) { جوڑو_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچا <قسم_ناں T>
  منطقي جوڑو_sorted_array (ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  { أرجع s.جوڑو_sorted_array (array, گنتی, stride); }
  سانچا <قسم_ناں T>
  منطقي جوڑو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جوڑو_sorted_array (&arr, arr.len ()); }

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
  { جوڑو (v); أرجع *هذا; }
  hb_sparseset_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { جوڑو_range (range.پہلا, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.may_intersect (other.s); }

  منطقي intersects (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.intersects (other.s); }

  منطقي intersects (hb_codepoint_t پہلا, hb_codepoint_t آخری) ثابت
  { أرجع s.intersects (پہلا, آخری); }

  فراغ سیٹ (ثابت hb_sparseset_t &other) { s.سیٹ (other.s); }

  منطقي is_equal (ثابت hb_sparseset_t &other) ثابت { أرجع s.is_equal (other.s); }
  منطقي عامل == (ثابت hb_سیٹ_t &other) ثابت { أرجع is_equal (other); }
  منطقي عامل != (ثابت hb_سیٹ_t &other) ثابت { أرجع !is_equal (other); }

  منطقي is_subset (ثابت hb_sparseset_t &larger_سیٹ) ثابت { أرجع s.is_subset (larger_سیٹ.s); }

  فراغ union_ (ثابت hb_sparseset_t &other) { s.union_ (other.s); }
  فراغ intersect (ثابت hb_sparseset_t &other,
		  hb_سمتیہ_t<بنانشانی> *workspace = خالی_سوچک)
  { s.intersect (other.s, workspace); }
  فراغ گھٹاؤ (ثابت hb_sparseset_t &other) { s.گھٹاؤ (other.s); }
  فراغ متشاکل_difference (ثابت hb_sparseset_t &other) { s.متشاکل_difference (other.s); }

  منطقي next (hb_codepoint_t *codepoint) ثابت { أرجع s.next (codepoint); }
  منطقي اگلا_بٹ (hb_codepoint_t *codepoint, uint64_t *بٹ) ثابت
  { أرجع s.اگلا_بٹ (codepoint, بٹ); }
  منطقي پچھلا (hb_codepoint_t *codepoint) ثابت { أرجع s.پچھلا (codepoint); }
  منطقي اگلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  { أرجع s.اگلا_range (پہلا, آخری); }
  منطقي پچھلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  { أرجع s.پچھلا_range (پہلا, آخری); }
  بنانشانی صحيح اگلا_many (hb_codepoint_t codepoint, hb_codepoint_t *باہرلا, بنانشانی صحيح ناپ) ثابت
  { أرجع s.اگلا_many (codepoint, باہرلا, ناپ); }

  بنانشانی صحيح get_population () ثابت { أرجع s.get_population (); }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت { أرجع s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ثابت { أرجع s.get_min (); }
  hb_codepoint_t get_max () ثابت { أرجع s.get_max (); }

  ساكن constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  ورتوں iter_t = قسم_ناں impl_t::iter_t;
  iter_t iter () ثابت { أرجع iter_t (هذا->s); }
  عامل iter_t () ثابت { أرجع iter (); }
};

بنية hb_سیٹ_t : hb_sparseset_t<hb_ثنائی_ہندسہ_سیٹ_invertible_t>
{
  ورتوں sparseset = hb_sparseset_t<hb_ثنائی_ہندسہ_سیٹ_invertible_t>;

  ~hb_سیٹ_t () = افتراضي;
  hb_سیٹ_t () : sparseset () {};
  hb_سیٹ_t (ثابت hb_سیٹ_t &o) : sparseset ((sparseset &) o) {};
  hb_سیٹ_t (hb_سیٹ_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_سیٹ_t& عامل = (ثابت hb_سیٹ_t&) = افتراضي;
  hb_سیٹ_t& عامل = (hb_سیٹ_t&&) = افتراضي;
  hb_سیٹ_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  سانچا <قسم_ناں Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_سیٹ_t (ثابت Iterable &o) : sparseset (o) {}

  hb_سیٹ_t& عامل << (hb_codepoint_t v)
  { sparseset::عامل<< (v); أرجع *هذا; }
  hb_سیٹ_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { sparseset::عامل<< (range); أرجع *هذا; }
};

static_assert (hb_سیٹ_t::INVALID == HB_سیٹ_قدر_INVALID, "");


#جے_انت /* HB_SET_HH */
