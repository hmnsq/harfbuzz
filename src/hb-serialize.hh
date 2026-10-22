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

#جے_بنا_تعریف HB_SERIALIZE_HH
#تعریف_کرو HB_SERIALIZE_HH

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
  HB_SERIALIZE_ERROR_وتھ_حد_توں_ودھ = 0x00000002u,
  HB_SERIALIZE_ERROR_باہرلا_دا_گنجائش =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_حد_توں_ودھ =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_حد_توں_ودھ =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

بنية hb_serialize_context_t
{
  تعريفنوع بنانشانی objidx_t;

  تعداد whence_t {
     سر,	/* Relative to the current object head (default). */
     پوچھ_2,	/* Relative to the current object tail after packed. */
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
      پوچھ = o.پوچھ;
      next = خالی_سوچک;
      real_links.alloc_exact (o.عدد_real_links);
      لكل (بنانشانی i = 0 ; i < o.عدد_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.عدد_virtual_links);
      لكل (بنانشانی i = 0; i < o.عدد_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    منطقي وچ_error () ثابت
    {
      أرجع real_links.وچ_error() || virtual_links.وچ_error();
    }

    منطقي جوڑو_virtual_link (objidx_t objidx)
    {
      إذا (!objidx)
        أرجع خطأ;

      خودکار& link = *virtual_links.push ();
      إذا (virtual_links.وچ_error ())
        أرجع خطأ;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      أرجع سچ;
    }

    یار فراغ swap (object_t& a, object_t& b) noexcept
    {
      hb_وٹاندرا (a.سر_2, b.سر_2);
      hb_وٹاندرا (a.پوچھ, b.پوچھ);
      hb_وٹاندرا (a.next, b.next);
      hb_وٹاندرا (a.real_links, b.real_links);
      hb_وٹاندرا (a.virtual_links, b.virtual_links);
    }

    منطقي عامل == (ثابت object_t &o) ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع (پوچھ - سر_2 == o.پوچھ - o.سر_2)
	  && (real_links.لمائی == o.real_links.لمائی)
	  && 0 == hb_memcmp (سر_2, o.سر_2, پوچھ - سر_2)
	  && real_links.as_بائٹاں () == o.real_links.as_بائٹاں ();
    }
    uint32_t hash () ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع hb_بائٹاں_t (سر_2, hb_min (128, پوچھ - سر_2)).hash () ^
          real_links.as_بائٹاں ().hash ();
    }

    بنية link_t
    {
      بنانشانی چوڑائی: 3;
      بنانشانی is_signed: 1;
      بنانشانی whence: 2;
      بنانشانی bias : 26;
      بنانشانی position;
      objidx_t objidx;

      link_t () = افتراضي;

      link_t (ثابت hb_subset_serialize_link_t &o)
      {
        چوڑائی = o.چوڑائی;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_اندرونی ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
      {
        صحيح cmp = ((ثابت link_t*)a)->position - ((ثابت link_t*)b)->position;
        إذا (cmp) أرجع cmp;

        أرجع ((ثابت link_t*)a)->objidx - ((ثابت link_t*)b)->objidx;
      }
    };

    حرف *سر_2;
    حرف *پوچھ;
    hb_سمتیہ_t<link_t> real_links;
    hb_سمتیہ_t<link_t> virtual_links;
    object_t *next;

    خودکار all_links () ثابت HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    خودکار all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  بنية snapshot_t
  {
    حرف *سر_2;
    حرف *پوچھ;
    object_t *current; // Just for sanity check
    بنانشانی عدد_real_links;
    بنانشانی عدد_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    أرجع snapshot_t {
      سر_2, پوچھ, current,
      current ? current->real_links.لمائی : 0,
      current ? current->virtual_links.لمائی : 0,
      errors
     };
  }

  hb_serialize_context_t (فراغ *شروع_2, size_t ناپ) :
    شروع ((حرف *) شروع_2),
    سرا (شروع + ناپ),
    current (خالی_سوچک)
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

  منطقي وچ_error () ثابت { أرجع منطقي (errors); }

  منطقي successful () ثابت { أرجع !منطقي (errors); }

  HB_NODISCARD منطقي ran_باہرلا_دا_گنجائش () ثابت { أرجع errors & HB_SERIALIZE_ERROR_باہرلا_دا_گنجائش; }
  HB_NODISCARD منطقي وتھ_حد_توں_ودھ () ثابت { أرجع errors & HB_SERIALIZE_ERROR_وتھ_حد_توں_ودھ; }
  HB_NODISCARD منطقي only_وتھ_حد_توں_ودھ () ثابت { أرجع errors == HB_SERIALIZE_ERROR_وتھ_حد_توں_ودھ; }
  HB_NODISCARD منطقي only_حد_توں_ودھ () ثابت
  {
    أرجع errors == HB_SERIALIZE_ERROR_وتھ_حد_توں_ودھ
        || errors == HB_SERIALIZE_ERROR_INT_حد_توں_ودھ
        || errors == HB_SERIALIZE_ERROR_ARRAY_حد_توں_ودھ;
  }

  فراغ reset (فراغ *شروع_2, size_t ناپ)
  {
    شروع = (حرف*) شروع_2;
    سرا = شروع + ناپ;
    reset ();
    current = خالی_سوچک;
  }

  فراغ reset ()
  {
    هذا->errors = HB_SERIALIZE_ERROR_NONE;
    هذا->سر_2 = هذا->شروع;
    هذا->پوچھ = هذا->سرا;
    هذا->zerocopy = خالی_سوچک;
    هذا->debug_ڈونگھائی = 0;

    fini ();
    هذا->packed.push (خالی_سوچک);
    هذا->packed_map.init ();
  }

  منطقي check_success (منطقي success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    أرجع successful ()
        && (success || err (err_type));
  }

  سانچا <قسم_ناں T1, قسم_ناں T2>
  منطقي check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    إذا ((لما لما) v1 != (لما لما) v2)
    {
      أرجع err (err_type);
    }
    أرجع سچ;
  }

  سانچا <قسم_ناں T1, قسم_ناں T2>
  منطقي check_سونپو (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { أرجع check_equal (v1 = v2, v2, err_type); }

  سانچا <قسم_ناں T> منطقي propagate_error (T &&obj)
  { أرجع check_success (!hb_deref (obj).وچ_error ()); }

  سانچا <قسم_ناں T1, قسم_ناں... Ts> منطقي propagate_error (T1 &&o1, Ts&&... os)
  { أرجع propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  سانچا <قسم_ناں Type=حرف>
  __attribute__((returns_nonnull))
  Type *شروع_serialize ()
  {
    DEBUG_پیغام_LEVEL (SERIALIZE, هذا->شروع, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->شروع, هذا->سرا,
		     (بنانشانی لما) (هذا->سرا - هذا->شروع));

    أكد (!current);
    أرجع push<Type> ();
  }
  فراغ سرا_serialize ()
  {
    DEBUG_پیغام_LEVEL (SERIALIZE, هذا->شروع, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     هذا->شروع, هذا->سرا,
		     (بنانشانی) (هذا->سر_2 - هذا->شروع),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    إذا (unlikely (!current)) أرجع;
    إذا (unlikely (وچ_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      إذا (وتھ_حد_توں_ودھ ()) err (HB_SERIALIZE_ERROR_OTHER);
      أرجع;
    }

    أكد (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    إذا (packed.لمائی <= 1)
      أرجع;

    نکالو_pack (خطأ);

    resolve_links ();
  }

  سانچا <قسم_ناں Type = فراغ>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    إذا (unlikely (وچ_error ())) أرجع شروع_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    إذا (unlikely (!obj))
      check_success (خطأ);
    وإلا
    {
      obj->سر_2 = سر_2;
      obj->پوچھ = پوچھ;
      obj->next = current;
      current = obj;
    }
    أرجع شروع_embed<Type> ();
  }
  فراغ نکالو_discard ()
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (وچ_error() && !only_حد_توں_ودھ ())) أرجع;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->سر_2, obj->پوچھ);
    zerocopy = خالی_سوچک;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t نکالو_pack (منطقي share=سچ)
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (وچ_error()  && !only_حد_توں_ودھ ())) أرجع 0;

    current = current->next;
    obj->پوچھ = سر_2;
    obj->next = خالی_سوچک;
    أكد (obj->سر_2 <= obj->پوچھ);
    بنانشانی len = obj->پوچھ - obj->سر_2;
    سر_2 = zerocopy ? zerocopy : obj->سر_2; /* Rewind head. */
    منطقي was_zerocopy = zerocopy;
    zerocopy = خالی_سوچک;

    إذا (!len)
    {
      أكد (!obj->real_links.لمائی);
      أكد (!obj->virtual_links.لمائی);
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
        ملاؤ_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	أرجع objidx;
      }
    }

    پوچھ -= len;
    إذا (was_zerocopy)
      أكد (پوچھ == obj->سر_2);
    وإلا
      memmove (پوچھ, obj->سر_2, len);

    obj->سر_2 = پوچھ;
    obj->پوچھ = پوچھ + len;

    packed.push (obj);

    إذا (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      أرجع 0;
    }

    objidx = packed.لمائی - 1;

    إذا (share) packed_map.سیٹ_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    أرجع objidx;
  }

  فراغ revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    إذا (unlikely (وچ_error () && !only_حد_توں_ودھ ())) أرجع;
    أكد (snap.current == current);
    إذا (current)
    {
      current->real_links.shrink (snap.عدد_real_links);
      current->virtual_links.shrink (snap.عدد_virtual_links);
    }
    errors = snap.errors;
    revert (snap.سر_2, snap.پوچھ);
  }

  فراغ revert (حرف *snap_سر,
	       حرف *snap_پوچھ)
  {
    إذا (unlikely (وچ_error ())) أرجع;
    أكد (snap_سر <= سر_2);
    أكد (پوچھ <= snap_پوچھ);
    سر_2 = snap_سر;
    پوچھ = snap_پوچھ;
    discard_stale_objects ();
  }

  فراغ discard_stale_objects ()
  {
    إذا (unlikely (وچ_error ())) أرجع;
    طالما (packed.لمائی > 1 &&
	   packed.پوچھ ()->سر_2 < پوچھ)
    {
      object_t *obj = packed.پوچھ ();
      packed_map.del (obj);
      أكد (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.نکالو ();
    }
    إذا (packed.لمائی > 1)
      أكد (packed.پوچھ ()->سر_2 == پوچھ);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  فراغ جوڑو_virtual_link (objidx_t objidx)
  {
    إذا (unlikely (وچ_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);

    إذا (!current->جوڑو_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t آخری_added_بچہ_اشاریہ() ثابت {
    إذا (unlikely (وچ_error ())) أرجع (objidx_t) -1;

    أكد (current);
    إذا (!منطقي(current->real_links)) {
      أرجع (objidx_t) -1;
    }

    أرجع current->real_links[current->real_links.لمائی - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  فراغ repack_آخری(objidx_t objidx) {
    إذا (unlikely (وچ_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    لكل (خودکار& l : current->real_links) {
      إذا (l.objidx == objidx) {
        استمر;
      }

      packed[l.objidx]->جوڑو_virtual_link(objidx);
    }
  }

  سانچا <قسم_ناں T>
  فراغ جوڑو_link (T &ofs, objidx_t objidx,
		 whence_t whence = سر,
		 بنانشانی bias = 0)
  {
    إذا (unlikely (وچ_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) &ofs);

    خودکار& link = *current->real_links.push ();
    إذا (current->real_links.وچ_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.چوڑائی = حجم (T);
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
    link.whence = (بنانشانی) whence;
    link.position = (ثابت حرف *) &ofs - current->سر_2;
    link.bias = bias;
  }

  بنانشانی منزل_bias (ثابت فراغ *ادھار) ثابت
  {
    إذا (unlikely (وچ_error ())) أرجع 0;
    إذا (!ادھار) أرجع 0;
    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) ادھار);
    أرجع (ثابت حرف *) ادھار - current->سر_2;
  }

  فراغ resolve_links ()
  {
    إذا (unlikely (وچ_error ())) أرجع;

    أكد (!current);
    أكد (packed.لمائی > 1);

    لكل (ثابت object_t* ماپا : ++hb_iter (packed))
      لكل (ثابت object_t::link_t &link : ماپا->real_links)
      {
	ثابت object_t* بچہ = packed[link.objidx];
	إذا (unlikely (!بچہ)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع; }
	بنانشانی وتھ = 0;
	بدل ((whence_t) link.whence) {
	حالة سر:     وتھ = بچہ->سر_2 - ماپا->سر_2; اكسر;
	حالة پوچھ_2:     وتھ = بچہ->سر_2 - ماپا->پوچھ; اكسر;
	حالة Absolute: وتھ = (سر_2 - شروع) + (بچہ->سر_2 - پوچھ); اكسر;
	}

	أكد (وتھ >= link.bias);
	وتھ -= link.bias;
	إذا (link.is_signed)
	{
	  أكد (link.چوڑائی == 2 || link.چوڑائی == 4);
	  إذا (link.چوڑائی == 4)
	    سونپو_وتھ<int32_t> (ماپا, link, وتھ);
	  وإلا
	    سونپو_وتھ<int16_t> (ماپا, link, وتھ);
	}
	وإلا
	{
	  أكد (link.چوڑائی == 2 || link.چوڑائی == 3 || link.چوڑائی == 4);
	  إذا (link.چوڑائی == 4)
	    سونپو_وتھ<uint32_t> (ماپا, link, وتھ);
	  وإلا إذا (link.چوڑائی == 3)
	    سونپو_وتھ<uint32_t, 3> (ماپا, link, وتھ);
	  وإلا
	    سونپو_وتھ<uint16_t> (ماپا, link, وتھ);
	}
      }
  }

  بنانشانی صحيح لمائی () ثابت
  {
    إذا (unlikely (!current)) أرجع 0;
    أرجع هذا->سر_2 - current->سر_2;
  }

  فراغ align (بنانشانی صحيح alignment)
  {
    بنانشانی صحيح l = لمائی () % alignment;
    إذا (l)
      (فراغ) allocate_ناپ<فراغ> (alignment - l);
  }

  سانچا <قسم_ناں Type = فراغ>
  __attribute__((returns_nonnull))
  Type *شروع_embed (ثابت Type *obj HB_بن_ورتیا = خالی_سوچک) ثابت
  { أرجع reinterpret_cast<Type *> (هذا->سر_2); }
  سانچا <قسم_ناں Type>
  __attribute__((returns_nonnull))
  Type *شروع_embed (ثابت Type &obj) ثابت
  { أرجع شروع_embed (std::addressof (obj)); }

  منطقي err (hb_serialize_error_t err_type)
  {
    أرجع !منطقي ((errors = (errors | err_type)));
  }

  منطقي شروع_zerocopy (size_t ناپ)
  {
    إذا (unlikely (وچ_error ())) أرجع خطأ;

    إذا (unlikely (ناپ > ودھ_توں_ودھ_عدد || هذا->پوچھ - هذا->سر_2 < ptrdiff_t (ناپ)))
    {
      err (HB_SERIALIZE_ERROR_باہرلا_دا_گنجائش);
      أرجع خطأ;
    }

    أكد (!هذا->zerocopy);
    هذا->zerocopy = هذا->سر_2;

    أكد (هذا->current->سر_2 == هذا->سر_2);
    هذا->current->سر_2 = هذا->current->پوچھ = هذا->سر_2 = هذا->پوچھ - ناپ;
    أرجع سچ;
  }

  سانچا <قسم_ناں Type>
  HB_NODISCARD
  Type *allocate_ناپ (size_t ناپ, منطقي clear = سچ)
  {
    إذا (unlikely (وچ_error ())) أرجع خالی_سوچک;

    إذا (unlikely (ناپ > ودھ_توں_ودھ_عدد || هذا->پوچھ - هذا->سر_2 < ptrdiff_t (ناپ)))
    {
      err (HB_SERIALIZE_ERROR_باہرلا_دا_گنجائش);
      أرجع خالی_سوچک;
    }
    إذا (clear)
      hb_memset (هذا->سر_2, 0, ناپ);
    حرف *ret = هذا->سر_2;
    هذا->سر_2 += ناپ;
    أرجع reinterpret_cast<Type *> (ret);
  }

  سانچا <قسم_ناں Type>
  Type *allocate_min ()
  { أرجع هذا->allocate_ناپ<Type> (Type::min_ناپ); }

  سانچا <قسم_ناں Type>
  HB_NODISCARD
  Type *embed (ثابت Type *obj)
  {
    size_t ناپ = obj->get_ناپ ();
    Type *ret = هذا->allocate_ناپ<Type> (ناپ, خطأ);
    إذا (unlikely (!ret)) أرجع خالی_سوچک;
    hb_memcpy (ret, obj, ناپ);
    أرجع ret;
  }
  سانچا <قسم_ناں Type>
  HB_NODISCARD
  Type *embed (ثابت Type &obj)
  { أرجع embed (std::addressof (obj)); }
  حرف *embed (ثابت حرف *obj, بنانشانی ناپ)
  {
    حرف *ret = هذا->allocate_ناپ<حرف> (ناپ, خطأ);
    إذا (unlikely (!ret)) أرجع خالی_سوچک;
    hb_memcpy (ret, obj, ناپ);
    أرجع ret;
  }

  سانچا <قسم_ناں Type, قسم_ناں ...Ts> خودکار
  _copy (ثابت Type &src, hb_ترجیح<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (هذا, std::forward<Ts> (ds)...))

  سانچا <قسم_ناں Type> خودکار
  _copy (ثابت Type &src, hb_ترجیح<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = هذا->allocate_ناپ<Type> (حجم (Type));
    إذا (unlikely (!ret)) أرجع خالی_سوچک;
    *ret = src;
    أرجع ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  سانچا <قسم_ناں Type, قسم_ناں ...Ts>
  Type *copy (ثابت Type &src, Ts&&... ds)
  { أرجع _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  سانچا <قسم_ناں Type, قسم_ناں ...Ts>
  Type *copy (ثابت Type *src, Ts&&... ds)
  { أرجع copy (*src, std::forward<Ts> (ds)...); }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   قسم_ناں ...Ts>
  فراغ copy_all (Iterator it, Ts&&... ds)
  { لكل (decltype (*it) _ : it) copy (_, ds...); }

  سانچا <قسم_ناں Type>
  hb_serialize_context_t& عامل << (ثابت Type &obj) & { embed (obj); أرجع *هذا; }

  سانچا <قسم_ناں Type>
  Type *extend_ناپ (Type *obj, size_t ناپ, منطقي clear = سچ)
  {
    إذا (unlikely (وچ_error ())) أرجع خالی_سوچک;
    إذا (unlikely (ناپ >= ودھ_توں_ودھ_عدد)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع خالی_سوچک; }

    أكد (هذا->شروع <= (حرف *) obj);
    أكد ((حرف *) obj <= هذا->سر_2);
    أكد ((size_t) (هذا->سر_2 - (حرف *) obj) <= ناپ);
    إذا (unlikely (((حرف *) obj + ناپ < (حرف *) obj) ||
		  !هذا->allocate_ناپ<Type> (((حرف *) obj) + ناپ - هذا->سر_2, clear))) أرجع خالی_سوچک;
    أرجع reinterpret_cast<Type *> (obj);
  }
  سانچا <قسم_ناں Type>
  Type *extend_ناپ (Type &obj, size_t ناپ, منطقي clear = سچ)
  { أرجع extend_ناپ (std::addressof (obj), ناپ, clear); }

  سانچا <قسم_ناں Type>
  Type *extend_min (Type *obj) { أرجع extend_ناپ (obj, obj->min_ناپ); }
  سانچا <قسم_ناں Type>
  Type *extend_min (Type &obj) { أرجع extend_min (std::addressof (obj)); }

  سانچا <قسم_ناں Type, قسم_ناں ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { أرجع extend_ناپ (obj, obj->get_ناپ (std::forward<Ts> (ds)...)); }
  سانچا <قسم_ناں Type, قسم_ناں ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { أرجع extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_بائٹاں_t copy_بائٹاں () ثابت
  {
    أكد (successful ());
    /* Copy both items from head side and tail side... */
    بنانشانی صحيح len = (هذا->سر_2 - هذا->شروع)
		     + (هذا->سرا  - هذا->پوچھ);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    إذا (!len) أرجع hb_بائٹاں_t ();

    حرف *p = (حرف *) hb_malloc (len);
    إذا (unlikely (!p)) أرجع hb_بائٹاں_t ();

    hb_memcpy (p, هذا->شروع, هذا->سر_2 - هذا->شروع);
    hb_memcpy (p + (هذا->سر_2 - هذا->شروع), هذا->پوچھ, هذا->سرا - هذا->پوچھ);
    أرجع hb_بائٹاں_t (p, len);
  }
  سانچا <قسم_ناں Type>
  Type *copy () ثابت
  { أرجع reinterpret_cast<Type *> ((حرف *) copy_بائٹاں ().arrayZ); }
  hb_blob_t *copy_blob () ثابت
  {
    hb_بائٹاں_t b = copy_بائٹاں ();
    أرجع hb_blob_create (b.arrayZ, b.لمائی,
			   HB_حافظہ_منوال_WRITABLE,
			   (حرف *) b.arrayZ, hb_free);
  }

  ثابت hb_سمتیہ_t<object_t *>& object_گراف() ثابت
  { أرجع packed; }

  خاص:
  سانچا <قسم_ناں T, بنانشانی ناپ_2 = حجم (T)>
  فراغ سونپو_وتھ (ثابت object_t* ماپا, ثابت object_t::link_t &link, بنانشانی وتھ)
  {
    // XXX We should stop assuming big-endian!
    خودکار &off = * ((HBInt<سچ, T, ناپ_2> *) (ماپا->سر_2 + link.position));
    أكد (0 == off);
    check_سونپو (off, وتھ, HB_SERIALIZE_ERROR_وتھ_حد_توں_ودھ);
  }

  عام:
  حرف *شروع, *سر_2, *پوچھ, *سرا, *zerocopy;
  بنانشانی صحيح debug_ڈونگھائی;
  hb_serialize_error_t errors;

  خاص:

  فراغ ملاؤ_virtual_links (ثابت object_t* from, objidx_t منزل_idx) {
    object_t* to = packed[منزل_idx];
    لكل (ثابت خودکار& l : from->virtual_links) {
      to->virtual_links.push (l);
    }
  }

  /* Object memory pool. */
  hb_free_pool_t<object_t> object_pool;

  /* Stack of currently under construction objects. */
  object_t *current;

  /* Stack of packed objects.  Object 0 is always nil object. */
  hb_سمتیہ_t<object_t *> packed;

  /* Map view of packed objects. */
  hb_hashmap_t<ثابت object_t *, objidx_t> packed_map;
};

#جے_انت /* HB_SERIALIZE_HH */
