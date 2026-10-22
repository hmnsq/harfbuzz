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

#جے_بنا_تعریف گراف_SERIALIZE_HH
#تعریف_کرو گراف_SERIALIZE_HH

#ضمن "graph-result.hh"

نطاق گراف {

بنية حد_توں_ودھ_ریکارڈ_t
{
  بنانشانی ماپا;
  بنانشانی بچہ;

  منطقي عامل != (ثابت حد_توں_ودھ_ریکارڈ_t o) ثابت
  { أرجع !(*هذا == o); }

  مضمن منطقي عامل == (ثابت حد_توں_ودھ_ریکارڈ_t& o) ثابت
  {
    أرجع ماپا == o.ماپا &&
        بچہ == o.بچہ;
  }

  مضمن uint32_t hash () ثابت
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (ماپا);
    current = current * 31 + hb_hash (بچہ);
    أرجع current;
  }
};

مضمن
int64_t compute_وتھ (
    ثابت گراف_t& گراف,
    بنانشانی ماپا_idx,
    ثابت hb_serialize_context_t::object_t::link_t& link)
{
  ثابت خودکار& ماپا = گراف.vertices_[ماپا_idx];
  ثابت خودکار& بچہ = گراف.vertices_[link.objidx];
  int64_t وتھ = 0;
  بدل ((hb_serialize_context_t::whence_t) link.whence) {
    حالة hb_serialize_context_t::whence_t::سر:
      وتھ = بچہ.شروع - ماپا.شروع; اكسر;
    حالة hb_serialize_context_t::whence_t::پوچھ_2:
      وتھ = بچہ.شروع - ماپا.سرا; اكسر;
    حالة hb_serialize_context_t::whence_t::Absolute:
      وتھ = بچہ.شروع; اكسر;
  }

  أكد (وتھ >= link.bias);
  وتھ -= link.bias;
  أرجع وتھ;
}

مضمن
منطقي is_valid_وتھ (int64_t وتھ,
                      ثابت hb_serialize_context_t::object_t::link_t& link)
{
  إذا (unlikely (!link.چوڑائی))
    // Virtual links can't overflow.
    أرجع link.is_signed || وتھ >= 0;

  إذا (link.is_signed)
  {
    إذا (link.چوڑائی == 4)
      أرجع وتھ >= -((int64_t) 1 << 31) && وتھ < ((int64_t) 1 << 31);
    وإلا
      أرجع وتھ >= -(1 << 15) && وتھ < (1 << 15);
  }
  وإلا
  {
    إذا (link.چوڑائی == 4)
      أرجع وتھ >= 0 && وتھ < ((int64_t) 1 << 32);
    وإلا إذا (link.چوڑائی == 3)
      أرجع وتھ >= 0 && وتھ < ((int32_t) 1 << 24);
    وإلا
      أرجع وتھ >= 0 && وتھ < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
مضمن گراف_نتیجہ_t<منطقي>
will_حد_توں_ودھ (گراف_t& گراف,
               hb_سمتیہ_t<حد_توں_ودھ_ریکارڈ_t>* overflows = خالی_سوچک)
{
  إذا (overflows) overflows->clear ();
  گراف.update_positions ();

  hb_hashmap_t<حد_توں_ودھ_ریکارڈ_t, منطقي> ریکارڈ_سیٹ;
  ثابت خودکار& vertices = گراف.vertices_;
  لكل (بنانشانی ماپا_idx : گراف.ordering_)
  {
    // Don't need to check virtual links for overflow
    لكل (ثابت خودکار& link : vertices.arrayZ[ماپا_idx].obj ().real_links)
    {
      int64_t وتھ = compute_وتھ (گراف, ماپا_idx, link);
      إذا (likely (is_valid_وتھ (وتھ, link)))
        استمر;

      إذا (!overflows) أرجع Ok(سچ);

      حد_توں_ودھ_ریکارڈ_t r;
      r.ماپا = ماپا_idx;
      r.بچہ = link.objidx;
      إذا (ریکارڈ_سیٹ.has(r)) استمر; // don't keep duplicate overflows.

      overflows->push (r);
      ریکارڈ_سیٹ.سیٹ (r, سچ);
    }
  }

  TRY (گراف_نتیجہ_t<فراغ>::from (ریکارڈ_سیٹ, ونڈ_FAILURE));
  إذا (overflows)
    TRY (گراف_نتیجہ_t<فراغ>::from (*overflows, ونڈ_FAILURE));

  أرجع Ok((منطقي) (overflows && overflows->لمائی));
}

مضمن
فراغ print_overflows (گراف_t& گراف,
                      ثابت hb_سمتیہ_t<حد_توں_ودھ_ریکارڈ_t>& overflows)
{
  إذا (!DEBUG_ENABLED(SUBSET_REPACK)) أرجع;

  خودکار r = گراف.update_parents ();
  إذا (r.is_err()) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    أرجع;
  }

  صحيح حد_بندی = 10;
  لكل (ثابت خودکار& o : overflows)
  {
    إذا (!حد_بندی--) اكسر;
    ثابت خودکار& ماپا = گراف.vertices_[o.ماپا];
    ثابت خودکار& بچہ = گراف.vertices_[o.بچہ];
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک,
               "  overflow from "
               "%4u (%4u in, %4u out, space %2u) => "
               "%4u (%4u in, %4u out, space %2u)",
               o.ماپا,
               ماپا.incoming_کنارے_2 (),
               ماپا.obj ().real_links.لمائی + ماپا.obj ().virtual_links.لمائی,
               گراف.space_for (o.ماپا),
               o.بچہ,
               بچہ.incoming_کنارے_2 (),
               بچہ.obj ().real_links.لمائی + بچہ.obj ().virtual_links.لمائی,
               گراف.space_for (o.بچہ));
  }
  إذا (overflows.لمائی > 10) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "  ... plus %u more overflows.", overflows.لمائی - 10);
  }
}

