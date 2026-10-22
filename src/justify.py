#!/usr/bin/env python3

இறக்குமதி gi

gi.require_version("Gtk", "3.0")
தஃ gi.repository இறக்குமதி Gtk, HarfBuzz இவ hb


POOL = {}


def move_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.move_to(to_x, to_y)


def வரி_to_f(funcs, draw_data, st, to_x, to_y, user_data):
    context = POOL[draw_data]
    context.வரி_to(to_x, to_y)


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
hb.draw_funcs_set_வரி_to_func(DFUNCS, வரி_to_f, None)
hb.draw_funcs_set_cubic_to_func(DFUNCS, cubic_to_f, None)
hb.draw_funcs_set_close_path_func(DFUNCS, close_path_f, None)


def தள்ளு_transform_f(funcs, paint_data, xx, yx, xy, yy, dx, dy, user_data):
    எழுப்பு NotImplementedError


def எடு_transform_f(funcs, paint_data, user_data):
    எழுப்பு NotImplementedError


def color_f(funcs, paint_data, is_foreground, color, user_data):
    context = POOL[paint_data]
    r = hb.color_get_red(color) / 255
    g = hb.color_get_green(color) / 255
    b = hb.color_get_blue(color) / 255
    a = hb.color_get_alpha(color) / 255
    context.set_source_rgba(r, g, b, a)
    context.paint()


def தள்ளு_clip_rectangle_f(funcs, paint_data, xmin, ymin, xmax, ymax, user_data):
    context = POOL[paint_data]
    context.save()
    context.rectangle(xmin, ymin, xmax, ymax)
    context.clip()


def தள்ளு_clip_glyph_f(funcs, paint_data, glyph, font, user_data):
    context = POOL[paint_data]
    context.save()
    context.new_path()
    hb.font_draw_glyph(font, glyph, DFUNCS, paint_data)
    context.close_path()
    context.clip()


def எடு_clip_f(funcs, paint_data, user_data):
    context = POOL[paint_data]
    context.restore()


def தள்ளு_group_f(funcs, paint_data, user_data):
    எழுப்பு NotImplementedError


def எடு_group_f(funcs, paint_data, mode, user_data):
    எழுப்பு NotImplementedError


PFUNCS = hb.paint_funcs_create()
hb.paint_funcs_set_தள்ளு_transform_func(PFUNCS, தள்ளு_transform_f, None)
hb.paint_funcs_set_எடு_transform_func(PFUNCS, எடு_transform_f, None)
hb.paint_funcs_set_color_func(PFUNCS, color_f, None)
hb.paint_funcs_set_தள்ளு_clip_glyph_func(PFUNCS, தள்ளு_clip_glyph_f, None)
hb.paint_funcs_set_தள்ளு_clip_rectangle_func(PFUNCS, தள்ளு_clip_rectangle_f, None)
hb.paint_funcs_set_எடு_clip_func(PFUNCS, எடு_clip_f, None)
hb.paint_funcs_set_தள்ளு_group_func(PFUNCS, தள்ளு_group_f, None)
hb.paint_funcs_set_எடு_group_func(PFUNCS, எடு_group_f, None)


def makebuffer(சொற்கள்):
    buf = hb.buffer_create()

    text = " ".join(சொற்கள்)
    hb.buffer_add_codepoints(buf, [ord(c) க்கு c உள் text], 0, len(text))

    hb.buffer_guess_segment_properties(buf)

    திரும்பு buf


def justify(face, சொற்கள், advance, target_advance):
    font = hb.font_create(face)
    buf = makebuffer(சொற்கள்)

    wiggle = 5
    shrink = target_advance - wiggle < advance
    expand = target_advance + wiggle > advance

    ret, advance, tag, மதிப்பு_2 = hb.shape_justify(
        font,
        buf,
        None,
        None,
        target_advance,
        target_advance,
        advance,
    )

    எனில் இல்லை ret:
        திரும்பு பொய், buf, None

    எனில் tag:
        variation = hb.variation_t()
        variation.tag = tag
        variation.மதிப்பு_2 = மதிப்பு_2
    இல்லையெனில்:
        variation = None

    எனில் shrink மற்றும் advance > target_advance + wiggle:
        திரும்பு பொய், buf, variation
    எனில் expand மற்றும் advance < target_advance - wiggle:
        திரும்பு பொய், buf, variation

    திரும்பு உண்மை, buf, variation


