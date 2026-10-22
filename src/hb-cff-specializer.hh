/*
 * CFF CharString Specializer
 *
 * Optimizes CharString bytecode by using specialized operators
 * (hlineto, vlineto, hhcurveto, etc.) to save bytes and respects
 * CFF1 stack limit (48 values).
 *
 * Based on fontTools.cffLib.specializer
 */

#که_نه_تعریف_شوی HB_CFF_SPECIALIZER_HH
#تعریف_کړه HB_CFF_SPECIALIZER_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
ساكن مضمن منطقي
is_zero (ثابت شمېره_t &n)
{
  أرجع n.منزل_real () == 0.;
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
generalize_commands (hb_ویکټور_t<cs_command_t> &commands)
{
  hb_ویکټور_t<cs_command_t> پایله;
  پایله.alloc (commands.اوږدتیا * 2);  /* Estimate: might expand */

  لكل (بینښه i = 0; i < commands.اوږدتیا; i++)
  {
    خپلکاری &cmd = commands[i];

    بدل (cmd.op)
    {
      حالة Op_کوډ_hmoveto:
      حالة Op_کوډ_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (Op_کوډ_rmoveto);
        gen.دلیلونه.alloc (2);

        إذا (cmd.op == Op_کوډ_hmoveto && cmd.دلیلونه.اوږدتیا >= 1)
        {
          gen.دلیلونه.push (cmd.دلیلونه[0]);  /* dx */
          شمېره_t zero; zero.سېټ_int (0);
          gen.دلیلونه.push (zero);          /* dy = 0 */
        }
        وإلا إذا (cmd.op == Op_کوډ_vmoveto && cmd.دلیلونه.اوږدتیا >= 1)
        {
          شمېره_t zero; zero.سېټ_int (0);
          gen.دلیلونه.push (zero);          /* dx = 0 */
          gen.دلیلونه.push (cmd.دلیلونه[0]);  /* dy */
        }
        پایله.push (gen);
        اكسر;
      }

      حالة Op_کوډ_hlineto:
      حالة Op_کوډ_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        منطقي is_h = (cmd.op == Op_کوډ_hlineto);
        شمېره_t zero; zero.سېټ_int (0);

        لكل (بینښه j = 0; j < cmd.دلیلونه.اوږدتیا; j++)
        {
          cs_command_t seg (Op_کوډ_rlineto);
          seg.دلیلونه.alloc (2);

          منطقي is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          إذا (is_horizontal)
          {
            seg.دلیلونه.push (cmd.دلیلونه[j]);  /* dx */
            seg.دلیلونه.push (zero);          /* dy = 0 */
          }
          وإلا
          {
            seg.دلیلونه.push (zero);          /* dx = 0 */
            seg.دلیلونه.push (cmd.دلیلونه[j]);  /* dy */
          }
          پایله.push (seg);
        }
        اكسر;
      }

      حالة Op_کوډ_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        لكل (بینښه j = 0; j + 1 < cmd.دلیلونه.اوږدتیا; j += 2)
        {
          cs_command_t seg (Op_کوډ_rlineto);
          seg.دلیلونه.alloc (2);
          seg.دلیلونه.push (cmd.دلیلونه[j]);
          seg.دلیلونه.push (cmd.دلیلونه[j + 1]);
          پایله.push (seg);
        }
        اكسر;
      }

      حالة Op_کوډ_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        لكل (بینښه j = 0; j + 5 < cmd.دلیلونه.اوږدتیا; j += 6)
        {
          cs_command_t seg (Op_کوډ_rrcurveto);
          seg.دلیلونه.alloc (6);
          لكل (بینښه k = 0; k < 6; k++)
            seg.دلیلونه.push (cmd.دلیلونه[j + k]);
          پایله.push (seg);
        }
        اكسر;
      }

      افتراضي:
        /* Keep other operators as-is */
        پایله.push (cmd);
        اكسر;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  لكل (بینښه i = 0; i < پایله.اوږدتیا; i++)
    commands.push (پایله[i]);
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
specialize_commands (hb_ویکټور_t<cs_command_t> &commands,
                     بینښه maxstack = 48)
{
  إذا (commands.اوږدتیا == 0) أرجع;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  لكل (بینښه i = 0; i < commands.اوږدتیا; i++)
  {
    خپلکاری &cmd = commands[i];

    إذا ((cmd.op == Op_کوډ_rmoveto || cmd.op == Op_کوډ_rlineto) &&
        cmd.دلیلونه.اوږدتیا == 2)
    {
      منطقي dx_zero = is_zero (cmd.دلیلونه[0]);
      منطقي dy_zero = is_zero (cmd.دلیلونه[1]);

      إذا (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == Op_کوډ_rmoveto) ? Op_کوډ_vmoveto : Op_کوډ_vlineto;
        /* Shift dy to position 0 */
        cmd.دلیلونه[0] = cmd.دلیلونه[1];
        cmd.دلیلونه.resize (1);
      }
      وإلا إذا (!dx_zero && dy_zero)
      {
        /* Horizontal movement (dy=0): keep only dx */
        cmd.op = (cmd.op == Op_کوډ_rmoveto) ? Op_کوډ_hmoveto : Op_کوډ_hlineto;
        cmd.دلیلونه.resize (1);  /* Keep only dx */
      }
      /* else: both zero or both non-zero, keep as rmoveto/rlineto */
    }
  }

  /* Pass 2: Combine adjacent hlineto/vlineto operators
   * hlineto can take multiple args alternating with vlineto
   * This saves operator bytes */
  لكل (صحيح i = (صحيح)commands.اوږدتیا - 1; i > 0; i--)
  {
    خپلکاری &cmd = commands[i];
    خپلکاری &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    إذا (((prev.op == Op_کوډ_hlineto && cmd.op == Op_کوډ_vlineto) ||
         (prev.op == Op_کوډ_vlineto && cmd.op == Op_کوډ_hlineto)) &&
        (prev.دلیلونه.اوږدتیا & 1))
    {
      /* Check stack depth */
      بینښه combined_دلیلونه = prev.دلیلونه.اوږدتیا + cmd.دلیلونه.اوږدتیا;
      إذا (combined_دلیلونه < maxstack)
      {
        /* Merge into first command, keep its operator */
        لكل (بینښه j = 0; j < cmd.دلیلونه.اوږدتیا; j++)
          prev.دلیلونه.push (cmd.دلیلونه[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  لكل (صحيح i = (صحيح)commands.اوږدتیا - 1; i > 0; i--)
  {
    خپلکاری &cmd = commands[i];
    خپلکاری &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    إذا (prev.op == cmd.op &&
        (cmd.op == Op_کوډ_rlineto || cmd.op == Op_کوډ_rrcurveto ||
         ((cmd.op == Op_کوډ_hlineto || cmd.op == Op_کوډ_vlineto) &&
          !(prev.دلیلونه.اوږدتیا & 1))))
    {
      /* Check stack depth */
      بینښه combined_دلیلونه = prev.دلیلونه.اوږدتیا + cmd.دلیلونه.اوږدتیا;
      إذا (combined_دلیلونه < maxstack)
      {
        /* Merge args */
        لكل (بینښه j = 0; j < cmd.دلیلونه.اوږدتیا; j++)
          prev.دلیلونه.push (cmd.دلیلونه[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
ساكن منطقي
encode_commands (ثابت hb_ویکټور_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  لكل (ثابت خپلکاری &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    لكل (ثابت خپلکاری &arg : cmd.دلیلونه)
      encoder.encode_عدد_cs (arg);

    /* Encode operator */
    إذا (cmd.op != Op_کوډ_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    إذا (cmd.op == Op_کوډ_hintmask || cmd.op == Op_کوډ_cntrmask)
    {
      لكل (ثابت خپلکاری &byte : cmd.ماسک_بایټونه)
        encoder.encode_byte (byte);
    }

    إذا (encoder.دننه_error ())
      أرجع خطأ;
  }

  أرجع ریښتیا;
}

} /* namespace CFF */

#د_که_پای /* HB_CFF_SPECIALIZER_HH */
