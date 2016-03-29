import sys
import json
import os, os.path
import shutil
from optparse import OptionParser

def os_is_win32():
    return sys.platform == 'win32'

def add_path_prefix(path_str):
    if not os_is_win32():
        return path_str

    if path_str.startswith("\\\\?\\"):
        return path_str

    ret = "\\\\?\\" + os.path.abspath(path_str)
    ret = ret.replace("/", "\\")
    return ret

def copy_engine(dst):

    src = './cocos2d'

    cocosx_files_json = os.path.join(src, 'templates', 'cocos2dx_files.json')

    f = open(cocosx_files_json)
    data = json.load(f)
    f.close()

    fileList = data['common']

    # begin copy engine

    for index in range(len(fileList)):
        srcfile = os.path.join(src, fileList[index])
        dstfile = os.path.join(dst, fileList[index])

        srcfile = add_path_prefix(srcfile)
        dstfile = add_path_prefix(dstfile)

        if not os.path.exists(os.path.dirname(dstfile)):
            os.makedirs(add_path_prefix(os.path.dirname(dstfile)))

        # copy file or folder
        if os.path.exists(srcfile):
            if os.path.isdir(srcfile):
                if os.path.exists(dstfile):
                    shutil.rmtree(dstfile)
                shutil.copytree(srcfile, dstfile)
            else:
                if os.path.exists(dstfile):
                    os.remove(dstfile)
                shutil.copy2(srcfile, dstfile)

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
    shutil.copytree("./templates", new_path)
    vr_path = os.path.join(new_path, 'cocosvr')
    shutil.copytree("./cocosvr", vr_path)
    cocos_path = os.path.join(new_path, 'cocos2d')
    copy_engine(cocos_path)
