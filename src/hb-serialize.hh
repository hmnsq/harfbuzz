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

#جيڪڏهن_وصف_نه_ٿيل HB_SERIALIZE_HH
#وصف_ڪريو HB_SERIALIZE_HH

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
  HB_SERIALIZE_ERROR_وٿي_حد_کان_مٿي = 0x00000002u,
  HB_SERIALIZE_ERROR_ٻاهريون_جو_گنجائش =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_حد_کان_مٿي =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_حد_کان_مٿي =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

بنية hb_serialize_context_t
{
  تعريفنوع بنانشان objidx_t;

  تعداد whence_t {
     سر,	/* Relative to the current object head (default). */
     پڇ_2,	/* Relative to the current object tail after packed. */
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
      پڇ = o.پڇ;
      next = خالي_اشارو;
      real_links.alloc_exact (o.انگ_real_links);
      لكل (بنانشان i = 0 ; i < o.انگ_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.انگ_virtual_links);
      لكل (بنانشان i = 0; i < o.انگ_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    منطقي اندريون_error () ثابت
    {
      أرجع real_links.اندريون_error() || virtual_links.اندريون_error();
    }

    منطقي جوڙيو_virtual_link (objidx_t objidx)
    {
      إذا (!objidx)
        أرجع خطأ;

      خودڪار& link = *virtual_links.push ();
      إذا (virtual_links.اندريون_error ())
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

    دوست فراغ swap (object_t& a, object_t& b) noexcept
    {
      hb_مٽا_سٽا (a.سر_2, b.سر_2);
      hb_مٽا_سٽا (a.پڇ, b.پڇ);
      hb_مٽا_سٽا (a.next, b.next);
      hb_مٽا_سٽا (a.real_links, b.real_links);
      hb_مٽا_سٽا (a.virtual_links, b.virtual_links);
    }

    منطقي عامل == (ثابت object_t &o) ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع (پڇ - سر_2 == o.پڇ - o.سر_2)
	  && (real_links.ڊيگهائي == o.real_links.ڊيگهائي)
	  && 0 == hb_memcmp (سر_2, o.سر_2, پڇ - سر_2)
	  && real_links.as_بائيٽون () == o.real_links.as_بائيٽون ();
    }
    uint32_t hash () ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      أرجع hb_بائيٽون_t (سر_2, hb_min (128, پڇ - سر_2)).hash () ^
          real_links.as_بائيٽون ().hash ();
    }

    بنية link_t
    {
      بنانشان ويڪر: 3;
      بنانشان is_signed: 1;
      بنانشان whence: 2;
      بنانشان bias : 26;
      بنانشان position;
      objidx_t objidx;

      link_t () = افتراضي;

      link_t (ثابت hb_subset_serialize_link_t &o)
      {
        ويڪر = o.ويڪر;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_اندروني ساكن صحيح cmp (ثابت فراغ* a, ثابت فراغ* b)
      {
        صحيح cmp = ((ثابت link_t*)a)->position - ((ثابت link_t*)b)->position;
        إذا (cmp) أرجع cmp;

        أرجع ((ثابت link_t*)a)->objidx - ((ثابت link_t*)b)->objidx;
      }
    };

    حرف *سر_2;
    حرف *پڇ;
    hb_ويڪٽر_t<link_t> real_links;
    hb_ويڪٽر_t<link_t> virtual_links;
    object_t *next;

    خودڪار all_links () ثابت HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    خودڪار all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  بنية snapshot_t
  {
    حرف *سر_2;
    حرف *پڇ;
    object_t *current; // Just for sanity check
    بنانشان انگ_real_links;
    بنانشان انگ_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    أرجع snapshot_t {
      سر_2, پڇ, current,
      current ? current->real_links.ڊيگهائي : 0,
      current ? current->virtual_links.ڊيگهائي : 0,
      errors
     };
  }

  hb_serialize_context_t (فراغ *شروعات_2, size_t سائيز) :
    شروعات ((حرف *) شروعات_2),
    پڇاڙي (شروعات + سائيز),
    current (خالي_اشارو)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  فراغ fini ()
  {
    لكل (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    هذا->packed_map.fini ();

    طالما (current)
    {
      خودڪار *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  منطقي اندريون_error () ثابت { أرجع منطقي (errors); }

  منطقي successful () ثابت { أرجع !منطقي (errors); }

  HB_NODISCARD منطقي ran_ٻاهريون_جو_گنجائش () ثابت { أرجع errors & HB_SERIALIZE_ERROR_ٻاهريون_جو_گنجائش; }
  HB_NODISCARD منطقي وٿي_حد_کان_مٿي () ثابت { أرجع errors & HB_SERIALIZE_ERROR_وٿي_حد_کان_مٿي; }
  HB_NODISCARD منطقي only_وٿي_حد_کان_مٿي () ثابت { أرجع errors == HB_SERIALIZE_ERROR_وٿي_حد_کان_مٿي; }
  HB_NODISCARD منطقي only_حد_کان_مٿي () ثابت
  {
    أرجع errors == HB_SERIALIZE_ERROR_وٿي_حد_کان_مٿي
        || errors == HB_SERIALIZE_ERROR_INT_حد_کان_مٿي
        || errors == HB_SERIALIZE_ERROR_ARRAY_حد_کان_مٿي;
  }

  فراغ reset (فراغ *شروعات_2, size_t سائيز)
  {
    شروعات = (حرف*) شروعات_2;
    پڇاڙي = شروعات + سائيز;
    reset ();
    current = خالي_اشارو;
  }

  فراغ reset ()
  {
    هذا->errors = HB_SERIALIZE_ERROR_NONE;
    هذا->سر_2 = هذا->شروعات;
    هذا->پڇ = هذا->پڇاڙي;
    هذا->zerocopy = خالي_اشارو;
    هذا->debug_اونهائي = 0;

    fini ();
    هذا->packed.push (خالي_اشارو);
    هذا->packed_map.init ();
  }

  منطقي check_success (منطقي success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    أرجع successful ()
        && (success || err (err_type));
  }

  سانچو <قسم_نالو T1, قسم_نالو T2>
  منطقي check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    إذا ((ڊگهو ڊگهو) v1 != (ڊگهو ڊگهو) v2)
    {
      أرجع err (err_type);
    }
    أرجع سچ;
  }

  سانچو <قسم_نالو T1, قسم_نالو T2>
  منطقي check_سونپيو (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { أرجع check_equal (v1 = v2, v2, err_type); }

  سانچو <قسم_نالو T> منطقي propagate_error (T &&obj)
  { أرجع check_success (!hb_deref (obj).اندريون_error ()); }

  سانچو <قسم_نالو T1, قسم_نالو... Ts> منطقي propagate_error (T1 &&o1, Ts&&... os)
  { أرجع propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  سانچو <قسم_نالو Type=حرف>
  __attribute__((returns_nonnull))
  Type *شروعات_serialize ()
  {
    DEBUG_نياپو_LEVEL (SERIALIZE, هذا->شروعات, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     هذا->شروعات, هذا->پڇاڙي,
		     (بنانشان ڊگهو) (هذا->پڇاڙي - هذا->شروعات));

    أكد (!current);
    أرجع push<Type> ();
  }
  فراغ پڇاڙي_serialize ()
  {
    DEBUG_نياپو_LEVEL (SERIALIZE, هذا->شروعات, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     هذا->شروعات, هذا->پڇاڙي,
		     (بنانشان) (هذا->سر_2 - هذا->شروعات),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    إذا (unlikely (!current)) أرجع;
    إذا (unlikely (اندريون_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      إذا (وٿي_حد_کان_مٿي ()) err (HB_SERIALIZE_ERROR_OTHER);
      أرجع;
    }

    أكد (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    إذا (packed.ڊيگهائي <= 1)
      أرجع;

    ڪڍو_pack (خطأ);

    resolve_links ();
  }

  سانچو <قسم_نالو Type = فراغ>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    إذا (unlikely (اندريون_error ())) أرجع شروعات_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    إذا (unlikely (!obj))
      check_success (خطأ);
    وإلا
    {
      obj->سر_2 = سر_2;
      obj->پڇ = پڇ;
      obj->next = current;
      current = obj;
    }
    أرجع شروعات_embed<Type> ();
  }
  فراغ ڪڍو_discard ()
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (اندريون_error() && !only_حد_کان_مٿي ())) أرجع;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->سر_2, obj->پڇ);
    zerocopy = خالي_اشارو;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t ڪڍو_pack (منطقي share=سچ)
  {
    object_t *obj = current;
    إذا (unlikely (!obj)) أرجع 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    إذا (unlikely (اندريون_error()  && !only_حد_کان_مٿي ())) أرجع 0;

    current = current->next;
    obj->پڇ = سر_2;
    obj->next = خالي_اشارو;
    أكد (obj->سر_2 <= obj->پڇ);
    بنانشان len = obj->پڇ - obj->سر_2;
    سر_2 = zerocopy ? zerocopy : obj->سر_2; /* Rewind head. */
    منطقي was_zerocopy = zerocopy;
    zerocopy = خالي_اشارو;

    إذا (!len)
    {
      أكد (!obj->real_links.ڊيگهائي);
      أكد (!obj->virtual_links.ڊيگهائي);
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
        ملايو_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	أرجع objidx;
      }
    }

    پڇ -= len;
    إذا (was_zerocopy)
      أكد (پڇ == obj->سر_2);
    وإلا
      memmove (پڇ, obj->سر_2, len);

    obj->سر_2 = پڇ;
    obj->پڇ = پڇ + len;

    packed.push (obj);

    إذا (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      أرجع 0;
    }

    objidx = packed.ڊيگهائي - 1;

    إذا (share) packed_map.سيٽ_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    أرجع objidx;
  }

  فراغ revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    إذا (unlikely (اندريون_error () && !only_حد_کان_مٿي ())) أرجع;
    أكد (snap.current == current);
    إذا (current)
    {
      current->real_links.shrink (snap.انگ_real_links);
      current->virtual_links.shrink (snap.انگ_virtual_links);
    }
    errors = snap.errors;
    revert (snap.سر_2, snap.پڇ);
  }

  فراغ revert (حرف *snap_سر,
	       حرف *snap_پڇ)
  {
    إذا (unlikely (اندريون_error ())) أرجع;
    أكد (snap_سر <= سر_2);
    أكد (پڇ <= snap_پڇ);
    سر_2 = snap_سر;
    پڇ = snap_پڇ;
    discard_stale_objects ();
  }

  فراغ discard_stale_objects ()
  {
    إذا (unlikely (اندريون_error ())) أرجع;
    طالما (packed.ڊيگهائي > 1 &&
	   packed.پڇ ()->سر_2 < پڇ)
    {
      object_t *obj = packed.پڇ ();
      packed_map.del (obj);
      أكد (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.ڪڍو ();
    }
    إذا (packed.ڊيگهائي > 1)
      أكد (packed.پڇ ()->سر_2 == پڇ);
  }

  // Adds a virtual link from the current object to objidx. A virtual link is not associated with
  // an actual offset field. They are solely used to enforce ordering constraints between objects.
  // Adding a virtual link from object a to object b will ensure that object b is always packed after
  // object a in the final serialized order.
  //
  // This is useful in certain situations where there needs to be a specific ordering in the
  // final serialization. Such as when platform bugs require certain orderings, or to provide
  //  guidance to the repacker for better offset overflow resolution.
  فراغ جوڙيو_virtual_link (objidx_t objidx)
  {
    إذا (unlikely (اندريون_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);

    إذا (!current->جوڙيو_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t آخري_added_ٻار_اشاريو() ثابت {
    إذا (unlikely (اندريون_error ())) أرجع (objidx_t) -1;

    أكد (current);
    إذا (!منطقي(current->real_links)) {
      أرجع (objidx_t) -1;
    }

    أرجع current->real_links[current->real_links.ڊيگهائي - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  فراغ repack_آخري(objidx_t objidx) {
    إذا (unlikely (اندريون_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    لكل (خودڪار& l : current->real_links) {
      إذا (l.objidx == objidx) {
        استمر;
      }

      packed[l.objidx]->جوڙيو_virtual_link(objidx);
    }
  }

  سانچو <قسم_نالو T>
  فراغ جوڙيو_link (T &ofs, objidx_t objidx,
		 whence_t whence = سر,
		 بنانشان bias = 0)
  {
    إذا (unlikely (اندريون_error ())) أرجع;

    إذا (!objidx)
      أرجع;

    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) &ofs);

    خودڪار& link = *current->real_links.push ();
    إذا (current->real_links.اندريون_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.ويڪر = حجم (T);
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
    link.whence = (بنانشان) whence;
    link.position = (ثابت حرف *) &ofs - current->سر_2;
    link.bias = bias;
  }

  بنانشان منزل_bias (ثابت فراغ *بنياد) ثابت
  {
    إذا (unlikely (اندريون_error ())) أرجع 0;
    إذا (!بنياد) أرجع 0;
    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) بنياد);
    أرجع (ثابت حرف *) بنياد - current->سر_2;
  }

  فراغ resolve_links ()
  {
    إذا (unlikely (اندريون_error ())) أرجع;

    أكد (!current);
    أكد (packed.ڊيگهائي > 1);

    لكل (ثابت object_t* والد : ++hb_iter (packed))
      لكل (ثابت object_t::link_t &link : والد->real_links)
      {
	ثابت object_t* ٻار = packed[link.objidx];
	إذا (unlikely (!ٻار)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع; }
	بنانشان وٿي = 0;
	بدل ((whence_t) link.whence) {
	حالة سر:     وٿي = ٻار->سر_2 - والد->سر_2; اكسر;
	حالة پڇ_2:     وٿي = ٻار->سر_2 - والد->پڇ; اكسر;
	حالة Absolute: وٿي = (سر_2 - شروعات) + (ٻار->سر_2 - پڇ); اكسر;
	}

	أكد (وٿي >= link.bias);
	وٿي -= link.bias;
	إذا (link.is_signed)
	{
	  أكد (link.ويڪر == 2 || link.ويڪر == 4);
	  إذا (link.ويڪر == 4)
	    سونپيو_وٿي<int32_t> (والد, link, وٿي);
	  وإلا
	    سونپيو_وٿي<int16_t> (والد, link, وٿي);
	}
	وإلا
	{
	  أكد (link.ويڪر == 2 || link.ويڪر == 3 || link.ويڪر == 4);
	  إذا (link.ويڪر == 4)
	    سونپيو_وٿي<uint32_t> (والد, link, وٿي);
	  وإلا إذا (link.ويڪر == 3)
	    سونپيو_وٿي<uint32_t, 3> (والد, link, وٿي);
	  وإلا
	    سونپيو_وٿي<uint16_t> (والد, link, وٿي);
	}
      }
  }

  بنانشان صحيح ڊيگهائي () ثابت
  {
    إذا (unlikely (!current)) أرجع 0;
    أرجع هذا->سر_2 - current->سر_2;
  }

  فراغ align (بنانشان صحيح alignment)
  {
    بنانشان صحيح l = ڊيگهائي () % alignment;
    إذا (l)
      (فراغ) allocate_سائيز<فراغ> (alignment - l);
  }

  سانچو <قسم_نالو Type = فراغ>
  __attribute__((returns_nonnull))
  Type *شروعات_embed (ثابت Type *obj HB_اڻ_استعمال_ٿيل = خالي_اشارو) ثابت
  { أرجع reinterpret_cast<Type *> (هذا->سر_2); }
  سانچو <قسم_نالو Type>
  __attribute__((returns_nonnull))
  Type *شروعات_embed (ثابت Type &obj) ثابت
  { أرجع شروعات_embed (std::addressof (obj)); }

  منطقي err (hb_serialize_error_t err_type)
  {
    أرجع !منطقي ((errors = (errors | err_type)));
  }

  منطقي شروعات_zerocopy (size_t سائيز)
  {
    إذا (unlikely (اندريون_error ())) أرجع خطأ;

    إذا (unlikely (سائيز > وڌ_کان_وڌ_عدد || هذا->پڇ - هذا->سر_2 < ptrdiff_t (سائيز)))
    {
      err (HB_SERIALIZE_ERROR_ٻاهريون_جو_گنجائش);
      أرجع خطأ;
    }

    أكد (!هذا->zerocopy);
    هذا->zerocopy = هذا->سر_2;

    أكد (هذا->current->سر_2 == هذا->سر_2);
    هذا->current->سر_2 = هذا->current->پڇ = هذا->سر_2 = هذا->پڇ - سائيز;
    أرجع سچ;
  }

  سانچو <قسم_نالو Type>
  HB_NODISCARD
  Type *allocate_سائيز (size_t سائيز, منطقي clear = سچ)
  {
    إذا (unlikely (اندريون_error ())) أرجع خالي_اشارو;

    إذا (unlikely (سائيز > وڌ_کان_وڌ_عدد || هذا->پڇ - هذا->سر_2 < ptrdiff_t (سائيز)))
    {
      err (HB_SERIALIZE_ERROR_ٻاهريون_جو_گنجائش);
      أرجع خالي_اشارو;
    }
    إذا (clear)
      hb_memset (هذا->سر_2, 0, سائيز);
    حرف *ret = هذا->سر_2;
    هذا->سر_2 += سائيز;
    أرجع reinterpret_cast<Type *> (ret);
  }

  سانچو <قسم_نالو Type>
  Type *allocate_min ()
  { أرجع هذا->allocate_سائيز<Type> (Type::min_سائيز); }

  سانچو <قسم_نالو Type>
  HB_NODISCARD
  Type *embed (ثابت Type *obj)
  {
    size_t سائيز = obj->get_سائيز ();
    Type *ret = هذا->allocate_سائيز<Type> (سائيز, خطأ);
    إذا (unlikely (!ret)) أرجع خالي_اشارو;
    hb_memcpy (ret, obj, سائيز);
    أرجع ret;
  }
  سانچو <قسم_نالو Type>
  HB_NODISCARD
  Type *embed (ثابت Type &obj)
  { أرجع embed (std::addressof (obj)); }
  حرف *embed (ثابت حرف *obj, بنانشان سائيز)
  {
    حرف *ret = هذا->allocate_سائيز<حرف> (سائيز, خطأ);
    إذا (unlikely (!ret)) أرجع خالي_اشارو;
    hb_memcpy (ret, obj, سائيز);
    أرجع ret;
  }

  سانچو <قسم_نالو Type, قسم_نالو ...Ts> خودڪار
  _copy (ثابت Type &src, hb_ترجيح<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (هذا, std::forward<Ts> (ds)...))

  سانچو <قسم_نالو Type> خودڪار
  _copy (ثابت Type &src, hb_ترجيح<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = هذا->allocate_سائيز<Type> (حجم (Type));
    إذا (unlikely (!ret)) أرجع خالي_اشارو;
    *ret = src;
    أرجع ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  سانچو <قسم_نالو Type, قسم_نالو ...Ts>
  Type *copy (ثابت Type &src, Ts&&... ds)
  { أرجع _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  سانچو <قسم_نالو Type, قسم_نالو ...Ts>
  Type *copy (ثابت Type *src, Ts&&... ds)
  { أرجع copy (*src, std::forward<Ts> (ds)...); }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   قسم_نالو ...Ts>
  فراغ copy_all (Iterator it, Ts&&... ds)
  { لكل (decltype (*it) _ : it) copy (_, ds...); }

  سانچو <قسم_نالو Type>
  hb_serialize_context_t& عامل << (ثابت Type &obj) & { embed (obj); أرجع *هذا; }

  سانچو <قسم_نالو Type>
  Type *extend_سائيز (Type *obj, size_t سائيز, منطقي clear = سچ)
  {
    إذا (unlikely (اندريون_error ())) أرجع خالي_اشارو;
    إذا (unlikely (سائيز >= وڌ_کان_وڌ_عدد)) { err (HB_SERIALIZE_ERROR_OTHER); أرجع خالي_اشارو; }

    أكد (هذا->شروعات <= (حرف *) obj);
    أكد ((حرف *) obj <= هذا->سر_2);
    أكد ((size_t) (هذا->سر_2 - (حرف *) obj) <= سائيز);
    إذا (unlikely (((حرف *) obj + سائيز < (حرف *) obj) ||
		  !هذا->allocate_سائيز<Type> (((حرف *) obj) + سائيز - هذا->سر_2, clear))) أرجع خالي_اشارو;
    أرجع reinterpret_cast<Type *> (obj);
  }
  سانچو <قسم_نالو Type>
  Type *extend_سائيز (Type &obj, size_t سائيز, منطقي clear = سچ)
  { أرجع extend_سائيز (std::addressof (obj), سائيز, clear); }

  سانچو <قسم_نالو Type>
  Type *extend_min (Type *obj) { أرجع extend_سائيز (obj, obj->min_سائيز); }
  سانچو <قسم_نالو Type>
  Type *extend_min (Type &obj) { أرجع extend_min (std::addressof (obj)); }

  سانچو <قسم_نالو Type, قسم_نالو ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { أرجع extend_سائيز (obj, obj->get_سائيز (std::forward<Ts> (ds)...)); }
  سانچو <قسم_نالو Type, قسم_نالو ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { أرجع extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_بائيٽون_t copy_بائيٽون () ثابت
  {
    أكد (successful ());
    /* Copy both items from head side and tail side... */
    بنانشان صحيح len = (هذا->سر_2 - هذا->شروعات)
		     + (هذا->پڇاڙي  - هذا->پڇ);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    إذا (!len) أرجع hb_بائيٽون_t ();

    حرف *p = (حرف *) hb_malloc (len);
    إذا (unlikely (!p)) أرجع hb_بائيٽون_t ();

    hb_memcpy (p, هذا->شروعات, هذا->سر_2 - هذا->شروعات);
    hb_memcpy (p + (هذا->سر_2 - هذا->شروعات), هذا->پڇ, هذا->پڇاڙي - هذا->پڇ);
    أرجع hb_بائيٽون_t (p, len);
  }
  سانچو <قسم_نالو Type>
  Type *copy () ثابت
  { أرجع reinterpret_cast<Type *> ((حرف *) copy_بائيٽون ().arrayZ); }
  hb_blob_t *copy_blob () ثابت
  {
    hb_بائيٽون_t b = copy_بائيٽون ();
    أرجع hb_blob_create (b.arrayZ, b.ڊيگهائي,
			   HB_يادگيري_گهڻائي_WRITABLE,
			   (حرف *) b.arrayZ, hb_free);
  }

  ثابت hb_ويڪٽر_t<object_t *>& object_گراف() ثابت
  { أرجع packed; }

  خاص:
  سانچو <قسم_نالو T, بنانشان سائيز_2 = حجم (T)>
  فراغ سونپيو_وٿي (ثابت object_t* والد, ثابت object_t::link_t &link, بنانشان وٿي)
  {
    // XXX We should stop assuming big-endian!
    خودڪار &off = * ((HBInt<سچ, T, سائيز_2> *) (والد->سر_2 + link.position));
    أكد (0 == off);
    check_سونپيو (off, وٿي, HB_SERIALIZE_ERROR_وٿي_حد_کان_مٿي);
  }

  عام:
  حرف *شروعات, *سر_2, *پڇ, *پڇاڙي, *zerocopy;
  بنانشان صحيح debug_اونهائي;
  hb_serialize_error_t errors;

  خاص:

  فراغ ملايو_virtual_links (ثابت object_t* from, objidx_t منزل_idx) {
    object_t* to = packed[منزل_idx];
    لكل (ثابت خودڪار& l : from->virtual_links) {
      to->virtual_links.push (l);
    }
  }

  /* Object memory pool. */
  hb_free_pool_t<object_t> object_pool;

  /* Stack of currently under construction objects. */
  object_t *current;

  /* Stack of packed objects.  Object 0 is always nil object. */
  hb_ويڪٽر_t<object_t *> packed;

  /* Map view of packed objects. */
  hb_hashmap_t<ثابت object_t *, objidx_t> packed_map;
};

#جيڪڏهن_پورو /* HB_SERIALIZE_HH */
