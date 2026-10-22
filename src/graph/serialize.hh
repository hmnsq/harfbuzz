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

#اگر_غیر_تعریف_شدہ گراف_SERIALIZE_HH
#تعریف_کرو گراف_SERIALIZE_HH

#شامل "graph-result.hh"

نامخلا گراف {

ساخت حد_سے_تجاوز_ریکارڈ_t
{
  غیرعلامتی والد;
  غیرعلامتی بچہ;

  منطقی عامل != (ثابت حد_سے_تجاوز_ریکارڈ_t o) ثابت
  { واپس !(*یہ == o); }

  اندرونی منطقی عامل == (ثابت حد_سے_تجاوز_ریکارڈ_t& o) ثابت
  {
    واپس والد == o.والد &&
        بچہ == o.بچہ;
  }

  اندرونی uint32_t hash () ثابت
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (والد);
    current = current * 31 + hb_hash (بچہ);
    واپس current;
  }
};

اندرونی
int64_t compute_انحراف (
    ثابت گراف_t& گراف,
    غیرعلامتی والد_idx,
    ثابت hb_serialize_context_t::object_t::link_t& link)
{
  ثابت خودکار& والد = گراف.vertices_[والد_idx];
  ثابت خودکار& بچہ = گراف.vertices_[link.objidx];
  int64_t انحراف = 0;
  انتخاب ((hb_serialize_context_t::whence_t) link.whence) {
    حالت hb_serialize_context_t::whence_t::سر:
      انحراف = بچہ.آغاز - والد.آغاز; توڑو;
    حالت hb_serialize_context_t::whence_t::دم_2:
      انحراف = بچہ.آغاز - والد.انتہا; توڑو;
    حالت hb_serialize_context_t::whence_t::Absolute:
      انحراف = بچہ.آغاز; توڑو;
  }

  أكد (انحراف >= link.bias);
  انحراف -= link.bias;
  واپس انحراف;
}

اندرونی
منطقی is_valid_انحراف (int64_t انحراف,
                      ثابت hb_serialize_context_t::object_t::link_t& link)
{
  اگر (unlikely (!link.چوڑائی))
    // Virtual links can't overflow.
    واپس link.is_signed || انحراف >= 0;

  اگر (link.is_signed)
  {
    اگر (link.چوڑائی == 4)
      واپس انحراف >= -((int64_t) 1 << 31) && انحراف < ((int64_t) 1 << 31);
    ورنہ
      واپس انحراف >= -(1 << 15) && انحراف < (1 << 15);
  }
  ورنہ
  {
    اگر (link.چوڑائی == 4)
      واپس انحراف >= 0 && انحراف < ((int64_t) 1 << 32);
    ورنہ اگر (link.چوڑائی == 3)
      واپس انحراف >= 0 && انحراف < ((int32_t) 1 << 24);
    ورنہ
      واپس انحراف >= 0 && انحراف < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
اندرونی گراف_نتیجہ_t<منطقی>
will_حد_سے_تجاوز (گراف_t& گراف,
               hb_سمتیہ_t<حد_سے_تجاوز_ریکارڈ_t>* overflows = خالی_اشاریہ)
{
  اگر (overflows) overflows->clear ();
  گراف.update_positions ();

  hb_hashmap_t<حد_سے_تجاوز_ریکارڈ_t, منطقی> ریکارڈ_سیٹ;
  ثابت خودکار& vertices = گراف.vertices_;
  برائے (غیرعلامتی والد_idx : گراف.ordering_)
  {
    // Don't need to check virtual links for overflow
    برائے (ثابت خودکار& link : vertices.arrayZ[والد_idx].obj ().real_links)
    {
      int64_t انحراف = compute_انحراف (گراف, والد_idx, link);
      اگر (likely (is_valid_انحراف (انحراف, link)))
        جاری;

      اگر (!overflows) واپس Ok(سچ);

      حد_سے_تجاوز_ریکارڈ_t r;
      r.والد = والد_idx;
      r.بچہ = link.objidx;
      اگر (ریکارڈ_سیٹ.has(r)) جاری; // don't keep duplicate overflows.

      overflows->push (r);
      ریکارڈ_سیٹ.سیٹ (r, سچ);
    }
  }

  TRY (گراف_نتیجہ_t<خالی>::from (ریکارڈ_سیٹ, تخصیص_FAILURE));
  اگر (overflows)
    TRY (گراف_نتیجہ_t<خالی>::from (*overflows, تخصیص_FAILURE));

  واپس Ok((منطقی) (overflows && overflows->لمبائی));
}

اندرونی
خالی print_overflows (گراف_t& گراف,
                      ثابت hb_سمتیہ_t<حد_سے_تجاوز_ریکارڈ_t>& overflows)
{
  اگر (!DEBUG_ENABLED(SUBSET_REPACK)) واپس;

  خودکار r = گراف.update_parents ();
  اگر (r.is_err()) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    واپس;
  }

  صحیح حد_بندی = 10;
  برائے (ثابت خودکار& o : overflows)
  {
    اگر (!حد_بندی--) توڑو;
    ثابت خودکار& والد = گراف.vertices_[o.والد];
    ثابت خودکار& بچہ = گراف.vertices_[o.بچہ];
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ,
               "  overflow from "
               "%4u (%4u in, %4u out, space %2u) => "
               "%4u (%4u in, %4u out, space %2u)",
               o.والد,
               والد.incoming_کنارے_2 (),
               والد.obj ().real_links.لمبائی + والد.obj ().virtual_links.لمبائی,
               گراف.space_for (o.والد),
               o.بچہ,
               بچہ.incoming_کنارے_2 (),
               بچہ.obj ().real_links.لمبائی + بچہ.obj ().virtual_links.لمبائی,
               گراف.space_for (o.بچہ));
  }
  اگر (overflows.لمبائی > 10) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "  ... plus %u more overflows.", overflows.لمبائی - 10);
  }
}

