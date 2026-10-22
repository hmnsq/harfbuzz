#include "hb-ot-shaper-use-table.hh"

整数 主要 (整数 argc, char **argv)
{
  如果 (argc != 2)
  {
    为 (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    返回 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  返回 0;
}
