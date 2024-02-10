idf_component_register(
  SRCS
    "main.cpp"
    "task/hx711Task.cpp"
    "task/uartTask.cpp"
    "task/blinkTask.cpp"
    "task/buttonTask.cpp"
    "task/encoderTask.cpp"
    # "screen/screenTask.cpp"
    # "task/stepperTask.cpp"
    # "task/DendoStepper.cpp"
    # "screen/fontx.cpp"
    # "screen/st7789.cpp"
    # INCLUDE_DIRS "util" "task" "screen"
  INCLUDE_DIRS 
    "."
    "util" 
    "task"
)