سانچہ <قسم_نام O> اندرونی خالی
serialize_link_کا_type (ثابت hb_serialize_context_t::object_t::link_t& link,
                        حرف* سر_2,
                        غیرعلامتی سائز,
                        ثابت hb_سمتیہ_t<غیرعلامتی>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  اگر (unlikely (link.position + link.چوڑائی > سائز))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    واپس;
  }

  OT::انحراف_2<O>* انحراف = reinterpret_cast<OT::انحراف_2<O>*> (سر_2 + link.position);
  *انحراف = 0;
  c->جوڑو_link (*انحراف,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

اندرونی
خالی serialize_link (ثابت hb_serialize_context_t::object_t::link_t& link,
                     حرف* سر_2,
                     غیرعلامتی سائز,
                     ثابت hb_سمتیہ_t<غیرعلامتی>& id_map,
                     hb_serialize_context_t* c)
{
  انتخاب (link.چوڑائی)
  {
    حالت 0:
      // Virtual links aren't serialized.
      واپس;
    حالت 4:
      اگر (link.is_signed)
      {
        serialize_link_کا_type<OT::HBINT32> (link, سر_2, سائز, id_map, c);
      } ورنہ {
        serialize_link_کا_type<OT::HBUINT32> (link, سر_2, سائز, id_map, c);
      }
      واپس;
    حالت 2:
      اگر (link.is_signed)
      {
        serialize_link_کا_type<OT::HBINT16> (link, سر_2, سائز, id_map, c);
      } ورنہ {
        serialize_link_کا_type<OT::HBUINT16> (link, سر_2, سائز, id_map, c);
      }
      واپس;
    حالت 3:
      serialize_link_کا_type<OT::HBUINT24> (link, سر_2, سائز, id_map, c);
      واپس;
    طےشدہ:
      // Unexpected link width.
      أكد (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
اندرونی گراف_نتیجہ_t<hb_blob_t*> serialize (ثابت گراف_t& گراف)
{
  hb_سمتیہ_t<حرف> بفر;
  size_t سائز = گراف.کل_سائز_اندر_بائٹیں ();

  اگر (!سائز) واپس Ok(hb_blob_get_خالی_خانے ());

  اگر (unlikely (!بفر.alloc (سائز))) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Unable to allocate output buffer.");
    واپس Err(تخصیص_FAILURE);
  }
  hb_serialize_context_t c((خالی *) بفر, سائز);

  c.آغاز_serialize<خالی> ();
  ثابت خودکار& vertices = گراف.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_سمتیہ_t<غیرعلامتی> id_map;
  اگر (unlikely (!id_map.resize(گراف.ordering_.لمبائی))) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Unable to allocate id_map buffer.");
    واپس Err(تخصیص_FAILURE);
  }

  برائے (صحیح pos = گراف.ordering_.لمبائی - 1; pos >= 0; pos--) {
    غیرعلامتی i = گراف.ordering_[pos];
    c.push ();

    خودکار& v = vertices[i];

    size_t سائز = v.جدول_سائز ();

    حرف* آغاز = c.allocate_سائز <حرف> (سائز);
    اگر (unlikely (!آغاز)) {
      DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Buffer out of space.");
      واپس Err(تخصیص_FAILURE);
    }

    hb_memcpy (آغاز, v.obj ().سر_2, سائز);

    // Only real links needs to be serialized.
    برائے (ثابت خودکار& link : v.obj ().real_links)
      serialize_link (link, آغاز, سائز, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.نکالو_pack (جھوٹ);
  }
  c.انتہا_serialize ();

  اگر (unlikely (c.اندر_error ())) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_اشاریہ, "Error during serialization. Err flag: %d",
               c.errors);
    اگر (c.errors & HB_SERIALIZE_ERROR_انحراف_حد_سے_تجاوز)
      واپس Err(حد_سے_تجاوز_RESOLUTION_FAILED);
    واپس Err(تخصیص_FAILURE);
  }

  خودکار* blob = c.copy_blob ();
  اگر (unlikely (!blob)) واپس Err(تخصیص_FAILURE);
  واپس Ok(blob);
}

} // namespace graph

#اگر_ختم // GRAPH_SERIALIZE_HH
