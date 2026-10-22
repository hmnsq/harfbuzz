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

#અન્તર્ભાવ "graph.hh"
#અન્તર્ભાવ "../hb-ot-layout-common.hh"
#અન્તર્ભાવ "graph-result.hh"

#ifndef GRAPH_CLASSDEF_GRAPH_HH
#define GRAPH_CLASSDEF_GRAPH_HH

નામાકાશ graph {

template <typename Types>
સંરચના ClassDefFormat1_3 : જાહેર OT::ClassDefFormat1_3<Types>
{
  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size();
    constexpr unsigned min_size = OT::ClassDefFormat1_3<Types>::min_size;
    જો (unlikely (vertex_len < min_size)) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();
    જો (unlikely (vertex_len < min_size + આ->class_કિંમત.get_size () - આ->class_કિંમત.len.get_size ()))
      પાછા Err(SANITIZE_FAILURE);
    પાછા Ok();
  }
};

template <typename Types>
સંરચના ClassDefFormat2_4 : જાહેર OT::ClassDefFormat2_4<Types>
{
  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size();
    constexpr unsigned min_size = OT::ClassDefFormat2_4<Types>::min_size;
    જો (unlikely (vertex_len < min_size)) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();
    જો (unlikely (vertex_len < min_size + આ->rangeRecord.get_size () - આ->rangeRecord.len.get_size ()))
      પાછા Err(SANITIZE_FAILURE);
    પાછા Ok();
  }
};

સંરચના ClassDef : જાહેર OT::ClassDef
{
  template<typename It>
  સ્થિર graph_result_t<શૂન્ય> add_class_def (gsubgpos_graph_context_t& c,
                                             unsigned parent_id,
                                             unsigned link_position,
                                             It glyph_and_class,
                                             unsigned max_size)
  {
    TRY_ASSIGN (unsigned class_def_અવિભાજ્ય_id, c.graph.new_ગાંઠ (nullptr, nullptr));
    auto& class_def_અવિભાજ્ય_vertex = c.graph.vertices_[class_def_અવિભાજ્ય_id];
    TRY (make_class_def (c, glyph_and_class, class_def_અવિભાજ્ય_id, max_size));

    TRY(c.graph.vertices_[parent_id].add_real_link (SmallTypes::size, class_def_અવિભાજ્ય_id, link_position));
    TRY(class_def_અવિભાજ્ય_vertex.add_parent (parent_id, ખોટું));

    પાછા Ok();
  }

  template<typename It>
  સ્થિર graph_result_t<શૂન્ય> make_class_def (gsubgpos_graph_context_t& c,
                                              It glyph_and_class,
                                              unsigned dest_obj,
                                              unsigned max_size)
  {
    અક્ષર* buffer = (અક્ષર*) hb_calloc (1, max_size);
    જો (unlikely (!buffer))
      પાછા Err(ALLOCATION_FAILURE);

    hb_serialize_context_t serializer (buffer, max_size);
    OT::ClassDef_serialize (&serializer, glyph_and_class);
    serializer.end_serialize ();
    જો (unlikely (serializer.in_error ()))
    {
      hb_free (buffer);
      પાછા Err(ALLOCATION_FAILURE);
    }

    hb_bytes_t class_def_copy = serializer.copy_bytes ();
    જો (unlikely (!class_def_copy.arrayZ))
    {
      hb_free (buffer);
      પાછા Err(ALLOCATION_FAILURE);
    }

    // Give ownership to the context, it will cleanup the buffer.
    auto res = c.add_buffer ((અક્ષર *) class_def_copy.arrayZ);
    જો (unlikely (!res.is_ok ()))
    {
      hb_free (buffer);
      hb_free ((અક્ષર *) class_def_copy.arrayZ);
      પાછા res;
    }


    auto& v = c.graph.vertices_[dest_obj];
    અક્ષર* head = (અક્ષર *) class_def_copy.arrayZ;
    v.set_buffer (head, head + class_def_copy.length);

    hb_free (buffer);
    પાછા Ok();
  }

  graph_result_t<શૂન્ય> sanitize (અચલ graph_t::vertex_t& vertex) અચલ
  {
    size_t vertex_len = vertex.table_size();
    જો (unlikely (vertex_len < OT::ClassDef::min_size)) પાછા Err(SANITIZE_FAILURE);
    hb_barrier ();
    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: પાછા ((ClassDefFormat1_3<SmallTypes>*)આ)->sanitize (vertex);
    સ્થિતિ 2: પાછા ((ClassDefFormat2_4<SmallTypes>*)આ)->sanitize (vertex);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: પાછા ((ClassDefFormat1_3<MediumTypes>*)આ)->sanitize (vertex);
    સ્થિતિ 4: પાછા ((ClassDefFormat2_4<MediumTypes>*)આ)->sanitize (vertex);
#endif
    મૂળભૂત: પાછા Err(SANITIZE_FAILURE);
    }
  }
};


