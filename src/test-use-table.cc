#include "hb-ot-shaper-use-table.hh"

จำนวนเต็ม หลัก (จำนวนเต็ม argc, var_na **argv)
{
  ถ้า (argc != 2)
  {
    สำหรับ (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    คืน 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  คืน 0;
}
