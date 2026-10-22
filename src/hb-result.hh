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

#ifndef HB_RESULT_HH
#define HB_RESULT_HH

#ضمن "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
بنية hb_result_ok_t
{
  T value;
};

template <>
بنية hb_result_ok_t<فراغ> {};

template <typename E>
بنية hb_result_err_t
{
  E error;

  template <typename OtherE>
  constexpr منطقي operator == (ثابت hb_result_err_t<OtherE>& other) ثابت { ܦܢܐ error == other.error; }
  template <typename OtherE>
  constexpr منطقي operator != (ثابت hb_result_err_t<OtherE>& other) ثابت { ܦܢܐ error != other.error; }
};

template <typename X> بنية _hb_is_result_tag : hb_false_type {};
template <typename X> بنية _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
template <typename X> بنية _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

template <typename T>
ساكن مضمن constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  ܦܢܐ hb_result_ok_t<T&&>{std::forward<T> (v)};
}

ساكن مضمن constexpr hb_result_ok_t<فراغ> Ok ()
{
  ܦܢܐ hb_result_ok_t<فراغ>{};
}

template <typename E>
ساكن مضمن constexpr hb_result_err_t<E&&> Err (E&& e)
{
  ܦܢܐ hb_result_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
بنية HB_NODISCARD_STRUCT hb_result_t
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
    ܐܢ (is_ok_)
      val_.~T ();
    ܐܚܪܢܐ
      err_.~E ();
  }

  فراغ init(ثابت hb_result_t& o) {
    is_ok_ = o.is_ok_;
    ܐܢ (is_ok_)
      جديد (std::addressof (val_)) T (o.val_);
    ܐܚܪܢܐ
      جديد (std::addressof (err_)) E (o.err_);
  }

  فراغ init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    ܐܢ (is_ok_)
      جديد (std::addressof (val_)) T (std::move (o.val_));
    ܐܚܪܢܐ
      جديد (std::addressof (err_)) E (std::move (o.err_));
  }

  عام:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (ثابت hb_result_t& o) : is_ok_ (ܕܓܠܐ)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (ܕܓܠܐ)
  {
    init (std::move (o));
  }

  hb_result_t& operator = (ثابت hb_result_t& o)
  {
    ܐܢ (هذا == &o) ܦܢܐ *هذا;
    ܐܢ (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    ܐܚܪܢܐ ܐܢ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ܐܚܪܢܐ
    {
      destroy ();
      init (o);
    }
    ܦܢܐ *هذا;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    ܐܢ (هذا == &o) ܦܢܐ *هذا;
    ܐܢ (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    ܐܚܪܢܐ ܐܢ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ܐܚܪܢܐ
    {
      destroy ();
      init (std::move (o));
    }
    ܦܢܐ *هذا;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (ܫܪܪܐ)
  {
    جديد (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, ثابت U&>::value))>
  hb_result_t (ثابت hb_result_ok_t<U>& o) : is_ok_ (ܫܪܪܐ)
  {
    جديد (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (ܕܓܠܐ)
  {
    جديد (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_result_t (ثابت hb_result_err_t<F>& e) : is_ok_ (ܕܓܠܐ)
  {
    جديد (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (ܕܓܠܐ)
  {
    جديد (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (ܫܪܪܐ)
  {
    جديد (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  ساكن hb_result_t from (T&& fallible, E error_value) {
    ܐܢ (fallible.in_error()) {
      ܦܢܐ Err(error_value);
    }
    ܦܢܐ Ok(fallible);
  }

  منطقي is_ok () ثابت { ܦܢܐ is_ok_; }
  منطقي is_err () ثابت { ܦܢܐ !is_ok_; }
  explicit operator منطقي () ثابت { ܦܢܐ is_ok (); }

  T& value () & { أكد (is_ok_); ܦܢܐ val_; }
  ثابت T& value () ثابت & { أكد (is_ok_); ܦܢܐ val_; }
  T value () && { أكد (is_ok_); ܦܢܐ std::move (val_); }

  T& operator * () & { ܦܢܐ value (); }
  ثابت T& operator * () ثابت & { ܦܢܐ value (); }
  T operator * () && { ܦܢܐ std::move (*هذا).value (); }

  T* operator -> () { ܦܢܐ std::addressof (value ()); }
  ثابت T* operator -> () ثابت { ܦܢܐ std::addressof (value ()); }

  template <typename U>
  T value_or (U&& default_value) ثابت &
  {
    ܐܢ (is_ok_) ܦܢܐ val_;
    ܦܢܐ std::forward<U> (default_value);
  }

  template <typename U>
  T value_or (U&& default_value) &&
  {
    ܐܢ (is_ok_) ܦܢܐ std::move (val_);
    ܦܢܐ std::forward<U> (default_value);
  }

  T value_or_default () ثابت &
  {
    ܐܢ (is_ok_) ܦܢܐ val_;
    ܦܢܐ T ();
  }

  T value_or_default () &&
  {
    ܐܢ (is_ok_) ܦܢܐ std::move (val_);
    ܦܢܐ T ();
  }

  ثابت E& error () ثابت & { أكد (!is_ok_); ܦܢܐ err_; }
  E& error () & { أكد (!is_ok_); ܦܢܐ err_; }
  E error () && { أكد (!is_ok_); ܦܢܐ std::move (err_); }

  منطقي operator == (ثابت hb_result_t& o) ثابت
  {
    ܐܢ (is_ok_ != o.is_ok_) ܦܢܐ ܕܓܠܐ;
    ܐܢ (is_ok_) ܦܢܐ val_ == o.val_;
    ܦܢܐ err_ == o.err_;
  }

  منطقي operator != (ثابت hb_result_t& o) ثابت
  {
    ܦܢܐ !(*هذا == o);
  }

  template <typename U>
  منطقي operator == (ثابت hb_result_ok_t<U>& o) ثابت
  {
    ܦܢܐ is_ok_ && val_ == o.value;
  }

  template <typename U>
  منطقي operator != (ثابت hb_result_ok_t<U>& o) ثابت
  {
    ܦܢܐ !(*هذا == o);
  }

  template <typename F>
  منطقي operator == (ثابت hb_result_err_t<F>& e) ثابت
  {
    ܦܢܐ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  منطقي operator != (ثابت hb_result_err_t<F>& e) ثابت
  {
    ܦܢܐ !(*هذا == e);
  }

  template <typename U>
  منطقي operator == (ثابت U&) ثابت = delete;

  template <typename U>
  منطقي operator != (ثابت U&) ثابت = delete;
};

// Partial specialization for void
template <typename E>
بنية HB_NODISCARD_STRUCT hb_result_t<فراغ, E>
{
  خاص:
  اتحاد
  {
    E err_;
  };
  منطقي is_ok_;

  template <typename, typename> friend بنية hb_result_t;

  عام:
  ~hb_result_t ()
  {
    ܐܢ (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (ܫܪܪܐ) {}
  hb_result_t (hb_result_ok_t<فراغ>) : is_ok_ (ܫܪܪܐ) {}

  hb_result_t (ثابت hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    ܐܢ (!is_ok_)
      جديد (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    ܐܢ (!is_ok_)
      جديد (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& operator = (ثابت hb_result_t& o)
  {
    ܐܢ (هذا == &o) ܦܢܐ *هذا;
    ܐܢ (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ܐܚܪܢܐ ܐܢ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ܫܪܪܐ;
    }
    ܐܚܪܢܐ ܐܢ (is_ok_ && !o.is_ok_)
    {
      جديد (std::addressof (err_)) E (o.err_);
      is_ok_ = ܕܓܠܐ;
    }
    ܦܢܐ *هذا;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    ܐܢ (هذا == &o) ܦܢܐ *هذا;
    ܐܢ (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ܐܚܪܢܐ ܐܢ (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ܫܪܪܐ;
    }
    ܐܚܪܢܐ ܐܢ (is_ok_ && !o.is_ok_)
    {
      جديد (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = ܕܓܠܐ;
    }
    ܦܢܐ *هذا;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (ܕܓܠܐ)
  {
    جديد (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ثابت F&>::value))>
  hb_result_t (ثابت hb_result_err_t<F>& e) : is_ok_ (ܕܓܠܐ)
  {
    جديد (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (ܕܓܠܐ)
  {
    جديد (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  ساكن hb_result_t<فراغ, E> from (ثابت V& fallible, E error_value) {
    ܐܢ (fallible.in_error()) {
      ܦܢܐ Err(error_value);
    }
    ܦܢܐ Ok();
  }

  منطقي is_ok () ثابت { ܦܢܐ is_ok_; }
  منطقي is_err () ثابت { ܦܢܐ !is_ok_; }
  explicit operator منطقي () ثابت { ܦܢܐ is_ok (); }

  ثابت E& error () ثابت & { أكد (!is_ok_); ܦܢܐ err_; }
  E& error () & { أكد (!is_ok_); ܦܢܐ err_; }
  E error () && { أكد (!is_ok_); ܦܢܐ std::move (err_); }

  فراغ operator * () & { ܦܢܐ; }
  فراغ operator * () ثابت & { ܦܢܐ; }
  فراغ operator * () && { ܦܢܐ; }

  منطقي operator == (ثابت hb_result_t<فراغ, E>& o) ثابت
  {
    ܦܢܐ is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  منطقي operator != (ثابت hb_result_t<فراغ, E>& o) ثابت
  {
    ܦܢܐ !(*هذا == o);
  }

  منطقي operator == (hb_result_ok_t<فراغ>) ثابت
  {
    ܦܢܐ is_ok_;
  }

  منطقي operator != (hb_result_ok_t<فراغ>) ثابت
  {
    ܦܢܐ !is_ok_;
  }

  template <typename F>
  منطقي operator == (ثابت hb_result_err_t<F>& e) ثابت
  {
    ܦܢܐ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  منطقي operator != (ثابت hb_result_err_t<F>& e) ثابت
  {
    ܦܢܐ !(*هذا == e);
  }

  template <typename U>
  منطقي operator == (ثابت U&) ثابت = delete;

  template <typename U>
  منطقي operator != (ثابت U&) ثابت = delete;
};

template <typename U, typename T, typename E>
ساكن مضمن منطقي operator == (ثابت hb_result_ok_t<U>& o, ثابت hb_result_t<T, E>& r)
{
  ܦܢܐ r == o;
}
template <typename U, typename T, typename E>
ساكن مضمن منطقي operator != (ثابت hb_result_ok_t<U>& o, ثابت hb_result_t<T, E>& r)
{
  ܦܢܐ r != o;
}
template <typename F, typename T, typename E>
ساكن مضمن منطقي operator == (ثابت hb_result_err_t<F>& e, ثابت hb_result_t<T, E>& r)
{
  ܦܢܐ r == e;
}
template <typename F, typename T, typename E>
ساكن مضمن منطقي operator != (ثابت hb_result_err_t<F>& e, ثابت hb_result_t<T, E>& r)
{
  ܦܢܐ r != e;
}

#ifndef TRY
#define TRY(expr)                                                             \
  ܥܒܕ {                                                                        \
    auto _res = (expr);                                                       \
    ܐܢ (unlikely (!_res.is_ok())) ܦܢܐ Err(std::move(_res).error());        \
  } ܟܡܐ (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  ܐܢ (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) ܦܢܐ Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
