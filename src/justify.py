#!/usr/bin/env python3

استورد gi

gi.require_نسخو("Gtk", "3.0")
من gi.repository استورد Gtk, HarfBuzz كـ hb


POOL = {}


وصف move_منزل_f(funcs, draw_معلومات, st, منزل_x, منزل_y, user_معلومات):
    context = POOL[draw_معلومات]
    context.move_منزل(منزل_x, منزل_y)


وصف سٽ_منزل_f(funcs, draw_معلومات, st, منزل_x, منزل_y, user_معلومات):
    context = POOL[draw_معلومات]
    context.سٽ_منزل(منزل_x, منزل_y)


وصف cubic_منزل_f(
    funcs,
    draw_معلومات,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    منزل_x,
    منزل_y,
    user_معلومات,
):
    context = POOL[draw_معلومات]
    context.وڪر_منزل(control1_x, control1_y, control2_x, control2_y, منزل_x, منزل_y)


وصف close_رستو_f(funcs, draw_معلومات, st, user_معلومات):
    context = POOL[draw_معلومات]
    context.close_رستو()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_سيٽ_move_منزل_func(DFUNCS, move_منزل_f, ڪجهه_نه)
hb.draw_funcs_سيٽ_سٽ_منزل_func(DFUNCS, سٽ_منزل_f, ڪجهه_نه)
hb.draw_funcs_سيٽ_cubic_منزل_func(DFUNCS, cubic_منزل_f, ڪجهه_نه)
hb.draw_funcs_سيٽ_close_رستو_func(DFUNCS, close_رستو_f, ڪجهه_نه)


وصف ڌڪو_transform_f(funcs, paint_معلومات, xx, yx, xy, yy, dx, dy, user_معلومات):
    ارفع NotImplementedError


وصف ڪڍو_transform_f(funcs, paint_معلومات, user_معلومات):
    ارفع NotImplementedError


وصف color_f(funcs, paint_معلومات, is_foreground, color, user_معلومات):
    context = POOL[paint_معلومات]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.سيٽ_ذريعو_rgba(r, g, b, a)
    context.paint()


وصف ڌڪو_clip_rectangle_f(funcs, paint_معلومات, xmin, ymin, xmax, ymax, user_معلومات):
    context = POOL[paint_معلومات]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


وصف ڌڪو_clip_glyph_f(funcs, paint_معلومات, glyph, font, user_معلومات):
    context = POOL[paint_معلومات]
    context.save()
    context.new_رستو()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_معلومات)
    context.close_رستو()
    context.clip()


وصف ڪڍو_clip_f(funcs, paint_معلومات, user_معلومات):
    context = POOL[paint_معلومات]
    context.restore()


وصف ڌڪو_گروهه_f(funcs, paint_معلومات, user_معلومات):
    ارفع NotImplementedError


وصف ڪڍو_گروهه_f(funcs, paint_معلومات, گهڻائي, user_معلومات):
    ارفع NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_سيٽ_ڌڪو_transform_func(PFUNCS, ڌڪو_transform_f, ڪجهه_نه)
hb.paint_funcs_سيٽ_ڪڍو_transform_func(PFUNCS, ڪڍو_transform_f, ڪجهه_نه)
hb.paint_funcs_سيٽ_color_func(PFUNCS, color_f, ڪجهه_نه)
hb.paint_funcs_سيٽ_ڌڪو_clip_glyph_func(PFUNCS, ڌڪو_clip_glyph_f, ڪجهه_نه)
hb.paint_funcs_سيٽ_ڌڪو_clip_rectangle_func(PFUNCS, ڌڪو_clip_rectangle_f, ڪجهه_نه)
hb.paint_funcs_سيٽ_ڪڍو_clip_func(PFUNCS, ڪڍو_clip_f, ڪجهه_نه)
hb.paint_funcs_سيٽ_ڌڪو_گروهه_func(PFUNCS, ڌڪو_گروهه_f, ڪجهه_نه)
hb.paint_funcs_سيٽ_ڪڍو_گروهه_func(PFUNCS, ڪڍو_گروهه_f, ڪجهه_نه)


وصف makebuffer(لفظ_2):
    buf = hb.بفر_create()

    متن = " ".join(لفظ_2)
    hb.بفر_جوڙيو_codepoints(buf, [ord(c) لكل c في متن], 0, ڊيگهه(متن))

    hb.بفر_guess_segment_properties(buf)

    أرجع buf


