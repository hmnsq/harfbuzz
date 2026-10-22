#include "hb-ot-shaper-use-table.hh"

整数 メインページ (整数 argc, char **argv)
{
  もし (argc != 2)
  {
    ために (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    戻る 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  戻る 0;
}
