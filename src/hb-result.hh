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

#اگر_غیر_تعریف_شدہ HB_نتیجہ_HH
#تعریف_کرو HB_نتیجہ_HH

#شامل "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
سانچہ <قسم_نام T>
ساخت hb_نتیجہ_ok_t
{
  T value;
};

سانچہ <>
ساخت hb_نتیجہ_ok_t<خالی> {};

سانچہ <قسم_نام E>
ساخت hb_نتیجہ_err_t
{
  E error;

  سانچہ <قسم_نام OtherE>
  constexpr منطقی عامل == (ثابت hb_نتیجہ_err_t<OtherE>& other) ثابت { واپس error == other.error; }
  سانچہ <قسم_نام OtherE>
  constexpr منطقی عامل != (ثابت hb_نتیجہ_err_t<OtherE>& other) ثابت { واپس error != other.error; }
};

سانچہ <قسم_نام X> ساخت hb_is_نتیجہ_لیبل : hb_false_type {};
سانچہ <قسم_نام X> ساخت hb_is_نتیجہ_لیبل<hb_نتیجہ_ok_t<X>> : hb_true_type {};
سانچہ <قسم_نام X> ساخت hb_is_نتیجہ_لیبل<hb_نتیجہ_err_t<X>> : hb_true_type {};

سانچہ <قسم_نام T>
ساکن اندرونی constexpr hb_نتیجہ_ok_t<T&&> Ok (T&& v)
{
  واپس hb_نتیجہ_ok_t<T&&>{std::forward<T> (v)};
}

ساکن اندرونی constexpr hb_نتیجہ_ok_t<خالی> Ok ()
{
  واپس hb_نتیجہ_ok_t<خالی>{};
}

سانچہ <قسم_نام E>
ساکن اندرونی constexpr hb_نتیجہ_err_t<E&&> Err (E&& e)
{
  واپس hb_نتیجہ_err_t<E&&>{std::forward<E> (e)};
}

