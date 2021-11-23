from typing import Tuple, List, final
from PIL import Image, ImageDraw, ImageFont
from sys import exit, argv
import os
import tempfile
import shutil


class SheetWriter:
    """
    Writes pdf document sheet
    """
    work_dir = tempfile.mkdtemp('neosent')

    def __init__(self, dimension: Tuple[int, int] = (1280, 720), bg_color: Tuple[float, ...] = (0, 0, 0), font: Tuple[str] = 'OpenSansEmoji.ttf'):
        self.dimension = dimension
        self.bg_color = bg_color
        self.font = ImageFont.truetype(font, size=60)

        if len(argv) == 1:
            print("Usage: neosent FILE")
        try:
            self.file = self.parse_file(argv[1])
        except IndexError:
            exit(1)
        except FileNotFoundError:
            print(f'No such file or directory: {argv[1]}')
            exit(2)

    @staticmethod
    def parse_file(file) -> List:
        contents = ''
        # open presentation file
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
        # remove empty lists
        list_of_pages = list(
            filter(lambda lines: False if lines == [] else True, list_of_pages))

        return list_of_pages

    @staticmethod
    def convert_to_pdf():
        """
        Convert png files to a single pdf file
        """
        img = Image.open('0.png')
        img.convert('RGB')
        imgs = []
        for file in os.listdir():
            # make sure that only png files are appended
            if file[-3:] == 'png':
                f = Image.open(file)
                f.convert('RGB')
                imgs.append(f)
                os.remove(file)
        # images are listed in decreased order in /tmp
        imgs.reverse()
        # strip away file extension from presentation file
        filename = argv[1][:-3] if argv[1][-2:] == 'ns' else argv[1]
        img.save(f'{filename}.pdf', save_all=True, append_images=imgs[1:])

    def _draw_text_page(self, name: str, text: str):
        img = Image.new('RGB', self.dimension, self.bg_color)
        page = ImageDraw.Draw(img)
        # centered text

        def wrap_text(text: str, width: int):
            """
            Limits text to a maximum character size per line
            Except for long strings, which will be left as is
            """
            final_text = ''
            count = 0
            for c in text:
                if count > width and c == ' ':
                    final_text += c + '\n'
                    count = 0
                    continue
                if c == '\n':
                    count = 0
                count += 1
                final_text += c
            return final_text

        text = wrap_text(text, 22)
        page.text((self.dimension[0]/2, (self.dimension[1])/2.2), text,
                  font=self.font, anchor='mm', align='left', )
        img.save(name)

    def _draw_image_page(self, name: str, img):
        """
        Center and generate image page
        """
        try:
            # get images' height and width
            img2 = Image.open(img)
            img2_w, img2_h = img2.size
            img = Image.new('RGB', self.dimension, self.bg_color)
            img_w, img_h = img.size

            # get offset needed to center image
            offset = ((img_w - img2_w) // 2, (img_h - img2_h) // 2)
            # make sure image fits in slide
            img2.resize(self.dimension)
            # put image on top of solid background
            img.paste(img2, offset)
            img.save(name)
        except FileNotFoundError:
            print(f"Error: {img} was not found.")
            exit(1)

    # class end point
    def create_sheet(self):
        page_number = 0
        newp = ''
        # iterates through a parsed presentation file
        for page in self.file:
            for line in page:
                # generate file path for each slide
                filename = f'{self.work_dir}/{page_number}'
                # draw image slide if line starts with @
                if line[0] == '@':
                    self._draw_image_page(f'{filename}.png', line[1:])
                    page_number += 1
                    continue
                # draw empty slide if line is \
                if line == '\\':
                    self._draw_text_page(f'{filename}.png', '')
                    page_number += 1
                    continue
                newp += '\n' + line

            # create slide on empty line occurrence
            if newp != '':
                self._draw_text_page(
                    f'{self.work_dir}/{page_number}.png', newp)
                newp = ''
                page_number += 1
        # get user pwd
        main_dir = os.getcwd()
        # move required files to tmp folder
        shutil.copy(f'./{argv[1]}', self.work_dir)
        os.chdir(self.work_dir)
        self.convert_to_pdf()
        # get generated pdf file's name and move the final pdf to user's dir
        pdf_file = f'{argv[1][:-3]}.pdf'
        try:
            shutil.move(pdf_file, main_dir)
        except:
            os.remove(f'{main_dir}/{pdf_file}')
            shutil.move(pdf_file, main_dir)
        # cleanup tmp folder
        shutil.rmtree(self.work_dir)
