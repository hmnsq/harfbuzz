#अन्तर्भाव "hb-ot-shaper-use-table.hh"

पूर्णाङ्क main (पूर्णाङ्क argc, वर्ण **argv)
{
  यदि (argc != 2)
  {
    लागि (अचिन्हित u = 0; u < 0x10FFFFu; u++)
      म_लिखो ("U+%04X %d\n", u, hb_use_get_category (u));
    फर्काऊ 0;
  }

  hb_codepoint_t u;
  माला_पूछो (argv[1], "%x", &u);

  म_लिखो ("%d\n", hb_use_get_category (u));

  फर्काऊ 0;
}
