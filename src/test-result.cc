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

#अन्तर्भाव "hb-result.hh"


// Track constructor and destructor counts to verify leak-freedom
स्थिर पूर्णांक live_instances = 0;

गणना error_code_t {
  ERR_A,
  ERR_B,
  ERR_C,
};

ढाँचा <typename T>
using result = hb_result_t<T, error_code_t>;

संरचना resource_t
{
  पूर्णांक id;

  explicit resource_t (पूर्णांक id_) : id (id_) { live_instances++; }
  resource_t (स्थिरांक resource_t& o) : id (o.id) { live_instances++; }
  resource_t (resource_t&& o) : id (o.id) { o.id = -1; live_instances++; }
  ~resource_t () { live_instances--; }

  resource_t& चालक = (स्थिरांक resource_t& o)
  {
    id = o.id;
    परत *हे;
  }
  resource_t& चालक = (resource_t&& o)
  {
    id = o.id;
    o.id = -1;
    परत *हे;
  }

  तार्किक चालक == (स्थिरांक resource_t& o) स्थिरांक { परत id == o.id; }
};

स्थिर result<पूर्णांक> returns_ok (पूर्णांक v)
{
  परत Ok(v);
}

स्थिर result<पूर्णांक> returns_err (error_code_t e)
{
  परत Err(e);
}

स्थिर result<पूर्णांक> try_callee (तार्किक fail, पूर्णांक v, error_code_t e = ERR_A)
{
  जर (fail) परत Err(e);
  परत Ok(v);
}

स्थिर result<शून्य> try_callee_void (तार्किक fail,  error_code_t e)
{
  जर (fail) परत e;
  परत Ok();
}

स्थिर result<पूर्णांक> try_caller (तार्किक fail1, तार्किक fail2)
{
  TRY_ASSIGN (पूर्णांक index, try_callee (fail1, 10, ERR_A));
  TRY_ASSIGN (पूर्णांक index2, try_callee (fail2, 20, ERR_B));
  परत Ok(index + index2);
}

स्थिर result<शून्य> try_caller_void (तार्किक fail)
{
  TRY (try_callee_void (fail, ERR_C));
  परत Ok();
}

स्थिर result<दशांश> try_caller_type_change (तार्किक fail)
{
  TRY_ASSIGN (पूर्णांक index, try_callee (fail, 100, ERR_A));
  परत Ok((दशांश) index * 1.5f);
}

स्थिर result<पूर्णांक> try_assign_existing (तार्किक fail)
{
  पूर्णांक val = 1;
  TRY_ASSIGN (val, try_callee (fail, 42, ERR_A));
  परत Ok(val);
}

स्थिर शून्य test_ok_basic ()
{
  result<पूर्णांक> r = returns_ok (42);
  hb_always_assert (r.is_ok ());
  hb_always_assert (!r.is_err ());
  hb_always_assert ((तार्किक) r);
  hb_always_assert (r.value () == 42);
  hb_always_assert (*r == 42);
  hb_always_assert (r.मूल्य_or (0) == 42);
  hb_always_assert (r.मूल्य_or_default () == 42);
  hb_always_assert (r == Ok(42));
  hb_always_assert (Ok(42) == r);
}

स्थिर शून्य test_err_basic ()
{
  result<पूर्णांक> r = returns_err (ERR_A);
  hb_always_assert (!r.is_ok ());
  hb_always_assert (r.is_err ());
  hb_always_assert (!r);
  hb_always_assert (r.error () == ERR_A);
  hb_always_assert (r.मूल्य_or (99) == 99);
  hb_always_assert (r.मूल्य_or_default () == 0);
  hb_always_assert (r == Err(ERR_A));
  hb_always_assert (Err(ERR_A) == r);
  hb_always_assert (r != Err(ERR_B));
  hb_always_assert (Err(ERR_B) != r);
}

स्थिर शून्य test_explicit_ok_err ()
{
  result<अचिन्हित> r1 = Ok(100u);
  hb_always_assert (r1.is_ok ());
  hb_always_assert (r1.value () == 100u);

  result<अचिन्हित> r2 = Err(ERR_A);
  hb_always_assert (r2.is_err ());
  hb_always_assert (r2.error () == ERR_A);
}

