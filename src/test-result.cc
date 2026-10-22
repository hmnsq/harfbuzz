/*
 * Copyright © 2025  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#অন্তর্ভাব "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
স্থির পূর্ণসংখ্যা live_instances = 0;

গণনা error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

template <typename T>
using ফলাফল = hb_ফলাফল_t<T, error_code_t>;

কাঠামো resource_t
{
  পূর্ণসংখ্যা id;

  explicit resource_t (পূর্ণসংখ্যা id_) : id (id_) { live_instances++; }
  resource_t (ধ্রুবক resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& operator = (ধ্রুবক resource_t& o)
  {
    id = o.id;
    ফেরত *এই;
  }
  resource_t& operator = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    ফেরত *এই;
  }

  যৌক্তিক operator == (ধ্রুবক resource_t& o) ধ্রুবক { ফেরত id == o.id; }
};

স্থির ফলাফল<পূর্ণসংখ্যা> returns_ok (পূর্ণসংখ্যা v)
{
  ফেরত Ok(v);
}

স্থির ফলাফল<পূর্ণসংখ্যা> returns_err (error_code_t e)
{
  ফেরত Err(e);
}

স্থির ফলাফল<পূর্ণসংখ্যা> try_callee (যৌক্তিক fail, পূর্ণসংখ্যা v, error_code_t e = ERR_A)
{
  যদি (fail) ফেরত Err(e);
  ফেরত Ok(v);
}

স্থির ফলাফল<শূন্য> try_callee_void (যৌক্তিক fail,  error_code_t e)
{
  যদি (fail) ফেরত e;
  ফেরত Ok();
}

স্থির ফলাফল<পূর্ণসংখ্যা> try_caller (যৌক্তিক fail1, যৌক্তিক fail2)
{
  TRY_ASSIGN (পূর্ণসংখ্যা সূচক_2, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (পূর্ণসংখ্যা index2, try_callee (fail2, 20, ERR_B));
  ফেরত Ok(সূচক_2 + index2);
}

স্থির ফলাফল<শূন্য> try_caller_void (যৌক্তিক fail)
{
  TRY (try_callee_void (fail, ERR_C));
  ফেরত Ok();
}

স্থির ফলাফল<দশমিক> try_caller_type_change (যৌক্তিক fail)
{
  TRY_ASSIGN (পূর্ণসংখ্যা সূচক_2, try_callee (fail, 100, ERR_A));
  ফেরত Ok((দশমিক) সূচক_2 * 1.5f);
}

স্থির ফলাফল<পূর্ণসংখ্যা> try_assign_existing (যৌক্তিক fail)
{
  পূর্ণসংখ্যা val = 1;
  TRY_ASSIGN (val, try_callee (fail, 42, ERR_A));
  ফেরত Ok(val);
}

স্থির শূন্য test_ok_basic ()
{
  ফলাফল<পূর্ণসংখ্যা> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((যৌক্তিক) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.মান_or (0) == 42);
  hb_always_assert (r.মান_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

স্থির শূন্য test_err_basic ()
{
  ফলাফল<পূর্ণসংখ্যা> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.মান_or (99) == 99);
  hb_always_assert (r.মান_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

স্থির শূন্য test_explicit_ok_err ()
{
  ফলাফল<unsigned> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  ফলাফল<unsigned> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

স্থির শূন্য test_pointers ()
{
  পূর্ণসংখ্যা x = 123;
  ফলাফল<পূর্ণসংখ্যা*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

স্থির শূন্য test_void ()
{
  ফলাফল<শূন্য> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((যৌক্তিক) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  ফলাফল<শূন্য> r2 = Err(ERR_A);
  hb_always_assert (!r2.is_ok ());
  hb_always_assert (r2.is_err ());
  hb_always_assert (!r2);
  hb_always_assert (r2.error () == ERR_A);
  hb_always_assert (r2 == Err(ERR_A));
  hb_always_assert (r2 != Err(ERR_B));
  hb_always_assert (r2 != Ok());
  hb_always_assert (Err(ERR_A) == r2);
  hb_always_assert (Err(ERR_B) != r2);
  hb_always_assert (Ok() != r2);
}

স্থির শূন্য test_resource_cleanup ()
{
  hb_always_assert (live_instances == 0);

  {
    ফলাফল<resource_t> r = resource_t (1);
    hb_always_assert (live_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (live_instances == 0);

  {
    ফলাফল<resource_t> r1 = resource_t (2);
    hb_always_assert (live_instances == 1);
    ফলাফল<resource_t> r2 = r1;
    hb_always_assert (live_instances == 2);
    ফলাফল<resource_t> r3 = std::move (r1);
    hb_always_assert (live_instances == 3);
  }
  hb_always_assert (live_instances == 0);

  {
    ফলাফল<resource_t> r = resource_t (3);
    hb_always_assert (live_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (live_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (live_instances == 0);
}

স্থির শূন্য test_try_macro ()
{
  // Test success path
  ফলাফল<পূর্ণসংখ্যা> r_success = try_caller (মিথ্যা, মিথ্যা);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  ফলাফল<পূর্ণসংখ্যা> r_fail1 = try_caller (সত্য, মিথ্যা);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  ফলাফল<পূর্ণসংখ্যা> r_fail2 = try_caller (মিথ্যা, সত্য);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  ফলাফল<শূন্য> v_ok = try_caller_void (মিথ্যা);
  hb_always_assert (v_ok.is_ok ());

  ফলাফল<শূন্য> v_err = try_caller_void (সত্য);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  ফলাফল<দশমিক> f_ok = try_caller_type_change (মিথ্যা);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  ফলাফল<দশমিক> f_err = try_caller_type_change (সত্য);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  ফলাফল<পূর্ণসংখ্যা> a_ok = try_assign_existing (মিথ্যা);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  ফলাফল<পূর্ণসংখ্যা> a_err = try_assign_existing (সত্য);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

template <typename T, typename U, typename = শূন্য>
কাঠামো is_equality_comparable : std::false_type {};

template <typename T, typename U>
কাঠামো is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

স্থির শূন্য test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<ফলাফল<পূর্ণসংখ্যা>, পূর্ণসংখ্যা>::value, "");
  static_assert (!is_equality_comparable<ফলাফল<পূর্ণসংখ্যা>, int64_t>::value, "");
  static_assert (!is_equality_comparable<পূর্ণসংখ্যা, ফলাফল<পূর্ণসংখ্যা>>::value, "");
  static_assert (!is_equality_comparable<ফলাফল<পূর্ণসংখ্যা>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, ফলাফল<পূর্ণসংখ্যা>>::value, "");
  static_assert (!is_equality_comparable<ফলাফল<শূন্য>, error_code_t>::value, "");
  static_assert (is_equality_comparable<ফলাফল<পূর্ণসংখ্যা>, ফলাফল<পূর্ণসংখ্যা>>::value, "");
  static_assert (is_equality_comparable<ফলাফল<পূর্ণসংখ্যা>, hb_ফলাফল_ok_t<পূর্ণসংখ্যা>>::value, "");
  static_assert (is_equality_comparable<ফলাফল<পূর্ণসংখ্যা>, hb_ফলাফল_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_ফলাফল_t<int64_t, int32_t>, পূর্ণসংখ্যা>::value, "");
  static_assert (!std::is_constructible<hb_ফলাফল_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_ফলাফল_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_ফলাফল_t<int64_t, int32_t>, hb_ফলাফল_ok_t<পূর্ণসংখ্যা>>::value, "");
  static_assert (std::is_constructible<hb_ফলাফল_t<int64_t, int32_t>, hb_ফলাফল_err_t<পূর্ণসংখ্যা>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_ফলাফল_t<পূর্ণসংখ্যা, পূর্ণসংখ্যা>, পূর্ণসংখ্যা>::value, "");
  static_assert (std::is_constructible<hb_ফলাফল_t<পূর্ণসংখ্যা, পূর্ণসংখ্যা>, hb_ফলাফল_ok_t<পূর্ণসংখ্যা>>::value, "");
  static_assert (std::is_constructible<hb_ফলাফল_t<পূর্ণসংখ্যা, পূর্ণসংখ্যা>, hb_ফলাফল_err_t<পূর্ণসংখ্যা>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_ফলাফল_t<পূর্ণসংখ্যা, error_code_t>, পূর্ণসংখ্যা>::value, "");
  static_assert (!std::is_constructible<hb_ফলাফল_t<পূর্ণসংখ্যা, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_ফলাফল_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_ফলাফল_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_ফলাফল_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_ফলাফল_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_ফলাফল_t<পূর্ণসংখ্যা, পূর্ণসংখ্যা> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_ফলাফল_t<পূর্ণসংখ্যা, পূর্ণসংখ্যা> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

কাঠামো any_মান_t
{
  template <typename X>
  any_মান_t (X&&) {}
};

স্থির শূন্য test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_ফলাফল_t<any_মান_t, error_code_t>, hb_ফলাফল_err_t<ধ্রুবক অক্ষর*>>::value, "");

  hb_ফলাফল_t<any_মান_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_ফলাফল_t<any_মান_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

কাঠামো move_only_err_t
{
  পূর্ণসংখ্যা code;

  explicit move_only_err_t (পূর্ণসংখ্যা c) : code (c) {}
  move_only_err_t (ধ্রুবক move_only_err_t&) = delete;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& operator = (ধ্রুবক move_only_err_t&) = delete;
  move_only_err_t& operator = (move_only_err_t&& o) { code = o.code; o.code = -1; ফেরত *এই; }
  ~move_only_err_t () = পূর্বনির্ধারিত;

  যৌক্তিক operator == (ধ্রুবক move_only_err_t& o) ধ্রুবক { ফেরত code == o.code; }
};

কাঠামো move_only_val_t
{
  পূর্ণসংখ্যা val;

  explicit move_only_val_t (পূর্ণসংখ্যা v) : val (v) {}
  move_only_val_t (ধ্রুবক move_only_val_t&) = delete;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& operator = (ধ্রুবক move_only_val_t&) = delete;
  move_only_val_t& operator = (move_only_val_t&& o) { val = o.val; o.val = -1; ফেরত *এই; }
  ~move_only_val_t () = পূর্বনির্ধারিত;

  যৌক্তিক operator == (ধ্রুবক move_only_val_t& o) ধ্রুবক { ফেরত val == o.val; }
};

স্থির শূন্য test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_ফলাফল_t<পূর্ণসংখ্যা, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_ফলাফল_t<পূর্ণসংখ্যা, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_ফলাফল_t<শূন্য, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_ফলাফল_t<শূন্য, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_ফলাফল_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).মান_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_ফলাফল_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).মান_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

কাঠামো copy_move_গণক_t
{
  স্থির পূর্ণসংখ্যা copy_গুনতি;
  স্থির পূর্ণসংখ্যা move_গুনতি;

  স্থির শূন্য reset ()
  {
    copy_গুনতি = 0;
    move_গুনতি = 0;
  }

  পূর্ণসংখ্যা val;

  যৌক্তিক in_error() ধ্রুবক {
    ফেরত val == -1;
  }

  explicit copy_move_গণক_t (পূর্ণসংখ্যা v) : val (v) {}
  copy_move_গণক_t (ধ্রুবক copy_move_গণক_t& o) : val (o.val) { copy_গুনতি++; }
  copy_move_গণক_t (copy_move_গণক_t&& o) : val (o.val) { o.val = -1; move_গুনতি++; }
  ~copy_move_গণক_t () = পূর্বনির্ধারিত;
};

পূর্ণসংখ্যা copy_move_গণক_t::copy_গুনতি = 0;
পূর্ণসংখ্যা copy_move_গণক_t::move_গুনতি = 0;

স্থির ফলাফল<copy_move_গণক_t> return_tracked_from_local ()
{
  copy_move_গণক_t out (42);
  ফেরত Ok (std::move (out));
}

স্থির ফলাফল<copy_move_গণক_t> return_tracked_from_local_with_tag ()
{
  copy_move_গণক_t out (42);
  ফেরত Ok(out);
}

স্থির hb_ফলাফল_t<পূর্ণসংখ্যা, copy_move_গণক_t> return_tracked_error_from_local ()
{
  copy_move_গণক_t out (42);
  ফেরত Err (std::move (out));
}

স্থির hb_ফলাফল_t<পূর্ণসংখ্যা, copy_move_গণক_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_গণক_t out (42);
  ফেরত Err(out);
}

স্থির ফলাফল<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  ফেরত Ok (std::move (out));
}

স্থির শূন্য test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_গণক_t::reset ();
    ফলাফল<copy_move_গণক_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_গণক_t::copy_গুনতি == 0);
    hb_always_assert (copy_move_গণক_t::move_গুনতি >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_গণক_t::reset ();
    ফলাফল<copy_move_গণক_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_গণক_t::copy_গুনতি == 0);
    hb_always_assert (copy_move_গণক_t::move_গুনতি >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_গণক_t::reset ();
    hb_ফলাফল_t<পূর্ণসংখ্যা, copy_move_গণক_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_গণক_t::copy_গুনতি == 0);
    hb_always_assert (copy_move_গণক_t::move_গুনতি >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_গণক_t::reset ();
    hb_ফলাফল_t<পূর্ণসংখ্যা, copy_move_গণক_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_গণক_t::copy_গুনতি == 0);
    hb_always_assert (copy_move_গণক_t::move_গুনতি >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    ফলাফল<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

কাঠামো in_error_t {
  যৌক্তিক successful;

  in_error_t(যৌক্তিক s) : successful(s) {}

  যৌক্তিক in_error() ধ্রুবক {
    ফেরত !successful;
  }
};

স্থির শূন্য test_from () {
  in_error_t has_error(মিথ্যা);
  in_error_t no_error(সত্য);
  ফলাফল<শূন্য> r = ফলাফল<শূন্য>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = ফলাফল<শূন্য>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

কাঠামো default_constructible_t
{
  পূর্ণসংখ্যা val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (পূর্ণসংখ্যা v) : val (v) {}
};

কাঠামো move_only_default_t
{
  পূর্ণসংখ্যা val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (পূর্ণসংখ্যা v) : val (v) {}
  move_only_default_t (ধ্রুবক move_only_default_t&) = delete;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& operator = (ধ্রুবক move_only_default_t&) = delete;
  move_only_default_t& operator = (move_only_default_t&& o) { val = o.val; o.val = -1; ফেরত *এই; }
  ~move_only_default_t () = পূর্বনির্ধারিত;
};

স্থির শূন্য test_মান_or_default ()
{
  // Test const &
  {
    ধ্রুবক ফলাফল<পূর্ণসংখ্যা> r_ok = Ok(42);
    hb_always_assert (r_ok.মান_or_default () == 42);

    ধ্রুবক ফলাফল<পূর্ণসংখ্যা> r_err = Err(ERR_A);
    hb_always_assert (r_err.মান_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    ফলাফল<পূর্ণসংখ্যা> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).মান_or_default () == 42);

    ফলাফল<পূর্ণসংখ্যা> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).মান_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    ফলাফল<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.মান_or_default ().val == 456);

    ফলাফল<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.মান_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).মান_or_default ().val == 456);
    hb_always_assert (std::move (r_err).মান_or_default ().val == 123);
  }

  // Test pointers
  {
    পূর্ণসংখ্যা x = 10;
    ফলাফল<পূর্ণসংখ্যা*> r_ok = &x;
    hb_always_assert (r_ok.মান_or_default () == &x);

    ফলাফল<পূর্ণসংখ্যা*> r_err = ERR_A;
    hb_always_assert (r_err.মান_or_default () == nullptr);
  }

  // Test move-only types
  {
    ফলাফল<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).মান_or_default ().val == 88);

    ফলাফল<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).মান_or_default ().val == 77);
  }
}

স্থির শূন্য test_from_move()
{
  {
    copy_move_গণক_t::reset();
    copy_move_গণক_t ok (24);
    ফলাফল<copy_move_গণক_t> r_ok = ফলাফল<copy_move_গণক_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_গণক_t::copy_গুনতি == 0);
    hb_always_assert (copy_move_গণক_t::move_গুনতি == 1);
  }

  {
    copy_move_গণক_t::reset();
    copy_move_গণক_t fail (-1);
    ফলাফল<copy_move_গণক_t> r_fail = ফলাফল<copy_move_গণক_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_গণক_t::copy_গুনতি == 0);
    hb_always_assert (copy_move_গণক_t::move_গুনতি == 0);
  }
}

পূর্ণসংখ্যা প্রধান ()
{
  test_ok_basic ();
  test_err_basic ();
  test_explicit_ok_err ();
  test_pointers ();
  test_void ();
  test_resource_cleanup ();
  test_try_macro ();
  test_same_or_convertible_types ();
  test_error_interception ();
  test_move_only_types ();
  test_return_moves_from_local ();
  test_from();
  test_মান_or_default ();
  test_from_move ();

  ফেরত 0;
}
