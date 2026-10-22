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

#ifndef HB_ফলাফল_HH
#define HB_ফলাফল_HH

#অন্তর্ভাব "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
কাঠামো hb_ফলাফল_ok_t
{
  T value;
};

template <>
কাঠামো hb_ফলাফল_ok_t<শূন্য> {};

template <typename E>
কাঠামো hb_ফলাফল_err_t
{
  E error;

  template <typename OtherE>
  constexpr যৌক্তিক operator == (ধ্রুবক hb_ফলাফল_err_t<OtherE>& other) ধ্রুবক { ফেরত error == other.error; }
  template <typename OtherE>
  constexpr যৌক্তিক operator != (ধ্রুবক hb_ফলাফল_err_t<OtherE>& other) ধ্রুবক { ফেরত error != other.error; }
};

template <typename X> কাঠামো hb_is_ফলাফল_tag : hb_false_type {};
template <typename X> কাঠামো hb_is_ফলাফল_tag<hb_ফলাফল_ok_t<X>> : hb_true_type {};
template <typename X> কাঠামো hb_is_ফলাফল_tag<hb_ফলাফল_err_t<X>> : hb_true_type {};

template <typename T>
স্থির অন্তঃরেখ constexpr hb_ফলাফল_ok_t<T&&> Ok (T&& v)
{
  ফেরত hb_ফলাফল_ok_t<T&&>{std::forward<T> (v)};
}

স্থির অন্তঃরেখ constexpr hb_ফলাফল_ok_t<শূন্য> Ok ()
{
  ফেরত hb_ফলাফল_ok_t<শূন্য>{};
}

