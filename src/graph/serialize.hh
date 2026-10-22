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

#ifndef GRAPH_SERIALIZE_HH
#define GRAPH_SERIALIZE_HH

#अन्तर्भाव "graph-result.hh"

नामाकाश graph {

संरचना overflow_record_t
{
  अचिन्हित parent;
  अचिन्हित child;

  तार्किक चालक != (ध्रुव overflow_record_t o) ध्रुव
  { प्रत्यागम !(*एतत् == o); }

  अन्तःस्थ तार्किक चालक == (ध्रुव overflow_record_t& o) ध्रुव
  {
    प्रत्यागम parent == o.parent &&
        child == o.child;
  }

  अन्तःस्थ uint32_t hash () ध्रुव
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    प्रत्यागम current;
  }
};

अन्तःस्थ
int64_t compute_offset (
    ध्रुव graph_t& graph,
    अचिन्हित parent_idx,
    ध्रुव hb_serialize_context_t::object_t::link_t& link)
{
  ध्रुव स्वतः& parent = graph.vertices_[parent_idx];
  ध्रुव स्वतः& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  विकल्प ((hb_serialize_context_t::whence_t) link.whence) {
    पक्ष hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; भङ्ग;
    पक्ष hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; भङ्ग;
    पक्ष hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; भङ्ग;
  }

  प्रतिज्ञा (offset >= link.bias);
  offset -= link.bias;
  प्रत्यागम offset;
}

अन्तःस्थ
तार्किक is_valid_offset (int64_t offset,
                      ध्रुव hb_serialize_context_t::object_t::link_t& link)
{
  यदि (unlikely (!link.width))
    // Virtual links can't overflow.
    प्रत्यागम link.is_signed || offset >= 0;

  यदि (link.is_signed)
  {
    यदि (link.width == 4)
      प्रत्यागम offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    अन्यथा
      प्रत्यागम offset >= -(1 << 15) && offset < (1 << 15);
  }
  अन्यथा
  {
    यदि (link.width == 4)
      प्रत्यागम offset >= 0 && offset < ((int64_t) 1 << 32);
    अन्यथा यदि (link.width == 3)
      प्रत्यागम offset >= 0 && offset < ((int32_t) 1 << 24);
    अन्यथा
      प्रत्यागम offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
अन्तःस्थ graph_result_t<तार्किक>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  यदि (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, तार्किक> record_set;
  ध्रुव स्वतः& vertices = graph.vertices_;
  प्रति (अचिन्हित parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    प्रति (ध्रुव स्वतः& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      यदि (likely (is_valid_offset (offset, link)))
        अनुवर्त;

      यदि (!overflows) प्रत्यागम Ok(सत्य);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      यदि (record_set.has(r)) अनुवर्त; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, सत्य);
    }
  }

  TRY (graph_result_t<शून्य>::from (record_set, ALLOCATION_FAILURE));
  यदि (overflows)
    TRY (graph_result_t<शून्य>::from (*overflows, ALLOCATION_FAILURE));

  प्रत्यागम Ok((तार्किक) (overflows && overflows->length));
}

अन्तःस्थ
शून्य print_overflows (graph_t& graph,
                      ध्रुव hb_vector_t<overflow_record_t>& overflows)
{
  यदि (!DEBUG_ENABLED(SUBSET_REPACK)) प्रत्यागम;

  स्वतः r = graph.update_parents ();
  यदि (r.is_err()) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    प्रत्यागम;
  }

  पूर्णाङ्क limit = 10;
  प्रति (ध्रुव स्वतः& o : overflows)
  {
    यदि (!limit--) भङ्ग;
    ध्रुव स्वतः& parent = graph.vertices_[o.parent];
    ध्रुव स्वतः& child = graph.vertices_[o.child];
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  overflow from "
               "%4u (%4u in, %4u out, space %2u) => "
               "%4u (%4u in, %4u out, space %2u)",
               o.parent,
               parent.incoming_edges (),
               parent.obj ().real_links.length + parent.obj ().virtual_links.length,
               graph.space_for (o.parent),
               o.child,
               child.incoming_edges (),
               child.obj ().real_links.length + child.obj ().virtual_links.length,
               graph.space_for (o.child));
  }
  यदि (overflows.length > 10) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

ढाँचा <typename O> अन्तःस्थ शून्य
serialize_link_of_type (ध्रुव hb_serialize_context_t::object_t::link_t& link,
                        वर्ण* head,
                        अचिन्हित size,
                        ध्रुव hb_vector_t<अचिन्हित>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  यदि (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    प्रत्यागम;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

अन्तःस्थ
शून्य serialize_link (ध्रुव hb_serialize_context_t::object_t::link_t& link,
                     वर्ण* head,
                     अचिन्हित size,
                     ध्रुव hb_vector_t<अचिन्हित>& id_map,
                     hb_serialize_context_t* c)
{
  विकल्प (link.width)
  {
    पक्ष 0:
      // Virtual links aren't serialized.
      प्रत्यागम;
    पक्ष 4:
      यदि (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } अन्यथा {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      प्रत्यागम;
    पक्ष 2:
      यदि (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } अन्यथा {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      प्रत्यागम;
    पक्ष 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      प्रत्यागम;
    स्वभाव:
      // Unexpected link width.
      प्रतिज्ञा (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
अन्तःस्थ graph_result_t<hb_blob_t*> serialize (ध्रुव graph_t& graph)
{
  hb_vector_t<वर्ण> buffer;
  माप_प्रकार size = graph.total_size_in_bytes ();

  यदि (!size) प्रत्यागम Ok(hb_blob_get_empty ());

  यदि (unlikely (!buffer.alloc (size))) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    प्रत्यागम Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((शून्य *) buffer, size);

  c.start_serialize<शून्य> ();
  ध्रुव स्वतः& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<अचिन्हित> id_map;
  यदि (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    प्रत्यागम Err(ALLOCATION_FAILURE);
  }

  प्रति (पूर्णाङ्क pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    अचिन्हित i = graph.ordering_[pos];
    c.push ();

    स्वतः& v = vertices[i];

    माप_प्रकार size = v.table_size ();

    वर्ण* start = c.allocate_size <वर्ण> (size);
    यदि (unlikely (!start)) {
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "Buffer out of space.");
      प्रत्यागम Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    प्रति (ध्रुव स्वतः& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.निकालो_pack (असत्य);
  }
  c.end_serialize ();

  यदि (unlikely (c.in_error ())) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    यदि (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      प्रत्यागम Err(OVERFLOW_RESOLUTION_FAILED);
    प्रत्यागम Err(ALLOCATION_FAILURE);
  }

  स्वतः* blob = c.copy_blob ();
  यदि (unlikely (!blob)) प्रत्यागम Err(ALLOCATION_FAILURE);
  प्रत्यागम Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH
