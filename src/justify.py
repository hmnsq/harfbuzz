#!/usr/bin/env python3

استورد gi

gi.require_نسخه("Gtk", "3.0")
من gi.repository استورد Gtk, HarfBuzz كـ hb


POOL = {}


تعریف move_منزل_f(funcs, draw_معلومات, st, منزل_x, منزل_y, user_معلومات):
    context = POOL[draw_معلومات]
    context.move_منزل(منزل_x, منزل_y)


تعریف کرښه_منزل_f(funcs, draw_معلومات, st, منزل_x, منزل_y, user_معلومات):
    context = POOL[draw_معلومات]
    context.کرښه_منزل(منزل_x, منزل_y)


تعریف cubic_منزل_f(
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
    context.منحني_منزل(control1_x, control1_y, control2_x, control2_y, منزل_x, منزل_y)


تعریف close_لار_f(funcs, draw_معلومات, st, user_معلومات):
    context = POOL[draw_معلومات]
    context.close_لار()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_سېټ_move_منزل_func(DFUNCS, move_منزل_f, هیڅ)
hb.draw_funcs_سېټ_کرښه_منزل_func(DFUNCS, کرښه_منزل_f, هیڅ)
hb.draw_funcs_سېټ_cubic_منزل_func(DFUNCS, cubic_منزل_f, هیڅ)
hb.draw_funcs_سېټ_close_لار_func(DFUNCS, close_لار_f, هیڅ)


تعریف ټېل_transform_f(funcs, paint_معلومات, xx, yx, xy, yy, dx, dy, user_معلومات):
    ارفع NotImplementedError


تعریف راوباسه_transform_f(funcs, paint_معلومات, user_معلومات):
    ارفع NotImplementedError


تعریف color_f(funcs, paint_معلومات, is_foreground, color, user_معلومات):
    context = POOL[paint_معلومات]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.سېټ_سرچینه_rgba(r, g, b, a)
    context.paint()


تعریف ټېل_clip_rectangle_f(funcs, paint_معلومات, xmin, ymin, xmax, ymax, user_معلومات):
    context = POOL[paint_معلومات]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


تعریف ټېل_clip_glyph_f(funcs, paint_معلومات, glyph, font, user_معلومات):
    context = POOL[paint_معلومات]
    context.save()
    context.new_لار()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_معلومات)
    context.close_لار()
    context.clip()


تعریف راوباسه_clip_f(funcs, paint_معلومات, user_معلومات):
    context = POOL[paint_معلومات]
    context.restore()


تعریف ټېل_ډله_f(funcs, paint_معلومات, user_معلومات):
    ارفع NotImplementedError


تعریف راوباسه_ډله_f(funcs, paint_معلومات, ډېرپېښ, user_معلومات):
    ارفع NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_سېټ_ټېل_transform_func(PFUNCS, ټېل_transform_f, هیڅ)
hb.paint_funcs_سېټ_راوباسه_transform_func(PFUNCS, راوباسه_transform_f, هیڅ)
hb.paint_funcs_سېټ_color_func(PFUNCS, color_f, هیڅ)
hb.paint_funcs_سېټ_ټېل_clip_glyph_func(PFUNCS, ټېل_clip_glyph_f, هیڅ)
hb.paint_funcs_سېټ_ټېل_clip_rectangle_func(PFUNCS, ټېل_clip_rectangle_f, هیڅ)
hb.paint_funcs_سېټ_راوباسه_clip_func(PFUNCS, راوباسه_clip_f, هیڅ)
hb.paint_funcs_سېټ_ټېل_ډله_func(PFUNCS, ټېل_ډله_f, هیڅ)
hb.paint_funcs_سېټ_راوباسه_ډله_func(PFUNCS, راوباسه_ډله_f, هیڅ)


تعریف makebuffer(ټکي):
    buf = hb.بفر_create()

    متن = " ".join(ټکي)
    hb.بفر_جمع_کړه_codepoints(buf, [ord(c) لكل c في متن], 0, اوږدوالی(متن))

    hb.بفر_guess_segment_properties(buf)

    أرجع buf


