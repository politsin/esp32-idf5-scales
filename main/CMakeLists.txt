idf_component_register(
  SRCS
    "main.cpp"
    "task/hx711Task.cpp"
    "task/uartTask.cpp"
    "task/blinkTask.cpp"
    "task/buttonTask.cpp"
    "task/screenTask.cpp"
    "task/encoderTask.cpp"
    # screen
    "screen/fontx.cpp"
    "screen/st7789.cpp"
  INCLUDE_DIRS 
    "."
    "util" 
    "task"
    "screen"
)
