#!/usr/bin/env python3

ಆನಯನ gi

gi.require_version("Gtk", "3.0")
ತಃ gi.repository ಆನಯನ Gtk, HarfBuzz ಇವ hb


POOL = {}


def move_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.move_to(to_x, to_y)


def ಸಾಲು_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.ಸಾಲು_to(to_x, to_y)


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
hb.draw_funcs_set_ಸಾಲು_to_func(DFUNCS, ಸಾಲು_to_f, None)
hb.draw_funcs_set_cubic_to_func(DFUNCS, cubic_to_f, None)
hb.draw_funcs_set_close_path_func(DFUNCS, close_path_f, None)


def ತಳ್ಳು_transform_f(funcs, paint_data, xx, yx, xy, yy, dx, dy, user_data):
    ಉತ್ಥಾಪಯ NotImplementedError


def ತೆಗೆ_transform_f(funcs, paint_data, user_data):
    ಉತ್ಥಾಪಯ NotImplementedError


def color_f(funcs, paint_data, is_foreground, color, user_data):
    context = POOL[paint_data]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.set_source_rgba(r, g, b, a)
    context.paint()


def ತಳ್ಳು_clip_rectangle_f(funcs, paint_data, xmin, ymin, xmax, ymax, user_data):
    context = POOL[paint_data]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


def ತಳ್ಳು_clip_glyph_f(funcs, paint_data, glyph, font, user_data):
    context = POOL[paint_data]
    context.save()
    context.new_path()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_data)
    context.close_path()
    context.clip()


def ತೆಗೆ_clip_f(funcs, paint_data, user_data):
    context = POOL[paint_data]
    context.restore()


def ತಳ್ಳು_group_f(funcs, paint_data, user_data):
    ಉತ್ಥಾಪಯ NotImplementedError


def ತೆಗೆ_group_f(funcs, paint_data, mode, user_data):
    ಉತ್ಥಾಪಯ NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_set_ತಳ್ಳು_transform_func(PFUNCS, ತಳ್ಳು_transform_f, None)
hb.paint_funcs_set_ತೆಗೆ_transform_func(PFUNCS, ತೆಗೆ_transform_f, None)
hb.paint_funcs_set_color_func(PFUNCS, color_f, None)
hb.paint_funcs_set_ತಳ್ಳು_clip_glyph_func(PFUNCS, ತಳ್ಳು_clip_glyph_f, None)
hb.paint_funcs_set_ತಳ್ಳು_clip_rectangle_func(PFUNCS, ತಳ್ಳು_clip_rectangle_f, None)
hb.paint_funcs_set_ತೆಗೆ_clip_func(PFUNCS, ತೆಗೆ_clip_f, None)
hb.paint_funcs_set_ತಳ್ಳು_group_func(PFUNCS, ತಳ್ಳು_group_f, None)
hb.paint_funcs_set_ತೆಗೆ_group_func(PFUNCS, ತೆಗೆ_group_f, None)


def makebuffer(ಪದಗಳು):
    buf = hb.buffer_create()

    text = " ".join(ಪದಗಳು)
    hb.buffer_add_codepoints(buf, [ord(c) ಪ್ರತಿ c ಅನ್ತಃ text], 0, len(text))

    hb.buffer_guess_segment_properties(buf)

    ಪ್ರತ್ಯಾಗಮ buf


def justify(face, ಪದಗಳು, advance, target_advance):
    font = hb.font_create(face)
    buf = makebuffer(ಪದಗಳು)

    wiggle = 5
    shrink = target_advance - wiggle < advance
    expand = target_advance + wiggle > advance

    ret, advance, tag, ಮೌಲ್ಯ_2 = hb.shape_justify(
        font,
        buf,
        None,
        None,
        target_advance,
        target_advance,
        advance,
    )

    ಯದಿ ನ ret:
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ, buf, None

    ಯದಿ tag:
        variation = hb.variation_t()
        variation.tag = tag
        variation.ಮೌಲ್ಯ_2 = ಮೌಲ್ಯ_2
    ಅನ್ಯಥಾ:
        variation = None

    ಯದಿ shrink ಚ advance > target_advance + wiggle:
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ, buf, variation
    ಯದಿ expand ಚ advance < target_advance - wiggle:
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ, buf, variation

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ, buf, variation