سانچا <قسم_ناں O> مضمن فراغ
serialize_link_دا_type (ثابت hb_serialize_context_t::object_t::link_t& link,
                        حرف* سر_2,
                        بنانشانی ناپ,
                        ثابت hb_سمتیہ_t<بنانشانی>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  إذا (unlikely (link.position + link.چوڑائی > ناپ))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    أرجع;
  }

  OT::وتھ_2<O>* وتھ = reinterpret_cast<OT::وتھ_2<O>*> (سر_2 + link.position);
  *وتھ = 0;
  c->جوڑو_link (*وتھ,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

مضمن
فراغ serialize_link (ثابت hb_serialize_context_t::object_t::link_t& link,
                     حرف* سر_2,
                     بنانشانی ناپ,
                     ثابت hb_سمتیہ_t<بنانشانی>& id_map,
                     hb_serialize_context_t* c)
{
  بدل (link.چوڑائی)
  {
    حالة 0:
      // Virtual links aren't serialized.
      أرجع;
    حالة 4:
      إذا (link.is_signed)
      {
        serialize_link_دا_type<OT::HBINT32> (link, سر_2, ناپ, id_map, c);
      } وإلا {
        serialize_link_دا_type<OT::HBUINT32> (link, سر_2, ناپ, id_map, c);
      }
      أرجع;
    حالة 2:
      إذا (link.is_signed)
      {
        serialize_link_دا_type<OT::HBINT16> (link, سر_2, ناپ, id_map, c);
      } وإلا {
        serialize_link_دا_type<OT::HBUINT16> (link, سر_2, ناپ, id_map, c);
      }
      أرجع;
    حالة 3:
      serialize_link_دا_type<OT::HBUINT24> (link, سر_2, ناپ, id_map, c);
      أرجع;
    افتراضي:
      // Unexpected link width.
      أكد (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
مضمن گراف_نتیجہ_t<hb_blob_t*> serialize (ثابت گراف_t& گراف)
{
  hb_سمتیہ_t<حرف> بفر;
  size_t ناپ = گراف.کل_ناپ_وچ_بائٹاں ();

  إذا (!ناپ) أرجع Ok(hb_blob_get_خالی_تھاواں ());

  إذا (unlikely (!بفر.alloc (ناپ))) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Unable to allocate output buffer.");
    أرجع Err(ونڈ_FAILURE);
  }
  hb_serialize_context_t c((فراغ *) بفر, ناپ);

  c.شروع_serialize<فراغ> ();
  ثابت خودکار& vertices = گراف.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_سمتیہ_t<بنانشانی> id_map;
  إذا (unlikely (!id_map.resize(گراف.ordering_.لمائی))) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Unable to allocate id_map buffer.");
    أرجع Err(ونڈ_FAILURE);
  }

  لكل (صحيح pos = گراف.ordering_.لمائی - 1; pos >= 0; pos--) {
    بنانشانی i = گراف.ordering_[pos];
    c.push ();

    خودکار& v = vertices[i];

    size_t ناپ = v.جدول_ناپ ();

    حرف* شروع = c.allocate_ناپ <حرف> (ناپ);
    إذا (unlikely (!شروع)) {
      DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Buffer out of space.");
      أرجع Err(ونڈ_FAILURE);
    }

    hb_memcpy (شروع, v.obj ().سر_2, ناپ);

    // Only real links needs to be serialized.
    لكل (ثابت خودکار& link : v.obj ().real_links)
      serialize_link (link, شروع, ناپ, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.نکالو_pack (خطأ);
  }
  c.سرا_serialize ();

  إذا (unlikely (c.وچ_error ())) {
    DEBUG_پیغام (SUBSET_REPACK, خالی_سوچک, "Error during serialization. Err flag: %d",
               c.errors);
    إذا (c.errors & HB_SERIALIZE_ERROR_وتھ_حد_توں_ودھ)
      أرجع Err(حد_توں_ودھ_RESOLUTION_FAILED);
    أرجع Err(ونڈ_FAILURE);
  }

  خودکار* blob = c.copy_blob ();
  إذا (unlikely (!blob)) أرجع Err(ونڈ_FAILURE);
  أرجع Ok(blob);
}

} // namespace graph

#جے_انت // GRAPH_SERIALIZE_HH
