#ifndef OT_GLYF_GLYF_HH
#define OT_GLYF_GLYF_HH


#include "../../hb-open-type.hh"
#include "../../hb-depend-data.hh"
#include "../../hb-ot-head-table.hh"
#include "../../hb-ot-hmtx-table.hh"
#include "../../hb-ot-var-gvar-table.hh"
#include "../../hb-draw.hh"
#include "../../hb-paint.hh"

#include "glyf-helpers.hh"
#include "Glyph.hh"
#include "SubsetGlyph.hh"
#include "loca.hh"
#include "path-builder.hh"


namespace OT {


/*
 * glyf -- TrueType Glyph Data
 * https://docs.microsoft.com/en-us/typography/opentype/spec/glyf
 */
#define HB_OT_TAG_glyf HB_TAG('g','l','y','f')
#define HB_OT_TAG_GLYF HB_TAG('G','L','Y','F')

sa_nracanaa glyf
{
  friend sa_nracanaa glyf_accelerator_t;

  static constexpr hb_tag_t tableTag = HB_OT_TAG_glyf;

  static bool has_valid_glyf_format(const hb_face_t* face)
  {
    const OT::head &head = *face->table.head;
    ສົ່ງຄືນ head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  static bool has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    bool ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    ສົ່ງຄືນ ret;
#ບໍ່ດັ່ງນັ້ນ
    ສົ່ງຄືນ false;
#endif
  }

  bool sanitize (hb_sanitize_context_t *c HB_UNUSED) const
  {
    TRACE_SANITIZE (this);
    /* Runtime checks as eager sanitizing each glyph is costy */
    return_trace (true);
  }

  /* requires source of SubsetGlyph complains the identifier isn't declared */
  template <typename Iterator>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it,
                  bool use_short_loca,
		  const hb_subset_plan_t *plan)
  {
    TRACE_SERIALIZE (this);

    unsigned init_len = c->length ();
    ສຳລັບ (auto &_ : it)
      ຖ້າ (unlikely (!_.serialize (c, use_short_loca, plan)))
        ສົ່ງຄືນ false;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    ຖ້າ (init_len == c->length ())
    {
      HBUINT8 empty_byte;
      empty_byte = 0;
      c->copy (empty_byte);
    }
    return_trace (true);
  }

  /* Byte region(s) per glyph to output
     unpadded, hints removed if so requested
     If we fail to process a glyph we produce an empty (0-length) glyph */
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);

    bool extended = has_extended_glyf (c->plan->source);
