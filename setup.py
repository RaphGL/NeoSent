import os
from setuptools import find_namespace_packages, setup

# Utility function to read the README file.
# Used for the long_description.  It's nice, because now 1) we have a top level
# README file and 2) it's easier to type in the README file than to put a raw
# string in below ...


def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()


setup(
    name='NeoSent',
    version='0.8',
    url='https://github.com/RaphGL/NeoSent',
    author='RaphGL',
    author_email='raphfl_dev@gmail.com',
    description='Suckful Sent',
    long_description=read('README.md'),
    packages=find_namespace_packages(),
    install_requires=['pillow', 'click'],
    classifiers=[
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
    ],
    entry_points={
        'console_scripts': [
            'neosent = neosent.main:cli',
        ],
    },
    include_package_data=True,
    package_data={
        '': ['data/*']
        },
)