تعریف justify(face, ټکي, advance, موخه_advance):
    font = hb.font_create(face)
    buf = makebuffer(ټکي)

    wiggle = 5
    shrink = موخه_advance - wiggle < advance
    expand = موخه_advance + wiggle > advance

    ret, advance, نښان_2, ارزښت_2 = hb.shape_justify(
        font,
        buf,
        هیڅ,
        هیڅ,
        موخه_advance,
        موخه_advance,
        advance,
    )

    إذا ليس ret:
        أرجع خطأ, buf, هیڅ

    إذا نښان_2:
        variation = hb.variation_t()
        variation.نښان_2 = نښان_2
        variation.ارزښت_2 = ارزښت_2
    وإلا:
        variation = هیڅ

    إذا shrink و advance > موخه_advance + wiggle:
        أرجع خطأ, buf, variation
    إذا expand و advance < موخه_advance - wiggle:
        أرجع خطأ, buf, variation

    أرجع ریښتیا, buf, variation


تعریف shape(face, ټکي):
    font = hb.font_create(face)
    buf = makebuffer(ټکي)
    hb.shape(font, buf)
    positions = hb.بفر_get_glyph_positions(buf)
    advance = مجموعه(p.x_advance لكل p في positions)
    أرجع buf, advance


تعریف typeset(face, متن, موخه_advance):
    کرښې = []
    ټکي = []
    لكل کلمه_2 في متن.وېشل():
        ټکي.append(کلمه_2)
        buf, advance = shape(face, ټکي)
        إذا advance > موخه_advance:
            # Shrink
            ret, buf, variation = justify(face, ټکي, advance, موخه_advance)
            إذا ret:
                کرښې.append((buf, variation))
                ټکي = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            وإلاإذا اوږدوالی(ټکي) > 1:
                ټکي.راوباسه()
                _, buf, variation = justify(face, ټکي, advance, موخه_advance)
                کرښې.append((buf, variation))
                ټکي = [کلمه_2]
            # But if it is one word, meh.
            وإلا:
                کرښې.append((buf, variation))
                ټکي = []

    # Justify last line
    إذا ټکي:
        _, buf, variation = justify(face, ټکي, advance, موخه_advance)
        کرښې.append((buf, variation))

    أرجع کرښې


تعریف render(face, متن, context, پلنوالی, لوړوالی, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    موخه_advance = (پلنوالی - (margin * 2)) / scale

    کرښې = typeset(face, متن, موخه_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.کرښه_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.سېټ_font_اندازه(12)
    context.سېټ_سرچینه_rgb(1, 0, 0)
    لكل buf, variation في کرښې:
        rtl = hb.بفر_get_direction(buf) == hb.direction_t.RTL
        إذا rtl:
            hb.بفر_سرچپه(buf)
        infos = hb.بفر_get_glyph_infos(buf)
        positions = hb.بفر_get_glyph_positions(buf)
        advance = مجموعه(p.x_advance لكل p في positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_منزل(0, -20)
        إذا variation:
            نښان_2 = hb.نښان_منزل_string(variation.نښان_2).decode("ascii")
            context.show_متن(f" {tag}={variation.value:g}")
        context.move_منزل(0, 0)
        context.show_متن(f" {advance:g}/{target_advance:g}")
        context.restore()

        إذا variation:
            hb.font_سېټ_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        إذا rtl:
            context.translate(موخه_advance, 0)

        لكل info, pos في zip(infos, positions):
            إذا rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_دننه_واټن_2, pos.y_دننه_واټن_2)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            إذا ليس rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


تعریف اصلي(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    سره open(textpath) كـ f:
        متن = f.اقرأ()

    تعریف on_draw(da, context):
        alloc = da.get_ویش()
        POOL[id(context)] = context
        render(face, متن, context, alloc.پلنوالی, alloc.لوړوالی, fontsize)
        لرېکړه POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.کړکۍ()
    win.connect("destroy", Gtk.main_quit)
    win.سېټ_default_اندازه(1000, 700)
    win.جمع_کړه(drawingarea)

    win.show_all()
    Gtk.اصلي()


إذا __name__ == "__main__":
    استورد argparse

    parser = argparse.دلیل_Parser(description="HarfBuzz justification demo.")
    parser.جمع_کړه_دلیل("fontfile", help="font file")
    parser.جمع_کړه_دلیل("textfile", help="text")
    دلیلونه = parser.parse_دلیلونه()
    اصلي(دلیلونه.fontfile, دلیلونه.textfile)
