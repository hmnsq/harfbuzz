#!/usr/bin/env python3

استورد gi

gi.require_نسخہ("Gtk", "3.0")
من gi.repository استورد Gtk, HarfBuzz كـ hb


POOL = {}


تعریف move_منزل_f(funcs, draw_انکڑے, st, منزل_x, منزل_y, user_انکڑے):
    context = POOL[draw_انکڑے]
    context.move_منزل(منزل_x, منزل_y)


تعریف سطر_منزل_f(funcs, draw_انکڑے, st, منزل_x, منزل_y, user_انکڑے):
    context = POOL[draw_انکڑے]
    context.سطر_منزل(منزل_x, منزل_y)


تعریف cubic_منزل_f(
    funcs,
    draw_انکڑے,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    منزل_x,
    منزل_y,
    user_انکڑے,
):
    context = POOL[draw_انکڑے]
    context.ونگ_منزل(control1_x, control1_y, control2_x, control2_y, منزل_x, منزل_y)


تعریف close_رستہ_f(funcs, draw_انکڑے, st, user_انکڑے):
    context = POOL[draw_انکڑے]
    context.close_رستہ()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_سیٹ_move_منزل_func(DFUNCS, move_منزل_f, کجھ_نئیں)
hb.draw_funcs_سیٹ_سطر_منزل_func(DFUNCS, سطر_منزل_f, کجھ_نئیں)
hb.draw_funcs_سیٹ_cubic_منزل_func(DFUNCS, cubic_منزل_f, کجھ_نئیں)
hb.draw_funcs_سیٹ_close_رستہ_func(DFUNCS, close_رستہ_f, کجھ_نئیں)


تعریف ڈالو_transform_f(funcs, paint_انکڑے, xx, yx, xy, yy, dx, dy, user_انکڑے):
    ارفع NotImplementedError


تعریف نکالو_transform_f(funcs, paint_انکڑے, user_انکڑے):
    ارفع NotImplementedError


تعریف color_f(funcs, paint_انکڑے, is_foreground, color, user_انکڑے):
    context = POOL[paint_انکڑے]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.سیٹ_سروت_rgba(r, g, b, a)
    context.paint()


تعریف ڈالو_clip_rectangle_f(funcs, paint_انکڑے, xmin, ymin, xmax, ymax, user_انکڑے):
    context = POOL[paint_انکڑے]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


تعریف ڈالو_clip_glyph_f(funcs, paint_انکڑے, glyph, font, user_انکڑے):
    context = POOL[paint_انکڑے]
    context.save()
    context.new_رستہ()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_انکڑے)
    context.close_رستہ()
    context.clip()


تعریف نکالو_clip_f(funcs, paint_انکڑے, user_انکڑے):
    context = POOL[paint_انکڑے]
    context.restore()


تعریف ڈالو_گروہ_f(funcs, paint_انکڑے, user_انکڑے):
    ارفع NotImplementedError


تعریف نکالو_گروہ_f(funcs, paint_انکڑے, منوال, user_انکڑے):
    ارفع NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_سیٹ_ڈالو_transform_func(PFUNCS, ڈالو_transform_f, کجھ_نئیں)
hb.paint_funcs_سیٹ_نکالو_transform_func(PFUNCS, نکالو_transform_f, کجھ_نئیں)
hb.paint_funcs_سیٹ_color_func(PFUNCS, color_f, کجھ_نئیں)
hb.paint_funcs_سیٹ_ڈالو_clip_glyph_func(PFUNCS, ڈالو_clip_glyph_f, کجھ_نئیں)
hb.paint_funcs_سیٹ_ڈالو_clip_rectangle_func(PFUNCS, ڈالو_clip_rectangle_f, کجھ_نئیں)
hb.paint_funcs_سیٹ_نکالو_clip_func(PFUNCS, نکالو_clip_f, کجھ_نئیں)
hb.paint_funcs_سیٹ_ڈالو_گروہ_func(PFUNCS, ڈالو_گروہ_f, کجھ_نئیں)
hb.paint_funcs_سیٹ_نکالو_گروہ_func(PFUNCS, نکالو_گروہ_f, کجھ_نئیں)


تعریف makebuffer(الفاظ):
    buf = hb.بفر_create()

    متن = " ".join(الفاظ)
    hb.بفر_جوڑو_codepoints(buf, [ord(c) لكل c في متن], 0, لمبائی(متن))

    hb.بفر_guess_segment_properties(buf)

    أرجع buf