स्थिर शून्य test_pointers ()
{
  पूर्णांक x = 123;
  result<पूर्णांक*> r = &x;
  hb_always_assert (r.is_ok ());
  hb_always_assert (*r == &x);
  hb_always_assert (**r == 123);
  **r = 456;
  hb_always_assert (x == 456);
}

स्थिर शून्य test_void ()
{
  result<शून्य> r1 = Ok();
  hb_always_assert (r1.is_ok ());
  hb_always_assert (!r1.is_err ());
  hb_always_assert ((तार्किक) r1);
  hb_always_assert (r1 == Ok());
  hb_always_assert (r1 != Err(ERR_A));
  hb_always_assert (Ok() == r1);
  hb_always_assert (Err(ERR_A) != r1);

  result<शून्य> r2 = Err(ERR_A);
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

स्थिर शून्य test_resource_cleanup ()
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

स्थिर शून्य test_try_macro ()
{
  // Test success path
  result<पूर्णांक> r_success = try_caller (असत्य, असत्य);
  hb_always_assert (r_success.is_ok ());
  hb_always_assert (r_success.value () == 30);

  // Test first failure propagates
  result<पूर्णांक> r_fail1 = try_caller (सत्य, असत्य);
  hb_always_assert (r_fail1.is_err ());
  hb_always_assert (r_fail1.error () == ERR_A);

  // Test second failure propagates
  result<पूर्णांक> r_fail2 = try_caller (असत्य, सत्य);
  hb_always_assert (r_fail2.is_err ());
  hb_always_assert (r_fail2.error () == ERR_B);

  // Test TRY in void function
  result<शून्य> v_ok = try_caller_void (असत्य);
  hb_always_assert (v_ok.is_ok ());

  result<शून्य> v_err = try_caller_void (सत्य);
  hb_always_assert (v_err.is_err ());
  hb_always_assert (v_err.error () == ERR_C);

  // Test TRY with different return type
  result<दशांश> f_ok = try_caller_type_change (असत्य);
  hb_always_assert (f_ok.is_ok ());
  hb_always_assert (f_ok.value () == 150.0f);

  result<दशांश> f_err = try_caller_type_change (सत्य);
  hb_always_assert (f_err.is_err ());
  hb_always_assert (f_err.error () == ERR_A);

  // Test TRY_ASSIGN into existing variable
  result<पूर्णांक> a_ok = try_assign_existing (असत्य);
  hb_always_assert (a_ok.is_ok ());
  hb_always_assert (a_ok.value () == 42);

  result<पूर्णांक> a_err = try_assign_existing (सत्य);
  hb_always_assert (a_err.is_err ());
  hb_always_assert (a_err.error () == ERR_A);
}

ढाँचा <typename T, typename U, typename = शून्य>
संरचना is_equality_comparable : std::false_type {};

ढाँचा <typename T, typename U>
संरचना is_equality_comparable<T, U, hb_void_t<decltype (hb_declval (T) == hb_declval (U))>> : std::true_type {};

स्थिर शून्य test_same_or_convertible_types ()
{
  // Comparisons between result and raw types are disallowed
  static_assert (!is_equality_comparable<result<पूर्णांक>, पूर्णांक>::value, "");
  static_assert (!is_equality_comparable<result<पूर्णांक>, int64_t>::value, "");
  static_assert (!is_equality_comparable<पूर्णांक, result<पूर्णांक>>::value, "");
  static_assert (!is_equality_comparable<result<पूर्णांक>, error_code_t>::value, "");
  static_assert (!is_equality_comparable<error_code_t, result<पूर्णांक>>::value, "");
  static_assert (!is_equality_comparable<result<शून्य>, error_code_t>::value, "");
  static_assert (is_equality_comparable<result<पूर्णांक>, result<पूर्णांक>>::value, "");
  static_assert (is_equality_comparable<result<पूर्णांक>, hb_result_ok_t<पूर्णांक>>::value, "");
  static_assert (is_equality_comparable<result<पूर्णांक>, hb_result_err_t<error_code_t>>::value, "");

  // Types convertible into each other cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, पूर्णांक>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int64_t>::value, "");
  static_assert (!std::is_constructible<hb_result_t<int64_t, int32_t>, int32_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_ok_t<पूर्णांक>>::value, "");
  static_assert (std::is_constructible<hb_result_t<int64_t, int32_t>, hb_result_err_t<पूर्णांक>>::value, "");

  // Same types cannot be directly constructed without Ok/Err
  static_assert (!std::is_constructible<hb_result_t<पूर्णांक, पूर्णांक>, पूर्णांक>::value, "");
  static_assert (std::is_constructible<hb_result_t<पूर्णांक, पूर्णांक>, hb_result_ok_t<पूर्णांक>>::value, "");
  static_assert (std::is_constructible<hb_result_t<पूर्णांक, पूर्णांक>, hb_result_err_t<पूर्णांक>>::value, "");

  // Types convertible into each other cannot be directly constructed
  static_assert (!std::is_constructible<hb_result_t<पूर्णांक, error_code_t>, पूर्णांक>::value, "");
  static_assert (!std::is_constructible<hb_result_t<पूर्णांक, error_code_t>, error_code_t>::value, "");

  // Types not convertible into each other can be directly constructed
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, resource_t>::value, "");
  static_assert (std::is_constructible<hb_result_t<resource_t, error_code_t>, error_code_t>::value, "");

  hb_result_t<int64_t, int32_t> r_ok = Ok(1);
  hb_always_assert (r_ok.is_ok ());
  hb_always_assert (r_ok.value () == 1);

  hb_result_t<int64_t, int32_t> r_err = Err(2);
  hb_always_assert (r_err.is_err ());
  hb_always_assert (r_err.error () == 2);

  hb_result_t<पूर्णांक, पूर्णांक> same_ok = Ok(42);
  hb_always_assert (same_ok.is_ok ());
  hb_always_assert (same_ok.value () == 42);

  hb_result_t<पूर्णांक, पूर्णांक> same_err = Err(99);
  hb_always_assert (same_err.is_err ());
  hb_always_assert (same_err.error () == 99);
}

