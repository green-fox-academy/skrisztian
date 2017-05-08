let ball_dir_y = false
let ball_dir_x = false
let ball_y = 0
let ball_x = 0
let pad_y = 0
let pad_x = 0
let score = 0
let in_game = false
input.onButtonPressed(Button.B, () => {
    basic.clearScreen()
    basic.showNumber(score)
    basic.clearScreen()
})
input.onButtonPressed(Button.A, () => {
    in_game = true
})
basic.forever(() => {
    if (in_game) {
        led.unplot(pad_x, pad_y)
        led.unplot(ball_x, ball_y)
        if (input.rotation(Rotation.Pitch) > 10) {
            pad_y += 1
            if (pad_y > 4) {
                pad_y = 4
            }
        } else if (input.rotation(Rotation.Pitch) < -10) {
            pad_y += -1
            if (pad_y < 0) {
                pad_y = 0
            }
        }
        if (ball_dir_x) {
            ball_x += 1
        } else {
            ball_x += -1
        }
        if (ball_dir_y) {
            ball_y += 1
        } else {
            ball_y += -1
        }
        if (ball_x == pad_x + 1 && ball_y == pad_y) {
            ball_dir_x = !(ball_dir_x)
            score += 1
        }
        if (ball_x >= 4) {
            ball_dir_x = !(ball_dir_x)
        }
        if (ball_y <= 0) {
            ball_dir_y = !(ball_dir_y)
        }
        if (ball_y >= 4) {
            ball_dir_y = !(ball_dir_y)
        }
        if (ball_x <= 0) {
            in_game = false
        }
        led.plot(pad_x, pad_y)
        led.plot(ball_x, ball_y)
        basic.pause(200)
        if (ball_x == 4) {
            ball_dir_x = !(ball_dir_x)
        }
        serial.writeString("px:")
        serial.writeNumber(pad_x)
        serial.writeString(", py:")
        serial.writeNumber(pad_y)
        serial.writeString(", bx:")
        serial.writeNumber(ball_x)
        serial.writeString(", by:")
        serial.writeNumber(ball_y)
        serial.writeLine("")
    }
})
basic.clearScreen()
score = 0
pad_x = 0
pad_y = 2
ball_x = Math.random(4) + 1
ball_y = Math.random(5)
ball_dir_x = true
ball_dir_y = Math.randomBoolean()
in_game = false
led.plot(ball_x, ball_y)
led.plot(pad_x, pad_y)