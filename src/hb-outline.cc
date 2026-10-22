/*
 * Copyright © 2023  Behdad Esfahbod
 * Copyright © 1999  David Turner
 * Copyright © 2005  Werner Lemberg
 * Copyright © 2013-2015  Alexei Podtelezhnikov
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
 */

#అన్తర్భావ "hb.hh"

#నిర్వచితం_కాకపోతే HB_NO_OUTLINE

#అన్తర్భావ "hb-outline.hh"

#అన్తర్భావ "hb-machinery.hh"


శూన్య hb_outline_t::replay (hb_draw_funcs_t *pen, శూన్య *pen_దత్తాంశం) స్థిరరాశి
{
  hb_draw_state_t st = HB_DRAW_STATE_DEFAULT;

  గుర్తులేని మొదటి = 0;
  కొరకు (గుర్తులేని contour : contours)
  {
    స్వయంచాలక it = points.as_array ().sub_array (మొదటి, contour - మొదటి);
    అయితే (it)
    {
      hb_outline_బిందువు_t p1 = *it++;
      ఎంపిక (p1.type)
      {
	స్థితి hb_outline_బిందువు_t::type_t::MOVE_గమ్యం:
	{
	  pen->move_గమ్యం (pen_దత్తాంశం, st,
			   p1.x, p1.y);
	}
	ఆపు;
	స్థితి hb_outline_బిందువు_t::type_t::పంక్తి_గమ్యం_2:
	{
	  pen->పంక్తి_గమ్యం (pen_దత్తాంశం, st,
			   p1.x, p1.y);
	}
	ఆపు;
	స్థితి hb_outline_బిందువు_t::type_t::QUADRATIC_గమ్యం:
	{
	  hb_outline_బిందువు_t p2 = *it++;
	  pen->quadratic_గమ్యం (pen_దత్తాంశం, st,
				p1.x, p1.y,
				p2.x, p2.y);
	}
	ఆపు;
	స్థితి hb_outline_బిందువు_t::type_t::CUBIC_గమ్యం:
	{
	  hb_outline_బిందువు_t p2 = *it++;
	  hb_outline_బిందువు_t p3 = *it++;
	  pen->cubic_గమ్యం (pen_దత్తాంశం, st,
			    p1.x, p1.y,
			    p2.x, p2.y,
			    p3.x, p3.y);
	}
	ఆపు;
      }
    }
    pen->close_మార్గం (pen_దత్తాంశం, st);
    మొదటి = contour;
  }
}

శూన్య hb_outline_t::translate (దశాంశం dx, దశాంశం dy)
{
  కొరకు (స్వయంచాలక &p : points)
  {
    p.x += dx;
    p.y += dy;
  }
}

శూన్య hb_outline_t::slant (దశాంశం slant_xy)
{
  కొరకు (స్వయంచాలక &p : points)
    p.x += slant_xy * p.y;
}

దశాంశం hb_outline_t::control_area () స్థిరరాశి
{
  దశాంశం a = 0;
  గుర్తులేని మొదటి = 0;
  కొరకు (గుర్తులేని contour : contours)
  {
    కొరకు (గుర్తులేని i = మొదటి; i < contour; i++)
    {
      గుర్తులేని j = i + 1 < contour ? i + 1 : మొదటి;

      స్వయంచాలక &pi = points[i];
      స్వయంచాలక &pj = points[j];
      a += pi.x * pj.y - pi.y * pj.x;
    }

    మొదటి = contour;
  }
  తిరిగి a * .5f;
}