संरचना any_मूल्य_t
{
  ढाँचा <typename X>
  any_मूल्य_t (X&&) {}
};

स्थिर शून्य test_error_interception() {
  // Checks that U&& constructor does not intercept mismatched error tags as a success value.
  static_assert (!std::is_constructible<hb_result_t<any_मूल्य_t, error_code_t>, hb_result_err_t<स्थिरांक अक्षर*>>::value, "");

  hb_result_t<any_मूल्य_t, error_code_t> r = Err(ERR_A);
  hb_always_assert (r.is_err ());

  hb_result_t<any_मूल्य_t, error_code_t> r_ok = Ok(123);
  hb_always_assert (r_ok.is_ok ());
}

संरचना move_only_err_t
{
  पूर्णांक code;

  explicit move_only_err_t (पूर्णांक c) : code (c) {}
  move_only_err_t (स्थिरांक move_only_err_t&) = delete;
  move_only_err_t (move_only_err_t&& o) : code (o.code) { o.code = -1; }
  move_only_err_t& चालक = (स्थिरांक move_only_err_t&) = delete;
  move_only_err_t& चालक = (move_only_err_t&& o) { code = o.code; o.code = -1; परत *हे; }
  ~move_only_err_t () = पूर्वनिर्धारित;

  तार्किक चालक == (स्थिरांक move_only_err_t& o) स्थिरांक { परत code == o.code; }
};

संरचना move_only_val_t
{
  पूर्णांक val;

  explicit move_only_val_t (पूर्णांक v) : val (v) {}
  move_only_val_t (स्थिरांक move_only_val_t&) = delete;
  move_only_val_t (move_only_val_t&& o) : val (o.val) { o.val = -1; }
  move_only_val_t& चालक = (स्थिरांक move_only_val_t&) = delete;
  move_only_val_t& चालक = (move_only_val_t&& o) { val = o.val; o.val = -1; परत *हे; }
  ~move_only_val_t () = पूर्वनिर्धारित;

  तार्किक चालक == (स्थिरांक move_only_val_t& o) स्थिरांक { परत val == o.val; }
};

