/*
 * Copyright © 2020  Google, Inc.
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

#اگر_غیر_تعریف_شدہ HB_ترجیح_قطار_HH
#تعریف_کرو HB_ترجیح_قطار_HH

#شامل "hb.hh"
#شامل "hb-vector.hh"

/*
 * hb_priority_queue_t
 *
 * Priority queue implemented as a binary heap. Supports extract minimum
 * and insert operations.
 *
 * The priority queue is implemented as a binary heap, which is a complete
 * binary tree. The root of the tree is the minimum element. The heap
 * property is that the priority of a node is less than or equal to the
 * priority of its children. The heap is stored in an array, with the
 * children of node i stored at indices 2i + 1 and 2i + 2.
 */
سانچہ <قسم_نام K>
ساخت hb_ترجیح_قطار_t
{
 عوامی:
  قسمتعریف hb_pair_t<K, غیرعلامتی> شے_t;

 نجی:
  hb_سمتیہ_t<شے_t> ڈھیر_2;

 عوامی:

  hb_ترجیح_قطار_t () = طےشدہ;
  hb_ترجیح_قطار_t (hb_سمتیہ_t<شے_t>&& other) : ڈھیر_2 (std::move (other))
  {
    // Heapify the vector.
    برائے (صحیح i = (ڈھیر_2.لمبائی / 2) - 1; i >= 0; i--)
      bubble_نیچے_کو (i);
  }

  خالی reset () { ڈھیر_2.clear (); }

  منطقی اندر_error () ثابت { واپس ڈھیر_2.اندر_error (); }

  منطقی alloc (غیرعلامتی سائز)
  { واپس ڈھیر_2.alloc (سائز); }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  خالی insert (K ترجیح, غیرعلامتی value)
  {
    ڈھیر_2.push (شے_t (ترجیح, value));
    اگر (unlikely (ڈھیر_2.اندر_error ())) واپس;
    bubble_اوپر (ڈھیر_2.لمبائی - 1);
  }

#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
  HB_ALWAYS_INLINE
#اگر_ختم
  شے_t نکالو_minimum ()
  {
    أكد (!is_empty ());

    شے_t نتیجہ = ڈھیر_2.arrayZ[0];

    ڈھیر_2.arrayZ[0] = ڈھیر_2.arrayZ[ڈھیر_2.لمبائی - 1];
    ڈھیر_2.resize (ڈھیر_2.لمبائی - 1);

    اگر (!is_empty ())
      bubble_نیچے_کو (0);

    واپس نتیجہ;
  }

  ثابت شے_t& minimum ()
  {
    واپس ڈھیر_2[0];
  }

  منطقی is_empty () ثابت { واپس ڈھیر_2.لمبائی == 0; }
  صریح عامل منطقی () ثابت { واپس !is_empty (); }
  غیرعلامتی صحیح get_population () ثابت { واپس ڈھیر_2.لمبائی; }

  /* Sink interface. */
  hb_ترجیح_قطار_t& عامل << (شے_t شے)
  { insert (شے.پہلا, شے.second); واپس *یہ; }

 نجی:

  ساکن constexpr غیرعلامتی والد (غیرعلامتی اشاریہ_2)
  {
    واپس (اشاریہ_2 - 1) / 2;
  }

  ساکن constexpr غیرعلامتی بایاں_بچہ (غیرعلامتی اشاریہ_2)
  {
    واپس 2 * اشاریہ_2 + 1;
  }

  ساکن constexpr غیرعلامتی دایاں_بچہ (غیرعلامتی اشاریہ_2)
  {
    واپس 2 * اشاریہ_2 + 2;
  }

  HB_ALWAYS_INLINE
  خالی bubble_نیچے_کو (غیرعلامتی اشاریہ_2)
  {
    repeat:
    أكد (اشاریہ_2 < ڈھیر_2.لمبائی);

    غیرعلامتی بائیں = بایاں_بچہ (اشاریہ_2);
    غیرعلامتی دائیں = دایاں_بچہ (اشاریہ_2);

    منطقی has_بایاں = بائیں < ڈھیر_2.لمبائی;
    اگر (!has_بایاں)
      // If there's no left, then there's also no right.
      واپس;

    منطقی has_دایاں = دائیں < ڈھیر_2.لمبائی;
    اگر (ڈھیر_2.arrayZ[اشاریہ_2].پہلا <= ڈھیر_2.arrayZ[بائیں].پہلا
        && (!has_دایاں || ڈھیر_2.arrayZ[اشاریہ_2].پہلا <= ڈھیر_2.arrayZ[دائیں].پہلا))
      واپس;

    غیرعلامتی بچہ;
    اگر (!has_دایاں || ڈھیر_2.arrayZ[بائیں].پہلا < ڈھیر_2.arrayZ[دائیں].پہلا)
      بچہ = بائیں;
    ورنہ
      بچہ = دائیں;

    swap (اشاریہ_2, بچہ);
    اشاریہ_2 = بچہ;
    جاؤ repeat;
  }

  HB_ALWAYS_INLINE
  خالی bubble_اوپر (غیرعلامتی اشاریہ_2)
  {
    repeat:
    أكد (اشاریہ_2 < ڈھیر_2.لمبائی);

    اگر (اشاریہ_2 == 0) واپس;

    غیرعلامتی والد_اشاریہ = والد (اشاریہ_2);
    اگر (ڈھیر_2.arrayZ[والد_اشاریہ].پہلا <= ڈھیر_2.arrayZ[اشاریہ_2].پہلا)
      واپس;

    swap (اشاریہ_2, والد_اشاریہ);
    اشاریہ_2 = والد_اشاریہ;
    جاؤ repeat;
  }

  خالی swap (غیرعلامتی a, غیرعلامتی b) noexcept
  {
    أكد (a < ڈھیر_2.لمبائی);
    أكد (b < ڈھیر_2.لمبائی);
    hb_تبادلہ (ڈھیر_2.arrayZ[a], ڈھیر_2.arrayZ[b]);
  }
};

#اگر_ختم /* HB_PRIORITY_QUEUE_HH */
