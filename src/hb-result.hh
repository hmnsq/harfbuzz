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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
ସଂରଚନା hb_result_ok_t
{
  T value;
};

template <>
ସଂରଚନା hb_result_ok_t<ଶୂନ୍ୟ> {};

template <typename E>
ସଂରଚନା hb_result_err_t
{
  E error;

  template <typename OtherE>
  constexpr ତାର୍କିକ operator == (ଅଚଳ hb_result_err_t<OtherE>& other) ଅଚଳ { ଫେରନ୍ତୁ error == other.error; }
  template <typename OtherE>
  constexpr ତାର୍କିକ operator != (ଅଚଳ hb_result_err_t<OtherE>& other) ଅଚଳ { ଫେରନ୍ତୁ error != other.error; }
};

template <typename X> ସଂରଚନା _hb_is_result_tag : hb_false_type {};
template <typename X> ସଂରଚନା _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
template <typename X> ସଂରଚନା _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

template <typename T>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  ଫେରନ୍ତୁ hb_result_ok_t<T&&>{std::forward<T> (v)};
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr hb_result_ok_t<ଶୂନ୍ୟ> Ok ()
{
  ଫେରନ୍ତୁ hb_result_ok_t<ଶୂନ୍ୟ>{};
}

template <typename E>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ constexpr hb_result_err_t<E&&> Err (E&& e)
{
  ଫେରନ୍ତୁ hb_result_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
ସଂରଚନା HB_NODISCARD_STRUCT hb_result_t
{
  ବ୍ୟକ୍ତିଗତ:
  ସଂଘ
  {
    T val_;
    E err_;
  };
  ତାର୍କିକ is_ok_;

  ଶୂନ୍ୟ destroy ()
  {
    ଯଦି (is_ok_)
      val_.~T ();
    ଅନ୍ୟଥା
      err_.~E ();
  }

  ଶୂନ୍ୟ init(ଅଚଳ hb_result_t& o) {
    is_ok_ = o.is_ok_;
    ଯଦି (is_ok_)
      ନୂତନ (std::addressof (val_)) T (o.val_);
    ଅନ୍ୟଥା
      ନୂତନ (std::addressof (err_)) E (o.err_);
  }

  ଶୂନ୍ୟ init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    ଯଦି (is_ok_)
      ନୂତନ (std::addressof (val_)) T (std::move (o.val_));
    ଅନ୍ୟଥା
      ନୂତନ (std::addressof (err_)) E (std::move (o.err_));
  }

  ସର୍ବସାଧାରଣ:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (ଅଚଳ hb_result_t& o) : is_ok_ (ମିଥ୍ୟା)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (ମିଥ୍ୟା)
  {
    init (std::move (o));
  }

  hb_result_t& operator = (ଅଚଳ hb_result_t& o)
  {
    ଯଦି (ଏହି == &o) ଫେରନ୍ତୁ *ଏହି;
    ଯଦି (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    ଅନ୍ୟଥା ଯଦି (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ଅନ୍ୟଥା
    {
      destroy ();
      init (o);
    }
    ଫେରନ୍ତୁ *ଏହି;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    ଯଦି (ଏହି == &o) ଫେରନ୍ତୁ *ଏହି;
    ଯଦି (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    ଅନ୍ୟଥା ଯଦି (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ଅନ୍ୟଥା
    {
      destroy ();
      init (std::move (o));
    }
    ଫେରନ୍ତୁ *ଏହି;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (ସତ୍ୟ)
  {
    ନୂତନ (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, ଅଚଳ U&>::value))>
  hb_result_t (ଅଚଳ hb_result_ok_t<U>& o) : is_ok_ (ସତ୍ୟ)
  {
    ନୂତନ (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (ମିଥ୍ୟା)
  {
    ନୂତନ (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ଅଚଳ F&>::value))>
  hb_result_t (ଅଚଳ hb_result_err_t<F>& e) : is_ok_ (ମିଥ୍ୟା)
  {
    ନୂତନ (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (ମିଥ୍ୟା)
  {
    ନୂତନ (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (ସତ୍ୟ)
  {
    ନୂତନ (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  ସ୍ଥିର hb_result_t from (T&& fallible, E error_ମୂଲ୍ୟ) {
    ଯଦି (fallible.in_error()) {
      ଫେରନ୍ତୁ Err(error_ମୂଲ୍ୟ);
    }
    ଫେରନ୍ତୁ Ok(fallible);
  }

  ତାର୍କିକ is_ok () ଅଚଳ { ଫେରନ୍ତୁ is_ok_; }
  ତାର୍କିକ is_err () ଅଚଳ { ଫେରନ୍ତୁ !is_ok_; }
  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ is_ok (); }

  T& value () & { ପ୍ରତିଜ୍ଞା (is_ok_); ଫେରନ୍ତୁ val_; }
  ଅଚଳ T& value () ଅଚଳ & { ପ୍ରତିଜ୍ଞା (is_ok_); ଫେରନ୍ତୁ val_; }
  T value () && { ପ୍ରତିଜ୍ଞା (is_ok_); ଫେରନ୍ତୁ std::move (val_); }

  T& operator * () & { ଫେରନ୍ତୁ value (); }
  ଅଚଳ T& operator * () ଅଚଳ & { ଫେରନ୍ତୁ value (); }
  T operator * () && { ଫେରନ୍ତୁ std::move (*ଏହି).value (); }

  T* operator -> () { ଫେରନ୍ତୁ std::addressof (value ()); }
  ଅଚଳ T* operator -> () ଅଚଳ { ଫେରନ୍ତୁ std::addressof (value ()); }

  template <typename U>
  T ମୂଲ୍ୟ_or (U&& default_ମୂଲ୍ୟ) ଅଚଳ &
  {
    ଯଦି (is_ok_) ଫେରନ୍ତୁ val_;
    ଫେରନ୍ତୁ std::forward<U> (default_ମୂଲ୍ୟ);
  }

  template <typename U>
  T ମୂଲ୍ୟ_or (U&& default_ମୂଲ୍ୟ) &&
  {
    ଯଦି (is_ok_) ଫେରନ୍ତୁ std::move (val_);
    ଫେରନ୍ତୁ std::forward<U> (default_ମୂଲ୍ୟ);
  }

  T ମୂଲ୍ୟ_or_default () ଅଚଳ &
  {
    ଯଦି (is_ok_) ଫେରନ୍ତୁ val_;
    ଫେରନ୍ତୁ T ();
  }

  T ମୂଲ୍ୟ_or_default () &&
  {
    ଯଦି (is_ok_) ଫେରନ୍ତୁ std::move (val_);
    ଫେରନ୍ତୁ T ();
  }

  ଅଚଳ E& error () ଅଚଳ & { ପ୍ରତିଜ୍ଞା (!is_ok_); ଫେରନ୍ତୁ err_; }
  E& error () & { ପ୍ରତିଜ୍ଞା (!is_ok_); ଫେରନ୍ତୁ err_; }
  E error () && { ପ୍ରତିଜ୍ଞା (!is_ok_); ଫେରନ୍ତୁ std::move (err_); }

  ତାର୍କିକ operator == (ଅଚଳ hb_result_t& o) ଅଚଳ
  {
    ଯଦି (is_ok_ != o.is_ok_) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (is_ok_) ଫେରନ୍ତୁ val_ == o.val_;
    ଫେରନ୍ତୁ err_ == o.err_;
  }

  ତାର୍କିକ operator != (ଅଚଳ hb_result_t& o) ଅଚଳ
  {
    ଫେରନ୍ତୁ !(*ଏହି == o);
  }

  template <typename U>
  ତାର୍କିକ operator == (ଅଚଳ hb_result_ok_t<U>& o) ଅଚଳ
  {
    ଫେରନ୍ତୁ is_ok_ && val_ == o.value;
  }

  template <typename U>
  ତାର୍କିକ operator != (ଅଚଳ hb_result_ok_t<U>& o) ଅଚଳ
  {
    ଫେରନ୍ତୁ !(*ଏହି == o);
  }

  template <typename F>
  ତାର୍କିକ operator == (ଅଚଳ hb_result_err_t<F>& e) ଅଚଳ
  {
    ଫେରନ୍ତୁ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  ତାର୍କିକ operator != (ଅଚଳ hb_result_err_t<F>& e) ଅଚଳ
  {
    ଫେରନ୍ତୁ !(*ଏହି == e);
  }

  template <typename U>
  ତାର୍କିକ operator == (ଅଚଳ U&) ଅଚଳ = delete;

  template <typename U>
  ତାର୍କିକ operator != (ଅଚଳ U&) ଅଚଳ = delete;
};

// Partial specialization for void
template <typename E>
ସଂରଚନା HB_NODISCARD_STRUCT hb_result_t<ଶୂନ୍ୟ, E>
{
  ବ୍ୟକ୍ତିଗତ:
  ସଂଘ
  {
    E err_;
  };
  ତାର୍କିକ is_ok_;

  template <typename, typename> friend ସଂରଚନା hb_result_t;

  ସର୍ବସାଧାରଣ:
  ~hb_result_t ()
  {
    ଯଦି (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (ସତ୍ୟ) {}
  hb_result_t (hb_result_ok_t<ଶୂନ୍ୟ>) : is_ok_ (ସତ୍ୟ) {}

  hb_result_t (ଅଚଳ hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    ଯଦି (!is_ok_)
      ନୂତନ (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    ଯଦି (!is_ok_)
      ନୂତନ (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& operator = (ଅଚଳ hb_result_t& o)
  {
    ଯଦି (ଏହି == &o) ଫେରନ୍ତୁ *ଏହି;
    ଯଦି (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    ଅନ୍ୟଥା ଯଦି (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ସତ୍ୟ;
    }
    ଅନ୍ୟଥା ଯଦି (is_ok_ && !o.is_ok_)
    {
      ନୂତନ (std::addressof (err_)) E (o.err_);
      is_ok_ = ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ *ଏହି;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    ଯଦି (ଏହି == &o) ଫେରନ୍ତୁ *ଏହି;
    ଯଦି (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    ଅନ୍ୟଥା ଯଦି (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = ସତ୍ୟ;
    }
    ଅନ୍ୟଥା ଯଦି (is_ok_ && !o.is_ok_)
    {
      ନୂତନ (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ *ଏହି;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (ମିଥ୍ୟା)
  {
    ନୂତନ (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, ଅଚଳ F&>::value))>
  hb_result_t (ଅଚଳ hb_result_err_t<F>& e) : is_ok_ (ମିଥ୍ୟା)
  {
    ନୂତନ (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (ମିଥ୍ୟା)
  {
    ନୂତନ (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  ସ୍ଥିର hb_result_t<ଶୂନ୍ୟ, E> from (ଅଚଳ V& fallible, E error_ମୂଲ୍ୟ) {
    ଯଦି (fallible.in_error()) {
      ଫେରନ୍ତୁ Err(error_ମୂଲ୍ୟ);
    }
    ଫେରନ୍ତୁ Ok();
  }

  ତାର୍କିକ is_ok () ଅଚଳ { ଫେରନ୍ତୁ is_ok_; }
  ତାର୍କିକ is_err () ଅଚଳ { ଫେରନ୍ତୁ !is_ok_; }
  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ is_ok (); }

  ଅଚଳ E& error () ଅଚଳ & { ପ୍ରତିଜ୍ଞା (!is_ok_); ଫେରନ୍ତୁ err_; }
  E& error () & { ପ୍ରତିଜ୍ଞା (!is_ok_); ଫେରନ୍ତୁ err_; }
  E error () && { ପ୍ରତିଜ୍ଞା (!is_ok_); ଫେରନ୍ତୁ std::move (err_); }

  ଶୂନ୍ୟ operator * () & { ଫେରନ୍ତୁ; }
  ଶୂନ୍ୟ operator * () ଅଚଳ & { ଫେରନ୍ତୁ; }
  ଶୂନ୍ୟ operator * () && { ଫେରନ୍ତୁ; }

  ତାର୍କିକ operator == (ଅଚଳ hb_result_t<ଶୂନ୍ୟ, E>& o) ଅଚଳ
  {
    ଫେରନ୍ତୁ is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  ତାର୍କିକ operator != (ଅଚଳ hb_result_t<ଶୂନ୍ୟ, E>& o) ଅଚଳ
  {
    ଫେରନ୍ତୁ !(*ଏହି == o);
  }

  ତାର୍କିକ operator == (hb_result_ok_t<ଶୂନ୍ୟ>) ଅଚଳ
  {
    ଫେରନ୍ତୁ is_ok_;
  }

  ତାର୍କିକ operator != (hb_result_ok_t<ଶୂନ୍ୟ>) ଅଚଳ
  {
    ଫେରନ୍ତୁ !is_ok_;
  }

  template <typename F>
  ତାର୍କିକ operator == (ଅଚଳ hb_result_err_t<F>& e) ଅଚଳ
  {
    ଫେରନ୍ତୁ !is_ok_ && err_ == e.error;
  }

  template <typename F>
  ତାର୍କିକ operator != (ଅଚଳ hb_result_err_t<F>& e) ଅଚଳ
  {
    ଫେରନ୍ତୁ !(*ଏହି == e);
  }

  template <typename U>
  ତାର୍କିକ operator == (ଅଚଳ U&) ଅଚଳ = delete;

  template <typename U>
  ତାର୍କିକ operator != (ଅଚଳ U&) ଅଚଳ = delete;
};

template <typename U, typename T, typename E>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ operator == (ଅଚଳ hb_result_ok_t<U>& o, ଅଚଳ hb_result_t<T, E>& r)
{
  ଫେରନ୍ତୁ r == o;
}
template <typename U, typename T, typename E>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ operator != (ଅଚଳ hb_result_ok_t<U>& o, ଅଚଳ hb_result_t<T, E>& r)
{
  ଫେରନ୍ତୁ r != o;
}
template <typename F, typename T, typename E>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ operator == (ଅଚଳ hb_result_err_t<F>& e, ଅଚଳ hb_result_t<T, E>& r)
{
  ଫେରନ୍ତୁ r == e;
}
template <typename F, typename T, typename E>
ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ operator != (ଅଚଳ hb_result_err_t<F>& e, ଅଚଳ hb_result_t<T, E>& r)
{
  ଫେରନ୍ତୁ r != e;
}

#ifndef TRY
#define TRY(ପଦସମଷ୍ଟି)                                                             \
  କର {                                                                        \
    auto _res = (ପଦସମଷ୍ଟି);                                                       \
    ଯଦି (unlikely (!_res.is_ok())) ଫେରନ୍ତୁ Err(std::move(_res).error());        \
  } ଯେତେବେଳେ (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  ଯଦି (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) ଫେରନ୍ତୁ Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
