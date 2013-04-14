#-------------------------------------------------------------------
# This file is part of the CMake build system for SkyX
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

#############################################################
# install dir setup
#############################################################
if (WIN32)
  set(SKYX_CFG_INSTALL_PATH "bin")
elseif (APPLE)
  set(SKYX_CFG_INSTALL_PATH "bin")
elseif (UNIX)
  set(SKYX_CFG_INSTALL_PATH "share/SKYX")
endif ()

#############################################################
# install config files
#############################################################
if (SKYX_INSTALL_SAMPLES)
  install(FILES 
          Bin/resources_d.cfg
          Bin/Plugins_d.cfg
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_DEBUG_PATH}"
          CONFIGURATIONS Debug)
  install(FILES 
          Bin/resources.cfg
          Bin/Plugins.cfg
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_RELEASE_PATH}"
          CONFIGURATIONS Release None "")
  install(FILES 
          Bin/resources.cfg
          Bin/Plugins.cfg
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_RELWDBG_PATH}"
          CONFIGURATIONS RelWithDebInfo)
  install(FILES 
          Bin/resources.cfg
          Bin/Plugins.cfg
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_MINSIZE_PATH}"
          CONFIGURATIONS MinSizeRel)
  
###############################################################
# Install media
###############################################################

  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin/Media
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_DEBUG_PATH}"
          CONFIGURATIONS Debug
          PATTERN ".svn" EXCLUDE)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin/Media
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_RELEASE_PATH}"
          CONFIGURATIONS Release None ""
          PATTERN ".svn" EXCLUDE)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin/Media
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_RELWDBG_PATH}"
          CONFIGURATIONS RelWithDebInfo
          PATTERN ".svn" EXCLUDE)
  install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bin/Media
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_MINSIZE_PATH}"
          CONFIGURATIONS MinSizeRel
          PATTERN ".svn" EXCLUDE)

###############################################################
# Install dependency dlls
###############################################################
  install(FILES 
          ${OGRE_BINARY_DBG}
          ${OGRE_RenderSystem_Direct3D9_DBG}
          ${OGRE_RenderSystem_GL_DBG}
          ${OGRE_Plugin_OctreeSceneManager_DBG}
          ${OIS_BINARY_DBG}
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_DEBUG_PATH}"
          CONFIGURATIONS Debug)
  install(FILES 
          ${OGRE_BINARY_REL}
          ${OGRE_RenderSystem_Direct3D9_REL}
          ${OGRE_RenderSystem_GL_REL}
          ${OGRE_Plugin_OctreeSceneManager_REL}
          ${OIS_BINARY_REL}
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_RELEASE_PATH}"
          CONFIGURATIONS Release None "")
  install(FILES 
          ${OGRE_BINARY_REL}
          ${OGRE_RenderSystem_Direct3D9_REL}
          ${OGRE_RenderSystem_GL_REL}
          ${OGRE_Plugin_OctreeSceneManager_REL}
          ${OIS_BINARY_REL}
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_RELWDBG_PATH}"
          CONFIGURATIONS RelWithDebInfo)
  install(FILES 
          ${OGRE_BINARY_REL}
          ${OGRE_RenderSystem_Direct3D9_REL}
          ${OGRE_RenderSystem_GL_REL}
          ${OGRE_Plugin_OctreeSceneManager_REL}
          ${OIS_BINARY_REL}
          DESTINATION "${SKYX_CFG_INSTALL_PATH}${SKYX_MINSIZE_PATH}"
          CONFIGURATIONS MinSizeRel)

endif ()
