from distutils.core import setup
setup(name='BTELspendfrom',
      version='1.0',
      description='Command-line utility for bethel "coin control"',
      author='Gavin Andresen',
      author_email='gavin@bethelfoundation.org',
      requires=['jsonrpc'],
      scripts=['spendfrom.py'],
      )
