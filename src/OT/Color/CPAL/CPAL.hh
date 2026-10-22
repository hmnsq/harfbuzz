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


struct CPALV1Tail
{
  friend struct CPAL;

  private:
  hb_ot_color_palette_flags_t get_palette_flags (const void *base,
						 unsigned 整数 palette_index,
						 unsigned 整数 palette_count) const
  {
    如果 (!paletteFlagsZ) 返回 HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    返回 (hb_ot_color_palette_flags_t) (uint32_t)
	   (base+paletteFlagsZ).as_array (palette_count)[palette_index];
  }

  hb_ot_name_id_t get_palette_name_id (const void *base,
				       unsigned 整数 palette_index,
				       unsigned 整数 palette_count) const
  {
    如果 (!paletteLabelsZ) 返回 HB_OT_NAME_ID_INVALID;
    返回 (base+paletteLabelsZ).as_array (palette_count)[palette_index];
  }

  hb_ot_name_id_t get_color_name_id (const void *base,
				     unsigned 整数 color_index,
				     unsigned 整数 color_count) const
  {
    如果 (!colorLabelsZ) 返回 HB_OT_NAME_ID_INVALID;
    返回 (base+colorLabelsZ).as_array (color_count)[color_index];
  }

  public:
  void collect_name_ids (const void *base,
                         unsigned palette_count,
                         unsigned color_count,
                         const hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) const
  {
    如果 (paletteLabelsZ)
    {
      + (base+paletteLabelsZ).as_array (palette_count)
      | hb_sink (nameids_to_retain)
      ;
    }

    如果 (colorLabelsZ)
    {
      const hb_array_t<const NameID> colorLabels = (base+colorLabelsZ).as_array (color_count);
      为 (unsigned i = 0; i < color_count; i++)
      {
        如果 (!color_index_map->has (i)) continue;
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
    如果 (unlikely (!out)) return_trace (false);

    out->paletteFlagsZ = 0;
    如果 (paletteFlagsZ)
      out->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, base, 0, hb_serialize_context_t::Head, palette_count);

    out->paletteLabelsZ = 0;
    如果 (paletteLabelsZ)
      out->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, base, 0, hb_serialize_context_t::Head, palette_count);

    const hb_array_t<const NameID> colorLabels = (base+colorLabelsZ).as_array (color_count);
    如果 (colorLabelsZ)
    {
      c->push ();
      为 (unsigned i = 0; i < color_count; i++)
      {
        如果 (!color_index_map->has (i)) continue;
        如果 (!c->copy<NameID> (colorLabels[i]))
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
		 unsigned 整数 palette_count,
		 unsigned 整数 color_count) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  (!paletteFlagsZ  || (base+paletteFlagsZ).sanitize (c, palette_count)) &&
		  (!paletteLabelsZ || (base+paletteLabelsZ).sanitize (c, palette_count)) &&
		  (!colorLabelsZ   || (base+colorLabelsZ).sanitize (c, color_count)));
  }

  protected:
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

struct CPAL
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_CPAL;

  bool has_data () const { 返回 numPalettes; }

  size_t get_size () const
  { 返回 min_size + numPalettes * sizeof (colorRecordIndicesZ[0]); }

  unsigned 整数 get_palette_count () const { 返回 numPalettes; }
  unsigned 整数   get_color_count () const { 返回 numColors; }

  hb_ot_color_palette_flags_t get_palette_flags (unsigned 整数 palette_index) const
  { 返回 v1 ().get_palette_flags (this, palette_index, numPalettes); }

  hb_ot_name_id_t get_palette_name_id (unsigned 整数 palette_index) const
  { 返回 v1 ().get_palette_name_id (this, palette_index, numPalettes); }

  hb_ot_name_id_t get_color_name_id (unsigned 整数 color_index) const
  { 返回 v1 ().get_color_name_id (this, color_index, numColors); }

