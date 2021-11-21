import os
from setuptools import setup

# Utility function to read the README file.
# Used for the long_description.  It's nice, because now 1) we have a top level
# README file and 2) it's easier to type in the README file than to put a raw
# string in below ...
def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

setup(
    name = "NeoSent",
    version = "0.1",
    author = "RaphGL",
    author_email = "raphfl.dev@gmail.com",
    description = ("Suckful Neosent"),
    license = "GPLv3",
    keywords = "sent suckless presentation sheet",
    url = "https://github.com/RaphGL/NeoSent",
    packages=['Pillow', 'tests'],
    long_description=read('README'),
    classifiers=[
        "Topic :: Utilities",
    ],
)