سانچہ <قسم_نام T, قسم_نام E>
ساخت HB_NODISCARD_STRUCT hb_نتیجہ_t
{
  نجی:
  اتحاد
  {
    T val_;
    E err_;
  };
  منطقی is_ok_;

  خالی destroy ()
  {
    اگر (is_ok_)
      val_.~T ();
    ورنہ
      err_.~E ();
  }

  خالی init(ثابت hb_نتیجہ_t& o) {
    is_ok_ = o.is_ok_;
    اگر (is_ok_)
      نیا (std::addressof (val_)) T (o.val_);
    ورنہ
      نیا (std::addressof (err_)) E (o.err_);
  }

  خالی init(hb_نتیجہ_t&& o) {
    is_ok_ = o.is_ok_;
    اگر (is_ok_)
      نیا (std::addressof (val_)) T (std::move (o.val_));
    ورنہ
      نیا (std::addressof (err_)) E (std::move (o.err_));
  }

  عوامی:
  ~hb_نتیجہ_t ()
  {
    destroy ();
  }

  hb_نتیجہ_t () = حذف;

  hb_نتیجہ_t (ثابت hb_نتیجہ_t& o) : is_ok_ (جھوٹ)
  {
    init (o);
  }

  hb_نتیجہ_t (hb_نتیجہ_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (جھوٹ)
  {
    init (std::move (o));
  }

  hb_نتیجہ_t& عامل = (ثابت hb_نتیجہ_t& o)
  {
    اگر (یہ == &o) واپس *یہ;
    اگر (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    ورنہ اگر (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ورنہ
    {
      destroy ();
      init (o);
    }
    واپس *یہ;
  }

  hb_نتیجہ_t& عامل = (hb_نتیجہ_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    اگر (یہ == &o) واپس *یہ;
    اگر (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    ورنہ اگر (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ورنہ
    {
      destroy ();
      init (std::move (o));
    }
    واپس *یہ;
  }

  // Construct from Ok(...) tag
  سانچہ <قسم_نام U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_نتیجہ_t (hb_نتیجہ_ok_t<U>&& o) : is_ok_ (سچ)
  {
    نیا (std::addressof (val_)) T (std::move (o.value));
  }

  سانچہ <قسم_نام U,
            hb_enable_if ((std::is_constructible<T, ثابت U&>::value))>
  hb_نتیجہ_t (ثابت hb_نتیجہ_ok_t<U>& o) : is_ok_ (سچ)
  {
    نیا (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  سانچہ <قسم_نام F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_نتیجہ_t (hb_نتیجہ_err_t<F>&& e) : is_ok_ (جھوٹ)
  {
    نیا (std::addressof (err_)) E (std::move (e.error));
  }

  سانچہ <قسم_نام F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_نتیجہ_t (ثابت hb_نتیجہ_err_t<F>& e) : is_ok_ (جھوٹ)
  {
    نیا (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  سانچہ <قسم_نام F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_نتیجہ_t (F e) : is_ok_ (جھوٹ)
  {
    نیا (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  سانچہ <قسم_نام U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_نتیجہ_t) &&
                           !hb_is_نتیجہ_لیبل<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_نتیجہ_t (U&& v) : is_ok_ (سچ)
  {
    نیا (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  ساکن hb_نتیجہ_t from (T&& fallible, E error_قدر) {
    اگر (fallible.اندر_error()) {
      واپس Err(error_قدر);
    }
    واپس Ok(fallible);
  }

  منطقی is_ok () ثابت { واپس is_ok_; }
  منطقی is_err () ثابت { واپس !is_ok_; }
  صریح عامل منطقی () ثابت { واپس is_ok (); }

  T& value () & { أكد (is_ok_); واپس val_; }
  ثابت T& value () ثابت & { أكد (is_ok_); واپس val_; }
  T value () && { أكد (is_ok_); واپس std::move (val_); }

  T& عامل * () & { واپس value (); }
  ثابت T& عامل * () ثابت & { واپس value (); }
  T عامل * () && { واپس std::move (*یہ).value (); }

  T* عامل -> () { واپس std::addressof (value ()); }
  ثابت T* عامل -> () ثابت { واپس std::addressof (value ()); }

  سانچہ <قسم_نام U>
  T قدر_or (U&& default_قدر) ثابت &
  {
    اگر (is_ok_) واپس val_;
    واپس std::forward<U> (default_قدر);
  }

  سانچہ <قسم_نام U>
  T قدر_or (U&& default_قدر) &&
  {
    اگر (is_ok_) واپس std::move (val_);
    واپس std::forward<U> (default_قدر);
  }

  T قدر_or_default () ثابت &
  {
    اگر (is_ok_) واپس val_;
    واپس T ();
  }

  T قدر_or_default () &&
  {
    اگر (is_ok_) واپس std::move (val_);
    واپس T ();
  }

  ثابت E& error () ثابت & { أكد (!is_ok_); واپس err_; }
  E& error () & { أكد (!is_ok_); واپس err_; }
  E error () && { أكد (!is_ok_); واپس std::move (err_); }

  منطقی عامل == (ثابت hb_نتیجہ_t& o) ثابت
  {
    اگر (is_ok_ != o.is_ok_) واپس جھوٹ;
    اگر (is_ok_) واپس val_ == o.val_;
    واپس err_ == o.err_;
  }

  منطقی عامل != (ثابت hb_نتیجہ_t& o) ثابت
  {
    واپس !(*یہ == o);
  }

  سانچہ <قسم_نام U>
  منطقی عامل == (ثابت hb_نتیجہ_ok_t<U>& o) ثابت
  {
    واپس is_ok_ && val_ == o.value;
  }

  سانچہ <قسم_نام U>
  منطقی عامل != (ثابت hb_نتیجہ_ok_t<U>& o) ثابت
  {
    واپس !(*یہ == o);
  }

  سانچہ <قسم_نام F>
  منطقی عامل == (ثابت hb_نتیجہ_err_t<F>& e) ثابت
  {
    واپس !is_ok_ && err_ == e.error;
  }

  سانچہ <قسم_نام F>
  منطقی عامل != (ثابت hb_نتیجہ_err_t<F>& e) ثابت
  {
    واپس !(*یہ == e);
  }

  سانچہ <قسم_نام U>
  منطقی عامل == (ثابت U&) ثابت = حذف;

  سانچہ <قسم_نام U>
  منطقی عامل != (ثابت U&) ثابت = حذف;
};

// Partial specialization for void
سانچہ <قسم_نام E>
ساخت HB_NODISCARD_STRUCT hb_نتیجہ_t<خالی, E>
{
  نجی:
  اتحاد
  {
    E err_;
  };
  منطقی is_ok_;

  سانچہ <قسم_نام, قسم_نام> دوست ساخت hb_نتیجہ_t;

  عوامی:
  ~hb_نتیجہ_t ()
  {
    اگر (!is_ok_)
      err_.~E ();
  }

  hb_نتیجہ_t () : is_ok_ (سچ) {}
  hb_نتیجہ_t (hb_نتیجہ_ok_t<خالی>) : is_ok_ (سچ) {}

  hb_نتیجہ_t (ثابت hb_نتیجہ_t& o) : is_ok_ (o.is_ok_)
  {
    اگر (!is_ok_)
      نیا (std::addressof (err_)) E (o.err_);
  }

  hb_نتیجہ_t (hb_نتیجہ_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    اگر (!is_ok_)
      نیا (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_نتیجہ_t& عامل = (ثابت hb_نتیجہ_t& o)
  {
    اگر (یہ == &o) واپس *یہ;
    اگر (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ورنہ اگر (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = سچ;
    }
    ورنہ اگر (is_ok_ && !o.is_ok_)
    {
      نیا (std::addressof (err_)) E (o.err_);
      is_ok_ = جھوٹ;
    }
    واپس *یہ;
  }

  hb_نتیجہ_t& عامل = (hb_نتیجہ_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    اگر (یہ == &o) واپس *یہ;
    اگر (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ورنہ اگر (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = سچ;
    }
    ورنہ اگر (is_ok_ && !o.is_ok_)
    {
      نیا (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = جھوٹ;
    }
    واپس *یہ;
  }

  سانچہ <قسم_نام F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_نتیجہ_t (hb_نتیجہ_err_t<F>&& e) : is_ok_ (جھوٹ)
  {
    نیا (std::addressof (err_)) E (std::move (e.error));
  }

  سانچہ <قسم_نام F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_نتیجہ_t (ثابت hb_نتیجہ_err_t<F>& e) : is_ok_ (جھوٹ)
  {
    نیا (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_نتیجہ_t (E e) : is_ok_ (جھوٹ)
  {
    نیا (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  سانچہ<قسم_نام V>
  ساکن hb_نتیجہ_t<خالی, E> from (ثابت V& fallible, E error_قدر) {
    اگر (fallible.اندر_error()) {
      واپس Err(error_قدر);
    }
    واپس Ok();
  }

  منطقی is_ok () ثابت { واپس is_ok_; }
  منطقی is_err () ثابت { واپس !is_ok_; }
  صریح عامل منطقی () ثابت { واپس is_ok (); }

  ثابت E& error () ثابت & { أكد (!is_ok_); واپس err_; }
  E& error () & { أكد (!is_ok_); واپس err_; }
  E error () && { أكد (!is_ok_); واپس std::move (err_); }

  خالی عامل * () & { واپس; }
  خالی عامل * () ثابت & { واپس; }
  خالی عامل * () && { واپس; }

  منطقی عامل == (ثابت hb_نتیجہ_t<خالی, E>& o) ثابت
  {
    واپس is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  منطقی عامل != (ثابت hb_نتیجہ_t<خالی, E>& o) ثابت
  {
    واپس !(*یہ == o);
  }

  منطقی عامل == (hb_نتیجہ_ok_t<خالی>) ثابت
  {
    واپس is_ok_;
  }

  منطقی عامل != (hb_نتیجہ_ok_t<خالی>) ثابت
  {
    واپس !is_ok_;
  }

  سانچہ <قسم_نام F>
  منطقی عامل == (ثابت hb_نتیجہ_err_t<F>& e) ثابت
  {
    واپس !is_ok_ && err_ == e.error;
  }

  سانچہ <قسم_نام F>
  منطقی عامل != (ثابت hb_نتیجہ_err_t<F>& e) ثابت
  {
    واپس !(*یہ == e);
  }

  سانچہ <قسم_نام U>
  منطقی عامل == (ثابت U&) ثابت = حذف;

  سانچہ <قسم_نام U>
  منطقی عامل != (ثابت U&) ثابت = حذف;
};

سانچہ <قسم_نام U, قسم_نام T, قسم_نام E>
ساکن اندرونی منطقی عامل == (ثابت hb_نتیجہ_ok_t<U>& o, ثابت hb_نتیجہ_t<T, E>& r)
{
  واپس r == o;
}
سانچہ <قسم_نام U, قسم_نام T, قسم_نام E>
ساکن اندرونی منطقی عامل != (ثابت hb_نتیجہ_ok_t<U>& o, ثابت hb_نتیجہ_t<T, E>& r)
{
  واپس r != o;
}
سانچہ <قسم_نام F, قسم_نام T, قسم_نام E>
ساکن اندرونی منطقی عامل == (ثابت hb_نتیجہ_err_t<F>& e, ثابت hb_نتیجہ_t<T, E>& r)
{
  واپس r == e;
}
سانچہ <قسم_نام F, قسم_نام T, قسم_نام E>
ساکن اندرونی منطقی عامل != (ثابت hb_نتیجہ_err_t<F>& e, ثابت hb_نتیجہ_t<T, E>& r)
{
  واپس r != e;
}

#اگر_غیر_تعریف_شدہ TRY
#تعریف_کرو TRY(اظہاریہ)                                                             \
  کرو {                                                                        \
    خودکار _res = (اظہاریہ);                                                       \
    اگر (unlikely (!_res.is_ok())) واپس Err(std::move(_res).error());        \
  } جب (0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ TRY_تفویض
#تعریف_کرو TRY_تفویض(var, ...)                                                  \
  خودکار HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  اگر (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) واپس Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#اگر_ختم

#اگر_ختم /* HB_RESULT_HH */
