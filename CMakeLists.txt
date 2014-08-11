
########### next target ###############

set(konq_pi_PART_SRCS pi.cpp )


kde4_add_plugin(konq_pi ${konq_pi_PART_SRCS})


target_link_libraries(konq_pi ${KDE4_KPARTS_LIBS} ${KDE4_KDESU_LIBS})

install(TARGETS konq_pi  DESTINATION ${PLUGIN_INSTALL_DIR} )


########### install files ###############
install( FILES pi.rc pi.desktop  DESTINATION  ${DATA_INSTALL_DIR}/dolphinpart/kpartplugins )
