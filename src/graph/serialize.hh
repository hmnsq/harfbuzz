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

  तार्किक चालक != (अचर overflow_record_t o) अचर
  { फर्काऊ !(*एतत् == o); }

  अन्तःस्थ तार्किक चालक == (अचर overflow_record_t& o) अचर
  {
    फर्काऊ parent == o.parent &&
        child == o.child;
  }

  अन्तःस्थ uint32_t hash () अचर
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    फर्काऊ current;
  }
};

अन्तःस्थ
int64_t compute_offset (
    अचर graph_t& graph,
    अचिन्हित parent_idx,
    अचर hb_serialize_context_t::object_t::link_t& link)
{
  अचर स्वतः& parent = graph.vertices_[parent_idx];
  अचर स्वतः& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  विकल्प ((hb_serialize_context_t::whence_t) link.whence) {
    पक्ष hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; रोक;
    पक्ष hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; रोक;
    पक्ष hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; रोक;
  }

  प्रतिज्ञा (offset >= link.bias);
  offset -= link.bias;
  फर्काऊ offset;
}

अन्तःस्थ
तार्किक is_valid_offset (int64_t offset,
                      अचर hb_serialize_context_t::object_t::link_t& link)
{
  यदि (unlikely (!link.width))
    // Virtual links can't overflow.
    फर्काऊ link.is_signed || offset >= 0;

  यदि (link.is_signed)
  {
    यदि (link.width == 4)
      फर्काऊ offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    अन्यथा
      फर्काऊ offset >= -(1 << 15) && offset < (1 << 15);
  }
  अन्यथा
  {
    यदि (link.width == 4)
      फर्काऊ offset >= 0 && offset < ((int64_t) 1 << 32);
    अन्यथा यदि (link.width == 3)
      फर्काऊ offset >= 0 && offset < ((int32_t) 1 << 24);
    अन्यथा
      फर्काऊ offset >= 0 && offset < (1 << 16);
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
  अचर स्वतः& vertices = graph.vertices_;
  लागि (अचिन्हित parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    लागि (अचर स्वतः& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      यदि (likely (is_valid_offset (offset, link)))
        जारी;

      यदि (!overflows) फर्काऊ Ok(सत्य);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      यदि (record_set.has(r)) जारी; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, सत्य);
    }
  }

  TRY (graph_result_t<शून्य>::from (record_set, ALLOCATION_FAILURE));
  यदि (overflows)
    TRY (graph_result_t<शून्य>::from (*overflows, ALLOCATION_FAILURE));

  फर्काऊ Ok((तार्किक) (overflows && overflows->length));
}

अन्तःस्थ
शून्य print_overflows (graph_t& graph,
                      अचर hb_vector_t<overflow_record_t>& overflows)
{
  यदि (!DEBUG_ENABLED(SUBSET_REPACK)) फर्काऊ;

  स्वतः r = graph.update_parents ();
  यदि (r.is_err()) {
    DEBUG_सन्देश (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    फर्काऊ;
  }

  पूर्णाङ्क limit = 10;
  लागि (अचर स्वतः& o : overflows)
  {
    यदि (!limit--) रोक;
    अचर स्वतः& parent = graph.vertices_[o.parent];
    अचर स्वतः& child = graph.vertices_[o.child];
    DEBUG_सन्देश (SUBSET_REPACK, nullptr,
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
    DEBUG_सन्देश (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

ढाँचा <typename O> अन्तःस्थ शून्य
serialize_link_of_type (अचर hb_serialize_context_t::object_t::link_t& link,
                        वर्ण* head,
                        अचिन्हित size,
                        अचर hb_vector_t<अचिन्हित>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  यदि (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    फर्काऊ;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

अन्तःस्थ
शून्य serialize_link (अचर hb_serialize_context_t::object_t::link_t& link,
                     वर्ण* head,
                     अचिन्हित size,
                     अचर hb_vector_t<अचिन्हित>& id_map,
                     hb_serialize_context_t* c)
{
  विकल्प (link.width)
  {
    पक्ष 0:
      // Virtual links aren't serialized.
      फर्काऊ;
    पक्ष 4:
      यदि (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } अन्यथा {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      फर्काऊ;
    पक्ष 2:
      यदि (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } अन्यथा {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      फर्काऊ;
    पक्ष 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      फर्काऊ;
    स्वभाव:
      // Unexpected link width.
      प्रतिज्ञा (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
अन्तःस्थ graph_result_t<hb_blob_t*> serialize (अचर graph_t& graph)
{
  hb_vector_t<वर्ण> buffer;
  माप_प्रकार size = graph.total_size_in_bytes ();

  यदि (!size) फर्काऊ Ok(hb_blob_get_empty ());

  यदि (unlikely (!buffer.alloc (size))) {
    DEBUG_सन्देश (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    फर्काऊ Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((शून्य *) buffer, size);

  c.start_serialize<शून्य> ();
  अचर स्वतः& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<अचिन्हित> id_map;
  यदि (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_सन्देश (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    फर्काऊ Err(ALLOCATION_FAILURE);
  }

  लागि (पूर्णाङ्क pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    अचिन्हित i = graph.ordering_[pos];
    c.push ();

    स्वतः& v = vertices[i];

    माप_प्रकार size = v.table_size ();

    वर्ण* start = c.allocate_size <वर्ण> (size);
    यदि (unlikely (!start)) {
      DEBUG_सन्देश (SUBSET_REPACK, nullptr, "Buffer out of space.");
      फर्काऊ Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    लागि (अचर स्वतः& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.झिक_pack (असत्य);
  }
  c.end_serialize ();

  यदि (unlikely (c.in_error ())) {
    DEBUG_सन्देश (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    यदि (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      फर्काऊ Err(OVERFLOW_RESOLUTION_FAILED);
    फर्काऊ Err(ALLOCATION_FAILURE);
  }

  स्वतः* blob = c.copy_blob ();
  यदि (unlikely (!blob)) फर्काऊ Err(ALLOCATION_FAILURE);
  फर्काऊ Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH
