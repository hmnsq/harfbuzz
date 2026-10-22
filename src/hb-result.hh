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

#اگر_تعریف_نشده HB_نتیجه_HH
#تعریف_کن HB_نتیجه_HH

#ضمن "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
قالب <نام_نوع T>
بنية hb_نتیجه_ok_t
{
  T value;
};

قالب <>
بنية hb_نتیجه_ok_t<فراغ> {};

قالب <نام_نوع E>
بنية hb_نتیجه_err_t
{
  E error;

  قالب <نام_نوع OtherE>
  constexpr منطقي عملگر == (ثابت hb_نتیجه_err_t<OtherE>& other) ثابت { أرجع error == other.error; }
  قالب <نام_نوع OtherE>
  constexpr منطقي عملگر != (ثابت hb_نتیجه_err_t<OtherE>& other) ثابت { أرجع error != other.error; }
};

قالب <نام_نوع X> بنية hb_is_نتیجه_برچسب : hb_false_type {};
قالب <نام_نوع X> بنية hb_is_نتیجه_برچسب<hb_نتیجه_ok_t<X>> : hb_true_type {};
قالب <نام_نوع X> بنية hb_is_نتیجه_برچسب<hb_نتیجه_err_t<X>> : hb_true_type {};

قالب <نام_نوع T>
ساكن مضمن constexpr hb_نتیجه_ok_t<T&&> Ok (T&& v)
{
  أرجع hb_نتیجه_ok_t<T&&>{std::forward<T> (v)};
}

ساكن مضمن constexpr hb_نتیجه_ok_t<فراغ> Ok ()
{
  أرجع hb_نتیجه_ok_t<فراغ>{};
}

قالب <نام_نوع E>
ساكن مضمن constexpr hb_نتیجه_err_t<E&&> Err (E&& e)
{
  أرجع hb_نتیجه_err_t<E&&>{std::forward<E> (e)};
}