تعریف justify(face, الفاظ, advance, نشانہ_advance):
    font = hb.font_create(face)
    buf = makebuffer(الفاظ)

    wiggle = 5
    shrink = نشانہ_advance - wiggle < advance
    expand = نشانہ_advance + wiggle > advance

    ret, advance, ٹیگ_2, قدر_2 = hb.shape_justify(
        font,
        buf,
        کجھ_نئیں,
        کجھ_نئیں,
        نشانہ_advance,
        نشانہ_advance,
        advance,
    )

    إذا ليس ret:
        أرجع خطأ, buf, کجھ_نئیں

    إذا ٹیگ_2:
        variation = hb.variation_t()
        variation.ٹیگ_2 = ٹیگ_2
        variation.قدر_2 = قدر_2
    وإلا:
        variation = کجھ_نئیں

    إذا shrink و advance > نشانہ_advance + wiggle:
        أرجع خطأ, buf, variation
    إذا expand و advance < نشانہ_advance - wiggle:
        أرجع خطأ, buf, variation

    أرجع سچ, buf, variation


تعریف shape(face, الفاظ):
    font = hb.font_create(face)
    buf = makebuffer(الفاظ)
    hb.shape(font, buf)
    positions = hb.بفر_get_glyph_positions(buf)
    advance = جوڑ(p.x_advance لكل p في positions)
    أرجع buf, advance


تعریف typeset(face, متن, نشانہ_advance):
    سطریں = []
    الفاظ = []
    لكل لفظ_2 في متن.ونڈو():
        الفاظ.append(لفظ_2)
        buf, advance = shape(face, الفاظ)
        إذا advance > نشانہ_advance:
            # Shrink
            ret, buf, variation = justify(face, الفاظ, advance, نشانہ_advance)
            إذا ret:
                سطریں.append((buf, variation))
                الفاظ = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            وإلاإذا لمبائی(الفاظ) > 1:
                الفاظ.نکالو()
                _, buf, variation = justify(face, الفاظ, advance, نشانہ_advance)
                سطریں.append((buf, variation))
                الفاظ = [لفظ_2]
            # But if it is one word, meh.
            وإلا:
                سطریں.append((buf, variation))
                الفاظ = []

    # Justify last line
    إذا الفاظ:
        _, buf, variation = justify(face, الفاظ, advance, نشانہ_advance)
        سطریں.append((buf, variation))

    أرجع سطریں


تعریف render(face, متن, context, چوڑائی, اچائی, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    نشانہ_advance = (چوڑائی - (margin * 2)) / scale

    سطریں = typeset(face, متن, نشانہ_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.سطر_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.سیٹ_font_ناپ(12)
    context.سیٹ_سروت_rgb(1, 0, 0)
    لكل buf, variation في سطریں:
        rtl = hb.بفر_get_direction(buf) == hb.direction_t.RTL
        إذا rtl:
            hb.بفر_الٹ(buf)
        infos = hb.بفر_get_glyph_infos(buf)
        positions = hb.بفر_get_glyph_positions(buf)
        advance = جوڑ(p.x_advance لكل p في positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_منزل(0, -20)
        إذا variation:
            ٹیگ_2 = hb.ٹیگ_منزل_string(variation.ٹیگ_2).decode("ascii")
            context.show_متن(f" {tag}={variation.value:g}")
        context.move_منزل(0, 0)
        context.show_متن(f" {advance:g}/{target_advance:g}")
        context.restore()

        إذا variation:
            hb.font_سیٹ_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        إذا rtl:
            context.translate(نشانہ_advance, 0)

        لكل info, pos في zip(infos, positions):
            إذا rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_وتھ_2, pos.y_وتھ_2)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            إذا ليس rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


تعریف مکھ(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    نال open(textpath) كـ f:
        متن = f.اقرأ()

    تعریف on_draw(da, context):
        alloc = da.get_ونڈ()
        POOL[id(context)] = context
        render(face, متن, context, alloc.چوڑائی, alloc.اچائی, fontsize)
        ہٹاؤ POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.کھڑکی()
    win.connect("destroy", Gtk.main_quit)
    win.سیٹ_default_ناپ(1000, 700)
    win.جوڑو(drawingarea)

    win.show_all()
    Gtk.مکھ()


إذا __name__ == "__main__":
    استورد argparse

    parser = argparse.دلیل_Parser(description="HarfBuzz justification demo.")
    parser.جوڑو_دلیل("fontfile", help="font file")
    parser.جوڑو_دلیل("textfile", help="text")
    دلیلاں = parser.parse_دلیلاں()
    مکھ(دلیلاں.fontfile, دلیلاں.textfile)
