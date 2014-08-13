project(Pi)
find_package(KDE4 REQUIRED)
find_package(QT4 COMPONENTS QTCORE QTGUI)

include(KDE4Defaults)
include_directories(${KDE4_INCLUDES})

set(konq_pi_PART_SRCS pi.cpp )


kde4_add_plugin(konq_pi ${konq_pi_PART_SRCS})


install(TARGETS konq_pi  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############
install( FILES pi.rc pi.desktop  DESTINATION  ${DATA_INSTALL_DIR}/dolphinpart/kpartplugins )