శూన్య hb_outline_t::embolden (దశాంశం x_strength, దశాంశం y_strength,
			     దశాంశం x_జరుపు, దశాంశం y_జరుపు)
{
  /* This function is a straight port of FreeType's FT_Outline_EmboldenXY.
   * Permission has been obtained from the FreeType authors of the code
   * to relicense it under the HarfBuzz license. */

  ఒకవేళ (!x_strength && !y_strength) తిరిగి;
  ఒకవేళ (!points) తిరిగి;

  x_strength /= 2.f;
  y_strength /= 2.f;

  తార్కిక orientation_negative = control_area () < 0;

  గుర్తుగల మొదటి = 0;
  కొరకు (గుర్తులేని c = 0; c < contours.నిడివి; c++)
  {
    hb_outline_సదిశ_t లోపల, బహిర్గతం, anchor, జరుపు;
    దశాంశం l_లోపల, l_బహిర్గతం, l_anchor = 0, l, q, d;

    l_లోపల = 0;
    గుర్తుగల చివరి = (పూర్ణాంకం) contours[c] - 1;

    /* pacify compiler */
    లోపల.x = లోపల.y = anchor.x = anchor.y = 0;

    /* Counter j cycles though the points; counter i advances only  */
    /* when points are moved; anchor k marks the first moved point. */
    కొరకు ( గుర్తుగల i = చివరి, j = మొదటి, k = -1;
	  j != i && i != k;
	  j = j < చివరి ? j + 1 : మొదటి )
    {
      ఒకవేళ ( j != k )
      {
	బహిర్గతం.x = points[j].x - points[i].x;
	బహిర్గతం.y = points[j].y - points[i].y;
	l_బహిర్గతం = బహిర్గతం.normalize_len ();

	ఒకవేళ ( l_బహిర్గతం == 0 )
	  కొనసాగు;
      }
      లేకపోతే
      {
	బహిర్గతం   = anchor;
	l_బహిర్గతం = l_anchor;
      }

      ఒకవేళ ( l_లోపల != 0 )
      {
	ఒకవేళ ( k < 0 )
	{
	  k        = i;
	  anchor   = లోపల;
	  l_anchor = l_లోపల;
	}

	d = లోపల.x * బహిర్గతం.x + లోపల.y * బహిర్గతం.y;

	/* shift only if turn is less than ~160 degrees */
	ఒకవేళ ( d > -15.f/16.f )
	{
	  d = d + 1.f;

	  /* shift components along lateral bisector in proper orientation */
	  జరుపు.x = లోపల.y + బహిర్గతం.y;
	  జరుపు.y = లోపల.x + బహిర్గతం.x;

	  ఒకవేళ ( orientation_negative )
	    జరుపు.x = -జరుపు.x;
	  లేకపోతే
	    జరుపు.y = -జరుపు.y;

	  /* restrict shift magnitude to better handle collapsing segments */
	  q = బహిర్గతం.x * లోపల.y - బహిర్గతం.y * లోపల.x;
	  ఒకవేళ ( orientation_negative )
	    q = -q;

	  l = hb_min (l_లోపల, l_బహిర్గతం);

	  /* non-strict inequalities avoid divide-by-zero when q == l == 0 */
	  ఒకవేళ (x_strength * q <= l * d)
	    జరుపు.x = జరుపు.x * x_strength / d;
	  లేకపోతే
	    జరుపు.x = జరుపు.x * l / q;


	  ఒకవేళ (y_strength * q <= l * d)
	    జరుపు.y = జరుపు.y * y_strength / d;
	  లేకపోతే
	    జరుపు.y = జరుపు.y * l / q;
	}
	లేకపోతే
	  జరుపు.x = జరుపు.y = 0;

	కొరకు ( ;
	      i != j;
	      i = i < చివరి ? i + 1 : మొదటి )
	{
	  points[i].x += x_జరుపు + జరుపు.x;
	  points[i].y += y_జరుపు + జరుపు.y;
	}
      }
      లేకపోతే
	i = j;

      లోపల   = బహిర్గతం;
      l_లోపల = l_బహిర్గతం;
    }

    మొదటి = చివరి + 1;
  }
}

స్థిర శూన్య
hb_outline_recording_pen_move_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
				  శూన్య *దత్తాంశం,
				  hb_draw_state_t *st,
				  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
				  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_outline_t *c = (hb_outline_t *) దత్తాంశం;

  c->points.push (hb_outline_బిందువు_t {గమ్యం_x, గమ్యం_y, hb_outline_బిందువు_t::type_t::MOVE_గమ్యం});
}

స్థిర శూన్య
hb_outline_recording_pen_పంక్తి_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
				  శూన్య *దత్తాంశం,
				  hb_draw_state_t *st,
				  దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
				  శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_outline_t *c = (hb_outline_t *) దత్తాంశం;

  c->points.push (hb_outline_బిందువు_t {గమ్యం_x, గమ్యం_y, hb_outline_బిందువు_t::type_t::పంక్తి_గమ్యం_2});
}

