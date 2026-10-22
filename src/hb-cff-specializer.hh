/*
 * CFF CharString Specializer
 *
 * Optimizes CharString bytecode by using specialized operators
 * (hlineto, vlineto, hhcurveto, etc.) to save bytes and respects
 * CFF1 stack limit (48 values).
 *
 * Based on fontTools.cffLib.specializer
 */

#اگر_غیر_تعریف_شدہ HB_CFF_SPECIALIZER_HH
#تعریف_کرو HB_CFF_SPECIALIZER_HH

#شامل "hb.hh"
#شامل "hb-cff-interp-cs-common.hh"

نامخلا CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
ساکن اندرونی منطقی
is_zero (ثابت نمبر_t &n)
{
  واپس n.مقصد_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
ساکن خالی
generalize_commands (hb_سمتیہ_t<cs_command_t> &commands)
{
  hb_سمتیہ_t<cs_command_t> نتیجہ;
  نتیجہ.alloc (commands.لمبائی * 2);  /* Estimate: might expand */

  برائے (غیرعلامتی i = 0; i < commands.لمبائی; i++)
  {
    خودکار &cmd = commands[i];

    انتخاب (cmd.op)
    {
      حالت Op_رمز_hmoveto:
      حالت Op_رمز_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (Op_رمز_rmoveto);
        gen.دلائل.alloc (2);

        اگر (cmd.op == Op_رمز_hmoveto && cmd.دلائل.لمبائی >= 1)
        {
          gen.دلائل.push (cmd.دلائل[0]);  /* dx */
          نمبر_t zero; zero.سیٹ_int (0);
          gen.دلائل.push (zero);          /* dy = 0 */
        }
        ورنہ اگر (cmd.op == Op_رمز_vmoveto && cmd.دلائل.لمبائی >= 1)
        {
          نمبر_t zero; zero.سیٹ_int (0);
          gen.دلائل.push (zero);          /* dx = 0 */
          gen.دلائل.push (cmd.دلائل[0]);  /* dy */
        }
        نتیجہ.push (gen);
        توڑو;
      }

      حالت Op_رمز_hlineto:
      حالت Op_رمز_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        منطقی is_h = (cmd.op == Op_رمز_hlineto);
        نمبر_t zero; zero.سیٹ_int (0);

        برائے (غیرعلامتی j = 0; j < cmd.دلائل.لمبائی; j++)
        {
          cs_command_t seg (Op_رمز_rlineto);
          seg.دلائل.alloc (2);

          منطقی is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          اگر (is_horizontal)
          {
            seg.دلائل.push (cmd.دلائل[j]);  /* dx */
            seg.دلائل.push (zero);          /* dy = 0 */
          }
          ورنہ
          {
            seg.دلائل.push (zero);          /* dx = 0 */
            seg.دلائل.push (cmd.دلائل[j]);  /* dy */
          }
          نتیجہ.push (seg);
        }
        توڑو;
      }

      حالت Op_رمز_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        برائے (غیرعلامتی j = 0; j + 1 < cmd.دلائل.لمبائی; j += 2)
        {
          cs_command_t seg (Op_رمز_rlineto);
          seg.دلائل.alloc (2);
          seg.دلائل.push (cmd.دلائل[j]);
          seg.دلائل.push (cmd.دلائل[j + 1]);
          نتیجہ.push (seg);
        }
        توڑو;
      }

      حالت Op_رمز_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        برائے (غیرعلامتی j = 0; j + 5 < cmd.دلائل.لمبائی; j += 6)
        {
          cs_command_t seg (Op_رمز_rrcurveto);
          seg.دلائل.alloc (6);
          برائے (غیرعلامتی k = 0; k < 6; k++)
            seg.دلائل.push (cmd.دلائل[j + k]);
          نتیجہ.push (seg);
        }
        توڑو;
      }

      طےشدہ:
        /* Keep other operators as-is */
        نتیجہ.push (cmd);
        توڑو;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  برائے (غیرعلامتی i = 0; i < نتیجہ.لمبائی; i++)
    commands.push (نتیجہ[i]);
}

