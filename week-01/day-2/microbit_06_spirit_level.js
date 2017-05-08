let ypix = 0
let xpix = 0

basic.forever(() => {
    xpix = input.rotation(Rotation.Roll) / -5
    if (xpix > 2) {
        xpix = 2
    }
    if (xpix <= -2) {
        xpix = -2
    }
    ypix = input.rotation(Rotation.Pitch) / -5
    if (ypix > 2) {
        ypix = 2
    }
    if (ypix <= -2) {
        ypix = -2
    }
    basic.clearScreen()
    led.plot(xpix + 2, ypix + 2)
})