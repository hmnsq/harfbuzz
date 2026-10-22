#నిర్వచితం_కాకపోతే OT_GLYF_SIMPLEGLYPH_HH
#నిర్వచించు OT_GLYF_SIMPLEGLYPH_HH


#అన్తర్భావ "../../hb-open-type.hh"


నామాకాశ OT {
నామాకాశ glyf_impl {


నిర్మాణం SimpleGlyph
{
  గణన simple_glyph_flag_t
  {
    FLAG_ON_వక్రం       = 0x01,
    FLAG_X_SHORT        = 0x02,
    FLAG_Y_SHORT        = 0x04,
    FLAG_REPEAT         = 0x08,
    FLAG_X_SAME         = 0x10,
    FLAG_Y_SAME         = 0x20,
    FLAG_OVERLAP_SIMPLE = 0x40,
    FLAG_CUBIC          = 0x80
  };

  స్థిరరాశి Glyph_శీర్షిక &శీర్షిక;
  hb_బైట్లు_t బైట్లు;
  SimpleGlyph (స్థిరరాశి Glyph_శీర్షిక &శీర్షిక_2, hb_బైట్లు_t బైట్లు_2) :
    శీర్షిక (శీర్షిక_2), బైట్లు (బైట్లు_2) {}

  గుర్తులేని పూర్ణాంకం instruction_len_స్థానభ్రంశం () స్థిరరాశి
  { తిరిగి Glyph_శీర్షిక::static_కొలత + 2 * శీర్షిక.సంఖ్య_యొక్క_Contours; }

  గుర్తులేని పూర్ణాంకం నిడివి (గుర్తులేని పూర్ణాంకం instruction_len) స్థిరరాశి
  { తిరిగి instruction_len_స్థానభ్రంశం () + 2 + instruction_len; }

  తార్కిక has_instructions_నిడివి () స్థిరరాశి
  {
    తిరిగి instruction_len_స్థానభ్రంశం () + 2 <= బైట్లు.నిడివి;
  }

  గుర్తులేని పూర్ణాంకం instructions_నిడివి () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం instruction_నిడివి_స్థానభ్రంశం = instruction_len_స్థానభ్రంశం ();
    ఒకవేళ (unlikely (instruction_నిడివి_స్థానభ్రంశం + 2 > బైట్లు.నిడివి)) తిరిగి 0;

    స్థిరరాశి HBUINT16 &instruction_నిడివి = Struct_At_స్థానభ్రంశం<HBUINT16> (&బైట్లు, instruction_నిడివి_స్థానభ్రంశం);
    /* Out of bounds of the current glyph */
    ఒకవేళ (unlikely (నిడివి (instruction_నిడివి) > బైట్లు.నిడివి)) తిరిగి 0;
    తిరిగి instruction_నిడివి;
  }

  స్థిరరాశి hb_బైట్లు_t trim_padding () స్థిరరాశి
  {
    /* based on FontTools _g_l_y_f.py::trim */
    స్థిరరాశి uint8_t *glyph = (uint8_t*) బైట్లు.arrayZ;
    స్థిరరాశి uint8_t *glyph_చివర = glyph + బైట్లు.నిడివి;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_స్థానభ్రంశం ();

    ఒకవేళ (unlikely (glyph + 2 >= glyph_చివర)) తిరిగి hb_బైట్లు_t ();
    గుర్తులేని పూర్ణాంకం అంకె_coordinates = Struct_At_స్థానభ్రంశం<HBUINT16> (glyph - 2, 0) + 1;
    గుర్తులేని పూర్ణాంకం అంకె_instructions = Struct_At_స్థానభ్రంశం<HBUINT16> (glyph, 0);

    glyph += 2 + అంకె_instructions;

    గుర్తులేని పూర్ణాంకం coord_బైట్లు = 0;
    గుర్తులేని పూర్ణాంకం coords_with_flags = 0;
    అయితే (glyph < glyph_చివర)
    {
      uint8_t flag = *glyph;
      glyph++;

      గుర్తులేని పూర్ణాంకం repeat = 1;
      ఒకవేళ (flag & FLAG_REPEAT)
      {
	ఒకవేళ (unlikely (glyph >= glyph_చివర)) తిరిగి hb_బైట్లు_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      గుర్తులేని పూర్ణాంకం x_బైట్లు, y_బైట్లు;
      x_బైట్లు = y_బైట్లు = 0;
      ఒకవేళ (flag & FLAG_X_SHORT) x_బైట్లు = 1;
      లేకపోతే ఒకవేళ ((flag & FLAG_X_SAME) == 0) x_బైట్లు = 2;

      ఒకవేళ (flag & FLAG_Y_SHORT) y_బైట్లు = 1;
      లేకపోతే ఒకవేళ ((flag & FLAG_Y_SAME) == 0) y_బైట్లు = 2;

      coord_బైట్లు += (x_బైట్లు + y_బైట్లు) * repeat;
      coords_with_flags += repeat;
      ఒకవేళ (coords_with_flags >= అంకె_coordinates) ఆపు;
    }

    ఒకవేళ (unlikely (coords_with_flags != అంకె_coordinates)) తిరిగి hb_బైట్లు_t ();
    తిరిగి బైట్లు.sub_array (0, బైట్లు.నిడివి + coord_బైట్లు - (glyph_చివర - glyph));
  }

  /* zero instruction length */
  శూన్య drop_hints ()
  {
    ఒకవేళ (!has_instructions_నిడివి ()) తిరిగి;
    Glyph_శీర్షిక &glyph_శీర్షిక = const_cast<Glyph_శీర్షిక &> (శీర్షిక);
    (HBUINT16 &) Struct_At_స్థానభ్రంశం<HBUINT16> (&glyph_శీర్షిక, instruction_len_స్థానభ్రంశం ()) = 0;
  }

  శూన్య drop_hints_బైట్లు (hb_బైట్లు_t &dest_ప్రారంభం, hb_బైట్లు_t &dest_చివర) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం instructions_len = instructions_నిడివి ();
    గుర్తులేని పూర్ణాంకం glyph_నిడివి = నిడివి (instructions_len);
    dest_ప్రారంభం = బైట్లు.sub_array (0, glyph_నిడివి - instructions_len);
    dest_చివర = బైట్లు.sub_array (glyph_నిడివి, బైట్లు.నిడివి - glyph_నిడివి);
  }

  శూన్య సమితి_overlaps_flag ()
  {
    ఒకవేళ (unlikely (!శీర్షిక.సంఖ్య_యొక్క_Contours)) తిరిగి;

    గుర్తులేని flags_స్థానభ్రంశం = నిడివి (instructions_నిడివి ());
    ఒకవేళ (unlikely (flags_స్థానభ్రంశం + 1 > బైట్లు.నిడివి)) తిరిగి;

    HBUINT8 &మొదటి_flag = (HBUINT8 &) Struct_At_స్థానభ్రంశం<HBUINT16> (&బైట్లు, flags_స్థానభ్రంశం);
    మొదటి_flag = (uint8_t) మొదటి_flag | FLAG_OVERLAP_SIMPLE;
  }

  స్థిర తార్కిక read_flags (స్థిరరాశి HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_బిందువు_t> points_ /* IN/OUT */,
			  స్థిరరాశి HBUINT8 *చివర,
			  తార్కిక extended)
  {
    స్వయంచాలక *points = points_.arrayZ;
    గుర్తులేని లెక్క = points_.నిడివి;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క;)
    {
      ఒకవేళ (unlikely (p + 1 > చివర)) తిరిగి అబద్ధం;
      uint8_t flag = *p++;
      ఒకవేళ (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      ఒకవేళ (flag & FLAG_REPEAT)
      {
	ఒకవేళ (unlikely (p + 1 > చివర)) తిరిగి అబద్ధం;
	గుర్తులేని పూర్ణాంకం repeat_లెక్క = *p++;
	గుర్తులేని stop = hb_min (i + repeat_లెక్క, లెక్క);
	కొరకు (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    తిరిగి నిజం;
  }

  స్థిర తార్కిక read_points (స్థిరరాశి HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_బిందువు_t> points_ /* IN/OUT */,
			   స్థిరరాశి HBUINT8 *చివర,
			   దశాంశం contour_బిందువు_t::*m,
			   స్థిరరాశి simple_glyph_flag_t short_flag,
			   స్థిరరాశి simple_glyph_flag_t same_flag)
  {
    పూర్ణాంకం v = 0;

    కొరకు (స్వయంచాలక &బిందువు : points_)
    {
      గుర్తులేని flag = బిందువు.flag;
      ఒకవేళ (flag & short_flag)
      {
	ఒకవేళ (unlikely (p + 1 > చివర)) తిరిగి అబద్ధం;
	v += (తార్కిక(flag & same_flag) * 2 - 1) * *p++;
      }
      లేకపోతే
      {
	ఒకవేళ (!(flag & same_flag))
	{
	  ఒకవేళ (unlikely (p + HBINT16::static_కొలత > చివర)) తిరిగి అబద్ధం;
	  v += *(స్థిరరాశి HBINT16 *) p;
	  p += HBINT16::static_కొలత;
	}
      }
      బిందువు.*m = v;
    }
    తిరిగి నిజం;
  }

  తార్కిక get_contour_points (contour_బిందువు_సదిశ_t &points /*బహిర్గతం*/,
			   తార్కిక extended,
			   తార్కిక phantom_only = అబద్ధం,
			   int64_t *budget = శూన్యసూచిక) స్థిరరాశి
  {
    స్థిరరాశి HBUINT16 *చివర_Pts_యొక్క_Contours = &StructAfter<HBUINT16> (శీర్షిక);
    పూర్ణాంకం అంకె_contours = శీర్షిక.సంఖ్య_యొక్క_Contours;
    ప్రతిజ్ఞా (అంకె_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    ఒకవేళ (unlikely (!బైట్లు.check_range (&చివర_Pts_యొక్క_Contours[అంకె_contours]))) తిరిగి అబద్ధం;
    గుర్తులేని పూర్ణాంకం అంకె_points = చివర_Pts_యొక్క_Contours[అంకె_contours - 1] + 1;
    ఒకవేళ (unlikely (అంకె_points < (గుర్తులేని) అంకె_contours)) తిరిగి అబద్ధం;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    ఒకవేళ (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, అంకె_points))) తిరిగి అబద్ధం;

    గుర్తులేని old_నిడివి = points.నిడివి;
    points.alloc (points.నిడివి + అంకె_points + 4); // Allocate for phantom points, to avoid a possible copy
    ఒకవేళ (unlikely (!points.resize_dirty (points.నిడివి + అంకె_points))) తిరిగి అబద్ధం;
    స్వయంచాలక points_ = points.as_array ().sub_array (old_నిడివి);
    ఒకవేళ (!phantom_only)
      hb_memset (points_.arrayZ, 0, పరిమాణం (contour_బిందువు_t) * అంకె_points);
    ఒకవేళ (phantom_only) తిరిగి నిజం;

    కొరకు (పూర్ణాంకం i = 0; i < అంకె_contours; i++)
      points_[చివర_Pts_యొక్క_Contours[i]].is_చివర_బిందువు = నిజం;

    /* Skip instructions */
    స్థిరరాశి HBUINT8 *p = &Struct_At_స్థానభ్రంశం<HBUINT8> (&చివర_Pts_యొక్క_Contours[అంకె_contours + 1],
						 చివర_Pts_యొక్క_Contours[అంకె_contours]);

    ఒకవేళ (unlikely ((స్థిరరాశి అక్షరం *) p < బైట్లు.arrayZ)) తిరిగి అబద్ధం; /* Unlikely overflow */
    స్థిరరాశి HBUINT8 *చివర = (స్థిరరాశి HBUINT8 *) (బైట్లు.arrayZ + బైట్లు.నిడివి);
    ఒకవేళ (unlikely (p >= చివర)) తిరిగి అబద్ధం;

    /* Read x & y coordinates */
    తిరిగి read_flags (p, points_, చివర, extended)
        && read_points (p, points_, చివర, &contour_బిందువు_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, చివర, &contour_బిందువు_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  స్థిర శూన్య encode_coord (పూర్ణాంకం value,
                            గుర్తులేని &flag,
                            స్థిరరాశి simple_glyph_flag_t short_flag,
                            స్థిరరాశి simple_glyph_flag_t same_flag,
                            hb_సదిశ_t<uint8_t> &coords /*బహిర్గతం*/)
  {
    ఒకవేళ (value == 0)
    {
      flag |= same_flag;
    }
    లేకపోతే ఒకవేళ (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      ఒకవేళ (value > 0) flag |= same_flag;
      లేకపోతే value = -value;

      coords.arrayZ[coords.నిడివి++] = (uint8_t) value;
    }
    లేకపోతే
    {
      int16_t val = value;
      coords.arrayZ[coords.నిడివి++] = val >> 8;
      coords.arrayZ[coords.నిడివి++] = val & 0xff;
    }
  }

  స్థిర శూన్య encode_flag (గుర్తులేని flag,
                           గుర్తులేని &repeat,
                           గుర్తులేని lastflag,
                           hb_సదిశ_t<uint8_t> &flags /*బహిర్గతం*/)
  {
    ఒకవేళ (flag == lastflag && repeat != 255)
    {
      repeat++;
      ఒకవేళ (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.నిడివి++] = flag;
      }
      లేకపోతే
      {
        గుర్తులేని len = flags.నిడివి;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    లేకపోతే
    {
      repeat = 0;
      flags.arrayZ[flags.నిడివి++] = flag;
    }
  }

  తార్కిక compile_బైట్లు_with_deltas (స్థిరరాశి contour_బిందువు_సదిశ_t &all_points,
                                  తార్కిక no_hinting,
                                  hb_బైట్లు_t &dest_బైట్లు /*బహిర్గతం*/)
  {
    ఒకవేళ (శీర్షిక.సంఖ్య_యొక్క_Contours == 0 || all_points.నిడివి <= 4)
    {
      dest_బైట్లు = hb_బైట్లు_t ();
      తిరిగి నిజం;
    }
    గుర్తులేని అంకె_points = all_points.నిడివి - 4;

    hb_సదిశ_t<uint8_t> flags, x_coords, y_coords;
    ఒకవేళ (unlikely (!flags.alloc_exact (అంకె_points))) తిరిగి అబద్ధం;
    ఒకవేళ (unlikely (!x_coords.alloc_exact (2*అంకె_points))) తిరిగి అబద్ధం;
    ఒకవేళ (unlikely (!y_coords.alloc_exact (2*అంకె_points))) తిరిగి అబద్ధం;

    గుర్తులేని lastflag = 255, repeat = 0;
    పూర్ణాంకం prev_x = 0, prev_y = 0;

    కొరకు (గుర్తులేని i = 0; i < అంకె_points; i++)
    {
      గుర్తులేని flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_వక్రం | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      పూర్ణాంకం cur_x = roundf (all_points.arrayZ[i].x);
      పూర్ణాంకం cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    గుర్తులేని len_before_instrs = 2 * శీర్షిక.సంఖ్య_యొక్క_Contours + 2;
    గుర్తులేని len_instrs = instructions_నిడివి ();
    గుర్తులేని మొత్తం_len = len_before_instrs + flags.నిడివి + x_coords.నిడివి + y_coords.నిడివి;

    ఒకవేళ (!no_hinting)
      మొత్తం_len += len_instrs;

    అక్షరం *p = (అక్షరం *) hb_malloc (మొత్తం_len);
    ఒకవేళ (unlikely (!p)) తిరిగి అబద్ధం;

    స్థిరరాశి అక్షరం *src = బైట్లు.arrayZ + Glyph_శీర్షిక::static_కొలత;
    అక్షరం *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    ఒకవేళ (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.నిడివి);
    cur += flags.నిడివి;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.నిడివి);
    cur += x_coords.నిడివి;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.నిడివి);

    dest_బైట్లు = hb_బైట్లు_t (p, మొత్తం_len);
    తిరిగి నిజం;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#అయితే_ముగింపు /* OT_GLYF_SIMPLEGLYPH_HH */