स्थिर शून्य test_move_only_types ()
{
  // Test hb_result_t<T, move_only_err_t> with Err(...)
  {
    hb_result_t<पूर्णांक, move_only_err_t> r = Err(move_only_err_t (123));
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 123);

    move_only_err_t err = std::move (r).error ();
    hb_always_assert (err.code == 123);
  }

  // Test direct construction from move_only_err_t
  {
    hb_result_t<पूर्णांक, move_only_err_t> r = move_only_err_t (456);
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error ().code == 456);
  }

  // Test hb_result_t<void, move_only_err_t> with Err(...) and direct construction
  {
    hb_result_t<शून्य, move_only_err_t> v1 = Err(move_only_err_t (789));
    hb_always_assert (v1.is_err ());
    hb_always_assert (v1.error ().code == 789);

    hb_result_t<शून्य, move_only_err_t> v2 = move_only_err_t (999);
    hb_always_assert (v2.is_err ());
    hb_always_assert (v2.error ().code == 999);
  }

  // Test value_or with move-only value types
  {
    hb_result_t<move_only_val_t, error_code_t> r_ok = Ok(move_only_val_t (42));
    move_only_val_t v = std::move (r_ok).मूल्य_or (move_only_val_t (0));
    hb_always_assert (v.val == 42);

    hb_result_t<move_only_val_t, error_code_t> r_err = Err(ERR_A);
    move_only_val_t v_fallback = std::move (r_err).मूल्य_or (move_only_val_t (99));
    hb_always_assert (v_fallback.val == 99);
  }
}

संरचना copy_move_counter_t
{
  स्थिर पूर्णांक copy_मोजणी;
  स्थिर पूर्णांक move_मोजणी;

  स्थिर शून्य reset ()
  {
    copy_मोजणी = 0;
    move_मोजणी = 0;
  }

  पूर्णांक val;

  तार्किक in_error() स्थिरांक {
    परत val == -1;
  }

  explicit copy_move_counter_t (पूर्णांक v) : val (v) {}
  copy_move_counter_t (स्थिरांक copy_move_counter_t& o) : val (o.val) { copy_मोजणी++; }
  copy_move_counter_t (copy_move_counter_t&& o) : val (o.val) { o.val = -1; move_मोजणी++; }
  ~copy_move_counter_t () = पूर्वनिर्धारित;
};

पूर्णांक copy_move_counter_t::copy_मोजणी = 0;
पूर्णांक copy_move_counter_t::move_मोजणी = 0;

स्थिर result<copy_move_counter_t> return_tracked_from_local ()
{
  copy_move_counter_t out (42);
  परत Ok (std::move (out));
}

स्थिर result<copy_move_counter_t> return_tracked_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  परत Ok(out);
}

स्थिर hb_result_t<पूर्णांक, copy_move_counter_t> return_tracked_error_from_local ()
{
  copy_move_counter_t out (42);
  परत Err (std::move (out));
}

स्थिर hb_result_t<पूर्णांक, copy_move_counter_t> return_tracked_error_from_local_with_tag ()
{
  copy_move_counter_t out (42);
  परत Err(out);
}

स्थिर result<move_only_val_t> return_move_only_from_local ()
{
  move_only_val_t out (42);
  परत Ok (std::move (out));
}

स्थिर शून्य test_return_moves_from_local ()
{
  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_मोजणी == 0);
    hb_always_assert (copy_move_counter_t::move_मोजणी >= 1);
  }

  // Verify 0 copies and only moves occur when returning local value directly
  {
    copy_move_counter_t::reset ();
    result<copy_move_counter_t> r = return_tracked_from_local_with_tag ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
    hb_always_assert (copy_move_counter_t::copy_मोजणी == 0);
    hb_always_assert (copy_move_counter_t::move_मोजणी >= 1);
  }


  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<पूर्णांक, copy_move_counter_t> r = return_tracked_error_from_local ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_मोजणी == 0);
    hb_always_assert (copy_move_counter_t::move_मोजणी >= 1);
  }

  // Verify 0 copies and only moves occur when returning local error directly
  {
    copy_move_counter_t::reset ();
    hb_result_t<पूर्णांक, copy_move_counter_t> r = return_tracked_error_from_local_with_tag ();
    hb_always_assert (r.is_err ());
    hb_always_assert (r.error().val == 42);
    hb_always_assert (copy_move_counter_t::copy_मोजणी == 0);
    hb_always_assert (copy_move_counter_t::move_मोजणी >= 1);
  }


  // Test returning move-only type (statically proves no copy is attempted)
  {
    result<move_only_val_t> r = return_move_only_from_local ();
    hb_always_assert (r.is_ok ());
    hb_always_assert (r.value ().val == 42);
  }
}

संरचना in_error_t {
  तार्किक successful;

  in_error_t(तार्किक s) : successful(s) {}

  तार्किक in_error() स्थिरांक {
    परत !successful;
  }
};

