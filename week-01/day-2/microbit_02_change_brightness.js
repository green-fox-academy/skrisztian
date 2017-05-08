basic.forever(() => {
    led.setBrightness(123)
    basic.showArrow(ArrowNames.North)
    basic.pause(500)
    basic.clearScreen()
    basic.pause(500)
})
