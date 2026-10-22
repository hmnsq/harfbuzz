#अन्तर्भाव "hb-ot-shaper-use-table.hh"

पूर्णाङ्क main (पूर्णाङ्क argc, वर्ण **argv)
{
  जइ (argc != 2)
  {
    प्रति (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    प्रत्यागम 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  प्रत्यागम 0;
}
