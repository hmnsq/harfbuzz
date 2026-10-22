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

#اگر_تعریف_نشده HB_مجموعه_HH
#تعریف_کن HB_مجموعه_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set-invertible.hh"
#ضمن "hb-bit-vector.hh" // Just to include


قالب <نام_نوع impl_t>
بنية hb_sparseset_t
{
  ساكن constexpr منطقي realloc_move = درست;

  hb_object_سرآیند_t سرآیند;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ثابت hb_sparseset_t& other) : hb_sparseset_t () { مجموعه (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& عملگر = (ثابت hb_sparseset_t& other) { مجموعه (other); أرجع *هذا; }
  hb_sparseset_t& عملگر = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); أرجع *هذا; }
  دوست فراغ swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_جابجا (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    لكل (خودکار&& قلم : lst)
      جمع_کن (قلم);
  }
  قالب <نام_نوع Iterable,
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

  صریح عملگر منطقي () ثابت { أرجع !is_empty (); }

  فراغ err () { s.err (); }
  منطقي درون_error () ثابت { أرجع s.درون_error (); }

  فراغ alloc (بیعلامت sz) { s.alloc (sz); }
  فراغ reset () { s.reset (); }
  فراغ clear () { s.clear (); }
  فراغ invert () { s.invert (); }
  منطقي is_inverted () ثابت { أرجع s.is_inverted (); }
  منطقي is_empty () ثابت { أرجع s.is_empty (); }
  uint32_t hash () ثابت { أرجع s.hash (); }

  فراغ جمع_کن (hb_codepoint_t g) { s.جمع_کن (g); }
  فراغ جمع_کن_بیتها (hb_codepoint_t g, uint64_t بیتها) { s.جمع_کن_بیتها (g, بیتها); }
  منطقي جمع_کن_range (hb_codepoint_t اولی, hb_codepoint_t آخرین) { أرجع s.جمع_کن_range (اولی, آخرین); }

  قالب <نام_نوع T>
  فراغ جمع_کن_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  { s.جمع_کن_array (array, شمار, stride); }
  قالب <نام_نوع T>
  فراغ جمع_کن_array (ثابت hb_array_t<ثابت T>& arr) { جمع_کن_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  قالب <نام_نوع T>
  منطقي جمع_کن_sorted_array (ثابت T *array, بیعلامت صحيح شمار, بیعلامت صحيح stride=حجم(T))
  { أرجع s.جمع_کن_sorted_array (array, شمار, stride); }
  قالب <نام_نوع T>
  منطقي جمع_کن_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جمع_کن_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { أرجع s.get (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي عملگر [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }

  /* Predicate. */
  منطقي عملگر () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_sparseset_t& عملگر << (hb_codepoint_t v)
  { جمع_کن (v); أرجع *هذا; }
  hb_sparseset_t& عملگر << (ثابت hb_codepoint_pair_t& range)
  { جمع_کن_range (range.اولی, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.may_intersect (other.s); }

  منطقي intersects (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.intersects (other.s); }

  منطقي intersects (hb_codepoint_t اولی, hb_codepoint_t آخرین) ثابت
  { أرجع s.intersects (اولی, آخرین); }

  فراغ مجموعه (ثابت hb_sparseset_t &other) { s.مجموعه (other.s); }

  منطقي is_equal (ثابت hb_sparseset_t &other) ثابت { أرجع s.is_equal (other.s); }
  منطقي عملگر == (ثابت hb_مجموعه_t &other) ثابت { أرجع is_equal (other); }
  منطقي عملگر != (ثابت hb_مجموعه_t &other) ثابت { أرجع !is_equal (other); }

  منطقي is_subset (ثابت hb_sparseset_t &larger_مجموعه) ثابت { أرجع s.is_subset (larger_مجموعه.s); }

  فراغ union_ (ثابت hb_sparseset_t &other) { s.union_ (other.s); }
  فراغ intersect (ثابت hb_sparseset_t &other,
		  hb_وکتور_t<بیعلامت> *workspace = اشاره_تهی)
  { s.intersect (other.s, workspace); }
  فراغ تفریق (ثابت hb_sparseset_t &other) { s.تفریق (other.s); }
  فراغ متقارن_difference (ثابت hb_sparseset_t &other) { s.متقارن_difference (other.s); }

  منطقي next (hb_codepoint_t *codepoint) ثابت { أرجع s.next (codepoint); }
  منطقي بعدی_بیتها (hb_codepoint_t *codepoint, uint64_t *بیتها) ثابت
  { أرجع s.بعدی_بیتها (codepoint, بیتها); }
  منطقي قبلی (hb_codepoint_t *codepoint) ثابت { أرجع s.قبلی (codepoint); }
  منطقي بعدی_range (hb_codepoint_t *اولی, hb_codepoint_t *آخرین) ثابت
  { أرجع s.بعدی_range (اولی, آخرین); }
  منطقي قبلی_range (hb_codepoint_t *اولی, hb_codepoint_t *آخرین) ثابت
  { أرجع s.قبلی_range (اولی, آخرین); }
  بیعلامت صحيح بعدی_many (hb_codepoint_t codepoint, hb_codepoint_t *بیرون, بیعلامت صحيح اندازه) ثابت
  { أرجع s.بعدی_many (codepoint, بیرون, اندازه); }

  بیعلامت صحيح get_population () ثابت { أرجع s.get_population (); }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت { أرجع s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ثابت { أرجع s.get_min (); }
  hb_codepoint_t get_max () ثابت { أرجع s.get_max (); }

  ساكن constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  استفاده iter_t = نام_نوع impl_t::iter_t;
  iter_t iter () ثابت { أرجع iter_t (هذا->s); }
  عملگر iter_t () ثابت { أرجع iter (); }
};

بنية hb_مجموعه_t : hb_sparseset_t<hb_بیت_مجموعه_invertible_t>
{
  استفاده sparseset = hb_sparseset_t<hb_بیت_مجموعه_invertible_t>;

  ~hb_مجموعه_t () = افتراضي;
  hb_مجموعه_t () : sparseset () {};
  hb_مجموعه_t (ثابت hb_مجموعه_t &o) : sparseset ((sparseset &) o) {};
  hb_مجموعه_t (hb_مجموعه_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_مجموعه_t& عملگر = (ثابت hb_مجموعه_t&) = افتراضي;
  hb_مجموعه_t& عملگر = (hb_مجموعه_t&&) = افتراضي;
  hb_مجموعه_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  قالب <نام_نوع Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_مجموعه_t (ثابت Iterable &o) : sparseset (o) {}

  hb_مجموعه_t& عملگر << (hb_codepoint_t v)
  { sparseset::عملگر<< (v); أرجع *هذا; }
  hb_مجموعه_t& عملگر << (ثابت hb_codepoint_pair_t& range)
  { sparseset::عملگر<< (range); أرجع *هذا; }
};

static_assert (hb_مجموعه_t::INVALID == HB_مجموعه_مقدار_INVALID, "");


#پایان_اگر /* HB_SET_HH */
