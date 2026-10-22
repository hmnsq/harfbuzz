/*
 * Copyright © 2020  Google, Inc.
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
 */
#అన్తర్భావ "hb-ot-var-cvar-table.hh"

// cvar table data from Multi-ABC.ttf
స్థిరరాశి గుర్తులేని అక్షరం cvar_దత్తాంశం[] = "\x0\x1\x0\x0\x0\x2\x0\x14\x0\x51\xa0\x0\xc0\x0\x0\x54\xa0\x0\x40\x0\x2a\x29\x17\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\xd\xff\x0\xfd\x1\x0\xff\x0\xfd\x1\x0\xdb\xdb\xe6\xe6\x82\x0\xfd\x84\x6\xfd\x0\x2\xe3\xe3\xec\xec\x82\x4\x1\xe3\xe3\xec\xec\x82\x0\x1\x2a\x29\x17\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\x1\xd\x1\x0\x5\xfd\x0\x1\x0\x5\xfd\x0\x61\x61\x44\x44\x82\x0\x5\x81\x9\x1\xff\x1\x7\xff\xfb\x49\x49\x35\x35\x82\x4\xff\x49\x49\x35\x35\x82\x0\xff";

స్థిర శూన్య
test_decompile_cvar ()
{
  స్థిరరాశి OT::cvar* cvar_పట్టిక = reinterpret_cast<స్థిరరాశి OT::cvar*> (cvar_దత్తాంశం);
  గుర్తులేని బిందువు_లెక్క = 65;
  గుర్తులేని axis_లెక్క = 1;

  hb_ట్యాగ్_t axis_ట్యాగ్_2 = HB_ట్యాగ్ ('w', 'g', 'h', 't');
  hb_map_t axis_idx_ట్యాగ్_map;
  axis_idx_ట్యాగ్_map.సమితి (0, axis_ట్యాగ్_2);

  OT::Tuple_Variation_దత్తాంశం<>::tuple_variations_t tuple_variations;
  hb_సదిశ_t<గుర్తులేని> shared_indices;
  OT::Tuple_Variation_దత్తాంశం<>::tuple_iterator_t iterator;

  స్థిరరాశి OT::Tuple_Variation_దత్తాంశం<>* tuple_var_దత్తాంశం = reinterpret_cast<స్థిరరాశి OT::Tuple_Variation_దత్తాంశం<>*> (cvar_దత్తాంశం + 4);

  గుర్తులేని len = పరిమాణం (cvar_దత్తాంశం);
  hb_బైట్లు_t var_దత్తాంశం_బైట్లు{(స్థిరరాశి అక్షరం* ) cvar_దత్తాంశం + 4, len - 4};
  తార్కిక ఫలితం = OT::Tuple_Variation_దత్తాంశం<>::get_tuple_iterator (var_దత్తాంశం_బైట్లు, axis_లెక్క, cvar_పట్టిక,
                                                            shared_indices, &iterator);
  hb_always_assert (ఫలితం);

  ఫలితం = tuple_var_దత్తాంశం->decompile_tuple_variations (బిందువు_లెక్క, అబద్ధం, iterator, &axis_idx_ట్యాగ్_map,
                                                       shared_indices, hb_array<స్థిరరాశి OT::F2DOT14> (),
                                                       tuple_variations);

  hb_always_assert (ఫలితం);
  hb_always_assert (tuple_variations.tuple_vars.నిడివి == 2);
  కొరకు (గుర్తులేని i = 0; i < 2; i++)
  {
    hb_always_assert (tuple_variations.tuple_vars[i].axis_tuples.get_population () == 1);
    hb_always_assert (!tuple_variations.tuple_vars[i].deltas_y);
    hb_always_assert (tuple_variations.tuple_vars[i].indices.నిడివి == 65);
    hb_always_assert (tuple_variations.tuple_vars[i].indices.నిడివి == tuple_variations.tuple_vars[i].deltas_x.నిడివి);
  }
  hb_always_assert (tuple_variations.tuple_vars[0].axis_tuples.get (axis_ట్యాగ్_2) == Triple (-1.0, -1.0, 0.0));
  hb_always_assert (tuple_variations.tuple_vars[1].axis_tuples.get (axis_ట్యాగ్_2) == Triple (0.0, 1.0, 1.0));

  hb_సదిశ_t<దశాంశం> deltas_1 {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -3.0, 1.0, 0.0, -1.0, 0.0, -3.0, 1.0, 0.0, -37.0, -37.0, -26.0, -26.0, 0.0, 0.0, 0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 0.0, -3.0, 0.0, 2.0, -29.0, -29.0, -20.0, -20.0, 0.0, 0.0, 0.0, 1.0, -29.0, -29.0, -20.0, -20.0, 0.0, 0.0, 0.0, 1.0};
  కొరకు (గుర్తులేని i = 0; i < 65; i++)
  {
    ఒకవేళ (i < 23)
      hb_always_assert (tuple_variations.tuple_vars[0].indices[i] == 0);
    లేకపోతే
    {
      hb_always_assert (tuple_variations.tuple_vars[0].indices[i] == 1);
      hb_always_assert (tuple_variations.tuple_vars[0].deltas_x[i] == deltas_1[i]);
    }
  }

  hb_సదిశ_t<దశాంశం> deltas_2 {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 5.0, -3.0, 0.0, 1.0, 0.0, 5.0, -3.0, 0.0, 97.0, 97.0, 68.0, 68.0, 0.0, 0.0, 0.0, 5.0, 0.0, 0.0, 1.0, -1.0, 1.0, 7.0, -1.0, -5.0, 73.0, 73.0, 53.0, 53.0, 0.0, 0.0, 0.0, -1.0, 73.0, 73.0, 53.0, 53.0, 0.0, 0.0, 0.0, -1.0};
  కొరకు (గుర్తులేని i = 0 ; i < 65; i++)
  {
    ఒకవేళ (i < 23)
      hb_always_assert (tuple_variations.tuple_vars[1].indices[i] == 0);
    లేకపోతే
    {
      hb_always_assert (tuple_variations.tuple_vars[1].indices[i] == 1);
      hb_always_assert (tuple_variations.tuple_vars[1].deltas_x[i] == deltas_2[i]);
    }
  }

  /* partial instancing wght=300:800 */
  hb_hashmap_t<hb_ట్యాగ్_t, Triple> normalized_axes_location;
  normalized_axes_location.సమితి (axis_ట్యాగ్_2, Triple (-0.512817, 0.0, 0.700012));

  hb_hashmap_t<hb_ట్యాగ్_t, TripleDistances> axes_triple_distances;
  axes_triple_distances.సమితి (axis_ట్యాగ్_2, TripleDistances (1.0, 1.0));

  OT::optimize_scratch_t scratch;
  tuple_variations.instantiate (normalized_axes_location, axes_triple_distances, scratch);

  hb_always_assert (tuple_variations.tuple_vars[0].indices.నిడివి == 65);
  hb_always_assert (tuple_variations.tuple_vars[1].indices.నిడివి == 65);
  hb_always_assert (!tuple_variations.tuple_vars[0].deltas_y);
  hb_always_assert (!tuple_variations.tuple_vars[1].deltas_y);
  hb_always_assert (tuple_variations.tuple_vars[0].axis_tuples.get (axis_ట్యాగ్_2) == Triple (-1.0, -1.0, 0.0));
  hb_always_assert (tuple_variations.tuple_vars[1].axis_tuples.get (axis_ట్యాగ్_2) == Triple (0.0, 1.0, 1.0));

  hb_సదిశ_t<దశాంశం> rounded_deltas_1 {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1, 0.0, -2, 1, 0.0, -1, 0.0, -2, 1, 0.0, -19, -19, -13, -13, 0.0, 0.0, 0.0, -2, 0.0, 0.0, 0.0, 0.0, 0.0, -2, 0.0, 1, -15, -15, -10.0, -10.0, 0.0, 0.0, 0.0, 1, -15, -15, -10.0, -10.0, 0.0, 0.0, 0.0, 1};

  hb_సదిశ_t<దశాంశం> rounded_deltas_2 {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1, 0.0, 4, -2, 0.0, 1, 0.0, 4, -2, 0.0, 68, 68, 48, 48, 0.0, 0.0, 0.0, 4, 0.0, 0.0, 1, -1, 1, 5, -1, -4, 51, 51, 37, 37, 0.0, 0.0, 0.0, -1, 51, 51, 37, 37, 0.0, 0.0, 0.0, -1};

  కొరకు (గుర్తులేని i = 0; i < 65; i++)
  {
    ఒకవేళ (i < 23)
    {
      hb_always_assert (tuple_variations.tuple_vars[0].indices[i] == 0);
      hb_always_assert (tuple_variations.tuple_vars[1].indices[i] == 0);
    }
    లేకపోతే
    {
      hb_always_assert (tuple_variations.tuple_vars[0].indices[i] == 1);
      hb_always_assert (tuple_variations.tuple_vars[1].indices[i] == 1);
      hb_always_assert (roundf (tuple_variations.tuple_vars[0].deltas_x[i]) == rounded_deltas_1[i]);
      hb_always_assert (roundf (tuple_variations.tuple_vars[1].deltas_x[i]) == rounded_deltas_2[i]);
    }
  }

  hb_map_t axes_సూచీ_map;
  axes_సూచీ_map.సమితి (0, 0);
  తార్కిక res = tuple_variations.compile_బైట్లు (axes_సూచీ_map, axis_idx_ట్యాగ్_map, అబద్ధం);
  hb_always_assert (res);
  hb_always_assert (tuple_variations.tuple_vars[0].compiled_tuple_శీర్షిక.నిడివి == 6);
  స్థిరరాశి గుర్తులేని అక్షరం tuple_var_శీర్షిక_1[] = "\x0\x51\xa0\x0\xc0\x0";
  కొరకు (గుర్తులేని i = 0; i < 6; i++)
    hb_always_assert(tuple_variations.tuple_vars[0].compiled_tuple_శీర్షిక.arrayZ[i] == tuple_var_శీర్షిక_1[i]);

  hb_always_assert (tuple_variations.tuple_vars[1].compiled_tuple_శీర్షిక.నిడివి == 6);
  స్థిరరాశి గుర్తులేని అక్షరం tuple_var_శీర్షిక_2[] = "\x0\x54\xa0\x0\x40\x0";
  కొరకు (గుర్తులేని i = 0; i < 6; i++)
    hb_always_assert(tuple_variations.tuple_vars[1].compiled_tuple_శీర్షిక.arrayZ[i] == tuple_var_శీర్షిక_2[i]);

  hb_always_assert (tuple_variations.tuple_vars[0].compiled_deltas.నిడివి == 37);
  hb_always_assert (tuple_variations.tuple_vars[1].compiled_deltas.నిడివి == 40);
  స్థిరరాశి గుర్తులేని అక్షరం compiled_deltas_1[] = "\x0d\xff\x00\xfe\x01\x00\xff\x00\xfe\x01\x00\xed\xed\xf3\xf3\x82\x00\xfe\x84\x06\xfe\x00\x01\xf1\xf1\xf6\xf6\x82\x04\x01\xf1\xf1\xf6\xf6\x82\x00\x01";
  కొరకు (గుర్తులేని i = 0; i < 37; i++)
    hb_always_assert (tuple_variations.tuple_vars[0].compiled_deltas.arrayZ[i] == compiled_deltas_1[i]);

  స్థిరరాశి గుర్తులేని అక్షరం compiled_deltas_2[] = "\x0d\x01\x00\x04\xfe\x00\x01\x00\x04\xfe\x00\x44\x44\x30\x30\x82\x00\x04\x81\x09\x01\xff\x01\x05\xff\xfc\x33\x33\x25\x25\x82\x04\xff\x33\x33\x25\x25\x82\x00\xff";
  కొరకు (గుర్తులేని i = 0; i < 40; i++)
    hb_always_assert (tuple_variations.tuple_vars[1].compiled_deltas.arrayZ[i] == compiled_deltas_2[i]);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  test_decompile_cvar ();
}
