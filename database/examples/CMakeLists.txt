add_custom_target(examples)

function(add_example name)
    add_executable(${name})
    set_target_properties(${name} PROPERTIES C_STANDARD 11)
    target_sources(${name} PRIVATE ${name}.c)
    target_link_libraries(${name} PRIVATE btree_packed_db)
    add_dependencies(${name} btree_packed_db)
    add_dependencies(examples ${name})
endfunction()

add_example(pack)
add_example(unpack)