وصف justify(face, لفظ_2, advance, هدف_advance):
    font = hb.font_create(face)
    buf = makebuffer(لفظ_2)

    wiggle = 5
    shrink = هدف_advance - wiggle < advance
    expand = هدف_advance + wiggle > advance

    ret, advance, ٽيگ_2, قدر_2 = hb.shape_justify(
        font,
        buf,
        ڪجهه_نه,
        ڪجهه_نه,
        هدف_advance,
        هدف_advance,
        advance,
    )

    إذا ليس ret:
        أرجع خطأ, buf, ڪجهه_نه

    إذا ٽيگ_2:
        variation = hb.variation_t()
        variation.ٽيگ_2 = ٽيگ_2
        variation.قدر_2 = قدر_2
    وإلا:
        variation = ڪجهه_نه

    إذا shrink و advance > هدف_advance + wiggle:
        أرجع خطأ, buf, variation
    إذا expand و advance < هدف_advance - wiggle:
        أرجع خطأ, buf, variation

    أرجع سچ, buf, variation


وصف shape(face, لفظ_2):
    font = hb.font_create(face)
    buf = makebuffer(لفظ_2)
    hb.shape(font, buf)
    positions = hb.بفر_get_glyph_positions(buf)
    advance = جوڙ(p.x_advance لكل p في positions)
    أرجع buf, advance


وصف typeset(face, متن, هدف_advance):
    سٽون = []
    لفظ_2 = []
    لكل لفظ_3 في متن.ورهايو():
        لفظ_2.append(لفظ_3)
        buf, advance = shape(face, لفظ_2)
        إذا advance > هدف_advance:
            # Shrink
            ret, buf, variation = justify(face, لفظ_2, advance, هدف_advance)
            إذا ret:
                سٽون.append((buf, variation))
                لفظ_2 = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            وإلاإذا ڊيگهه(لفظ_2) > 1:
                لفظ_2.ڪڍو()
                _, buf, variation = justify(face, لفظ_2, advance, هدف_advance)
                سٽون.append((buf, variation))
                لفظ_2 = [لفظ_3]
            # But if it is one word, meh.
            وإلا:
                سٽون.append((buf, variation))
                لفظ_2 = []

    # Justify last line
    إذا لفظ_2:
        _, buf, variation = justify(face, لفظ_2, advance, هدف_advance)
        سٽون.append((buf, variation))

    أرجع سٽون


وصف render(face, متن, context, ويڪر, اوچائي, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    هدف_advance = (ويڪر - (margin * 2)) / scale

    سٽون = typeset(face, متن, هدف_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.سٽ_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.سيٽ_font_سائيز(12)
    context.سيٽ_ذريعو_rgb(1, 0, 0)
    لكل buf, variation في سٽون:
        rtl = hb.بفر_get_direction(buf) == hb.direction_t.RTL
        إذا rtl:
            hb.بفر_ابتو(buf)
        infos = hb.بفر_get_glyph_infos(buf)
        positions = hb.بفر_get_glyph_positions(buf)
        advance = جوڙ(p.x_advance لكل p في positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_منزل(0, -20)
        إذا variation:
            ٽيگ_2 = hb.ٽيگ_منزل_string(variation.ٽيگ_2).decode("ascii")
            context.show_متن(f" {tag}={variation.value:g}")
        context.move_منزل(0, 0)
        context.show_متن(f" {advance:g}/{target_advance:g}")
        context.restore()

        إذا variation:
            hb.font_سيٽ_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        إذا rtl:
            context.translate(هدف_advance, 0)

        لكل info, pos في zip(infos, positions):
            إذا rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_وٿي_2, pos.y_وٿي_2)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            إذا ليس rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


وصف مکيه(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    سان open(textpath) كـ f:
        متن = f.اقرأ()

    وصف on_draw(da, context):
        alloc = da.get_ورهاست()
        POOL[id(context)] = context
        render(face, متن, context, alloc.ويڪر, alloc.اوچائي, fontsize)
        هٽايو POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.دري()
    win.connect("destroy", Gtk.main_quit)
    win.سيٽ_default_سائيز(1000, 700)
    win.جوڙيو(drawingarea)

    win.show_all()
    Gtk.مکيه()


إذا __name__ == "__main__":
    استورد argparse

    parser = argparse.دليل_Parser(description="HarfBuzz justification demo.")
    parser.جوڙيو_دليل("fontfile", help="font file")
    parser.جوڙيو_دليل("textfile", help="text")
    دليلون = parser.parse_دليلون()
    مکيه(دليلون.fontfile, دليلون.textfile)
