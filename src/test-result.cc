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

#ଅନ୍ତର୍ଭାଵ "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା live_instances = 0;

ଗଣନା error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

template <typename T>
using result = hb_result_t<T, error_code_t>;

ସଂରଚନା resource_t
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;

  explicit resource_t (ପୂର୍ଣ୍ଣସଂଖ୍ୟା id_) : id (id_) { live_instances++; }
  resource_t (ଅଚଳ resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& operator = (ଅଚଳ resource_t& o)
  {
    id = o.id;
    ଫେରନ୍ତୁ *ଏହି;
  }
  resource_t& operator = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    ଫେରନ୍ତୁ *ଏହି;
  }

  ତାର୍କିକ operator == (ଅଚଳ resource_t& o) ଅଚଳ { ଫେରନ୍ତୁ id == o.id; }
};

ସ୍ଥିର result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> returns_ok (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v)
{
  ଫେରନ୍ତୁ Ok(v);
}

ସ୍ଥିର result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> returns_err (error_code_t e)
{
  ଫେରନ୍ତୁ Err(e);
}

ସ୍ଥିର result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> try_callee (ତାର୍କିକ fail, ପୂର୍ଣ୍ଣସଂଖ୍ୟା v, error_code_t e = ERR_A)
{
  ଯଦି (fail) ଫେରନ୍ତୁ Err(e);
  ଫେରନ୍ତୁ Ok(v);
}

ସ୍ଥିର result<ଶୂନ୍ୟ> try_callee_void (ତାର୍କିକ fail,  error_code_t e)
{
  ଯଦି (fail) ଫେରନ୍ତୁ e;
  ଫେରନ୍ତୁ Ok();
}

ସ୍ଥିର result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> try_caller (ତାର୍କିକ fail1, ତାର୍କିକ fail2)
{
  TRY_ASSIGN (ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (ପୂର୍ଣ୍ଣସଂଖ୍ୟା index2, try_callee (fail2, 20, ERR_B));
  ଫେରନ୍ତୁ Ok(index + index2);
}

ସ୍ଥିର result<ଶୂନ୍ୟ> try_caller_void (ତାର୍କିକ fail)
{
  TRY (try_callee_void (fail, ERR_C));
  ଫେରନ୍ତୁ Ok();
}

ସ୍ଥିର result<ଦଶମିକ> try_caller_type_change (ତାର୍କିକ fail)
{
  TRY_ASSIGN (ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, try_callee (fail, 100, ERR_A));
  ଫେରନ୍ତୁ Ok((ଦଶମିକ) index * 1.5f);
}

ସ୍ଥିର result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> try_assign_existing (ତାର୍କିକ fail)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା val = 1;
  TRY_ASSIGN (val, try_callee (fail, 42, ERR_A));
  ଫେରନ୍ତୁ Ok(val);
}

