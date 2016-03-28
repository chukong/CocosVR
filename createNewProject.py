
import sys
import os, os.path
import shutil
from optparse import OptionParser

# -------------- main --------------
if __name__ == '__main__':

    parser = OptionParser()
    parser.add_option("-n", "--name", dest="proj_name", help='the name of project')
    parser.add_option("-p", "--path", dest="proj_path", help='the path of project')
    (opts, args) = parser.parse_args()

    if opts.proj_name is None:
        name = 'HelloVR'
    else:
        name = opts.proj_name

    if opts.proj_path is None:
        path = './'
    else:
        path = opts.proj_path

    new_path = os.path.join(path, name)
    shutil.copytree("./cocosvr", new_path)
    shutil.copytree("./templates", new_path)
