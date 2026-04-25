find_package(Qt6 REQUIRED COMPONENTS Protobuf Grpc)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(proto-ui STATIC)
set_target_properties(
    proto-ui
    PROPERTIES
        AUTOMOC ON
        POSITION_INDEPENDENT_CODE ON
)
qt_add_protobuf(proto-ui
    PROTO_FILES
    ${CMAKE_CURRENT_LIST_DIR}/m_ui_items.proto
    ${CMAKE_CURRENT_LIST_DIR}/messages.proto
)
target_link_libraries(proto-ui PUBLIC Qt6::Protobuf)

add_library(proto-ui-client STATIC)
set_target_properties(
    proto-ui-client
    PROPERTIES
        AUTOMOC ON
        POSITION_INDEPENDENT_CODE ON
)
qt_add_grpc(proto-ui-client CLIENT
    PROTO_FILES
        ${CMAKE_CURRENT_LIST_DIR}/messages.proto
)
target_link_libraries(proto-ui-client PUBLIC Qt6::Protobuf Qt6::Grpc proto-ui)

# add_library(proto-ui-server STATIC)
# set_target_properties(
#     proto-ui-server
#     PROPERTIES
#         AUTOMOC ON
#         POSITION_INDEPENDENT_CODE ON
# )
# qt_add_grpc(proto-ui-server SERVER
#     PROTO_FILES
#         ${CMAKE_CURRENT_LIST_DIR}/messages.proto
# )
# target_link_libraries(proto-ui-server PUBLIC Qt6::Protobuf Qt6::Grpc proto-ui)