#!/usr/bin/env python3

आनयन gi

gi.require_version("Gtk", "3.0")
तः gi.repository आनयन Gtk, HarfBuzz इव hb


POOL = {}


def move_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.move_to(to_x, to_y)


def पङ्क्ति_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.पङ्क्ति_to(to_x, to_y)


def cubic_to_f(
    funcs,
    draw_data,
    st,
    control1_x,
    control1_y,
    control2_x,
    control2_y,
    to_x,
    to_y,
    user_data,
):
    context = POOL[draw_data]
    context.curve_to(control1_x, control1_y, control2_x, control2_y, to_x, to_y)


def close_path_f(funcs, draw_data, st, user_data):
    context = POOL[draw_data]
    context.close_path()


DFUNCS = hb.draw_funcs_create()
hb.draw_funcs_set_move_to_func(DFUNCS, move_to_f, None)
hb.draw_funcs_set_पङ्क्ति_to_func(DFUNCS, पङ्क्ति_to_f, None)
hb.draw_funcs_set_cubic_to_func(DFUNCS, cubic_to_f, None)
hb.draw_funcs_set_close_path_func(DFUNCS, close_path_f, None)


def थप_transform_f(funcs, paint_data, xx, yx, xy, yy, dx, dy, user_data):
    उत्थापय NotImplementedError


def झिक_transform_f(funcs, paint_data, user_data):
    उत्थापय NotImplementedError


def color_f(funcs, paint_data, is_foreground, color, user_data):
    context = POOL[paint_data]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.set_source_rgba(r, g, b, a)
    context.paint()


def थप_clip_rectangle_f(funcs, paint_data, xmin, ymin, xmax, ymax, user_data):
    context = POOL[paint_data]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


def थप_clip_glyph_f(funcs, paint_data, glyph, font, user_data):
    context = POOL[paint_data]
    context.save()
    context.new_path()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_data)
    context.close_path()
    context.clip()


def झिक_clip_f(funcs, paint_data, user_data):
    context = POOL[paint_data]
    context.restore()


def थप_group_f(funcs, paint_data, user_data):
    उत्थापय NotImplementedError


def झिक_group_f(funcs, paint_data, mode, user_data):
    उत्थापय NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_set_थप_transform_func(PFUNCS, थप_transform_f, None)
hb.paint_funcs_set_झिक_transform_func(PFUNCS, झिक_transform_f, None)
hb.paint_funcs_set_color_func(PFUNCS, color_f, None)
hb.paint_funcs_set_थप_clip_glyph_func(PFUNCS, थप_clip_glyph_f, None)
hb.paint_funcs_set_थप_clip_rectangle_func(PFUNCS, थप_clip_rectangle_f, None)
hb.paint_funcs_set_झिक_clip_func(PFUNCS, झिक_clip_f, None)
hb.paint_funcs_set_थप_group_func(PFUNCS, थप_group_f, None)
hb.paint_funcs_set_झिक_group_func(PFUNCS, झिक_group_f, None)


def makebuffer(शब्दहरू):
    buf = hb.buffer_create()

    text = " ".join(शब्दहरू)
    hb.buffer_add_codepoints(buf, [ord(c) लागि c अन्तः text], 0, len(text))

    hb.buffer_guess_segment_properties(buf)

    फर्काऊ buf


def justify(face, शब्दहरू, advance, target_advance):
    font = hb.font_create(face)
    buf = makebuffer(शब्दहरू)

    wiggle = 5
    shrink = target_advance - wiggle < advance
    expand = target_advance + wiggle > advance

    ret, advance, tag, मान_2 = hb.shape_justify(
        font,
        buf,
        None,
        None,
        target_advance,
        target_advance,
        advance,
    )

    यदि होइन ret:
        फर्काऊ असत्य, buf, None

    यदि tag:
        variation = hb.variation_t()
        variation.tag = tag
        variation.मान_2 = मान_2
    अन्यथा:
        variation = None

    यदि shrink र advance > target_advance + wiggle:
        फर्काऊ असत्य, buf, variation
    यदि expand र advance < target_advance - wiggle:
        फर्काऊ असत्य, buf, variation

    फर्काऊ सत्य, buf, variation


def shape(face, शब्दहरू):
    font = hb.font_create(face)
    buf = makebuffer(शब्दहरू)
    hb.shape(font, buf)
    positions = hb.buffer_get_glyph_positions(buf)
    advance = sum(p.x_advance लागि p अन्तः positions)
    फर्काऊ buf, advance


def typeset(face, text, target_advance):
    पङ्क्तिहरू = []
    शब्दहरू = []
    लागि word अन्तः text.split():
        शब्दहरू.append(word)
        buf, advance = shape(face, शब्दहरू)
        यदि advance > target_advance:
            # Shrink
            ret, buf, variation = justify(face, शब्दहरू, advance, target_advance)
            यदि ret:
                पङ्क्तिहरू.append((buf, variation))
                शब्दहरू = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            अथवायदि len(शब्दहरू) > 1:
                शब्दहरू.झिक()
                _, buf, variation = justify(face, शब्दहरू, advance, target_advance)
                पङ्क्तिहरू.append((buf, variation))
                शब्दहरू = [word]
            # But if it is one word, meh.
            अन्यथा:
                पङ्क्तिहरू.append((buf, variation))
                शब्दहरू = []

    # Justify last line
    यदि शब्दहरू:
        _, buf, variation = justify(face, शब्दहरू, advance, target_advance)
        पङ्क्तिहरू.append((buf, variation))

    फर्काऊ पङ्क्तिहरू


def render(face, text, context, width, height, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    target_advance = (width - (margin * 2)) / scale

    पङ्क्तिहरू = typeset(face, text, target_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.पङ्क्ति_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.set_font_size(12)
    context.set_source_rgb(1, 0, 0)
    लागि buf, variation अन्तः पङ्क्तिहरू:
        rtl = hb.buffer_get_direction(buf) == hb.direction_t.RTL
        यदि rtl:
            hb.buffer_reverse(buf)
        infos = hb.buffer_get_glyph_infos(buf)
        positions = hb.buffer_get_glyph_positions(buf)
        advance = sum(p.x_advance लागि p अन्तः positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_to(0, -20)
        यदि variation:
            tag = hb.tag_to_string(variation.tag).decode("ascii")
            context.show_text(f" {tag}={variation.value:g}")
        context.move_to(0, 0)
        context.show_text(f" {advance:g}/{target_advance:g}")
        context.restore()

        यदि variation:
            hb.font_set_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        यदि rtl:
            context.translate(target_advance, 0)

        लागि info, pos अन्तः zip(infos, positions):
            यदि rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_offset, pos.y_offset)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            यदि होइन rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


def main(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    with open(textpath) इव f:
        text = f.पठ()

    def on_draw(da, context):
        alloc = da.get_allocation()
        POOL[id(context)] = context
        render(face, text, context, alloc.width, alloc.height, fontsize)
        del POOL[id(context)]

    drawingarea = Gtk.DrawingArea()
    drawingarea.connect("draw", on_draw)

    win = Gtk.Window()
    win.connect("destroy", Gtk.main_quit)
    win.set_default_size(1000, 700)
    win.add(drawingarea)

    win.show_all()
    Gtk.main()


यदि __name__ == "__main__":
    आनयन argparse

    parser = argparse.ArgumentParser(description="HarfBuzz justification demo.")
    parser.add_argument("fontfile", help="font file")
    parser.add_argument("textfile", help="text")
    args = parser.parse_args()
    main(args.fontfile, args.textfile)
