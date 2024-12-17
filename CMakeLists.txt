cmake_minimum_required(VERSION 3.16)
project(mvcMouseTrackCollect VERSION 1.0 LANGUAGES CXX)

# Указываем путь к Qt
set(CMAKE_PREFIX_PATH "C:/Qt/6.7.2/mingw_64/lib/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматическая обработка Qt MOC, UIC, и RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Находим требуемые пакеты Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets SerialPort)

# Исходные файлы из .pro файла
set(PROJECT_SOURCES
    main.cpp
    DataManager.cpp
    InputEmulator/ArduinoCommand.cpp
    Main/MainController.cpp
    Main/MainWindow.cpp
    Mouse/MouseController.cpp
    Mouse/MouseWidget.cpp
    Result/GenerateNewMouseTrack.cpp
    Selection/SettingsController.cpp
    Selection/SettingsModel.cpp
    Selection/SettingsWidget.cpp
    TransformaController.cpp
)

# Заголовочные файлы
set(PROJECT_HEADERS
    DataManager.h
    InputEmulator/ArduinoCommand.h
    Main/MainController.h
    Main/MainWindow.h
    Mouse/MouseController.h
    Mouse/MouseWidget.h
    Result/GenerateNewMouseTrack.h
    Selection/SettingsController.h
    Selection/SettingsModel.h
    Selection/SettingsWidget.h
    TransformaController.h
)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

# Подключаем директории с заголовочными файлами
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/InputEmulator
    ${CMAKE_CURRENT_SOURCE_DIR}/Main
    ${CMAKE_CURRENT_SOURCE_DIR}/Mouse
    ${CMAKE_CURRENT_SOURCE_DIR}/Result
    ${CMAKE_CURRENT_SOURCE_DIR}/Selection
)

# Линкуем библиотеки Qt
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::SerialPort
)

# Настройки для Windows
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
    
    # Копируем необходимые DLL Qt
    get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${_qt_bin_dir}/windeployqt.exe"
        --verbose 0
        --no-compiler-runtime
        "$<TARGET_FILE:${PROJECT_NAME}>"
    )
endif()

# Отключаем устаревшие API Qt (как в .pro файле)
target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_DISABLE_DEPRECATED_BEFORE=0x060000
) 