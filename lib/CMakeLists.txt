add_subdirectory( IlmCtl )
add_subdirectory( IlmCtlMath )
add_subdirectory( IlmCtlSimd )
add_subdirectory( CtlCodeEmitter )

#if ( OpenEXR_FOUND )
#add_subdirectory( IlmImfCtl )
#endif()

#add_subdirectory( dpx )

#add_custom_target( CTL DEPENDS IlmCtl IlmCtlMath IlmCtlSimd )
add_custom_target( CTL DEPENDS IlmCtl IlmCtlMath )

### TODO: Add install, pkgconfig stuff
