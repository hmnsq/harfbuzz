/*
 * Copyright © 2022  Google, Inc.
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

#اگر_تعریف_نشده گراف_SERIALIZE_HH
#تعریف_کن گراف_SERIALIZE_HH

#ضمن "graph-result.hh"

نطاق گراف {

بنية سرریز_رکورد_t
{
  بیعلامت والد;
  بیعلامت فرزند;

  منطقي عملگر != (ثابت سرریز_رکورد_t o) ثابت
  { أرجع !(*هذا == o); }

  مضمن منطقي عملگر == (ثابت سرریز_رکورد_t& o) ثابت
  {
    أرجع والد == o.والد &&
        فرزند == o.فرزند;
  }

  مضمن uint32_t hash () ثابت
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (والد);
    current = current * 31 + hb_hash (فرزند);
    أرجع current;
  }
};

مضمن
int64_t compute_جابه_جایی (
    ثابت گراف_t& گراف,
    بیعلامت والد_idx,
    ثابت hb_serialize_context_t::object_t::link_t& link)
{
  ثابت خودکار& والد = گراف.vertices_[والد_idx];
  ثابت خودکار& فرزند = گراف.vertices_[link.objidx];
  int64_t جابه_جایی = 0;
  بدل ((hb_serialize_context_t::whence_t) link.whence) {
    حالة hb_serialize_context_t::whence_t::سر:
      جابه_جایی = فرزند.شروع - والد.شروع; اكسر;
    حالة hb_serialize_context_t::whence_t::دم_2:
      جابه_جایی = فرزند.شروع - والد.انتها; اكسر;
    حالة hb_serialize_context_t::whence_t::Absolute:
      جابه_جایی = فرزند.شروع; اكسر;
  }

  أكد (جابه_جایی >= link.bias);
  جابه_جایی -= link.bias;
  أرجع جابه_جایی;
}

مضمن
منطقي is_valid_جابه_جایی (int64_t جابه_جایی,
                      ثابت hb_serialize_context_t::object_t::link_t& link)
{
  إذا (unlikely (!link.عرض))
    // Virtual links can't overflow.
    أرجع link.is_signed || جابه_جایی >= 0;

  إذا (link.is_signed)
  {
    إذا (link.عرض == 4)
      أرجع جابه_جایی >= -((int64_t) 1 << 31) && جابه_جایی < ((int64_t) 1 << 31);
    وإلا
      أرجع جابه_جایی >= -(1 << 15) && جابه_جایی < (1 << 15);
  }
  وإلا
  {
    إذا (link.عرض == 4)
      أرجع جابه_جایی >= 0 && جابه_جایی < ((int64_t) 1 << 32);
    وإلا إذا (link.عرض == 3)
      أرجع جابه_جایی >= 0 && جابه_جایی < ((int32_t) 1 << 24);
    وإلا
      أرجع جابه_جایی >= 0 && جابه_جایی < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
مضمن گراف_نتیجه_t<منطقي>
will_سرریز (گراف_t& گراف,
               hb_وکتور_t<سرریز_رکورد_t>* overflows = اشاره_تهی)
{
  إذا (overflows) overflows->clear ();
  گراف.update_positions ();

  hb_hashmap_t<سرریز_رکورد_t, منطقي> رکورد_مجموعه;
  ثابت خودکار& vertices = گراف.vertices_;
  لكل (بیعلامت والد_idx : گراف.ordering_)
  {
    // Don't need to check virtual links for overflow
    لكل (ثابت خودکار& link : vertices.arrayZ[والد_idx].obj ().real_links)
    {
      int64_t جابه_جایی = compute_جابه_جایی (گراف, والد_idx, link);
      إذا (likely (is_valid_جابه_جایی (جابه_جایی, link)))
        استمر;

      إذا (!overflows) أرجع Ok(درست);

      سرریز_رکورد_t r;
      r.والد = والد_idx;
      r.فرزند = link.objidx;
      إذا (رکورد_مجموعه.has(r)) استمر; // don't keep duplicate overflows.

      overflows->push (r);
      رکورد_مجموعه.مجموعه (r, درست);
    }
  }

  TRY (گراف_نتیجه_t<فراغ>::from (رکورد_مجموعه, تخصیص_FAILURE));
  إذا (overflows)
    TRY (گراف_نتیجه_t<فراغ>::from (*overflows, تخصیص_FAILURE));

  أرجع Ok((منطقي) (overflows && overflows->درازی));
}

مضمن
فراغ print_overflows (گراف_t& گراف,
                      ثابت hb_وکتور_t<سرریز_رکورد_t>& overflows)
{
  إذا (!DEBUG_ENABLED(SUBSET_REPACK)) أرجع;

  خودکار r = گراف.update_parents ();
  إذا (r.is_err()) {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    أرجع;
  }

  صحيح حد = 10;
  لكل (ثابت خودکار& o : overflows)
  {
    إذا (!حد--) اكسر;
    ثابت خودکار& والد = گراف.vertices_[o.والد];
    ثابت خودکار& فرزند = گراف.vertices_[o.فرزند];
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی,
               "  overflow from "
               "%4u (%4u in, %4u out, space %2u) => "
               "%4u (%4u in, %4u out, space %2u)",
               o.والد,
               والد.incoming_یالها_2 (),
               والد.obj ().real_links.درازی + والد.obj ().virtual_links.درازی,
               گراف.space_for (o.والد),
               o.فرزند,
               فرزند.incoming_یالها_2 (),
               فرزند.obj ().real_links.درازی + فرزند.obj ().virtual_links.درازی,
               گراف.space_for (o.فرزند));
  }
  إذا (overflows.درازی > 10) {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "  ... plus %u more overflows.", overflows.درازی - 10);
  }
}

قالب <نام_نوع O> مضمن فراغ
serialize_link_از_type (ثابت hb_serialize_context_t::object_t::link_t& link,
                        حرف* سر_2,
                        بیعلامت اندازه,
                        ثابت hb_وکتور_t<بیعلامت>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  إذا (unlikely (link.position + link.عرض > اندازه))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    أرجع;
  }

  OT::جابه_جایی_2<O>* جابه_جایی = reinterpret_cast<OT::جابه_جایی_2<O>*> (سر_2 + link.position);
  *جابه_جایی = 0;
  c->جمع_کن_link (*جابه_جایی,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

مضمن
فراغ serialize_link (ثابت hb_serialize_context_t::object_t::link_t& link,
                     حرف* سر_2,
                     بیعلامت اندازه,
                     ثابت hb_وکتور_t<بیعلامت>& id_map,
                     hb_serialize_context_t* c)
{
  بدل (link.عرض)
  {
    حالة 0:
      // Virtual links aren't serialized.
      أرجع;
    حالة 4:
      إذا (link.is_signed)
      {
        serialize_link_از_type<OT::HBINT32> (link, سر_2, اندازه, id_map, c);
      } وإلا {
        serialize_link_از_type<OT::HBUINT32> (link, سر_2, اندازه, id_map, c);
      }
      أرجع;
    حالة 2:
      إذا (link.is_signed)
      {
        serialize_link_از_type<OT::HBINT16> (link, سر_2, اندازه, id_map, c);
      } وإلا {
        serialize_link_از_type<OT::HBUINT16> (link, سر_2, اندازه, id_map, c);
      }
      أرجع;
    حالة 3:
      serialize_link_از_type<OT::HBUINT24> (link, سر_2, اندازه, id_map, c);
      أرجع;
    افتراضي:
      // Unexpected link width.
      أكد (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
مضمن گراف_نتیجه_t<hb_blob_t*> serialize (ثابت گراف_t& گراف)
{
  hb_وکتور_t<حرف> میانگیر;
  size_t اندازه = گراف.مجموع_اندازه_درون_بایتها ();

  إذا (!اندازه) أرجع Ok(hb_blob_get_خالی ());

  إذا (unlikely (!میانگیر.alloc (اندازه))) {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Unable to allocate output buffer.");
    أرجع Err(تخصیص_FAILURE);
  }
  hb_serialize_context_t c((فراغ *) میانگیر, اندازه);

  c.شروع_serialize<فراغ> ();
  ثابت خودکار& vertices = گراف.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_وکتور_t<بیعلامت> id_map;
  إذا (unlikely (!id_map.resize(گراف.ordering_.درازی))) {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Unable to allocate id_map buffer.");
    أرجع Err(تخصیص_FAILURE);
  }

  لكل (صحيح pos = گراف.ordering_.درازی - 1; pos >= 0; pos--) {
    بیعلامت i = گراف.ordering_[pos];
    c.push ();

    خودکار& v = vertices[i];

    size_t اندازه = v.جدول_اندازه ();

    حرف* شروع = c.allocate_اندازه <حرف> (اندازه);
    إذا (unlikely (!شروع)) {
      DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Buffer out of space.");
      أرجع Err(تخصیص_FAILURE);
    }

    hb_memcpy (شروع, v.obj ().سر_2, اندازه);

    // Only real links needs to be serialized.
    لكل (ثابت خودکار& link : v.obj ().real_links)
      serialize_link (link, شروع, اندازه, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.برداشت_pack (خطأ);
  }
  c.انتها_serialize ();

  إذا (unlikely (c.درون_error ())) {
    DEBUG_پیام (SUBSET_REPACK, اشاره_تهی, "Error during serialization. Err flag: %d",
               c.errors);
    إذا (c.errors & HB_SERIALIZE_ERROR_جابه_جایی_سرریز)
      أرجع Err(سرریز_RESOLUTION_FAILED);
    أرجع Err(تخصیص_FAILURE);
  }

  خودکار* blob = c.copy_blob ();
  إذا (unlikely (!blob)) أرجع Err(تخصیص_FAILURE);
  أرجع Ok(blob);
}

} // namespace graph

#پایان_اگر // GRAPH_SERIALIZE_HH
