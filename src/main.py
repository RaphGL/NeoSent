from sheetwriter import *
from sys import argv, exit


def main():
    sheet = SheetWriter((1280, 720), (0, 0, 0), './OpenSansEmoji.ttf')
    sheet.create_sheet()

if __name__ == '__main__':
    main()
