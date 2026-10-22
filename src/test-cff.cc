/*
 * Copyright © 2024  David Corbett
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
#అన్తర్భావ "hb-ot-cff-common.hh"
#అన్తర్భావ "hb-subset-cff-common.hh"
#అన్తర్భావ "hb-cff-specializer.hh"

స్థిర శూన్య
test_specialized_పంక్తి_geometry ()
{
  /* Reconstruct every point after specializing horizontal/vertical line
   * sequences, including repeated directions and merged alternating runs. */
  కొరకు (గుర్తులేని n = 1; n <= 8; n++)
    కొరకు (గుర్తులేని ముసుగు = 0; ముసుగు < (1u << n); ముసుగు++)
      కొరకు (గుర్తులేని maxstack : {4u, 48u})
      {
        hb_సదిశ_t<CFF::cs_command_t> commands;
        కొరకు (గుర్తులేని i = 0; i < n; i++)
        {
          తార్కిక horizontal = ముసుగు & (1u << i);
          పూర్ణాంకం delta = (i & 1) ? -(పూర్ణాంకం) i - 1 : (పూర్ణాంకం) i + 1;
          CFF::cs_command_t cmd (Op_సంకేతం_rlineto);
          CFF::సంఖ్య_t dx, dy;
          dx.సమితి_int (horizontal ? delta : 0);
          dy.సమితి_int (horizontal ? 0 : delta);
          cmd.వాదనలు.push (dx);
          cmd.వాదనలు.push (dy);
          commands.push (std::move (cmd));
        }
        CFF::specialize_commands (commands, maxstack);
        కొరకు (స్థిరరాశి స్వయంచాలక &cmd : commands)
          hb_always_assert (cmd.వాదనలు.నిడివి <= maxstack);
        CFF::str_buff_t encoded;
        hb_always_assert (CFF::encode_commands (commands, encoded));
        CFF::generalize_commands (commands);
        hb_always_assert (commands.నిడివి == n);
        పూర్ణాంకం x = 0, y = 0, expected_x = 0, expected_y = 0;
        కొరకు (గుర్తులేని i = 0; i < n; i++)
        {
          తార్కిక horizontal = ముసుగు & (1u << i);
          పూర్ణాంకం delta = (i & 1) ? -(పూర్ణాంకం) i - 1 : (పూర్ణాంకం) i + 1;
          expected_x += horizontal ? delta : 0;
          expected_y += horizontal ? 0 : delta;
          x += commands[i].వాదనలు[0].గమ్యం_int ();
          y += commands[i].వాదనలు[1].గమ్యం_int ();
          hb_always_assert (x == expected_x && y == expected_y);
        }
      }
}

స్థిర శూన్య
test_specialized_fractional_geometry ()
{
  స్థిరరాశి ద్విగుణ deltas[][2] = {
    {0.5, 12}, {-0.5, 12}, {12, 0.5}, {12, -0.5},
    {0.25, -0.75}, {0, 12}, {12, 0},
  };
  కొరకు (CFF::op_సంకేతం_t op : {Op_సంకేతం_rmoveto, Op_సంకేతం_rlineto})
    కొరకు (స్థిరరాశి స్వయంచాలక &delta : deltas)
    {
      hb_సదిశ_t<CFF::cs_command_t> commands;
      CFF::cs_command_t cmd (op);
      CFF::సంఖ్య_t dx, dy;
      dx.సమితి_real (delta[0]);
      dy.సమితి_real (delta[1]);
      cmd.వాదనలు.push (dx);
      cmd.వాదనలు.push (dy);
      commands.push (std::move (cmd));
      CFF::specialize_commands (commands);
      CFF::str_buff_t encoded;
      hb_always_assert (CFF::encode_commands (commands, encoded));
      CFF::generalize_commands (commands);
      hb_always_assert (commands.నిడివి == 1);
      hb_always_assert (commands[0].op == op);
      hb_always_assert (commands[0].వాదనలు.నిడివి == 2);
      hb_always_assert (commands[0].వాదనలు[0].గమ్యం_real () == delta[0]);
      hb_always_assert (commands[0].వాదనలు[1].గమ్యం_real () == delta[1]);
    }
}

