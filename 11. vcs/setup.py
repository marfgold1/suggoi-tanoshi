from setuptools import find_packages, setup

setup(
    name='plz',
    version='1.0.0',
    author='Amar Fadil',
    author_email='13520103@std.stei.itb.ac.id',
    description=''.join([
        'A basic version control system.',
    ]),
    packages=find_packages(),
    include_package_data=True,
    zip_safe=False,
    install_requires=[
        
    ],
    python_requires='>=3.8',
)
