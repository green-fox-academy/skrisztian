basic.forever(() => {
    if (input.buttonIsPressed(Button.B)) {

    } else {
        basic.clearScreen()
    }
})
input.onGesture(Gesture.Shake, () => {
    basic.showLeds(`
        . . # . .
        . . # . .
        . . # . .
        . . . . .
        . . # . .
        `)
    basic.clearScreen()
})
