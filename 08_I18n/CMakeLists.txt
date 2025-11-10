cmake_minimum_required(VERSION 3.10)
project(guess C)

find_package(Intl REQUIRED)
find_program(XGETTEXT_EXECUTABLE xgettext)
find_program(MSGMERGE_EXECUTABLE msgmerge)
find_program(MSGFMT_EXECUTABLE msgfmt)

set(LANGUAGES ru)
set(PO_DIR ${CMAKE_SOURCE_DIR}/po)
set(POT_FILE ${PO_DIR}/guess.pot)

add_executable(guess src/guess.c)

target_link_libraries(guess PRIVATE ${Intl_LIBRARIES})
target_include_directories(guess PRIVATE ${Intl_INCLUDE_DIRS})


file(MAKE_DIRECTORY ${PO_DIR})

add_custom_command(
    OUTPUT ${POT_FILE}
    COMMAND ${XGETTEXT_EXECUTABLE} --keyword=_ --from-code=UTF-8 -o ${POT_FILE} ${CMAKE_SOURCE_DIR}/src/guess.c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating POT template"
)

add_custom_target(pot ALL DEPENDS ${POT_FILE})

foreach(LANG ${LANGUAGES})
    set(PO_FILE ${PO_DIR}/${LANG}.po)
    set(MO_DIR ${CMAKE_BINARY_DIR}/locale/${LANG}/LC_MESSAGES)
    set(MO_FILE ${MO_DIR}/guess.mo)
    
    if(NOT EXISTS ${PO_FILE})
        add_custom_command(
            OUTPUT ${PO_FILE}
            COMMAND ${CMAKE_COMMAND} -E copy ${POT_FILE} ${PO_FILE}
            COMMAND sed -i.bak "s/charset=CHARSET/charset=UTF-8/" ${PO_FILE} && rm -f ${PO_FILE}.bak
            COMMAND sed -i.bak "s/Language: /Language: ${LANG}/" ${PO_FILE} && rm -f ${PO_FILE}.bak
            DEPENDS ${POT_FILE}
            COMMENT "Creating ${LANG}.po"
        )
    else()
        add_custom_command(
            OUTPUT ${PO_FILE}.updated
            COMMAND ${MSGMERGE_EXECUTABLE} --update --backup=none ${PO_FILE} ${POT_FILE}
            DEPENDS ${POT_FILE}
            COMMENT "Updating ${LANG}.po"
        )
    endif()
    
    file(MAKE_DIRECTORY ${MO_DIR})
    
    add_custom_command(
        OUTPUT ${MO_FILE}
        COMMAND ${MSGFMT_EXECUTABLE} -o ${MO_FILE} ${PO_FILE}
        DEPENDS ${PO_FILE}
        COMMENT "Compiling ${LANG}.po"
    )
    
    add_custom_target(mo_${LANG} ALL DEPENDS ${MO_FILE})
    add_dependencies(guess mo_${LANG})
endforeach()

install(TARGETS guess DESTINATION bin)
install(DIRECTORY ${CMAKE_BINARY_DIR}/locale DESTINATION share)
