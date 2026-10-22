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

#અન્તર્ભાવ "hb.hh"

// Helper structs for Ok(...) and Err(...), not used directly
// by users of hb_result_t.
template <typename T>
સંરચના hb_result_ok_t
{
  T value;
};

template <>
સંરચના hb_result_ok_t<શૂન્ય> {};

template <typename E>
સંરચના hb_result_err_t
{
  E error;

  template <typename OtherE>
  constexpr તાર્કિક operator == (અચલ hb_result_err_t<OtherE>& other) અચલ { પાછા error == other.error; }
  template <typename OtherE>
  constexpr તાર્કિક operator != (અચલ hb_result_err_t<OtherE>& other) અચલ { પાછા error != other.error; }
};

template <typename X> સંરચના _hb_is_result_tag : hb_false_type {};
template <typename X> સંરચના _hb_is_result_tag<hb_result_ok_t<X>> : hb_true_type {};
template <typename X> સંરચના _hb_is_result_tag<hb_result_err_t<X>> : hb_true_type {};

template <typename T>
સ્થિર અંતઃરેખ constexpr hb_result_ok_t<T&&> Ok (T&& v)
{
  પાછા hb_result_ok_t<T&&>{std::forward<T> (v)};
}

સ્થિર અંતઃરેખ constexpr hb_result_ok_t<શૂન્ય> Ok ()
{
  પાછા hb_result_ok_t<શૂન્ય>{};
}

template <typename E>
સ્થિર અંતઃરેખ constexpr hb_result_err_t<E&&> Err (E&& e)
{
  પાછા hb_result_err_t<E&&>{std::forward<E> (e)};
}

