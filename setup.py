import os
from setuptools import find_packages, setup

# Utility function to read the README file.
# Used for the long_description.  It's nice, because now 1) we have a top level
# README file and 2) it's easier to type in the README file than to put a raw
# string in below ...


def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()


setup(
    name="NeoSent",
    version="0.1",
    author="RaphGL",
    author_email="raphfl.dev@gmail.com",
    description=("Suckful Neosent"),
    license="GPLv3",
    entry_points={
        "console_scripts": [
            "neosent = src.main:main"
        ]
    },
    keywords="sent suckless presentation sheet",
    url="https://github.com/RaphGL/NeoSent",
    long_description=read('README.md'),
    classifiers=[
        "Topic :: Utilities",
    ],

    packages=find_packages(where="neosent"),
    data_files=["neosent/OpenSansEmoji.ttf"],
    include_package_data=True,
    install_requires=["Pillow"],
)