स्थिर शून्य test_from () {
  in_error_t has_error(असत्य);
  in_error_t no_error(सत्य);
  result<शून्य> r = result<शून्य>::from(has_error, ERR_B);
  hb_always_assert(r.is_err());
  hb_always_assert(r.error() == ERR_B);
  r = result<शून्य>::from(no_error, ERR_B);
  hb_always_assert(!r.is_err());
}

संरचना default_constructible_t
{
  पूर्णांक val;

  default_constructible_t () : val (123) {}
  explicit default_constructible_t (पूर्णांक v) : val (v) {}
};

संरचना move_only_default_t
{
  पूर्णांक val;
  move_only_default_t () : val (77) {}
  explicit move_only_default_t (पूर्णांक v) : val (v) {}
  move_only_default_t (स्थिरांक move_only_default_t&) = delete;
  move_only_default_t (move_only_default_t&& o) : val (o.val) { o.val = -1; }
  move_only_default_t& चालक = (स्थिरांक move_only_default_t&) = delete;
  move_only_default_t& चालक = (move_only_default_t&& o) { val = o.val; o.val = -1; परत *हे; }
  ~move_only_default_t () = पूर्वनिर्धारित;
};

स्थिर शून्य test_मूल्य_or_default ()
{
  // Test const &
  {
    स्थिरांक result<पूर्णांक> r_ok = Ok(42);
    hb_always_assert (r_ok.मूल्य_or_default () == 42);

    स्थिरांक result<पूर्णांक> r_err = Err(ERR_A);
    hb_always_assert (r_err.मूल्य_or_default () == 0);
  }

  // Test && (rvalues and move)
  {
    result<पूर्णांक> r_ok = Ok(42);
    hb_always_assert (std::move (r_ok).मूल्य_or_default () == 42);

    result<पूर्णांक> r_err = Err(ERR_A);
    hb_always_assert (std::move (r_err).मूल्य_or_default () == 0);
  }

  // Test custom struct with default constructor
  {
    result<default_constructible_t> r_ok = default_constructible_t (456);
    hb_always_assert (r_ok.मूल्य_or_default ().val == 456);

    result<default_constructible_t> r_err = ERR_A;
    hb_always_assert (r_err.मूल्य_or_default ().val == 123);

    hb_always_assert (std::move (r_ok).मूल्य_or_default ().val == 456);
    hb_always_assert (std::move (r_err).मूल्य_or_default ().val == 123);
  }

  // Test pointers
  {
    पूर्णांक x = 10;
    result<पूर्णांक*> r_ok = &x;
    hb_always_assert (r_ok.मूल्य_or_default () == &x);

    result<पूर्णांक*> r_err = ERR_A;
    hb_always_assert (r_err.मूल्य_or_default () == nullptr);
  }

  // Test move-only types
  {
    result<move_only_default_t> r_mo_ok = move_only_default_t (88);
    hb_always_assert (std::move (r_mo_ok).मूल्य_or_default ().val == 88);

    result<move_only_default_t> r_mo_err = ERR_A;
    hb_always_assert (std::move (r_mo_err).मूल्य_or_default ().val == 77);
  }
}

स्थिर शून्य test_from_move()
{
  {
    copy_move_counter_t::reset();
    copy_move_counter_t ok (24);
    result<copy_move_counter_t> r_ok = result<copy_move_counter_t>::from(std::move(ok), ERR_B);
    hb_always_assert (r_ok.is_ok());
    hb_always_assert (r_ok->val == 24);
    hb_always_assert (copy_move_counter_t::copy_मोजणी == 0);
    hb_always_assert (copy_move_counter_t::move_मोजणी == 1);
  }

  {
    copy_move_counter_t::reset();
    copy_move_counter_t fail (-1);
    result<copy_move_counter_t> r_fail = result<copy_move_counter_t>::from(std::move(fail), ERR_B);
    hb_always_assert (r_fail.is_err());
    hb_always_assert (r_fail.error() == ERR_B);
    hb_always_assert (copy_move_counter_t::copy_मोजणी == 0);
    hb_always_assert (copy_move_counter_t::move_मोजणी == 0);
  }
}

पूर्णांक main ()
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
  test_मूल्य_or_default ();
  test_from_move ();

  परत 0;
}