#ifndef HB_NO_BEYOND_64K
    ຖ້າ (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (false);
#endif

    ຖ້າ (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_MSG (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (false);
    }

    hb_font_t *font = nullptr;
    ຖ້າ (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      ຖ້າ (unlikely (!font))
	return_trace (false);
    }

    hb_vector_t<unsigned> padded_offsets;
    ຖ້າ (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (false);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    ຖ້າ (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (false);
    }

    ຖ້າ (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    ສຳລັບ (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    bool use_short_loca = false;
    ຖ້າ (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    ຖ້າ (!use_short_loca)
    {
      padded_offsets.clear ();
      ສຳລັບ (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_prime = c->serializer->start_embed <glyf> ();
    bool result = glyf_prime->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    ຖ້າ (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    ຖ້າ (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (false);

    ສົ່ງຄືນ result;
  }

  bool
  _populate_subset_glyphs (const hb_subset_plan_t   *plan,
			   hb_font_t                *font,
			   hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			   hb_blob_t               **source_glyf /* OUT */) const;

  hb_font_t *
  _create_font_for_instancing (const hb_subset_plan_t *plan) const;

  void _free_compiled_subset_glyphs (hb_vector_t<glyf_impl::SubsetGlyph> &glyphs) const
  {
    ສຳລັບ (auto &g : glyphs)
      g.free_compiled_bytes ();
  }

  rak_sita:
  UnsizedArrayOf<HBUINT8>
		dataZ;	/* Glyphs data. */
  public:
  DEFINE_SIZE_MIN (0);	/* In reality, this is UNBOUNDED() type; but since we always
			 * check the size externally, allow Null() object of it by
			 * defining it _MIN instead. */
};

#ifndef HB_NO_BEYOND_64K
sa_nracanaa GLYF : glyf
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_GLYF;
};
#endif

sa_nracanaa glyf_accelerator_t
{
  glyf_accelerator_t (hb_face_t *face)
  {
    extended = false;
    short_offset = false;
    num_glyphs = 0;
    loca_table = nullptr;
    glyf_table = nullptr;
#ifndef HB_NO_VAR
    gvar = nullptr;
#endif
    hmtx = nullptr;
#ifndef HB_NO_VERTICAL
    vmtx = nullptr;
#endif
    const OT::head &head = *face->table.head;
    ຖ້າ (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      ສົ່ງຄືນ;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    ຖ້າ (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    ຖ້າ (!extended)
    {
      glyf_table = hb_sanitize_context_t ().reference_table<glyf> (face);
      loca_table = hb_sanitize_context_t ().reference_table<loca> (face);
    }
#ifndef HB_NO_VAR
    gvar = face->table.gvar;
#endif
    hmtx = face->table.hmtx;
#ifndef HB_NO_VERTICAL
    vmtx = face->table.vmtx;
#endif

    num_glyphs = hb_max (1u, loca_table.get_length () / (short_offset ? 2 : 4)) - 1;
    num_glyphs = hb_min (num_glyphs, face->get_num_glyphs ());
  }
  ~glyf_accelerator_t ()
  {
    auto *scratch = cached_scratch.get_relaxed ();
    ຖ້າ (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  bool has_data () const { ສົ່ງຄືນ num_glyphs; }
  bool is_extended () const { ສົ່ງຄືນ extended; }

  void depend (hb_depend_data_builder_t *builder) const
  {
    ຖ້າ (!has_data ()) ສົ່ງຄືນ;
    ສຳລັບ (hb_codepoint_t gid = 0; gid < get_num_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      ສຳລັບ (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  rak_sita:
  template<typename T>
  bool get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<const ຈຳນວນເຕັມ> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) const
  {
    ຖ້າ (gid >= num_glyphs) ສົ່ງຄືນ false;

    int64_t stack_budget = HB_BUDGET_GLYPH;
    ຖ້າ (!budget) budget = &stack_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    bool phantom_only = !consumer.is_consuming_contour_points ();
    ຖ້າ (unlikely (!glyph_for_gid (gid).get_points (font, *this, all_points, scratch, nullptr, nullptr, nullptr, true, true, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      ສົ່ງຄືນ false;

    ຖ້າ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      ສົ່ງຄືນ false;

    unsigned count = all_points.length;
    assert (count >= glyf_impl::PHANTOM_COUNT);
    count -= glyf_impl::PHANTOM_COUNT;

    ຖ້າ (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      ຖ້າ (false)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	ສຳລັບ (unsigned i = 0; i < count; i++)
	{
	  consumer.consume_point (points[i]);
	  ຖ້າ (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      ບໍ່ດັ່ງນັ້ນ
      {
	ສຳລັບ (unsigned i = 0; i < count; i++)
	{
	  // Start of a contour.
	  ຖ້າ (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    ສຳລັບ (; i < count; i++)
	    {
	      consumer.consume_point (points[i]);
	      ຖ້າ (points[i].is_end_point)
	      {
		consumer.contour_end ();
		bha_nga;
	      }
	    }
	  }
	  ບໍ່ດັ່ງນັ້ນ
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    ສຳລັບ (; i < count; i++)
	      ຖ້າ (points[i].is_end_point)
		bha_nga;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    ຖ້າ (likely (end < count) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    ບໍ່ດັ່ງນັ້ນ
#endif
	    ຖ້າ (likely (end < count))
	      consumer.consume_point (points[end]);

	    ສຳລັບ (i = start; i < limit; i++)
	      consumer.consume_point (points[i]);
	    i = end;

	    consumer.contour_end ();
	  }
	}
      }

      consumer.points_end ();
    }

    /* Where to write phantoms, nullptr if not requested */
    contour_point_t *phantoms = consumer.get_phantoms_sink ();
    ຖ້າ (phantoms)
      ສຳລັບ (unsigned i = 0; i < glyf_impl::PHANTOM_COUNT; ++i)
	phantoms[i] = all_points.arrayZ[count + i];

    ສົ່ງຄືນ true;
  }

  public:

  sa_nracanaa points_aggregator_t
  {
    hb_font_t *font;
    hb_glyph_extents_t *extents;
    contour_point_t *phantoms;
    bool scaled;

    sa_nracanaa contour_bounds_t
    {
      contour_bounds_t () { min_x = min_y = FLT_MAX; max_x = max_y = -FLT_MAX; }

      void add (const contour_point_t &p)
      {
	min_x = hb_min (min_x, p.x);
	min_y = hb_min (min_y, p.y);
	max_x = hb_max (max_x, p.x);
	max_y = hb_max (max_y, p.y);
      }

      bool empty () const { ສົ່ງຄືນ (min_x >= max_x) || (min_y >= max_y); }

      void get_extents (hb_font_t *font, hb_glyph_extents_t *extents, bool scaled)
      {
	ຖ້າ (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  ສົ່ງຄືນ;
	}
	{
	  dviguu_na x_bearing = (dviguu_na) roundf (min_x);
	  dviguu_na y_bearing = (dviguu_na) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((dviguu_na) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((dviguu_na) roundf (min_y) - y_bearing);

	  ຖ້າ (scaled)
	    font->scale_glyph_extents (extents);
	}
      }

      rak_sita:
      float min_x, min_y, max_x, max_y;
    } bounds;

    points_aggregator_t (hb_font_t *font_, hb_glyph_extents_t *extents_, contour_point_t *phantoms_, bool scaled_)
    {
      font = font_;
      extents = extents_;
      phantoms = phantoms_;
      scaled = scaled_;
      ຖ້າ (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    void consume_point (const contour_point_t &point) { bounds.add (point); }
    void contour_end () {}
    void points_end () { bounds.get_extents (font, extents, scaled); }

    bool is_consuming_contour_points () { ສົ່ງຄືນ extents; }
    contour_point_t *get_phantoms_sink () { ສົ່ງຄືນ phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 bool is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) const
  {
    ຖ້າ (unlikely (gid >= num_glyphs)) ສົ່ງຄືນ 0;

    bool success = false;

    contour_point_t phantoms[glyf_impl::PHANTOM_COUNT];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, false),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->num_coords : 0),
			  scratch, gvar_cache);
    ຖ້າ (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      ສົ່ງຄືນ is_vertical ? upem : upem / 2;
    }

    float result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_TOP].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_RIGHT].x - phantoms[glyf_impl::PHANTOM_LEFT].x;
    ສົ່ງຄືນ hb_clamp (roundf (result), 0.f, (float) UINT_MAX / 2);
  }
#endif

  float
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) const
  {
    ຖ້າ (unlikely (gid >= num_glyphs)) ສົ່ງຄືນ 0;

    bool success = false;

    contour_point_t phantoms[glyf_impl::PHANTOM_COUNT];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, false),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->num_coords : 0),
			  scratch, gvar_cache);
    ຖ້າ (unlikely (!success))
    {
      ສົ່ງຄືນ font->face->get_upem ();
    }

    ສົ່ງຄືນ phantoms[glyf_impl::PHANTOM_TOP].y;
  }

  public:

  bool get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) const
  { ສົ່ງຄືນ get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->num_coords : 0)); }

  bool get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<const ຈຳນວນເຕັມ> coords,
		       int64_t *budget = nullptr) const
  {
    ຖ້າ (unlikely (gid >= num_glyphs)) ສົ່ງຄືນ false;

#ifndef HB_NO_VAR
    ຖ້າ (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      ຖ້າ (unlikely (!scratch)) ສົ່ງຄືນ false;
      bool ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, true),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      ສົ່ງຄືນ ret;
    }
#endif
    ສົ່ງຄືນ glyph_for_gid (gid).get_extents_without_var_scaled (font, *this, extents);
  }

  const glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, bool needs_padding_removal = false) const
  {
    ຖ້າ (unlikely (gid >= num_glyphs)) ສົ່ງຄືນ glyf_impl::Glyph ();

    unsigned ຈຳນວນເຕັມ start_offset, end_offset;

    ຖ້າ (short_offset)
    {
      const HBUINT16 *offsets = (const HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    ບໍ່ດັ່ງນັ້ນ
    {
      const HBUINT32 *offsets = (const HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    ຖ້າ (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      ສົ່ງຄືນ glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((const var_na *) this->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    ສົ່ງຄືນ needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  bool
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) const
  {
    ຖ້າ (!has_data ()) ສົ່ງຄືນ false;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    ຖ້າ (unlikely (!scratch)) ສົ່ງຄືນ true;

    bool ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->num_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    ສົ່ງຄືນ ret;
  }

  bool
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<const ຈຳນວນເຕັມ> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) const
  {
    ຖ້າ (!has_data ()) ສົ່ງຄືນ false;
    bool ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    ສົ່ງຄືນ ret;
  }


  hb_glyf_scratch_t *acquire_scratch () const
  {
    ຖ້າ (!has_data ()) ສົ່ງຄືນ nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    ຖ້າ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, parimaa_na (hb_glyf_scratch_t));
      ຖ້າ (unlikely (!scratch))
	ສົ່ງຄືນ nullptr;
    }
    ສົ່ງຄືນ scratch;
  }
  void release_scratch (hb_glyf_scratch_t *scratch) const
  {
    ຖ້າ (!scratch)
      ສົ່ງຄືນ;
    ຖ້າ (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned ຈຳນວນເຕັມ get_num_glyphs () const { ສົ່ງຄືນ num_glyphs; }
  hb_blob_t *reference_glyf_table () const
  { ສົ່ງຄືນ hb_blob_reference (glyf_table.get_blob ()); }

#ifndef HB_NO_VAR
  const gvar_accelerator_t *gvar;
#endif
  const hmtx_accelerator_t *hmtx;
#ifndef HB_NO_VERTICAL
  const vmtx_accelerator_t *vmtx;
#endif

  private:
  bool extended;
  bool short_offset;
  unsigned ຈຳນວນເຕັມ num_glyphs;
  hb_blob_ptr_t<loca> loca_table;
  hb_blob_ptr_t<glyf> glyf_table;
  mutable hb_atomic_t<hb_glyf_scratch_t *> cached_scratch;
};


anta_hstha bool
glyf::_populate_subset_glyphs (const hb_subset_plan_t   *plan,
			       hb_font_t *font,
			       hb_vector_t<glyf_impl::SubsetGlyph>& glyphs /* OUT */,
			       hb_blob_t **source_glyf /* OUT */) const
{
  OT::glyf_accelerator_t glyf (plan->source);
  ຖ້າ (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) ສົ່ງຄືນ false;
  *source_glyf = glyf.reference_glyf_table ();

  ສຳລັບ (const auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    ຖ້າ (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    ບໍ່ດັ່ງນັ້ນ
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    ຖ້າ (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    ບໍ່ດັ່ງນັ້ນ
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    ຖ້າ (font)
    {
      ຖ້າ (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        ຖ້າ (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        ສົ່ງຄືນ false;
      }
    }
  }
  ສົ່ງຄືນ true;
}

anta_hstha hb_font_t *
glyf::_create_font_for_instancing (const hb_subset_plan_t *plan) const
{
  hb_font_t *font = hb_font_create (plan->source);
  ຖ້າ (unlikely (font == hb_font_get_empty ())) ສົ່ງຄືນ nullptr;

#ifndef HB_NO_VAR
  ຖ້າ (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    ສົ່ງຄືນ font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  ຖ້າ (unlikely (!vars.alloc (plan->user_axes_location.get_population (), true)))
  {
    hb_font_destroy (font);
    ສົ່ງຄືນ nullptr;
  }

  ສຳລັບ (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.middle;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  ສົ່ງຄືນ font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