def shape(face, சொற்கள்):
    font = hb.font_create(face)
    buf = makebuffer(சொற்கள்)
    hb.shape(font, buf)
    positions = hb.buffer_get_glyph_positions(buf)
    advance = sum(p.x_advance க்கு p உள் positions)
    திரும்பு buf, advance


def typeset(face, text, target_advance):
    வரிகள் = []
    சொற்கள் = []
    க்கு word உள் text.split():
        சொற்கள்.append(word)
        buf, advance = shape(face, சொற்கள்)
        எனில் advance > target_advance:
            # Shrink
            ret, buf, variation = justify(face, சொற்கள், advance, target_advance)
            எனில் ret:
                வரிகள்.append((buf, variation))
                சொற்கள் = []
            # If if fails, pop the last word and shrink, and hope for the best.
            # A too short line is better than too long.
            அல்லதெனில் len(சொற்கள்) > 1:
                சொற்கள்.எடு()
                _, buf, variation = justify(face, சொற்கள், advance, target_advance)
                வரிகள்.append((buf, variation))
                சொற்கள் = [word]
            # But if it is one word, meh.
            இல்லையெனில்:
                வரிகள்.append((buf, variation))
                சொற்கள் = []

    # Justify last line
    எனில் சொற்கள்:
        _, buf, variation = justify(face, சொற்கள், advance, target_advance)
        வரிகள்.append((buf, variation))

    திரும்பு வரிகள்


def render(face, text, context, width, height, fontsize):
    font = hb.font_create(face)

    margin = fontsize * 2
    scale = fontsize / hb.face_get_upem(face)
    target_advance = (width - (margin * 2)) / scale

    வரிகள் = typeset(face, text, target_advance)

    _, extents = hb.font_get_h_extents(font)
    lineheight = extents.ascender - extents.descender + extents.வரி_gap
    lineheight *= scale

    context.save()
    context.translate(0, margin)
    context.set_font_size(12)
    context.set_source_rgb(1, 0, 0)
    க்கு buf, variation உள் வரிகள்:
        rtl = hb.buffer_get_direction(buf) == hb.direction_t.RTL
        எனில் rtl:
            hb.buffer_reverse(buf)
        infos = hb.buffer_get_glyph_infos(buf)
        positions = hb.buffer_get_glyph_positions(buf)
        advance = sum(p.x_advance க்கு p உள் positions)

        context.translate(0, lineheight)
        context.save()

        context.save()
        context.move_to(0, -20)
        எனில் variation:
            tag = hb.tag_to_string(variation.tag).decode("ascii")
            context.show_text(f" {tag}={variation.value:g}")
        context.move_to(0, 0)
        context.show_text(f" {advance:g}/{target_advance:g}")
        context.restore()

        எனில் variation:
            hb.font_set_variations(font, [variation])

        context.translate(margin, 0)
        context.scale(scale, -scale)

        எனில் rtl:
            context.translate(target_advance, 0)

        க்கு info, pos உள் zip(infos, positions):
            எனில் rtl:
                context.translate(-pos.x_advance, pos.y_advance)
            context.save()
            context.translate(pos.x_offset, pos.y_offset)
            hb.font_paint_glyph(font, info.codepoint, PFUNCS, id(context), 0, 0x0000FF)
            context.restore()
            எனில் இல்லை rtl:
                context.translate(+pos.x_advance, pos.y_advance)

        context.restore()
    context.restore()


def முதன்மை(fontpath, textpath):
    fontsize = 70

    blob = hb.blob_create_from_file(fontpath)
    face = hb.face_create(blob, 0)

    with open(textpath) இவ f:
        text = f.பட()

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
    Gtk.முதன்மை()


எனில் __name__ == "__main__":
    இறக்குமதி argparse

    parser = argparse.ArgumentParser(description="HarfBuzz justification demo.")
    parser.add_argument("fontfile", help="font file")
    parser.add_argument("textfile", help="text")
    args = parser.parse_args()
    முதன்மை(args.fontfile, args.textfile)
