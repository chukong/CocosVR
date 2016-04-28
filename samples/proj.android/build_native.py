#!/usr/bin/python
# build_native.py
# Build native codes
# 
# Please use cocos console instead


import sys
import os, os.path
import shutil
from optparse import OptionParser

def build(build_mode):


    current_dir = os.path.dirname(os.path.realpath(__file__))
    cocos_root = os.path.join(current_dir, "../cocos2d")

    app_android_root = os.path.join(current_dir, "../")
		
    if build_mode is None:
    	  build_mode = 'debug'
    elif build_mode != 'release':
        build_mode = 'debug'
    
    command = 'cocos compile -p android -s %s -m %s' % (app_android_root, build_mode) 
    if os.system(command) != 0:
        raise Exception("Build dynamic library for project [ " + app_android_root + " ] fails!")

def vrPlatform(vr_platform):
    
    if os.path.exists("./libs"):
        shutil.rmtree("./libs")
    os.mkdir("./libs")

    if os.path.exists("./src/org/cocos2dx/cpp/CbAPIWrapper.java"):
        os.remove("./src/org/cocos2dx/cpp/CbAPIWrapper.java")

    if vr_platform is None or vr_platform == 'gearvr':
        os.environ["VR_PLATFORM"] = 'GEAR_VR'
        shutil.copyfile('AndroidManifest-gearvr.xml', 'AndroidManifest.xml')
        shutil.copy("../../external/gearvr/prebuild/VrApi.jar", "./libs")
        #shutil.copy("../../external/gearvr/prebuild/SystemUtils.jar", "./libs")
    elif vr_platform == 'cardboard':
        os.environ["VR_PLATFORM"] = 'CARDBOARD_VR'
        shutil.copyfile('../../cocosvr/cardboard/CbAPIWrapper.java', './src/org/cocos2dx/cpp/CbAPIWrapper.java')
        shutil.copyfile('AndroidManifest-cardboard.xml', 'AndroidManifest.xml')
        shutil.copy("../../external/cardboard/cardboard.jar", "./libs")
    elif vr_platform != 'gearvr':
        os.environ["VR_PLATFORM"] = 'DEEPOON_VR'
        shutil.copyfile('AndroidManifest-deepoon.xml', 'AndroidManifest.xml')
        shutil.copy("../../external/deepoon/prebuild/deepoon_sdk.jar", "./libs")
    #os.system('copyVRlibs.bat')

# -------------- main --------------
if __name__ == '__main__':

    parser = OptionParser()
    parser.add_option("-n", "--ndk", dest="ndk_build_param", help='it is not used', action="append")
    parser.add_option("-p", "--platform", dest="android_platform", 
    help='it is not used')
    parser.add_option("-b", "--build", dest="build_mode", 
    help='the build mode for java project,debug[default] or release.Get more information,please refer to http://developer.android.com/tools/building/building-cmdline.html')
    parser.add_option("-V", "--vrplatform", dest="vr_platform", help='define vr platform (gearvr/deepoon/cardboard)')
    (opts, args) = parser.parse_args()
    
    print "Please use cocos console instead.\n"
    
    vrPlatform(opts.vr_platform)
    build(opts.build_mode)
