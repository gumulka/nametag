#!/usr/bin/env python3

import sys
import logging
from pathlib import Path

from PIL import Image

def usage(name):
    print("usage:")
    print(f"  {name} path_to_image.jpg")


def convert(imagename: Path):
    im = Image.open(imagename)

    basename = imagename.stem.replace(" ", "_")
    grey_image = im.convert("L")
    black = grey_image.point(
        lambda x: 255 if (x > 0 and x < 10) else 0
    ).convert("1")
    dark_grey = grey_image.point(
        lambda x: 255 if (x > 10 and x < 100) else 0
    ).convert("1")
    light_grey = grey_image.point(
        lambda x: 255 if (x > 100 and x < 200) else 0
    ).convert("1")
    print(black.tobitmap(basename + "_black").decode("ascii"))
    print(dark_grey.tobitmap(basename + "_dark_grey").decode("ascii"))
    print(light_grey.tobitmap(basename + "_light_grey").decode("ascii"))


if __name__ == "__main__":
    if len(sys.argv) != 2:
        usage(sys.argv[0])
    convert(Path(sys.argv[1]))
