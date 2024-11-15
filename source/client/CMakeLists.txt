project(client_prj)

include_directories(${CMAKE_BINARY_DIR})
include_directories(${COMMON_PATH})

set(EXE_NAME client)
set(SRC_PATH ${CMAKE_SOURCE_DIR}/client)

set(CLIENT_FILES
    main.cpp	main.h
)

set(LOGGER_FILES
    logger.cpp  logger.h
)

set(NETWORK_FILES
    network_handler.cpp	network_handler.h
)

set(PARSER_FILES
    parser.cpp	parser.h
)

set(CONTROLLER_FILES
    controller.cpp	controller.h
    i_inputdata.h
)

set(GAMELOGIC_FILES
    game_logic.cpp	game_logic.h game_logic.ipp
    client_settings.h
    menus.h menus.cpp
)

set(DATABASE_FILES
    db_handler.cpp	db_handler.h
    texts.h
)

set(VIEW_FILES
    view.cpp	view.h
)


add_executable(${EXE_NAME} 
    ${CLIENT_FILES} ${COMMON_FILES} 

    #contoller
    ${CONTROLLER_FILES} 
    ${NETWORK_FILES} 
    ${PARSER_FILES}
    ${GAMELOGIC_FILES} 
    
    #model
    ${LOGGER_FILES}
    ${DATABASE_FILES} 

    #view
    ${VIEW_FILES}
)

add_dependencies(${EXE_NAME} generate_protobuf_files)

if(MSVC)
    set_property(DIRECTORY ${SRC_PATH} PROPERTY VS_STARTUP_PROJECT ${EXE_NAME})	
    source_group("Client" FILES ${CLIENT_FILES})  
    source_group("Common" FILES ${COMMON_FILES})   
    source_group("Controller\\Controller" FILES ${CONTROLLER_FILES})
    source_group("Controller\\Network" FILES ${NETWORK_FILES})
    source_group("Controller\\Parser" FILES ${PARSER_FILES})
    source_group("Controller\\GameLogic" FILES ${GAMELOGIC_FILES})
 
    source_group("Model\\Database" FILES ${DATABASE_FILES})
    source_group("Model\\Logger" FILES ${LOGGER_FILES})


    source_group("View" FILES ${VIEW_FILES}) 
endif ()

set_common_target_properties(${EXE_NAME})