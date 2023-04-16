# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2022, Benoit BLANCHON
# MIT License

cmake_minimum_required(VERSION 3.15)

if(ESP_PLATFORM)
   # Build ArduinoJson as an ESP-IDF component
   idf_component_register(INCLUDE_DIRS src)
   return()
endif()

project(AeonLabsLibraries VERSION 1.0.0)


add_subdirectory(src)


