if not exist .\libs md .\libs

if "%VR_PLATFORM%"=="GEAR_VR" (
    copy /y %OVRSDKMOBILEROOT%VrApi\Libs\Android\VrApi.jar .\libs 
    copy /y %OVRSDKMOBILEROOT%VrAppSupport\SystemUtils\Libs\Android\SystemUtils.jar .\libs
)
if "%VR_PLATFORM%"=="DEEPOON_VR" (
    copy /y %DEEPOONSDKROOT%lib\deepoon_sdk.jar .\libs
)