/* Specialize CharString commands to optimize bytecode size
 *
 * Follows fontTools approach:
 * 0. Generalize: Break down to canonical single-segment form
 * 1. Specialize: Convert rmoveto/rlineto to h/v variants when dx or dy is zero
 * 2. Combine: Merge adjacent compatible operators
 * 3. Enforce: Respect maxstack limit (default 48 for CFF1)
 *
 * This ensures we never exceed stack depth while optimizing bytecode.
 */
ساکن خالی
specialize_commands (hb_سمتیہ_t<cs_command_t> &commands,
                     غیرعلامتی maxstack = 48)
{
  اگر (commands.لمبائی == 0) واپس;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  برائے (غیرعلامتی i = 0; i < commands.لمبائی; i++)
  {
    خودکار &cmd = commands[i];

    اگر ((cmd.op == Op_رمز_rmoveto || cmd.op == Op_رمز_rlineto) &&
        cmd.دلائل.لمبائی == 2)
    {
      منطقی dx_zero = is_zero (cmd.دلائل[0]);
      منطقی dy_zero = is_zero (cmd.دلائل[1]);

      اگر (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == Op_رمز_rmoveto) ? Op_رمز_vmoveto : Op_رمز_vlineto;
        /* Shift dy to position 0 */
        cmd.دلائل[0] = cmd.دلائل[1];
        cmd.دلائل.resize (1);
      }
      ورنہ اگر (!dx_zero && dy_zero)
      {
        /* Horizontal movement (dy=0): keep only dx */
        cmd.op = (cmd.op == Op_رمز_rmoveto) ? Op_رمز_hmoveto : Op_رمز_hlineto;
        cmd.دلائل.resize (1);  /* Keep only dx */
      }
      /* else: both zero or both non-zero, keep as rmoveto/rlineto */
    }
  }

  /* Pass 2: Combine adjacent hlineto/vlineto operators
   * hlineto can take multiple args alternating with vlineto
   * This saves operator bytes */
  برائے (صحیح i = (صحیح)commands.لمبائی - 1; i > 0; i--)
  {
    خودکار &cmd = commands[i];
    خودکار &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    اگر (((prev.op == Op_رمز_hlineto && cmd.op == Op_رمز_vlineto) ||
         (prev.op == Op_رمز_vlineto && cmd.op == Op_رمز_hlineto)) &&
        (prev.دلائل.لمبائی & 1))
    {
      /* Check stack depth */
      غیرعلامتی combined_دلائل = prev.دلائل.لمبائی + cmd.دلائل.لمبائی;
      اگر (combined_دلائل < maxstack)
      {
        /* Merge into first command, keep its operator */
        برائے (غیرعلامتی j = 0; j < cmd.دلائل.لمبائی; j++)
          prev.دلائل.push (cmd.دلائل[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  برائے (صحیح i = (صحیح)commands.لمبائی - 1; i > 0; i--)
  {
    خودکار &cmd = commands[i];
    خودکار &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    اگر (prev.op == cmd.op &&
        (cmd.op == Op_رمز_rlineto || cmd.op == Op_رمز_rrcurveto ||
         ((cmd.op == Op_رمز_hlineto || cmd.op == Op_رمز_vlineto) &&
          !(prev.دلائل.لمبائی & 1))))
    {
      /* Check stack depth */
      غیرعلامتی combined_دلائل = prev.دلائل.لمبائی + cmd.دلائل.لمبائی;
      اگر (combined_دلائل < maxstack)
      {
        /* Merge args */
        برائے (غیرعلامتی j = 0; j < cmd.دلائل.لمبائی; j++)
          prev.دلائل.push (cmd.دلائل[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
ساکن منطقی
encode_commands (ثابت hb_سمتیہ_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  برائے (ثابت خودکار &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    برائے (ثابت خودکار &arg : cmd.دلائل)
      encoder.encode_عدد_cs (arg);

    /* Encode operator */
    اگر (cmd.op != Op_رمز_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    اگر (cmd.op == Op_رمز_hintmask || cmd.op == Op_رمز_cntrmask)
    {
      برائے (ثابت خودکار &byte : cmd.نقاب_بائٹیں)
        encoder.encode_byte (byte);
    }

    اگر (encoder.اندر_error ())
      واپس جھوٹ;
  }

  واپس سچ;
}

} /* namespace CFF */

#اگر_ختم /* HB_CFF_SPECIALIZER_HH */