template <typename T, typename E>
સંરચના HB_NODISCARD_STRUCT hb_result_t
{
  ખાનગી:
  સંઘ
  {
    T val_;
    E err_;
  };
  તાર્કિક is_ok_;

  શૂન્ય destroy ()
  {
    જો (is_ok_)
      val_.~T ();
    નહીંતર
      err_.~E ();
  }

  શૂન્ય init(અચલ hb_result_t& o) {
    is_ok_ = o.is_ok_;
    જો (is_ok_)
      નવું (std::addressof (val_)) T (o.val_);
    નહીંતર
      નવું (std::addressof (err_)) E (o.err_);
  }

  શૂન્ય init(hb_result_t&& o) {
    is_ok_ = o.is_ok_;
    જો (is_ok_)
      નવું (std::addressof (val_)) T (std::move (o.val_));
    નહીંતર
      નવું (std::addressof (err_)) E (std::move (o.err_));
  }

  જાહેર:
  ~hb_result_t ()
  {
    destroy ();
  }

  hb_result_t () = delete;

  hb_result_t (અચલ hb_result_t& o) : is_ok_ (ખોટું)
  {
    init (o);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<T>::value &&
                                     std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (ખોટું)
  {
    init (std::move (o));
  }

  hb_result_t& operator = (અચલ hb_result_t& o)
  {
    જો (આ == &o) પાછા *આ;
    જો (is_ok_ && o.is_ok_)
    {
      val_ = o.val_;
    }
    નહીંતર જો (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    નહીંતર
    {
      destroy ();
      init (o);
    }
    પાછા *આ;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<T>::value &&
                                                std::is_nothrow_move_assignable<E>::value)
  {
    જો (આ == &o) પાછા *આ;
    જો (is_ok_ && o.is_ok_)
    {
      val_ = std::move (o.val_);
    }
    નહીંતર જો (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    નહીંતર
    {
      destroy ();
      init (std::move (o));
    }
    પાછા *આ;
  }

  // Construct from Ok(...) tag
  template <typename U,
            hb_enable_if ((std::is_constructible<T, U>::value))>
  hb_result_t (hb_result_ok_t<U>&& o) : is_ok_ (સાચું)
  {
    નવું (std::addressof (val_)) T (std::move (o.value));
  }

  template <typename U,
            hb_enable_if ((std::is_constructible<T, અચલ U&>::value))>
  hb_result_t (અચલ hb_result_ok_t<U>& o) : is_ok_ (સાચું)
  {
    નવું (std::addressof (val_)) T (o.value);
  }

  // Construct from Err(...) tag
  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (ખોટું)
  {
    નવું (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, અચલ F&>::value))>
  hb_result_t (અચલ hb_result_err_t<F>& e) : is_ok_ (ખોટું)
  {
    નવું (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E (when neither T or E are convertible into each other)
  template <typename F = E,
            hb_enable_if ((hb_is_same (F, E) &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (F e) : is_ok_ (ખોટું)
  {
    નવું (std::addressof (err_)) E (std::move (e));
  }

  // Construct directly from value T (when neither T or E are convertible into each other)
  template <typename U = T,
            hb_enable_if ((std::is_constructible<T, U>::value &&
                           !hb_is_same (hb_decay<U>, E) &&
                           !hb_is_same (hb_decay<U>, hb_result_t) &&
                           !_hb_is_result_tag<hb_decay<U>>::value &&
                           !(hb_is_convertible (T, E) || hb_is_convertible (E, T))))>
  hb_result_t (U&& v) : is_ok_ (સાચું)
  {
    નવું (std::addressof (val_)) T (std::forward<U> (v));
  }

  // Move fallible into a result if it's in_error() method returns false.
  સ્થિર hb_result_t from (T&& fallible, E error_કિંમત) {
    જો (fallible.in_error()) {
      પાછા Err(error_કિંમત);
    }
    પાછા Ok(fallible);
  }

  તાર્કિક is_ok () અચલ { પાછા is_ok_; }
  તાર્કિક is_err () અચલ { પાછા !is_ok_; }
  explicit operator તાર્કિક () અચલ { પાછા is_ok (); }

  T& value () & { પ્રતિજ્ઞા (is_ok_); પાછા val_; }
  અચલ T& value () અચલ & { પ્રતિજ્ઞા (is_ok_); પાછા val_; }
  T value () && { પ્રતિજ્ઞા (is_ok_); પાછા std::move (val_); }

  T& operator * () & { પાછા value (); }
  અચલ T& operator * () અચલ & { પાછા value (); }
  T operator * () && { પાછા std::move (*આ).value (); }

  T* operator -> () { પાછા std::addressof (value ()); }
  અચલ T* operator -> () અચલ { પાછા std::addressof (value ()); }

  template <typename U>
  T કિંમત_or (U&& default_કિંમત) અચલ &
  {
    જો (is_ok_) પાછા val_;
    પાછા std::forward<U> (default_કિંમત);
  }

  template <typename U>
  T કિંમત_or (U&& default_કિંમત) &&
  {
    જો (is_ok_) પાછા std::move (val_);
    પાછા std::forward<U> (default_કિંમત);
  }

  T કિંમત_or_default () અચલ &
  {
    જો (is_ok_) પાછા val_;
    પાછા T ();
  }

  T કિંમત_or_default () &&
  {
    જો (is_ok_) પાછા std::move (val_);
    પાછા T ();
  }

  અચલ E& error () અચલ & { પ્રતિજ્ઞા (!is_ok_); પાછા err_; }
  E& error () & { પ્રતિજ્ઞા (!is_ok_); પાછા err_; }
  E error () && { પ્રતિજ્ઞા (!is_ok_); પાછા std::move (err_); }

  તાર્કિક operator == (અચલ hb_result_t& o) અચલ
  {
    જો (is_ok_ != o.is_ok_) પાછા ખોટું;
    જો (is_ok_) પાછા val_ == o.val_;
    પાછા err_ == o.err_;
  }

  તાર્કિક operator != (અચલ hb_result_t& o) અચલ
  {
    પાછા !(*આ == o);
  }

  template <typename U>
  તાર્કિક operator == (અચલ hb_result_ok_t<U>& o) અચલ
  {
    પાછા is_ok_ && val_ == o.value;
  }

  template <typename U>
  તાર્કિક operator != (અચલ hb_result_ok_t<U>& o) અચલ
  {
    પાછા !(*આ == o);
  }

  template <typename F>
  તાર્કિક operator == (અચલ hb_result_err_t<F>& e) અચલ
  {
    પાછા !is_ok_ && err_ == e.error;
  }

  template <typename F>
  તાર્કિક operator != (અચલ hb_result_err_t<F>& e) અચલ
  {
    પાછા !(*આ == e);
  }

  template <typename U>
  તાર્કિક operator == (અચલ U&) અચલ = delete;

  template <typename U>
  તાર્કિક operator != (અચલ U&) અચલ = delete;
};

// Partial specialization for void
template <typename E>
સંરચના HB_NODISCARD_STRUCT hb_result_t<શૂન્ય, E>
{
  ખાનગી:
  સંઘ
  {
    E err_;
  };
  તાર્કિક is_ok_;

  template <typename, typename> friend સંરચના hb_result_t;

  જાહેર:
  ~hb_result_t ()
  {
    જો (!is_ok_)
      err_.~E ();
  }

  hb_result_t () : is_ok_ (સાચું) {}
  hb_result_t (hb_result_ok_t<શૂન્ય>) : is_ok_ (સાચું) {}

  hb_result_t (અચલ hb_result_t& o) : is_ok_ (o.is_ok_)
  {
    જો (!is_ok_)
      નવું (std::addressof (err_)) E (o.err_);
  }

  hb_result_t (hb_result_t&& o) noexcept (std::is_nothrow_move_constructible<E>::value)
      : is_ok_ (o.is_ok_)
  {
    જો (!is_ok_)
      નવું (std::addressof (err_)) E (std::move (o.err_));
  }

  hb_result_t& operator = (અચલ hb_result_t& o)
  {
    જો (આ == &o) પાછા *આ;
    જો (!is_ok_ && !o.is_ok_)
    {
      err_ = o.err_;
    }
    નહીંતર જો (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = સાચું;
    }
    નહીંતર જો (is_ok_ && !o.is_ok_)
    {
      નવું (std::addressof (err_)) E (o.err_);
      is_ok_ = ખોટું;
    }
    પાછા *આ;
  }

  hb_result_t& operator = (hb_result_t&& o) noexcept (std::is_nothrow_move_assignable<E>::value)
  {
    જો (આ == &o) પાછા *આ;
    જો (!is_ok_ && !o.is_ok_)
    {
      err_ = std::move (o.err_);
    }
    નહીંતર જો (!is_ok_ && o.is_ok_)
    {
      err_.~E ();
      is_ok_ = સાચું;
    }
    નહીંતર જો (is_ok_ && !o.is_ok_)
    {
      નવું (std::addressof (err_)) E (std::move (o.err_));
      is_ok_ = ખોટું;
    }
    પાછા *આ;
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, F>::value))>
  hb_result_t (hb_result_err_t<F>&& e) : is_ok_ (ખોટું)
  {
    નવું (std::addressof (err_)) E (std::move (e.error));
  }

  template <typename F,
            hb_enable_if ((std::is_constructible<E, અચલ F&>::value))>
  hb_result_t (અચલ hb_result_err_t<F>& e) : is_ok_ (ખોટું)
  {
    નવું (std::addressof (err_)) E (e.error);
  }

  // Construct directly from error E
  hb_result_t (E e) : is_ok_ (ખોટું)
  {
    નવું (std::addressof (err_)) E (std::move(e));
  }

  // V is any class which has an in_error() method. This checks the result
  // of in_error() and returns either Ok() or Err(error_value).
  template<typename V>
  સ્થિર hb_result_t<શૂન્ય, E> from (અચલ V& fallible, E error_કિંમત) {
    જો (fallible.in_error()) {
      પાછા Err(error_કિંમત);
    }
    પાછા Ok();
  }

  તાર્કિક is_ok () અચલ { પાછા is_ok_; }
  તાર્કિક is_err () અચલ { પાછા !is_ok_; }
  explicit operator તાર્કિક () અચલ { પાછા is_ok (); }

  અચલ E& error () અચલ & { પ્રતિજ્ઞા (!is_ok_); પાછા err_; }
  E& error () & { પ્રતિજ્ઞા (!is_ok_); પાછા err_; }
  E error () && { પ્રતિજ્ઞા (!is_ok_); પાછા std::move (err_); }

  શૂન્ય operator * () & { પાછા; }
  શૂન્ય operator * () અચલ & { પાછા; }
  શૂન્ય operator * () && { પાછા; }

  તાર્કિક operator == (અચલ hb_result_t<શૂન્ય, E>& o) અચલ
  {
    પાછા is_ok_ == o.is_ok_ && (is_ok_ || err_ == o.err_);
  }

  તાર્કિક operator != (અચલ hb_result_t<શૂન્ય, E>& o) અચલ
  {
    પાછા !(*આ == o);
  }

  તાર્કિક operator == (hb_result_ok_t<શૂન્ય>) અચલ
  {
    પાછા is_ok_;
  }

  તાર્કિક operator != (hb_result_ok_t<શૂન્ય>) અચલ
  {
    પાછા !is_ok_;
  }

  template <typename F>
  તાર્કિક operator == (અચલ hb_result_err_t<F>& e) અચલ
  {
    પાછા !is_ok_ && err_ == e.error;
  }

  template <typename F>
  તાર્કિક operator != (અચલ hb_result_err_t<F>& e) અચલ
  {
    પાછા !(*આ == e);
  }

  template <typename U>
  તાર્કિક operator == (અચલ U&) અચલ = delete;

  template <typename U>
  તાર્કિક operator != (અચલ U&) અચલ = delete;
};

template <typename U, typename T, typename E>
સ્થિર અંતઃરેખ તાર્કિક operator == (અચલ hb_result_ok_t<U>& o, અચલ hb_result_t<T, E>& r)
{
  પાછા r == o;
}
template <typename U, typename T, typename E>
સ્થિર અંતઃરેખ તાર્કિક operator != (અચલ hb_result_ok_t<U>& o, અચલ hb_result_t<T, E>& r)
{
  પાછા r != o;
}
template <typename F, typename T, typename E>
સ્થિર અંતઃરેખ તાર્કિક operator == (અચલ hb_result_err_t<F>& e, અચલ hb_result_t<T, E>& r)
{
  પાછા r == e;
}
template <typename F, typename T, typename E>
સ્થિર અંતઃરેખ તાર્કિક operator != (અચલ hb_result_err_t<F>& e, અચલ hb_result_t<T, E>& r)
{
  પાછા r != e;
}

#ifndef TRY
#define TRY(પદાવલિ)                                                             \
  કરો {                                                                        \
    auto _res = (પદાવલિ);                                                       \
    જો (unlikely (!_res.is_ok())) પાછા Err(std::move(_res).error());        \
  } જ્યારે (0)
#endif

#ifndef TRY_ASSIGN
#define TRY_ASSIGN(var, ...)                                                  \
  auto HB_PASTE(_res_, __LINE__) = (__VA_ARGS__);                             \
  જો (unlikely (!HB_PASTE(_res_, __LINE__).is_ok())) પાછા Err(std::move(HB_PASTE(_res_, __LINE__)).error()); \
  var = *std::move(HB_PASTE(_res_, __LINE__))
#endif

#endif /* HB_RESULT_HH */
