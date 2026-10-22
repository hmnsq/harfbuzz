/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012,2018  Google, Inc.
 * Copyright © 2019  Facebook, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 * Facebook Author(s): Behdad Esfahbod
 */

#اگر_تعریف_نشده HB_SERIALIZE_HH
#تعریف_کن HB_SERIALIZE_HH

#ضمن "hb.hh"
#ضمن "hb-blob.hh"
#ضمن "hb-map.hh"
#ضمن "hb-free-pool.hh"

#ضمن "hb-subset-serialize.h"

/*
 * Serialize
 */

تعداد hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_جابه_جایی_سرریز = 0x00000002u,
  HB_SERIALIZE_ERROR_بیرون_از_جا =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_سرریز =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_سرریز =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

بنية hb_serialize_context_t
{
  تعريفنوع بیعلامت objidx_t;

  تعداد whence_t {
     سر,	/* Relative to the current object head (default). */
     دم_2,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  بنية object_t
  {
    فراغ fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = افتراضي;

    object_t (ثابت hb_subset_serialize_object_t &o)
    {
      سر_2 = o.سر_2;
      دم = o.دم;
      next = اشاره_تهی;
      real_links.alloc_exact (o.رقم_real_links);
      لكل (بیعلامت i = 0 ; i < o.رقم_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.رقم_virtual_links);
      لكل (بیعلامت i = 0; i < o.رقم_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    منطقي درون_error () ثابت
    {
      أرجع real_links.درون_error() || virtual_links.درون_error();
    }

    منطقي جمع_کن_virtual_link (objidx_t objidx)
    {
      إذا (!objidx)
        أرجع خطأ;

      خودکار& link = *virtual_links.push ();
      إذا (virtual_links.درون_error ())
        أرجع خطأ;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      أرجع درست;
    }

    دوست فراغ swap (object_t& a, object_t& b) noexcept
    {
      hb_جابجا (a.سر_2, b.سر_2);
      hb_جابجا (a.دم, b.دم);
      hb_جابجا (a.next, b.next);
      hb_جابجا (a.real_links, b.real_links);
      hb_جابجا (a.virtual_links, b.virtual_links);
    }

    منطقي عملگر == (ثابت object_t &o) ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع (دم - سر_2 == o.دم - o.سر_2)
	  && (real_links.درازی == o.real_links.درازی)
	  && 0 == hb_memcmp (سر_2, o.سر_2, دم - سر_2)
	  && real_links.as_بایتها () == o.real_links.as_بایتها ();
    }
    uint32_t hash () ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع hb_بایتها_t (سر_2, hb_min (128, دم - سر_2)).hash () ^
          real_links.as_بایتها ().hash ();
    }

    بنية link_t
    {
      بیعلامت عرض: 3;
      بیعلامت is_signed: 1;
      بیعلامت whence: 2;
      بیعلامت bias : 26;
      بیعلامت position;
      objidx_t objidx;

      link_t () = افتراضي;

      link_t (ثابت hb_subset_serialize_link_t &o)
      {
        عرض = o.عرض;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_داخلی ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
      {
        صحيح cmp = ((ثابت link_t*)a)->position - ((ثابت link_t*)b)->position;
        إذا (cmp) أرجع cmp;

        أرجع ((ثابت link_t*)a)->objidx - ((ثابت link_t*)b)->objidx;
      }
    };

    حرف *سر_2;
    حرف *دم;
    hb_وکتور_t<link_t> real_links;
    hb_وکتور_t<link_t> virtual_links;
    object_t *next;

    خودکار all_links () ثابت HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    خودکار all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  بنية snapshot_t
  {
    حرف *سر_2;
    حرف *دم;
    object_t *current; // Just for sanity check
    بیعلامت رقم_real_links;
    بیعلامت رقم_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    أرجع snapshot_t {
      سر_2, دم, current,
      current ? current->real_links.درازی : 0,
      current ? current->virtual_links.درازی : 0,
      errors
     };
  }

  hb_serialize_context_t (فراغ *شروع_2, size_t اندازه) :
    شروع ((حرف *) شروع_2),
    انتها (شروع + اندازه),
    current (اشاره_تهی)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  فراغ fini ()
  {
    لكل (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    هذا->packed_map.fini ();

    طالما (current)
    {
      خودکار *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  منطقي درون_error () ثابت { أرجع منطقي (errors); }

  منطقي successful () ثابت { أرجع !منطقي (errors); }

  HB_NODISCARD منطقي ran_بیرون_از_جا () ثابت { أرجع errors & HB_SERIALIZE_ERROR_بیرون_از_جا; }
  HB_NODISCARD منطقي جابه_جایی_سرریز () ثابت { أرجع errors & HB_SERIALIZE_ERROR_جابه_جایی_سرریز; }
  HB_NODISCARD منطقي only_جابه_جایی_سرریز () ثابت { أرجع errors == HB_SERIALIZE_ERROR_جابه_جایی_سرریز; }
  HB_NODISCARD منطقي only_سرریز () ثابت
  {
    أرجع errors == HB_SERIALIZE_ERROR_جابه_جایی_سرریز
        || errors == HB_SERIALIZE_ERROR_INT_سرریز
        || errors == HB_SERIALIZE_ERROR_ARRAY_سرریز;
  }

  فراغ reset (فراغ *شروع_2, size_t اندازه)
  {
    شروع = (حرف*) شروع_2;
    انتها = شروع + اندازه;
    reset ();
    current = اشاره_تهی;
  }

  فراغ reset ()
  {
    هذا->errors = HB_SERIALIZE_ERROR_NONE;
    هذا->سر_2 = هذا->شروع;
    هذا->دم = هذا->انتها;
    هذا->zerocopy = اشاره_تهی;
    هذا->debug_عمق = 0;

    fini ();
    هذا->packed.push (اشاره_تهی);
    هذا->packed_map.init ();
  }

  منطقي check_success (منطقي success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    أرجع successful ()
        && (success || err (err_type));
  }

  قالب <نام_نوع T1, نام_نوع T2>
  منطقي check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    إذا ((دراز دراز) v1 != (دراز دراز) v2)
    {
      أرجع err (err_type);
    }
    أرجع درست;
  }

  قالب <نام_نوع T1, نام_نوع T2>
  منطقي check_تخصیص_بده (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { أرجع check_equal (v1 = v2, v2, err_type); }

  قالب <نام_نوع T> منطقي propagate_error (T &&obj)
  { أرجع check_success (!hb_deref (obj).درون_error ()); }

  قالب <نام_نوع T1, نام_نوع... Ts> منطقي propagate_error (T1 &&o1, Ts&&... os)
  { أرجع propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  قالب <نام_نوع Type=حرف>
  __attribute__((returns_nonnull))
  Type *شروع_serialize ()
  {
    DEBUG_پیام_LEVEL (SERIALIZE, هذا->شروع, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->شروع, هذا->انتها,
		     (بیعلامت دراز) (هذا->انتها - هذا->شروع));

    أكد (!current);
    أرجع push<Type> ();
  }
  فراغ انتها_serialize ()
  {
    DEBUG_پیام_LEVEL (SERIALIZE, هذا->شروع, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     هذا->شروع, هذا->انتها,
		     (بیعلامت) (هذا->سر_2 - هذا->شروع),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    إذا (unlikely (!current)) أرجع;
    إذا (unlikely (درون_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      إذا (جابه_جایی_سرریز ()) err (HB_SERIALIZE_ERROR_OTHER);
      أرجع;
    }

    أكد (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    إذا (packed.درازی <= 1)
      أرجع;

    برداشت_pack (خطأ);

    resolve_links ();
  }

  قالب <نام_نوع Type = فراغ>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    إذا (unlikely (درون_error ())) أرجع شروع_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    إذا (unlikely (!obj))
      check_success (خطأ);
    وإلا
    {
      obj->سر_2 = سر_2;
      obj->دم = دم;
      obj->next = current;
      current = obj;
    }
    أرجع شروع_embed<Type> ();
  }
  فراغ برداشت_discard ()
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (درون_error() && !only_سرریز ())) أرجع;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->سر_2, obj->دم);
    zerocopy = اشاره_تهی;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t برداشت_pack (منطقي share=درست)
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (درون_error()  && !only_سرریز ())) أرجع 0;

    current = current->next;
    obj->دم = سر_2;
    obj->next = اشاره_تهی;
    أكد (obj->سر_2 <= obj->دم);
    بیعلامت len = obj->دم - obj->سر_2;
    سر_2 = zerocopy ? zerocopy : obj->سر_2; /* Rewind head. */
    منطقي was_zerocopy = zerocopy;
    zerocopy = اشاره_تهی;

    إذا (!len)
    {
      أكد (!obj->real_links.درازی);
      أكد (!obj->virtual_links.درازی);
      أرجع 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    إذا (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      إذا (objidx)
      {
        ادغام_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	أرجع objidx;
      }
    }

    دم -= len;
    إذا (was_zerocopy)
      أكد (دم == obj->سر_2);
    وإلا
      memmove (دم, obj->سر_2, len);

    obj->سر_2 = دم;
    obj->دم = دم + len;

    packed.push (obj);

    إذا (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      أرجع 0;
    }

    objidx = packed.درازی - 1;

    إذا (share) packed_map.مجموعه_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    أرجع objidx;
  }

  فراغ revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    إذا (unlikely (درون_error () && !only_سرریز ())) أرجع;
    أكد (snap.current == current);
    إذا (current)
    {
      current->real_links.shrink (snap.رقم_real_links);
      current->virtual_links.shrink (snap.رقم_virtual_links);
    }
    errors = snap.errors;
    revert (snap.سر_2, snap.دم);
  }

  فراغ revert (حرف *snap_سر,
	       حرف *snap_دم)
  {
    إذا (unlikely (درون_error ())) أرجع;
    أكد (snap_سر <= سر_2);
    أكد (دم <= snap_دم);
    سر_2 = snap_سر;
    دم = snap_دم;
    discard_stale_objects ();
  }

  فراغ discard_stale_objects ()
  {
    إذا (unlikely (درون_error ())) أرجع;
    طالما (packed.درازی > 1 &&
	   packed.دم ()->سر_2 < دم)
    {
      object_t *obj = packed.دم ();
      packed_map.del (obj);
      أكد (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.برداشت ();
    }
    إذا (packed.درازی > 1)
      أكد (packed.دم ()->سر_2 == دم);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  فراغ جمع_کن_virtual_link (objidx_t objidx)
  {
    إذا (unlikely (درون_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);

    إذا (!current->جمع_کن_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t آخرین_added_فرزند_شاخص() ثابت {
    إذا (unlikely (درون_error ())) أرجع (objidx_t) -1;

    أكد (current);
    إذا (!منطقي(current->real_links)) {
      أرجع (objidx_t) -1;
    }

    أرجع current->real_links[current->real_links.درازی - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  فراغ repack_آخرین(objidx_t objidx) {
    إذا (unlikely (درون_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    لكل (خودکار& l : current->real_links) {
      إذا (l.objidx == objidx) {
        استمر;
      }

      packed[l.objidx]->جمع_کن_virtual_link(objidx);
    }
  }

  قالب <نام_نوع T>
  فراغ جمع_کن_link (T &ofs, objidx_t objidx,
		 whence_t whence = سر,
		 بیعلامت bias = 0)
  {
    إذا (unlikely (درون_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) &ofs);

    خودکار& link = *current->real_links.push ();
    إذا (current->real_links.درون_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.عرض = حجم (T);
    link.objidx = objidx;
    إذا (unlikely (!حجم (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      أرجع;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (بیعلامت) whence;
    link.position = (ثابت حرف *) &ofs - current->سر_2;
    link.bias = bias;
  }

  بیعلامت مقصد_bias (ثابت فراغ *پایه) ثابت
  {
    إذا (unlikely (درون_error ())) أرجع 0;
    إذا (!پایه) أرجع 0;
    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) پایه);
    أرجع (ثابت حرف *) پایه - current->سر_2;
  }

  فراغ resolve_links ()
  {
    إذا (unlikely (درون_error ())) أرجع;

    أكد (!current);
    أكد (packed.درازی > 1);

    لكل (ثابت object_t* والد : ++hb_iter (packed))
      لكل (ثابت object_t::link_t &link : والد->real_links)
      {
	ثابت object_t* فرزند = packed[link.objidx];
	إذا (unlikely (!فرزند)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع; }
	بیعلامت جابه_جایی = 0;
	بدل ((whence_t) link.whence) {
	حالة سر:     جابه_جایی = فرزند->سر_2 - والد->سر_2; اكسر;
	حالة دم_2:     جابه_جایی = فرزند->سر_2 - والد->دم; اكسر;
	حالة Absolute: جابه_جایی = (سر_2 - شروع) + (فرزند->سر_2 - دم); اكسر;
	}

	أكد (جابه_جایی >= link.bias);
	جابه_جایی -= link.bias;
	إذا (link.is_signed)
	{
	  أكد (link.عرض == 2 || link.عرض == 4);
	  إذا (link.عرض == 4)
	    تخصیص_بده_جابه_جایی<int32_t> (والد, link, جابه_جایی);
	  وإلا
	    تخصیص_بده_جابه_جایی<int16_t> (والد, link, جابه_جایی);
	}
	وإلا
	{
	  أكد (link.عرض == 2 || link.عرض == 3 || link.عرض == 4);
	  إذا (link.عرض == 4)
	    تخصیص_بده_جابه_جایی<uint32_t> (والد, link, جابه_جایی);
	  وإلا إذا (link.عرض == 3)
	    تخصیص_بده_جابه_جایی<uint32_t, 3> (والد, link, جابه_جایی);
	  وإلا
	    تخصیص_بده_جابه_جایی<uint16_t> (والد, link, جابه_جایی);
	}
      }
  }

  بیعلامت صحيح درازی () ثابت
  {
    إذا (unlikely (!current)) أرجع 0;
    أرجع هذا->سر_2 - current->سر_2;
  }

  فراغ align (بیعلامت صحيح alignment)
  {
    بیعلامت صحيح l = درازی () % alignment;
    إذا (l)
      (فراغ) allocate_اندازه<فراغ> (alignment - l);
  }

  قالب <نام_نوع Type = فراغ>
  __attribute__((returns_nonnull))
  Type *شروع_embed (ثابت Type *obj HB_استفادهنشده = اشاره_تهی) ثابت
  { أرجع reinterpret_cast<Type *> (هذا->سر_2); }
  قالب <نام_نوع Type>
  __attribute__((returns_nonnull))
  Type *شروع_embed (ثابت Type &obj) ثابت
  { أرجع شروع_embed (std::addressof (obj)); }

  منطقي err (hb_serialize_error_t err_type)
  {
    أرجع !منطقي ((errors = (errors | err_type)));
  }

  منطقي شروع_zerocopy (size_t اندازه)
  {
    إذا (unlikely (درون_error ())) أرجع خطأ;

    إذا (unlikely (اندازه > بیشترین_صحیح || هذا->دم - هذا->سر_2 < ptrdiff_t (اندازه)))
    {
      err (HB_SERIALIZE_ERROR_بیرون_از_جا);
      أرجع خطأ;
    }

    أكد (!هذا->zerocopy);
    هذا->zerocopy = هذا->سر_2;

    أكد (هذا->current->سر_2 == هذا->سر_2);
    هذا->current->سر_2 = هذا->current->دم = هذا->سر_2 = هذا->دم - اندازه;
    أرجع درست;
  }

  قالب <نام_نوع Type>
  HB_NODISCARD
  Type *allocate_اندازه (size_t اندازه, منطقي clear = درست)
  {
    إذا (unlikely (درون_error ())) أرجع اشاره_تهی;

    إذا (unlikely (اندازه > بیشترین_صحیح || هذا->دم - هذا->سر_2 < ptrdiff_t (اندازه)))
    {
      err (HB_SERIALIZE_ERROR_بیرون_از_جا);
      أرجع اشاره_تهی;
    }
    إذا (clear)
      hb_memset (هذا->سر_2, 0, اندازه);
    حرف *ret = هذا->سر_2;
    هذا->سر_2 += اندازه;
    أرجع reinterpret_cast<Type *> (ret);
  }

  قالب <نام_نوع Type>
  Type *allocate_min ()
  { أرجع هذا->allocate_اندازه<Type> (Type::min_اندازه); }

  قالب <نام_نوع Type>
  HB_NODISCARD
  Type *embed (ثابت Type *obj)
  {
    size_t اندازه = obj->get_اندازه ();
    Type *ret = هذا->allocate_اندازه<Type> (اندازه, خطأ);
    إذا (unlikely (!ret)) أرجع اشاره_تهی;
    hb_memcpy (ret, obj, اندازه);
    أرجع ret;
  }
  قالب <نام_نوع Type>
  HB_NODISCARD
  Type *embed (ثابت Type &obj)
  { أرجع embed (std::addressof (obj)); }
  حرف *embed (ثابت حرف *obj, بیعلامت اندازه)
  {
    حرف *ret = هذا->allocate_اندازه<حرف> (اندازه, خطأ);
    إذا (unlikely (!ret)) أرجع اشاره_تهی;
    hb_memcpy (ret, obj, اندازه);
    أرجع ret;
  }

  قالب <نام_نوع Type, نام_نوع ...Ts> خودکار
  _copy (ثابت Type &src, hb_اولویت<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (هذا, std::forward<Ts> (ds)...))

  قالب <نام_نوع Type> خودکار
  _copy (ثابت Type &src, hb_اولویت<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = هذا->allocate_اندازه<Type> (حجم (Type));
    إذا (unlikely (!ret)) أرجع اشاره_تهی;
    *ret = src;
    أرجع ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  قالب <نام_نوع Type, نام_نوع ...Ts>
  Type *copy (ثابت Type &src, Ts&&... ds)
  { أرجع _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  قالب <نام_نوع Type, نام_نوع ...Ts>
  Type *copy (ثابت Type *src, Ts&&... ds)
  { أرجع copy (*src, std::forward<Ts> (ds)...); }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   نام_نوع ...Ts>
  فراغ copy_all (Iterator it, Ts&&... ds)
  { لكل (decltype (*it) _ : it) copy (_, ds...); }

  قالب <نام_نوع Type>
  hb_serialize_context_t& عملگر << (ثابت Type &obj) & { embed (obj); أرجع *هذا; }

  قالب <نام_نوع Type>
  Type *extend_اندازه (Type *obj, size_t اندازه, منطقي clear = درست)
  {
    إذا (unlikely (درون_error ())) أرجع اشاره_تهی;
    إذا (unlikely (اندازه >= بیشترین_صحیح)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع اشاره_تهی; }

    أكد (هذا->شروع <= (حرف *) obj);
    أكد ((حرف *) obj <= هذا->سر_2);
    أكد ((size_t) (هذا->سر_2 - (حرف *) obj) <= اندازه);
    إذا (unlikely (((حرف *) obj + اندازه < (حرف *) obj) ||
		  !هذا->allocate_اندازه<Type> (((حرف *) obj) + اندازه - هذا->سر_2, clear))) أرجع اشاره_تهی;
    أرجع reinterpret_cast<Type *> (obj);
  }
  قالب <نام_نوع Type>
  Type *extend_اندازه (Type &obj, size_t اندازه, منطقي clear = درست)
  { أرجع extend_اندازه (std::addressof (obj), اندازه, clear); }

  قالب <نام_نوع Type>
  Type *extend_min (Type *obj) { أرجع extend_اندازه (obj, obj->min_اندازه); }
  قالب <نام_نوع Type>
  Type *extend_min (Type &obj) { أرجع extend_min (std::addressof (obj)); }

  قالب <نام_نوع Type, نام_نوع ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { أرجع extend_اندازه (obj, obj->get_اندازه (std::forward<Ts> (ds)...)); }
  قالب <نام_نوع Type, نام_نوع ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { أرجع extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_بایتها_t copy_بایتها () ثابت
  {
    أكد (successful ());
    /* Copy both items from head side and tail side... */
    بیعلامت صحيح len = (هذا->سر_2 - هذا->شروع)
		     + (هذا->انتها  - هذا->دم);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    إذا (!len) أرجع hb_بایتها_t ();

    حرف *p = (حرف *) hb_malloc (len);
    إذا (unlikely (!p)) أرجع hb_بایتها_t ();

    hb_memcpy (p, هذا->شروع, هذا->سر_2 - هذا->شروع);
    hb_memcpy (p + (هذا->سر_2 - هذا->شروع), هذا->دم, هذا->انتها - هذا->دم);
    أرجع hb_بایتها_t (p, len);
  }
  قالب <نام_نوع Type>
  Type *copy () ثابت
  { أرجع reinterpret_cast<Type *> ((حرف *) copy_بایتها ().arrayZ); }
  hb_blob_t *copy_blob () ثابت
  {
    hb_بایتها_t b = copy_بایتها ();
    أرجع hb_blob_create (b.arrayZ, b.درازی,
			   HB_حافظه_نما_WRITABLE,
			   (حرف *) b.arrayZ, hb_free);
  }

  ثابت hb_وکتور_t<object_t *>& object_گراف() ثابت
  { أرجع packed; }

  خاص:
  قالب <نام_نوع T, بیعلامت اندازه_2 = حجم (T)>
  فراغ تخصیص_بده_جابه_جایی (ثابت object_t* والد, ثابت object_t::link_t &link, بیعلامت جابه_جایی)
  {
    // XXX We should stop assuming big-endian!
    خودکار &off = * ((HBInt<درست, T, اندازه_2> *) (والد->سر_2 + link.position));
    أكد (0 == off);
    check_تخصیص_بده (off, جابه_جایی, HB_SERIALIZE_ERROR_جابه_جایی_سرریز);
  }

  عام:
  حرف *شروع, *سر_2, *دم, *انتها, *zerocopy;
  بیعلامت صحيح debug_عمق;
  hb_serialize_error_t errors;

  خاص:

  فراغ ادغام_virtual_links (ثابت object_t* from, objidx_t مقصد_idx) {
    object_t* to = packed[مقصد_idx];
    لكل (ثابت خودکار& l : from->virtual_links) {
      to->virtual_links.push (l);
    }
  }

  /* Object memory pool. */
  hb_free_pool_t<object_t> object_pool;

  /* Stack of currently under construction objects. */
  object_t *current;

  /* Stack of packed objects.  Object 0 is always nil object. */
  hb_وکتور_t<object_t *> packed;

  /* Map view of packed objects. */
  hb_hashmap_t<ثابت object_t *, objidx_t> packed_map;
};

#پایان_اگر /* HB_SERIALIZE_HH */
