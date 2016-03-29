#!/usr/bin/python

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

def replace_string(filepath, src_string, dst_string):
    """ From file's content replace specified string
    Arg:
        filepath: Specify a file contains the path
        src_string: old string
        dst_string: new string
    """
    if src_string is None or dst_string is None:
        raise TypeError

    content = ""
    f1 = open(filepath, "rb")
    for line in f1:
        strline = line.decode('utf8')
        if src_string in strline:
            content += strline.replace(src_string, dst_string)
        else:
            content += strline
    f1.close()
    f2 = open(filepath, "wb")
    f2.write(content.encode('utf8'))
    f2.close()
# end of replace_string

def project_rename(dir, name, v):

    dst_project_dir = dir
    dst_project_name = name
    src_project_name = v['src_project_name']
    if dst_project_name == src_project_name:
        return

    files = v['files']
    for f in files:
        src = f.replace("PROJECT_NAME", src_project_name)
        dst = f.replace("PROJECT_NAME", dst_project_name)
        src_file_path = os.path.join(dst_project_dir, src)
        dst_file_path = os.path.join(dst_project_dir, dst)
        if os.path.exists(src_file_path):
            if dst_project_name.lower() == src_project_name.lower():
                temp_file_path = "%s-temp" % src_file_path
                os.rename(src_file_path, temp_file_path)
                os.rename(temp_file_path, dst_file_path)
            else:
                if os.path.exists(dst_file_path):
                    os.remove(dst_file_path)
                os.rename(src_file_path, dst_file_path)

def project_replace_project_name(dir, name, v):

    dst_project_dir = dir
    dst_project_name = name
    src_project_name = v['src_project_name']
    if dst_project_name == src_project_name:
        return

    files = v['files']
    for f in files:
        dst = f.replace("PROJECT_NAME", dst_project_name)
        if os.path.exists(os.path.join(dst_project_dir, dst)):
            replace_string(
                os.path.join(dst_project_dir, dst), src_project_name, dst_project_name)

def project_replace_package_name(dir, name, v):

    dst_project_dir = dir
    dst_project_name = name
    src_package_name = v['src_package_name']
    dst_package_name = 'org.cocos2dx.' + name
    if dst_package_name == src_package_name:
        return

    files = v['files']
    for f in files:
        dst = f.replace("PROJECT_NAME", dst_project_name)
        if os.path.exists(os.path.join(dst_project_dir, dst)):
            replace_string(
                os.path.join(dst_project_dir, dst), src_package_name, dst_package_name)

def project_replace_mac_bundleid(dir, name, v):

    dst_project_dir = dir
    dst_project_name = name
    src_bundleid = v['src_bundle_id']
    dst_bundleid = 'org.cocos2dx.' + name
    if src_bundleid == dst_bundleid:
        return

    files = v['files']
    for f in files:
        dst = f.replace("PROJECT_NAME", dst_project_name)
        if os.path.exists(os.path.join(dst_project_dir, dst)):
            replace_string(
                os.path.join(dst_project_dir, dst), src_bundleid, dst_bundleid)

def project_replace_ios_bundleid(dir, name, v):

    dst_project_dir = dir
    dst_project_name = name
    src_bundleid = v['src_bundle_id']
    dst_bundleid = 'org.cocos2dx.' + name
    if src_bundleid == dst_bundleid:
        return

    files = v['files']
    for f in files:
        dst = f.replace("PROJECT_NAME", dst_project_name)
        if os.path.exists(os.path.join(dst_project_dir, dst)):
            replace_string(
                os.path.join(dst_project_dir, dst), src_bundleid, dst_bundleid)

def append_x_engine(src_dir, dst_dir, v):

    src = os.path.join(src_dir, v['from'])
    dst = os.path.join(dst_dir, v['to'])

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

    f = open('cocos-project-template.json')
    data = json.load(f)
    f.close()
    do_default = data['do_default']
    #cocos_path = os.path.join(new_path, 'cocos2d')
    append_x_engine('', new_path, do_default['append_x_engine'])
    project_rename(new_path, name, do_default['project_rename'])
    project_replace_project_name(new_path, name, do_default['project_replace_project_name'])
    project_replace_package_name(new_path, name, do_default['project_replace_package_name'])
    project_replace_mac_bundleid(new_path, name, do_default['project_replace_mac_bundleid'])
    project_replace_ios_bundleid(new_path, name, do_default['project_replace_ios_bundleid'])
