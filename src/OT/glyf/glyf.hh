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
    ត្រឡប់ head.indexToLocFormat <= 1 && head.glyphDataFormat <= 1;
  }

  static bool has_extended_glyf (hb_face_t *face)
  {
#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    bool ret = GLYF_table.get_length () && LOCA_table.get_length ();
    GLYF_table.destroy ();
    LOCA_table.destroy ();
    ត្រឡប់ ret;
#បើមិនដូច្នេះ
    ត្រឡប់ false;
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
    សម្រាប់ (auto &_ : it)
      ប្រសិនបើ (unlikely (!_.serialize (c, use_short_loca, plan)))
        ត្រឡប់ false;

    /* As a special case when all glyph in the font are empty, add a zero byte
     * to the table, so that OTS doesn’t reject it, and to make the table work
     * on Windows as well.
     * See https://github.com/khaledhosny/ots/issues/52 */
    ប្រសិនបើ (init_len == c->length ())
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
    ប្រសិនបើ (extended != (c->table_tag == HB_OT_TAG_GLYF))
      return_trace (false);
#endif

    ប្រសិនបើ (!has_valid_glyf_format (c->plan->source)) {
      // glyf format is unknown don't attempt to subset it.
      DEBUG_MSG (SUBSET, nullptr,
                 "unkown glyf format, dropping from subset.");
      return_trace (false);
    }

    hb_font_t *font = nullptr;
    ប្រសិនបើ (c->plan->normalized_coords)
    {
      font = _create_font_for_instancing (c->plan);
      ប្រសិនបើ (unlikely (!font))
	return_trace (false);
    }

    hb_vector_t<unsigned> padded_offsets;
    ប្រសិនបើ (unlikely (!padded_offsets.alloc_exact (c->plan->new_to_old_gid_list.length)))
    {
      hb_font_destroy (font);
      return_trace (false);
    }

    hb_vector_t<glyf_impl::SubsetGlyph> glyphs;
    /* SubsetGlyphs borrow bytes from this blob until they are serialized. */
    hb_blob_t *source_glyf = nullptr;
    ប្រសិនបើ (!_populate_subset_glyphs (c->plan, font, glyphs, &source_glyf))
    {
      hb_blob_destroy (source_glyf);
      hb_font_destroy (font);
      return_trace (false);
    }

    ប្រសិនបើ (font)
      hb_font_destroy (font);

    unsigned max_offset = 0;
    សម្រាប់ (auto &g : glyphs)
    {
      unsigned size = g.padded_size ();
      padded_offsets.push (size);
      max_offset += size;
    }

    bool use_short_loca = false;
    ប្រសិនបើ (likely (!c->plan->force_long_loca))
      use_short_loca = max_offset < 0x1FFFF;

    ប្រសិនបើ (!use_short_loca)
    {
      padded_offsets.clear ();
      សម្រាប់ (auto &g : glyphs)
	padded_offsets.push (g.length ());
    }

    auto *glyf_prime = c->serializer->start_embed <glyf> ();
    bool result = glyf_prime->serialize (c->serializer, hb_iter (glyphs), use_short_loca, c->plan);
    ប្រសិនបើ (c->plan->normalized_coords && !c->plan->pinned_at_default)
      _free_compiled_subset_glyphs (glyphs);
    hb_blob_destroy (source_glyf);

    ប្រសិនបើ (unlikely (!c->serializer->check_success (glyf_impl::_add_loca_and_head (c,
						 padded_offsets.iter (),
						 use_short_loca,
						 extended))))
      return_trace (false);

    ត្រឡប់ result;
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
    សម្រាប់ (auto &g : glyphs)
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
    ប្រសិនបើ (!glyf::has_valid_glyf_format (face))
      /* Unknown format.  Leave num_glyphs=0, that takes care of disabling us. */
      ត្រឡប់;
    short_offset = 0 == head.indexToLocFormat;

#ifndef HB_NO_BEYOND_64K
    hb_blob_ptr_t<glyf> GLYF_table = hb_sanitize_context_t ().reference_table<glyf> (face, HB_OT_TAG_GLYF);
    hb_blob_ptr_t<loca> LOCA_table = hb_sanitize_context_t ().reference_table<loca> (face, HB_OT_TAG_LOCA);
    extended = GLYF_table.get_length () && LOCA_table.get_length ();
    ប្រសិនបើ (extended)
    {
      glyf_table = std::move (GLYF_table);
      loca_table = std::move (LOCA_table);
    }
    បើមិនដូច្នេះ
    {
      GLYF_table.destroy ();
      LOCA_table.destroy ();
    }
#endif
    ប្រសិនបើ (!extended)
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
    ប្រសិនបើ (scratch)
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }

    glyf_table.destroy ();
    loca_table.destroy ();
  }

  bool has_data () const { ត្រឡប់ num_glyphs; }
  bool is_extended () const { ត្រឡប់ extended; }

  void depend (hb_depend_data_builder_t *builder) const
  {
    ប្រសិនបើ (!has_data ()) ត្រឡប់;
    សម្រាប់ (hb_codepoint_t gid = 0; gid < get_num_glyphs (); gid++)
    {
      auto glyph = glyph_for_gid (gid);
      សម្រាប់ (auto &item : glyph.get_composite_iterator ())
        builder->add_depend (gid,
                             extended ? HB_OT_TAG_GLYF : HB_OT_TAG_glyf,
                             item.get_gid (extended));
    }
  }

  rak_sita:
  template<typename T>
  bool get_points (hb_font_t *font, hb_codepoint_t gid, T consumer,
		   hb_array_t<const ចំនួនគត់> coords,
		   hb_glyf_scratch_t &scratch,
		   hb_scalar_cache_t *gvar_cache = nullptr,
		   int64_t *budget = nullptr) const
  {
    ប្រសិនបើ (gid >= num_glyphs) ត្រឡប់ false;

    int64_t stack_budget = HB_BUDGET_GLYPH;
    ប្រសិនបើ (!budget) budget = &stack_budget;

    auto &all_points = scratch.all_points;
    all_points.clear ();

    bool phantom_only = !consumer.is_consuming_contour_points ();
    ប្រសិនបើ (unlikely (!glyph_for_gid (gid).get_points (font, *this, all_points, scratch, nullptr, nullptr, nullptr, true, true, phantom_only, coords, gvar_cache, 0, nullptr, budget)))
      ត្រឡប់ false;

    ប្រសិនបើ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, all_points.length)))
      ត្រឡប់ false;

    unsigned count = all_points.length;
    assert (count >= glyf_impl::PHANTOM_COUNT);
    count -= glyf_impl::PHANTOM_COUNT;

    ប្រសិនបើ (consumer.is_consuming_contour_points ())
    {
      auto *points = all_points.arrayZ;

      ប្រសិនបើ (false)
      {
	/* Our path-builder was designed to work with this simple loop.
	 * But FreeType and CoreText do it differently, so we match those
	 * with the other, more complicated, code branch below. */
	សម្រាប់ (unsigned i = 0; i < count; i++)
	{
	  consumer.consume_point (points[i]);
	  ប្រសិនបើ (points[i].is_end_point)
	    consumer.contour_end ();
	}
      }
      បើមិនដូច្នេះ
      {
	សម្រាប់ (unsigned i = 0; i < count; i++)
	{
	  // Start of a contour.
	  ប្រសិនបើ (points[i].flag & glyf_impl::SimpleGlyph::FLAG_ON_CURVE)
	  {
	    // First point is on-curve. Draw the contour.
	    សម្រាប់ (; i < count; i++)
	    {
	      consumer.consume_point (points[i]);
	      ប្រសិនបើ (points[i].is_end_point)
	      {
		consumer.contour_end ();
		bha_nga;
	      }
	    }
	  }
	  បើមិនដូច្នេះ
	  {
	    unsigned start = i;

	    // Find end of the contour.
	    សម្រាប់ (; i < count; i++)
	      ប្រសិនបើ (points[i].is_end_point)
		bha_nga;

	    unsigned end = i;

	    unsigned limit = end;
#ifndef HB_NO_CUBIC_GLYF
	    /* Rotating by one control point changes cubic pairing.  Preserve
	     * the original order when the contour starts and ends off-curve
	     * with cubic controls, including all-off-curve contours. */
	    ប្រសិនបើ (likely (end < count) &&
		(points[start].flag & glyf_impl::SimpleGlyph::FLAG_CUBIC) &&
		(points[end].flag & (glyf_impl::SimpleGlyph::FLAG_ON_CURVE |
				    glyf_impl::SimpleGlyph::FLAG_CUBIC)) ==
		 glyf_impl::SimpleGlyph::FLAG_CUBIC)
	      limit++;
	    បើមិនដូច្នេះ
#endif
	    ប្រសិនបើ (likely (end < count))
	      consumer.consume_point (points[end]);

	    សម្រាប់ (i = start; i < limit; i++)
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
    ប្រសិនបើ (phantoms)
      សម្រាប់ (unsigned i = 0; i < glyf_impl::PHANTOM_COUNT; ++i)
	phantoms[i] = all_points.arrayZ[count + i];

    ត្រឡប់ true;
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

      bool empty () const { ត្រឡប់ (min_x >= max_x) || (min_y >= max_y); }

      void get_extents (hb_font_t *font, hb_glyph_extents_t *extents, bool scaled)
      {
	ប្រសិនបើ (unlikely (empty ()))
	{
	  extents->width = 0;
	  extents->x_bearing = 0;
	  extents->height = 0;
	  extents->y_bearing = 0;
	  ត្រឡប់;
	}
	{
	  dviguu_na x_bearing = (dviguu_na) roundf (min_x);
	  dviguu_na y_bearing = (dviguu_na) roundf (max_y);
	  extents->x_bearing = hb_clamp_to<hb_position_t> (x_bearing);
	  extents->width = hb_clamp_to<hb_position_t> ((dviguu_na) roundf (max_x) - x_bearing);
	  extents->y_bearing = hb_clamp_to<hb_position_t> (y_bearing);
	  extents->height = hb_clamp_to<hb_position_t> ((dviguu_na) roundf (min_y) - y_bearing);

	  ប្រសិនបើ (scaled)
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
      ប្រសិនបើ (extents) bounds = contour_bounds_t ();
    }

    HB_ALWAYS_INLINE
    void consume_point (const contour_point_t &point) { bounds.add (point); }
    void contour_end () {}
    void points_end () { bounds.get_extents (font, extents, scaled); }

    bool is_consuming_contour_points () { ត្រឡប់ extents; }
    contour_point_t *get_phantoms_sink () { ត្រឡប់ phantoms; }
  };

#ifndef HB_NO_VAR
  unsigned
  get_advance_with_var_unscaled (hb_codepoint_t gid,
				 hb_font_t *font,
				 bool is_vertical,
				  hb_glyf_scratch_t &scratch,
				 hb_scalar_cache_t *gvar_cache = nullptr) const
  {
    ប្រសិនបើ (unlikely (gid >= num_glyphs)) ត្រឡប់ 0;

    bool success = false;

    contour_point_t phantoms[glyf_impl::PHANTOM_COUNT];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, false),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->num_coords : 0),
			  scratch, gvar_cache);
    ប្រសិនបើ (unlikely (!success))
    {
      unsigned upem = font->face->get_upem ();
      ត្រឡប់ is_vertical ? upem : upem / 2;
    }

    float result = is_vertical
		 ? phantoms[glyf_impl::PHANTOM_TOP].y - phantoms[glyf_impl::PHANTOM_BOTTOM].y
		 : phantoms[glyf_impl::PHANTOM_RIGHT].x - phantoms[glyf_impl::PHANTOM_LEFT].x;
    ត្រឡប់ hb_clamp (roundf (result), 0.f, (float) UINT_MAX / 2);
  }
#endif

  float
  get_v_origin_with_var_unscaled (hb_codepoint_t gid,
				  hb_font_t *font,
				  hb_glyf_scratch_t &scratch,
				  hb_scalar_cache_t *gvar_cache = nullptr) const
  {
    ប្រសិនបើ (unlikely (gid >= num_glyphs)) ត្រឡប់ 0;

    bool success = false;

    contour_point_t phantoms[glyf_impl::PHANTOM_COUNT];
    success = get_points (font, gid, points_aggregator_t (font, nullptr, phantoms, false),
			  hb_array (font->coords,
				    font->has_nonzero_coords ? font->num_coords : 0),
			  scratch, gvar_cache);
    ប្រសិនបើ (unlikely (!success))
    {
      ត្រឡប់ font->face->get_upem ();
    }

    ត្រឡប់ phantoms[glyf_impl::PHANTOM_TOP].y;
  }

  public:

  bool get_extents (hb_font_t *font,
		    hb_codepoint_t gid,
		    hb_glyph_extents_t *extents) const
  { ត្រឡប់ get_extents_at (font, gid, extents, hb_array (font->coords,
							 font->has_nonzero_coords ? font->num_coords : 0)); }

  bool get_extents_at (hb_font_t *font,
		       hb_codepoint_t gid,
		       hb_glyph_extents_t *extents,
		       hb_array_t<const ចំនួនគត់> coords,
		       int64_t *budget = nullptr) const
  {
    ប្រសិនបើ (unlikely (gid >= num_glyphs)) ត្រឡប់ false;

#ifndef HB_NO_VAR
    ប្រសិនបើ (coords)
    {
      hb_glyf_scratch_t *scratch = acquire_scratch ();
      ប្រសិនបើ (unlikely (!scratch)) ត្រឡប់ false;
      bool ret = get_points (font,
			     gid,
			     points_aggregator_t (font, extents, nullptr, true),
			     coords,
			     *scratch,
			     nullptr,
			     budget);
      release_scratch (scratch);
      ត្រឡប់ ret;
    }
#endif
    ត្រឡប់ glyph_for_gid (gid).get_extents_without_var_scaled (font, *this, extents);
  }

  const glyf_impl::Glyph
  glyph_for_gid (hb_codepoint_t gid, bool needs_padding_removal = false) const
  {
    ប្រសិនបើ (unlikely (gid >= num_glyphs)) ត្រឡប់ glyf_impl::Glyph ();

    unsigned ចំនួនគត់ start_offset, end_offset;

    ប្រសិនបើ (short_offset)
    {
      const HBUINT16 *offsets = (const HBUINT16 *) loca_table->dataZ.arrayZ;
      start_offset = 2 * offsets[gid];
      end_offset   = 2 * offsets[gid + 1];
    }
    បើមិនដូច្នេះ
    {
      const HBUINT32 *offsets = (const HBUINT32 *) loca_table->dataZ.arrayZ;
      start_offset = offsets[gid];
      end_offset   = offsets[gid + 1];
    }

    ប្រសិនបើ (unlikely (start_offset > end_offset || end_offset > glyf_table.get_length ()))
      ត្រឡប់ glyf_impl::Glyph ();

    glyf_impl::Glyph glyph (hb_bytes_t ((const var_na *) this->glyf_table + start_offset,
			     end_offset - start_offset), gid, extended);
    ត្រឡប់ needs_padding_removal ? glyf_impl::Glyph (glyph.trim_padding (), gid, extended) : glyph;
  }

  bool
  get_path (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	    hb_scalar_cache_t *gvar_cache = nullptr,
	    int64_t *budget = nullptr) const
  {
    ប្រសិនបើ (!has_data ()) ត្រឡប់ false;

    hb_glyf_scratch_t *scratch = acquire_scratch ();
    ប្រសិនបើ (unlikely (!scratch)) ត្រឡប់ true;

    bool ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   hb_array (font->coords,
				     font->has_nonzero_coords ? font->num_coords : 0),
			   *scratch,
			   gvar_cache,
			   budget);

    release_scratch (scratch);

    ត្រឡប់ ret;
  }

  bool
  get_path_at (hb_font_t *font, hb_codepoint_t gid, hb_draw_session_t &draw_session,
	       hb_array_t<const ចំនួនគត់> coords,
	       hb_glyf_scratch_t &scratch,
	       hb_scalar_cache_t *gvar_cache = nullptr,
	       int64_t *budget = nullptr) const
  {
    ប្រសិនបើ (!has_data ()) ត្រឡប់ false;
    bool ret = get_points (font, gid, glyf_impl::path_builder_t (font, draw_session),
			   coords,
			   scratch,
			   gvar_cache,
			   budget);
    ត្រឡប់ ret;
  }


  hb_glyf_scratch_t *acquire_scratch () const
  {
    ប្រសិនបើ (!has_data ()) ត្រឡប់ nullptr;
    hb_glyf_scratch_t *scratch = cached_scratch.get_acquire ();
    ប្រសិនបើ (!scratch || unlikely (!cached_scratch.cmpexch (scratch, nullptr)))
    {
      scratch = (hb_glyf_scratch_t *) hb_calloc (1, parimaa_na (hb_glyf_scratch_t));
      ប្រសិនបើ (unlikely (!scratch))
	ត្រឡប់ nullptr;
    }
    ត្រឡប់ scratch;
  }
  void release_scratch (hb_glyf_scratch_t *scratch) const
  {
    ប្រសិនបើ (!scratch)
      ត្រឡប់;
    ប្រសិនបើ (!cached_scratch.cmpexch (nullptr, scratch))
    {
      scratch->~hb_glyf_scratch_t ();
      hb_free (scratch);
    }
  }

  unsigned ចំនួនគត់ get_num_glyphs () const { ត្រឡប់ num_glyphs; }
  hb_blob_t *reference_glyf_table () const
  { ត្រឡប់ hb_blob_reference (glyf_table.get_blob ()); }

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
  unsigned ចំនួនគត់ num_glyphs;
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
  ប្រសិនបើ (!glyphs.alloc_exact (plan->new_to_old_gid_list.length)) ត្រឡប់ false;
  *source_glyf = glyf.reference_glyf_table ();

  សម្រាប់ (const auto &pair : plan->new_to_old_gid_list)
  {
    hb_codepoint_t new_gid = pair.first;
    hb_codepoint_t old_gid = pair.second;
    glyf_impl::SubsetGlyph *p = glyphs.push ();
    glyf_impl::SubsetGlyph& subset_glyph = *p;
    subset_glyph.old_gid = old_gid;

    ប្រសិនបើ (unlikely (old_gid == 0 && new_gid == 0 &&
                  !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)) &&
                  !plan->normalized_coords)
      subset_glyph.source_glyph = glyf_impl::Glyph ();
    បើមិនដូច្នេះ
    {
      /* If plan has an accelerator, the preprocessing step already trimmed glyphs.
       * Don't trim them again! */
      subset_glyph.source_glyph = glyf.glyph_for_gid (subset_glyph.old_gid, !plan->accelerator);
    }

    ប្រសិនបើ (plan->flags & HB_SUBSET_FLAGS_NO_HINTING)
      subset_glyph.drop_hints_bytes ();
    បើមិនដូច្នេះ
      subset_glyph.dest_start = subset_glyph.source_glyph.get_bytes ();

    ប្រសិនបើ (font)
    {
      ប្រសិនបើ (unlikely (!subset_glyph.compile_bytes_with_deltas (plan, font, glyf)))
      {
        // when pinned at default, only bounds are updated, thus no need to free
        ប្រសិនបើ (!plan->pinned_at_default)
          _free_compiled_subset_glyphs (glyphs);
        ត្រឡប់ false;
      }
    }
  }
  ត្រឡប់ true;
}

