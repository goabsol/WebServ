#!/usr/bin/python
print("Content-Type=text/html\r")
print("\r")
print ("<html><head><title>Situation snapshot</title></head><body><pre>")

import sys
sys.stderr = sys.stdout

import os
from cgi import escape
print ("<strong>Python %s</strong>") % sys.version
keys = os.environ.keys(  )
keys.sort(  )
for k in keys:
    print ("%s\t%s") % (escape(k), escape(os.environ[k]))
print ("</pre></body></html>")
