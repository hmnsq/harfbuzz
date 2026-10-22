#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_GLYPH_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_GLYPH_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-open-type.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "GlyphHeader.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "SimpleGlyph.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "CompositeGlyph.hh"


ਨਾਮਾਕਾਸ਼ OT {

ਬਣਤਰ glyf_accelerator_t;

ਨਾਮਾਕਾਸ਼ glyf_impl {


ਗਿਣਤੀ phantom_ਬਿੰਦੂ_ਸੂਚਕਾਂਕ_t
{
  PHANTOM_ਖੱਬਾ   = 0,
  PHANTOM_ਸੱਜਾ  = 1,
  PHANTOM_ਸਿਖਰ    = 2,
  PHANTOM_BOTTOM = 3,
  PHANTOM_ਗਿਣਤੀ  = 4
};

ਬਣਤਰ Glyph
{
  ਗਿਣਤੀ glyph_type_t {
    ਖਾਲੀ_ਥਾਵਾਂ,
    SIMPLE,
    ਸੰਯੁਕਤ,
  };

  ਜਨਤਕ:
  ਸੰਯੁਕਤ_iter_t get_ਸੰਯੁਕਤ_iterator () ਅਚਲ
  {
    ਜੇ (type != ਸੰਯੁਕਤ) ਵਾਪਸ ਸੰਯੁਕਤ_iter_t ();
    ਵਾਪਸ ਸੰਯੁਕਤ_Glyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ, extended).iter ();
  }

  ਅਚਲ hb_ਬਾਈਟਾਂ_t trim_padding () ਅਚਲ
  {
    ਚੋਣ (type) {
    ਹਾਲਤ ਸੰਯੁਕਤ: ਵਾਪਸ ਸੰਯੁਕਤ_Glyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ, extended).trim_padding ();
    ਹਾਲਤ SIMPLE:    ਵਾਪਸ SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).trim_padding ();
    ਹਾਲਤ ਖਾਲੀ_ਥਾਵਾਂ:     ਵਾਪਸ ਬਾਈਟਾਂ;
    ਮੂਲ:        ਵਾਪਸ ਬਾਈਟਾਂ;
    }
  }

  ਖਾਲੀ drop_hints ()
  {
    ਚੋਣ (type) {
    ਹਾਲਤ ਸੰਯੁਕਤ: ਸੰਯੁਕਤ_Glyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ, extended).drop_hints (); ਵਾਪਸ;
    ਹਾਲਤ SIMPLE:    SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).drop_hints (); ਵਾਪਸ;
    ਹਾਲਤ ਖਾਲੀ_ਥਾਵਾਂ:
      ਜੇ (ਬਾਈਟਾਂ.ਲੰਮਾਈ >= Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ)
        SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).drop_hints ();
      ਵਾਪਸ;
    }
  }

  ਖਾਲੀ ਸੈੱਟ_overlaps_flag ()
  {
    ਚੋਣ (type) {
    ਹਾਲਤ ਸੰਯੁਕਤ: ਸੰਯੁਕਤ_Glyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ, extended).ਸੈੱਟ_overlaps_flag (); ਵਾਪਸ;
    ਹਾਲਤ SIMPLE:    SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).ਸੈੱਟ_overlaps_flag (); ਵਾਪਸ;
    ਹਾਲਤ ਖਾਲੀ_ਥਾਵਾਂ:     ਵਾਪਸ;
    }
  }

  ਖਾਲੀ drop_hints_ਬਾਈਟਾਂ (hb_ਬਾਈਟਾਂ_t &dest_ਸ਼ੁਰੂ, hb_ਬਾਈਟਾਂ_t &dest_ਸਿਰਾ) ਅਚਲ
  {
    ਚੋਣ (type) {
    ਹਾਲਤ ਸੰਯੁਕਤ: ਸੰਯੁਕਤ_Glyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ, extended).drop_hints_ਬਾਈਟਾਂ (dest_ਸ਼ੁਰੂ); ਵਾਪਸ;
    ਹਾਲਤ SIMPLE:    SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).drop_hints_ਬਾਈਟਾਂ (dest_ਸ਼ੁਰੂ, dest_ਸਿਰਾ); ਵਾਪਸ;
    ਹਾਲਤ ਖਾਲੀ_ਥਾਵਾਂ:
      dest_ਸ਼ੁਰੂ = ਬਾਈਟਾਂ;
      dest_ਸਿਰਾ = hb_ਬਾਈਟਾਂ_t ();
      ਜੇ (ਬਾਈਟਾਂ.ਲੰਮਾਈ >= Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ &&
          SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).has_instructions_ਲੰਮਾਈ ())
        SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).drop_hints_ਬਾਈਟਾਂ (dest_ਸ਼ੁਰੂ, dest_ਸਿਰਾ);
      ਵਾਪਸ;
    }
  }

  ਤਾਰਕਿਕ is_ਸੰਯੁਕਤ () ਅਚਲ
  { ਵਾਪਸ type == ਸੰਯੁਕਤ; }

  ਤਾਰਕਿਕ get_all_points_without_var (ਅਚਲ hb_face_t *face,
                                   contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &points /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    ਚੋਣ (type) {
    ਹਾਲਤ SIMPLE:
      ਜੇ (unlikely (!SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).get_contour_points (points, extended)))
        ਵਾਪਸ ਝੂਠ;
      ਰੋਕੋ;
    ਹਾਲਤ ਸੰਯੁਕਤ:
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ &ਵਸਤੂ : get_ਸੰਯੁਕਤ_iterator ())
        ਜੇ (unlikely (!ਵਸਤੂ.get_points (points, extended))) ਵਾਪਸ ਝੂਠ;
      ਰੋਕੋ;
    }
    ਹਾਲਤ ਖਾਲੀ_ਥਾਵਾਂ:
      ਰੋਕੋ;
    }

    /* Init phantom points */
    ਜੇ (unlikely (!points.resize (points.ਲੰਮਾਈ + PHANTOM_ਗਿਣਤੀ))) ਵਾਪਸ ਝੂਠ;
    hb_array_t<contour_ਬਿੰਦੂ_t> phantoms = points.as_array ().sub_array (points.ਲੰਮਾਈ - PHANTOM_ਗਿਣਤੀ, PHANTOM_ਗਿਣਤੀ);
    {
      // Duplicated code.
      ਪੂਰਨ lsb = 0;
      face->ਸਾਰਣੀ.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      ਪੂਰਨ h_delta = (ਪੂਰਨ) ਸਿਰਲੇਖ->xMin - lsb;
      HB_ਅਣਵਰਤਿਆ ਪੂਰਨ tsb = 0;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
      face->ਸਾਰਣੀ.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#ਜੇ_ਅੰਤ
      ਪੂਰਨ v_orig  = (ਪੂਰਨ) ਸਿਰਲੇਖ->yMax + tsb;
      ਬਿਨਾਨਿਸ਼ਾਨੀ h_adv = face->ਸਾਰਣੀ.hmtx->get_advance_without_var_unscaled (gid);
      ਬਿਨਾਨਿਸ਼ਾਨੀ v_adv =
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
                       face->ਸਾਰਣੀ.vmtx->get_advance_without_var_unscaled (gid)
#ਨਹੀਂਤਾਂ
                       - face->get_upem ()
#ਜੇ_ਅੰਤ
                       ;
      phantoms[PHANTOM_ਖੱਬਾ].x = h_delta;
      phantoms[PHANTOM_ਸੱਜਾ].x = (ਪੂਰਨ) h_adv + h_delta;
      phantoms[PHANTOM_ਸਿਖਰ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (ਪੂਰਨ) v_adv;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਖਾਲੀ update_mtx (ਅਚਲ hb_subset_plan_t *plan,
                   ਪੂਰਨ xMin, ਪੂਰਨ xMax,
                   ਪੂਰਨ yMin, ਪੂਰਨ yMax,
                   ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &all_points) ਅਚਲ
  {
    hb_codepoint_t new_gid = 0;
    ਜੇ (!plan->new_gid_for_old_gid (gid, &new_gid))
      ਵਾਪਸ;

    ਜੇ (type != ਖਾਲੀ_ਥਾਵਾਂ)
    {
      plan->bounds_ਚੌੜਾਈ_vec[new_gid] = xMax - xMin;
      plan->bounds_ਉਚਾਈ_vec[new_gid] = yMax - yMin;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ len = all_points.ਲੰਮਾਈ;
    ਦਸ਼ਮਲਵ ਖੱਬਾ_Side_X = all_points[len - 4].x;
    ਦਸ਼ਮਲਵ ਸੱਜਾ_Side_X = all_points[len - 3].x;
    ਦਸ਼ਮਲਵ ਸਿਖਰ_Side_Y = all_points[len - 2].y;
    ਦਸ਼ਮਲਵ bottomSideY = all_points[len - 1].y;

    uint32_t hash = hb_hash (new_gid);

    ਨਿਸ਼ਾਨੀਵਾਲਾ hori_aw = roundf (ਸੱਜਾ_Side_X - ਖੱਬਾ_Side_X);
    ਜੇ (hori_aw < 0) hori_aw = 0;
    ਪੂਰਨ lsb = roundf (xMin - ਖੱਬਾ_Side_X);
    plan->hmtx_map.ਸੈੱਟ_with_hash (new_gid, hash, hb_pair ((ਬਿਨਾਨਿਸ਼ਾਨੀ) hori_aw, lsb));
    //flag value should be computed using non-empty glyphs
    ਜੇ (type != ਖਾਲੀ_ਥਾਵਾਂ && lsb != xMin)
      plan->ਸਿਰ_maxp_info.allXMinIsLsb = ਝੂਠ;

    ਨਿਸ਼ਾਨੀਵਾਲਾ vert_aw = roundf (ਸਿਖਰ_Side_Y - bottomSideY);
    ਜੇ (vert_aw < 0) vert_aw = 0;
    ਪੂਰਨ tsb = roundf (ਸਿਖਰ_Side_Y - yMax);
    plan->vmtx_map.ਸੈੱਟ_with_hash (new_gid, hash, hb_pair ((ਬਿਨਾਨਿਸ਼ਾਨੀ) vert_aw, tsb));
  }

  ਤਾਰਕਿਕ compile_ਸਿਰਲੇਖ_ਬਾਈਟਾਂ (ਅਚਲ hb_subset_plan_t *plan,
                             ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &all_points,
                             hb_ਬਾਈਟਾਂ_t &dest_ਬਾਈਟਾਂ /*ਬਾਹਰਲਾ*/) ਅਚਲ
  {
    Glyph_ਸਿਰਲੇਖ *glyph_ਸਿਰਲੇਖ = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ (!plan->pinned_at_default && type != ਖਾਲੀ_ਥਾਵਾਂ && all_points.ਲੰਮਾਈ >= 4)
    {
      glyph_ਸਿਰਲੇਖ = (Glyph_ਸਿਰਲੇਖ *) hb_calloc (1, Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ);
      ਜੇ (unlikely (!glyph_ਸਿਰਲੇਖ)) ਵਾਪਸ ਝੂਠ;
    }

    ਦਸ਼ਮਲਵ xMin = 0, xMax = 0;
    ਦਸ਼ਮਲਵ yMin = 0, yMax = 0;
    ਜੇ (all_points.ਲੰਮਾਈ > 4)
    {
      xMin = xMax = all_points[0].x;
      yMin = yMax = all_points[0].y;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = all_points.ਲੰਮਾਈ - 4;
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i < ਗਿਣਤੀ_2; i++)
      {
	ਦਸ਼ਮਲਵ x = all_points[i].x;
	ਦਸ਼ਮਲਵ y = all_points[i].y;
	xMin = hb_min (xMin, x);
	xMax = hb_max (xMax, x);
	yMin = hb_min (yMin, y);
	yMax = hb_max (yMax, y);
      }
    }


    // These are destined for storage in a 16 bit field to clamp the values to
    // fit into a 16 bit signed integer.
    ਪੂਰਨ rounded_xMin = hb_clamp (roundf (xMin), -32768.0f, 32767.0f);
    ਪੂਰਨ rounded_xMax = hb_clamp (roundf (xMax), -32768.0f, 32767.0f);
    ਪੂਰਨ rounded_yMin = hb_clamp (roundf (yMin), -32768.0f, 32767.0f);
    ਪੂਰਨ rounded_yMax = hb_clamp (roundf (yMax), -32768.0f, 32767.0f);

    update_mtx (plan, rounded_xMin, rounded_xMax, rounded_yMin, rounded_yMax, all_points);

    ਜੇ (type != ਖਾਲੀ_ਥਾਵਾਂ)
    {
      plan->ਸਿਰ_maxp_info.xMin = hb_min (plan->ਸਿਰ_maxp_info.xMin, rounded_xMin);
      plan->ਸਿਰ_maxp_info.yMin = hb_min (plan->ਸਿਰ_maxp_info.yMin, rounded_yMin);
      plan->ਸਿਰ_maxp_info.xMax = hb_max (plan->ਸਿਰ_maxp_info.xMax, rounded_xMax);
      plan->ਸਿਰ_maxp_info.yMax = hb_max (plan->ਸਿਰ_maxp_info.yMax, rounded_yMax);
    }

    /* when pinned at default, no need to compile glyph header
     * and for empty glyphs: all_points only include phantom points.
     * just update metrics and then return */
    ਜੇ (!glyph_ਸਿਰਲੇਖ)
      ਵਾਪਸ ਸੱਚ;

    glyph_ਸਿਰਲੇਖ->ਸੰਖਿਆ_ਦਾ_Contours = ਸਿਰਲੇਖ->ਸੰਖਿਆ_ਦਾ_Contours;

    glyph_ਸਿਰਲੇਖ->xMin = rounded_xMin;
    glyph_ਸਿਰਲੇਖ->yMin = rounded_yMin;
    glyph_ਸਿਰਲੇਖ->xMax = rounded_xMax;
    glyph_ਸਿਰਲੇਖ->yMax = rounded_yMax;

    dest_ਬਾਈਟਾਂ = hb_ਬਾਈਟਾਂ_t ((ਅਚਲ ਅੱਖਰ *)glyph_ਸਿਰਲੇਖ, Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ);
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ compile_ਬਾਈਟਾਂ_with_deltas (ਅਚਲ hb_subset_plan_t *plan,
                                  hb_font_t *font,
                                  ਅਚਲ glyf_accelerator_t &glyf,
                                  hb_ਬਾਈਟਾਂ_t &dest_ਸ਼ੁਰੂ,  /* IN/OUT */
                                  hb_ਬਾਈਟਾਂ_t &dest_ਸਿਰਾ /*ਬਾਹਰਲਾ*/)
  {
    contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t all_points, points_with_deltas;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੰਯੁਕਤ_contours = 0;
    ਸਿਰ_maxp_info_t *ਸਿਰ_maxp_info_p = &plan->ਸਿਰ_maxp_info;
    ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸੰਯੁਕਤ_contours_p = &ਸੰਯੁਕਤ_contours;

    // don't compute head/maxp values when glyph has no contours(type is EMPTY)
    // also ignore .notdef glyph when --notdef-outline is not enabled
    ਜੇ (type == ਖਾਲੀ_ਥਾਵਾਂ ||
        (gid == 0 && !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE)))
    {
      ਸਿਰ_maxp_info_p = ਖਾਲੀ_ਸੂਚਕ;
      ਸੰਯੁਕਤ_contours_p = ਖਾਲੀ_ਸੂਚਕ;
    }

    hb_glyf_scratch_t scratch;
    ਜੇ (!get_points (font, glyf, all_points, scratch, &points_with_deltas, ਸਿਰ_maxp_info_p, ਸੰਯੁਕਤ_contours_p, ਝੂਠ, ਝੂਠ))
      ਵਾਪਸ ਝੂਠ;

    // .notdef, set type to empty so we only update metrics and don't compile bytes for
    // it
    ਜੇ (gid == 0 &&
        !(plan->flags & HB_SUBSET_FLAGS_NOTDEF_OUTLINE))
    {
      type = ਖਾਲੀ_ਥਾਵਾਂ;
      dest_ਸ਼ੁਰੂ = hb_ਬਾਈਟਾਂ_t ();
      dest_ਸਿਰਾ = hb_ਬਾਈਟਾਂ_t ();
    }

    //dont compile bytes when pinned at default, just recalculate bounds
    ਜੇ (!plan->pinned_at_default)
    {
      ਚੋਣ (type)
      {
      ਹਾਲਤ ਸੰਯੁਕਤ:
        ਜੇ (!ਸੰਯੁਕਤ_Glyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ, extended).compile_ਬਾਈਟਾਂ_with_deltas (dest_ਸ਼ੁਰੂ,
										  points_with_deltas,
										  dest_ਸਿਰਾ))
          ਵਾਪਸ ਝੂਠ;
        ਰੋਕੋ;
      ਹਾਲਤ SIMPLE:
        ਜੇ (!SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).compile_ਬਾਈਟਾਂ_with_deltas (all_points,
								     plan->flags & HB_SUBSET_FLAGS_NO_HINTING,
								     dest_ਸਿਰਾ))
          ਵਾਪਸ ਝੂਠ;
        ਰੋਕੋ;
      ਹਾਲਤ ਖਾਲੀ_ਥਾਵਾਂ:
        dest_ਸ਼ੁਰੂ = hb_ਬਾਈਟਾਂ_t ();
        dest_ਸਿਰਾ = hb_ਬਾਈਟਾਂ_t ();
        /* Zero-contour glyphs may carry instructions for phantom points.
         * Copy them into owned memory like the other compiled glyphs. */
        ਜੇ (!(plan->flags & HB_SUBSET_FLAGS_NO_HINTING) &&
            ਬਾਈਟਾਂ.ਲੰਮਾਈ >= Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ &&
            ਸਿਰਲੇਖ->ਸੰਖਿਆ_ਦਾ_Contours == 0)
        {
          SimpleGlyph simple (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ);
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ = simple.instructions_ਲੰਮਾਈ ();
          ਜੇ (ਲੰਮਾਈ)
          {
            ਲੰਮਾਈ = simple.ਲੰਮਾਈ (ਲੰਮਾਈ);
            ਅੱਖਰ *p = (ਅੱਖਰ *) hb_malloc (ਲੰਮਾਈ);
            ਜੇ (unlikely (!p)) ਵਾਪਸ ਝੂਠ;
            hb_memcpy (p, ਬਾਈਟਾਂ.arrayZ, ਲੰਮਾਈ);
            dest_ਸ਼ੁਰੂ = hb_ਬਾਈਟਾਂ_t (p, ਲੰਮਾਈ);
          }
        }
        ਰੋਕੋ;
      }
    }

    ਜੇ (!compile_ਸਿਰਲੇਖ_ਬਾਈਟਾਂ (plan, all_points, dest_ਸ਼ੁਰੂ))
    {
      dest_ਸਿਰਾ.fini ();
      ਵਾਪਸ ਝੂਠ;
    }
    ਵਾਪਸ ਸੱਚ;
  }


  /* Note: Recursively calls itself.
   * all_points includes phantom points
   */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ accelerator_t>
  ਤਾਰਕਿਕ get_points (hb_font_t *font, ਅਚਲ accelerator_t &glyf_accelerator,
		   contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &all_points /*ਬਾਹਰਲਾ*/,
		   hb_glyf_scratch_t &scratch,
		   contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t *points_with_deltas = ਖਾਲੀ_ਸੂਚਕ, /*ਬਾਹਰਲਾ*/
		   ਸਿਰ_maxp_info_t * ਸਿਰ_maxp_info = ਖਾਲੀ_ਸੂਚਕ, /*ਬਾਹਰਲਾ*/
		   ਬਿਨਾਨਿਸ਼ਾਨੀ *ਸੰਯੁਕਤ_contours = ਖਾਲੀ_ਸੂਚਕ, /*ਬਾਹਰਲਾ*/
		   ਤਾਰਕਿਕ ਖਿਸਕਾਓ_points_hori = ਸੱਚ,
		   ਤਾਰਕਿਕ use_my_metrics = ਸੱਚ,
		   ਤਾਰਕਿਕ phantom_only = ਝੂਠ,
		   hb_array_t<ਅਚਲ ਪੂਰਨ> coords = hb_array_t<ਅਚਲ ਪੂਰਨ> (),
		   hb_scalar_cache_t *gvar_cache = ਖਾਲੀ_ਸੂਚਕ,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਡੂੰਘਾਈ = 0,
		   ਬਿਨਾਨਿਸ਼ਾਨੀ *ਕਿਨਾਰਾ_ਗਿਣਤੀ = ਖਾਲੀ_ਸੂਚਕ,
		   int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਜੇ (unlikely (ਡੂੰਘਾਈ > HB_MAX_NESTING_LEVEL)) ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਢੇਰ_ਕਿਨਾਰਾ_ਗਿਣਤੀ = 0;
    ਜੇ (!ਕਿਨਾਰਾ_ਗਿਣਤੀ) ਕਿਨਾਰਾ_ਗਿਣਤੀ = &ਢੇਰ_ਕਿਨਾਰਾ_ਗਿਣਤੀ;
    ਜੇ (unlikely (*ਕਿਨਾਰਾ_ਗਿਣਤੀ > HB_MAX_ਗ੍ਰਾਫ_ਕਿਨਾਰਾ_ਗਿਣਤੀ)) ਵਾਪਸ ਝੂਠ;
    (*ਕਿਨਾਰਾ_ਗਿਣਤੀ)++;

    int64_t ਢੇਰ_budget = HB_BUDGET_GLYPH;
    ਜੇ (!budget) budget = &ਢੇਰ_budget;
    ਜੇ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16))) ਵਾਪਸ ਝੂਠ;

    ਜੇ (ਸਿਰ_maxp_info)
    {
      ਸਿਰ_maxp_info->max_Component_ਡੂੰਘਾਈ = hb_max (ਸਿਰ_maxp_info->max_Component_ਡੂੰਘਾਈ, ਡੂੰਘਾਈ);
    }

    ਜੇ (!coords && font->has_nonzero_coords)
      coords = hb_array (font->coords, font->ਅੰਕ_coords);

    contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &points = type == SIMPLE ? all_points : scratch.comp_points;
    ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਲੰਮਾਈ = points.ਲੰਮਾਈ;

    ਚੋਣ (type) {
    ਹਾਲਤ SIMPLE:
      ਜੇ (ਡੂੰਘਾਈ == 0 && ਸਿਰ_maxp_info)
        ਸਿਰ_maxp_info->maxContours = hb_max (ਸਿਰ_maxp_info->maxContours, (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਸਿਰਲੇਖ->ਸੰਖਿਆ_ਦਾ_Contours);
      ਜੇ (ਡੂੰਘਾਈ > 0 && ਸੰਯੁਕਤ_contours)
        *ਸੰਯੁਕਤ_contours += (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਸਿਰਲੇਖ->ਸੰਖਿਆ_ਦਾ_Contours;
      ਜੇ (unlikely (!SimpleGlyph (*ਸਿਰਲੇਖ, ਬਾਈਟਾਂ).get_contour_points (all_points, extended, phantom_only, budget)))
	ਵਾਪਸ ਝੂਠ;
      ਰੋਕੋ;
    ਹਾਲਤ ਸੰਯੁਕਤ:
    {
      ਲਈ (ਸਵੈਚਾਲਿਤ &ਵਸਤੂ : get_ਸੰਯੁਕਤ_iterator ())
      {
        /* Preparing a component is a traversal step, even if cycle
         * detection later skips its child glyph. */
        ਜੇ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_16) ||
                      !ਵਸਤੂ.get_points (points, extended)))
        {
          points.resize (old_ਲੰਮਾਈ);
          ਵਾਪਸ ਝੂਠ;
        }
      }
      ਰੋਕੋ;
    }
    ਹਾਲਤ ਖਾਲੀ_ਥਾਵਾਂ:
      ਰੋਕੋ;
    }

    /* Init phantom points */
    ਜੇ (unlikely (!points.resize (points.ਲੰਮਾਈ + PHANTOM_ਗਿਣਤੀ))) ਵਾਪਸ ਝੂਠ;
    hb_array_t<contour_ਬਿੰਦੂ_t> phantoms = points.as_array ().sub_array (points.ਲੰਮਾਈ - PHANTOM_ਗਿਣਤੀ, PHANTOM_ਗਿਣਤੀ);
    {
      // Duplicated code.
      ਪੂਰਨ lsb = 0;
      glyf_accelerator.hmtx->get_leading_bearing_without_var_unscaled (gid, &lsb);
      ਪੂਰਨ h_delta = (ਪੂਰਨ) ਸਿਰਲੇਖ->xMin - lsb;
      HB_ਅਣਵਰਤਿਆ ਪੂਰਨ tsb = 0;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
      glyf_accelerator.vmtx->get_leading_bearing_without_var_unscaled (gid, &tsb);
#ਜੇ_ਅੰਤ
      ਪੂਰਨ v_orig  = (ਪੂਰਨ) ਸਿਰਲੇਖ->yMax + tsb;
      ਬਿਨਾਨਿਸ਼ਾਨੀ h_adv = glyf_accelerator.hmtx->get_advance_without_var_unscaled (gid);
      ਬਿਨਾਨਿਸ਼ਾਨੀ v_adv =
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
                       glyf_accelerator.vmtx->get_advance_without_var_unscaled (gid)
#ਨਹੀਂਤਾਂ
                       - font->face->get_upem ()
#ਜੇ_ਅੰਤ
                       ;
      phantoms[PHANTOM_ਖੱਬਾ].x = h_delta;
      phantoms[PHANTOM_ਸੱਜਾ].x = (ਪੂਰਨ) h_adv + h_delta;
      phantoms[PHANTOM_ਸਿਖਰ].y = v_orig;
      phantoms[PHANTOM_BOTTOM].y = v_orig - (ਪੂਰਨ) v_adv;
    }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VAR
    ਜੇ (hb_any (coords))
    {
      ਜੇ (!glyf_accelerator.gvar->apply_deltas_ਮੰਜ਼ਿਲ_points (gid,
							  coords,
							  points.as_array ().sub_array (old_ਲੰਮਾਈ),
							  scratch,
							  gvar_cache,
							  phantom_only && type == SIMPLE,
							  budget))
        ਵਾਪਸ ਝੂਠ;
    }
