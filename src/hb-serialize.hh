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

#که_نه_تعریف_شوی HB_SERIALIZE_HH
#تعریف_کړه HB_SERIALIZE_HH

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
  HB_SERIALIZE_ERROR_دننه_واټن_ډکېدنه = 0x00000002u,
  HB_SERIALIZE_ERROR_بهر_د_ځای =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_ډکېدنه =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_ډکېدنه =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

بنية hb_serialize_context_t
{
  تعريفنوع بینښه objidx_t;

  تعداد whence_t {
     سر,	/* Relative to the current object head (default). */
     لکۍ_2,	/* Relative to the current object tail after packed. */
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
      لکۍ = o.لکۍ;
      next = تش_اشاره;
      real_links.alloc_exact (o.عدد_real_links);
      لكل (بینښه i = 0 ; i < o.عدد_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.عدد_virtual_links);
      لكل (بینښه i = 0; i < o.عدد_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    منطقي دننه_error () ثابت
    {
      أرجع real_links.دننه_error() || virtual_links.دننه_error();
    }

    منطقي جمع_کړه_virtual_link (objidx_t objidx)
    {
      إذا (!objidx)
        أرجع خطأ;

      خپلکاری& link = *virtual_links.push ();
      إذا (virtual_links.دننه_error ())
        أرجع خطأ;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      أرجع ریښتیا;
    }

    ملګری فراغ swap (object_t& a, object_t& b) noexcept
    {
      hb_بدلون (a.سر_2, b.سر_2);
      hb_بدلون (a.لکۍ, b.لکۍ);
      hb_بدلون (a.next, b.next);
      hb_بدلون (a.real_links, b.real_links);
      hb_بدلون (a.virtual_links, b.virtual_links);
    }

    منطقي چلوونکی == (ثابت object_t &o) ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع (لکۍ - سر_2 == o.لکۍ - o.سر_2)
	  && (real_links.اوږدتیا == o.real_links.اوږدتیا)
	  && 0 == hb_memcmp (سر_2, o.سر_2, لکۍ - سر_2)
	  && real_links.as_بایټونه () == o.real_links.as_بایټونه ();
    }
    uint32_t hash () ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع hb_بایټونه_t (سر_2, hb_min (128, لکۍ - سر_2)).hash () ^
          real_links.as_بایټونه ().hash ();
    }

    بنية link_t
    {
      بینښه پلنوالی: 3;
      بینښه is_signed: 1;
      بینښه whence: 2;
      بینښه bias : 26;
      بینښه position;
      objidx_t objidx;

      link_t () = افتراضي;

      link_t (ثابت hb_subset_serialize_link_t &o)
      {
        پلنوالی = o.پلنوالی;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_دننی ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
      {
        صحيح cmp = ((ثابت link_t*)a)->position - ((ثابت link_t*)b)->position;
        إذا (cmp) أرجع cmp;

        أرجع ((ثابت link_t*)a)->objidx - ((ثابت link_t*)b)->objidx;
      }
    };

    حرف *سر_2;
    حرف *لکۍ;
    hb_ویکټور_t<link_t> real_links;
    hb_ویکټور_t<link_t> virtual_links;
    object_t *next;

    خپلکاری all_links () ثابت HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    خپلکاری all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  بنية snapshot_t
  {
    حرف *سر_2;
    حرف *لکۍ;
    object_t *current; // Just for sanity check
    بینښه عدد_real_links;
    بینښه عدد_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    أرجع snapshot_t {
      سر_2, لکۍ, current,
      current ? current->real_links.اوږدتیا : 0,
      current ? current->virtual_links.اوږدتیا : 0,
      errors
     };
  }

  hb_serialize_context_t (فراغ *پیل_2, size_t اندازه) :
    پیل ((حرف *) پیل_2),
    پای_ټکی (پیل + اندازه),
    current (تش_اشاره)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  فراغ fini ()
  {
    لكل (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    هذا->packed_map.fini ();

    طالما (current)
    {
      خپلکاری *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  منطقي دننه_error () ثابت { أرجع منطقي (errors); }

  منطقي successful () ثابت { أرجع !منطقي (errors); }

  HB_NODISCARD منطقي ran_بهر_د_ځای () ثابت { أرجع errors & HB_SERIALIZE_ERROR_بهر_د_ځای; }
  HB_NODISCARD منطقي دننه_واټن_ډکېدنه () ثابت { أرجع errors & HB_SERIALIZE_ERROR_دننه_واټن_ډکېدنه; }
  HB_NODISCARD منطقي only_دننه_واټن_ډکېدنه () ثابت { أرجع errors == HB_SERIALIZE_ERROR_دننه_واټن_ډکېدنه; }
  HB_NODISCARD منطقي only_ډکېدنه () ثابت
  {
    أرجع errors == HB_SERIALIZE_ERROR_دننه_واټن_ډکېدنه
        || errors == HB_SERIALIZE_ERROR_INT_ډکېدنه
        || errors == HB_SERIALIZE_ERROR_ARRAY_ډکېدنه;
  }

  فراغ reset (فراغ *پیل_2, size_t اندازه)
  {
    پیل = (حرف*) پیل_2;
    پای_ټکی = پیل + اندازه;
    reset ();
    current = تش_اشاره;
  }

  فراغ reset ()
  {
    هذا->errors = HB_SERIALIZE_ERROR_NONE;
    هذا->سر_2 = هذا->پیل;
    هذا->لکۍ = هذا->پای_ټکی;
    هذا->zerocopy = تش_اشاره;
    هذا->debug_ژوروالی = 0;

    fini ();
    هذا->packed.push (تش_اشاره);
    هذا->packed_map.init ();
  }

  منطقي check_success (منطقي success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    أرجع successful ()
        && (success || err (err_type));
  }

  کينډۍ <ډول_نوم T1, ډول_نوم T2>
  منطقي check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    إذا ((اوږد اوږد) v1 != (اوږد اوږد) v2)
    {
      أرجع err (err_type);
    }
    أرجع ریښتیا;
  }

  کينډۍ <ډول_نوم T1, ډول_نوم T2>
  منطقي check_ټاکل (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { أرجع check_equal (v1 = v2, v2, err_type); }

  کينډۍ <ډول_نوم T> منطقي propagate_error (T &&obj)
  { أرجع check_success (!hb_deref (obj).دننه_error ()); }

  کينډۍ <ډول_نوم T1, ډول_نوم... Ts> منطقي propagate_error (T1 &&o1, Ts&&... os)
  { أرجع propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  کينډۍ <ډول_نوم Type=حرف>
  __attribute__((returns_nonnull))
  Type *پیل_serialize ()
  {
    DEBUG_پیغام_LEVEL (SERIALIZE, هذا->پیل, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->پیل, هذا->پای_ټکی,
		     (بینښه اوږد) (هذا->پای_ټکی - هذا->پیل));

    أكد (!current);
    أرجع push<Type> ();
  }
  فراغ پای_ټکی_serialize ()
  {
    DEBUG_پیغام_LEVEL (SERIALIZE, هذا->پیل, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     هذا->پیل, هذا->پای_ټکی,
		     (بینښه) (هذا->سر_2 - هذا->پیل),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    إذا (unlikely (!current)) أرجع;
    إذا (unlikely (دننه_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      إذا (دننه_واټن_ډکېدنه ()) err (HB_SERIALIZE_ERROR_OTHER);
      أرجع;
    }

    أكد (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    إذا (packed.اوږدتیا <= 1)
      أرجع;

    راوباسه_pack (خطأ);

    resolve_links ();
  }

  کينډۍ <ډول_نوم Type = فراغ>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    إذا (unlikely (دننه_error ())) أرجع پیل_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    إذا (unlikely (!obj))
      check_success (خطأ);
    وإلا
    {
      obj->سر_2 = سر_2;
      obj->لکۍ = لکۍ;
      obj->next = current;
      current = obj;
    }
    أرجع پیل_embed<Type> ();
  }
  فراغ راوباسه_discard ()
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (دننه_error() && !only_ډکېدنه ())) أرجع;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->سر_2, obj->لکۍ);
    zerocopy = تش_اشاره;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t راوباسه_pack (منطقي share=ریښتیا)
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (دننه_error()  && !only_ډکېدنه ())) أرجع 0;

    current = current->next;
    obj->لکۍ = سر_2;
    obj->next = تش_اشاره;
    أكد (obj->سر_2 <= obj->لکۍ);
    بینښه len = obj->لکۍ - obj->سر_2;
    سر_2 = zerocopy ? zerocopy : obj->سر_2; /* Rewind head. */
    منطقي was_zerocopy = zerocopy;
    zerocopy = تش_اشاره;

    إذا (!len)
    {
      أكد (!obj->real_links.اوږدتیا);
      أكد (!obj->virtual_links.اوږدتیا);
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
        یوځای_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	أرجع objidx;
      }
    }

    لکۍ -= len;
    إذا (was_zerocopy)
      أكد (لکۍ == obj->سر_2);
    وإلا
      memmove (لکۍ, obj->سر_2, len);

    obj->سر_2 = لکۍ;
    obj->لکۍ = لکۍ + len;

    packed.push (obj);

    إذا (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      أرجع 0;
    }

    objidx = packed.اوږدتیا - 1;

    إذا (share) packed_map.سېټ_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    أرجع objidx;
  }

  فراغ revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    إذا (unlikely (دننه_error () && !only_ډکېدنه ())) أرجع;
    أكد (snap.current == current);
    إذا (current)
    {
      current->real_links.shrink (snap.عدد_real_links);
      current->virtual_links.shrink (snap.عدد_virtual_links);
    }
    errors = snap.errors;
    revert (snap.سر_2, snap.لکۍ);
  }

  فراغ revert (حرف *snap_سر,
	       حرف *snap_لکۍ)
  {
    إذا (unlikely (دننه_error ())) أرجع;
    أكد (snap_سر <= سر_2);
    أكد (لکۍ <= snap_لکۍ);
    سر_2 = snap_سر;
    لکۍ = snap_لکۍ;
    discard_stale_objects ();
  }

  فراغ discard_stale_objects ()
  {
    إذا (unlikely (دننه_error ())) أرجع;
    طالما (packed.اوږدتیا > 1 &&
	   packed.لکۍ ()->سر_2 < لکۍ)
    {
      object_t *obj = packed.لکۍ ();
      packed_map.del (obj);
      أكد (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.راوباسه ();
    }
    إذا (packed.اوږدتیا > 1)
      أكد (packed.لکۍ ()->سر_2 == لکۍ);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  فراغ جمع_کړه_virtual_link (objidx_t objidx)
  {
    إذا (unlikely (دننه_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);

    إذا (!current->جمع_کړه_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t وروستی_added_ماشوم_شاخص() ثابت {
    إذا (unlikely (دننه_error ())) أرجع (objidx_t) -1;

    أكد (current);
    إذا (!منطقي(current->real_links)) {
      أرجع (objidx_t) -1;
    }

    أرجع current->real_links[current->real_links.اوږدتیا - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  فراغ repack_وروستی(objidx_t objidx) {
    إذا (unlikely (دننه_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    لكل (خپلکاری& l : current->real_links) {
      إذا (l.objidx == objidx) {
        استمر;
      }

      packed[l.objidx]->جمع_کړه_virtual_link(objidx);
    }
  }

  کينډۍ <ډول_نوم T>
  فراغ جمع_کړه_link (T &ofs, objidx_t objidx,
		 whence_t whence = سر,
		 بینښه bias = 0)
  {
    إذا (unlikely (دننه_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) &ofs);

    خپلکاری& link = *current->real_links.push ();
    إذا (current->real_links.دننه_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.پلنوالی = حجم (T);
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
    link.whence = (بینښه) whence;
    link.position = (ثابت حرف *) &ofs - current->سر_2;
    link.bias = bias;
  }

  بینښه منزل_bias (ثابت فراغ *بنسټ) ثابت
  {
    إذا (unlikely (دننه_error ())) أرجع 0;
    إذا (!بنسټ) أرجع 0;
    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) بنسټ);
    أرجع (ثابت حرف *) بنسټ - current->سر_2;
  }

  فراغ resolve_links ()
  {
    إذا (unlikely (دننه_error ())) أرجع;

    أكد (!current);
    أكد (packed.اوږدتیا > 1);

    لكل (ثابت object_t* پلار : ++hb_iter (packed))
      لكل (ثابت object_t::link_t &link : پلار->real_links)
      {
	ثابت object_t* ماشوم = packed[link.objidx];
	إذا (unlikely (!ماشوم)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع; }
	بینښه دننه_واټن = 0;
	بدل ((whence_t) link.whence) {
	حالة سر:     دننه_واټن = ماشوم->سر_2 - پلار->سر_2; اكسر;
	حالة لکۍ_2:     دننه_واټن = ماشوم->سر_2 - پلار->لکۍ; اكسر;
	حالة Absolute: دننه_واټن = (سر_2 - پیل) + (ماشوم->سر_2 - لکۍ); اكسر;
	}

	أكد (دننه_واټن >= link.bias);
	دننه_واټن -= link.bias;
	إذا (link.is_signed)
	{
	  أكد (link.پلنوالی == 2 || link.پلنوالی == 4);
	  إذا (link.پلنوالی == 4)
	    ټاکل_دننه_واټن<int32_t> (پلار, link, دننه_واټن);
	  وإلا
	    ټاکل_دننه_واټن<int16_t> (پلار, link, دننه_واټن);
	}
	وإلا
	{
	  أكد (link.پلنوالی == 2 || link.پلنوالی == 3 || link.پلنوالی == 4);
	  إذا (link.پلنوالی == 4)
	    ټاکل_دننه_واټن<uint32_t> (پلار, link, دننه_واټن);
	  وإلا إذا (link.پلنوالی == 3)
	    ټاکل_دننه_واټن<uint32_t, 3> (پلار, link, دننه_واټن);
	  وإلا
	    ټاکل_دننه_واټن<uint16_t> (پلار, link, دننه_واټن);
	}
      }
  }

  بینښه صحيح اوږدتیا () ثابت
  {
    إذا (unlikely (!current)) أرجع 0;
    أرجع هذا->سر_2 - current->سر_2;
  }

  فراغ align (بینښه صحيح alignment)
  {
    بینښه صحيح l = اوږدتیا () % alignment;
    إذا (l)
      (فراغ) allocate_اندازه<فراغ> (alignment - l);
  }

  کينډۍ <ډول_نوم Type = فراغ>
  __attribute__((returns_nonnull))
  Type *پیل_embed (ثابت Type *obj HB_ناکارول_شوی = تش_اشاره) ثابت
  { أرجع reinterpret_cast<Type *> (هذا->سر_2); }
  کينډۍ <ډول_نوم Type>
  __attribute__((returns_nonnull))
  Type *پیل_embed (ثابت Type &obj) ثابت
  { أرجع پیل_embed (std::addressof (obj)); }

  منطقي err (hb_serialize_error_t err_type)
  {
    أرجع !منطقي ((errors = (errors | err_type)));
  }

  منطقي پیل_zerocopy (size_t اندازه)
  {
    إذا (unlikely (دننه_error ())) أرجع خطأ;

    إذا (unlikely (اندازه > لوی_عدد || هذا->لکۍ - هذا->سر_2 < ptrdiff_t (اندازه)))
    {
      err (HB_SERIALIZE_ERROR_بهر_د_ځای);
      أرجع خطأ;
    }

    أكد (!هذا->zerocopy);
    هذا->zerocopy = هذا->سر_2;

    أكد (هذا->current->سر_2 == هذا->سر_2);
    هذا->current->سر_2 = هذا->current->لکۍ = هذا->سر_2 = هذا->لکۍ - اندازه;
    أرجع ریښتیا;
  }

  کينډۍ <ډول_نوم Type>
  HB_NODISCARD
  Type *allocate_اندازه (size_t اندازه, منطقي clear = ریښتیا)
  {
    إذا (unlikely (دننه_error ())) أرجع تش_اشاره;

    إذا (unlikely (اندازه > لوی_عدد || هذا->لکۍ - هذا->سر_2 < ptrdiff_t (اندازه)))
    {
      err (HB_SERIALIZE_ERROR_بهر_د_ځای);
      أرجع تش_اشاره;
    }
    إذا (clear)
      hb_memset (هذا->سر_2, 0, اندازه);
    حرف *ret = هذا->سر_2;
    هذا->سر_2 += اندازه;
    أرجع reinterpret_cast<Type *> (ret);
  }

  کينډۍ <ډول_نوم Type>
  Type *allocate_min ()
  { أرجع هذا->allocate_اندازه<Type> (Type::min_اندازه); }

  کينډۍ <ډول_نوم Type>
  HB_NODISCARD
  Type *embed (ثابت Type *obj)
  {
    size_t اندازه = obj->get_اندازه ();
    Type *ret = هذا->allocate_اندازه<Type> (اندازه, خطأ);
    إذا (unlikely (!ret)) أرجع تش_اشاره;
    hb_memcpy (ret, obj, اندازه);
    أرجع ret;
  }
  کينډۍ <ډول_نوم Type>
  HB_NODISCARD
  Type *embed (ثابت Type &obj)
  { أرجع embed (std::addressof (obj)); }
  حرف *embed (ثابت حرف *obj, بینښه اندازه)
  {
    حرف *ret = هذا->allocate_اندازه<حرف> (اندازه, خطأ);
    إذا (unlikely (!ret)) أرجع تش_اشاره;
    hb_memcpy (ret, obj, اندازه);
    أرجع ret;
  }

  کينډۍ <ډول_نوم Type, ډول_نوم ...Ts> خپلکاری
  _copy (ثابت Type &src, hb_لومړیتوب<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (هذا, std::forward<Ts> (ds)...))

  کينډۍ <ډول_نوم Type> خپلکاری
  _copy (ثابت Type &src, hb_لومړیتوب<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = هذا->allocate_اندازه<Type> (حجم (Type));
    إذا (unlikely (!ret)) أرجع تش_اشاره;
    *ret = src;
    أرجع ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  کينډۍ <ډول_نوم Type, ډول_نوم ...Ts>
  Type *copy (ثابت Type &src, Ts&&... ds)
  { أرجع _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  کينډۍ <ډول_نوم Type, ډول_نوم ...Ts>
  Type *copy (ثابت Type *src, Ts&&... ds)
  { أرجع copy (*src, std::forward<Ts> (ds)...); }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   ډول_نوم ...Ts>
  فراغ copy_all (Iterator it, Ts&&... ds)
  { لكل (decltype (*it) _ : it) copy (_, ds...); }

  کينډۍ <ډول_نوم Type>
  hb_serialize_context_t& چلوونکی << (ثابت Type &obj) & { embed (obj); أرجع *هذا; }

  کينډۍ <ډول_نوم Type>
  Type *extend_اندازه (Type *obj, size_t اندازه, منطقي clear = ریښتیا)
  {
    إذا (unlikely (دننه_error ())) أرجع تش_اشاره;
    إذا (unlikely (اندازه >= لوی_عدد)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع تش_اشاره; }

    أكد (هذا->پیل <= (حرف *) obj);
    أكد ((حرف *) obj <= هذا->سر_2);
    أكد ((size_t) (هذا->سر_2 - (حرف *) obj) <= اندازه);
    إذا (unlikely (((حرف *) obj + اندازه < (حرف *) obj) ||
		  !هذا->allocate_اندازه<Type> (((حرف *) obj) + اندازه - هذا->سر_2, clear))) أرجع تش_اشاره;
    أرجع reinterpret_cast<Type *> (obj);
  }
  کينډۍ <ډول_نوم Type>
  Type *extend_اندازه (Type &obj, size_t اندازه, منطقي clear = ریښتیا)
  { أرجع extend_اندازه (std::addressof (obj), اندازه, clear); }

  کينډۍ <ډول_نوم Type>
  Type *extend_min (Type *obj) { أرجع extend_اندازه (obj, obj->min_اندازه); }
  کينډۍ <ډول_نوم Type>
  Type *extend_min (Type &obj) { أرجع extend_min (std::addressof (obj)); }

  کينډۍ <ډول_نوم Type, ډول_نوم ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { أرجع extend_اندازه (obj, obj->get_اندازه (std::forward<Ts> (ds)...)); }
  کينډۍ <ډول_نوم Type, ډول_نوم ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { أرجع extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_بایټونه_t copy_بایټونه () ثابت
  {
    أكد (successful ());
    /* Copy both items from head side and tail side... */
    بینښه صحيح len = (هذا->سر_2 - هذا->پیل)
		     + (هذا->پای_ټکی  - هذا->لکۍ);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    إذا (!len) أرجع hb_بایټونه_t ();

    حرف *p = (حرف *) hb_malloc (len);
    إذا (unlikely (!p)) أرجع hb_بایټونه_t ();

    hb_memcpy (p, هذا->پیل, هذا->سر_2 - هذا->پیل);
    hb_memcpy (p + (هذا->سر_2 - هذا->پیل), هذا->لکۍ, هذا->پای_ټکی - هذا->لکۍ);
    أرجع hb_بایټونه_t (p, len);
  }
  کينډۍ <ډول_نوم Type>
  Type *copy () ثابت
  { أرجع reinterpret_cast<Type *> ((حرف *) copy_بایټونه ().arrayZ); }
  hb_blob_t *copy_blob () ثابت
  {
    hb_بایټونه_t b = copy_بایټونه ();
    أرجع hb_blob_create (b.arrayZ, b.اوږدتیا,
			   HB_حافظه_ډېرپېښ_WRITABLE,
			   (حرف *) b.arrayZ, hb_free);
  }

  ثابت hb_ویکټور_t<object_t *>& object_ګراف() ثابت
  { أرجع packed; }

  خاص:
  کينډۍ <ډول_نوم T, بینښه اندازه_2 = حجم (T)>
  فراغ ټاکل_دننه_واټن (ثابت object_t* پلار, ثابت object_t::link_t &link, بینښه دننه_واټن)
  {
    // XXX We should stop assuming big-endian!
    خپلکاری &off = * ((HBInt<ریښتیا, T, اندازه_2> *) (پلار->سر_2 + link.position));
    أكد (0 == off);
    check_ټاکل (off, دننه_واټن, HB_SERIALIZE_ERROR_دننه_واټن_ډکېدنه);
  }

  عام:
  حرف *پیل, *سر_2, *لکۍ, *پای_ټکی, *zerocopy;
  بینښه صحيح debug_ژوروالی;
  hb_serialize_error_t errors;

  خاص:

  فراغ یوځای_virtual_links (ثابت object_t* from, objidx_t منزل_idx) {
    object_t* to = packed[منزل_idx];
    لكل (ثابت خپلکاری& l : from->virtual_links) {
      to->virtual_links.push (l);
    }
  }

  /* Object memory pool. */
  hb_free_pool_t<object_t> object_pool;

  /* Stack of currently under construction objects. */
  object_t *current;

  /* Stack of packed objects.  Object 0 is always nil object. */
  hb_ویکټور_t<object_t *> packed;

  /* Map view of packed objects. */
  hb_hashmap_t<ثابت object_t *, objidx_t> packed_map;
};

#د_که_پای /* HB_SERIALIZE_HH */