ସ୍ଥିର ଶୂନ୍ୟ test_ok_basic ()
{
  result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((ତାର୍କିକ) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.ମୂଲ୍ୟ_or (0) == 42);
  hb_always_assert (r.ମୂଲ୍ୟ_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

ସ୍ଥିର ଶୂନ୍ୟ test_err_basic ()
{
  result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.ମୂଲ୍ୟ_or (99) == 99);
  hb_always_assert (r.ମୂଲ୍ୟ_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

ସ୍ଥିର ଶୂନ୍ୟ test_explicit_ok_err ()
{
  result<unsigned> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  result<unsigned> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

ସ୍ଥିର ଶୂନ୍ୟ test_pointers ()
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 123;
  result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

ସ୍ଥିର ଶୂନ୍ୟ test_void ()
{
  result<ଶୂନ୍ୟ> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((ତାର୍କିକ) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  result<ଶୂନ୍ୟ> r2 = Err(ERR_A);
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

ସ୍ଥିର ଶୂନ୍ୟ test_resource_cleanup ()
{
  hb_always_assert (live_instances == 0);

  {
    result<resource_t> r = resource_t (1);
    hb_always_assert (live_instances == 1);
    hb_always_assert (r.value ().id == 1);
  }
  hb_always_assert (live_instances == 0);

  {
    result<resource_t> r1 = resource_t (2);
    hb_always_assert (live_instances == 1);
    result<resource_t> r2 = r1;
    hb_always_assert (live_instances == 2);
    result<resource_t> r3 = std::move (r1);
    hb_always_assert (live_instances == 3);
  }
  hb_always_assert (live_instances == 0);

  {
    result<resource_t> r = resource_t (3);
    hb_always_assert (live_instances == 1);
    r = Err(ERR_A);
    hb_always_assert (live_instances == 0);
    hb_always_assert (r.is_err ());
  }
  hb_always_assert (live_instances == 0);
}

ସ୍ଥିର ଶୂନ୍ୟ test_try_macro ()
{
  // Test success path
  result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> r_success = try_caller (ମିଥ୍ୟା, ମିଥ୍ୟା);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> r_fail1 = try_caller (ସତ୍ୟ, ମିଥ୍ୟା);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> r_fail2 = try_caller (ମିଥ୍ୟା, ସତ୍ୟ);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  result<ଶୂନ୍ୟ> v_ok = try_caller_void (ମିଥ୍ୟା);
  hb_always_assert (v_ok.is_ok ());

  result<ଶୂନ୍ୟ> v_err = try_caller_void (ସତ୍ୟ);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  result<ଦଶମିକ> f_ok = try_caller_type_change (ମିଥ୍ୟା);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  result<ଦଶମିକ> f_err = try_caller_type_change (ସତ୍ୟ);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> a_ok = try_assign_existing (ମିଥ୍ୟା);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> a_err = try_assign_existing (ସତ୍ୟ);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

template <typename T, typename U, typename = ଶୂନ୍ୟ>
ସଂରଚନା is_equality_comparable : std::false_type {};

template <typename T, typename U>
ସଂରଚନା is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

ସ୍ଥିର ଶୂନ୍ୟ test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::value, "");
  static_assert (!is_equality_comparable<result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, int64_t>::value, "");
  static_assert (!is_equality_comparable<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>::value, "");
  static_assert (!is_equality_comparable<result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>::value, "");
  static_assert (!is_equality_comparable<result<ଶୂନ୍ୟ>, error_code_t>::value, "");
  static_assert (is_equality_comparable<result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>::value, "");
  static_assert (is_equality_comparable<result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, hb_result_ok_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>::value, "");
  static_assert (is_equality_comparable<result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, hb_result_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_ok_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_err_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::value, "");
  static_assert (std::is_constructible<hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, hb_result_ok_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>::value, "");
  static_assert (std::is_constructible<hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, hb_result_err_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, error_code_t>, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>::value, "");
  static_assert (!std::is_constructible<hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_result_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_result_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

ସଂରଚନା any_ମୂଲ୍ୟ_t
{
  template <typename X>
  any_ମୂଲ୍ୟ_t (X&&) {}
};

ସ୍ଥିର ଶୂନ୍ୟ test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_result_t<any_ମୂଲ୍ୟ_t, error_code_t>, hb_result_err_t<ଅଚଳ ଅକ୍ଷର*>>::value, "");

  hb_result_t<any_ମୂଲ୍ୟ_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_result_t<any_ମୂଲ୍ୟ_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

ସଂରଚନା move_only_err_t
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା code;

  explicit move_only_err_t (ପୂର୍ଣ୍ଣସଂଖ୍ୟା c) : code (c) {}
  move_only_err_t (ଅଚଳ move_only_err_t&) = delete;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& operator = (ଅଚଳ move_only_err_t&) = delete;
  move_only_err_t& operator = (move_only_err_t&& o) { code = o.code; o.code = -1; ଫେରନ୍ତୁ *ଏହି; }
  ~move_only_err_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  ତାର୍କିକ operator == (ଅଚଳ move_only_err_t& o) ଅଚଳ { ଫେରନ୍ତୁ code == o.code; }
};

ସଂରଚନା move_only_val_t
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା val;

  explicit move_only_val_t (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v) : val (v) {}
  move_only_val_t (ଅଚଳ move_only_val_t&) = delete;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& operator = (ଅଚଳ move_only_val_t&) = delete;
  move_only_val_t& operator = (move_only_val_t&& o) { val = o.val; o.val = -1; ଫେରନ୍ତୁ *ଏହି; }
  ~move_only_val_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  ତାର୍କିକ operator == (ଅଚଳ move_only_val_t& o) ଅଚଳ { ଫେରନ୍ତୁ val == o.val; }
};

ସ୍ଥିର ଶୂନ୍ୟ test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_result_t<ଶୂନ୍ୟ, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_result_t<ଶୂନ୍ୟ, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_result_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).ମୂଲ୍ୟ_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_result_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).ମୂଲ୍ୟ_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

ସଂରଚନା copy_move_counter_t
{
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_ଗଣନା;
  ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା move_ଗଣନା;

  ସ୍ଥିର ଶୂନ୍ୟ reset ()
  {
    copy_ଗଣନା = 0;
    move_ଗଣନା = 0;
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା val;

  ତାର୍କିକ in_error() ଅଚଳ {
    ଫେରନ୍ତୁ val == -1;
  }

  explicit copy_move_counter_t (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v) : val (v) {}
  copy_move_counter_t (ଅଚଳ copy_move_counter_t& o) : val (o.val) { copy_ଗଣନା++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_ଗଣନା++; }
  ~copy_move_counter_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
};

ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_move_counter_t::copy_ଗଣନା = 0;
ପୂର୍ଣ୍ଣସଂଖ୍ୟା copy_move_counter_t::move_ଗଣନା = 0;

ସ୍ଥିର result<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t out (42);
  ଫେରନ୍ତୁ Ok (std::move (out));
}

ସ୍ଥିର result<copy_move_counter_t> return_tracked_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  ଫେରନ୍ତୁ Ok(out);
}

ସ୍ଥିର hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t out (42);
  ଫେରନ୍ତୁ Err (std::move (out));
}