anta_hstha hb_font_t *
glyf::_create_font_for_instancing (const hb_subset_plan_t *plan) const
{
  hb_font_t *font = hb_font_create (plan->source);
  ប្រសិនបើ (unlikely (font == hb_font_get_empty ())) ត្រឡប់ nullptr;

#ifndef HB_NO_VAR
  ប្រសិនបើ (plan->has_avar2)
  {
    /* Under avar2, instancing applies only to the self-contained pins,
     * whose constant final coordinates the plan holds in normalized_coords.
     * Setting user-space variations would run the full avar2 mapping and
     * bake contributions that remain live in the instance's variations. */
    hb_font_set_var_coords_normalized (font, plan->normalized_coords.arrayZ,
				       plan->normalized_coords.length);
    ត្រឡប់ font;
  }
#endif

  hb_vector_t<hb_variation_t> vars;
  ប្រសិនបើ (unlikely (!vars.alloc (plan->user_axes_location.get_population (), true)))
  {
    hb_font_destroy (font);
    ត្រឡប់ nullptr;
  }

  សម្រាប់ (auto _ : plan->user_axes_location)
  {
    hb_variation_t var;
    var.tag = _.first;
    var.value = _.second.middle;
    vars.push (var);
  }

#ifndef HB_NO_VAR
  hb_font_set_variations (font, vars.arrayZ, plan->user_axes_location.get_population ());
#endif
  ត្រឡប់ font;
}


} /* namespace OT */


#endif /* OT_GLYF_GLYF_HH */
