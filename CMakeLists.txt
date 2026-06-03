cmake_minimum_required(VERSION 3.16)

# ESP-IDF component registration
idf_component_register(
    SRCS
        "src/main.c"
        "src/gesture_recognition.c"
        "src/dfplayer.c"
    INCLUDE_DIRS
        "include"
    REQUIRES
        driver
        freertos
        esp_common
)
