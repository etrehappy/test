project(server_prj)

include_directories(${CMAKE_BINARY_DIR})
include_directories(${COMMON_PATH})

set(EXE_NAME server)
set(SRC_PATH ${CMAKE_SOURCE_DIR}/server)

set(SERVER_FILES
    main.h  main.cpp
    session.h session.cpp    
)


set(DATABASE_FILES
    database.h database.cpp
    db_handler.h db_handler.cpp db_handler.ipp
)

set(EVENT_FILES
    event_dispatcher.h event_dispatcher.cpp
    event_handler.h event_handler.ipp
    event_queue.h event_queue.cpp
    event.h event.cpp
)
set(GAME_FILES
    game_handler.h game_handler.cpp game_handler.ipp    
)

set(NETWORK_FILES
    network_handler.h network_handler.cpp network_handler.ipp
)

set(LOGGING_FILES
    logger.h logger.cpp
)

set(USER_FILES
    user_handler.h user_handler.cpp user_handler.ipp
 )


#find_package(SQLiteCpp REQUIRED)

add_executable(${EXE_NAME} ${SERVER_FILES} ${COMMON_FILES} ${DATABASE_FILES} 
${EVENT_FILES} ${NETWORK_FILES} ${LOGGING_FILES} ${USER_FILES} ${GAME_FILES}
)

add_dependencies(${EXE_NAME} generate_protobuf_files)

#target_link_libraries(${EXE_NAME} PRIVATE SQLiteCpp)

if(MSVC)
    set_property(DIRECTORY ${SRC_PATH} PROPERTY VS_STARTUP_PROJECT ${EXE_NAME})	
    source_group("Server" FILES ${SERVER_FILES})  
    source_group("Common" FILES ${COMMON_FILES}) 
    source_group("Database" FILES ${DATABASE_FILES})
    source_group("Event" FILES ${EVENT_FILES})
    source_group("Network" FILES ${NETWORK_FILES})
    source_group("Logging" FILES ${LOGGING_FILES})
    source_group("User" FILES ${USER_FILES})
    source_group("Game" FILES ${GAME_FILES}) 
endif ()

set_common_target_properties(${EXE_NAME})