స్థిర శూన్య
hb_outline_recording_pen_quadratic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
				       శూన్య *దత్తాంశం,
				       hb_draw_state_t *st,
				       దశాంశం control_x, దశాంశం control_y,
				       దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
				       శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_outline_t *c = (hb_outline_t *) దత్తాంశం;

  c->points.push (hb_outline_బిందువు_t {control_x, control_y, hb_outline_బిందువు_t::type_t::QUADRATIC_గమ్యం});
  c->points.push (hb_outline_బిందువు_t {గమ్యం_x, గమ్యం_y, hb_outline_బిందువు_t::type_t::QUADRATIC_గమ్యం});
}

స్థిర శూన్య
hb_outline_recording_pen_cubic_గమ్యం (hb_draw_funcs_t *dfuncs HB_వాడని,
				   శూన్య *దత్తాంశం,
				   hb_draw_state_t *st,
				   దశాంశం control1_x, దశాంశం control1_y,
				   దశాంశం control2_x, దశాంశం control2_y,
				   దశాంశం గమ్యం_x, దశాంశం గమ్యం_y,
				   శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_outline_t *c = (hb_outline_t *) దత్తాంశం;

  c->points.push (hb_outline_బిందువు_t {control1_x, control1_y, hb_outline_బిందువు_t::type_t::CUBIC_గమ్యం});
  c->points.push (hb_outline_బిందువు_t {control2_x, control2_y, hb_outline_బిందువు_t::type_t::CUBIC_గమ్యం});
  c->points.push (hb_outline_బిందువు_t {గమ్యం_x, గమ్యం_y, hb_outline_బిందువు_t::type_t::CUBIC_గమ్యం});
}

స్థిర శూన్య
hb_outline_recording_pen_close_మార్గం (hb_draw_funcs_t *dfuncs HB_వాడని,
				     శూన్య *దత్తాంశం,
				     hb_draw_state_t *st,
				     శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_outline_t *c = (hb_outline_t *) దత్తాంశం;

  c->contours.push (c->points.నిడివి);
}

స్థిర int64_t *
hb_outline_recording_pen_get_budget_మిగిలిన (hb_draw_funcs_t *dfuncs HB_వాడని,
					       శూన్య *దత్తాంశం,
					       శూన్య *user_దత్తాంశం HB_వాడని)
{
  hb_outline_t *c = (hb_outline_t *) దత్తాంశం;

  తిరిగి c->budget_మిగిలిన;
}

స్థిర అంతఃవరుస శూన్య free_static_outline_recording_pen_funcs ();

స్థిర నిర్మాణం hb_outline_recording_pen_funcs_lazy_loader_t : hb_draw_funcs_lazy_loader_t<hb_outline_recording_pen_funcs_lazy_loader_t>
{
  స్థిర hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();

    hb_draw_funcs_సమితి_move_గమ్యం_func (funcs, hb_outline_recording_pen_move_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_పంక్తి_గమ్యం_func (funcs, hb_outline_recording_pen_పంక్తి_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_quadratic_గమ్యం_func (funcs, hb_outline_recording_pen_quadratic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_cubic_గమ్యం_func (funcs, hb_outline_recording_pen_cubic_గమ్యం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_close_మార్గం_func (funcs, hb_outline_recording_pen_close_మార్గం, శూన్యసూచిక, శూన్యసూచిక);
    hb_draw_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_outline_recording_pen_get_budget_మిగిలిన, శూన్యసూచిక, శూన్యసూచిక);

    hb_draw_funcs_make_immutable (funcs);

    hb_atexit (free_static_outline_recording_pen_funcs);

    తిరిగి funcs;
  }
} static_outline_recording_pen_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_outline_recording_pen_funcs ()
{
  static_outline_recording_pen_funcs.free_instance ();
}

hb_draw_funcs_t *
hb_outline_recording_pen_get_funcs ()
{
  తిరిగి static_outline_recording_pen_funcs.get_unconst ();
}


#అయితే_ముగింపు
