cmake_minimum_required(VERSION 3.16)

project(FocusIntegral VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Находим Qt6 (или Qt5 если Qt6 не найден)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Путь к исходникам
set(SOURCES
    main.cpp
    mainwindow.cpp
)

set(HEADERS
    mainwindow.h
)

set(FORMS
    mainwindow.ui
)

# Создаём исполняемый файл
add_executable(FocusIntegral ${SOURCES} ${HEADERS} ${FORMS})

# Линкуем с Qt
target_link_libraries(FocusIntegral Qt6::Core Qt6::Widgets)

# Копируем файл с картинкой фоном в папку сборки
if(EXISTS ${CMAKE_SOURCE_DIR}/fon.jpg)
    file(COPY ${CMAKE_SOURCE_DIR}/fon.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

# Настройки для Windows
if(WIN32)
    set_target_properties(FocusIntegral PROPERTIES WIN32_EXECUTABLE TRUE)
endif()