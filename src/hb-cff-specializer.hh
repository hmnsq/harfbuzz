/*
 * CFF CharString Specializer
 *
 * Optimizes CharString bytecode by using specialized operators
 * (hlineto, vlineto, hhcurveto, etc.) to save bytes and respects
 * CFF1 stack limit (48 values).
 *
 * Based on fontTools.cffLib.specializer
 */

#اگر_تعریف_نشده HB_CFF_SPECIALIZER_HH
#تعریف_کن HB_CFF_SPECIALIZER_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
ساكن مضمن منطقي
is_zero (ثابت عدد_t &n)
{
  أرجع n.مقصد_real () == 0.;
}

/* Generalize CharString commands to canonical form
 *
 * Converts all operators to their general forms and breaks down
 * multi-segment operators into single segments. This ensures we
 * start from a clean baseline before specialization.
 *
 * Based on fontTools.cffLib.specializer.generalizeCommands
 */
ساكن فراغ
generalize_commands (hb_وکتور_t<cs_command_t> &commands)
{
  hb_وکتور_t<cs_command_t> نتیجه;
  نتیجه.alloc (commands.درازی * 2);  /* Estimate: might expand */

  لكل (بیعلامت i = 0; i < commands.درازی; i++)
  {
    خودکار &cmd = commands[i];

    بدل (cmd.op)
    {
      حالة Op_کد_hmoveto:
      حالة Op_کد_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (Op_کد_rmoveto);
        gen.آرگومانها.alloc (2);

        إذا (cmd.op == Op_کد_hmoveto && cmd.آرگومانها.درازی >= 1)
        {
          gen.آرگومانها.push (cmd.آرگومانها[0]);  /* dx */
          عدد_t zero; zero.مجموعه_int (0);
          gen.آرگومانها.push (zero);          /* dy = 0 */
        }
        وإلا إذا (cmd.op == Op_کد_vmoveto && cmd.آرگومانها.درازی >= 1)
        {
          عدد_t zero; zero.مجموعه_int (0);
          gen.آرگومانها.push (zero);          /* dx = 0 */
          gen.آرگومانها.push (cmd.آرگومانها[0]);  /* dy */
        }
        نتیجه.push (gen);
        اكسر;
      }

      حالة Op_کد_hlineto:
      حالة Op_کد_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        منطقي is_h = (cmd.op == Op_کد_hlineto);
        عدد_t zero; zero.مجموعه_int (0);

        لكل (بیعلامت j = 0; j < cmd.آرگومانها.درازی; j++)
        {
          cs_command_t seg (Op_کد_rlineto);
          seg.آرگومانها.alloc (2);

          منطقي is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          إذا (is_horizontal)
          {
            seg.آرگومانها.push (cmd.آرگومانها[j]);  /* dx */
            seg.آرگومانها.push (zero);          /* dy = 0 */
          }
          وإلا
          {
            seg.آرگومانها.push (zero);          /* dx = 0 */
            seg.آرگومانها.push (cmd.آرگومانها[j]);  /* dy */
          }
          نتیجه.push (seg);
        }
        اكسر;
      }

      حالة Op_کد_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        لكل (بیعلامت j = 0; j + 1 < cmd.آرگومانها.درازی; j += 2)
        {
          cs_command_t seg (Op_کد_rlineto);
          seg.آرگومانها.alloc (2);
          seg.آرگومانها.push (cmd.آرگومانها[j]);
          seg.آرگومانها.push (cmd.آرگومانها[j + 1]);
          نتیجه.push (seg);
        }
        اكسر;
      }

      حالة Op_کد_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        لكل (بیعلامت j = 0; j + 5 < cmd.آرگومانها.درازی; j += 6)
        {
          cs_command_t seg (Op_کد_rrcurveto);
          seg.آرگومانها.alloc (6);
          لكل (بیعلامت k = 0; k < 6; k++)
            seg.آرگومانها.push (cmd.آرگومانها[j + k]);
          نتیجه.push (seg);
        }
        اكسر;
      }

      افتراضي:
        /* Keep other operators as-is */
        نتیجه.push (cmd);
        اكسر;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  لكل (بیعلامت i = 0; i < نتیجه.درازی; i++)
    commands.push (نتیجه[i]);
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
ساكن فراغ
specialize_commands (hb_وکتور_t<cs_command_t> &commands,
                     بیعلامت maxstack = 48)
{
  إذا (commands.درازی == 0) أرجع;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  لكل (بیعلامت i = 0; i < commands.درازی; i++)
  {
    خودکار &cmd = commands[i];

    إذا ((cmd.op == Op_کد_rmoveto || cmd.op == Op_کد_rlineto) &&
        cmd.آرگومانها.درازی == 2)
    {
      منطقي dx_zero = is_zero (cmd.آرگومانها[0]);
      منطقي dy_zero = is_zero (cmd.آرگومانها[1]);

      إذا (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == Op_کد_rmoveto) ? Op_کد_vmoveto : Op_کد_vlineto;
        /* Shift dy to position 0 */
        cmd.آرگومانها[0] = cmd.آرگومانها[1];
        cmd.آرگومانها.resize (1);
      }
      وإلا إذا (!dx_zero && dy_zero)
      {
        /* Horizontal movement (dy=0): keep only dx */
        cmd.op = (cmd.op == Op_کد_rmoveto) ? Op_کد_hmoveto : Op_کد_hlineto;
        cmd.آرگومانها.resize (1);  /* Keep only dx */
      }
      /* else: both zero or both non-zero, keep as rmoveto/rlineto */
    }
  }

  /* Pass 2: Combine adjacent hlineto/vlineto operators
   * hlineto can take multiple args alternating with vlineto
   * This saves operator bytes */
  لكل (صحيح i = (صحيح)commands.درازی - 1; i > 0; i--)
  {
    خودکار &cmd = commands[i];
    خودکار &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    إذا (((prev.op == Op_کد_hlineto && cmd.op == Op_کد_vlineto) ||
         (prev.op == Op_کد_vlineto && cmd.op == Op_کد_hlineto)) &&
        (prev.آرگومانها.درازی & 1))
    {
      /* Check stack depth */
      بیعلامت combined_آرگومانها = prev.آرگومانها.درازی + cmd.آرگومانها.درازی;
      إذا (combined_آرگومانها < maxstack)
      {
        /* Merge into first command, keep its operator */
        لكل (بیعلامت j = 0; j < cmd.آرگومانها.درازی; j++)
          prev.آرگومانها.push (cmd.آرگومانها[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  لكل (صحيح i = (صحيح)commands.درازی - 1; i > 0; i--)
  {
    خودکار &cmd = commands[i];
    خودکار &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    إذا (prev.op == cmd.op &&
        (cmd.op == Op_کد_rlineto || cmd.op == Op_کد_rrcurveto ||
         ((cmd.op == Op_کد_hlineto || cmd.op == Op_کد_vlineto) &&
          !(prev.آرگومانها.درازی & 1))))
    {
      /* Check stack depth */
      بیعلامت combined_آرگومانها = prev.آرگومانها.درازی + cmd.آرگومانها.درازی;
      إذا (combined_آرگومانها < maxstack)
      {
        /* Merge args */
        لكل (بیعلامت j = 0; j < cmd.آرگومانها.درازی; j++)
          prev.آرگومانها.push (cmd.آرگومانها[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
ساكن منطقي
encode_commands (ثابت hb_وکتور_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  لكل (ثابت خودکار &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    لكل (ثابت خودکار &arg : cmd.آرگومانها)
      encoder.encode_رقم_cs (arg);

    /* Encode operator */
    إذا (cmd.op != Op_کد_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    إذا (cmd.op == Op_کد_hintmask || cmd.op == Op_کد_cntrmask)
    {
      لكل (ثابت خودکار &byte : cmd.ماسک_بایتها)
        encoder.encode_byte (byte);
    }

    إذا (encoder.درون_error ())
      أرجع خطأ;
  }

  أرجع درست;
}

} /* namespace CFF */

#پایان_اگر /* HB_CFF_SPECIALIZER_HH */