સંરચના class_def_size_estimator_t
{
  template<typename It>
  સ્થિર graph_result_t<class_def_size_estimator_t> create (It glyph_and_class)
  {
    class_def_size_estimator_t estimator;
    estimator.reset();
    માટે (auto p : + glyph_and_class)
    {
      unsigned gid = p.first;
      unsigned klass = p.second;

      hb_set_t* glyphs;
      જો (estimator.glyphs_per_class.has (klass, &glyphs) && glyphs) {
        glyphs->add (gid);
        ચાલુ;
      }

      hb_set_t new_glyphs;
      new_glyphs.add (gid);
      estimator.glyphs_per_class.set (klass, std::move (new_glyphs));
    }

    TRY(estimator.to_result ());

    માટે (unsigned klass : estimator.glyphs_per_class.ચાવીઓ ())
    {
      જો (!klass) ચાલુ; // class 0 doesn't get encoded.

      અચલ hb_set_t& glyphs = estimator.glyphs_per_class.get (klass);
      hb_codepoint_t start = HB_SET_કિંમત_INVALID;
      hb_codepoint_t end = HB_SET_કિંમત_INVALID;

      unsigned ગણતરી = 0;
      જ્યારે (glyphs.next_range (&start, &end))
        ગણતરી++;

      estimator.અંક_ranges_per_class.set (klass, ગણતરી);
    }

    TRY(estimator.to_result ());
    પાછા estimator;
  }

  શૂન્ય reset() {
    included_glyphs.clear();
    included_class_def_glyphs.clear();
    included_classes.clear();
    class_def_range_ગણતરી = 0;
    class_max = 0;
    coverage_size_val = compute_coverage_size ();
  }

  // Size of coverage for all glyphs added via 'add_class_def_size'.
  unsigned coverage_size () અચલ
  {
    પાછા coverage_size_val;
  }

  // Compute the new size of the ClassDef table if all glyphs associated with 'klass' were added.
  unsigned add_class_def_size (unsigned klass)
  {
    જો (!included_classes.has(klass)) {
      hb_set_t* glyphs = nullptr;
      જો (glyphs_per_class.has(klass, &glyphs)) {
        unsigned અંક_glyphs = included_glyphs.get_population();
        included_glyphs.union_(*glyphs);
        જો (klass)
          included_class_def_glyphs.union_(*glyphs);
        જો (અંક_glyphs != included_glyphs.get_population())
          coverage_size_val = compute_coverage_size ();
      }

      જો (klass)
      {
        class_def_range_ગણતરી += અંક_ranges_per_class.get (klass);
        જો (glyphs && klass > class_max)
          class_max = klass;
      }
      included_classes.add(klass);
    }

    પાછા class_def_size ();
  }

  unsigned અંક_glyph_ranges (અચલ hb_set_t &glyphs) અચલ {
    hb_codepoint_t start = HB_SET_કિંમત_INVALID;
    hb_codepoint_t end = HB_SET_કિંમત_INVALID;

    unsigned ગણતરી = 0;
    જ્યારે (glyphs.next_range (&start, &end)) {
        ગણતરી++;
    }
    પાછા ગણતરી;
  }

 ખાનગી:
  graph_result_t<શૂન્ય> to_result() અચલ
  {
    TRY (graph_result_t<શૂન્ય>::from (અંક_ranges_per_class, ALLOCATION_FAILURE));
    TRY (graph_result_t<શૂન્ય>::from (glyphs_per_class, ALLOCATION_FAILURE));
    TRY (graph_result_t<શૂન્ય>::from (included_classes, ALLOCATION_FAILURE));
    TRY (graph_result_t<શૂન્ય>::from (included_glyphs, ALLOCATION_FAILURE));
    TRY (graph_result_t<શૂન્ય>::from (included_class_def_glyphs, ALLOCATION_FAILURE));

    માટે (અચલ hb_set_t& s : glyphs_per_class.values ())
    {
          TRY (graph_result_t<શૂન્ય>::from (s, ALLOCATION_FAILURE));
    }
    પાછા Ok();
  }

  class_def_size_estimator_t ()
      : અંક_ranges_per_class (), glyphs_per_class () {}

  unsigned compute_coverage_size () અચલ
  {
    unsigned glyph_size = glyph_id_size (included_glyphs);
    unsigned coverage_base_size = OT::HBUINT16::static_size + glyph_size;
    unsigned range_size = 3 * glyph_size;
    unsigned format1_size = coverage_base_size + glyph_size * included_glyphs.get_population();
    unsigned format2_size = coverage_base_size + range_size * અંક_glyph_ranges (included_glyphs);
    પાછા hb_min(format1_size, format2_size);
  }

  unsigned glyph_id_size (અચલ hb_set_t &glyphs) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (!glyphs.is_empty () && glyphs.get_max () > 0xFFFFu)
      પાછા OT::Layout::MediumTypes::size;
#endif
    પાછા OT::Layout::SmallTypes::size;
  }

  unsigned class_def_size () અચલ
  {
    જો (included_class_def_glyphs.is_empty ())
      પાછા OT::HBUINT16::static_size + OT::Layout::SmallTypes::size;

    unsigned best_size = UINT_MAX;
    hb_codepoint_t glyph_max = included_class_def_glyphs.get_max ();
    unsigned glyph_span = glyph_max - included_class_def_glyphs.get_min () + 1;

    જો (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
      best_size = hb_min (best_size, 6 + 2 * glyph_span);
    જો (glyph_max <= 0xFFFFu && class_def_range_ગણતરી <= 0xFFFFu && class_max <= 0xFFFFu)
      best_size = hb_min (best_size, 4 + 6 * class_def_range_ગણતરી);
#ifndef HB_NO_BEYOND_64K
    જો (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
      best_size = hb_min (best_size, 8 + 3 * glyph_span);
    જો (glyph_max <= 0xFFFFFFu && class_def_range_ગણતરી <= 0xFFFFFFu && class_max <= 0xFFFFu)
      best_size = hb_min (best_size, 5 + 8 * class_def_range_ગણતરી);
#endif
    પાછા best_size;
  }

  hb_hashmap_t<unsigned, unsigned> અંક_ranges_per_class;
  hb_hashmap_t<unsigned, hb_set_t> glyphs_per_class;
  hb_set_t included_classes;
  hb_set_t included_glyphs;
  hb_set_t included_class_def_glyphs;
  unsigned class_def_range_ગણતરી;
  unsigned class_max;
  unsigned coverage_size_val;
};


}

#endif  // GRAPH_CLASSDEF_GRAPH_HH