మూస <రకం_పేరు GID, రకం_పేరు FD>
స్థిర శూన్య
test_fd_select_sentinel ()
{
  గుర్తులేని అక్షరం దత్తాంశం[3 * GID::static_కొలత + FD::static_కొలత] = {};
  స్వయంచాలక *select = reinterpret_cast<CFF::FDSelect3_4<GID, FD> *> (దత్తాంశం);
  select->ranges.len = 1;
  select->ranges.arrayZ[0].మొదటి = 0;
  select->ranges.arrayZ[0].fd = 0;
  hb_sanitize_context_t c ((స్థిరరాశి అక్షరం *) దత్తాంశం, (స్థిరరాశి అక్షరం *) దత్తాంశం + పరిమాణం (దత్తాంశం));
  c.సమితి_అంకె_glyphs (4);
  c.ప్రారంభం_processing ();

  select->sentinel () = 3;
  hb_always_assert (select->sanitize (&c, 1, 3));
  hb_always_assert (!select->sanitize (&c, 1));
  select->sentinel () = 4;
  hb_always_assert (select->sanitize (&c, 1, 3));
  hb_always_assert (select->sanitize (&c, 1));
  select->sentinel () = 2;
  hb_always_assert (!select->sanitize (&c, 1, 3));
  select->sentinel () = 5;
  hb_always_assert (!select->sanitize (&c, 1, 3));
  c.చివర_processing ();
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  test_specialized_పంక్తి_geometry ();
  test_specialized_fractional_geometry ();
  test_fd_select_sentinel<OT::HBUINT16, OT::HBUINT8> ();
  test_fd_select_sentinel<OT::HBUINT32, OT::HBUINT16> ();

  /* Test encode_num_tp */
  {
    CFF::str_buff_t buff;
    CFF::str_encoder_t encoder (buff);
    CFF::సంఖ్య_t number;
    నిర్మాణం అంకె_tp_test {
      ద్విగుణ ఆదానం;
      గుర్తులేని నిడివి;
      గుర్తులేని అక్షరం output[7];
    };
    నిర్మాణం అంకె_tp_test అంకె_tp_tests[] = {
      { -9.399999999999999, 4, { 0x1E, 0xE9, 0xA4, 0xFF } }, // -9.4
      { 9.399999999999999999, 3, { 0x1E, 0x9A, 0x4F } }, // 9.4
      { 456.8, 4, { 0x1E, 0x45, 0x6A, 0x8F } }, // 456.8
      { 98765.37e2, 5, { 0x1E, 0x98, 0x76, 0x53, 0x7F } }, // 9876537
      { 1234567890.0, 7, { 0x1E, 0x12, 0x34, 0x56, 0x79, 0xB2, 0xFF } }, // 12345679E2
      { 9.876537e-4, 7, { 0x1E, 0x98, 0x76, 0x53, 0x7C, 0x10, 0xFF } }, // 9876537E-10
      { 9.876537e4, 6, { 0x1E, 0x98, 0x76, 0x5A, 0x37, 0xFF } }, // 98765.37
      { 1e8, 3, { 0x1E, 0x1B, 0x8F } }, // 1E8
      { 1e-5, 3, { 0x1E, 0x1C, 0x5F } }, // 1E-5
      { 1.2e8, 4, { 0x1E, 0x12, 0xB7, 0xFF } }, // 12E7
      { 1.2345e-5, 5, { 0x1E, 0x12, 0x34, 0x5C, 0x9F } }, // 12345E-9
      { 9.0987654e8, 6, { 0x1E, 0x90, 0x98, 0x76, 0x54, 0x0F } }, // 909876540
      { 0.1, 3, { 0x1E, 0xA1, 0xFF } }, // .1
      { -0.1, 3, { 0x1E, 0xEA, 0x1F } }, // -.1
      { 0.01, 3, { 0x1E, 0x1C, 0x2F } }, // 1E-2
      { -0.01, 4, { 0x1E, 0xE1, 0xC2, 0xFF } }, // -1E-2
      { 0.0123, 4, { 0x1E, 0x12, 0x3C, 0x4F } }, // 123E-4
      { -0.0123, 5, { 0x1E, 0xE1, 0x23, 0xC4, 0xFF } }, // -123E-4
    };
    కొరకు (size_t t = 0; t < పరిమాణం అంకె_tp_tests / పరిమాణం అంకె_tp_tests[0]; t++)
    {
      నిర్మాణం అంకె_tp_test అంకె_tp_test = అంకె_tp_tests[t];
      number.సమితి_real (అంకె_tp_test.ఆదానం);
      encoder.encode_అంకె_tp (number);
      hb_always_assert (buff.నిడివి == అంకె_tp_test.నిడివి);
      కొరకు (గుర్తులేని i = 0; i < buff.నిడివి; i++)
	hb_always_assert (buff[i] == అంకె_tp_test.output[i]);
      encoder.reset ();
    }
  }

  తిరిగి 0;
}
