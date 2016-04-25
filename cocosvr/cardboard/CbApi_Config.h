#ifndef _CbApi_Config_h
#define _CbApi_Config_h

#if defined( _MSC_VER ) || defined( __ICL )

#if defined( CBAPI_ENABLE_EXPORT )
    #define CBAPI_EXPORT  __declspec(dllexport)
#else
    #define CBAPI_EXPORT
#endif

#define CBAPI_DEPRECATED __declspec(deprecated)

#else

#if defined( CBAPI_ENABLE_EXPORT )
    #define CBAPI_EXPORT __attribute__((__visibility__("default")))
#else
    #define CBAPI_EXPORT 
#endif

#define CBAPI_DEPRECATED __attribute__ ((deprecated))

#endif

#endif
