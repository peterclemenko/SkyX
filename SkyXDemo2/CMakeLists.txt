#-------------------------------------------------------------------
# This file is part of the CMake build system for SKYX
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

# Configure SkyX SkyXDemo2 build
set(SOURCE_FILES Source/main.cpp)

# Add static flag if needed
if(SKYX_STATIC)
  add_definitions(-DSKYX_STATIC_LIB)
  set(STATIC_DEPS_INCLUDE_DIRS ${OGRE_Plugin_CgProgramManager_INCLUDE_DIR}
                               ${OGRE_Plugin_OctreeSceneManager_INCLUDE_DIR}
                               ${OGRE_Plugin_ParticleFX_INCLUDE_DIR}
                               ${OGRE_Plugin_BSPSceneManager_INCLUDE_DIR}
                               ${OGRE_RenderSystem_GL_INCLUDE_DIR}
                               ${OGRE_RenderSystem_Direct3D9_INCLUDE_DIR})
  set(STATIC_DEPS_LIBRARIES debug ${Cg_LIBRARY_DBG} optimized ${Cg_LIBRARY_REL}
                            ${DirectX_LIBRARY}
                            ${DirectX_D3DX9_LIBRARY}
                            ${DirectX_DXERR_LIBRARY}
                            ${DirectX_DXGUID_LIBRARY}
                            ${OPENGL_gl_LIBRARY}
                            ${OPENGL_glu_LIBRARY}
                            debug ${OGRE_Plugin_CgProgramManager_LIBRARY_DBG} optimized ${OGRE_Plugin_CgProgramManager_LIBRARY_REL}
                            debug ${OGRE_Plugin_OctreeSceneManager_LIBRARY_DBG} optimized ${OGRE_Plugin_OctreeSceneManager_LIBRARY_REL}
                            debug ${OGRE_Plugin_ParticleFX_LIBRARY_DBG} optimized ${OGRE_Plugin_ParticleFX_LIBRARY_REL}
                            debug ${OGRE_Plugin_BSPSceneManager_LIBRARY_DBG} optimized ${OGRE_Plugin_BSPSceneManager_LIBRARY_REL}
                            debug ${OGRE_RenderSystem_GL_LIBRARY_DBG} optimized ${OGRE_RenderSystem_GL_LIBRARY_REL}
                            debug ${OGRE_RenderSystem_Direct3D9_LIBRARY_DBG} optimized ${OGRE_RenderSystem_Direct3D9_LIBRARY_REL})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../SkyX/Include ${OIS_INCLUDE_DIR} ${STATIC_DEPS_INCLUDE_DIRS})
add_executable(SkyXDemo2 ${SOURCE_FILES})
target_link_libraries(SkyXDemo2 ${SKYX_LIBRARIES} ${OIS_LIBRARY} ${STATIC_DEPS_LIBRARIES})

# Install sample sources
if (SKYX_INSTALL_SAMPLES)
	skyx_install_target(SkyXDemo2 "")
	if (SKYX_INSTALL_PDB)
		# install debug pdb files
		install(FILES ${SKYX_BINARY_DIR}/bin${SKYX_DEBUG_PATH}/SkyXDemo2.pdb
			DESTINATION bin${SKYX_DEBUG_PATH} CONFIGURATIONS Debug
		)
		
		install(FILES ${SKYX_BINARY_DIR}/bin${SKYX_RELWDBG_PATH}/SkyXDemo2.pdb
			DESTINATION bin${SKYX_RELWDBG_PATH} CONFIGURATIONS RelWithDebInfo
		)
	endif ()
endif ()

