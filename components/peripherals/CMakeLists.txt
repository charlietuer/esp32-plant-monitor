idf_component_register(
    SRCS 
        "src/pump.cpp"
        "src/button.cpp"
        "src/moisture_sensor.cpp"
        "src/led.cpp"
        "src/display.cpp"
        "src/setting_dial.cpp"
    INCLUDE_DIRS "inc"
    REQUIRES driver
)