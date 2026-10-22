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

#اگر_غیر_تعریف_شدہ HB_SERIALIZE_HH
#تعریف_کرو HB_SERIALIZE_HH

#شامل "hb.hh"
#شامل "hb-blob.hh"
#شامل "hb-map.hh"
#شامل "hb-free-pool.hh"

#شامل "hb-subset-serialize.h"

/*
 * Serialize
 */

شمار hb_serialize_error_t {
  HB_SERIALIZE_ERROR_NONE =            0x00000000u,
  HB_SERIALIZE_ERROR_OTHER =           0x00000001u,
  HB_SERIALIZE_ERROR_انحراف_حد_سے_تجاوز = 0x00000002u,
  HB_SERIALIZE_ERROR_برآمد_کا_گنجائش =     0x00000004u,
  HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز =    0x00000008u,
  HB_SERIALIZE_ERROR_ARRAY_حد_سے_تجاوز =  0x00000010u
};
HB_MARK_AS_FLAG_T (hb_serialize_error_t);

ساخت hb_serialize_context_t
{
  قسمتعریف غیرعلامتی objidx_t;

  شمار whence_t {
     سر,	/* Relative to the current object head (default). */
     دم_2,	/* Relative to the current object tail after packed. */
     Absolute	/* Absolute: from the start of the serialize buffer. */
   };



  ساخت object_t
  {
    خالی fini () {
      real_links.fini ();
      virtual_links.fini ();
    }

    object_t () = طےشدہ;

    object_t (ثابت hb_subset_serialize_object_t &o)
    {
      سر_2 = o.سر_2;
      دم = o.دم;
      next = خالی_اشاریہ;
      real_links.alloc_exact (o.عدد_real_links);
      برائے (غیرعلامتی i = 0 ; i < o.عدد_real_links; i++)
        real_links.push (o.real_links[i]);

      virtual_links.alloc_exact (o.عدد_virtual_links);
      برائے (غیرعلامتی i = 0; i < o.عدد_virtual_links; i++)
        virtual_links.push (o.virtual_links[i]);
    }

    منطقی اندر_error () ثابت
    {
      واپس real_links.اندر_error() || virtual_links.اندر_error();
    }

    منطقی جوڑو_virtual_link (objidx_t objidx)
    {
      اگر (!objidx)
        واپس جھوٹ;

      خودکار& link = *virtual_links.push ();
      اگر (virtual_links.اندر_error ())
        واپس جھوٹ;

      link.objidx = objidx;
      // Remaining fields were previously zero'd by push():
      // link.width = 0;
      // link.is_signed = 0;
      // link.whence = 0;
      // link.position = 0;
      // link.bias = 0;

      واپس سچ;
    }

    دوست خالی swap (object_t& a, object_t& b) noexcept
    {
      hb_تبادلہ (a.سر_2, b.سر_2);
      hb_تبادلہ (a.دم, b.دم);
      hb_تبادلہ (a.next, b.next);
      hb_تبادلہ (a.real_links, b.real_links);
      hb_تبادلہ (a.virtual_links, b.virtual_links);
    }

    منطقی عامل == (ثابت object_t &o) ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      واپس (دم - سر_2 == o.دم - o.سر_2)
	  && (real_links.لمبائی == o.real_links.لمبائی)
	  && 0 == hb_memcmp (سر_2, o.سر_2, دم - سر_2)
	  && real_links.as_بائٹیں () == o.real_links.as_بائٹیں ();
    }
    uint32_t hash () ثابت
    {
      // Virtual links aren't considered for equality since they don't affect the functionality
      // of the object.
      واپس hb_بائٹیں_t (سر_2, hb_min (128, دم - سر_2)).hash () ^
          real_links.as_بائٹیں ().hash ();
    }

    ساخت link_t
    {
      غیرعلامتی چوڑائی: 3;
      غیرعلامتی is_signed: 1;
      غیرعلامتی whence: 2;
      غیرعلامتی bias : 26;
      غیرعلامتی position;
      objidx_t objidx;

      link_t () = طےشدہ;

      link_t (ثابت hb_subset_serialize_link_t &o)
      {
        چوڑائی = o.چوڑائی;
        is_signed = 0;
        whence = 0;
        position = o.position;
        bias = 0;
        objidx = o.objidx;
      }

      HB_اندرونی ساکن صحیح cmp (ثابت خالی* a, ثابت خالی* b)
      {
        صحیح cmp = ((ثابت link_t*)a)->position - ((ثابت link_t*)b)->position;
        اگر (cmp) واپس cmp;

        واپس ((ثابت link_t*)a)->objidx - ((ثابت link_t*)b)->objidx;
      }
    };

    حرف *سر_2;
    حرف *دم;
    hb_سمتیہ_t<link_t> real_links;
    hb_سمتیہ_t<link_t> virtual_links;
    object_t *next;

    خودکار all_links () ثابت HB_AUTO_RETURN
        (( hb_concat (real_links, virtual_links) ));
    خودکار all_links_writer () HB_AUTO_RETURN
        (( hb_concat (real_links.writer (), virtual_links.writer ()) ));
  };

  ساخت snapshot_t
  {
    حرف *سر_2;
    حرف *دم;
    object_t *current; // Just for sanity check
    غیرعلامتی عدد_real_links;
    غیرعلامتی عدد_virtual_links;
    hb_serialize_error_t errors;
  };

  snapshot_t snapshot ()
  {
    واپس snapshot_t {
      سر_2, دم, current,
      current ? current->real_links.لمبائی : 0,
      current ? current->virtual_links.لمبائی : 0,
      errors
     };
  }

  hb_serialize_context_t (خالی *آغاز_2, size_t سائز) :
    آغاز ((حرف *) آغاز_2),
    انتہا (آغاز + سائز),
    current (خالی_اشاریہ)
  { reset (); }
  ~hb_serialize_context_t () { fini (); }

  خالی fini ()
  {
    برائے (object_t *_ : ++hb_iter (packed)) _->fini ();
    packed.fini ();
    یہ->packed_map.fini ();

    جب (current)
    {
      خودکار *_ = current;
      current = current->next;
      _->fini ();
    }
  }

  منطقی اندر_error () ثابت { واپس منطقی (errors); }

  منطقی successful () ثابت { واپس !منطقی (errors); }

  HB_NODISCARD منطقی ran_برآمد_کا_گنجائش () ثابت { واپس errors & HB_SERIALIZE_ERROR_برآمد_کا_گنجائش; }
  HB_NODISCARD منطقی انحراف_حد_سے_تجاوز () ثابت { واپس errors & HB_SERIALIZE_ERROR_انحراف_حد_سے_تجاوز; }
  HB_NODISCARD منطقی only_انحراف_حد_سے_تجاوز () ثابت { واپس errors == HB_SERIALIZE_ERROR_انحراف_حد_سے_تجاوز; }
  HB_NODISCARD منطقی only_حد_سے_تجاوز () ثابت
  {
    واپس errors == HB_SERIALIZE_ERROR_انحراف_حد_سے_تجاوز
        || errors == HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز
        || errors == HB_SERIALIZE_ERROR_ARRAY_حد_سے_تجاوز;
  }

  خالی reset (خالی *آغاز_2, size_t سائز)
  {
    آغاز = (حرف*) آغاز_2;
    انتہا = آغاز + سائز;
    reset ();
    current = خالی_اشاریہ;
  }

  خالی reset ()
  {
    یہ->errors = HB_SERIALIZE_ERROR_NONE;
    یہ->سر_2 = یہ->آغاز;
    یہ->دم = یہ->انتہا;
    یہ->zerocopy = خالی_اشاریہ;
    یہ->debug_گہرائی = 0;

    fini ();
    یہ->packed.push (خالی_اشاریہ);
    یہ->packed_map.init ();
  }

  منطقی check_success (منطقی success,
                      hb_serialize_error_t err_type = HB_SERIALIZE_ERROR_OTHER)
  {
    واپس successful ()
        && (success || err (err_type));
  }

  سانچہ <قسم_نام T1, قسم_نام T2>
  منطقی check_equal (T1 &&v1, T2 &&v2, hb_serialize_error_t err_type)
  {
    اگر ((طویل طویل) v1 != (طویل طویل) v2)
    {
      واپس err (err_type);
    }
    واپس سچ;
  }

  سانچہ <قسم_نام T1, قسم_نام T2>
  منطقی check_تفویض (T1 &v1, T2 &&v2, hb_serialize_error_t err_type)
  { واپس check_equal (v1 = v2, v2, err_type); }

  سانچہ <قسم_نام T> منطقی propagate_error (T &&obj)
  { واپس check_success (!hb_deref (obj).اندر_error ()); }

  سانچہ <قسم_نام T1, قسم_نام... Ts> منطقی propagate_error (T1 &&o1, Ts&&... os)
  { واپس propagate_error (std::forward<T1> (o1)) &&
	   propagate_error (std::forward<Ts> (os)...); }

  /* To be called around main operation. */
  سانچہ <قسم_نام Type=حرف>
  __attribute__((returns_nonnull))
  Type *آغاز_serialize ()
  {
    DEBUG_پیغام_LEVEL (SERIALIZE, یہ->آغاز, 0, +1,
		     "start [%p..%p] (%lu bytes)",
		     یہ->آغاز, یہ->انتہا,
		     (غیرعلامتی طویل) (یہ->انتہا - یہ->آغاز));

    أكد (!current);
    واپس push<Type> ();
  }
  خالی انتہا_serialize ()
  {
    DEBUG_پیغام_LEVEL (SERIALIZE, یہ->آغاز, 0, -1,
		     "end [%p..%p] serialized %u bytes; %s",
		     یہ->آغاز, یہ->انتہا,
		     (غیرعلامتی) (یہ->سر_2 - یہ->آغاز),
		     successful () ? "successful" : "UNSUCCESSFUL");

    propagate_error (packed, packed_map);

    اگر (unlikely (!current)) واپس;
    اگر (unlikely (اندر_error()))
    {
      // Offset overflows that occur before link resolution cannot be handled
      // by repacking, so set a more general error.
      اگر (انحراف_حد_سے_تجاوز ()) err (HB_SERIALIZE_ERROR_OTHER);
      واپس;
    }

    أكد (!current->next);

    /* Only "pack" if there exist other objects... Otherwise, don't bother.
     * Saves a move. */
    اگر (packed.لمبائی <= 1)
      واپس;

    نکالو_pack (جھوٹ);

    resolve_links ();
  }

  سانچہ <قسم_نام Type = خالی>
  __attribute__((returns_nonnull))
  Type *push ()
  {
    اگر (unlikely (اندر_error ())) واپس آغاز_embed<Type> ();

    object_t *obj = object_pool.alloc ();
    اگر (unlikely (!obj))
      check_success (جھوٹ);
    ورنہ
    {
      obj->سر_2 = سر_2;
      obj->دم = دم;
      obj->next = current;
      current = obj;
    }
    واپس آغاز_embed<Type> ();
  }
  خالی نکالو_discard ()
  {
    object_t *obj = current;
    اگر (unlikely (!obj)) واپس;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    اگر (unlikely (اندر_error() && !only_حد_سے_تجاوز ())) واپس;

    current = current->next;
    revert (zerocopy ? zerocopy : obj->سر_2, obj->دم);
    zerocopy = خالی_اشاریہ;
    obj->fini ();
    object_pool.release (obj);
  }

  /* Set share to false when an object is unlikely shareable with others
   * so not worth an attempt, or a contiguous table is serialized as
   * multiple consecutive objects in the reverse order so can't be shared.
   */
  objidx_t نکالو_pack (منطقی share=سچ)
  {
    object_t *obj = current;
    اگر (unlikely (!obj)) واپس 0;
    // Allow cleanup when we've error'd out on int overflows which don't compromise
    // the serializer state.
    اگر (unlikely (اندر_error()  && !only_حد_سے_تجاوز ())) واپس 0;

    current = current->next;
    obj->دم = سر_2;
    obj->next = خالی_اشاریہ;
    أكد (obj->سر_2 <= obj->دم);
    غیرعلامتی len = obj->دم - obj->سر_2;
    سر_2 = zerocopy ? zerocopy : obj->سر_2; /* Rewind head. */
    منطقی was_zerocopy = zerocopy;
    zerocopy = خالی_اشاریہ;

    اگر (!len)
    {
      أكد (!obj->real_links.لمبائی);
      أكد (!obj->virtual_links.لمبائی);
      واپس 0;
    }

    objidx_t objidx;
    uint32_t hash = 0;
    اگر (share)
    {
      hash = hb_hash (obj);
      objidx = packed_map.get_with_hash (obj, hash);
      اگر (objidx)
      {
        ملاؤ_virtual_links (obj, objidx);
	obj->fini ();
        object_pool.release (obj);
	واپس objidx;
      }
    }

    دم -= len;
    اگر (was_zerocopy)
      أكد (دم == obj->سر_2);
    ورنہ
      memmove (دم, obj->سر_2, len);

    obj->سر_2 = دم;
    obj->دم = دم + len;

    packed.push (obj);

    اگر (unlikely (!propagate_error (packed)))
    {
      /* Obj wasn't successfully added to packed, so clean it up otherwise its
       * links will be leaked. When we use constructor/destructors properly, we
       * can remove these. */
      obj->fini ();
      واپس 0;
    }

    objidx = packed.لمبائی - 1;

    اگر (share) packed_map.سیٹ_with_hash (obj, hash, objidx);
    propagate_error (packed_map);

    واپس objidx;
  }

  خالی revert (snapshot_t snap)
  {
    // Overflows that happened after the snapshot will be erased by the revert.
    اگر (unlikely (اندر_error () && !only_حد_سے_تجاوز ())) واپس;
    أكد (snap.current == current);
    اگر (current)
    {
      current->real_links.shrink (snap.عدد_real_links);
      current->virtual_links.shrink (snap.عدد_virtual_links);
    }
    errors = snap.errors;
    revert (snap.سر_2, snap.دم);
  }

  خالی revert (حرف *snap_سر,
	       حرف *snap_دم)
  {
    اگر (unlikely (اندر_error ())) واپس;
    أكد (snap_سر <= سر_2);
    أكد (دم <= snap_دم);
    سر_2 = snap_سر;
    دم = snap_دم;
    discard_stale_objects ();
  }

  خالی discard_stale_objects ()
  {
    اگر (unlikely (اندر_error ())) واپس;
    جب (packed.لمبائی > 1 &&
	   packed.دم ()->سر_2 < دم)
    {
      object_t *obj = packed.دم ();
      packed_map.del (obj);
      أكد (!obj->next);
      obj->fini ();
      object_pool.release (obj);
      packed.نکالو ();
    }
    اگر (packed.لمبائی > 1)
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
  خالی جوڑو_virtual_link (objidx_t objidx)
  {
    اگر (unlikely (اندر_error ())) واپس;

    اگر (!objidx)
      واپس;

    أكد (current);

    اگر (!current->جوڑو_virtual_link(objidx))
      err (HB_SERIALIZE_ERROR_OTHER);
  }

  objidx_t آخری_added_بچہ_اشاریہ() ثابت {
    اگر (unlikely (اندر_error ())) واپس (objidx_t) -1;

    أكد (current);
    اگر (!منطقی(current->real_links)) {
      واپس (objidx_t) -1;
    }

    واپس current->real_links[current->real_links.لمبائی - 1].objidx;
  }

  // For the current object ensure that the sub-table bytes for child objidx are always placed
  // after the subtable bytes for any other existing children. This only ensures that the
  // repacker will not move the target subtable before the other children
  // (by adding virtual links). It is up to the caller to ensure the initial serialization
  // order is correct.
  خالی repack_آخری(objidx_t objidx) {
    اگر (unlikely (اندر_error ())) واپس;

    اگر (!objidx)
      واپس;

    أكد (current);
    برائے (خودکار& l : current->real_links) {
      اگر (l.objidx == objidx) {
        جاری;
      }

      packed[l.objidx]->جوڑو_virtual_link(objidx);
    }
  }

  سانچہ <قسم_نام T>
  خالی جوڑو_link (T &ofs, objidx_t objidx,
		 whence_t whence = سر,
		 غیرعلامتی bias = 0)
  {
    اگر (unlikely (اندر_error ())) واپس;

    اگر (!objidx)
      واپس;

    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) &ofs);

    خودکار& link = *current->real_links.push ();
    اگر (current->real_links.اندر_error ())
      err (HB_SERIALIZE_ERROR_OTHER);

    link.چوڑائی = حجم (T);
    link.objidx = objidx;
    اگر (unlikely (!حجم (T)))
    {
      // This link is not associated with an actual offset and exists merely to enforce
      // an ordering constraint.
      link.is_signed = 0;
      link.whence = 0;
      link.position = 0;
      link.bias = 0;
      واپس;
    }

    link.is_signed = std::is_signed<hb_unwrap_type (T)>::value;
    link.whence = (غیرعلامتی) whence;
    link.position = (ثابت حرف *) &ofs - current->سر_2;
    link.bias = bias;
  }

  غیرعلامتی مقصد_bias (ثابت خالی *بنیاد) ثابت
  {
    اگر (unlikely (اندر_error ())) واپس 0;
    اگر (!بنیاد) واپس 0;
    أكد (current);
    أكد (current->سر_2 <= (ثابت حرف *) بنیاد);
    واپس (ثابت حرف *) بنیاد - current->سر_2;
  }

  خالی resolve_links ()
  {
    اگر (unlikely (اندر_error ())) واپس;

    أكد (!current);
    أكد (packed.لمبائی > 1);

    برائے (ثابت object_t* والد : ++hb_iter (packed))
      برائے (ثابت object_t::link_t &link : والد->real_links)
      {
	ثابت object_t* بچہ = packed[link.objidx];
	اگر (unlikely (!بچہ)) { err (HB_SERIALIZE_ERROR_OTHER); واپس; }
	غیرعلامتی انحراف = 0;
	انتخاب ((whence_t) link.whence) {
	حالت سر:     انحراف = بچہ->سر_2 - والد->سر_2; توڑو;
	حالت دم_2:     انحراف = بچہ->سر_2 - والد->دم; توڑو;
	حالت Absolute: انحراف = (سر_2 - آغاز) + (بچہ->سر_2 - دم); توڑو;
	}

	أكد (انحراف >= link.bias);
	انحراف -= link.bias;
	اگر (link.is_signed)
	{
	  أكد (link.چوڑائی == 2 || link.چوڑائی == 4);
	  اگر (link.چوڑائی == 4)
	    تفویض_انحراف<int32_t> (والد, link, انحراف);
	  ورنہ
	    تفویض_انحراف<int16_t> (والد, link, انحراف);
	}
	ورنہ
	{
	  أكد (link.چوڑائی == 2 || link.چوڑائی == 3 || link.چوڑائی == 4);
	  اگر (link.چوڑائی == 4)
	    تفویض_انحراف<uint32_t> (والد, link, انحراف);
	  ورنہ اگر (link.چوڑائی == 3)
	    تفویض_انحراف<uint32_t, 3> (والد, link, انحراف);
	  ورنہ
	    تفویض_انحراف<uint16_t> (والد, link, انحراف);
	}
      }
  }

  غیرعلامتی صحیح لمبائی () ثابت
  {
    اگر (unlikely (!current)) واپس 0;
    واپس یہ->سر_2 - current->سر_2;
  }

  خالی align (غیرعلامتی صحیح alignment)
  {
    غیرعلامتی صحیح l = لمبائی () % alignment;
    اگر (l)
      (خالی) allocate_سائز<خالی> (alignment - l);
  }

  سانچہ <قسم_نام Type = خالی>
  __attribute__((returns_nonnull))
  Type *آغاز_embed (ثابت Type *obj HB_غیر_مستعمل = خالی_اشاریہ) ثابت
  { واپس reinterpret_cast<Type *> (یہ->سر_2); }
  سانچہ <قسم_نام Type>
  __attribute__((returns_nonnull))
  Type *آغاز_embed (ثابت Type &obj) ثابت
  { واپس آغاز_embed (std::addressof (obj)); }

  منطقی err (hb_serialize_error_t err_type)
  {
    واپس !منطقی ((errors = (errors | err_type)));
  }

  منطقی آغاز_zerocopy (size_t سائز)
  {
    اگر (unlikely (اندر_error ())) واپس جھوٹ;

    اگر (unlikely (سائز > زیادہ_سے_زیادہ_عدد || یہ->دم - یہ->سر_2 < ptrdiff_t (سائز)))
    {
      err (HB_SERIALIZE_ERROR_برآمد_کا_گنجائش);
      واپس جھوٹ;
    }

    أكد (!یہ->zerocopy);
    یہ->zerocopy = یہ->سر_2;

    أكد (یہ->current->سر_2 == یہ->سر_2);
    یہ->current->سر_2 = یہ->current->دم = یہ->سر_2 = یہ->دم - سائز;
    واپس سچ;
  }

  سانچہ <قسم_نام Type>
  HB_NODISCARD
  Type *allocate_سائز (size_t سائز, منطقی clear = سچ)
  {
    اگر (unlikely (اندر_error ())) واپس خالی_اشاریہ;

    اگر (unlikely (سائز > زیادہ_سے_زیادہ_عدد || یہ->دم - یہ->سر_2 < ptrdiff_t (سائز)))
    {
      err (HB_SERIALIZE_ERROR_برآمد_کا_گنجائش);
      واپس خالی_اشاریہ;
    }
    اگر (clear)
      hb_memset (یہ->سر_2, 0, سائز);
    حرف *ret = یہ->سر_2;
    یہ->سر_2 += سائز;
    واپس reinterpret_cast<Type *> (ret);
  }

  سانچہ <قسم_نام Type>
  Type *allocate_min ()
  { واپس یہ->allocate_سائز<Type> (Type::min_سائز); }

  سانچہ <قسم_نام Type>
  HB_NODISCARD
  Type *embed (ثابت Type *obj)
  {
    size_t سائز = obj->get_سائز ();
    Type *ret = یہ->allocate_سائز<Type> (سائز, جھوٹ);
    اگر (unlikely (!ret)) واپس خالی_اشاریہ;
    hb_memcpy (ret, obj, سائز);
    واپس ret;
  }
  سانچہ <قسم_نام Type>
  HB_NODISCARD
  Type *embed (ثابت Type &obj)
  { واپس embed (std::addressof (obj)); }
  حرف *embed (ثابت حرف *obj, غیرعلامتی سائز)
  {
    حرف *ret = یہ->allocate_سائز<حرف> (سائز, جھوٹ);
    اگر (unlikely (!ret)) واپس خالی_اشاریہ;
    hb_memcpy (ret, obj, سائز);
    واپس ret;
  }

  سانچہ <قسم_نام Type, قسم_نام ...Ts> خودکار
  _copy (ثابت Type &src, hb_ترجیح<1>, Ts&&... ds) HB_RETURN
  (Type *, src.copy (یہ, std::forward<Ts> (ds)...))

  سانچہ <قسم_نام Type> خودکار
  _copy (ثابت Type &src, hb_ترجیح<0>) -> decltype (&(hb_declval<Type> () = src))
  {
    Type *ret = یہ->allocate_سائز<Type> (حجم (Type));
    اگر (unlikely (!ret)) واپس خالی_اشاریہ;
    *ret = src;
    واپس ret;
  }

  /* Like embed, but active: calls obj.operator=() or obj.copy() to transfer data
   * instead of hb_memcpy(). */
  سانچہ <قسم_نام Type, قسم_نام ...Ts>
  Type *copy (ثابت Type &src, Ts&&... ds)
  { واپس _copy (src, hb_prioritize, std::forward<Ts> (ds)...); }
  سانچہ <قسم_نام Type, قسم_نام ...Ts>
  Type *copy (ثابت Type *src, Ts&&... ds)
  { واپس copy (*src, std::forward<Ts> (ds)...); }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_iterator (Iterator)),
	   قسم_نام ...Ts>
  خالی copy_all (Iterator it, Ts&&... ds)
  { برائے (decltype (*it) _ : it) copy (_, ds...); }

  سانچہ <قسم_نام Type>
  hb_serialize_context_t& عامل << (ثابت Type &obj) & { embed (obj); واپس *یہ; }

  سانچہ <قسم_نام Type>
  Type *extend_سائز (Type *obj, size_t سائز, منطقی clear = سچ)
  {
    اگر (unlikely (اندر_error ())) واپس خالی_اشاریہ;
    اگر (unlikely (سائز >= زیادہ_سے_زیادہ_عدد)) { err (HB_SERIALIZE_ERROR_OTHER); واپس خالی_اشاریہ; }

    أكد (یہ->آغاز <= (حرف *) obj);
    أكد ((حرف *) obj <= یہ->سر_2);
    أكد ((size_t) (یہ->سر_2 - (حرف *) obj) <= سائز);
    اگر (unlikely (((حرف *) obj + سائز < (حرف *) obj) ||
		  !یہ->allocate_سائز<Type> (((حرف *) obj) + سائز - یہ->سر_2, clear))) واپس خالی_اشاریہ;
    واپس reinterpret_cast<Type *> (obj);
  }
  سانچہ <قسم_نام Type>
  Type *extend_سائز (Type &obj, size_t سائز, منطقی clear = سچ)
  { واپس extend_سائز (std::addressof (obj), سائز, clear); }

  سانچہ <قسم_نام Type>
  Type *extend_min (Type *obj) { واپس extend_سائز (obj, obj->min_سائز); }
  سانچہ <قسم_نام Type>
  Type *extend_min (Type &obj) { واپس extend_min (std::addressof (obj)); }

  سانچہ <قسم_نام Type, قسم_نام ...Ts>
  Type *extend (Type *obj, Ts&&... ds)
  { واپس extend_سائز (obj, obj->get_سائز (std::forward<Ts> (ds)...)); }
  سانچہ <قسم_نام Type, قسم_نام ...Ts>
  Type *extend (Type &obj, Ts&&... ds)
  { واپس extend (std::addressof (obj), std::forward<Ts> (ds)...); }

  /* Output routines. */
  hb_بائٹیں_t copy_بائٹیں () ثابت
  {
    أكد (successful ());
    /* Copy both items from head side and tail side... */
    غیرعلامتی صحیح len = (یہ->سر_2 - یہ->آغاز)
		     + (یہ->انتہا  - یہ->دم);

    // If len is zero don't hb_malloc as the memory won't get properly
    // cleaned up later.
    اگر (!len) واپس hb_بائٹیں_t ();

    حرف *p = (حرف *) hb_malloc (len);
    اگر (unlikely (!p)) واپس hb_بائٹیں_t ();

    hb_memcpy (p, یہ->آغاز, یہ->سر_2 - یہ->آغاز);
    hb_memcpy (p + (یہ->سر_2 - یہ->آغاز), یہ->دم, یہ->انتہا - یہ->دم);
    واپس hb_بائٹیں_t (p, len);
  }
  سانچہ <قسم_نام Type>
  Type *copy () ثابت
  { واپس reinterpret_cast<Type *> ((حرف *) copy_بائٹیں ().arrayZ); }
  hb_blob_t *copy_blob () ثابت
  {
    hb_بائٹیں_t b = copy_بائٹیں ();
    واپس hb_blob_create (b.arrayZ, b.لمبائی,
			   HB_حافظہ_منوال_WRITABLE,
			   (حرف *) b.arrayZ, hb_free);
  }

  ثابت hb_سمتیہ_t<object_t *>& object_گراف() ثابت
  { واپس packed; }

  نجی:
  سانچہ <قسم_نام T, غیرعلامتی سائز_2 = حجم (T)>
  خالی تفویض_انحراف (ثابت object_t* والد, ثابت object_t::link_t &link, غیرعلامتی انحراف)
  {
    // XXX We should stop assuming big-endian!
    خودکار &off = * ((HBInt<سچ, T, سائز_2> *) (والد->سر_2 + link.position));
    أكد (0 == off);
    check_تفویض (off, انحراف, HB_SERIALIZE_ERROR_انحراف_حد_سے_تجاوز);
  }

  عوامی:
  حرف *آغاز, *سر_2, *دم, *انتہا, *zerocopy;
  غیرعلامتی صحیح debug_گہرائی;
  hb_serialize_error_t errors;

  نجی:

  خالی ملاؤ_virtual_links (ثابت object_t* from, objidx_t مقصد_idx) {
    object_t* to = packed[مقصد_idx];
    برائے (ثابت خودکار& l : from->virtual_links) {
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

#اگر_ختم /* HB_SERIALIZE_HH */