#ਜੇ_ਅੰਤ

    // mainly used by CompositeGlyph calculating new X/Y offset value so no need to extend it
    // with child glyphs' points
    ਜੇ (points_with_deltas != ਖਾਲੀ_ਸੂਚਕ && ਡੂੰਘਾਈ == 0 && type == ਸੰਯੁਕਤ)
    {
      ਪ੍ਰਤਿਜ੍ਞਾ (old_ਲੰਮਾਈ == 0);
      *points_with_deltas = points;
    }

    ਦਸ਼ਮਲਵ ਖਿਸਕਾਓ = 0;
    ਚੋਣ (type) {
    ਹਾਲਤ SIMPLE:
      ਜੇ (ਡੂੰਘਾਈ == 0 && ਸਿਰ_maxp_info)
        ਸਿਰ_maxp_info->maxPoints = hb_max (ਸਿਰ_maxp_info->maxPoints, all_points.ਲੰਮਾਈ - old_ਲੰਮਾਈ - 4);
      ਖਿਸਕਾਓ = phantoms[PHANTOM_ਖੱਬਾ].x;
      ਰੋਕੋ;
    ਹਾਲਤ ਸੰਯੁਕਤ:
    {
      hb_decycler_ਗੰਢ_t decycler_ਗੰਢ (scratch.decycler);

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ comp_ਸੂਚਕਾਂਕ = 0;
      ਲਈ (ਸਵੈਚਾਲਿਤ &ਵਸਤੂ : get_ਸੰਯੁਕਤ_iterator ())
      {
	hb_codepoint_t ਵਸਤੂ_gid = ਵਸਤੂ.get_gid (extended);

        ਜੇ (unlikely (!decycler_ਗੰਢ.ਦੇਖੋ (ਵਸਤੂ_gid)))
	{
	  comp_ਸੂਚਕਾਂਕ++;
	  ਜਾਰੀ;
	}

	ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਗਿਣਤੀ = all_points.ਲੰਮਾਈ;

	ਜੇ (unlikely ((!phantom_only || (use_my_metrics && ਵਸਤੂ.is_use_my_metrics ())) &&
		      !glyf_accelerator.glyph_for_gid (ਵਸਤੂ_gid)
				       .get_points (font,
						    glyf_accelerator,
						    all_points,
						    scratch,
						    points_with_deltas,
						    ਸਿਰ_maxp_info,
						    ਸੰਯੁਕਤ_contours,
						    ਖਿਸਕਾਓ_points_hori,
						    use_my_metrics,
						    phantom_only,
						    coords,
						    gvar_cache,
						    ਡੂੰਘਾਈ + 1,
						    ਕਿਨਾਰਾ_ਗਿਣਤੀ,
						    budget)))
	{
	  points.resize (old_ਲੰਮਾਈ);
	  ਵਾਪਸ ਝੂਠ;
	}

	// points might have been reallocated. Relocate phantoms.
	phantoms = points.as_array ().sub_array (points.ਲੰਮਾਈ - PHANTOM_ਗਿਣਤੀ, PHANTOM_ਗਿਣਤੀ);

	ਸਵੈਚਾਲਿਤ comp_points = all_points.as_array ().sub_array (old_ਗਿਣਤੀ);

	/* Copy phantom points from component if USE_MY_METRICS flag set */
	ਜੇ (use_my_metrics && ਵਸਤੂ.is_use_my_metrics ())
	  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < PHANTOM_ਗਿਣਤੀ; i++)
	    phantoms[i] = comp_points[comp_points.ਲੰਮਾਈ - PHANTOM_ਗਿਣਤੀ + i];

	/* Each composite level transforms the whole subtree below it;
	 * charge that work here so deep nesting is bounded by the budget
	 * rather than by the per-draw point cap alone. */
	ਜੇ (unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, comp_points.ਲੰਮਾਈ)))
	{
	  points.resize (old_ਲੰਮਾਈ);
	  ਵਾਪਸ ਝੂਠ;
	}

	ਜੇ (comp_points) // Empty in case of phantom_only
	{
	  ਦਸ਼ਮਲਵ ਮੈਟ੍ਰਿਕਸ[4];
	  contour_ਬਿੰਦੂ_t default_trans;
	  ਵਸਤੂ.get_transformation (ਮੈਟ੍ਰਿਕਸ, default_trans, extended);

	  /* Apply component transformation & translation (with deltas applied) */
	  ਵਸਤੂ.transform_points (comp_points, ਮੈਟ੍ਰਿਕਸ, points[old_ਲੰਮਾਈ + comp_ਸੂਚਕਾਂਕ]);
	}

	ਜੇ (ਵਸਤੂ.is_anchored () && !phantom_only)
	{
	  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ p1, p2;
	  ਵਸਤੂ.get_anchor_points (p1, p2, extended);
	  ਜੇ (likely (p1 < all_points.ਲੰਮਾਈ && p2 < comp_points.ਲੰਮਾਈ))
	  {
	    contour_ਬਿੰਦੂ_t delta;
	    delta.init (all_points[p1].x - comp_points[p2].x,
			all_points[p1].y - comp_points[p2].y);

	    ਵਸਤੂ.translate (delta, comp_points);
	  }
	}

	all_points.resize (all_points.ਲੰਮਾਈ - PHANTOM_ਗਿਣਤੀ);

	ਜੇ (all_points.ਲੰਮਾਈ > HB_GLYF_MAX_POINTS)
	{
	  points.resize (old_ਲੰਮਾਈ);
	  ਵਾਪਸ ਝੂਠ;
	}

	comp_ਸੂਚਕਾਂਕ++;
      }

      ਜੇ (ਸਿਰ_maxp_info && ਡੂੰਘਾਈ == 0)
      {
        ਜੇ (ਸੰਯੁਕਤ_contours)
          ਸਿਰ_maxp_info->max_ਸੰਯੁਕਤ_Contours = hb_max (ਸਿਰ_maxp_info->max_ਸੰਯੁਕਤ_Contours, *ਸੰਯੁਕਤ_contours);
        ਸਿਰ_maxp_info->max_ਸੰਯੁਕਤ_Points = hb_max (ਸਿਰ_maxp_info->max_ਸੰਯੁਕਤ_Points, all_points.ਲੰਮਾਈ);
        ਸਿਰ_maxp_info->maxComponentElements = hb_max (ਸਿਰ_maxp_info->maxComponentElements, comp_ਸੂਚਕਾਂਕ);
      }
      all_points.extend (phantoms);
      ਖਿਸਕਾਓ = phantoms[PHANTOM_ਖੱਬਾ].x;
      points.resize (old_ਲੰਮਾਈ);
    } ਰੋਕੋ;
    ਹਾਲਤ ਖਾਲੀ_ਥਾਵਾਂ:
      all_points.extend (phantoms);
      ਖਿਸਕਾਓ = phantoms[PHANTOM_ਖੱਬਾ].x;
      points.resize (old_ਲੰਮਾਈ);
      ਰੋਕੋ;
    }

    ਜੇ (ਡੂੰਘਾਈ == 0 && ਖਿਸਕਾਓ_points_hori) /* Apply at top level */
    {
      /* Undocumented rasterizer behavior:
       * Shift points horizontally by the updated left side bearing
       */
      ਜੇ (ਖਿਸਕਾਓ)
        ਲਈ (ਸਵੈਚਾਲਿਤ &ਬਿੰਦੂ : all_points)
	  ਬਿੰਦੂ.x -= ਖਿਸਕਾਓ;
    }

    ਵਾਪਸ !all_points.ਵਿੱਚ_error ();
  }

  ਤਾਰਕਿਕ get_extents_without_var_scaled (hb_font_t *font, ਅਚਲ glyf_accelerator_t &glyf_accelerator,
				       hb_glyph_extents_t *extents) ਅਚਲ
  {
    ਜੇ (type == ਖਾਲੀ_ਥਾਵਾਂ)
    {
      *extents = {0, 0, 0, 0};
      ਵਾਪਸ ਸੱਚ; /* Empty glyph; zero extents. */
    }
    ਵਾਪਸ ਸਿਰਲੇਖ->get_extents_without_var_scaled (font, glyf_accelerator, gid, extents);
  }

  hb_ਬਾਈਟਾਂ_t get_ਬਾਈਟਾਂ () ਅਚਲ { ਵਾਪਸ ਬਾਈਟਾਂ; }
  glyph_type_t get_type () ਅਚਲ { ਵਾਪਸ type; }
  ਅਚਲ Glyph_ਸਿਰਲੇਖ *get_ਸਿਰਲੇਖ () ਅਚਲ { ਵਾਪਸ ਸਿਰਲੇਖ; }
  ਤਾਰਕਿਕ is_extended () ਅਚਲ { ਵਾਪਸ extended; }

  Glyph () : ਬਾਈਟਾਂ (),
             ਸਿਰਲੇਖ (ਬਾਈਟਾਂ.as<Glyph_ਸਿਰਲੇਖ> ()),
             gid (-1),
             type(ਖਾਲੀ_ਥਾਵਾਂ),
	     extended (ਝੂਠ)
  {}

  Glyph (hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ_2,
	 hb_codepoint_t gid_,
	 ਤਾਰਕਿਕ extended_) : ਬਾਈਟਾਂ (ਬਾਈਟਾਂ_2),
			   ਸਿਰਲੇਖ (ਬਾਈਟਾਂ.as<Glyph_ਸਿਰਲੇਖ> ()),
			   gid (gid_),
			   extended (extended_)
  {
    ਪੂਰਨ ਅੰਕ_contours = ਸਿਰਲੇਖ->ਸੰਖਿਆ_ਦਾ_Contours;
    ਜੇ (unlikely (ਅੰਕ_contours == 0)) type = ਖਾਲੀ_ਥਾਵਾਂ;
    ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕ_contours > 0) type = SIMPLE;
    ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕ_contours <= -1) type = ਸੰਯੁਕਤ;
    ਨਹੀਂਤਾਂ type = ਖਾਲੀ_ਥਾਵਾਂ; // Spec deviation; Spec says COMPOSITE, but not seen in the wild.
  }

  ਸੁਰੱਖਿਅਤ:
  hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ;
  ਅਚਲ Glyph_ਸਿਰਲੇਖ *ਸਿਰਲੇਖ;
  hb_codepoint_t gid;
  glyph_type_t type;
  ਤਾਰਕਿਕ extended;
};


} /* namespace glyf_impl */
} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_GLYF_GLYPH_HH */
