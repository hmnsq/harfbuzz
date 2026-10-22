/*
 * CFF CharString Specializer
 *
 * Optimizes CharString bytecode by using specialized operators
 * (hlineto, vlineto, hhcurveto, etc.) to save bytes and respects
 * CFF1 stack limit (48 values).
 *
 * Based on fontTools.cffLib.specializer
 */

#جيڪڏهن_وصف_نه_ٿيل HB_CFF_SPECIALIZER_HH
#وصف_ڪريو HB_CFF_SPECIALIZER_HH

#ضمن "hb.hh"
#ضمن "hb-cff-interp-cs-common.hh"

نطاق CFF {

/* CharString command representation - forward declared in hb-subset-cff-common.hh */

/* Specialization must preserve nonzero fractional displacements. */
ساكن مضمن منطقي
is_zero (ثابت عدد_t &n)
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
generalize_commands (hb_ويڪٽر_t<cs_command_t> &commands)
{
  hb_ويڪٽر_t<cs_command_t> نتيجو;
  نتيجو.alloc (commands.ڊيگهائي * 2);  /* Estimate: might expand */

  لكل (بنانشان i = 0; i < commands.ڊيگهائي; i++)
  {
    خودڪار &cmd = commands[i];

    بدل (cmd.op)
    {
      حالة Op_ڪوڊ_hmoveto:
      حالة Op_ڪوڊ_vmoveto:
      {
        /* Convert to rmoveto with explicit dx,dy */
        cs_command_t gen (Op_ڪوڊ_rmoveto);
        gen.دليلون.alloc (2);

        إذا (cmd.op == Op_ڪوڊ_hmoveto && cmd.دليلون.ڊيگهائي >= 1)
        {
          gen.دليلون.push (cmd.دليلون[0]);  /* dx */
          عدد_t zero; zero.سيٽ_int (0);
          gen.دليلون.push (zero);          /* dy = 0 */
        }
        وإلا إذا (cmd.op == Op_ڪوڊ_vmoveto && cmd.دليلون.ڊيگهائي >= 1)
        {
          عدد_t zero; zero.سيٽ_int (0);
          gen.دليلون.push (zero);          /* dx = 0 */
          gen.دليلون.push (cmd.دليلون[0]);  /* dy */
        }
        نتيجو.push (gen);
        اكسر;
      }

      حالة Op_ڪوڊ_hlineto:
      حالة Op_ڪوڊ_vlineto:
      {
        /* Convert h/v lineto to rlineto, breaking into single segments
         * hlineto alternates: dx1 (→ dx1,0) dy1 (→ 0,dy1) dx2 (→ dx2,0) ...
         * vlineto alternates: dy1 (→ 0,dy1) dx1 (→ dx1,0) dy2 (→ 0,dy2) ... */
        منطقي is_h = (cmd.op == Op_ڪوڊ_hlineto);
        عدد_t zero; zero.سيٽ_int (0);

        لكل (بنانشان j = 0; j < cmd.دليلون.ڊيگهائي; j++)
        {
          cs_command_t seg (Op_ڪوڊ_rlineto);
          seg.دليلون.alloc (2);

          منطقي is_horizontal = is_h ? (j % 2 == 0) : (j % 2 == 1);
          إذا (is_horizontal)
          {
            seg.دليلون.push (cmd.دليلون[j]);  /* dx */
            seg.دليلون.push (zero);          /* dy = 0 */
          }
          وإلا
          {
            seg.دليلون.push (zero);          /* dx = 0 */
            seg.دليلون.push (cmd.دليلون[j]);  /* dy */
          }
          نتيجو.push (seg);
        }
        اكسر;
      }

      حالة Op_ڪوڊ_rlineto:
      {
        /* Break into single segments (dx,dy pairs) */
        لكل (بنانشان j = 0; j + 1 < cmd.دليلون.ڊيگهائي; j += 2)
        {
          cs_command_t seg (Op_ڪوڊ_rlineto);
          seg.دليلون.alloc (2);
          seg.دليلون.push (cmd.دليلون[j]);
          seg.دليلون.push (cmd.دليلون[j + 1]);
          نتيجو.push (seg);
        }
        اكسر;
      }

      حالة Op_ڪوڊ_rrcurveto:
      {
        /* Break into single segments (6 args each) */
        لكل (بنانشان j = 0; j + 5 < cmd.دليلون.ڊيگهائي; j += 6)
        {
          cs_command_t seg (Op_ڪوڊ_rrcurveto);
          seg.دليلون.alloc (6);
          لكل (بنانشان k = 0; k < 6; k++)
            seg.دليلون.push (cmd.دليلون[j + k]);
          نتيجو.push (seg);
        }
        اكسر;
      }

      افتراضي:
        /* Keep other operators as-is */
        نتيجو.push (cmd);
        اكسر;
    }
  }

  /* Replace commands with generalized result */
  commands.clear ();
  لكل (بنانشان i = 0; i < نتيجو.ڊيگهائي; i++)
    commands.push (نتيجو[i]);
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
specialize_commands (hb_ويڪٽر_t<cs_command_t> &commands,
                     بنانشان maxstack = 48)
{
  إذا (commands.ڊيگهائي == 0) أرجع;

  /* Pass 0: Generalize to canonical form (fontTools does this first) */
  generalize_commands (commands);

  /* Pass 1: Specialize rmoveto/rlineto into h/v variants */
  لكل (بنانشان i = 0; i < commands.ڊيگهائي; i++)
  {
    خودڪار &cmd = commands[i];

    إذا ((cmd.op == Op_ڪوڊ_rmoveto || cmd.op == Op_ڪوڊ_rlineto) &&
        cmd.دليلون.ڊيگهائي == 2)
    {
      منطقي dx_zero = is_zero (cmd.دليلون[0]);
      منطقي dy_zero = is_zero (cmd.دليلون[1]);

      إذا (dx_zero && !dy_zero)
      {
        /* Vertical movement (dx=0): keep only dy */
        cmd.op = (cmd.op == Op_ڪوڊ_rmoveto) ? Op_ڪوڊ_vmoveto : Op_ڪوڊ_vlineto;
        /* Shift dy to position 0 */
        cmd.دليلون[0] = cmd.دليلون[1];
        cmd.دليلون.resize (1);
      }
      وإلا إذا (!dx_zero && dy_zero)
      {
        /* Horizontal movement (dy=0): keep only dx */
        cmd.op = (cmd.op == Op_ڪوڊ_rmoveto) ? Op_ڪوڊ_hmoveto : Op_ڪوڊ_hlineto;
        cmd.دليلون.resize (1);  /* Keep only dx */
      }
      /* else: both zero or both non-zero, keep as rmoveto/rlineto */
    }
  }

  /* Pass 2: Combine adjacent hlineto/vlineto operators
   * hlineto can take multiple args alternating with vlineto
   * This saves operator bytes */
  لكل (صحيح i = (صحيح)commands.ڊيگهائي - 1; i > 0; i--)
  {
    خودڪار &cmd = commands[i];
    خودڪار &prev = commands[i-1];

    /* Different starting directions can combine only after an odd number
     * of alternating arguments in the preceding command. */
    إذا (((prev.op == Op_ڪوڊ_hlineto && cmd.op == Op_ڪوڊ_vlineto) ||
         (prev.op == Op_ڪوڊ_vlineto && cmd.op == Op_ڪوڊ_hlineto)) &&
        (prev.دليلون.ڊيگهائي & 1))
    {
      /* Check stack depth */
      بنانشان combined_دليلون = prev.دليلون.ڊيگهائي + cmd.دليلون.ڊيگهائي;
      إذا (combined_دليلون < maxstack)
      {
        /* Merge into first command, keep its operator */
        لكل (بنانشان j = 0; j < cmd.دليلون.ڊيگهائي; j++)
          prev.دليلون.push (cmd.دليلون[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }

  /* Pass 3: Combine adjacent identical operators */
  لكل (صحيح i = (صحيح)commands.ڊيگهائي - 1; i > 0; i--)
  {
    خودڪار &cmd = commands[i];
    خودڪار &prev = commands[i-1];

    /* Same-direction h/v commands can combine only after an even number
     * of alternating arguments. Other repeatable operators concatenate. */
    إذا (prev.op == cmd.op &&
        (cmd.op == Op_ڪوڊ_rlineto || cmd.op == Op_ڪوڊ_rrcurveto ||
         ((cmd.op == Op_ڪوڊ_hlineto || cmd.op == Op_ڪوڊ_vlineto) &&
          !(prev.دليلون.ڊيگهائي & 1))))
    {
      /* Check stack depth */
      بنانشان combined_دليلون = prev.دليلون.ڊيگهائي + cmd.دليلون.ڊيگهائي;
      إذا (combined_دليلون < maxstack)
      {
        /* Merge args */
        لكل (بنانشان j = 0; j < cmd.دليلون.ڊيگهائي; j++)
          prev.دليلون.push (cmd.دليلون[j]);
        commands.remove_ordered (i);
        i++;  /* Adjust for removed element */
      }
    }
  }
}

/* Encode commands back to binary CharString */
ساكن منطقي
encode_commands (ثابت hb_ويڪٽر_t<cs_command_t> &commands,
                 str_buff_t &output)
{
  لكل (ثابت خودڪار &cmd : commands)
  {
    str_encoder_t encoder (output);

    /* Encode arguments */
    لكل (ثابت خودڪار &arg : cmd.دليلون)
      encoder.encode_انگ_cs (arg);

    /* Encode operator */
    إذا (cmd.op != Op_ڪوڊ_Invalid)
      encoder.encode_op (cmd.op);

    /* hintmask/cntrmask are followed by raw mask bytes. */
    إذا (cmd.op == Op_ڪوڊ_hintmask || cmd.op == Op_ڪوڊ_cntrmask)
    {
      لكل (ثابت خودڪار &byte : cmd.نقاب_بائيٽون)
        encoder.encode_byte (byte);
    }

    إذا (encoder.اندريون_error ())
      أرجع خطأ;
  }

  أرجع سچ;
}

} /* namespace CFF */

#جيڪڏهن_پورو /* HB_CFF_SPECIALIZER_HH */