قالب <نام_نوع T, نام_نوع E>
بنية HB_NODISCARD_STRUCT hb_نتیجه_t
{
  خاص:
  اتحاد
  {
    T val_;
    E err_;
  };
  منطقي is_ok_;

  فراغ destroy ()
  {
    إذا (is_ok_)
      val_.~T ();
    وإلا
      err_.~E ();
  }

  فراغ init(ثابت hb_نتیجه_t& o) {
    is_ok_ = o.is_ok_;
    إذا (is_ok_)
      جديد (std::addressof (val_)) T (o.val_);
    وإلا
      جديد (std::addressof (err_)) E (o.err_);
  }

  فراغ init(hb_نتیجه_t&& o) {
    is_ok_ = o.is_ok_;
    إذا (is_ok_)
      جديد (std::addressof (val_)) T (std::move (o.val_));
    وإلا
      جديد (std::addressof (err_)) E (std::move (o.err_));
  }

  عام:
  ~hb_نتیجه_t ()
  {
    destroy ();
  }

  hb_نتیجه_t () = حذف;

  hb_نتیجه_t (ثابت hb_نتیجه_t& o) : is_ok_ (خطأ)
  {
    init (o);
  }

  hb_نتیجه_t (hb_نتیجه_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (خطأ)
  {
    init (std::move (o));
  }

  hb_نتیجه_t& عملگر = (ثابت hb_نتیجه_t& o)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    وإلا إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    وإلا
    {
      destroy ();
      init (o);
    }
    أرجع *هذا;
  }

  hb_نتیجه_t& عملگر = (hb_نتیجه_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    وإلا إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    وإلا
    {
      destroy ();
      init (std::move (o));
    }
    أرجع *هذا;
  }

  // Construct from Ok(...) tag
  قالب <نام_نوع U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_نتیجه_t (hb_نتیجه_ok_t<U>&& o) : is_ok_ (درست)
  {
    جديد (std::addressof (val_)) T (std::move (o.value));
  }

  قالب <نام_نوع U,
            hb_enable_if ((std::is_constructible<T, ثابت U&>::value))>
  hb_نتیجه_t (ثابت hb_نتیجه_ok_t<U>& o) : is_ok_ (درست)
  {
    جديد (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  قالب <نام_نوع F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_نتیجه_t (hb_نتیجه_err_t<F>&& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move (e.error));
  }

  قالب <نام_نوع F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_نتیجه_t (ثابت hb_نتیجه_err_t<F>& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  قالب <نام_نوع F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_نتیجه_t (F e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  قالب <نام_نوع U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_نتیجه_t) &&
                           !hb_is_نتیجه_برچسب<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_نتیجه_t (U&& v) : is_ok_ (درست)
  {
    جديد (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  ساكن hb_نتیجه_t from (T&& fallible, E error_مقدار) {
    إذا (fallible.درون_error()) {
      أرجع Err(error_مقدار);
    }
    أرجع Ok(fallible);
  }

  منطقي is_ok () ثابت { أرجع is_ok_; }
  منطقي is_err () ثابت { أرجع !is_ok_; }
  صریح عملگر منطقي () ثابت { أرجع is_ok (); }

  T& value () & { أكد (is_ok_); أرجع val_; }
  ثابت T& value () ثابت & { أكد (is_ok_); أرجع val_; }
  T value () && { أكد (is_ok_); أرجع std::move (val_); }

  T& عملگر * () & { أرجع value (); }
  ثابت T& عملگر * () ثابت & { أرجع value (); }
  T عملگر * () && { أرجع std::move (*هذا).value (); }

  T* عملگر -> () { أرجع std::addressof (value ()); }
  ثابت T* عملگر -> () ثابت { أرجع std::addressof (value ()); }

  قالب <نام_نوع U>
  T مقدار_or (U&& default_مقدار) ثابت &
  {
    إذا (is_ok_) أرجع val_;
    أرجع std::forward<U> (default_مقدار);
  }

  قالب <نام_نوع U>
  T مقدار_or (U&& default_مقدار) &&
  {
    إذا (is_ok_) أرجع std::move (val_);
    أرجع std::forward<U> (default_مقدار);
  }

  T مقدار_or_default () ثابت &
  {
    إذا (is_ok_) أرجع val_;
    أرجع T ();
  }

  T مقدار_or_default () &&
  {
    إذا (is_ok_) أرجع std::move (val_);
    أرجع T ();
  }

  ثابت E& error () ثابت & { أكد (!is_ok_); أرجع err_; }
  E& error () & { أكد (!is_ok_); أرجع err_; }
  E error () && { أكد (!is_ok_); أرجع std::move (err_); }

  منطقي عملگر == (ثابت hb_نتیجه_t& o) ثابت
  {
    إذا (is_ok_ != o.is_ok_) أرجع خطأ;
    إذا (is_ok_) أرجع val_ == o.val_;
    أرجع err_ == o.err_;
  }

  منطقي عملگر != (ثابت hb_نتیجه_t& o) ثابت
  {
    أرجع !(*هذا == o);
  }

  قالب <نام_نوع U>
  منطقي عملگر == (ثابت hb_نتیجه_ok_t<U>& o) ثابت
  {
    أرجع is_ok_ && val_ == o.value;
  }

  قالب <نام_نوع U>
  منطقي عملگر != (ثابت hb_نتیجه_ok_t<U>& o) ثابت
  {
    أرجع !(*هذا == o);
  }

  قالب <نام_نوع F>
  منطقي عملگر == (ثابت hb_نتیجه_err_t<F>& e) ثابت
  {
    أرجع !is_ok_ && err_ == e.error;
  }

  قالب <نام_نوع F>
  منطقي عملگر != (ثابت hb_نتیجه_err_t<F>& e) ثابت
  {
    أرجع !(*هذا == e);
  }

  قالب <نام_نوع U>
  منطقي عملگر == (ثابت U&) ثابت = حذف;

  قالب <نام_نوع U>
  منطقي عملگر != (ثابت U&) ثابت = حذف;
};

// Partial specialization for void
قالب <نام_نوع E>
بنية HB_NODISCARD_STRUCT hb_نتیجه_t<فراغ, E>
{
  خاص:
  اتحاد
  {
    E err_;
  };
  منطقي is_ok_;

  قالب <نام_نوع, نام_نوع> دوست بنية hb_نتیجه_t;

  عام:
  ~hb_نتیجه_t ()
  {
    إذا (!is_ok_)
      err_.~E ();
  }

  hb_نتیجه_t () : is_ok_ (درست) {}
  hb_نتیجه_t (hb_نتیجه_ok_t<فراغ>) : is_ok_ (درست) {}

  hb_نتیجه_t (ثابت hb_نتیجه_t& o) : is_ok_ (o.is_ok_)
  {
    إذا (!is_ok_)
      جديد (std::addressof (err_)) E (o.err_);
  }

  hb_نتیجه_t (hb_نتیجه_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    إذا (!is_ok_)
      جديد (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_نتیجه_t& عملگر = (ثابت hb_نتیجه_t& o)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    وإلا إذا (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = درست;
    }
    وإلا إذا (is_ok_ && !o.is_ok_)
    {
      جديد (std::addressof (err_)) E (o.err_);
      is_ok_ = خطأ;
    }
    أرجع *هذا;
  }

  hb_نتیجه_t& عملگر = (hb_نتیجه_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    إذا (هذا == &o) أرجع *هذا;
    إذا (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    وإلا إذا (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = درست;
    }
    وإلا إذا (is_ok_ && !o.is_ok_)
    {
      جديد (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = خطأ;
    }
    أرجع *هذا;
  }

  قالب <نام_نوع F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_نتیجه_t (hb_نتیجه_err_t<F>&& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move (e.error));
  }

  قالب <نام_نوع F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_نتیجه_t (ثابت hb_نتیجه_err_t<F>& e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_نتیجه_t (E e) : is_ok_ (خطأ)
  {
    جديد (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  قالب<نام_نوع V>
  ساكن hb_نتیجه_t<فراغ, E> from (ثابت V& fallible, E error_مقدار) {
    إذا (fallible.درون_error()) {
      أرجع Err(error_مقدار);
    }
    أرجع Ok();
  }

  منطقي is_ok () ثابت { أرجع is_ok_; }
  منطقي is_err () ثابت { أرجع !is_ok_; }
  صریح عملگر منطقي () ثابت { أرجع is_ok (); }

  ثابت E& error () ثابت & { أكد (!is_ok_); أرجع err_; }
  E& error () & { أكد (!is_ok_); أرجع err_; }
  E error () && { أكد (!is_ok_); أرجع std::move (err_); }

  فراغ عملگر * () & { أرجع; }
  فراغ عملگر * () ثابت & { أرجع; }
  فراغ عملگر * () && { أرجع; }

  منطقي عملگر == (ثابت hb_نتیجه_t<فراغ, E>& o) ثابت
  {
    أرجع is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  منطقي عملگر != (ثابت hb_نتیجه_t<فراغ, E>& o) ثابت
  {
    أرجع !(*هذا == o);
  }

  منطقي عملگر == (hb_نتیجه_ok_t<فراغ>) ثابت
  {
    أرجع is_ok_;
  }

  منطقي عملگر != (hb_نتیجه_ok_t<فراغ>) ثابت
  {
    أرجع !is_ok_;
  }

  قالب <نام_نوع F>
  منطقي عملگر == (ثابت hb_نتیجه_err_t<F>& e) ثابت
  {
    أرجع !is_ok_ && err_ == e.error;
  }

  قالب <نام_نوع F>
  منطقي عملگر != (ثابت hb_نتیجه_err_t<F>& e) ثابت
  {
    أرجع !(*هذا == e);
  }

  قالب <نام_نوع U>
  منطقي عملگر == (ثابت U&) ثابت = حذف;

  قالب <نام_نوع U>
  منطقي عملگر != (ثابت U&) ثابت = حذف;
};

قالب <نام_نوع U, نام_نوع T, نام_نوع E>
ساكن مضمن منطقي عملگر == (ثابت hb_نتیجه_ok_t<U>& o, ثابت hb_نتیجه_t<T, E>& r)
{
  أرجع r == o;
}
قالب <نام_نوع U, نام_نوع T, نام_نوع E>
ساكن مضمن منطقي عملگر != (ثابت hb_نتیجه_ok_t<U>& o, ثابت hb_نتیجه_t<T, E>& r)
{
  أرجع r != o;
}
قالب <نام_نوع F, نام_نوع T, نام_نوع E>
ساكن مضمن منطقي عملگر == (ثابت hb_نتیجه_err_t<F>& e, ثابت hb_نتیجه_t<T, E>& r)
{
  أرجع r == e;
}
قالب <نام_نوع F, نام_نوع T, نام_نوع E>
ساكن مضمن منطقي عملگر != (ثابت hb_نتیجه_err_t<F>& e, ثابت hb_نتیجه_t<T, E>& r)
{
  أرجع r != e;
}

#اگر_تعریف_نشده TRY
#تعریف_کن TRY(عبارت)                                                             \
  افعل {                                                                        \
    خودکار _res = (عبارت);                                                       \
    إذا (unlikely (!_res.is_ok())) أرجع Err(std::move(_res).error());        \
  } طالما (0)
#پایان_اگر

#اگر_تعریف_نشده TRY_تخصیص_بده
#تعریف_کن TRY_تخصیص_بده(var, ...)                                                  \
  خودکار HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  إذا (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) أرجع Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#پایان_اگر

#پایان_اگر /* HB_RESULT_HH */
