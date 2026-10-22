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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-cff-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-cff-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cff-specializer.hh"

ਸਥਿਰ ਖਾਲੀ
test_specialized_ਸਤਰ_geometry ()
{
  /* Reconstruct every point after specializing horizontal/vertical line
   * sequences, including repeated directions and merged alternating runs. */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ n = 1; n <= 8; n++)
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ = 0; ਨਕਾਬ < (1u << n); ਨਕਾਬ++)
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ maxstack : {4u, 48u})
      {
        hb_ਸਦਿਸ਼_t<CFF::cs_command_t> commands;
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
        {
          ਤਾਰਕਿਕ horizontal = ਨਕਾਬ & (1u << i);
          ਪੂਰਨ delta = (i & 1) ? -(ਪੂਰਨ) i - 1 : (ਪੂਰਨ) i + 1;
          CFF::cs_command_t cmd (Op_ਕੋਡ_rlineto);
          CFF::ਸੰਖਿਆ_t dx, dy;
          dx.ਸੈੱਟ_int (horizontal ? delta : 0);
          dy.ਸੈੱਟ_int (horizontal ? 0 : delta);
          cmd.ਦਲੀਲਾਂ.push (dx);
          cmd.ਦਲੀਲਾਂ.push (dy);
          commands.push (std::move (cmd));
        }
        CFF::specialize_commands (commands, maxstack);
        ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &cmd : commands)
          hb_always_assert (cmd.ਦਲੀਲਾਂ.ਲੰਮਾਈ <= maxstack);
        CFF::str_buff_t encoded;
        hb_always_assert (CFF::encode_commands (commands, encoded));
        CFF::generalize_commands (commands);
        hb_always_assert (commands.ਲੰਮਾਈ == n);
        ਪੂਰਨ x = 0, y = 0, expected_x = 0, expected_y = 0;
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n; i++)
        {
          ਤਾਰਕਿਕ horizontal = ਨਕਾਬ & (1u << i);
          ਪੂਰਨ delta = (i & 1) ? -(ਪੂਰਨ) i - 1 : (ਪੂਰਨ) i + 1;
          expected_x += horizontal ? delta : 0;
          expected_y += horizontal ? 0 : delta;
          x += commands[i].ਦਲੀਲਾਂ[0].ਮੰਜ਼ਿਲ_int ();
          y += commands[i].ਦਲੀਲਾਂ[1].ਮੰਜ਼ਿਲ_int ();
          hb_always_assert (x == expected_x && y == expected_y);
        }
      }
}

ਸਥਿਰ ਖਾਲੀ
test_specialized_fractional_geometry ()
{
  ਅਚਲ ਦੁੱਗਣਾ deltas[][2] = {
    {0.5, 12}, {-0.5, 12}, {12, 0.5}, {12, -0.5},
    {0.25, -0.75}, {0, 12}, {12, 0},
  };
  ਲਈ (CFF::op_ਕੋਡ_t op : {Op_ਕੋਡ_rmoveto, Op_ਕੋਡ_rlineto})
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &delta : deltas)
    {
      hb_ਸਦਿਸ਼_t<CFF::cs_command_t> commands;
      CFF::cs_command_t cmd (op);
      CFF::ਸੰਖਿਆ_t dx, dy;
      dx.ਸੈੱਟ_real (delta[0]);
      dy.ਸੈੱਟ_real (delta[1]);
      cmd.ਦਲੀਲਾਂ.push (dx);
      cmd.ਦਲੀਲਾਂ.push (dy);
      commands.push (std::move (cmd));
      CFF::specialize_commands (commands);
      CFF::str_buff_t encoded;
      hb_always_assert (CFF::encode_commands (commands, encoded));
      CFF::generalize_commands (commands);
      hb_always_assert (commands.ਲੰਮਾਈ == 1);
      hb_always_assert (commands[0].op == op);
      hb_always_assert (commands[0].ਦਲੀਲਾਂ.ਲੰਮਾਈ == 2);
      hb_always_assert (commands[0].ਦਲੀਲਾਂ[0].ਮੰਜ਼ਿਲ_real () == delta[0]);
      hb_always_assert (commands[0].ਦਲੀਲਾਂ[1].ਮੰਜ਼ਿਲ_real () == delta[1]);
    }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ GID, ਕਿਸਮ_ਨਾਂ FD>
ਸਥਿਰ ਖਾਲੀ
test_fd_select_sentinel ()
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਅੰਕੜੇ[3 * GID::static_ਮਾਪ + FD::static_ਮਾਪ] = {};
  ਸਵੈਚਾਲਿਤ *select = reinterpret_cast<CFF::FDSelect3_4<GID, FD> *> (ਅੰਕੜੇ);
  select->ranges.len = 1;
  select->ranges.arrayZ[0].ਪਹਿਲਾ = 0;
  select->ranges.arrayZ[0].fd = 0;
  hb_sanitize_context_t c ((ਅਚਲ ਅੱਖਰ *) ਅੰਕੜੇ, (ਅਚਲ ਅੱਖਰ *) ਅੰਕੜੇ + ਆਕਾਰ (ਅੰਕੜੇ));
  c.ਸੈੱਟ_ਅੰਕ_glyphs (4);
  c.ਸ਼ੁਰੂ_processing ();

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
  c.ਸਿਰਾ_processing ();
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  test_specialized_ਸਤਰ_geometry ();
  test_specialized_fractional_geometry ();
  test_fd_select_sentinel<OT::HBUINT16, OT::HBUINT8> ();
  test_fd_select_sentinel<OT::HBUINT32, OT::HBUINT16> ();

  /* Test encode_num_tp */
  {
    CFF::str_buff_t buff;
    CFF::str_encoder_t encoder (buff);
    CFF::ਸੰਖਿਆ_t number;
    ਬਣਤਰ ਅੰਕ_tp_test {
      ਦੁੱਗਣਾ ਨਿਵੇਸ਼;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ output[7];
    };
    ਬਣਤਰ ਅੰਕ_tp_test ਅੰਕ_tp_tests[] = {
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
    ਲਈ (size_t t = 0; t < ਆਕਾਰ ਅੰਕ_tp_tests / ਆਕਾਰ ਅੰਕ_tp_tests[0]; t++)
    {
      ਬਣਤਰ ਅੰਕ_tp_test ਅੰਕ_tp_test = ਅੰਕ_tp_tests[t];
      number.ਸੈੱਟ_real (ਅੰਕ_tp_test.ਨਿਵੇਸ਼);
      encoder.encode_ਅੰਕ_tp (number);
      hb_always_assert (buff.ਲੰਮਾਈ == ਅੰਕ_tp_test.ਲੰਮਾਈ);
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < buff.ਲੰਮਾਈ; i++)
	hb_always_assert (buff[i] == ਅੰਕ_tp_test.output[i]);
      encoder.reset ();
    }
  }

  ਵਾਪਸ 0;
}
