#!/usr/bin/env python3
"""Draw the menu table from menuos/hw/driver/files/hwfile.cpp as an SVG tree.

    python3 tools/docs/render_tree.py docs/img/menu-tree.svg
"""
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = os.path.join(ROOT, "menuos/hw/driver/files/hwfile.cpp")

TYPES = {
    "T_FOLDER": ("folder", "FOLDER"),
    "T_DFOLDER": ("dfolder", "DFOLDER"),
    "T_SFOLDER": ("folder", "SFOLDER"),
    "T_APP": ("app", "APP"),
    "T_CONF": ("conf", "CONF"),
    "T_SCONF": ("conf", "SCONF"),
    "T_DCONF": ("conf", "DCONF"),
}


def load():
    src = open(SRC, encoding="utf-8").read()
    table = src[src.index("fileStruct["):]
    table = table[table.index("{") + 1:table.index("};")]
    files = []
    for m in re.finditer(r"(T_\w+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)", table):
        files.append({"type": m.group(1), "parent": int(m.group(2)),
                      "mode1": int(m.group(3)), "mode2": int(m.group(4))})
    names = dict((int(i), n) for i, n in re.findall(r'file_(\d+)\[\]\s*MENUOS_ROM\s*=\s*"([^"]*)"', src))
    limits = re.search(r"configsLimit\[\]\s*MENUOS_ROM\s*=\s*\{(.*?)\};", src, re.S).group(1)
    lim = [int(x) for x in re.findall(r"-?\d+", re.sub(r"//[^\n]*", "", limits))]
    for i, f in enumerate(files):
        f["id"] = i
        f["name"] = names.get(i, f"file {i}")
    return files, lim


def walk(files, fid, depth, out, note=""):
    f = files[fid]
    out.append((depth, f, note))
    if f["type"] == "T_FOLDER":
        for c in range(f["mode1"], f["mode1"] + f["mode2"]):
            walk(files, c, depth + 1, out)
    elif f["type"] == "T_DFOLDER":
        walk(files, f["mode1"], depth + 1, out, note=f"× {f['mode2']} copies")


def detail(f, lim):
    t = f["type"]
    if t == "T_FOLDER":
        return f"children {f['mode1']}..{f['mode1'] + f['mode2'] - 1}"
    if t == "T_DFOLDER":
        return f"repeats file {f['mode1']} × {f['mode2']}"
    if t == "T_APP":
        known = {1: "no handler", 2: "sample app", 3: "clock, AVR + RTC only"}
        tail = known.get(f["mode1"], "")
        return f"app #{f['mode1']}" + (f" · {tail}" if tail else "") + (" (background)" if f["mode2"] == 0x1F else "")
    if t == "T_CONF":
        c = f["mode1"] * 3
        addr, lo, hi = lim[c], lim[c + 1], lim[c + 2]
        rng = "Yes / No" if hi == 0 else f"{lo} … {hi}"
        return f"config #{f['mode1']}: {rng} @ {addr}"
    return ""


CSS = """
  <style>
    .bg { fill: #ffffff; stroke: #d0d7de; }
    .name { font: 600 15px -apple-system, 'Segoe UI', Helvetica, Arial, sans-serif; fill: #1f2328; }
    .det { font: 13px 'SFMono-Regular', Menlo, Consolas, monospace; fill: #59636e; }
    .id { font: 12px 'SFMono-Regular', Menlo, Consolas, monospace; fill: #818b98; }
    .note { font: italic 13px -apple-system, 'Segoe UI', Helvetica, Arial, sans-serif; fill: #8250df; }
    .wire { stroke: #c4ccd5; stroke-width: 1.5; fill: none; }
    .badget { font: 700 10px 'SFMono-Regular', Menlo, Consolas, monospace; fill: #ffffff; letter-spacing: .5px; }
    .folder { fill: #bf8700; } .dfolder { fill: #8250df; } .app { fill: #0969da; } .conf { fill: #1a7f37; }
    .hdr { font: 600 13px -apple-system, 'Segoe UI', Helvetica, Arial, sans-serif; fill: #59636e; }
    @media (prefers-color-scheme: dark) {
      .bg { fill: #0d1117; stroke: #30363d; }
      .name { fill: #e6edf3; }
      .det { fill: #9198a1; }
      .id { fill: #6e7681; }
      .note { fill: #ab7df8; }
      .wire { stroke: #3d444d; }
      .folder { fill: #9e6a03; } .dfolder { fill: #8957e5; } .app { fill: #1f6feb; } .conf { fill: #238636; }
      .hdr { fill: #9198a1; }
    }
  </style>"""


def main():
    out_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(ROOT, "docs/img/menu-tree.svg")
    files, lim = load()
    rows = []
    walk(files, 0, 0, rows)
    RH, TOP, INDENT, X0 = 36, 52, 34, 70
    W = 820
    H = TOP + RH * len(rows) + 16
    p = [f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {W} {H}" width="{W}" height="{H}" '
         f'role="img" aria-label="Default MenuOS menu tree">', CSS,
         f'<rect class="bg" x="1" y="1" width="{W - 2}" height="{H - 2}" rx="12"/>',
         f'<text class="hdr" x="24" y="32">id</text>',
         f'<text class="hdr" x="{X0}" y="32">menu tree (from hwfile.cpp)</text>',
         f'<text class="hdr" x="{W - 24}" y="32" text-anchor="end">meaning of mode1 / mode2</text>']
    # wires: for every row connect to its parent row
    pos = {}
    for i, (d, f, note) in enumerate(rows):
        y = TOP + i * RH + RH / 2
        x = X0 + d * INDENT
        pos[(f["id"], d)] = (x, y)
    for i, (d, f, note) in enumerate(rows):
        if d == 0:
            continue
        x, y = X0 + d * INDENT, TOP + i * RH + RH / 2
        # find the closest row above with depth d-1
        for j in range(i - 1, -1, -1):
            if rows[j][0] == d - 1:
                px = X0 + (d - 1) * INDENT + 10
                py = TOP + j * RH + RH / 2 + 10
                p.append(f'<path class="wire" d="M{px} {py}V{y}H{x - 4}"/>')
                break
    for i, (d, f, note) in enumerate(rows):
        y = TOP + i * RH + RH / 2
        x = X0 + d * INDENT
        cls, label = TYPES.get(f["type"], ("app", f["type"]))
        bw = 12 + 7.2 * len(label)
        p.append(f'<text class="id" x="24" y="{y + 4}">{f["id"]:>2}</text>')
        p.append(f'<rect class="{cls}" x="{x}" y="{y - 10}" width="{bw:.0f}" height="20" rx="5"/>')
        p.append(f'<text class="badget" x="{x + bw / 2:.0f}" y="{y + 4}" text-anchor="middle">{label}</text>')
        p.append(f'<text class="name" x="{x + bw + 10:.0f}" y="{y + 5}">{f["name"]}</text>')
        if note:
            nx = x + bw + 18 + 8.4 * len(f["name"])
            p.append(f'<text class="note" x="{nx:.0f}" y="{y + 5}">{note}</text>')
        p.append(f'<text class="det" x="{W - 24}" y="{y + 5}" text-anchor="end">{detail(f, lim)}</text>')
    p.append("</svg>")
    open(out_path, "w").write("\n".join(p) + "\n")
    print("written:", out_path)


if __name__ == "__main__":
    main()