  hb_array_t<const BGRAColor> get_palette_colors (unsigned 整数 palette_index) const
  {
    如果 (unlikely (palette_index >= numPalettes))
      返回 hb_array_t<const BGRAColor> ();
    unsigned 整数 start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<const BGRAColor> all_colors ((this+colorRecordsZ).arrayZ, numColorRecords);
    返回 all_colors.sub_array (start_index, numColors);
  }
  unsigned 整数 get_palette_colors (unsigned 整数  palette_index,
				   unsigned 整数  start_offset,
				   unsigned 整数 *color_count, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) const
  {
    如果 (unlikely (palette_index >= numPalettes))
    {
      如果 (color_count) *color_count = 0;
      返回 0;
    }
    unsigned 整数 start_index = colorRecordIndicesZ[palette_index];
    hb_array_t<const BGRAColor> all_colors ((this+colorRecordsZ).arrayZ, numColorRecords);
    hb_array_t<const BGRAColor> palette_colors = all_colors.sub_array (start_index,
								       numColors);
    如果 (color_count && colors)
    {
      + palette_colors.sub_array (start_offset, color_count)
      | hb_sink (hb_array (colors, *color_count))
      ;
    }
    返回 numColors;
  }

  void collect_name_ids (const hb_map_t *color_index_map,
                         hb_set_t *nameids_to_retain /* OUT */) const
  {
    如果 (version == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (this, numPalettes, numColors, color_index_map, nameids_to_retain);
    }
  }

  private:
  const CPALV1Tail& v1 () const
  {
    如果 (version == 0) 返回 Null (CPALV1Tail);
    hb_barrier ();
    返回 StructAfter<CPALV1Tail> (*this);
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

    为 (const auto idx : color_record_indices)
    {
      hb_codepoint_t layer_index = first_color_to_layer_index[idx];

      HBUINT16 new_idx;
      new_idx = layer_index * retained_color_indices.get_population ();
      如果 (!c->copy<HBUINT16> (new_idx)) return_trace (false);
    }

    c->push ();
    为 (unsigned first_color_index : first_color_index_for_layer)
    {
      为 (hb_codepoint_t color_index : retained_color_indices)
      {
        如果 (!c->copy<BGRAColor> (color_records[first_color_index + color_index]))
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
    如果 (!numPalettes) return_trace (false);

    const hb_map_t *color_index_map = &c->plan->colr_palettes;
    如果 (color_index_map->is_empty ()) return_trace (false);

    hb_set_t retained_color_indices;
    为 (const auto _ : color_index_map->keys ())
    {
      如果 (_ == 0xFFFF) continue;
      retained_color_indices.add (_);
    }
    如果 (retained_color_indices.is_empty ()) return_trace (false);

    auto *out = c->serializer->start_embed (*this);
    如果 (unlikely (!c->serializer->extend_min (out))) return_trace (false);


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
    为 (const auto first_color_record_idx : colorRecordIndices)
    {
      如果 (first_color_to_layer_index.has (first_color_record_idx)) continue;

      hb_vector_t<BGRAColor> retained_colors;
      如果 (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (false);
      为 (hb_codepoint_t color_index : retained_color_indices)
	retained_colors.push (all_color_records[first_color_record_idx + color_index]);

      unsigned layer_index = first_color_index_for_layer.length;
      unsigned *candidate_layer_index = nullptr;
      如果 (retained_colors_to_layer_index.has (retained_colors.as_array (), &candidate_layer_index))
	layer_index = *candidate_layer_index;
      否则
      {
	first_color_index_for_layer.push (first_color_record_idx);
	如果 (unlikely (!c->serializer->propagate_error (first_color_index_for_layer))) return_trace (false);

	如果 (unlikely (!layer_retained_colors.push_or_fail (std::move (retained_colors)))) return_trace (false);
	如果 (unlikely (!retained_colors_to_layer_index.set (layer_retained_colors.tail ().as_array (), layer_index)))
	  return_trace (false);
      }
      first_color_to_layer_index.set (first_color_record_idx, layer_index);
      如果 (unlikely (!c->serializer->propagate_error (first_color_to_layer_index))) return_trace (false);
    }

    out->numColorRecords = first_color_index_for_layer.length
                           * retained_color_indices.get_population ();

    如果 (!out->serialize (c->serializer,
                         colorRecordIndices,
                         all_color_records,
                         first_color_index_for_layer,
                         first_color_to_layer_index,
                         retained_color_indices))
      return_trace (false);

    如果 (version == 1)
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

  protected:
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
