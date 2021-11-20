from os import pathconf
from typing import Tuple, List
from PIL import Image, ImageDraw, ImageFont
from PIL import Image
import os


def convert_to_pdf():
    img = Image.open('0.png')
    imgs = []
    for file in os.listdir():
        if file[-3:] == 'png':
            f = Image.open(file)
            imgs.append(f)
            os.remove(file)
    img.save('./new.pdf', save_all=True, append_images=imgs[1:])


def parse_file(file) -> List:
    contents = ''
    with open(file, 'r') as f:
        contents = f.read().split('\n\n')
    # clean up empty and newline characters
    contents = list(
        filter(lambda line: True if line != '' else False, contents))
    contents = list(map(lambda line: line.split('\n'), contents))
    tmp_list = []
    # ignore comments
    for page in contents:
        tmp_list.append(list(
            filter(lambda line: False if line != '' and line[0] == '#' else True, page)))
    # strip away empty strings from list
    list_of_pages = []
    for line in tmp_list:
        list_of_pages.append(
            list(filter(lambda line: False if line == '' else True, line)))
    list_of_pages = list(
        filter(lambda lines: False if lines == [] else True, list_of_pages))

    return list_of_pages


class SheetWriter:
    """
    Writes pdf document sheet
    """

    def __init__(self, file: List[List[str]], dimension: Tuple[int, int] = (1280, 720), bg_color: Tuple[float, ...] = (0, 0, 0), font: Tuple[str] = 'sans-serif'):
        self.dimension = dimension
        self.bg_color = bg_color
        self.file = file
        self.font = ImageFont.truetype(font, size=60)

    def _draw_text_page(self, name: str, text: str):
        img = Image.new('RGB', self.dimension, self.bg_color)
        page = ImageDraw.Draw(img)
        page.text((self.dimension[0]/2, self.dimension[1]/2.2), text,
                  font=self.font, anchor='mm', align='left', )
        img.save(name)

    def _draw_image_page(self, name: str, img):
        """
        Center and generate image page
        """
        img2 = Image.open(img)
        img2_w, img2_h = img2.size
        img = Image.new('RGB', self.dimension, self.bg_color)
        img_w, img_h = img.size
        offset = ((img_w - img2_w) // 2, (img_h - img2_h) // 2)
        img2.thumbnail(self.dimension)
        img.paste(img2, offset)
        img.save(name)

    def create_sheet(self):
        page_number = 0
        newp = ''
        for page in self.file:
            for line in page:
                if line[0] == '@':
                    self._draw_image_page(f'{page_number}.png', line[1:])
                    page_number += 1
                    continue
                newp += '\n' + line
            self._draw_text_page(f'{page_number}.png', newp)
            newp = ''
            page_number += 1
