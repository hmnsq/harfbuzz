/*
 * Copyright © 2016  Google, Inc.
 * Copyright © 2018  Ebrahim Byagowi
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
 * Google Author(s): Sascha Brawer
 */

#ifndef OT_COLOR_CPAL_CPAL_HH
#define OT_COLOR_CPAL_CPAL_HH

#include "../../../hb-open-type.hh"
#include "../../../hb-ot-color.h"
#include "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#define HB_OT_TAG_CPAL HB_TAG('C','P','A','L')

namespace OT {


sa_nracanaa CPALV1Tail
{
  friend sa_nracanaa CPAL;

  private:
  hb_ot_color_palette_flags_t get_palette_flags (const void *base,
						 unsigned ចំនួនគត់ palette_index,
						 unsigned ចំនួនគត់ palette_count) const
  {
    ប្រសិនបើ (!paletteFlagsZ) ត្រឡប់ HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    ត្រឡប់ (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_count)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (const void *base,
				       unsigned ចំនួនគត់ palette_index,
				       unsigned ចំនួនគត់ palette_count) const
  {
    ប្រសិនបើ (!paletteLabelsZ) ត្រឡប់ HB_OT_NAME_ID_INVALID;
    ត្រឡប់ (base+paletteLabelsZ).as_array (palette_count)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (const void *base,
				     unsigned ចំនួនគត់ color_index,
				     unsigned ចំនួនគត់ color_count) const
  {
    ប្រសិនបើ (!colorLabelsZ) ត្រឡប់ HB_OT_NAME_ID_INVALID;
    ត្រឡប់ (base+colorLabelsZ).as_array (color_count)[color_index];
  }

  public:
  void collect_name_ids (const void *base,
                         unsigned palette_count,
                         unsigned color_count,
                         const hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) const
  {
    ប្រសិនបើ (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_count)
      | hb_sink (nameids_to_retain)
      ;
    }

    ប្រសិនបើ (colorLabelsZ)
    {
      const hb_array_t<const NameID> colorLabels = (base+colorLabelsZ).as_array (color_count);
      សម្រាប់ (unsigned i = 0; i < color_count; i++)
      {
        ប្រសិនបើ (!color_index_map->has (i)) continue;
        nameids_to_retain->add (colorLabels[i]);
      }
    }
  }

  bool serialize (hb_serialize_context_t *c,
                  unsigned palette_count,
                  unsigned color_count,
                  const void *base,
                  const hb_map_t *color_index_map) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->allocate_size<CPALV1Tail> (static_size);
    ប្រសិនបើ (unlikely (!out)) return_trace (false);

    out->paletteFlagsZ = 0;
    ប្រសិនបើ (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_count);

    out->paletteLabelsZ = 0;
    ប្រសិនបើ (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_count);

    const hb_array_t<const NameID> colorLabels = (base+colorLabelsZ).as_array (color_count);
    ប្រសិនបើ (colorLabelsZ)
    {
      c->push ();
      សម្រាប់ (unsigned i = 0; i < color_count; i++)
      {
        ប្រសិនបើ (!color_index_map->has (i)) continue;
        ប្រសិនបើ (!c->copy<NameID> (colorLabels[i]))
        {
          c->pop_discard ();
          return_trace (false);
        }
      }
      c->add_link (out->colorLabelsZ, c->pop_pack ());
    }
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c,
		 const void *base,
		 unsigned ចំនួនគត់ palette_count,
		 unsigned ចំនួនគត់ color_count) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_count)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_count)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_count)));
  }

  rak_sita:
  // TODO(garretrieger): these offsets can hold nulls so we should not be using non-null offsets
  //                     here. Currently they are needed since UnsizedArrayOf doesn't define null_size
  NNOffset32To<UnsizedArrayOf<HBUINT32>>
		paletteFlagsZ;		/* Offset from the beginning of CPAL table to
					 * the Palette Type Array. Set to 0 if no array
					 * is provided. */
  NNOffset32To<UnsizedArrayOf<NameID>>
		paletteLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the palette labels array. Set to 0 if no
					 * array is provided. */
  NNOffset32To<UnsizedArrayOf<NameID>>
		colorLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the color labels array. Set to 0
					 * if no array is provided. */
  public:
  DEFINE_SIZE_STATIC (12);
};

typedef HBUINT32 BGRAColor;