ସ୍ଥିର hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, copy_move_counter_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  ଫେରନ୍ତୁ Err(out);
}

ସ୍ଥିର result<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  ଫେରନ୍ତୁ Ok (std::move (out));
}

ସ୍ଥିର ଶୂନ୍ୟ test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ଗଣନା == 0);
    hb_always_assert (copy_move_counter_t::move_ଗଣନା >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ଗଣନା == 0);
    hb_always_assert (copy_move_counter_t::move_ଗଣନା >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ଗଣନା == 0);
    hb_always_assert (copy_move_counter_t::move_ଗଣନା >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା, copy_move_counter_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_ଗଣନା == 0);
    hb_always_assert (copy_move_counter_t::move_ଗଣନା >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    result<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

ସଂରଚନା in_error_t {
  ତାର୍କିକ successful;

  in_error_t(ତାର୍କିକ s) : successful(s) {}

  ତାର୍କିକ in_error() ଅଚଳ {
    ଫେରନ୍ତୁ !successful;
  }
};

ସ୍ଥିର ଶୂନ୍ୟ test_from () {
  in_error_t has_error(ମିଥ୍ୟା);
  in_error_t no_error(ସତ୍ୟ);
  result<ଶୂନ୍ୟ> r = result<ଶୂନ୍ୟ>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = result<ଶୂନ୍ୟ>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

ସଂରଚନା default_constructible_t
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v) : val (v) {}
};

ସଂରଚନା move_only_default_t
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (ପୂର୍ଣ୍ଣସଂଖ୍ୟା v) : val (v) {}
  move_only_default_t (ଅଚଳ move_only_default_t&) = delete;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& operator = (ଅଚଳ move_only_default_t&) = delete;
  move_only_default_t& operator = (move_only_default_t&& o) { val = o.val; o.val = -1; ଫେରନ୍ତୁ *ଏହି; }
  ~move_only_default_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
};

ସ୍ଥିର ଶୂନ୍ୟ test_ମୂଲ୍ୟ_or_default ()
{
  // Test const &
  {
    ଅଚଳ result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> r_ok = Ok(42);
    hb_always_assert (r_ok.ମୂଲ୍ୟ_or_default () == 42);

    ଅଚଳ result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> r_err = Err(ERR_A);
    hb_always_assert (r_err.ମୂଲ୍ୟ_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).ମୂଲ୍ୟ_or_default () == 42);

    result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).ମୂଲ୍ୟ_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    result<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.ମୂଲ୍ୟ_or_default ().val == 456);

    result<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.ମୂଲ୍ୟ_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).ମୂଲ୍ୟ_or_default ().val == 456);
    hb_always_assert (std::move (r_err).ମୂଲ୍ୟ_or_default ().val == 123);
  }

  // Test pointers
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 10;
    result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା*> r_ok = &x;
    hb_always_assert (r_ok.ମୂଲ୍ୟ_or_default () == &x);

    result<ପୂର୍ଣ୍ଣସଂଖ୍ୟା*> r_err = ERR_A;
    hb_always_assert (r_err.ମୂଲ୍ୟ_or_default () == nullptr);
  }

  // Test move-only types
  {
    result<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).ମୂଲ୍ୟ_or_default ().val == 88);

    result<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).ମୂଲ୍ୟ_or_default ().val == 77);
  }
}

ସ୍ଥିର ଶୂନ୍ୟ test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    result<copy_move_counter_t> r_ok = result<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_ଗଣନା == 0);
    hb_always_assert (copy_move_counter_t::move_ଗଣନା == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    result<copy_move_counter_t> r_fail = result<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_ଗଣନା == 0);
    hb_always_assert (copy_move_counter_t::move_ଗଣନା == 0);
  }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ ()
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
  test_ମୂଲ୍ୟ_or_default ();
  test_from_move ();

  ଫେରନ୍ତୁ 0;
}
