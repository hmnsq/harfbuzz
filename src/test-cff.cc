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

#ضمن "hb.hh"
#ضمن "hb-ot-cff-common.hh"
#ضمن "hb-subset-cff-common.hh"
#ضمن "hb-cff-specializer.hh"

ساكن فراغ
test_specialized_کرښه_geometry ()
{
  /* Reconstruct every point after specializing horizontal/vertical line
   * sequences, including repeated directions and merged alternating runs. */
  لكل (بینښه n = 1; n <= 8; n++)
    لكل (بینښه ماسک = 0; ماسک < (1u << n); ماسک++)
      لكل (بینښه maxstack : {4u, 48u})
      {
        hb_ویکټور_t<CFF::cs_command_t> commands;
        لكل (بینښه i = 0; i < n; i++)
        {
          منطقي horizontal = ماسک & (1u << i);
          صحيح delta = (i & 1) ? -(صحيح) i - 1 : (صحيح) i + 1;
          CFF::cs_command_t cmd (Op_کوډ_rlineto);
          CFF::شمېره_t dx, dy;
          dx.سېټ_int (horizontal ? delta : 0);
          dy.سېټ_int (horizontal ? 0 : delta);
          cmd.دلیلونه.push (dx);
          cmd.دلیلونه.push (dy);
          commands.push (std::move (cmd));
        }
        CFF::specialize_commands (commands, maxstack);
        لكل (ثابت خپلکاری &cmd : commands)
          hb_always_assert (cmd.دلیلونه.اوږدتیا <= maxstack);
        CFF::str_buff_t encoded;
        hb_always_assert (CFF::encode_commands (commands, encoded));
        CFF::generalize_commands (commands);
        hb_always_assert (commands.اوږدتیا == n);
        صحيح x = 0, y = 0, expected_x = 0, expected_y = 0;
        لكل (بینښه i = 0; i < n; i++)
        {
          منطقي horizontal = ماسک & (1u << i);
          صحيح delta = (i & 1) ? -(صحيح) i - 1 : (صحيح) i + 1;
          expected_x += horizontal ? delta : 0;
          expected_y += horizontal ? 0 : delta;
          x += commands[i].دلیلونه[0].منزل_int ();
          y += commands[i].دلیلونه[1].منزل_int ();
          hb_always_assert (x == expected_x && y == expected_y);
        }
      }
}

ساكن فراغ
test_specialized_fractional_geometry ()
{
  ثابت مزدوج deltas[][2] = {
    {0.5, 12}, {-0.5, 12}, {12, 0.5}, {12, -0.5},
    {0.25, -0.75}, {0, 12}, {12, 0},
  };
  لكل (CFF::op_کوډ_t op : {Op_کوډ_rmoveto, Op_کوډ_rlineto})
    لكل (ثابت خپلکاری &delta : deltas)
    {
      hb_ویکټور_t<CFF::cs_command_t> commands;
      CFF::cs_command_t cmd (op);
      CFF::شمېره_t dx, dy;
      dx.سېټ_real (delta[0]);
      dy.سېټ_real (delta[1]);
      cmd.دلیلونه.push (dx);
      cmd.دلیلونه.push (dy);
      commands.push (std::move (cmd));
      CFF::specialize_commands (commands);
      CFF::str_buff_t encoded;
      hb_always_assert (CFF::encode_commands (commands, encoded));
      CFF::generalize_commands (commands);
      hb_always_assert (commands.اوږدتیا == 1);
      hb_always_assert (commands[0].op == op);
      hb_always_assert (commands[0].دلیلونه.اوږدتیا == 2);
      hb_always_assert (commands[0].دلیلونه[0].منزل_real () == delta[0]);
      hb_always_assert (commands[0].دلیلونه[1].منزل_real () == delta[1]);
    }
}

کينډۍ <ډول_نوم GID, ډول_نوم FD>
ساكن فراغ
test_fd_select_sentinel ()
{
  بینښه حرف معلومات[3 * GID::static_اندازه + FD::static_اندازه] = {};
  خپلکاری *select = reinterpret_cast<CFF::FDSelect3_4<GID, FD> *> (معلومات);
  select->ranges.len = 1;
  select->ranges.arrayZ[0].لومړی = 0;
  select->ranges.arrayZ[0].fd = 0;
  hb_sanitize_context_t c ((ثابت حرف *) معلومات, (ثابت حرف *) معلومات + حجم (معلومات));
  c.سېټ_عدد_glyphs (4);
  c.پیل_processing ();

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
  c.پای_ټکی_processing ();
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  test_specialized_کرښه_geometry ();
  test_specialized_fractional_geometry ();
  test_fd_select_sentinel<OT::HBUINT16, OT::HBUINT8> ();
  test_fd_select_sentinel<OT::HBUINT32, OT::HBUINT16> ();

  /* Test encode_num_tp */
  {
    CFF::str_buff_t buff;
    CFF::str_encoder_t encoder (buff);
    CFF::شمېره_t number;
    بنية عدد_tp_test {
      مزدوج ننوت;
      بینښه اوږدتیا;
      بینښه حرف output[7];
    };
    بنية عدد_tp_test عدد_tp_tests[] = {
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
    لكل (size_t t = 0; t < حجم عدد_tp_tests / حجم عدد_tp_tests[0]; t++)
    {
      بنية عدد_tp_test عدد_tp_test = عدد_tp_tests[t];
      number.سېټ_real (عدد_tp_test.ننوت);
      encoder.encode_عدد_tp (number);
      hb_always_assert (buff.اوږدتیا == عدد_tp_test.اوږدتیا);
      لكل (بینښه i = 0; i < buff.اوږدتیا; i++)
	hb_always_assert (buff[i] == عدد_tp_test.output[i]);
      encoder.reset ();
    }
  }

  أرجع 0;
}
