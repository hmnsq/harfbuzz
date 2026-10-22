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

#شامل "hb.hh"
#شامل "hb-ot-cff-common.hh"
#شامل "hb-subset-cff-common.hh"
#شامل "hb-cff-specializer.hh"

ساکن خالی
test_specialized_سطر_geometry ()
{
  /* Reconstruct every point after specializing horizontal/vertical line
   * sequences, including repeated directions and merged alternating runs. */
  برائے (غیرعلامتی n = 1; n <= 8; n++)
    برائے (غیرعلامتی نقاب = 0; نقاب < (1u << n); نقاب++)
      برائے (غیرعلامتی maxstack : {4u, 48u})
      {
        hb_سمتیہ_t<CFF::cs_command_t> commands;
        برائے (غیرعلامتی i = 0; i < n; i++)
        {
          منطقی horizontal = نقاب & (1u << i);
          صحیح delta = (i & 1) ? -(صحیح) i - 1 : (صحیح) i + 1;
          CFF::cs_command_t cmd (Op_رمز_rlineto);
          CFF::نمبر_t dx, dy;
          dx.سیٹ_int (horizontal ? delta : 0);
          dy.سیٹ_int (horizontal ? 0 : delta);
          cmd.دلائل.push (dx);
          cmd.دلائل.push (dy);
          commands.push (std::move (cmd));
        }
        CFF::specialize_commands (commands, maxstack);
        برائے (ثابت خودکار &cmd : commands)
          hb_always_assert (cmd.دلائل.لمبائی <= maxstack);
        CFF::str_buff_t encoded;
        hb_always_assert (CFF::encode_commands (commands, encoded));
        CFF::generalize_commands (commands);
        hb_always_assert (commands.لمبائی == n);
        صحیح x = 0, y = 0, expected_x = 0, expected_y = 0;
        برائے (غیرعلامتی i = 0; i < n; i++)
        {
          منطقی horizontal = نقاب & (1u << i);
          صحیح delta = (i & 1) ? -(صحیح) i - 1 : (صحیح) i + 1;
          expected_x += horizontal ? delta : 0;
          expected_y += horizontal ? 0 : delta;
          x += commands[i].دلائل[0].مقصد_int ();
          y += commands[i].دلائل[1].مقصد_int ();
          hb_always_assert (x == expected_x && y == expected_y);
        }
      }
}

ساکن خالی
test_specialized_fractional_geometry ()
{
  ثابت دوہرا deltas[][2] = {
    {0.5, 12}, {-0.5, 12}, {12, 0.5}, {12, -0.5},
    {0.25, -0.75}, {0, 12}, {12, 0},
  };
  برائے (CFF::op_رمز_t op : {Op_رمز_rmoveto, Op_رمز_rlineto})
    برائے (ثابت خودکار &delta : deltas)
    {
      hb_سمتیہ_t<CFF::cs_command_t> commands;
      CFF::cs_command_t cmd (op);
      CFF::نمبر_t dx, dy;
      dx.سیٹ_real (delta[0]);
      dy.سیٹ_real (delta[1]);
      cmd.دلائل.push (dx);
      cmd.دلائل.push (dy);
      commands.push (std::move (cmd));
      CFF::specialize_commands (commands);
      CFF::str_buff_t encoded;
      hb_always_assert (CFF::encode_commands (commands, encoded));
      CFF::generalize_commands (commands);
      hb_always_assert (commands.لمبائی == 1);
      hb_always_assert (commands[0].op == op);
      hb_always_assert (commands[0].دلائل.لمبائی == 2);
      hb_always_assert (commands[0].دلائل[0].مقصد_real () == delta[0]);
      hb_always_assert (commands[0].دلائل[1].مقصد_real () == delta[1]);
    }
}

سانچہ <قسم_نام GID, قسم_نام FD>
ساکن خالی
test_fd_select_sentinel ()
{
  غیرعلامتی حرف ڈیٹا[3 * GID::static_سائز + FD::static_سائز] = {};
  خودکار *select = reinterpret_cast<CFF::FDSelect3_4<GID, FD> *> (ڈیٹا);
  select->ranges.len = 1;
  select->ranges.arrayZ[0].پہلا = 0;
  select->ranges.arrayZ[0].fd = 0;
  hb_sanitize_context_t c ((ثابت حرف *) ڈیٹا, (ثابت حرف *) ڈیٹا + حجم (ڈیٹا));
  c.سیٹ_عدد_glyphs (4);
  c.آغاز_processing ();

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
  c.انتہا_processing ();
}

صحیح
مین (صحیح argc, حرف **argv)
{
  test_specialized_سطر_geometry ();
  test_specialized_fractional_geometry ();
  test_fd_select_sentinel<OT::HBUINT16, OT::HBUINT8> ();
  test_fd_select_sentinel<OT::HBUINT32, OT::HBUINT16> ();

  /* Test encode_num_tp */
  {
    CFF::str_buff_t buff;
    CFF::str_encoder_t encoder (buff);
    CFF::نمبر_t number;
    ساخت عدد_tp_test {
      دوہرا داخلہ;
      غیرعلامتی لمبائی;
      غیرعلامتی حرف output[7];
    };
    ساخت عدد_tp_test عدد_tp_tests[] = {
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
    برائے (size_t t = 0; t < حجم عدد_tp_tests / حجم عدد_tp_tests[0]; t++)
    {
      ساخت عدد_tp_test عدد_tp_test = عدد_tp_tests[t];
      number.سیٹ_real (عدد_tp_test.داخلہ);
      encoder.encode_عدد_tp (number);
      hb_always_assert (buff.لمبائی == عدد_tp_test.لمبائی);
      برائے (غیرعلامتی i = 0; i < buff.لمبائی; i++)
	hb_always_assert (buff[i] == عدد_tp_test.output[i]);
      encoder.reset ();
    }
  }

  واپس 0;
}