sa_nracanaa CPAL
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  bool has_data () const { ត្រឡប់ numPalettes; }

  size_t get_size () const
  { ត្រឡប់ min_size + numPalettes * parimaa_na (colorRecordIndicesZ[0]); }

  unsigned ចំនួនគត់ get_palette_count () const { ត្រឡប់ numPalettes; }
  unsigned ចំនួនគត់   get_color_count () const { ត្រឡប់ numColors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned ចំនួនគត់ palette_index) const
  { ត្រឡប់ v1 ().get_palette_flags (this, palette_index, numPalettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned ចំនួនគត់ palette_index) const
  { ត្រឡប់ v1 ().get_palette_name_id (this, palette_index, numPalettes); }

  hb_ot_name_id_t get_color_name_id (unsigned ចំនួនគត់ color_index) const
  { ត្រឡប់ v1 ().get_color_name_id (this, color_index, numColors); }

  hb_array_t<const BGRAColor> get_palette_colors (unsigned ចំនួនគត់ palette_index) const
  {
    ប្រសិនបើ (unlikely (palette_index >= numPalettes))
      ត្រឡប់ hb_array_t<const BGRAColor> ();
    unsigned ចំនួនគត់ start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<const BGRAColor> all_colors ((this+colorRecordsZ).arrayZ, numColorRecords);
    ត្រឡប់ all_colors.sub_array (start_index, numColors);
  }
  unsigned ចំនួនគត់ get_palette_colors (unsigned ចំនួនគត់  palette_index,
				   unsigned ចំនួនគត់  start_offset,
				   unsigned ចំនួនគត់ *color_count, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) const
  {
    ប្រសិនបើ (unlikely (palette_index >= numPalettes))
    {
      ប្រសិនបើ (color_count) *color_count = 0;
      ត្រឡប់ 0;
    }
    unsigned ចំនួនគត់ start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<const BGRAColor> all_colors ((this+colorRecordsZ).arrayZ, numColorRecords);
    hb_array_t<const BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       numColors);
    ប្រសិនបើ (color_count && colors)
    {
      + palette_colors.sub_array (start_offset, color_count)
      | hb_sink (hb_array (colors, *color_count))
      ;
    }
    ត្រឡប់ numColors;
  }

  void collect_name_ids (const hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) const
  {
    ប្រសិនបើ (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (this, numPalettes, numColors, color_index_map, nameids_to_retain);
    }
  }

  private:
  const CPALV1Tail& v1 () const
  {
    ប្រសិនបើ (version == 0) ត្រឡប់ Null (CPALV1Tail);
    hb_barrier ();
    ត្រឡប់ StructAfter<CPALV1Tail> (*this);
  }

  public:
  bool serialize (hb_serialize_context_t *c,
                  const hb_array_t<const HBUINT16> &color_record_indices,
                  const hb_array_t<const BGRAColor> &color_records,
                  const hb_vector_t<unsigned>& first_color_index_for_layer,
                  const hb_map_t& first_color_to_layer_index,
                  const hb_set_t &retained_color_indices) const
  {
    TRACE_SERIALIZE (this);

    // TODO(grieger): limit total final size.

    សម្រាប់ (const auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      ប្រសិនបើ (!c->copy<HBUINT16> (new_idx)) return_trace (false);
    }

    c->push ();
    សម្រាប់ (unsigned first_color_index : first_color_index_for_layer)
    {
      សម្រាប់ (hb_codepoint_t color_index : retained_color_indices)
      {
        ប្រសិនបើ (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
        {
          c->pop_discard ();
          return_trace (false);
        }
      }
    }

    c->add_link (colorRecordsZ, c->pop_pack ());
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    ប្រសិនបើ (!numPalettes) return_trace (false);

    const hb_map_t *color_index_map = &c->plan->colr_palettes;
    ប្រសិនបើ (color_index_map->is_empty ()) return_trace (false);

    hb_set_t retained_color_indices;
    សម្រាប់ (const auto _ : color_index_map->keys ())
    {
      ប្រសិនបើ (_ == 0xFFFF) continue;
      retained_color_indices.add (_);
    }
    ប្រសិនបើ (retained_color_indices.is_empty ()) return_trace (false);

    auto *out = c->serializer->start_embed (*this);
    ប្រសិនបើ (unlikely (!c->serializer->extend_min (out))) return_trace (false);


    out->version = version;
    out->numColors = retained_color_indices.get_population ();
    out->numPalettes = numPalettes;

    hb_vector_t<unsigned> first_color_index_for_layer;
    hb_map_t first_color_to_layer_index;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<const BGRAColor>, unsigned> retained_colors_to_layer_index;
    hb_vector_t<hb_vector_t<BGRAColor>> layer_retained_colors;

    const hb_array_t<const BGRAColor> all_color_records = (this+colorRecordsZ).as_array (numColorRecords);

    const hb_array_t<const HBUINT16> colorRecordIndices = colorRecordIndicesZ.as_array (numPalettes);
    សម្រាប់ (const auto first_color_record_idx : colorRecordIndices)
    {
      ប្រសិនបើ (first_color_to_layer_index.has (first_color_record_idx)) continue;

      hb_vector_t<BGRAColor> retained_colors;
      ប្រសិនបើ (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (false);
      សម្រាប់ (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      ប្រសិនបើ (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      បើមិនដូច្នេះ
      {
	first_color_index_for_layer.push (first_color_record_idx);
	ប្រសិនបើ (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (false);

	ប្រសិនបើ (unlikely (!layer_retained_colors.push_or_fail (std::move (retained_colors)))) return_trace (false);
	ប្រសិនបើ (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (false);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      ប្រសិនបើ (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (false);
    }

    out->numColorRecords = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    ប្រសិនបើ (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (false);

    ប្រសិនបើ (version == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, numPalettes, numColors, this, color_index_map));
    }

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  (this+colorRecordsZ).sanitize (c, numColorRecords) &&
		  colorRecordIndicesZ.sanitize (c, numPalettes) &&
		  (version == 0 || v1 ().sanitize (c, this, numPalettes, numColors)));
  }

  rak_sita:
  HBUINT16	version;		/* Table version number */
  /* Version 0 */
  HBUINT16	numColors;		/* Number of colors in each palette. */
  HBUINT16	numPalettes;		/* Number of palettes in the table. */
  HBUINT16	numColorRecords;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32To<UnsizedArrayOf<BGRAColor>>
		colorRecordsZ;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  UnsizedArrayOf<HBUINT16>
		colorRecordIndicesZ;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  public:
  DEFINE_SIZE_ARRAY (12, colorRecordIndicesZ);
};

} /* namespace OT */


#endif /* OT_COLOR_CPAL_CPAL_HH */