template <typename E>
স্থির অন্তঃরেখ constexpr hb_ফলাফল_err_t<E&&> Err (E&& e)
{
  ফেরত hb_ফলাফল_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
কাঠামো HB_NODISCARD_STRUCT hb_ফলাফল_t
{
  ব্যক্তিগত:
  সংঘ
  {
    T val_;
    E err_;
  };
  যৌক্তিক is_ok_;

  শূন্য destroy ()
  {
    যদি (is_ok_)
      val_.~T ();
    অন্যথা
      err_.~E ();
  }

  শূন্য init(ধ্রুবক hb_ফলাফল_t& o) {
    is_ok_ = o.is_ok_;
    যদি (is_ok_)
      নতুন (std::addressof (val_)) T (o.val_);
    অন্যথা
      নতুন (std::addressof (err_)) E (o.err_);
  }

  শূন্য init(hb_ফলাফল_t&& o) {
    is_ok_ = o.is_ok_;
    যদি (is_ok_)
      নতুন (std::addressof (val_)) T (std::move (o.val_));
    অন্যথা
      নতুন (std::addressof (err_)) E (std::move (o.err_));
  }

  সর্বজনীন:
  ~hb_ফলাফল_t ()
  {
    destroy ();
  }

  hb_ফলাফল_t () = delete;

  hb_ফলাফল_t (ধ্রুবক hb_ফলাফল_t& o) : is_ok_ (মিথ্যা)
  {
    init (o);
  }

  hb_ফলাফল_t (hb_ফলাফল_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (মিথ্যা)
  {
    init (std::move (o));
  }

  hb_ফলাফল_t& operator = (ধ্রুবক hb_ফলাফল_t& o)
  {
    যদি (এই == &o) ফেরত *এই;
    যদি (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    অন্যথা যদি (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    অন্যথা
    {
      destroy ();
      init (o);
    }
    ফেরত *এই;
  }

  hb_ফলাফল_t& operator = (hb_ফলাফল_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    যদি (এই == &o) ফেরত *এই;
    যদি (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    অন্যথা যদি (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    অন্যথা
    {
      destroy ();
      init (std::move (o));
    }
    ফেরত *এই;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_ফলাফল_t (hb_ফলাফল_ok_t<U>&& o) : is_ok_ (সত্য)
  {
    নতুন (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, ধ্রুবক U&>::value))>
  hb_ফলাফল_t (ধ্রুবক hb_ফলাফল_ok_t<U>& o) : is_ok_ (সত্য)
  {
    নতুন (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_ফলাফল_t (hb_ফলাফল_err_t<F>&& e) : is_ok_ (মিথ্যা)
  {
    নতুন (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ধ্রুবক F&>::value))>
  hb_ফলাফল_t (ধ্রুবক hb_ফলাফল_err_t<F>& e) : is_ok_ (মিথ্যা)
  {
    নতুন (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_ফলাফল_t (F e) : is_ok_ (মিথ্যা)
  {
    নতুন (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_ফলাফল_t) &&
                           !hb_is_ফলাফল_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_ফলাফল_t (U&& v) : is_ok_ (সত্য)
  {
    নতুন (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  স্থির hb_ফলাফল_t from (T&& fallible, E error_মান) {
    যদি (fallible.in_error()) {
      ফেরত Err(error_মান);
    }
    ফেরত Ok(fallible);
  }

  যৌক্তিক is_ok () ধ্রুবক { ফেরত is_ok_; }
  যৌক্তিক is_err () ধ্রুবক { ফেরত !is_ok_; }
  explicit operator যৌক্তিক () ধ্রুবক { ফেরত is_ok (); }

  T& value () & { প্রতিজ্ঞা (is_ok_); ফেরত val_; }
  ধ্রুবক T& value () ধ্রুবক & { প্রতিজ্ঞা (is_ok_); ফেরত val_; }
  T value () && { প্রতিজ্ঞা (is_ok_); ফেরত std::move (val_); }

  T& operator * () & { ফেরত value (); }
  ধ্রুবক T& operator * () ধ্রুবক & { ফেরত value (); }
  T operator * () && { ফেরত std::move (*এই).value (); }

  T* operator -> () { ফেরত std::addressof (value ()); }
  ধ্রুবক T* operator -> () ধ্রুবক { ফেরত std::addressof (value ()); }

  template <typename U>
  T মান_or (U&& default_মান) ধ্রুবক &
  {
    যদি (is_ok_) ফেরত val_;
    ফেরত std::forward<U> (default_মান);
  }

  template <typename U>
  T মান_or (U&& default_মান) &&
  {
    যদি (is_ok_) ফেরত std::move (val_);
    ফেরত std::forward<U> (default_মান);
  }

  T মান_or_default () ধ্রুবক &
  {
    যদি (is_ok_) ফেরত val_;
    ফেরত T ();
  }

  T মান_or_default () &&
  {
    যদি (is_ok_) ফেরত std::move (val_);
    ফেরত T ();
  }

  ধ্রুবক E& error () ধ্রুবক & { প্রতিজ্ঞা (!is_ok_); ফেরত err_; }
  E& error () & { প্রতিজ্ঞা (!is_ok_); ফেরত err_; }
  E error () && { প্রতিজ্ঞা (!is_ok_); ফেরত std::move (err_); }

  যৌক্তিক operator == (ধ্রুবক hb_ফলাফল_t& o) ধ্রুবক
  {
    যদি (is_ok_ != o.is_ok_) ফেরত মিথ্যা;
    যদি (is_ok_) ফেরত val_ == o.val_;
    ফেরত err_ == o.err_;
  }

  যৌক্তিক operator != (ধ্রুবক hb_ফলাফল_t& o) ধ্রুবক
  {
    ফেরত !(*এই == o);
  }

  template <typename U>
  যৌক্তিক operator == (ধ্রুবক hb_ফলাফল_ok_t<U>& o) ধ্রুবক
  {
    ফেরত is_ok_ && val_ == o.value;
  }

  template <typename U>
  যৌক্তিক operator != (ধ্রুবক hb_ফলাফল_ok_t<U>& o) ধ্রুবক
  {
    ফেরত !(*এই == o);
  }

  template <typename F>
  যৌক্তিক operator == (ধ্রুবক hb_ফলাফল_err_t<F>& e) ধ্রুবক
  {
    ফেরত !is_ok_ && err_ == e.error;
  }

  template <typename F>
  যৌক্তিক operator != (ধ্রুবক hb_ফলাফল_err_t<F>& e) ধ্রুবক
  {
    ফেরত !(*এই == e);
  }

  template <typename U>
  যৌক্তিক operator == (ধ্রুবক U&) ধ্রুবক = delete;

  template <typename U>
  যৌক্তিক operator != (ধ্রুবক U&) ধ্রুবক = delete;
};

// Partial specialization for void
template <typename E>
কাঠামো HB_NODISCARD_STRUCT hb_ফলাফল_t<শূন্য, E>
{
  ব্যক্তিগত:
  সংঘ
  {
    E err_;
  };
  যৌক্তিক is_ok_;

  template <typename, typename> friend কাঠামো hb_ফলাফল_t;

  সর্বজনীন:
  ~hb_ফলাফল_t ()
  {
    যদি (!is_ok_)
      err_.~E ();
  }

  hb_ফলাফল_t () : is_ok_ (সত্য) {}
  hb_ফলাফল_t (hb_ফলাফল_ok_t<শূন্য>) : is_ok_ (সত্য) {}

  hb_ফলাফল_t (ধ্রুবক hb_ফলাফল_t& o) : is_ok_ (o.is_ok_)
  {
    যদি (!is_ok_)
      নতুন (std::addressof (err_)) E (o.err_);
  }

  hb_ফলাফল_t (hb_ফলাফল_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    যদি (!is_ok_)
      নতুন (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_ফলাফল_t& operator = (ধ্রুবক hb_ফলাফল_t& o)
  {
    যদি (এই == &o) ফেরত *এই;
    যদি (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    অন্যথা যদি (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = সত্য;
    }
    অন্যথা যদি (is_ok_ && !o.is_ok_)
    {
      নতুন (std::addressof (err_)) E (o.err_);
      is_ok_ = মিথ্যা;
    }
    ফেরত *এই;
  }

  hb_ফলাফল_t& operator = (hb_ফলাফল_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    যদি (এই == &o) ফেরত *এই;
    যদি (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    অন্যথা যদি (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = সত্য;
    }
    অন্যথা যদি (is_ok_ && !o.is_ok_)
    {
      নতুন (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = মিথ্যা;
    }
    ফেরত *এই;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_ফলাফল_t (hb_ফলাফল_err_t<F>&& e) : is_ok_ (মিথ্যা)
  {
    নতুন (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ধ্রুবক F&>::value))>
  hb_ফলাফল_t (ধ্রুবক hb_ফলাফল_err_t<F>& e) : is_ok_ (মিথ্যা)
  {
    নতুন (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_ফলাফল_t (E e) : is_ok_ (মিথ্যা)
  {
    নতুন (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  স্থির hb_ফলাফল_t<শূন্য, E> from (ধ্রুবক V& fallible, E error_মান) {
    যদি (fallible.in_error()) {
      ফেরত Err(error_মান);
    }
    ফেরত Ok();
  }

  যৌক্তিক is_ok () ধ্রুবক { ফেরত is_ok_; }
  যৌক্তিক is_err () ধ্রুবক { ফেরত !is_ok_; }
  explicit operator যৌক্তিক () ধ্রুবক { ফেরত is_ok (); }

  ধ্রুবক E& error () ধ্রুবক & { প্রতিজ্ঞা (!is_ok_); ফেরত err_; }
  E& error () & { প্রতিজ্ঞা (!is_ok_); ফেরত err_; }
  E error () && { প্রতিজ্ঞা (!is_ok_); ফেরত std::move (err_); }

  শূন্য operator * () & { ফেরত; }
  শূন্য operator * () ধ্রুবক & { ফেরত; }
  শূন্য operator * () && { ফেরত; }

  যৌক্তিক operator == (ধ্রুবক hb_ফলাফল_t<শূন্য, E>& o) ধ্রুবক
  {
    ফেরত is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  যৌক্তিক operator != (ধ্রুবক hb_ফলাফল_t<শূন্য, E>& o) ধ্রুবক
  {
    ফেরত !(*এই == o);
  }

  যৌক্তিক operator == (hb_ফলাফল_ok_t<শূন্য>) ধ্রুবক
  {
    ফেরত is_ok_;
  }

  যৌক্তিক operator != (hb_ফলাফল_ok_t<শূন্য>) ধ্রুবক
  {
    ফেরত !is_ok_;
  }

  template <typename F>
  যৌক্তিক operator == (ধ্রুবক hb_ফলাফল_err_t<F>& e) ধ্রুবক
  {
    ফেরত !is_ok_ && err_ == e.error;
  }

  template <typename F>
  যৌক্তিক operator != (ধ্রুবক hb_ফলাফল_err_t<F>& e) ধ্রুবক
  {
    ফেরত !(*এই == e);
  }

  template <typename U>
  যৌক্তিক operator == (ধ্রুবক U&) ধ্রুবক = delete;

  template <typename U>
  যৌক্তিক operator != (ধ্রুবক U&) ধ্রুবক = delete;
};

template <typename U, typename T, typename E>
স্থির অন্তঃরেখ যৌক্তিক operator == (ধ্রুবক hb_ফলাফল_ok_t<U>& o, ধ্রুবক hb_ফলাফল_t<T, E>& r)
{
  ফেরত r == o;
}
template <typename U, typename T, typename E>
স্থির অন্তঃরেখ যৌক্তিক operator != (ধ্রুবক hb_ফলাফল_ok_t<U>& o, ধ্রুবক hb_ফলাফল_t<T, E>& r)
{
  ফেরত r != o;
}
template <typename F, typename T, typename E>
স্থির অন্তঃরেখ যৌক্তিক operator == (ধ্রুবক hb_ফলাফল_err_t<F>& e, ধ্রুবক hb_ফলাফল_t<T, E>& r)
{
  ফেরত r == e;
}
template <typename F, typename T, typename E>
স্থির অন্তঃরেখ যৌক্তিক operator != (ধ্রুবক hb_ফলাফল_err_t<F>& e, ধ্রুবক hb_ফলাফল_t<T, E>& r)
{
  ফেরত r != e;
}

#ifndef TRY
#define TRY(রাশিমালা)                                                             \
  করো {                                                                        \
    auto _res = (রাশিমালা);                                                       \
    যদি (unlikely (!_res.is_ok())) ফেরত Err(std::move(_res).error());        \
  } যখন (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  যদি (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) ফেরত Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