def shape(face, ಪದಗಳು):
    font = hb.font_create(face)
    buf = makebuffer(ಪದಗಳು)
    hb.shape(font, buf)
    positions = hb.buffer_get_glyph_positions(buf)
    advance = sum(p.x_advance ಪ್ರತಿ p ಅನ್ತಃ positions)
    ಪ್ರತ್ಯಾಗಮ buf, advance


def typeset(face, text, target_advance):
    ಸಾಲುಗಳು = []
    ಪದಗಳು = []
    ಪ್ರತಿ word ಅನ್ತಃ text.split():
        ಪದಗಳು.append(word)
        buf, advance = shape(face, ಪದಗಳು)
        ಯದಿ advance > target_advance:
            # Shrink
            ret, buf, variation = justify(face, ಪದಗಳು, advance, target_advance)
            ಯದಿ ret:
                ಸಾಲುಗಳು.append((buf, variation))
                ಪದಗಳು = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            ನೋಚೇದ್ಯದಿ len(ಪದಗಳು) > 1:
                ಪದಗಳು.ತೆಗೆ()
                _, buf, variation = justify(face, ಪದಗಳು, advance, target_advance)
                ಸಾಲುಗಳು.append((buf, variation))
                ಪದಗಳು = [word]
            # But if it is one word, meh.
            ಅನ್ಯಥಾ:
                ಸಾಲುಗಳು.append((buf, variation))
                ಪದಗಳು = []

    # Justify last line
    ಯದಿ ಪದಗಳು:
        _, buf, variation = justify(face, ಪದಗಳು, advance, target_advance)
        ಸಾಲುಗಳು.append((buf, variation))

    ಪ್ರತ್ಯಾಗಮ ಸಾಲುಗಳು


def render(face, text, context, width, height, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    target_advance = (width - (margin * 2)) / scale

    ಸಾಲುಗಳು = typeset(face, text, target_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.ಸಾಲು_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.set_font_size(12)
    context.set_source_rgb(1, 0, 0)
    ಪ್ರತಿ buf, variation ಅನ್ತಃ ಸಾಲುಗಳು:
        rtl = hb.buffer_get_direction(buf) == hb.direction_t.RTL
        ಯದಿ rtl:
            hb.buffer_reverse(buf)
        infos = hb.buffer_get_glyph_infos(buf)
        positions = hb.buffer_get_glyph_positions(buf)
        advance = sum(p.x_advance ಪ್ರತಿ p ಅನ್ತಃ positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_to(0, -20)
        ಯದಿ variation:
            tag = hb.tag_to_string(variation.tag).decode("ascii")
            context.show_text(f" {tag}={variation.value:g}")
        context.move_to(0, 0)
        context.show_text(f" {advance:g}/{target_advance:g}")
        context.restore()

        ಯದಿ variation:
            hb.font_set_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        ಯದಿ rtl:
            context.translate(target_advance, 0)

        ಪ್ರತಿ info, pos ಅನ್ತಃ zip(infos, positions):
            ಯದಿ rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_offset, pos.y_offset)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            ಯದಿ ನ rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


def main(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    with open(textpath) ಇವ f:
        text = f.ಪಠ()

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


ಯದಿ __name__ == "__main__":
    ಆನಯನ argparse

    parser = argparse.ArgumentParser(description="HarfBuzz justification demo.")
    parser.add_argument("fontfile", help="font file")
    parser.add_argument("textfile", help="text")
    args = parser.parse_args()
    main(args.fontfile, args.textfile)
